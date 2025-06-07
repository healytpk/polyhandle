#include <iostream>              // cout, endl
#include "polyhandle.hpp"

using std::cout, std::endl, std::polyhandle;

struct Base1 { int a; };               // not polymorphic
struct Base2 { virtual ~Base2(){} };   // polymorphic
struct Derived : Base1, virtual Base2 { virtual ~Derived(){} };

struct Base1z { int a[600]; };           // not polymorphic
struct Base2z { virtual ~Base2z(){} };   // polymorphic
struct Derivedz : Base1z, virtual Base2z { virtual ~Derivedz(){} };

template<class T> requires std::is_polymorphic_v<T>
void PrintOut(void)
{
    T obj;
    std::cout << (void*)&obj << std::endl;

    std::polyhandle p(obj);
    std::cout << *(void**)&p << std::endl;

    std::cout << p.object() << std::endl;

    std::cout << p.typeinfo().name() << std::endl;
    std::cout << "================================\n";
}

int main(void)
{
    PrintOut<Base2>();
    PrintOut<Base2z>();
    PrintOut<Derived>();
    PrintOut<Derivedz>();
}
