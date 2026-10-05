#include "RE/U/UIMessageQueue.h"

#include "RE/B/BSFixedString.h"
#include "RE/U/UIMessage.h"
#include "SKSE/Logger.h"

namespace RE
{
	UIMessageQueue* UIMessageQueue::GetSingleton()
	{
		REL::Relocation<UIMessageQueue**> singleton{ Offset::UIMessageQueue::Singleton };
		return *singleton;
	}

	void UIMessageQueue::AddMessage(const BSFixedString& a_menuName, UI_MESSAGE_TYPE a_type, IUIMessageData* a_data)
	{
		using func_t = decltype(&UIMessageQueue::AddMessage);
		REL::Relocation<func_t> func{ Offset::UIMessageQueue::AddMessage };
		// mit-3.7: the game's own number for the type (kChatterEvent is 14 on 1.7.104).
		if (a_type == UI_MESSAGE_TYPE::k1_7_104_Type13 && !REL::Module::IsExactly(SKSE::RUNTIME_SSE_1_7_104)) {
			SKSE::log::error("UIMessageQueue::AddMessage: k1_7_104_Type13 exists only on 1.7.104; message to {} not sent.", a_menuName.c_str());
			return;
		}
		return func(this, a_menuName, static_cast<UI_MESSAGE_TYPE>(ToRuntimeUIMessageType(a_type)), a_data);
	}

	IUIMessageData* UIMessageQueue::CreateUIMessageData(const BSFixedString& a_name)
	{
		using func_t = decltype(&UIMessageQueue::CreateUIMessageData);
		REL::Relocation<func_t> func{ Offset::UIMessageQueue::CreateUIMessageData };
		return func(this, a_name);
	}

	void UIMessageQueue::ProcessCommands()
	{
		using func_t = decltype(&UIMessageQueue::ProcessCommands);
		REL::Relocation<func_t> func{ Offset::UIMessageQueue::ProcessCommands };
		return func(this);
	}
}
