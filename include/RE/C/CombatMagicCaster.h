#pragma once

#include "RE/B/BSPointerHandle.h"
#include "RE/C/CombatInventoryItem.h"
#include "RE/C/CombatObject.h"
#include "RE/N/NiSmartPointer.h"

namespace RE
{
	class Actor;
	class CombatController;
	class CombatProjectileAimController;
	class MagicItem;

	class CombatMagicCaster : public CombatObject
	{
	public:
		inline static constexpr auto RTTI = RTTI_CombatMagicCaster;
		inline static constexpr auto VTABLE = VTABLE_CombatMagicCaster;

		// mit-3.7: what GetMagicTarget (0A) really returns. The engine returns this
		// 16-byte aggregate BY VALUE, which the x64 ABI turns into a hidden out-slot:
		// rcx = this, rdx = the caller's MagicTarget*, r8 = CombatController*, and rax
		// = that same out pointer. Upstream declared `void* (CombatController*)`, which
		// shifts every argument one register; a hook or a call through that shape
		// passes the CombatController where the out-slot belongs (principle 6 history:
		// a passive probe built on it crashed the game).
		// Verified: base impl 1.6.1170 0x81E020 (slot 0A of 15 of the 16 caster
		// vtables; mov rdi,rdx / mov rbx,r8 / mov [rdi],eax / mov rax,rdi /
		// mov [rdi+8],rcx), 1.5.97 0x781CB0 (-> helper 0x782100, same {u32 @0, ptr @8});
		// Reanimate override 1.6.1170 0x8222C0 / 1.5.97 0x785F60 (mov eax,[rcx+0x28] /
		// mov [rdx],eax / mov rax,rdx / mov qword [rdx+8],0).
		// Engine consumers read `handle` first and `actor` only when handle is 0.
		struct MagicTarget
		{
		public:
			// members
			ActorHandle   handle;  // 00
			std::uint32_t pad04;   // 04
			Actor*        actor;   // 08
		};
		static_assert(sizeof(MagicTarget) == 0x10);
		static_assert(offsetof(MagicTarget, actor) == 0x08);

		~CombatMagicCaster() override;  // 00

		// override (CombatObject)
		void SaveGame(BGSSaveGameBuffer* a_buf) override;  // 03
		void LoadGame(BGSLoadGameBuffer* a_buf) override;  // 04

		// add
		virtual CombatInventoryItem::CATEGORY GetCategory() = 0;                                                   // 05
		virtual bool                          CheckStartCast(CombatController* a_combatController);                // 06
		virtual bool                          CheckStopCast(CombatController* a_combatController);                 // 07
		virtual float                         CalcCastMagicChance(CombatController* a_combatController) const;     // 08
		virtual float                         CalcMagicHoldTime(CombatController* a_combatController) const;       // 09
		virtual MagicTarget                   GetMagicTarget(CombatController* a_combatController) const;          // 0A - returned through a hidden out-slot, see MagicTarget
		virtual void                          NotifyStartCast(CombatController* a_combatController);               // 0B
		virtual void                          NotifyStopCast(CombatController* a_combatController);                // 0C
		virtual void                          SetupAimController(CombatProjectileAimController* a_aimController);  // 0D

		bool CheckTargetValid(const CombatController* a_combatController)
		{
			using func_t = bool* (*)(CombatMagicCaster*, const CombatController*);
			REL::Relocation<func_t> func{ RELOCATION_ID(43956, 45348) };
			return func(this, a_combatController);
		}

		static bool CheckTargetValid(const CombatController* a_combatController, Actor* a_target, const CombatInventoryItemMagic* a_inventoryItem)
		{
			using func_t = bool* (*)(const CombatController*, Actor*, const CombatInventoryItemMagic*);
			REL::Relocation<func_t> func{ RELOCATION_ID(43952, 45343) };
			return func(a_combatController, a_target, a_inventoryItem);
		}

		// members
		NiPointer<CombatInventoryItemMagic> inventoryItem;  // 10
		MagicItem*                          magicItem;      // 18
	};
	static_assert(sizeof(CombatMagicCaster) == 0x20);
}
