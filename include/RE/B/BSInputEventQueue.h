#pragma once

#include "RE/B/BSTSingleton.h"
#include "RE/B/ButtonEvent.h"
#include "RE/C/CharEvent.h"
#include "RE/D/DeviceConnectEvent.h"
#include "RE/K/KinectEvent.h"
#include "RE/M/MouseMoveEvent.h"
#include "RE/T/ThumbstickEvent.h"

namespace RE
{
	class BSInputEventQueue : public BSTSingletonSDM<BSInputEventQueue>
	{
	public:
		inline static constexpr std::uint8_t MAX_BUTTON_EVENTS = 10;
		inline static constexpr std::uint8_t MAX_CHAR_EVENTS = 5;
		inline static constexpr std::uint8_t MAX_MOUSE_EVENTS = 1;
		inline static constexpr std::uint8_t MAX_THUMBSTICK_EVENTS = 2;
		inline static constexpr std::uint8_t MAX_CONNECT_EVENTS = 1;
		inline static constexpr std::uint8_t MAX_KINECT_EVENTS = 1;

		static BSInputEventQueue* GetSingleton();

		void AddButtonEvent(INPUT_DEVICE a_device, std::int32_t a_id, float a_value, float a_duration);
		void AddCharEvent(std::uint32_t a_keyCode);
		void AddMouseMoveEvent(std::int32_t a_mouseInputX, std::int32_t a_mouseInputY);
		void AddThumbstickEvent(ThumbstickEvent::InputType a_id, float a_xValue, float a_yValue);
		void AddConnectEvent(INPUT_DEVICE a_device, bool a_connected);
		void AddKinectEvent(const BSFixedString& a_userEvent, const BSFixedString& a_heard);
		void PushOntoInputQueue(InputEvent* a_event);
		void ClearInputQueue();

		// mit-3.7: everything after the six counts sits at a per-build offset, because 1.7.104
		// added three event kinds. Proven from the queue's own construction (one eh-vector-ctor
		// call per array, in BSInputDeviceManager's init: 1.6.1170 0xCD8C02, 1.7.104 0xCF91D2)
		// and from the engine's AddButtonEvent (1.6.1170 0xCDA920, 1.7.104 0xCFB340),
		// AddConnectEvent (0xCDAB80, 0xCFB5D0) and ClearInputQueue (0xCDACA0, 0xCFBA10):
		//                       1.5.97 / 1.6.1170       1.7.104
		//   buttonEvents[10]    +0x020 (0x30 each)      +0x028
		//   charEvents[5]       +0x200 (0x20 each)      +0x208
		//   mouseEvents[1]      +0x2A0 (0x30)           +0x2A8
		//   thumbstickEvents[2] +0x2D0 (0x30 each)      +0x2D8
		//   connectEvents[1]    +0x330 (0x20)           +0x338
		//   kinectEvents[1]     +0x350 (0x30)           +0x358
		//   (1.7.104 only)                              +0x388 SixaxisEvent (0x90), +0x4A8 2 x
		//                                               MotionGestureEvent (0x38), +0x518 AmiiboEvent (0x40)
		//   queueHead           +0x380                  +0x558
		//   queueTail           +0x388                  +0x560
		// The six counts stay at +0x04..+0x18 on every build (AddButtonEvent reads [+4] on both,
		// AddConnectEvent [+0x14]). 1.7.104 has three more counts at +0x1C..+0x27, which its
		// ClearInputQueue zeroes. ButtonEvent's own fields are at the same offsets on both
		// builds (value +0x28, heldDownSecs +0x2C, device +0x08, idCode +0x20, userEvent
		// +0x18 in AddButtonEvent). The 1.5.97 / 1.6.1170 column is upstream's, unchanged.
		// Use the accessors; there are no member declarations past the counts any more.
		[[nodiscard]] ButtonEvent*        GetButtonEvents() noexcept;
		[[nodiscard]] CharEvent*          GetCharEvents() noexcept;
		[[nodiscard]] MouseMoveEvent*     GetMouseEvents() noexcept;
		[[nodiscard]] ThumbstickEvent*    GetThumbstickEvents() noexcept;
		[[nodiscard]] DeviceConnectEvent* GetConnectEvents() noexcept;
		[[nodiscard]] KinectEvent*        GetKinectEvents() noexcept;
		[[nodiscard]] InputEvent*&        GetQueueHead() noexcept;
		[[nodiscard]] InputEvent*&        GetQueueTail() noexcept;

		// members
		std::uint8_t  pad001;                // 001
		std::uint16_t pad002;                // 002
		std::uint32_t buttonEventCount;      // 004
		std::uint32_t charEventCount;        // 008
		std::uint32_t mouseEventCount;       // 00C
		std::uint32_t thumbstickEventCount;  // 010
		std::uint32_t connectEventCount;     // 014
		std::uint32_t kinectEventCount;      // 018
		                                     // 01C: per build, use the accessors above
	};
	static_assert(sizeof(BSInputEventQueue) == 0x1C);
}
