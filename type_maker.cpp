#include "type_maker.h"

struct Test_9200
{
    int field_a;
    int flags;
    void* pointer;
    bool boolean;
    int* ppp;
    const char* conststr;
};

struct Test_9600
{
    int field_a;
    int flags;
    bool boolean;
    void* pointer;
    int* ppp;
    char* sss;
    const char* conststr;
};

int main()
{
    register_type(
        "Test",
        Test_9200,
        9200,
        &Test_9600::field_a,
        &Test_9600::flags,
        &Test_9600::pointer,
        &Test_9600::boolean,
        &Test_9600::ppp,
        &Test_9600::conststr);
        
    register_type(
        "Test",
        Test_9600,
        9600,
        &Test_9600::field_a,
        &Test_9600::flags,
        &Test_9600::pointer,
        &Test_9600::boolean,
        &Test_9600::ppp,
        &Test_9600::sss,
        &Test_9600::conststr);

    TypeMaker::GenerateSource();
}
