#include <cstdint>                  // uint16_t
#include <array>                    // array
#include <iomanip>                  // hex, setfill, setw
#include <iostream>                 // cout, endl
#include <memory>                   // addressof
#include <sstream>                  // stringstream
#include <type_traits>              // is_polymorphic, remove_cvref

template<class T> requires std::is_polymorphic_v< std::remove_cvref_t<T> >
__attribute__((noinline, no_ptrauth)) void *GetMostDerived(T &&obj) noexcept
{
    return dynamic_cast<void*>( std::addressof(obj) );
}

struct Base1 { int a; };               // not polymorphic
struct Base2 { virtual ~Base2(){} };   // polymorphic
struct Derived : Base1, virtual Base2 { virtual ~Derived(){} };
struct Frog { virtual ~Frog(){} };

std::uint16_t ExtractSecretNumber(void const *const arg_machine_code)
{
    char unsigned const *const p = static_cast<char unsigned const*>(arg_machine_code);

    for ( unsigned i = 0u; i < 64u; i += 4u ) // Scan first 64 bytes (adjust as needed)
    {
        // Look for the MOVK instruction (AArch64) 0xF2 0xFF [aa] [bb]
        if ( 0xF2 == p[i] )
        {
            if ( 0xFF == p[i + 1] )
            {
                // Extract immediate value from instruction encoding
                return *static_cast<std::uint16_t const*>(static_cast<void const*>(&p[i + 2]));
            }
        }
    }

    return 0; // No secret number found
}

template<class T> requires std::is_polymorphic_v< std::remove_cvref_t<T> >
std::uint16_t GetSecretNumber(T &&obj) noexcept
{
    void *(*const fp)(T&) = &GetMostDerived<T&>;
    return ExtractSecretNumber( (void*)fp );
}

extern decltype(auto) Func(void)
{
    Derived obj;
    Base2 &b = obj;
    Frog f;
    std::stringstream ss;
    return std::array {
            GetSecretNumber(obj),
            GetSecretNumber(b),
            GetSecretNumber(f),
            GetSecretNumber(ss),
        };
}

extern decltype(auto) Func2(void)
{
    Derived obj;
    Base2 &b = obj;
    Frog f;
    std::stringstream ss;
    return std::array {
            GetMostDerived(obj),
            GetMostDerived(b),
            GetMostDerived(f),
            GetMostDerived(ss),
        };
}

/*
extern decltype(auto) Func3(void)
{
    Derived obj;
    Base2 &b = obj;
    Frog f;
    std::stringstream ss;
    return std::array {
            &typeid(obj),
            &typeid(b),
            &typeid(f),
            &typeid(ss),
        };
}
*/

int main(void)
{
    auto myarray = Func();
    for ( auto const &e : myarray )
    {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2u) << e << std::endl;
    }

    auto myarray2 = Func2();
    for ( auto const &e : myarray2 )
    {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(16u) << e << std::endl;
    }

/*
    auto myarray3 = Func3();
    for ( auto const &e : myarray3 )
    {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(16u) << e << std::endl;
    }
*/
}

