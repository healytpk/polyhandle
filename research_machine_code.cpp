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

extern std::array<void*, 2u> Func(void)
{
    Derived obj;
    Base2 &b = obj;
    return { GetMostDerived(obj), GetMostDerived(b) };
}
