#include "RE/B/BGSDefaultObjectManager.h"

#include "SKSE/Logger.h"
#include "SKSE/Version.h"

using namespace REL;

namespace RE
{
	namespace
	{
		constexpr auto kInvalid = (std::numeric_limits<std::size_t>::max)();

		inline std::size_t MapIndex(std::underlying_type_t<DefaultObjectID> a_idx) noexcept
		{
			if (a_idx <= stl::to_underlying(DefaultObjectID::kKeywordActivatorFurnitureNoPlayer)) {
				return a_idx;
			}
			std::size_t result;
			if SKYRIM_REL_CONSTEXPR (Module::IsVR()) {
				result = (0xFFFF0000 & a_idx) >> 16;
			} else {
				result = 0x0000FFFF & a_idx;
			}
			return result ? result : kInvalid;
		}
	}

	namespace
	{
		struct Layout
		{
			std::size_t count;
			std::size_t initOffset;
		};

		// mit-3.7: the verified layouts, keyed on the EXACT build. See the header.
		const Layout* RuntimeLayout() noexcept
		{
			static constexpr Layout kAE1170{ 366, 0xB90 };
			static constexpr Layout kSE197{ 364, 0xB80 };
			if (Module::IsExactly(SKSE::RUNTIME_SSE_1_6_1170)) {
				return std::addressof(kAE1170);
			}
			if (Module::IsExactly(SKSE::RUNTIME_SSE_1_5_97)) {
				return std::addressof(kSE197);
			}
			static std::atomic_flag reported = ATOMIC_FLAG_INIT;
			if (!reported.test_and_set()) {
				SKSE::log::critical(
					"BGSDefaultObjectManager: the object/flag layout is not verified for game version {} "
					"(verified: 1.6.1170.0, 1.5.97.0). Every GetObject / IsObjectInitialized call is REFUSED "
					"and returns nullptr / false.",
					Module::get().version().string("."));
			}
			return nullptr;
		}
	}

	std::size_t BGSDefaultObjectManager::GetRuntimeObjectCount() noexcept
	{
		const auto layout = RuntimeLayout();
		return layout ? layout->count : 0;
	}

	std::size_t BGSDefaultObjectManager::GetRuntimeIndex(std::size_t a_seIndex) noexcept
	{
		const auto layout = RuntimeLayout();
		if (!layout) {
			return kInvalid;
		}
		// 1.6.1170 inserted HMCC (363) and HMAE (364) before MHFL, so the 1.5.97
		// index 363 (kModsHelpFormList) is 365 there. 0..362 are the same on both.
		// (Literal 363: in a cross-VR build DEFAULT_OBJECT stops at 182.)
		constexpr std::size_t kSEModsHelpFormList = 0x0000FFFF & stl::to_underlying(DefaultObjectID::kModsHelpFormList);
		static_assert(kSEModsHelpFormList == 363);
		if (a_seIndex == kSEModsHelpFormList && Module::IsExactly(SKSE::RUNTIME_SSE_1_6_1170)) {
			return 365;
		}
		return a_seIndex;
	}

	const bool* BGSDefaultObjectManager::GetInitArray() const noexcept
	{
		const auto layout = RuntimeLayout();
		return layout ? reinterpret_cast<const bool*>(reinterpret_cast<std::uintptr_t>(this) + layout->initOffset) : nullptr;
	}

	bool BGSDefaultObjectManager::IsObjectInitialized(std::size_t a_idx) const noexcept
	{
		const auto init = GetInitArray();
		if (!init || a_idx >= GetRuntimeObjectCount()) {
			return false;
		}
		return init[a_idx];
	}

	TESForm** BGSDefaultObjectManager::GetObject(DefaultObjectID a_object) noexcept
	{
		auto idx = MapIndex(stl::to_underlying(a_object));
		if (idx == kInvalid) {
			return nullptr;
		}
		idx = GetRuntimeIndex(idx);
		if (!IsObjectInitialized(idx)) {
			return nullptr;
		}
		return reinterpret_cast<TESForm**>(reinterpret_cast<std::uintptr_t>(this) + 0x20) + idx;
	}

	bool BGSDefaultObjectManager::IsObjectInitialized(DefaultObjectID a_object) const noexcept
	{
		const auto idx = MapIndex(stl::to_underlying(a_object));
		return idx != kInvalid && IsObjectInitialized(GetRuntimeIndex(idx));
	}

	bool BGSDefaultObjectManager::SupportsVR(DefaultObjectID a_object) noexcept
	{
		auto idx = stl::to_underlying(a_object);
		return idx <= stl::to_underlying(DefaultObjectID::kKeywordActivatorFurnitureNoPlayer) || idx & 0xFFFF0000;
	}

	bool BGSDefaultObjectManager::SupportsSE(DefaultObjectID a_object) noexcept
	{
		return (stl::to_underlying(a_object) & 0x0000FFFF) || a_object != DefaultObjectID::kWerewolfSpell;
	}

	bool BGSDefaultObjectManager::SupportsCurrentRuntime(DefaultObjectID a_object) noexcept
	{
		return MapIndex(stl::to_underlying(a_object)) != kInvalid;
	}
}
