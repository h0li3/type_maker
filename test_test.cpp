#include <iostream>
#include "test_test.h"

struct Test
{
    int field_a;
    unsigned flags;
    void* pointer;
    bool boolean;
    int* ppp;
    char* sss;
    const char* conststr;
};

void init_test(char* buf)
{
    auto* test = (Test*)buf;
    test->field_a = 32;
    test->flags = 0x40000;
    test->pointer = (void*)0xfff832332a;
    test->boolean = true;
    test->sss = nullptr;
    test->conststr = "foobar!";
}

void load_test(int bn, const void* buf, size_t length)
{
    const TypeDesc_Test* desc = nullptr;

    for (int i = 0; i < sizeof(__gtd_Test) / sizeof(TypeDesc_Test); ++i) {
        if (bn <= __gtd_Test[i].build_number) {
            desc = &__gtd_Test[i];
        }
    }

    std::cout << "BN: " << std::dec << bn << '\n';

    if (!desc) {
        std::cerr << " [-] Unsupported version " << bn << "!" << '\n';
        return;
    }

    char* p = (char*)buf;
    int field_a = *(int*)(p + desc->off_field_a);
    const char* conststr = *(const char**)(p + desc->off_conststr);

    std::cout << " field_a=" << field_a << '\n'
              << " conststr=" << conststr << '\n';
}

int main()
{
    char buf[256];
    init_test(buf);
    load_test(9200, buf, 256);
    load_test(9600, buf, 256);
    load_test(22621, buf, 256);
}
