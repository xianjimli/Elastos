#include <elastos.h>
#include <elastos/AutoFree.h>
#include <eltypes.h>
#include <stdio.h>

using namespace Elastos;

ArrayOf<Int32> * f1() {
	printf("==== enter f1 ====\n");
	ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);
	printf("==== return from f1 ====\n");
	return v1;
}

void f2() {
	printf("==== enter f2 ====\n");
	ArrayOf_<Int32, 10> v2;
	AutoFree<ArrayOf<Int32> > v = &v2;
	printf("==== return from f2 ====\n");
}

AutoFree<ArrayOf<Int32> > f3() {
	printf("==== enter f3 ====\n");
	AutoFree<ArrayOf<Int32> > v1 = ArrayOf<Int32>::Alloc(10);
	printf("==== return from f3 ====\n");
	return v1;
}

AutoFree<ArrayOf<Int32> > f4() {
	printf("==== enter f4 ====\n");
	ArrayOf<Int32> * v1 = ArrayOf<Int32>::Alloc(10);
	ArrayOf<Int32> * v2 = v1->Assign();
	ArrayOf<Int32>::Free(v1);
	printf("==== return from f4 ====\n");
	return v2;
}

void testArrayOf() {
	printf("==== call f1 ====\n");
	AutoFree<ArrayOf<Int32> > v = f1();
	printf("==== call f1 end ====\n");

	printf("==== call f2 ====\n");
	f2();
	printf("==== call f2 end ====\n");

	printf("==== call f3 ====\n");
	AutoFree<ArrayOf<Int32> > v3 = f3();
	printf("==== call f3 end ====\n");

	printf("==== call f4 ====\n");
	AutoFree<ArrayOf<Int32> > v4 = f4();
	printf("==== call f4 end ====\n");
}

int main(int argc, char *argv[]) {
	printf("==== call testAutoFree ====\n");
	testArrayOf();
	printf("==== call testAutoFree end ====\n");

	return 0;
}
