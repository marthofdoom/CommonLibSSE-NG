#include "RE/V/VirtualMachine.h"

#include "RE/S/SkyrimVM.h"

namespace RE
{
	namespace BSScript
	{
		namespace Internal
		{
			VirtualMachine* VirtualMachine::GetSingleton()
			{
				auto vm = SkyrimVM::GetSingleton();
				return vm ? static_cast<VirtualMachine*>(vm->GetRuntimeData().impl.get()) : nullptr;  // mit-3.7: +0x210 on 1.7.104
			}
		}
	}
}
