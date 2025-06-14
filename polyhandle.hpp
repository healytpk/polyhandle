#pragma once

#include <cstdint>        // uintptr_t
#include <memory>         // addressof
#include <type_traits>    // is_polymorphic, remove_cvref
#include <typeinfo>       // type_info

#if !defined(_MSC_VER)

namespace std {
class polyhandle final {
    void *p;
public:
    template<class Tref> requires is_polymorphic_v< remove_cvref_t<Tref> >
    constexpr polyhandle(Tref &&obj) noexcept
      : p( const_cast< remove_cvref_t<Tref> * >(addressof(obj)) ) {}

    constexpr void *object(void) const noexcept { return this->p; }

    constexpr void *most_derived(void) const noexcept
    {
        uintptr_t const *const ptr = *static_cast<uintptr_t const* const*>( this->p );
        return static_cast<char*>(this->p) + ptr[-2];
    }

    constexpr type_info const &typeinfo(void) const noexcept
    {
        auto *const ptr = *static_cast<type_info const* const* const*>( this->p );
        return ptr[-1][0];
    }
};
}  // close namespace std

#elif defined(_MSC_VER)

#include <cstdint>    // uint32_t

extern "C" {
    // Including <Windows.h> is too much
    void *__stdcall GetModuleHandleA(char const*);
    void *__stdcall LoadLibraryA    (char const*);
    void *__stdcall GetProcAddress  (void*,char const*);
    void *__stdcall __RTCastToVoid  (void*) noexcept(false);
    void *__stdcall __RTtypeid      (void*) noexcept(false);
}

namespace std {
class polyhandle final {
    void *p;

    // We will use a thread_local variable to keep track of
    // the address that gets passed to __RTCastToVoid, because
    // this address will have been adjusted by the offset to
    // the location of the vtable pointer inside the object.
    inline static thread_local void const *argument_to_RTCTV = nullptr;
    friend void *__stdcall ::__RTCastToVoid(void*) noexcept(false);

    template<class Tref> requires is_polymorphic_v< remove_cvref_t<Tref> >
    static uint32_t GetOffsetToVftable(Tref &&obj) noexcept
    {
        typedef remove_cvref_t<Tref> T;
        T *const p = const_cast<T*>(addressof(obj));
        (void)dynamic_cast<void*>(p);
        return (char*)argument_to_RTCTV - (char*)p;
    }

public:
    template<class Tref> requires is_polymorphic_v< remove_cvref_t<Tref> >
    constexpr polyhandle(Tref &&obj) noexcept
    {
        p = (char*)addressof(obj) + GetOffsetToVftable(obj);
    }

    constexpr void *object(void) const noexcept
    {
        uint32_t const **const pvtable = *static_cast<uint32_t const***>(this->p);
        return (char*)this->p - pvtable[-1][1];
    }

    void *most_derived(void) const noexcept
    {
        return __RTCastToVoid( this->p );
    }

    type_info const &typeinfo(void) const noexcept
    {
        return *static_cast<type_info*>(  __RTtypeid(this->p)  );
    }
};
} // close namespaces std

// Here we go, we intercept calls to this function:
extern "C" void *__RTCastToVoid(void *const arg) noexcept(false)
{
    std::polyhandle::argument_to_RTCTV = arg;  // save this to do a subtraction later!

    void* hRuntime = ::GetModuleHandleA("vcruntime140.dll");
    if ( nullptr == hRuntime ) hRuntime = ::LoadLibraryA("vcruntime140.dll");
    if ( nullptr == hRuntime ) return nullptr;
    auto const fp = (void* (*)(void*)) ::GetProcAddress(hRuntime, "__RTCastToVoid");
    if ( nullptr == fp ) return nullptr;
    return fp(arg);
}

#endif
