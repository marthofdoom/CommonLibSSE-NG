#pragma once

#include "RE/B/BSAtomic.h"
#include "RE/B/BSExtraData.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/E/ExtraDataTypes.h"
#include "RE/E/ExtraFlags.h"
#include "RE/E/ExtraLevCreaModifier.h"
#include "RE/F/FormTypes.h"
#include "RE/M/MemoryManager.h"
#include "RE/S/SoulLevels.h"

namespace RE
{
	class InventoryChanges;
	class TESBoundObject;

	class BaseExtraList
	{
	public:
		struct PresenceBitfield
		{
		public:
			[[nodiscard]] bool HasType(std::uint32_t a_type) const;
			void               MarkType(std::uint32_t a_type, bool a_cleared);

			// members
			std::uint8_t bits[0x18];  // 00
		};
		static_assert(sizeof(PresenceBitfield) == 0x18);

		[[nodiscard]] BSExtraData*& GetData() noexcept;

		[[nodiscard]] const BSExtraData*& GetData() const noexcept;

		[[nodiscard]] PresenceBitfield*& GetPresence() noexcept;

		[[nodiscard]] const PresenceBitfield*& GetPresence() const noexcept;

#ifndef ENABLE_SKYRIM_AE
		~BaseExtraList();  // 00, virtual on AE 1.6.629 and later.

		// members
		BSExtraData*      data = nullptr;      // 00, 08
		PresenceBitfield* presence = nullptr;  // 08, 10
#endif
	};
#ifndef ENABLE_SKYRIM_AE
	static_assert(sizeof(BaseExtraList) == 0x10);
#endif

	class ExtraDataList
	{
	public:
		template <class T>
		class iterator_base
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			constexpr iterator_base() noexcept :
				_cur(nullptr)
			{}

			constexpr iterator_base(pointer a_node) noexcept :
				_cur(a_node)
			{}

			constexpr iterator_base(const iterator_base& a_rhs) noexcept :
				_cur(a_rhs._cur)
			{}

			constexpr iterator_base(iterator_base&& a_rhs) noexcept :
				_cur(std::move(a_rhs._cur))
			{
				a_rhs._cur = nullptr;
			}

			~iterator_base() = default;

			constexpr iterator_base& operator=(const iterator_base& a_rhs) noexcept
			{
				if (this != std::addressof(a_rhs)) {
					_cur = a_rhs._cur;
				}
				return *this;
			}

			constexpr iterator_base& operator=(iterator_base&& a_rhs) noexcept
			{
				if (this != std::addressof(a_rhs)) {
					_cur = a_rhs._cur;
					a_rhs._cur = nullptr;
				}
				return *this;
			}

			[[nodiscard]] constexpr reference operator*() const noexcept { return *_cur; }
			[[nodiscard]] constexpr pointer   operator->() const noexcept { return _cur; }

			[[nodiscard]] constexpr friend bool operator==(const iterator_base& a_lhs, const iterator_base& a_rhs) noexcept { return a_lhs._cur == a_rhs._cur; }
			[[nodiscard]] constexpr friend bool operator!=(const iterator_base& a_lhs, const iterator_base& a_rhs) noexcept { return !(a_lhs == a_rhs); }

			// prefix
			constexpr iterator_base& operator++() noexcept
			{
				assert(_cur != nullptr);
				_cur = _cur->next;
				return *this;
			}

			// postfix
			[[nodiscard]] constexpr iterator_base operator++(int) noexcept
			{
				iterator_base tmp{ *this };
				++(*this);
				return tmp;
			}

			inline friend void swap(const iterator_base& a_lhs, const iterator_base& a_rhs) noexcept
			{
				std::swap(a_lhs._cur, a_rhs._cur);
			}

		private:
			pointer _cur;
		};

		using iterator = iterator_base<BSExtraData>;
		using const_iterator = iterator_base<const BSExtraData>;

		// mit-3.7 (upstream PR #108): defined, it calls the engine's own constructor.
		// HEAP ONLY. The real object is bigger than sizeof(ExtraDataList) (0x10 in a
		// multi-runtime build): the engine allocates 0x18 on 1.5.97 and 0x20 on
		// 1.6.1170, and its constructor writes all of it. So `new ExtraDataList()` goes
		// through the operator new below, which allocates the exact build's size. A
		// stack or member ExtraDataList, or an array, would be overrun by the ctor.
		ExtraDataList();
		~ExtraDataList();

		// mit-3.7: the engine size of one ExtraDataList on the running build (0x18 on
		// 1.5.97.0, 0x20 on 1.6.1170.0); a named fatal error on any other build.
		[[nodiscard]] static std::size_t GetRuntimeSize() noexcept;

		[[nodiscard]] inline void* operator new(std::size_t)
		{
			const auto mem = RE::malloc(GetRuntimeSize());
			if (mem) {
				return mem;
			} else {
				stl::report_and_fail("out of memory"sv);
			}
		}
		void* operator new[](std::size_t) = delete;
		[[nodiscard]] constexpr void* operator new(std::size_t, void* a_ptr) noexcept { return a_ptr; }
		inline void operator delete(void* a_ptr) { RE::free(a_ptr); }
		void operator delete[](void*) = delete;

		iterator       begin();
		const_iterator cbegin() const;
		const_iterator begin() const;
		iterator       end();
		const_iterator cend() const;
		const_iterator end() const;

		BSExtraData*       GetByType(ExtraDataType a_type);
		const BSExtraData* GetByType(ExtraDataType a_type) const;

		template <class T>
		inline T* GetByType()
		{
			return static_cast<T*>(GetByType(T::EXTRADATATYPE));
		}

		template <class T>
		inline const T* GetByType() const
		{
			return static_cast<const T*>(GetByType(T::EXTRADATATYPE));
		}

		bool HasType(ExtraDataType a_type) const;

		template <class T>
		inline bool HasType() const
		{
			return HasType(T::EXTRADATATYPE);
		}

		bool Remove(ExtraDataType a_type, BSExtraData* a_toRemove);

		template <class T>
		inline bool Remove(T* a_toRemove)
		{
			return Remove(T::EXTRADATATYPE, a_toRemove);
		}

		bool RemoveByType(ExtraDataType a_type);

		BSExtraData*          Add(BSExtraData* a_toAdd);
		ObjectRefHandle       GetAshPileRef();
		std::int32_t          GetCount() const;
		const char*           GetDisplayName(TESBoundObject* a_baseObject);
		BGSEncounterZone*     GetEncounterZone();
		ExtraTextDisplayData* GetExtraTextDisplayData();
		TESObjectREFR*        GetLinkedRef(BGSKeyword* a_keyword);
		TESForm*              GetOwner();
		SOUL_LEVEL            GetSoulLevel() const;
		ObjectRefHandle       GetTeleportLinkedDoor();
		void                  SetCount(std::uint16_t a_count);
		void                  SetEncounterZone(BGSEncounterZone* a_zone);
		void                  SetExtraFlags(ExtraFlags::Flag a_flags, bool a_enable);
		void                  SetInventoryChanges(InventoryChanges* a_changes);
		void                  SetOwner(TESForm* a_owner);

	private:
		[[nodiscard]] BSExtraData* GetByTypeImpl(ExtraDataType a_type) const;
		void         MarkType(std::uint32_t a_type, bool a_cleared);
		void         MarkType(ExtraDataType a_type, bool a_cleared);
		[[nodiscard]] BSReadWriteLock& GetLock() const noexcept;

		// members
		BaseExtraList           _extraData;  // 00
#ifndef ENABLE_SKYRIM_AE
		mutable BSReadWriteLock _lock;       // 10, 18; offset 18 only for AE versions .629 and later.
#endif
	};
}
