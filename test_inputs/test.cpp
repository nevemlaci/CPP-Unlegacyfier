#include <functional>
#include <vector>

typedef unsigned char u8;
typedef int (*FuncPtr)(double);

typedef struct {
    int x;
} UnnamedStruct;

// change to enum class
enum Color { Red, Green, Blue };
enum class AlreadyModern { One, Two };

template <typename F> void foo(const F& f) {}

struct Foo {
    virtual void foo() {}
};

struct Bar : Foo {
    // add missing override
    virtual void foo() {}
    // leave as is
    virtual void bar() {}
};

struct Baz : Foo {
    // add missing override
    virtual void foo() = 0;
};

int main() {
    std::vector<int> v{};
    auto vp = &v;
    // repalce with std::begin(*vp)
    (void)vp->begin();
    // replace with std::greater<>()
    foo(std::greater<int>());
    Color c = Red;
    return 0;
}