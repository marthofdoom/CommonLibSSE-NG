#include "SKSE/Logger.h"

#include "RE/V/VirtualMachine.h"
#include "SKSE/API.h"

#include <ShlObj.h>

namespace SKSE
{
	namespace Impl
	{
		class LogEventHandler : public RE::BSTEventSink<RE::BSScript::LogEvent>
		{
		public:
			using EventResult = RE::BSEventNotifyControl;

			[[nodiscard]] static inline LogEventHandler* GetSingleton()
			{
				static LogEventHandler singleton;
				return std::addressof(singleton);
			}

			inline void SetFilter(std::regex a_filter) { _filter = std::move(a_filter); }

			EventResult ProcessEvent(const RE::BSScript::LogEvent* a_event, RE::BSTEventSource<RE::BSScript::LogEvent>*) override
			{
				using Severity = RE::BSScript::ErrorLogger::Severity;

				if (a_event && a_event->errorMsg && std::regex_search(a_event->errorMsg, _filter)) {
					switch (a_event->severity) {
					case Severity::kInfo:
						log::info("{}"sv, a_event->errorMsg);
						break;
					case Severity::kWarning:
						log::warn("{}"sv, a_event->errorMsg);
						break;
					case Severity::kError:
						log::error("{}"sv, a_event->errorMsg);
						break;
					case Severity::kFatal:
						log::critical("{}"sv, a_event->errorMsg);
						break;
					}
				}

				return EventResult::kContinue;
			}

		private:
			LogEventHandler() = default;
			LogEventHandler(const LogEventHandler&) = delete;
			LogEventHandler(LogEventHandler&&) = delete;
			~LogEventHandler() override = default;

			LogEventHandler& operator=(const LogEventHandler&) = delete;
			LogEventHandler& operator=(LogEventHandler&&) = delete;

			std::regex _filter;
		};
	}

	void add_papyrus_sink(std::regex a_filter)
	{
		auto handler = Impl::LogEventHandler::GetSingleton();
		handler->SetFilter(std::move(a_filter));

		SKSE::RegisterForAPIInitEvent([]() {
			auto papyrus = SKSE::GetPapyrusInterface();
			if (papyrus) {
				papyrus->Register([](RE::BSScript::IVirtualMachine* a_vm) {
					auto handler = Impl::LogEventHandler::GetSingleton();
					a_vm->RegisterForLogEvent(handler);
					return true;
				});
			}
		});
	}

	void remove_papyrus_sink()
	{
		SKSE::RegisterForAPIInitEvent([]() {
			auto papyrus = SKSE::GetPapyrusInterface();
			if (papyrus) {
				papyrus->Register([](RE::BSScript::IVirtualMachine* a_vm) {
					auto handler = Impl::LogEventHandler::GetSingleton();
					a_vm->UnregisterForLogEvent(handler);
					return true;
				});
			}
		});
	}

	namespace log
	{
		std::optional<std::filesystem::path> log_directory()
		{
			wchar_t*                                               buffer{ nullptr };
			const auto                                             result = ::SHGetKnownFolderPath(::FOLDERID_Documents, ::KNOWN_FOLDER_FLAG::KF_FLAG_DEFAULT, nullptr, std::addressof(buffer));
			std::unique_ptr<wchar_t[], decltype(&::CoTaskMemFree)> knownPath(buffer, ::CoTaskMemFree);
			if (!knownPath || result != S_OK) {
				error("failed to get known folder path"sv);
				return std::nullopt;
			}

			std::filesystem::path path = knownPath.get();
			path /= "My Games";
			if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
				path /= "Skyrim VR";
			} else {
				// mit-3.7: the AE id was 380738, which is NOT in the 1.6.1170 Address
				// Library (versionlib-1-6-1170-0.bin). Upstream's lookup silently fell
				// through to the next id, 380740 (RVA 0x20123C0), a pointer to
				// "Skyrim.INI", so on AE this built "My Games\Skyrim.INI\SKSE". The real
				// variable is id 502114, RVA 0x20123B0, which points at the .rdata string
				// "Skyrim Special Edition" (RVA 0x1892F80). It is the AE twin of SE
				// 508778 (RVA 0x1DEEBF0): six xrefs each with the same instruction
				// shapes (SE 0x148CD9 mov rdx / 0x5AE0E0 mov rcx / 0x5AE102 mov rcx /
				// 0x5AE825 mov rax / 0x5B742B mov r9 / 0x5B74BA; AE 0x191589 / 0x640214 /
				// 0x640236 / 0x640CE5 / 0x64B1A8 / 0x64B284).
				path /= *REL::Relocation<const char**>(RELOCATION_ID(508778, 502114)).get();
			}
			path /= "SKSE"sv;
			return path;
		}
	}
}
