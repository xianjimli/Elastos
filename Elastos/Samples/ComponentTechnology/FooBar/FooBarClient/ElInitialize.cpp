

#include <elastos.h>

using namespace Elastos;

static IInterface *m_pCallbackContext;
static IInterface* pOrgCallbackContext;

ECode ElInitialize()
{
    void* r4;
    void* callerRet;
    void* callerIp;
    void* callerFp;
    void* callerCallerRet;
    void* callerCallerIp;
    void* callerCallerFp;
    UInt32 ssize;


    //save caller's fp
    asm("ldr r6, [fp, #-12]");
    asm("mov %[result], r6"
            : [result]"=r" (callerFp));
    //save caller's ip
    asm("ldr r6, [fp, #-8]");
    asm("mov %[result], r6"
            : [result]"=r" (callerIp));
    //save caller's Ret
    asm("ldr r6, [fp, #-4]");
    asm("mov %[result], r6"
            : [result]"=r" (callerRet));
    ssize = (UInt32)callerFp - (UInt32)callerIp;

    //save caller's caller's lr, ip, fp registers
    asm("ldr r6, [fp, #-12]");  //get caller's fp
    //save caller's caller's fp
    asm("ldr r5, [r6, #-12]");
    asm("mov %[result], r5"
            : [result]"=r" (callerCallerFp));
    //save caller's caller's ip
    asm("ldr r5, [r6, #-8]");
    asm("mov %[result], r5"
            : [result]"=r" (callerCallerIp));
    //save caller's caller's ret
    asm("ldr r5, [r6, #-4]");
    asm("mov %[result], r5"
            : [result]"=r" (callerCallerRet));


    _Impl_CallbackSink_InitCallbackContext(&m_pCallbackContext);
    pOrgCallbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    if (NULL != pOrgCallbackContext) {
        pOrgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, m_pCallbackContext);

    //save r4. r4 register will be used to get m_pCallbackContext
    //so r4 should be saved here!
    asm("mov %[result], r4"
            : [result]"=r" (r4));

    // move caller's frame to the top of the stack
    asm("ldr r7, [fp, #-8]");  //get caller's ip
    asm("mov r5, sp");
    asm("again:");
    asm("ldr r3, [r6]");
    asm("push {r3}");
    asm("sub r6, r6, #4");
    asm("cmp r7, r6");
    asm("bne again");
    asm("ldr r3, [r6]");
    asm("push {r3}");

    //change my fp, sp and lr to caller's caller's fp, sp and lr
    asm("mov r6, %[result]"
            :: [result]"r" (callerCallerFp));
    asm("str r6, [fp, #-12]");
    asm("mov r6, %[result]"
            :: [result]"r" (callerCallerIp));
    asm("str r6, [fp, #-8]");
    asm("mov r6, %[result]"
            :: [result]"r" (callerCallerRet));
    asm("str r6, [fp, #-4]");

    //change caller's fp, ip
    asm("str fp, [r5, #-16]");  //change caller's fp
    asm("str sp, [r5, #-12]");   //change caller's ip
    asm("mov r7, pc");
    asm("add r7, pc, #32");
    asm("str r7, [r5, #-8]");   //change caller's lr

    asm("mov r3, %[value]"
         :: [value]"r" (ssize));
    asm("sub r5, r5, #4");
    asm("sub sp, r5, r3");
    asm("mov r3, %[value]"
         :: [value]"r" (callerRet));
    asm("mov fp, r5");          //change fp
    asm("mov pc, r3");          //jump to caller

    //restore r4 register
    asm("mov r4, %[value]"
         :: [value]"r" (r4));


    _Impl_CallbackSink_TryToHandleEvents(m_pCallbackContext);

    //return to caller's caller
    return NOERROR;
}

void RequestToFinish()
{
    _Impl_CallbackSink_RequestToFinish(m_pCallbackContext, CallbackContextFinish_Nice);
}
