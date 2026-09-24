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

		std::string UnverifiedMessage(std::string_view a_what)
		{
			return fmt::format(
				"ControlMap::{}: the ControlMap layout is not verified for game version {} "
				"(verified: 1.6.1170.0, 1.5.97.0)."sv,
				a_what, REL::Module::get().version().string("."sv));
		}
	}

	bool ControlMap::IsRuntimeDataVerified() noexcept
	{
		return IsAE1170() || IsSE197();
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
			index = a_context == InputContextID::kFavor ? 17 : stl::to_underlying(a_context);
		} else if (IsSE197()) {
			count = 17;
			index = stl::to_underlying(a_context);
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
