
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <stdio.h>

using namespace Elastos;

class C1
{
public:
    C1():mRef(0){}

    ~C1()
    {
        printf("==== call ~C1() ====\n");
    }

    UInt32 AddRef()
    {
        atomic_inc(&mRef);
        printf("==== call AddRef, mRef: %d ====\n", mRef);

        return mRef;
    }

    UInt32 Release()
    {
        atomic_dec(&mRef);
        printf("==== call Release, mRef: %d ====\n", mRef);
        if (0 == mRef) {
            delete this;
            return 0;
        }
        return mRef;
    }

private:
    Int32 mRef;
};

C1* f1()
{
    printf("==== enter f1 ====\n");
    AutoPtr<C1> v1 = new C1();
    printf("==== return from f1 ====\n");
    return v1.Get();
}

AutoPtr<C1> f2()
{
    printf("==== enter f2 ====\n");
    AutoPtr<C1> v1 = new C1();
    printf("==== return from f2 ====\n");
    return v1;
}

void testAutoPtr()
{
    printf("==== call f1 ====\n");
    AutoPtr<C1> v = f1();
    printf("==== call f1 end ====\n");
    printf("==== call f2 ====\n");
    AutoPtr<C1> vv = f2();
    printf("==== call f2 end ====\n");
    printf("==== call f2 again ====\n");
    C1* vvv = f2();
    printf("==== ============ ====\n");
    vvv->AddRef();
    printf("==== call f2 end ====\n");
}

int main(int argc, char *argv[])
{
    printf("==== call testAutoPtr ====\n");
    testAutoPtr();
    printf("==== call testAutoPtr end ====\n");

    return 0;
}
