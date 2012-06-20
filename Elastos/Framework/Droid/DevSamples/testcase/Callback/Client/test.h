
#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tstprintf.h>

class CTest {

public:
    void Init() {
        m_nCount = 0;
        for (int i = 0; i < 5; i++) {
            m_apIObj[i] = NULL;
        }
    }

    void Uninit() {
        for (int i = 0; i < 5; i++) {
            if (m_apIObj[i]) {
                m_apIObj[i]->Release();
            }
        }
    }

    CARAPI SubRoutine(void *pArg);
    CARAPI OnEnter(PInterface pSender, String wstr);
    CARAPI OnTrigger1(PInterface pSender);
    CARAPI OnTrigger2(PInterface pSender);
    CARAPI OnTrigger3(PInterface pSender);
    CARAPI OnTrigger4(PInterface pSender);
    CARAPI OnTrigger5(PInterface pSender);
    CARAPI OnTrigger6(PInterface pSender);
    CARAPI OnTrigger7(PInterface pSender);
    CARAPI OnTrigger8(PInterface pSender);
    CARAPI OnTrigger9(PInterface pSender);
    CARAPI OnTrigger10(PInterface pSender);
    CARAPI OnTrigger11(PInterface pSender);
    CARAPI OnTrigger12(PInterface pSender);
    CARAPI OnTrigger13(PInterface pSender);
    CARAPI OnTrigger14(PInterface pSender);
    CARAPI OnTrigger15(PInterface pSender);
    CARAPI OnTrigger16(PInterface pSender);
    CARAPI OnTriggerA(PInterface pSender, int n);
    CARAPI OnTriggerB(PInterface pSender, int n);
    CARAPI OnTriggerC(PInterface pSender, int n);
    int __stdcall Test1(int argc, char *argv[]);
    int __stdcall Test2(int argc, char *argv[]);
    int __stdcall Test3(int argc, char *argv[]);
    int __stdcall Test4(int argc, char *argv[]);
    int __stdcall Test5(int argc, char *argv[]);
    int __stdcall Test6(int argc, char *argv[]);
    int __stdcall Test7(int argc, char *argv[]);
    int __stdcall Test8(int argc, char *argv[]);
    int __stdcall Test9(int argc, char *argv[]);
    int __stdcall Test10(int argc, char *argv[]);
    int __stdcall Test11(int argc, char *argv[]);
    int __stdcall Test12(int argc, char *argv[]);
    int __stdcall Test13(int argc, char *argv[]);
    int __stdcall Test14(int argc, char *argv[]);
    int __stdcall Test15(int argc, char *argv[]);
    int __stdcall Test16(int argc, char *argv[]);
    int __stdcall Test17(int argc, char *argv[]);
    int __stdcall Test18(int argc, char *argv[]);
    int __stdcall Test19(int argc, char *argv[]);
    int __stdcall Test20(int argc, char *argv[]);
    int __stdcall Test21(int argc, char *argv[]);
    int __stdcall Test22(int argc, char *argv[]);
    int __stdcall Test23(int argc, char *argv[]);
    int __stdcall Test24(int argc, char *argv[]);
    int __stdcall Test25(int argc, char *argv[]);
    int __stdcall Test26(int argc, char *argv[]);
    int __stdcall Test27(int argc, char *argv[]);
    int __stdcall Test28(int argc, char *argv[]);
    int __stdcall Test29(int argc, char *argv[]);
    int __stdcall Test30(int argc, char *argv[]);

private:
    int m_nCount;
    IInterface* m_apIObj[5];
};

#endif /*__TEST_H__*/
