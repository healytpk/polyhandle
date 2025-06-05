#include <array>                    // array
#include <memory>                   // addressof
#include <type_traits>              // is_polymorphic, remove_cvref

template<class T> requires std::is_polymorphic_v< std::remove_cvref_t<T> >
__attribute__((noinline)) void *GetMostDerived(T &&obj)
{
    return dynamic_cast<void*>( std::addressof(obj) );
}

struct Base1 { int a; };               // not polymorphic
struct Base2 { virtual ~Base2(){} };   // polymorphic
struct Derived : Base1, virtual Base2 { virtual ~Derived(){} };

struct Frog { virtual ~Frog(){} };

#include <sstream>

extern decltype(auto) Func(void)
{
    Derived obj;
    Base2 &b = obj;
    Frog f;
    std::stringstream ss;
    return std::array{
            GetMostDerived(obj),
            GetMostDerived(b),
            GetMostDerived(f),
            GetMostDerived(ss),
        };
}
