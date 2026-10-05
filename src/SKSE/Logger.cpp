#include "SKSE/Logger.h"

#include "RE/B/BSTEvent.h"
#include "RE/L/LogEvent.h"
#include "RE/V/VirtualMachine.h"

#include "REX/W32/OLE32.h"
#include "REX/W32/SHELL32.h"

#include "SKSE/API.h"

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

	namespace log
	{
		std::optional<std::filesystem::path> log_directory()
		{
			wchar_t*                                                       buffer{ nullptr };
			const auto                                                     result = REX::W32::SHGetKnownFolderPath(REX::W32::FOLDERID_Documents, REX::W32::KF_FLAG_DEFAULT, nullptr, std::addressof(buffer));
			std::unique_ptr<wchar_t[], decltype(&REX::W32::CoTaskMemFree)> knownPath(buffer, REX::W32::CoTaskMemFree);
			if (!knownPath || result != 0) {
				error("failed to get known folder path"sv);
				return std::nullopt;
			}

			std::filesystem::path path = knownPath.get();
			path /= "My Games"sv;
			if SKYRIM_REL_VR_CONSTEXPR (REL::Module::IsVR()) {
				path /= "Skyrim VR";
			} else {
				// mit-3.7: the game's own "My Games" folder name, read from the variable
				// the game itself uses (a const char* to .rdata "Skyrim Special Edition").
				// Its Address Library id depends on the build:
				//   SE  1.5.x          508778 (1.5.97 RVA 0x1DEEBF0)
				//   AE  1.6.317-1.6.659 380738 (in every versionlib from 317 to 659; not in
				//                       1130 and later)
				//   AE  1.6.1130+       502114 (1130 RVA 0x20053C0, 1170 RVA 0x20123B0,
				//                       1179 RVA 0x20133C0; not in 659 and earlier)
				// 1.6.1170 verified: 502114 points at "Skyrim Special Edition" (RVA
				// 0x1892F80), the AE twin of SE 508778 by six xrefs of the same shape (SE
				// 0x148CD9 / 0x5AE0E0 / 0x5AE102 / 0x5AE825 / 0x5B742B / 0x5B74BA; AE
				// 0x191589 / 0x640214 / 0x640236 / 0x640CE5 / 0x64B1A8 / 0x64B284).
				// Upstream 3.7.0 used 380738 everywhere, which on 1.6.1170 fell through to
				// 380740 ("Skyrim.INI"). This function returns an optional, so a build
				// whose library lacks the id is NEVER fatal: it is logged and returns
				// nullopt (try_id2offset, not the fatal id2offset).
				const auto& version = REL::Module::get().version();
				std::uint64_t id = 508778;
				if (REL::Module::IsAE()) {
					id = version >= REL::Version(1, 6, 1130, 0) ? 502114 : 380738;
				}
				const auto offset = REL::IDDatabase::get().try_id2offset(id);
				if (!offset) {
					error("log_directory: Address Library id {} (game folder name) is not in the library for game version {}; no log directory"sv,
						id, version.string("."sv));
					return std::nullopt;
				}
				const auto name = *reinterpret_cast<const char* const*>(REL::Module::get().base() + *offset);
				if (!name) {
					error("log_directory: the game folder name (id {}) is null on game version {}; no log directory"sv,
						id, version.string("."sv));
					return std::nullopt;
				}
				path /= name;
			}
			path /= "SKSE"sv;

			return path;
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
	}
}
