#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/B/BSInputDevice.h"
#include "RE/B/BSTArray.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTSingleton.h"
#include "RE/I/InputDevices.h"
#include "RE/P/PCGamepadType.h"
#include "RE/U/UserEvents.h"

namespace RE
{
	class UserEventEnabled;

	class ControlMap :
		public BSTSingletonSDM<ControlMap>,      // 00
		public BSTEventSource<UserEventEnabled>  // 08
	{
	public:
		using InputContextID = UserEvents::INPUT_CONTEXT_ID;
		using UEFlag = UserEvents::USER_EVENT_FLAG;

		enum : std::uint32_t
		{
			kInvalid = static_cast<std::uint8_t>(-1)
		};

		struct UserEventMapping
		{
		public:
			// members
			BSFixedString                           eventID;             // 00
			std::uint16_t                           inputKey;            // 08
			std::uint16_t                           modifier;            // 08
			std::int8_t                             indexInContext;      // 0C
			bool                                    remappable;          // 0D
			bool                                    linked;              // 0E
			stl::enumeration<UEFlag, std::uint32_t> userEventGroupFlag;  // 10
			std::uint32_t                           pad14;               // 14
		};
		static_assert(sizeof(UserEventMapping) == 0x18);

		struct InputContext
		{
		public:
			[[nodiscard]] static SKYRIM_REL_VR std::size_t GetNumDeviceMappings() noexcept
			{
#ifndef SKYRIM_CROSS_VR
				return INPUT_DEVICES::kTotal;
#else
				if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
					return INPUT_DEVICES::kTotal;
				} else {
					return static_cast<std::size_t>(INPUT_DEVICES::kVirtualKeyboard) + 1;
				}
#endif
			}

			// members
			BSTArray<UserEventMapping> deviceMappings[INPUT_DEVICES::kTotal];  // 00
		};
#ifdef ENABLE_SKYRIM_VR
		static_assert(sizeof(InputContext) == 0xA8);
#else
		static_assert(sizeof(InputContext) == 0x60);
#endif

		struct LinkedMapping
		{
		public:
			// members
			BSFixedString  linkedMappingName;     // 00
			InputContextID linkedMappingContext;  // 08
			INPUT_DEVICE   device;                // 0C
			InputContextID linkFromContext;       // 10
			std::uint32_t  pad14;                 // 14
			BSFixedString  linkFromName;          // 18
		};
		static_assert(sizeof(LinkedMapping) == 0x20);

		// mit-3.7: the members after controlMap[] sit at different offsets per build,
		// because 1.6.1170 has 18 input contexts and 1.5.97 has 17. Verified from each
		// ControlMap constructor (the function that stores the singleton):
		//   1.5.97   (0xC10DDB): memset(this+0x60, 0, 0x88) = 17 contexts, BSTArray
		//            ctors at +0xE8 / +0x100, [+0x118]=0xFFFFFFFF, [+0x11C]=0x80000000,
		//            word [+0x120]=0, byte [+0x122]=0, [+0x124]=0.
		//   1.6.1170 (0xCD4699): memset(this+0x60, 0, 0x90) = 18 contexts, BSTArray
		//            ctors at +0xF0 / +0x108, [+0x120]=0xFFFFFFFF, [+0x124]=0x80000000,
		//            word [+0x128]=0, byte [+0x12A]=0, [+0x12C]=0.
		// So everything past controlMap[] is RUNTIME_DATA, at +0xE8 on 1.5.97 and +0xF0
		// on 1.6.1170. Upstream declared the 1.5.97 layout for both, and its inline
		// ToggleControls wrote +0x118, which on 1.6.1170 is contextPriorityStack's
		// size (the crash in MFO CLAUDE.md principle 11).
		struct RUNTIME_DATA
		{
		public:
			// members
			BSTArray<LinkedMapping>                          linkedMappings;                // 00
			BSTArray<InputContextID>                         contextPriorityStack;          // 18
			stl::enumeration<UEFlag, std::uint32_t>          enabledControls;               // 30
			stl::enumeration<UEFlag, std::uint32_t>          unk11C;                        // 34 - saved state, kInvalid when none
			std::int8_t                                      textEntryCount;                // 38
			bool                                             ignoreKeyboardMouse;           // 39
			bool                                             ignoreActivateDisabledEvents;  // 3A
			std::uint8_t                                     pad3B;                         // 3B
			stl::enumeration<PC_GAMEPAD_TYPE, std::uint32_t> gamePadMapType;                // 3C
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x40);

		static ControlMap* GetSingleton();

		// mit-3.7: RUNTIME_DATA at the running build's offset. On a build whose layout
		// is not verified (anything but 1.5.97.0 / 1.6.1170.0) this is a named fatal
		// error, never a guessed offset. IsRuntimeDataVerified() lets a caller check first.
		[[nodiscard]] static bool IsRuntimeDataVerified() noexcept;
		[[nodiscard]] RUNTIME_DATA&       GetRuntimeData() noexcept;
		[[nodiscard]] const RUNTIME_DATA& GetRuntimeData() const noexcept;

		// mit-3.7: the running build's input context for a 1.5.97-numbered id, or
		// nullptr. 1.6.1170 inserted a context ("Creations Menu" in its controlmap.txt)
		// at 16, so kFavor is 17 there. Contexts 0..15 are the same on both builds
		// (menu ctors write the same inputContext: Favorites 6, Map 7, Book 10,
		// Journal 12, Lockpicking 15; IMenu's kNone is 0x12 on 1.5.97, 0x13 on
		// 1.6.1170). On an unverified build every lookup is refused (nullptr, one
		// critical log line). Prefer this to indexing controlMap[] directly.
		[[nodiscard]] InputContext* GetInputContext(InputContextID a_context) const noexcept;

		std::int8_t      AllowTextInput(bool a_allow);
		bool             AreControlsEnabled(UEFlag a_flags) const noexcept { return GetRuntimeData().enabledControls.all(a_flags); }
		std::uint32_t    GetMappedKey(std::string_view a_eventID, INPUT_DEVICE a_device, InputContextID a_context = InputContextID::kGameplay) const;
		std::string_view GetUserEventName(std::uint32_t a_buttonID, INPUT_DEVICE a_device, InputContextID a_context = InputContextID::kGameplay) const;
		bool             IsActivateControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kActivate); }
		bool             IsConsoleControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kConsole); }
		bool             IsFightingControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kFighting); }
		bool             IsLookingControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kLooking); }
		bool             IsMenuControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kMenu); }
		bool             IsMainFourControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kMainFour); }
		bool             IsMovementControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kMovement); }
		bool             IsPOVSwitchControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kPOVSwitch); }
		bool             IsSneakingControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kSneaking); }
		bool             IsVATSControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kVATS); }
		bool             IsWheelZoomControlsEnabled() const noexcept { return AreControlsEnabled(UEFlag::kWheelZoom); }

		// mit-3.7: calls the engine's own ToggleControls (1.5.97 id 67245 at 0xC11C60,
		// 1.6.1170 id 68545 at 0xCD5650; rcx this, edx flags, r8b enable, r9b
		// storeState). It sets or clears enabledControls, and with storeState it does
		// the same to the saved state unless that is kInvalid, then sends
		// UserEventEnabled{new, old} from the event source at +0x08. storeState = true
		// is exactly what 3.7.0's inline version meant to do. The engine function reads
		// its own build's layout, so this is correct wherever the id resolves; VR has
		// no verified id and is a named fatal error.
		void ToggleControls(UEFlag a_flags, bool a_enable, bool a_storeState = true);

		// members
		InputContext* controlMap[InputContextID::kTotal];  // 060 - 17 declared; 18 on 1.6.1170, use GetInputContext
		                                                   // everything after this: GetRuntimeData()
	};
	static_assert(sizeof(ControlMap) == 0xE8);
}
