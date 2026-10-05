#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTSingleton.h"
#include "RE/I/InputDevices.h"
#include "SKSE/Version.h"

namespace RE
{
	class BSIInputDevice;
	class BSInputDevice;
	class BSPCGamepadDeviceDelegate;
	class BSPCGamepadDeviceHandler;
	class BSTrackedControllerDevice;
	class BSWin32KeyboardDevice;
	class BSWin32MouseDevice;
	class BSWin32VirtualKeyboardDevice;
	class InputEvent;
	struct BSRemoteGamepadEvent;

	class BSInputDeviceManager :
		public BSTEventSource<InputEvent*>,           // 00
		public BSTSingletonSDM<BSInputDeviceManager>  // 58
	{
	public:
		struct RUNTIME_DATA
		{
#define RUNTIME_DATA_CONTENT                                                         \
	bool                                 queuedGamepadEnableValue{ false }; /* 00 */ \
	bool                                 valueQueued{ false };              /* 01 */ \
	bool                                 pollingEnabled{ false };           /* 02 */ \
	std::uint8_t                         pad03;                             /* 03 */ \
	std::uint32_t                        pad04;                             /* 04 */ \
	BSTEventSource<BSRemoteGamepadEvent> remoteGamepadEventSource;          /* 08 */ \
	std::uint8_t                         unk60;                             /* 60 */ \
	std::uint8_t                         unk61;                             /* 61 */ \
	std::uint16_t                        unk62;                             /* 62 */ \
	std::uint32_t                        unk64;                             /* 64 */ \
	std::uint64_t                        unk68;                             /* 68 */

			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x70);

		static BSInputDeviceManager* GetSingleton();

		BSPCGamepadDeviceDelegate*    GetGamepad();
		BSPCGamepadDeviceHandler*     GetGamepadHandler();
		BSWin32KeyboardDevice*        GetKeyboard();
		BSWin32MouseDevice*           GetMouse();
		BSTrackedControllerDevice*    GetVRControllerRight();
		BSTrackedControllerDevice*    GetVRControllerLeft();
		BSWin32VirtualKeyboardDevice* GetVirtualKeyboard();
		[[nodiscard]] bool            IsGamepadConnected();
		[[nodiscard]] bool            IsGamepadEnabled();
		[[nodiscard]] bool            IsMouseBackground();
		bool                          GetDeviceKeyMapping(INPUT_DEVICE a_device, std::uint32_t a_key, BSFixedString& a_mapping);
		bool                          GetDeviceMappedKeycode(INPUT_DEVICE a_device, std::uint32_t a_key, std::uint32_t& a_outKeyCode);
		void                          ProcessGamepadEnabledChange();
		void                          ReinitializeMouse();
		void                          CreateInputDevices();
		void                          ResetInputDevices();
		void                          DestroyInputDevices();
		void                          PollInputDevices(float a_secsSinceLastFrame);

		// mit-3.7: 1.7.104 has six devices, not four, so RUNTIME_DATA starts at +0x90.
		// PollInputDevices (1.6.1170 0xCD8F40, 1.7.104 0xCF95F0) loops over 4 / 6 devices from
		// +0x60 (mov edi,4 at 0xCD8F5E, mov edi,6 at 0xCF960E), and reads RUNTIME_DATA's +0x60
		// byte at [this+0xE0] / [this+0xF0] and its event source at +0x88 / +0x98. Init (0xCF93A4)
		// zeroes +0x60..+0x88 and stores factory(i) into each slot. 1.5.97, 1.6.x and VR keep
		// upstream's offsets.
		[[nodiscard]] inline RUNTIME_DATA& GetRuntimeData() noexcept
		{
			if (REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104)) {
				return REL::RelocateMember<RUNTIME_DATA>(this, 0x90);
			}
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x80, 0x98);
		}

		[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			if (REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104)) {
				return REL::RelocateMember<const RUNTIME_DATA>(this, 0x90);
			}
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x80, 0x98);
		}

		// mit-3.7: the device in the running build's slot for a 1.5.97-numbered INPUT_DEVICE,
		// or nullptr. 1.7.104's device factory (0xD6EBA0) builds keyboard 0, mouse 1, gamepad
		// handler 2 and the virtual keyboard at 5 (3 and 4 build nothing), against 1.6.1170's
		// 0xCDC7A0 with the virtual keyboard at 3. So kVirtualKeyboard is slot 5 there, and
		// devices[3] is empty. On every other build this is devices[a_device], as upstream.
		[[nodiscard]] BSIInputDevice* GetDevice(INPUT_DEVICE a_device) const noexcept;

		// members
		std::uint8_t    pad59;       // 59
		std::uint16_t   pad5A;       // 5A
		std::uint32_t   pad5C;       // 5C
		BSIInputDevice* devices[4];  // 60 - 6 on 1.7.104 (virtual keyboard at 5); use GetDevice
#ifndef SKYRIM_CROSS_VR
#	if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
		BSTrackedControllerDevice* unkDevice;     // 80
		BSTrackedControllerDevice* vrDevices[2];  // 88
		RUNTIME_DATA_CONTENT                      // 98
#	elif !defined(ENABLE_SKYRIM_VR)
		RUNTIME_DATA_CONTENT  // 80
#	endif
#endif
	};
#ifndef ENABLE_SKYRIM_VR
	static_assert(sizeof(BSInputDeviceManager) == 0xF0);
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
	static_assert(sizeof(BSInputDeviceManager) == 0x108);
#else
	static_assert(sizeof(BSInputDeviceManager) == 0x80);
#endif
}
#undef RUNTIME_DATA_CONTENT
