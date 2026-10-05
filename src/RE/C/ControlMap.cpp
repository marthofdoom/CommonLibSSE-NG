#include "RE/C/ControlMap.h"

#include "RE/U/UserEventEnabled.h"
#include "SKSE/Logger.h"
#include "SKSE/Version.h"

namespace RE
{
	ControlMap* ControlMap::GetSingleton()
	{
		REL::Relocation<ControlMap**> singleton{ Offset::ControlMap::Singleton };
		return *singleton;
	}

	namespace
	{
		// mit-3.7: the verified builds. See ControlMap.h for the evidence.
		bool IsAE1170() noexcept { return REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_6_1170); }
		bool IsSE197() noexcept { return REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_5_97); }
		bool Is17104() noexcept { return REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104); }

		std::string UnverifiedMessage(std::string_view a_what)
		{
			return std::format(
				"ControlMap::{}: the ControlMap layout is not verified for game version {} "
				"(verified: 1.6.1170.0, 1.5.97.0, 1.7.104.0)."sv,
				a_what, REL::Module::get().version().string("."sv));
		}
	}

	bool ControlMap::IsRuntimeDataVerified() noexcept
	{
		return IsAE1170() || IsSE197() || Is17104();
	}

	ControlMap::RUNTIME_DATA& ControlMap::GetRuntimeData() noexcept
	{
		return const_cast<RUNTIME_DATA&>(std::as_const(*this).GetRuntimeData());
	}

	const ControlMap::RUNTIME_DATA& ControlMap::GetRuntimeData() const noexcept
	{
		std::ptrdiff_t offset = 0;
		if (IsAE1170()) {
			offset = 0xF0;
		} else if (IsSE197()) {
			offset = 0xE8;
		} else if (Is17104()) {
			offset = 0xF0;
		} else {
			stl::report_and_fail(UnverifiedMessage("GetRuntimeData"sv));
		}
		return *reinterpret_cast<const RUNTIME_DATA*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

	ControlMap::InputContext* ControlMap::GetInputContext(InputContextID a_context) const noexcept
	{
		std::size_t index = 0;
		std::size_t count = 0;
		if (IsAE1170()) {
			count = 18;
			// 16 is "Creations Menu" on 1.6.1170; the 1.5.97-numbered kFavor is 17.
			index = a_context == InputContextID::kFavor ? 17 : std::to_underlying(a_context);
		} else if (IsSE197()) {
			count = 17;
			index = std::to_underlying(a_context);
		} else if (Is17104()) {
			count = 18;
			// Same 18 contexts as 1.6.1170 (see ControlMap.h): kFavor is 17.
			index = a_context == InputContextID::kFavor ? 17 : std::to_underlying(a_context);
		} else {
			static std::atomic_flag reported = ATOMIC_FLAG_INIT;
			if (!reported.test_and_set()) {
				SKSE::log::critical("{} Every input-context lookup is REFUSED (no context, no key).", UnverifiedMessage("GetInputContext"sv));
			}
			return nullptr;
		}
		if (index >= count) {
			return nullptr;
		}
		return reinterpret_cast<InputContext* const*>(reinterpret_cast<std::uintptr_t>(this) + 0x60)[index];
	}

	std::int8_t ControlMap::AllowTextInput(bool a_allow)
	{
		auto& textEntryCount = GetRuntimeData().textEntryCount;
		if (a_allow) {
			if (textEntryCount != -1) {
				++textEntryCount;
			}
		} else {
			if (textEntryCount != 0) {
				--textEntryCount;
			}
		}

		return textEntryCount;
	}

	std::uint32_t ControlMap::GetMappedKey(std::string_view a_eventID, INPUT_DEVICE a_device, InputContextID a_context) const
	{
		assert(a_device < INPUT_DEVICE::kTotal);
		assert(a_context < InputContextID::kTotal);

		if (const auto context = GetInputContext(a_context)) {
			const auto&   mappings = context->deviceMappings[a_device];
			BSFixedString eventID(a_eventID);
			for (auto& mapping : mappings) {
				if (mapping.eventID == eventID) {
					return mapping.inputKey;
				}
			}
		}

		return kInvalid;
	}

	std::string_view ControlMap::GetUserEventName(std::uint32_t a_buttonID, INPUT_DEVICE a_device, InputContextID a_context) const
	{
		assert(a_device < INPUT_DEVICE::kTotal);
		assert(a_context < InputContextID::kTotal);

		if (const auto context = GetInputContext(a_context)) {
			const auto&      mappings = context->deviceMappings[a_device];
			UserEventMapping tmp{};
			tmp.inputKey = static_cast<std::uint16_t>(a_buttonID);
			auto range = std::equal_range(
				mappings.begin(),
				mappings.end(),
				tmp,
				[](auto&& a_lhs, auto&& a_rhs) {
					return a_lhs.inputKey < a_rhs.inputKey;
				});

			if (std::distance(range.first, range.second) == 1) {
				return range.first->eventID;
			}
		}

		return ""sv;
	}

	namespace
	{
		// mit-3.7 (upstream sync 2024-09): Push/PopInputContext take the RUNTIME's own
		// context index. Verified: 1.5.97 Push id 67243 at 0xC11AE0 rejects edx >= 0x11,
		// 1.6.1170 Push id 68543 at 0xCD5450 rejects edx >= 0x12; both push onto
		// contextPriorityStack (+0x100 / +0x108). Pop ids 67244 / 68544 (0xC11BC0 /
		// 0xCD5530) walk the same stack. The enum is numbered as on 1.5.97, so kFavor is
		// translated to 17 on 1.6.1170 and 1.7.104 exactly as GetInputContext does. Unverified builds
		// are refused (one critical log line, no call).
		std::optional<std::uint32_t> RuntimeContextId(ControlMap::InputContextID a_context, std::string_view a_what)
		{
			if (IsAE1170()) {
				return a_context == ControlMap::InputContextID::kFavor ? 17u : static_cast<std::uint32_t>(std::to_underlying(a_context));
			}
			if (IsSE197()) {
				return static_cast<std::uint32_t>(std::to_underlying(a_context));
			}
			if (Is17104()) {
				// 1.7.104 Push 0xCEF8A0 / Pop 0xCEF980 (ids 68543 / 68544 through the id table)
				// are instruction-identical to 1.6.1170's: the same cmp edx,0x12 bound, the stack
				// at +0x108 and its size at +0x118. Same 18 contexts, so kFavor is 17.
				return a_context == ControlMap::InputContextID::kFavor ? 17u : static_cast<std::uint32_t>(std::to_underlying(a_context));
			}
			static std::atomic_flag reported = ATOMIC_FLAG_INIT;
			if (!reported.test_and_set()) {
				SKSE::log::critical("{} Every Push/PopInputContext call is REFUSED.", UnverifiedMessage(a_what));
			}
			return std::nullopt;
		}
	}

	void ControlMap::PopInputContext(InputContextID a_context)
	{
		const auto id = RuntimeContextId(a_context, "PopInputContext"sv);
		if (!id) {
			return;
		}
		using func_t = void(ControlMap*, std::uint32_t);
		REL::Relocation<func_t> func{ RELOCATION_ID(67244, 68544) };
		return func(this, *id);
	}

	void ControlMap::PushInputContext(InputContextID a_context)
	{
		const auto id = RuntimeContextId(a_context, "PushInputContext"sv);
		if (!id) {
			return;
		}
		using func_t = void(ControlMap*, std::uint32_t);
		REL::Relocation<func_t> func{ RELOCATION_ID(67243, 68543) };
		return func(this, *id);
	}

	void ControlMap::ToggleControls(UEFlag a_flags, bool a_enable, bool a_storeState)
	{
		if (REL::Module::IsVR()) {
			stl::report_and_fail("ControlMap::ToggleControls: no verified VR id for the engine function; refused."sv);
		}
		using func_t = void(ControlMap*, UEFlag, bool, bool);
		REL::Relocation<func_t> func{ RELOCATION_ID(67245, 68545) };
		func(this, a_flags, a_enable, a_storeState);
	}
}
