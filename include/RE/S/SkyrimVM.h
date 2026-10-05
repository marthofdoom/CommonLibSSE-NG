#pragma once

#include "RE/B/BSAtomic.h"
#include "RE/B/BSTEvent.h"
#include "RE/B/BSTFreeList.h"
#include "RE/B/BSTHashMap.h"
#include "RE/B/BSTMessageQueue.h"
#include "RE/B/BSTSingleton.h"
#include "RE/B/BSTSmartPointer.h"
#include "RE/C/CompiledScriptLoader.h"
#include "RE/D/DelayFunctor.h"
#include "RE/F/FragmentSystem.h"
#include "RE/H/HandlePolicy.h"
#include "RE/I/IFreezeQuery.h"
#include "RE/I/IStackCallbackSaveInterface.h"
#include "RE/L/Logger.h"
#include "RE/P/Profiler.h"
#include "RE/S/SavePatcher.h"
#include "RE/S/SimpleAllocMemoryPagePolicy.h"
#include "RE/S/SkyrimScriptObjectBindPolicy.h"
#include "SKSE/Version.h"
#include "RE/S/SkyrimScriptStore.h"

namespace RE
{
	namespace BSScript
	{
		class IFunctionArguments;
		class IVirtualMachine;
		class IVMDebugInterface;
		class IVMSaveLoadInterface;
		struct StatsEvent;
	}

	struct PositionPlayerEvent;
	struct TESActivateEvent;
	struct TESActiveEffectApplyRemoveEvent;
	struct TESActorLocationChangeEvent;
	struct TESBookReadEvent;
	struct TESCellAttachDetachEvent;
	struct TESCellFullyLoadedEvent;
	struct TESCombatEvent;
	struct TESContainerChangedEvent;
	struct TESDeathEvent;
	struct TESDestructionStageChangedEvent;
	struct TESEnterBleedoutEvent;
	struct TESEquipEvent;
	struct TESFastTravelEndEvent;
	struct TESFormDeleteEvent;
	struct TESFurnitureEvent;
	struct TESGrabReleaseEvent;
	struct TESHitEvent;
	struct TESInitScriptEvent;
	struct TESLoadGameEvent;
	struct TESLockChangedEvent;
	struct TESMagicEffectApplyEvent;
	struct TESMagicWardHitEvent;
	struct TESMoveAttachDetachEvent;
	struct TESObjectLoadedEvent;
	struct TESObjectREFRTranslationEvent;
	struct TESOpenCloseEvent;
	struct TESPackageEvent;
	struct TESPerkEntryRunEvent;
	struct TESPlayerBowShotEvent;
	struct TESQuestInitEvent;
	struct TESQuestStageEvent;
	struct TESResetEvent;
	struct TESResolveNPCTemplatesEvent;
	struct TESSceneActionEvent;
	struct TESSceneEvent;
	struct TESScenePhaseEvent;
	struct TESSellEvent;
	struct TESSleepStartEvent;
	struct TESSleepStopEvent;
	struct TESSpellCastEvent;
	struct TESSwitchRaceCompleteEvent;
	struct TESTopicInfoEvent;
	struct TESTrackedStatsEvent;
	struct TESTrapHitEvent;
	struct TESTriggerEnterEvent;
	struct TESTriggerEvent;
	struct TESTriggerLeaveEvent;
	struct TESUniqueIDChangeEvent;

	class SkyrimVM :
		public BSTSingletonSDM<SkyrimVM>,                      // 01A0
		public BSScript::IFreezeQuery,                         // 0000
		public BSScript::IStackCallbackSaveInterface,          // 0008
		public BSTEventSink<TESActivateEvent>,                 // 0010
		public BSTEventSink<TESActiveEffectApplyRemoveEvent>,  // 0018
		public BSTEventSink<TESActorLocationChangeEvent>,      // 0020
		public BSTEventSink<TESBookReadEvent>,                 // 0028
		public BSTEventSink<TESCellAttachDetachEvent>,         // 0030
		public BSTEventSink<TESCellFullyLoadedEvent>,          // 0038
		public BSTEventSink<TESCombatEvent>,                   // 0040
		public BSTEventSink<TESContainerChangedEvent>,         // 0048
		public BSTEventSink<TESDeathEvent>,                    // 0050
		public BSTEventSink<TESDestructionStageChangedEvent>,  // 0058
		public BSTEventSink<TESEnterBleedoutEvent>,            // 0060
		public BSTEventSink<TESEquipEvent>,                    // 0068
		public BSTEventSink<TESFormDeleteEvent>,               // 0070
		public BSTEventSink<TESFurnitureEvent>,                // 0078
		public BSTEventSink<TESGrabReleaseEvent>,              // 0080
		public BSTEventSink<TESHitEvent>,                      // 0088
		public BSTEventSink<TESInitScriptEvent>,               // 0090
		public BSTEventSink<TESLoadGameEvent>,                 // 0098
		public BSTEventSink<TESLockChangedEvent>,              // 00A0
		public BSTEventSink<TESMagicEffectApplyEvent>,         // 00A8
		public BSTEventSink<TESMagicWardHitEvent>,             // 00B0
		public BSTEventSink<TESMoveAttachDetachEvent>,         // 00B8
		public BSTEventSink<TESObjectLoadedEvent>,             // 00C0
		public BSTEventSink<TESObjectREFRTranslationEvent>,    // 00C8
		public BSTEventSink<TESOpenCloseEvent>,                // 00D0
		public BSTEventSink<TESPackageEvent>,                  // 00D8
		public BSTEventSink<TESPerkEntryRunEvent>,             // 00E0
		public BSTEventSink<TESQuestInitEvent>,                // 00E8
		public BSTEventSink<TESQuestStageEvent>,               // 00F0
		public BSTEventSink<TESResetEvent>,                    // 00F8
		public BSTEventSink<TESResolveNPCTemplatesEvent>,      // 0100
		public BSTEventSink<TESSceneEvent>,                    // 0108
		public BSTEventSink<TESSceneActionEvent>,              // 0110
		public BSTEventSink<TESScenePhaseEvent>,               // 0118
		public BSTEventSink<TESSellEvent>,                     // 0120
		public BSTEventSink<TESSleepStartEvent>,               // 0128
		public BSTEventSink<TESSleepStopEvent>,                // 0130
		public BSTEventSink<TESSpellCastEvent>,                // 0138
		public BSTEventSink<TESTopicInfoEvent>,                // 0140
		public BSTEventSink<TESTrackedStatsEvent>,             // 0148
		public BSTEventSink<TESTrapHitEvent>,                  // 0150
		public BSTEventSink<TESTriggerEvent>,                  // 0158
		public BSTEventSink<TESTriggerEnterEvent>,             // 0160
		public BSTEventSink<TESTriggerLeaveEvent>,             // 0168
		public BSTEventSink<TESUniqueIDChangeEvent>,           // 0170
		public BSTEventSink<TESSwitchRaceCompleteEvent>,       // 0178
		public BSTEventSink<TESPlayerBowShotEvent>,            // 0180
		public BSTEventSink<TESFastTravelEndEvent>,            // 0188
		public BSTEventSink<PositionPlayerEvent>,              // 0190
		public BSTEventSink<BSScript::StatsEvent>,             // 0198
		public BSTEventSource<BSScript::StatsEvent>            // 01A8
	{
	public:
		inline static constexpr auto RTTI = RTTI_SkyrimVM;

		struct UpdateDataEvent : public BSIntrusiveRefCounted
		{
		public:
			enum class UpdateType : bool
			{
				kRepeat = 0,    // RegisterForUpdate/RegisterForUpdateGameTime
				kNoRepeat = 1,  // RegisterForSingleUpdate/RegisterForSingleUpdateGameTime
			};

			// members
			UpdateType    updateType;       // 04
			std::uint16_t pad06;            // 06
			std::uint32_t timeToSendEvent;  // 08 - updateTime + currentVMMenuMode/currentVMDaysPassed
			std::uint32_t updateTime;       // 0C
			VMHandle      handle;           // 10
		};
		static_assert(sizeof(UpdateDataEvent) == 0x18);

		struct WaitCall
		{
		public:
			// members
			std::uint32_t              timeToSendEvent;  // 00 - Same as UpdateDataEvent, updateTime is discarded
			VMStackID                  stackID;          // 04 - used for vm->ReturnFromLatent()
			BSScript::IVirtualMachine* vm;               // 08
		};
		static_assert(sizeof(WaitCall) == 0x10);

		struct LOSDataEvent : public BSIntrusiveRefCounted
		{
		public:
			enum class LOSEventType
			{
				kGain = 0,
				kLost,
				kBoth,
			};

			enum class PreviousLOS
			{
				kLOS = 1,
				kNoLOS = 2
			};

			// members
			std::uint32_t pad04;               // 04
			VMHandle      handle;              // 08
			FormID        akViewerFormID;      // 10
			FormID        akTargetFormID;      // 14
			LOSEventType  losEventType;        // 18
			PreviousLOS   lastLOSCheckResult;  // 1C - (2 - (akViewer::HasLOS(akTarget) != 0))
		};
		static_assert(sizeof(LOSDataEvent) == 0x20);

		struct InventoryEventFilterLists : public BSIntrusiveRefCounted
		{
			// members
			BSTSet<FormID> itemsForFiltering;      // 08
			BSTSet<FormID> itemListsForFiltering;  // 38
		};
		static_assert(sizeof(InventoryEventFilterLists) == 0x68);

		struct ISendEventFilter
		{
			virtual bool matchesFilter(VMHandle handle) = 0;
		};

		~SkyrimVM() override;  // 00

		static SkyrimVM* GetSingleton();

		bool QueuePostRenderCall(const BSTSmartPointer<SkyrimScript::DelayFunctor>& a_functor);
		void RelayEvent(VMHandle handle, BSFixedString* event, BSScript::IFunctionArguments* args, ISendEventFilter* optionalFilter);
		void SendAndRelayEvent(VMHandle handle, BSFixedString* event, BSScript::IFunctionArguments* args, ISendEventFilter* optionalFilter);

		// mit-3.7: everything SkyrimVM declares itself sits at a per-build offset, because
		// 1.7.104 added two event-sink bases, BSTEventSink<TESAmiiboTouchEvent> at +0x180 and
		// BSTEventSink<TESAmiiboForcedStopDetectionEvent> at +0x188 (RTTI ClassHierarchyDescriptor
		// of .?AVSkyrimVM@@: 58 bases on 1.6.1170, 60 on 1.7.104). The sinks declared from +0x180
		// on (PlayerBowShot, FastTravelEnd, PositionPlayer, StatsEvent), the StatsEvent source and
		// the singleton base therefore sit 0x10 further on 1.7.104 (0x190/0x198/0x1A0/0x1A8,
		// source 0x1B8, singleton 0x1B0), and RUNTIME_DATA starts at +0x210, not +0x200.
		// Proven on 1.7.104:
		//   - every read through the singleton (id 400475; 1.6.1170 0x20FBA70, 1.7.104 0x21A36E0)
		//     in the 194 functions that load it: [+0x200] x59 -> [+0x210] x59 (impl), [+0x478] x2
		//     -> [+0x488] (fragmentSystem), [+0x190] x3 -> [+0x1A0] (PositionPlayer sink);
		//     e.g. 1.6.1170 0x1B9B28 mov rcx,[rax+0x200] / 1.7.104 0x1BF0D8 mov rcx,[rax+0x210];
		//   - RelayEvent (id 54033): 1.6.1170 0x9CB940 mov rcx,[rcx+0x200], 1.7.104 0x9E35B0
		//     mov rcx,[rcx+0x210];
		//   - the constructor (1.6.1170 0x9BDE90, 1.7.104 0x9D5760, aligned instruction by
		//     instruction): every member store moves by +0x10, up to the last at 0x8970 -> 0x8980,
		//     so sizeof is 0x8978 on 1.6.1170 and 0x8988 on 1.7.104.
		// Every other build keeps upstream's +0x200. Use GetRuntimeData(); the members are no
		// longer declared inline, so code that read them directly fails to compile instead of
		// reading the wrong bytes on 1.7.104. Upcasting a SkyrimVM* to one of the four sinks
		// above, to BSTEventSource<BSScript::StatsEvent> or to the singleton base is also wrong on
		// 1.7.104; use AsStatsEventSource().
		struct RUNTIME_DATA
		{
		public:
			// members
			BSTSmartPointer<BSScript::IVirtualMachine>                            impl;                         // 0000
			BSScript::IVMSaveLoadInterface*                                       saveLoadInterface;            // 0008
			BSScript::IVMDebugInterface*                                          debugInterface;               // 0010
			BSScript::SimpleAllocMemoryPagePolicy                                 memoryPagePolicy;             // 0018
			BSScript::CompiledScriptLoader                                        scriptLoader;                 // 0040
			SkyrimScript::Logger                                                  logger;                       // 0078
			SkyrimScript::HandlePolicy                                            handlePolicy;                 // 0128
			SkyrimScript::ObjectBindPolicy                                        objectBindPolicy;             // 0198
			BSTSmartPointer<SkyrimScript::Store>                                  scriptStore;                  // 0270
			SkyrimScript::FragmentSystem                                          fragmentSystem;               // 0278
			SkyrimScript::Profiler                                                profiler;                     // 0390
			SkyrimScript::SavePatcher                                             savePatcher;                  // 0470
			mutable BSSpinLock                                                    frozenLock;                   // 0478
			std::uint32_t                                                         isFrozen;                     // 0480
			mutable BSSpinLock                                                    currentVMTimeLock;            // 0484
			std::uint32_t                                                         currentVMTime;                // 048C
			std::uint32_t                                                         currentVMMenuModeTime;        // 0490
			std::uint32_t                                                         currentVMGameTime;            // 0494
			std::uint32_t                                                         currentVMDaysPassed;          // 0498 - Calender.GetDaysPassed() * 1000
			mutable BSSpinLock                                                    queuedWaitEventLock;          // 049C
			std::uint32_t                                                         pad06A4;                      // 04A4
			BSTArray<WaitCall>                                                    queuedWaitCalls;              // 04A8 - Utility.Wait() calls
			BSTArray<WaitCall>                                                    queuedWaitMenuModeCalls;      // 04C0 - Utility.WaitMenuMode() calls
			BSTArray<WaitCall>                                                    queuedWaitGameCalls;          // 04D8 - Utility.WaitGameTime() calls
			mutable BSSpinLock                                                    queuedLOSEventCheckLock;      // 04F0
			BSTArray<BSTSmartPointer<LOSDataEvent>>                               queuedLOSEventChecks;         // 04F8 - OnGainLOS/OnLostLOS
			std::uint32_t                                                         currentLOSEventCheckIndex;    // 0510
			mutable BSSpinLock                                                    queuedOnUpdateEventLock;      // 0514
			std::uint32_t                                                         pad071C;                      // 051C
			BSTArray<BSTSmartPointer<UpdateDataEvent>>                            queuedOnUpdateEvents;         // 0520
			BSTArray<BSTSmartPointer<UpdateDataEvent>>                            queuedOnUpdateGameEvents;     // 0538
			std::uint32_t                                                         unk0750;                      // 0550
			mutable BSSpinLock                                                    registeredSleepEventsLock;    // 0554
			std::uint32_t                                                         pad075C;                      // 055C
			BSTSet<VMHandle>                                                      registeredSleepEvents;        // 0560 - RegisterForSleep
			mutable BSSpinLock                                                    registeredStatsEventsLock;    // 0590
			BSTSet<VMHandle>                                                      registeredStatsEvents;        // 0598 - RegisterForTrackedStats
			BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   renderSafeFunctorPool1;       // 05C8
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  renderSafeFunctorQueue1;      // 25E0
			BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   renderSafeFunctorPool2;       // 2608
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  renderSafeFunctorQueue2;      // 4620
			BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   postRenderFunctorPool1;       // 4648
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  postRenderFunctorQueue1;      // 6660
			BSTStaticFreeList<BSTSmartPointer<SkyrimScript::DelayFunctor>, 512>   postRenderFunctorPool2;       // 6688
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>  postRenderFunctorQueue2;      // 86A0
			mutable BSSpinLock                                                    renderSafeQueueLock;          // 86C8
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* renderSafeQueueToReadFrom;    // 86D0
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* renderSafeQueueToWriteTo;     // 86D8
			mutable BSSpinLock                                                    postRenderQueueLock;          // 86E0
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* postRenderQueueToReadFrom;    // 86E8
			BSTCommonLLMessageQueue<BSTSmartPointer<SkyrimScript::DelayFunctor>>* postRenderQueueToWriteTo;     // 86F0
			mutable BSSpinLock                                                    userLogMapLock;               // 86F8
			BSTHashMap<const char*, SkyrimScript::Logger*>                        userLogMap;                   // 8700 - Debug.OpenUserLog()
			mutable BSSpinLock                                                    currentVMOverstressTimeLock;  // 8730
			std::uint32_t                                                         currentVMOverstressTime;      // 8738
			std::uint32_t                                                         lastVMStackDumpTime;          // 873C
			mutable BSSpinLock                                                    InventoryEventFilterMapLock;  // 8740
			BSTHashMap<VMHandle, InventoryEventFilterLists*>                      InventoryEventFilterMap;      // 8748 - AddInventoryEventFilter()
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x8778);

		[[nodiscard]] static bool IsExactly1_7_104() noexcept
		{
			return REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104);
		}

		[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA>(this, IsExactly1_7_104() ? 0x210 : 0x200);
		}

		[[nodiscard]] const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RelocateMember<const RUNTIME_DATA>(this, IsExactly1_7_104() ? 0x210 : 0x200);
		}

		[[nodiscard]] BSTEventSource<BSScript::StatsEvent>* AsStatsEventSource() noexcept
		{
			return &REL::RelocateMember<BSTEventSource<BSScript::StatsEvent>>(this, IsExactly1_7_104() ? 0x1B8 : 0x1A8);
		}
	};
	static_assert(sizeof(SkyrimVM) == 0x200);  // the bases; RUNTIME_DATA follows at the build's offset
}
