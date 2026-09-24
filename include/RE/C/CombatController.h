#pragma once

#include "RE/A/AITimer.h"
#include "RE/B/BSAtomic.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSTArray.h"
#include "RE/C/CombatState.h"
#include "RE/N/NiSmartPointer.h"
#include "SKSE/Version.h"

namespace RE
{
	class Actor;
	class CombatAimController;
	class CombatAreaStandard;
	class CombatBehaviorController;
	class CombatBlackboard;
	class CombatGroup;
	class CombatInventory;
	class CombatTargetSelectorStandard;
	class CombatState;
	class TESCombatStyle;

	class CombatController
	{
	public:
		[[nodiscard]] bool IsFleeing() const
		{
			return state->isFleeing;
		}

		// mit-3.7: 1.6.1170 has 8 more bytes at +0x68, so every member from +0x68 on
		// moves by 8. Upstream guarded that with SKYRIM_SUPPORT_AE, a macro nothing
		// defines, so it declared the 1.5.97 layout for every build. Verified from
		// the constructors (the call after operator new(size)):
		//   1.5.97   ctor 0x4FCA00 (id 32467, new(0xD8)): [+0x68]=0 [+0x70]=0,
		//            BSTArray at +0x78 and +0x98, [+0x90] [+0xB0] [+0xB8]=0, dword
		//            [+0xC0]=0, [+0xC8] [+0xD0]=0.
		//   1.6.1170 ctor 0x558070 (id 33214, new(0xE0)): [+0x68]=0 (8 bytes, new),
		//            [+0x70] [+0x78]=0, BSTArray at +0x80 and +0xA0, [+0x98] [+0xB8]
		//            [+0xC0]=0, dword [+0xC8]=0, [+0xD0] [+0xD8]=0.
		// GetMagicTarget confirms the tail: 1.6.1170 0x81E020 reads handleCount +0xC8,
		// cachedAttacker +0xD0, cachedTarget +0xD8; 1.5.97 0x782100 reads +0xC0,
		// +0xC8, +0xD0. The members below +0x68 are the same on both.
		struct RUNTIME_DATA
		{
		public:
			// members
			CombatAimController*                    currentAimController;    // 00
			CombatAimController*                    previousAimController;   // 08
			BSTArray<CombatAreaStandard*>           areas;                   // 10
			CombatAreaStandard*                     currentArea;             // 28
			BSTArray<CombatTargetSelectorStandard*> targetSelectors;         // 30
			CombatTargetSelectorStandard*           currentTargetSelector;   // 48
			CombatTargetSelectorStandard*           previousTargetSelector;  // 50
			std::uint32_t                           handleCount;             // 58
			std::int32_t                            unkC4;                   // 5C
			NiPointer<Actor>                        cachedAttacker;          // 60 - attackerHandle
			NiPointer<Actor>                        cachedTarget;            // 68 - targetHandle
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x70);

		// mit-3.7: RUNTIME_DATA at the running build's offset (+0x68 on 1.5.97.0,
		// +0x70 on 1.6.1170.0). Any other build is a named fatal error, never a guess.
		[[nodiscard]] static bool IsRuntimeDataVerified() noexcept
		{
			return REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_6_1170) || REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_5_97);
		}

		[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return const_cast<RUNTIME_DATA&>(std::as_const(*this).GetRuntimeData());
		}

		[[nodiscard]] const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			std::ptrdiff_t offset = 0;
			if (REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_6_1170)) {
				offset = 0x70;
			} else if (REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_5_97)) {
				offset = 0x68;
			} else {
				stl::report_and_fail(
					fmt::format(
						"CombatController::GetRuntimeData: the member layout past +0x68 is not verified for "
						"game version {} (verified: 1.6.1170.0, 1.5.97.0)."sv,
						REL::Module::get().version().string(".")));
			}
			return *reinterpret_cast<const RUNTIME_DATA*>(reinterpret_cast<std::uintptr_t>(this) + offset);
		}

		// members
		CombatGroup*                   combatGroup;           // 00
		CombatState*                   state;                 // 08
		CombatInventory*               inventory;             // 10
		CombatBlackboard*              blackboard;            // 18
		CombatBehaviorController*      behaviorController;    // 20
		ActorHandle                    attackerHandle;        // 28
		ActorHandle                    targetHandle;          // 2C
		ActorHandle                    previousTargetHandle;  // 30
		std::uint8_t                   unk34;                 // 34
		bool                           startedCombat;         // 35
		std::uint8_t                   unk36;                 // 36
		std::uint8_t                   unk37;                 // 37
		TESCombatStyle*                combatStyle;           // 38
		bool                           stoppedCombat;         // 40
		bool                           unk41;                 // 41 - isbeingMeleeAttacked?
		bool                           ignoringCombat;        // 42
		bool                           inactive;              // 43
		AITimer                        unk44;                 // 44
		float                          unk4C;                 // 4C
		BSTArray<CombatAimController*> aimControllers;        // 50
		                                                      // 68: GetRuntimeData()
	};
	static_assert(sizeof(CombatController) == 0x68);
}
