#include "RE/B/BSInputEventQueue.h"

#include "SKSE/Version.h"

namespace RE
{
	namespace
	{
		struct QueueLayout
		{
			std::ptrdiff_t button;
			std::ptrdiff_t chars;
			std::ptrdiff_t mouse;
			std::ptrdiff_t thumbstick;
			std::ptrdiff_t connect;
			std::ptrdiff_t kinect;
			std::ptrdiff_t head;
			std::ptrdiff_t tail;
		};

		// mit-3.7: see BSInputEventQueue.h for the proof of each value.
		const QueueLayout& Layout() noexcept
		{
			static constexpr QueueLayout kUpstream{ 0x20, 0x200, 0x2A0, 0x2D0, 0x330, 0x350, 0x380, 0x388 };
			static constexpr QueueLayout k17104{ 0x28, 0x208, 0x2A8, 0x2D8, 0x338, 0x358, 0x558, 0x560 };
			return REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104) ? k17104 : kUpstream;
		}

		template <class T>
		T* At(BSInputEventQueue* a_self, std::ptrdiff_t a_offset) noexcept
		{
			return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(a_self) + a_offset);
		}
	}

	ButtonEvent*        BSInputEventQueue::GetButtonEvents() noexcept { return At<ButtonEvent>(this, Layout().button); }
	CharEvent*          BSInputEventQueue::GetCharEvents() noexcept { return At<CharEvent>(this, Layout().chars); }
	MouseMoveEvent*     BSInputEventQueue::GetMouseEvents() noexcept { return At<MouseMoveEvent>(this, Layout().mouse); }
	ThumbstickEvent*    BSInputEventQueue::GetThumbstickEvents() noexcept { return At<ThumbstickEvent>(this, Layout().thumbstick); }
	DeviceConnectEvent* BSInputEventQueue::GetConnectEvents() noexcept { return At<DeviceConnectEvent>(this, Layout().connect); }
	KinectEvent*        BSInputEventQueue::GetKinectEvents() noexcept { return At<KinectEvent>(this, Layout().kinect); }
	InputEvent*&        BSInputEventQueue::GetQueueHead() noexcept { return *At<InputEvent*>(this, Layout().head); }
	InputEvent*&        BSInputEventQueue::GetQueueTail() noexcept { return *At<InputEvent*>(this, Layout().tail); }

	BSInputEventQueue* BSInputEventQueue::GetSingleton()
	{
		REL::Relocation<BSInputEventQueue**> singleton{ RELOCATION_ID(520856, 407374) };
		return *singleton;
	}

	void BSInputEventQueue::AddButtonEvent(INPUT_DEVICE a_device, std::int32_t a_id, float a_value, float a_duration)
	{
		if (buttonEventCount < MAX_BUTTON_EVENTS) {
			auto& cachedEvent = GetButtonEvents()[buttonEventCount];
			cachedEvent.value = a_value;
			cachedEvent.heldDownSecs = a_duration;
			cachedEvent.device = a_device;
			cachedEvent.idCode = a_id;
			cachedEvent.userEvent = {};

			PushOntoInputQueue(&cachedEvent);
			++buttonEventCount;
		}
	}

	void BSInputEventQueue::AddCharEvent(std::uint32_t a_keyCode)
	{
		if (charEventCount < MAX_CHAR_EVENTS) {
			auto& cachedEvent = GetCharEvents()[charEventCount];
			cachedEvent.keycode = a_keyCode;

			PushOntoInputQueue(&cachedEvent);
			++charEventCount;
		}
	}

	void BSInputEventQueue::AddMouseMoveEvent(std::int32_t a_mouseInputX, std::int32_t a_mouseInputY)
	{
		if (mouseEventCount < MAX_MOUSE_EVENTS) {
			auto& cachedEvent = GetMouseEvents()[mouseEventCount];
			cachedEvent.mouseInputX = a_mouseInputX;
			cachedEvent.mouseInputY = a_mouseInputY;
			cachedEvent.userEvent = {};

			PushOntoInputQueue(&cachedEvent);
			++mouseEventCount;
		}
	}

	void BSInputEventQueue::AddThumbstickEvent(ThumbstickEvent::InputType a_id, float a_xValue, float a_yValue)
	{
		if (thumbstickEventCount < MAX_THUMBSTICK_EVENTS) {
			auto& cachedEvent = GetThumbstickEvents()[thumbstickEventCount];
			cachedEvent.idCode = a_id;
			cachedEvent.xValue = a_xValue;
			cachedEvent.yValue = a_yValue;
			cachedEvent.userEvent = {};

			PushOntoInputQueue(&cachedEvent);
			++thumbstickEventCount;
		}
	}

	void BSInputEventQueue::AddConnectEvent(INPUT_DEVICE a_device, bool a_connected)
	{
		if (connectEventCount < MAX_CONNECT_EVENTS) {
			auto& cachedEvent = GetConnectEvents()[connectEventCount];
			cachedEvent.device = a_device;
			cachedEvent.connected = a_connected;

			PushOntoInputQueue(&cachedEvent);
			++connectEventCount;
		}
	}

	void BSInputEventQueue::AddKinectEvent(const BSFixedString& a_userEvent, const BSFixedString& a_heard)
	{
		if (kinectEventCount < MAX_KINECT_EVENTS) {
			auto& cachedEvent = GetKinectEvents()[kinectEventCount];
			cachedEvent.userEvent = a_userEvent;
			cachedEvent.heard = a_heard;

			PushOntoInputQueue(&cachedEvent);
			++kinectEventCount;
		}
	}

	void BSInputEventQueue::PushOntoInputQueue(InputEvent* a_event)
	{
		auto& queueHead = GetQueueHead();
		auto& queueTail = GetQueueTail();
		if (!queueHead) {
			queueHead = a_event;
		}

		if (queueTail) {
			queueTail->next = a_event;
		}

		queueTail = a_event;
		queueTail->next = nullptr;
	}

	void BSInputEventQueue::ClearInputQueue()
	{
		kinectEventCount = 0;
		connectEventCount = 0;
		thumbstickEventCount = 0;
		mouseEventCount = 0;
		charEventCount = 0;
		buttonEventCount = 0;
		if (REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104)) {
			// The three 1.7.104-only counts at +0x1C..+0x27, as its own ClearInputQueue
			// (0xCFBA10) zeroes them.
			std::memset(reinterpret_cast<std::uint8_t*>(this) + 0x1C, 0, 0x0C);
		}
		GetQueueTail() = nullptr;
		GetQueueHead() = nullptr;
	}
}
