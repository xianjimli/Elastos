
#include "MyThread.h"
#include <elastos/AutoPtr.h>

int main(int argc, char *argv[])
{
    AutoPtr<MyThread> t = new MyThread();
    return 0;
}