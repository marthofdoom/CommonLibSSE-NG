#include "RE/B/BSInputDeviceManager.h"

#include "RE/B/BSInputDeviceFactory.h"
#include "RE/B/BSPCGamepadDeviceDelegate.h"
#include "RE/B/BSPCGamepadDeviceHandler.h"
#include "RE/B/BSTrackedControllerDevice.h"
#include "RE/B/BSWin32KeyboardDevice.h"
#include "RE/B/BSWin32MouseDevice.h"
#include "RE/B/BSWin32VirtualKeyboardDevice.h"

namespace RE
{
	BSInputDeviceManager* BSInputDeviceManager::GetSingleton()
	{
		REL::Relocation<BSInputDeviceManager**> singleton{ Offset::BSInputDeviceManager::Singleton };
		return *singleton;
	}

	namespace
	{
		bool Is17104() noexcept { return REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104); }

		// 1.7.104: six slots from +0x60 (see BSInputDeviceManager.h).
		BSIInputDevice** Slots17104(const BSInputDeviceManager* a_self) noexcept
		{
			return reinterpret_cast<BSIInputDevice**>(reinterpret_cast<std::uintptr_t>(a_self) + 0x60);
		}

		constexpr std::uint32_t kSlots17104 = 6;
	}

	BSIInputDevice* BSInputDeviceManager::GetDevice(INPUT_DEVICE a_device) const noexcept
	{
		if (Is17104()) {
			auto index = static_cast<std::uint32_t>(std::to_underlying(a_device));  // kNone wraps to out of range
			if (a_device == INPUT_DEVICE::kVirtualKeyboard) {
				index = 5;
			}
			return index < kSlots17104 ? Slots17104(this)[index] : nullptr;
		}
		return devices[std::to_underlying(a_device)];
	}

	BSPCGamepadDeviceDelegate* BSInputDeviceManager::GetGamepad()
	{
		auto handler = GetGamepadHandler();
		return handler ? handler->currentPCGamePadDelegate : nullptr;
	}

	BSPCGamepadDeviceHandler* BSInputDeviceManager::GetGamepadHandler()
	{
		return static_cast<BSPCGamepadDeviceHandler*>(GetDevice(INPUT_DEVICE::kGamepad));
	}

	BSWin32KeyboardDevice* BSInputDeviceManager::GetKeyboard()
	{
		return static_cast<BSWin32KeyboardDevice*>(GetDevice(INPUT_DEVICE::kKeyboard));
	}

	BSWin32MouseDevice* BSInputDeviceManager::GetMouse()
	{
		return static_cast<BSWin32MouseDevice*>(GetDevice(INPUT_DEVICE::kMouse));
	}

	BSTrackedControllerDevice* BSInputDeviceManager::GetVRControllerRight()
	{
#ifndef ENABLE_SKYRIM_VR
		return nullptr;
#else
		if SKYRIM_REL_VR_CONSTEXPR (!REL::Module::IsVR()) {
			return nullptr;
		}
		return static_cast<BSTrackedControllerDevice*>(devices[std::to_underlying(INPUT_DEVICE::kVRRight)]);
#endif
	}

	BSTrackedControllerDevice* BSInputDeviceManager::GetVRControllerLeft()
	{
#ifndef ENABLE_SKYRIM_VR
		return nullptr;
#else
		if SKYRIM_REL_VR_CONSTEXPR (!REL::Module::IsVR()) {
			return nullptr;
		}
		return static_cast<BSTrackedControllerDevice*>(devices[std::to_underlying(INPUT_DEVICE::kVRLeft)]);
#endif
	}

	BSWin32VirtualKeyboardDevice* BSInputDeviceManager::GetVirtualKeyboard()
	{
		return static_cast<BSWin32VirtualKeyboardDevice*>(GetDevice(INPUT_DEVICE::kVirtualKeyboard));
	}

	bool BSInputDeviceManager::IsGamepadConnected()
	{
		auto handler = GetGamepadHandler();
		return handler && handler->currentPCGamePadDelegate;
	}

	bool BSInputDeviceManager::IsGamepadEnabled()
	{
		auto handler = GetGamepadHandler();
		return handler && handler->currentPCGamePadDelegate && handler->currentPCGamePadDelegate->IsEnabled();
	}

	bool BSInputDeviceManager::IsMouseBackground()
	{
		auto mouse = GetMouse();
		return mouse && mouse->backgroundMouse;
	}

	bool BSInputDeviceManager::GetDeviceKeyMapping(INPUT_DEVICE a_device, std::uint32_t a_key, BSFixedString& a_mapping)
	{
		auto device = GetDevice(a_device);
		return device && device->GetKeyMapping(a_key, a_mapping);
	}

	bool BSInputDeviceManager::GetDeviceMappedKeycode(INPUT_DEVICE a_device, std::uint32_t a_key, uint32_t& a_outKeyCode)
	{
		auto device = GetDevice(a_device);
		return device && device->GetMappedKeycode(a_key, a_outKeyCode);
	}

	void BSInputDeviceManager::ProcessGamepadEnabledChange()
	{
		if (GetRuntimeData().valueQueued) {
			bool* pGamepadEnable = reinterpret_cast<bool*>(RELOCATION_ID(511901, 388465).address());
			*pGamepadEnable = true;
			GetRuntimeData().valueQueued = false;
		}
	}

	void BSInputDeviceManager::ReinitializeMouse()
	{
		auto mouse = GetMouse();
		if (mouse) {
			mouse->Reinitialize();
		}
	}

	void BSInputDeviceManager::CreateInputDevices()
	{
		if (Is17104()) {
			// As the engine's own init (0xCF93C3): slot i = factory(i) for six slots, and
			// slots 3 and 4 stay empty (the factory builds nothing for them).
			for (std::uint32_t i = 0; i < kSlots17104; i++) {
				auto& slot = Slots17104(this)[i];
				slot = BSInputDeviceFactory::CreateInputDevice(static_cast<INPUT_DEVICE>(i));
				if (slot) {
					slot->Initialize();
				}
			}
			return;
		}
		for (std::uint32_t i = 0; i < INPUT_DEVICE::kTotal; i++) {
			devices[i] = BSInputDeviceFactory::CreateInputDevice(static_cast<INPUT_DEVICE>(i));
			devices[i]->Initialize();
		}
	}

	void BSInputDeviceManager::ResetInputDevices()
	{
		if (Is17104()) {
			for (std::uint32_t i = 0; i < kSlots17104; i++) {
				if (auto device = Slots17104(this)[i]) {
					device->Reset();
				}
			}
			return;
		}
		for (std::uint32_t i = 0; i < INPUT_DEVICE::kTotal; i++) {
			if (devices[i]) {
				devices[i]->Reset();
			}
		}
	}

	void BSInputDeviceManager::DestroyInputDevices()
	{
		if (Is17104()) {
			for (std::uint32_t i = 0; i < kSlots17104; i++) {
				if (auto device = Slots17104(this)[i]) {
					device->Release();
					BSInputDeviceFactory::DestroyInputDevice(device);
				}
			}
			return;
		}
		for (std::uint32_t i = 0; i < INPUT_DEVICE::kTotal; i++) {
			if (devices[i]) {
				devices[i]->Release();
				BSInputDeviceFactory::DestroyInputDevice(devices[i]);
			}
		}
	}

	// Called by Main::Update()
	void BSInputDeviceManager::PollInputDevices(float a_secsSinceLastFrame)
	{
		// Calls Process() on each device
		// Calls ControlMap::sub_140C11600(InputEvent*)
		// Calls Rumble::Update_140C10860(float secsSinceLastFrame)
		// Emits the last InputEvent
		// resets the global BSInputEventQueue
		using func_t = decltype(&BSInputDeviceManager::PollInputDevices);
		REL::Relocation<func_t> func{ RELOCATION_ID(67315, 68617) };
		return func(this, a_secsSinceLastFrame);
	}
}
