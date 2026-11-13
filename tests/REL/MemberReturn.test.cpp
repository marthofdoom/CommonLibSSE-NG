#include "catch2/catch_all.hpp"

#include "REL/REL.h"
#include "SKSE/SKSE.h"

// mit-3.7: REL::RelocateVirtual and REL::invoke call REAL member functions built by this compiler, so these tests
// fail (wrong value, a changed `this`, or a crash) if either one takes the free-function road for a class return.
// The engine is built by the same compiler family, so what holds here holds for an engine virtual.
namespace
{
	struct Pod4
	{
		std::uint32_t value;
	};
	static_assert(REL::detail::is_x64_pod_v<Pod4>);  // a FREE function returns this in RAX; a member function does not

	struct Pod8
	{
		void* value;
	};

	union Union4
	{
		std::uint32_t integer;
		float         real;
	};

	struct Handle  // the shape of RE::BSPointerHandle: 4 bytes, constructors
	{
		Handle() = default;
		explicit Handle(std::uint32_t a_value) :
			value(a_value) {}

		std::uint32_t value{ 0 };
	};

	struct Point3  // the shape of RE::NiPoint3
	{
		float x, y, z;
	};

	struct Counted  // the shape of a counted pointer: every copy, move and destruction is seen
	{
		explicit Counted(int* a_live) :
			live(a_live) { ++*live; }
		Counted(const Counted& a_rhs) :
			live(a_rhs.live) { ++*live; }
		Counted(Counted&& a_rhs) noexcept :
			live(a_rhs.live) { ++*live; }
		~Counted() { --*live; }

		int* live;
	};

	enum class Kind : std::uint32_t
	{
		kNone = 0,
		kSome = 0xABCD1234
	};

	struct Target
	{
		virtual ~Target() = default;                                                             // 00
		virtual Pod4          GetPod4() const { return { marker }; }                             // 01
		virtual Pod8          GetPod8() { return { this }; }                                     // 02
		virtual Union4        GetUnion4() const { return { marker }; }                           // 03
		virtual Handle        GetHandle() const { return Handle{ marker }; }                     // 04
		virtual Point3        GetPoint3(float a_scale) const { return { a_scale, 2 * a_scale, 3 * a_scale }; }  // 05
		virtual Counted       GetCounted(int* a_live) { return Counted{ a_live }; }              // 06
		virtual Kind          GetKind() const { return Kind::kSome; }                            // 07
		virtual std::uint32_t GetMarker() const { return marker; }                               // 08
		virtual const Point3& GetPointRef() const { return point; }                              // 09
		virtual void          SetMarker(std::uint32_t a_marker) { marker = a_marker; }           // 0A
		virtual Handle        Combine(std::uint32_t a_a, const Point3* a_b, int a_c, float a_d, std::uint64_t a_e) const  // 0B
		{
			return Handle{ marker + a_a + static_cast<std::uint32_t>(a_b->z) + static_cast<std::uint32_t>(a_c) +
						   static_cast<std::uint32_t>(a_d) + static_cast<std::uint32_t>(a_e) };
		}

		Handle NonVirtualHandle(std::uint32_t a_add) const { return Handle{ marker + a_add }; }
		Pod4   NonVirtualPod4() const { return { marker }; }
		Kind   NonVirtualKind() const { return Kind::kSome; }

		std::uint32_t marker{ 0x600DF00D };
		Point3        point{ 1.0F, 2.0F, 3.0F };
	};

	// What the rest of the object must still hold after every call: the bug wrote the result over the vptr.
	struct Witness
	{
		explicit Witness(const Target& a_target) :
			target(a_target),
			vptr(*reinterpret_cast<const std::uintptr_t*>(std::addressof(a_target)))
		{}

		[[nodiscard]] bool Intact() const
		{
			return *reinterpret_cast<const std::uintptr_t*>(std::addressof(target)) == vptr &&
			       target.marker == 0x600DF00D;
		}

		const Target&  target;
		std::uintptr_t vptr;
	};
}

static_assert(REL::detail::member_returns_in_register_v<void>);
static_assert(REL::detail::member_returns_in_register_v<bool>);
static_assert(REL::detail::member_returns_in_register_v<float>);
static_assert(REL::detail::member_returns_in_register_v<Kind>);
static_assert(REL::detail::member_returns_in_register_v<Target*>);
static_assert(REL::detail::member_returns_in_register_v<const Point3&>);
static_assert(!REL::detail::member_returns_in_register_v<Pod4>);
static_assert(!REL::detail::member_returns_in_register_v<Union4>);
static_assert(!REL::detail::member_returns_in_register_v<Handle>);
static_assert(REL::detail::member_returns_through_slot_v<Pod4>);
static_assert(REL::detail::member_returns_through_slot_v<Pod8>);
static_assert(REL::detail::member_returns_through_slot_v<Union4>);
static_assert(REL::detail::member_returns_through_slot_v<Handle>);
static_assert(REL::detail::member_returns_through_slot_v<Point3>);
static_assert(REL::detail::member_returns_through_slot_v<Counted>);

TEST_CASE("RelocateVirtual/MemberReturnConvention")
{
	// Each runtime kind takes its own index argument: SE and AE the first, VR the second. The unused one is a slot
	// that would return something else.
	const auto runtime = GENERATE(REL::Module::Runtime::SE, REL::Module::Runtime::AE, REL::Module::Runtime::VR);
	switch (runtime) {
		case REL::Module::Runtime::SE:
			REQUIRE(REL::Module::mock(SKSE::RUNTIME_SSE_1_5_97, runtime, L"SkyrimSE.exe", 0x1000));
			break;
		case REL::Module::Runtime::AE:
			REQUIRE(REL::Module::mock(SKSE::RUNTIME_SSE_1_6_353, runtime, L"SkyrimSE.exe", 0x1000));
			break;
		default:
			REQUIRE(REL::Module::mock(SKSE::RUNTIME_VR_1_4_15, runtime, L"SkyrimVR.exe", 0x1000));
			break;
	}
	const bool vr = runtime == REL::Module::Runtime::VR;
	const auto index = [vr](std::ptrdiff_t a_index) {
		return std::pair{ vr ? std::ptrdiff_t{ 8 } : a_index, vr ? a_index : std::ptrdiff_t{ 8 } };
	};

	Target        target;
	const Witness witness{ target };

	SECTION("a 4-byte trivial struct comes back through the slot")
	{
		const auto [se, vrIndex] = index(1);
		const auto result = REL::RelocateVirtual<decltype(&Target::GetPod4)>(se, vrIndex, &target);
		CHECK(result.value == 0x600DF00D);
		CHECK(witness.Intact());
	}
	SECTION("an 8-byte trivial struct")
	{
		const auto [se, vrIndex] = index(2);
		const auto result = REL::RelocateVirtual<decltype(&Target::GetPod8)>(se, vrIndex, &target);
		CHECK(result.value == &target);
		CHECK(witness.Intact());
	}
	SECTION("a union")
	{
		const auto [se, vrIndex] = index(3);
		const auto result = REL::RelocateVirtual<decltype(&Target::GetUnion4)>(se, vrIndex, &target);
		CHECK(result.integer == 0x600DF00D);
		CHECK(witness.Intact());
	}
	SECTION("a handle: the Actor::QLastRiddenMount shape")
	{
		const auto [se, vrIndex] = index(4);
		const auto result = REL::RelocateVirtual<decltype(&Target::GetHandle)>(se, vrIndex, &target);
		CHECK(result.value == 0x600DF00D);
		CHECK(witness.Intact());
	}
	SECTION("a 12-byte struct with an argument")
	{
		const auto [se, vrIndex] = index(5);
		const auto result = REL::RelocateVirtual<decltype(&Target::GetPoint3)>(se, vrIndex, &target, 2.0F);
		CHECK(result.x == 2.0F);
		CHECK(result.y == 4.0F);
		CHECK(result.z == 6.0F);
		CHECK(witness.Intact());
	}
	SECTION("a counted object keeps an exact count")
	{
		const auto [se, vrIndex] = index(6);
		int        live = 0;
		{
			const auto result = REL::RelocateVirtual<decltype(&Target::GetCounted)>(se, vrIndex, &target, &live);
			CHECK(live == 1);
			CHECK(result.live == &live);
		}
		CHECK(live == 0);
		CHECK(witness.Intact());
	}
	SECTION("five arguments after the slot: the Actor::DropObject shape")
	{
		const auto [se, vrIndex] = index(0xB);
		const Point3 point{ 0.0F, 0.0F, 7.0F };
		const auto   result = REL::RelocateVirtual<decltype(&Target::Combine)>(
			  se, vrIndex, &target, 1U, &point, 2, 3.0F, std::uint64_t{ 4 });
		CHECK(result.value == 0x600DF00D + 1 + 7 + 2 + 3 + 4);
		CHECK(witness.Intact());
	}
	SECTION("the free-function form of the signature takes the same road")
	{
		const auto [se, vrIndex] = index(4);
		const auto result = REL::RelocateVirtual<Handle(const Target*)>(se, vrIndex, &target);
		CHECK(result.value == 0x600DF00D);
		CHECK(witness.Intact());
	}
	SECTION("register returns are unchanged")
	{
		{
			const auto [se, vrIndex] = index(7);
			CHECK(REL::RelocateVirtual<decltype(&Target::GetKind)>(se, vrIndex, &target) == Kind::kSome);
		}
		{
			const auto [se, vrIndex] = index(8);
			CHECK(REL::RelocateVirtual<decltype(&Target::GetMarker)>(se, vrIndex, &target) == 0x600DF00D);
		}
		{
			const auto [se, vrIndex] = index(9);
			CHECK(&REL::RelocateVirtual<decltype(&Target::GetPointRef)>(se, vrIndex, &target) == &target.point);
		}
		CHECK(witness.Intact());
		{
			const auto [se, vrIndex] = index(0xA);
			REL::RelocateVirtual<decltype(&Target::SetMarker)>(se, vrIndex, &target, 5U);
			CHECK(target.marker == 5);
		}
	}

	REL::Module::reset();
}

TEST_CASE("Invoke/MemberReturnConvention")
{
	Target        target;
	const Witness witness{ target };

	SECTION("a member function pointer returning a handle")
	{
		const auto result = REL::invoke(&Target::NonVirtualHandle, &target, 2U);
		CHECK(result.value == 0x600DF00D + 2);
		CHECK(witness.Intact());
	}
	SECTION("a member function pointer returning a 4-byte trivial struct")
	{
		// 3.7.0 called this one as a free function: the result came back as whatever was in RAX.
		const auto result = REL::invoke(&Target::NonVirtualPod4, &target);
		CHECK(result.value == 0x600DF00D);
		CHECK(witness.Intact());
	}
	SECTION("a member function pointer returning an enum")
	{
		CHECK(REL::invoke(&Target::NonVirtualKind, &target) == Kind::kSome);
		CHECK(witness.Intact());
	}
	SECTION("REL::Relocation of a member function type")
	{
		// The address of a non-virtual member function, the way a Relocation holds an engine address.
		const auto                                             address = SKSE::stl::unrestricted_cast<std::uintptr_t>(&Target::NonVirtualHandle);
		const REL::Relocation<decltype(&Target::NonVirtualHandle)> func{ address };
		const auto                                             result = func(&target, 3U);
		CHECK(result.value == 0x600DF00D + 3);
		CHECK(witness.Intact());
	}
}
