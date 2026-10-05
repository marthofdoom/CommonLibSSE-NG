#pragma once

#include "RE/B/BSFixedString.h"
#include "SKSE/Version.h"

namespace RE
{
	class IUIMessageData;

	enum class UI_MESSAGE_TYPE
	{
		kUpdate = 0,
		kShow = 1,
		kReshow = 2,
		kHide = 3,
		kForceHide = 4,

		kScaleformEvent = 6,   // BSUIScaleformData
		kUserEvent = 7,        // BSUIMessageData
		kInventoryUpdate = 8,  // InventoryUpdateData
		kUserProfileChange = 9,
		kMUStatusChange = 10,
		kResumeCaching = 11,
		kUpdateController = 12,
		kChatterEvent = 13,  // 13 on 1.5.97 and 1.6.1170, 14 on 1.7.104 (see below)

		// mit-3.7: a type that only 1.7.104 has. The game numbers it 13 there; this value is the
		// fork's own name for it and is never sent to the game as is (ToRuntimeUIMessageType).
		k1_7_104_Type13 = 0x10D
	};

	// mit-3.7: 1.7.104 inserted a message type at 13, so kChatterEvent (13) is 14 there. 0..12 are
	// the same on every verified build. Proof on 1.7.104:
	//   - the UIMessage default constructor, inlined in UIMessageQueue::AddMessage (id 13631) and
	//     ConsoleLog::VPrint (id 51110), sets the type to the last value: 1.6.1170 0x1AF330 and
	//     0x8F93F3 store 0xD, 1.7.104 0x1B48C0 and 0x90F3B3 store 0xE;
	//   - the types the engine's AddMessage callers (203 / 205) pass are the same on both builds (0, 1, 3,
	//     6, 8 and the r9-relative 1/2/3/0xB), so nothing below 13 moved;
	//   - 1.7.104's MarketplaceMenu::ProcessMessage (0x57F390) handles a type 13 (0x57F3A3
	//     cmp eax,0xd) that its 1.6.1170 twin (0x577410) does not.
	// Use these at the boundary: ToRuntimeUIMessageType for a type you send, FromRuntimeUIMessageType
	// (or UIMessage::GetType) for a type you receive. The raw UIMessage::type field holds the game's
	// own number.
	[[nodiscard]] inline std::uint32_t ToRuntimeUIMessageType(UI_MESSAGE_TYPE a_type) noexcept
	{
		const auto value = static_cast<std::uint32_t>(a_type);
		if (REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104)) {
			if (a_type == UI_MESSAGE_TYPE::kChatterEvent) {
				return 14;
			}
			if (a_type == UI_MESSAGE_TYPE::k1_7_104_Type13) {
				return 13;
			}
		}
		return value;
	}

	[[nodiscard]] inline UI_MESSAGE_TYPE FromRuntimeUIMessageType(std::uint32_t a_value) noexcept
	{
		if (REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104)) {
			if (a_value == 14) {
				return UI_MESSAGE_TYPE::kChatterEvent;
			}
			if (a_value == 13) {
				return UI_MESSAGE_TYPE::k1_7_104_Type13;
			}
		}
		return static_cast<UI_MESSAGE_TYPE>(a_value);
	}

	class UIMessage
	{
	public:
		// mit-3.7: the type as the fork's enum, translated from the game's number (see above).
		[[nodiscard]] UI_MESSAGE_TYPE GetType() const noexcept
		{
			return FromRuntimeUIMessageType(static_cast<std::uint32_t>(type.underlying()));
		}

		BSFixedString                                    menu;      // 00
		stl::enumeration<UI_MESSAGE_TYPE, std::uint32_t> type;      // 08
		std::uint32_t                                    pad0C;     // 0C
		IUIMessageData*                                  data;      // 10
		bool                                             isPooled;  // 18
		std::uint8_t                                     pad19;     // 19
		std::uint16_t                                    pad1A;     // 1A
		std::uint32_t                                    pad1C;     // 1C
	};
	static_assert(sizeof(UIMessage) == 0x20);
}
