#include <elastos.h>
#include <elastos/AutoFree.h>
#include <eltypes.h>
#include <stdio.h>

using namespace Elastos;

//============== test raw pointer
ArrayOf<Int32> * f1() {
	printf("==== enter f1 ====\n");
	ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);
	printf("==== return from f1 ====\n");
	return v1;
}

void f1Test()
{
    printf("\n==== call f1 ====\n");
    AutoFree<ArrayOf<Int32> > v = f1();
    printf("==== call f1 end ====\n");
}

//============== test AutoFree on stack
void f2() {
	printf("==== enter f2 ====\n");
	ArrayOf_<Int32, 10> v2;
	AutoFree<ArrayOf<Int32> > v = &v2;
	printf("==== return from f2 ====\n");
}

void f2Test()
{
    printf("\n==== call f2 ====\n");
    f2();
    printf("==== call f2 end ====\n");
}

//============== test return AutoFree on heap
AutoFree<ArrayOf<Int32> > f3() {
	printf("==== enter f3 ====\n");
	AutoFree<ArrayOf<Int32> > v1 = ArrayOf<Int32>::Alloc(10);
	printf("==== return from f3 ====\n");
	return v1;
}

void f3Test()
{
    printf("\n==== call f3 ====\n");
    AutoFree<ArrayOf<Int32> > v3 = f3();
    printf("==== call f3 end ====\n");
}

//============== test assign AutoFree on heap
AutoFree<ArrayOf<Int32> > f4() {
	printf("==== enter f4 ====\n");
	ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);
	ArrayOf<Int32> * v2 = v1->Assign();
	assert(v1 == v2 && "v1 should equal v2 when use Assign.");
	ArrayOf<Int32>::Free(v1);
	printf("==== return from f4 ====\n");
	return v2;
}

void f4Test()
{
    printf("\n==== call f4 ====\n");
    AutoFree<ArrayOf<Int32> > v4 = f4();
    printf("==== call f4 end ====\n");
}

//============== test clone AutoFree on stack
AutoFree<ArrayOf<Int32> > f5() {
    printf("==== enter f5 ====\n");
    ArrayOf_<Int32, 10> v1;
    // Notes: Use Clone to create a new object.
    // It's dangerous to use Assign here.
    ArrayOf<Int32> * v2 = ((ArrayOf<Int32> *)(&v1))->Clone();
    assert(v1 != v2 && "v1 should not equal v2 when use Clone.");
    printf("==== return from f5 ====\n");
    return v2;
}

void f5Test()
{
    printf("\n==== call f5 ====\n");
    AutoFree<ArrayOf<Int32> > v5 = f5();
    printf("==== call f5 end ====\n");
}

void testArrayOf()
{
    f1Test();
    f2Test();
    f3Test();
    f4Test();
    f5Test();
}

int main(int argc, char *argv[]) {
	printf("==== call testAutoFree ====\n");
	testArrayOf();
	printf("\n==== call testAutoFree end ====\n");

	return 0;
}
