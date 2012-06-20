/*
 * 函数原型:
 *
 * 参数说明:
 *
 */

#include "carcallback01.h"
#include "carcallback02.h"
#include "carcallback03.h"
#include "carcallback04.h"
#include "carcallback05.h"
#include "carcallback06.h"
#include "carcallback07.h"
#include "carcallback09.h"
#include "carcallback10.h"
#include "carcallback12.h"
#include "carcallback13.h"

#include "test.h"
#include "elastos.h"
#include "limits.h"
//#include "CharacterSetTest.h"

ECode CTest::OnEnter(PInterface pSender, String wstr)
{
    printf("%S\n", (const char*)wstr);
    CThread::Sleep(150, NULL);
    return NOERROR;
}

ECode CTest::OnTrigger1(PInterface pSender)
{
    m_nCount += 0x00000001;
    return NOERROR;
}

ECode CTest::OnTrigger2(PInterface pSender)
{
    m_nCount += 0x00000010;
    return NOERROR;
}

ECode CTest::OnTrigger3(PInterface pSender)
{
    m_nCount += 0x00000100;
    return NOERROR;
}

ECode CTest::OnTrigger4(PInterface pSender)
{
    m_nCount += 0x00001000;
    return NOERROR;
}

ECode CTest::OnTrigger5(PInterface pSender)
{
    m_nCount += 0x00010000;
    return NOERROR;
}

ECode CTest::OnTrigger6(PInterface pSender)
{
    m_nCount += 0x00100000;
    return NOERROR;
}

ECode CTest::OnTrigger7(PInterface pSender)
{
    m_nCount += 0x01000000;
    return NOERROR;
}

ECode CTest::OnTrigger8(PInterface pSender)
{
    m_nCount += 0x00000002;
    return NOERROR;
}

ECode CTest::OnTrigger9(PInterface pSender)
{
    m_nCount += 0x00000020;
    return NOERROR;
}

ECode CTest::OnTrigger10(PInterface pSender)
{
    m_nCount += 0x00000200;
    return NOERROR;
}

ECode CTest::OnTrigger11(PInterface pSender)
{
    m_nCount += 0x00002000;
    return NOERROR;
}

ECode CTest::OnTrigger12(PInterface pSender)
{
    m_nCount += 0x00020000;
    return NOERROR;
}

ECode CTest::OnTrigger13(PInterface pSender)
{
    m_nCount += 0x00200000;
    return NOERROR;
}

ECode CTest::OnTrigger14(PInterface pSender)
{
    m_nCount++;
    return NOERROR;
}

ECode CTest::OnTrigger15(PInterface pSender)
{
    m_nCount += 2;
    return NOERROR;
}

ECode CTest::OnTrigger16(PInterface pSender)
{
    m_nCount *= 2;
    return NOERROR;
}

#include "test.h"

typedef enum MethodFlag {
    MethodFlag_OnTriggerA = 1,
    MethodFlag_OnTriggerB,
    MethodFlag_OnTriggerC,
    MethodFlag_CTest_OnTriggerA,
    MethodFlag_CTest_OnTriggerB,
    MethodFlag_CTest_OnTriggerC,
    MethodFlag_OnTrigger
} MethodFlag;

CTest*          g_pCTest;
IInterface *       g_pObj;
int             g_n;
MethodFlag      g_flag;
int             g_count;

FILE *          g_pFout;

ECode OnTrigger(void* pUserData, PInterface pObject,
    int a, String c, const BufferOf<Byte> & ebbBuf)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    g_flag = MethodFlag_OnTrigger;
    g_pObj = pSender;
    g_n += a;
    g_count = 0;

    if (a == 41) {
        if (strcmp(c, "CCBServer::Trigger4") != 0 ||
            strcmp((char*)ebbBuf.GetPayload(), "ByteArray_in") != 0) {
                printf("OnTrigger ERROR!\n");
                printf("OnTrigger: a:%d, c:%s\n", a, (const char*)c);
                printf("ebbBuf:%s\n", (char*)ebbBuf.GetPayload());
                g_count = 1;
        }
    }
    else if (a == 51) {
        if (strcmp(c, "CCBServer::Trigger5") != 0) {// ||
            //ebbBuf != NULL_ARRAYOF(Byte)) {
                printf("OnTrigger ERROR!\n");
                printf("OnTrigger: a:%d, c:%s\n", a, (const char*)c);
                //printf("ebbBuf:%s\n", (char*)(Byte*)ebbBuf);
                g_count = 2;
        }
    }
    else {
        printf("OnTrigger ERROR!\n");
        printf("OnTrigger: a:%d, c:%S\n", a, (const char*)c);
        printf("ebbBuf:%s\n", (char*)ebbBuf.GetPayload());
        g_count = 3;
    }

    return NOERROR;
}

ECode OnTriggerA(void* pUserData, PInterface pObject, int n)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    g_flag = MethodFlag_OnTriggerA;
    g_pObj = pSender;
    g_n = n;
    InterlockedIncrement((Int32*)&g_count);

//    printf("::OnTriggerA, pSender = %x; n = %d\n", pSender, n);
    return NOERROR;
}

ECode OnTriggerB(void* pUserData, PInterface pObject, int n)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    g_flag = MethodFlag_OnTriggerB;
    g_pObj = pSender;
    g_n = n;
    InterlockedIncrement((Int32*)&g_count);

//    printf("::OnTriggerB, pSender = %x; n = %d\n", pSender, n);
    return NOERROR;
}

ECode OnTriggerC(void* pUserData, PInterface pObject, int n)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    g_flag = MethodFlag_OnTriggerC;
    g_pObj = pSender;
    g_n = n;
    InterlockedIncrement((Int32*)&g_count);

//    printf("::OnTriggerC, pSender = %x; n = %d\n", pSender, n);
    return NOERROR;
}

ECode CTest::OnTriggerA(PInterface pObject, int n)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    g_flag = MethodFlag_CTest_OnTriggerA;
    g_pObj = pSender;
    g_n = n;
    InterlockedIncrement((Int32*)&g_count);

//    printf("CTest::OnTriggerA, pSender = %x; n = %d\n", pSender, n);
    return NOERROR;
}

ECode CTest::OnTriggerB(PInterface pObject, int n)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    g_flag = MethodFlag_CTest_OnTriggerB;
    g_pObj = pSender;
    g_n = n;
    InterlockedIncrement((Int32*)&g_count);

//    printf("CTest::OnTriggerB, pSender = %x; n = %d\n", pSender, n);
    return NOERROR;
}

ECode CTest::OnTriggerC(PInterface pObject, int n)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    g_flag = MethodFlag_CTest_OnTriggerC;
    g_pObj = pSender;
    g_n = n;
    InterlockedIncrement((Int32*)&g_count);

//    printf("CTest::OnTriggerC, pSender = %x; n = %d\n", pSender, n);
    return NOERROR;
}

ECode OnTriggerD(void* pUserData, PInterface pObject, int n)
{
    ICBServer* pSender = ICBServer::Probe(pObject);
    ECode ec = NOERROR;
    QuitCode qcRet = 0;

    switch (n) {
        case 1:
            ec = CCBServer::RemoveOnTriggerBCallback(
                pSender, &::OnTriggerB);
            if (ec != NOERROR) {
                printf("RemoveOnTriggerBCallback failed. ec = %x\n", ec);
                qcRet = 1;
                goto Exit;
            }
            ec = CCBServer::RemoveOnTriggerCCallback(
                pSender, g_pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("RemoveOnTriggerCCallback failed. ec = %x\n", ec);
                qcRet = 2;
                goto Exit;
            }
            break;
        case 2:
            ec = CCBServer::AddOnTriggerCCallback(
                pSender, &::OnTriggerB);
            if (ec != NOERROR) {
                printf("AddOnTriggerCCallback failed. ec = %x\n", ec);
                qcRet = 3;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                pSender, g_pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("AddOnTriggerBCallback failed. ec = %x\n", ec);
                qcRet = 4;
                goto Exit;
            }
            break;
        case 3:
            ec = CCBServer::RemoveAllCallbacks(pSender);
            if (ec != NOERROR) {
                printf("RemoveAllCallbacks failed. ec = %x\n", ec);
                qcRet = 5;
                goto Exit;
            }
            break;
        case 4:
            ec = CCBServer::RemoveOnTriggerACallback(pSender, &::OnTriggerA);
            if (ec != NOERROR) {
                printf("RemoveOnTriggerACallback failed. ec = %x\n", ec);
                qcRet = 6;
                goto Exit;
            }
            break;
        case 5:
            ec = CCBServer::RemoveOnTriggerACallback(
                pSender, &::OnTriggerA);
            if (ec != NOERROR) {
                printf("RemoveOnTriggerACallback Error! ec = %x\n", ec);
                qcRet = 7;
                goto Exit;
            }
            break;
        case 6:
            ec = CCBServer::AddOnTriggerACallback(pSender, &::OnTriggerA);
            if (ec != NOERROR) {
                printf("AddOnTriggerACallback Error! ec = %x\n", ec);
                qcRet = 8;
                goto Exit;
            }
            break;
        default:
            break;
    }

Exit:
    return qcRet;
}

ECode OnTriggerEnum(void* pUserData,
                    PInterface pSender, TestEnum emInfo)
{
    if (TestEnum_Three != emInfo) {
        printf("OnTriggerEnum emInfo:%d!\n", emInfo);
        CThread::Quit(-1);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerStruct(void* pUserData,
                    PInterface pSender, const TestStruct & stInfo)
{
    if (stInfo.nType != 2 || stInfo.nx != 6 || stInfo.ny != 12) {
        printf("OnTriggerStruct:%d, %d, %d!\n",
            stInfo.nType, stInfo.nx, stInfo.ny);
        CThread::Quit(-2);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerByte(void* pUserData, PInterface pSender, Byte byInfo)
{
    if (byInfo != 255) {
        printf("OnTriggerByte:%x!\n", byInfo);
        CThread::Quit(-3);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInt16(void* pUserData, PInterface pSender, Int16 i16Info)
{
    if (i16Info != -32768) {
        printf("OnTriggerInt16:%x!\n", i16Info);
        CThread::Quit(-4);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInt32(void* pUserData, PInterface pSender, Int32 i32Info)
{
    if (i32Info != 2147483647) {
        printf("OnTriggerInt32:%x!\n", i32Info);
        CThread::Quit(-5);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInt64(void* pUserData, PInterface pSender, Int64 i64Info)
{
    if (i64Info != 9223372036854775807LL) {
        printf("OnTriggerInt64:%x!\n", i64Info);
        CThread::Quit(-6);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerFloat(void* pUserData, PInterface pSender, Float fInfo)
{
    if (fInfo != 1111.1111f) {
        printf("OnTriggerFloat:%x!\n", fInfo);
        CThread::Quit(-7);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerDouble(void* pUserData, PInterface pSender, Double dInfo)
{
    if (dInfo != 2222.2222) {
        printf("OnTriggerDouble:%x!\n", dInfo);
        CThread::Quit(-8);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerBoolean(void* pUserData, PInterface pSender, Boolean bInfo)
{
    if (!bInfo) {
        printf("OnTriggerBoolean:%x!\n", bInfo);
        CThread::Quit(-9);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerChar8(void* pUserData, PInterface pSender, Char8 acInfo)
{
    if (acInfo != '@') {
        printf("OnTriggerChar8:%c!\n", acInfo);
        CThread::Quit(-10);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerChar16(void* pUserData, PInterface pSender, Char16 wcInfo)
{
    if (wcInfo != '中') {
        printf("OnTriggerChar16:%C!\n", wcInfo);
        CThread::Quit(-11);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerAString(void* pUserData, PInterface pSender, String asInfo)
{
    if (strcmp(asInfo, "AString") != 0) {
        printf("OnTriggerAString:%s!\n", (const char*)asInfo);
        CThread::Quit(-12);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerString(void* pUserData, PInterface pSender, String wsInfo)
{
    if (strcmp(wsInfo, "ＷＳｔｒｉｎｇ") != 0) {
        printf("OnTriggerString:%s!\n", (const char*)wsInfo);
        CThread::Quit(-13);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerACArray(void* pUserData,
        PInterface pSender, const BufferOf<Char8> & acaInfo)
{
    if (acaInfo[0] != 'a' || acaInfo[1] != 'b' ||
            acaInfo[2] != 'x' || acaInfo[3] != 'y' || acaInfo[4] != 'z') {
        printf("OnTriggerACArray 1:%c\t2:%c\t3:%c\t4:%c\t5:%c\n\n",
                acaInfo[0], acaInfo[1], acaInfo[2], acaInfo[3], acaInfo[4]);
        CThread::Quit(-14);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerWCArray(void* pUserData,
        PInterface pSender, const BufferOf<Char16> & wcaInfo)
{
    if (wcaInfo[0] != '一'
        || wcaInfo[1] != '二'
        || wcaInfo[2] != '三'
        || wcaInfo[3] != '四'
        || wcaInfo[4] != '五') {
        printf("OnTriggerWCArray 1:%C\t2:%C\t3:%C\t4:%C\t5:%C\n\n",
                wcaInfo[0], wcaInfo[1], wcaInfo[2], wcaInfo[3], wcaInfo[4]);
        CThread::Quit(-15);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerASArray(void* pUserData,
        PInterface pSender, const BufferOf<String> & asaInfo)
{
    if (strcmp(asaInfo[0], "1") != 0 ||
            strcmp(asaInfo[1], "22") != 0 ||
            strcmp(asaInfo[2], "333") != 0 ||
            strcmp(asaInfo[3], "4444") != 0 ||
            strcmp(asaInfo[4], "55555") != 0) {
        printf("OnTriggerASArray \n1:%s\n2:%s\n3:%s\n4:%s\n5:%s\n\n",
                (const char*)asaInfo[0], (const char*)asaInfo[1], (const char*)asaInfo[2],
                (const char*)asaInfo[3], (const char*)asaInfo[4]);
        CThread::Quit(-16);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerWSArray(void* pUserData,
        PInterface pSender, const BufferOf<String> & wsaInfo)
{
    if (strcmp(wsaInfo[0], "伟大") != 0 ||
            strcmp(wsaInfo[1], "渺小") != 0 ||
            strcmp(wsaInfo[2], "中庸") != 0 ||
            strcmp(wsaInfo[3], "可怜") != 0 ||
            strcmp(wsaInfo[4], "欢乐") != 0) {
        printf("OnTriggerWSArray \n1:%s\n2:%s\n3:%s\n4:%s\n5:%s\n\n",
                (const char*)wsaInfo[0], (const char*)wsaInfo[1],
                (const char*)wsaInfo[2], (const char*)wsaInfo[3],
                (const char*)wsaInfo[4]);
        CThread::Quit(-17);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerByteArray(void* pUserData,
        PInterface pSender, const BufferOf<Byte> & baInfo)
{
    if (baInfo[0] != 0 || baInfo[1] != 1 || baInfo[2] != 127 ||
            baInfo[3] != 128 || baInfo[4] != 255) {
        printf("OnTriggerByteArray 1:%d\t2:%d\t3:%d\t4:%d\t5:%d\n\n",
                baInfo[0], baInfo[1], baInfo[2], baInfo[3], baInfo[4]);
        CThread::Quit(-18);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInt16Array(void* pUserData,
        PInterface pSender, const BufferOf<Int16> & i16aInfo)
{
    if (i16aInfo[0] != 0 || i16aInfo[1] != 1 || i16aInfo[2] != -1 ||
            i16aInfo[3] != -32768 || i16aInfo[4] != 32767) {
        printf("OnTriggerInt16Array 1:%d\t2:%d\t3:%d\t4:%d\t5:%d\n\n",
                i16aInfo[0], i16aInfo[1], i16aInfo[2],
                i16aInfo[3], i16aInfo[4]);
        CThread::Quit(-19);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInt32Array(void* pUserData,
        PInterface pSender, const BufferOf<Int32> & i32aInfo)
{
    if (i32aInfo[0] != 0 || i32aInfo[1] != 1 || i32aInfo[2] != -1 ||
            i32aInfo[3] != (Int32)0x80000000 || i32aInfo[4] != 2147483647) {
        printf("OnTriggerInt32Array 1:%d\t2:%d\t3:%d\t4:%d\t5:%d\n\n",
                i32aInfo[0], i32aInfo[1], i32aInfo[2],
                i32aInfo[3], i32aInfo[4]);
        CThread::Quit(-20);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInt64Array(void* pUserData,
        PInterface pSender, const BufferOf<Int64> & i64aInfo)
{
    if (i64aInfo[0] != 0 || i64aInfo[1] != 1 || i64aInfo[2] != -1 ||
            i64aInfo[3] != -9223372036854775808LL ||
            i64aInfo[4] != 9223372036854775807LL) {
        printf("OnTriggerInt64Array "
                "1:%I64d\t2:%I64d\t3:%I64d\t4:%I64d\t5:%I64d\n\n",
                i64aInfo[0], i64aInfo[1], i64aInfo[2],
                i64aInfo[3], i64aInfo[4]);
        CThread::Quit(-21);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerFloatArray(void* pUserData,
        PInterface pSender, const BufferOf<Float> & faInfo)
{
    if (faInfo[0] != 0.0f || faInfo[1] != -0.000000001f ||
            faInfo[2] != 0.0000001f || faInfo[3] != 1111.1111f ||
            faInfo[4] != -9999.9999f) {
        printf("OnTriggerFloatArray 1:%e\t2:%e\t3:%e\t4:%e\t5:%e\n\n",
                faInfo[0], faInfo[1], faInfo[2],
                faInfo[3], faInfo[4]);
        CThread::Quit(-22);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerDoubleArray(void* pUserData,
        PInterface pSender, const BufferOf<Double> & daInfo)
{
    if (daInfo[0] != 0.0 || daInfo[1] != -0.000000001 ||
            daInfo[2] != 0.0000001 || daInfo[3] != 1111.1111 ||
            daInfo[4] != -9999.9999) {
        printf("OnTriggerDoubleArray 1:%e\t2:%e\t3:%e\t4:%e\t5:%e\n\n",
                daInfo[0], daInfo[1], daInfo[2],
                daInfo[3], daInfo[4]);
        CThread::Quit(-23);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerObjectArray(void* pUserData,
        PInterface pSender, const BufferOf<PInterface> & oaInfo)
{
    IEvent* pIEvent = NULL;
    IMutex* pIMutex = NULL;
    IThread* pIThread = NULL;

    pIEvent = IEvent::Probe(oaInfo[1]);
    pIMutex = IMutex::Probe(oaInfo[2]);
    pIThread = IThread::Probe(oaInfo[3]);

    if (!pIEvent || !pIMutex || !pIThread ||
            oaInfo[0] != pSender || oaInfo[4] != NULL) {
        printf("OnTriggerObjectArray pIEvent:%x\npIMutex:%x\npIThread:%x\n"
                "pSender:%x\toaInfo[0]:%x\noaInfo[4]:%x\n\n",
                pIEvent, pIMutex, pIThread, pSender, oaInfo[0], oaInfo[4]);
        CThread::Quit(-24);
    }

    oaInfo[1]->Release();
    oaInfo[2]->Release();
    oaInfo[3]->Release();
    g_count++;
    return NOERROR;
}

ECode OnTriggerArrayOf(void* pUserData,
                    PInterface pSender, const BufferOf<TestStruct> & saInfo)
{
    if (saInfo[0].nType != 1 || saInfo[0].nx != 1 || saInfo[0].ny != 1 ||
            saInfo[1].nType != 2 || saInfo[1].nx != 2 || saInfo[1].ny != 2 ||
            saInfo[2].nType != 3 || saInfo[2].nx != 3 || saInfo[2].ny != 3 ||
            saInfo[3].nType != 4 || saInfo[3].nx != 4 || saInfo[3].ny != 4 ||
            saInfo[4].nType != 5 || saInfo[4].nx != 5 || saInfo[4].ny != 5) {
        printf("OnTriggerArrayOf:\n1:%d, %d, %d\n2:%d, %d, %d\n"
                "3:%d, %d, %d\n4:%d, %d, %d\n5:%d, %d, %d\n\n",
                saInfo[0].nType, saInfo[0].nx, saInfo[0].ny,
                saInfo[1].nType, saInfo[1].nx, saInfo[1].ny,
                saInfo[2].nType, saInfo[2].nx, saInfo[2].ny,
                saInfo[3].nType, saInfo[3].nx, saInfo[3].ny,
                saInfo[4].nType, saInfo[4].nx, saInfo[4].ny);
        CThread::Quit(-25);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInterface(void* pUserData,
        PInterface pSender, IHello121* piInfo)
{
    IHello121 *pHello = (IHello121*)pSender->Probe(EIID_IHello121);
    if (pHello != piInfo) {
        printf("OnTriggerInterface pSender:%x\tpiInfo:%x\n",
                pSender, piInfo);
        CThread::Quit(-26);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerAll(void* pUserData,
        PInterface pSender,
        TestEnum emInfo,
        const TestStruct & stInfo,
        Byte byInfo,
        Int16 i16Info,
        Int32 i32Info,
        Int64 i64Info,
        Float fInfo,
        Double dInfo,
        Boolean bInfo,
        Char8 acInfo,
        Char16 wcInfo,
        String asInfo,
        String wsInfo,
        const BufferOf<Char8> & acaInfo,
        const BufferOf<Char16> & wcaInfo,
        const BufferOf<String> & asaInfo,
        const BufferOf<String> & wsaInfo,
        const BufferOf<Byte> & baInfo,
        const BufferOf<Int16> & i16aInfo,
        const BufferOf<Int32> & i32aInfo,
        const BufferOf<Int64> & i64aInfo,
        const BufferOf<Float> & faInfo,
        const BufferOf<Double> & daInfo,
        const BufferOf<PInterface> & oaInfo,
        const BufferOf<TestStruct> & saInfo,
        IHello121* piInfo)
{
    TestStruct stTmp = stInfo;
    printf("OnTriggerAll!\n");
    if (stTmp.nType != 27) {
        printf("OnTriggerAll Error! nType = %d!\n", stTmp.nType);
        CThread::Quit(-27);
    }
    stTmp.nType = 2;

    OnTriggerEnum(pUserData, pSender, emInfo);
    OnTriggerStruct(pUserData, pSender, stTmp);
    OnTriggerByte(pUserData, pSender, byInfo);
    OnTriggerInt16(pUserData, pSender, i16Info);
    OnTriggerInt32(pUserData, pSender, i32Info);
    OnTriggerInt64(pUserData, pSender, i64Info);
    OnTriggerFloat(pUserData, pSender, fInfo);
    OnTriggerDouble(pUserData, pSender, dInfo);
    OnTriggerBoolean(pUserData, pSender, bInfo);
    OnTriggerChar8(pUserData, pSender, acInfo);
    OnTriggerChar16(pUserData, pSender, wcInfo);
    OnTriggerAString(pUserData, pSender, asInfo);
    OnTriggerString(pUserData, pSender, wsInfo);
    OnTriggerACArray(pUserData, pSender, acaInfo);
    OnTriggerWCArray(pUserData, pSender, wcaInfo);
    OnTriggerASArray(pUserData, pSender, asaInfo);
    OnTriggerWSArray(pUserData, pSender, wsaInfo);
    OnTriggerByteArray(pUserData, pSender, baInfo);
    OnTriggerInt16Array(pUserData, pSender, i16aInfo);
    OnTriggerInt32Array(pUserData, pSender, i32aInfo);
    OnTriggerInt64Array(pUserData, pSender, i64aInfo);
    OnTriggerFloatArray(pUserData, pSender, faInfo);
    OnTriggerDoubleArray(pUserData, pSender, daInfo);
    OnTriggerObjectArray(pUserData, pSender, oaInfo);
    OnTriggerArrayOf(pUserData, pSender, saInfo);
    OnTriggerInterface(pUserData, pSender, piInfo);

    return NOERROR;
}

ECode OnTriggerInt8(void* pUserData, PInterface pSender, Int8 i8Info)
{
    if (i8Info != -128) {
        printf("OnTriggerInt8:%x!\n", i8Info);
        CThread::Quit(-28);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerUInt16(void* pUserData, PInterface pSender, UInt16 u16Info)
{
    if (u16Info != 65535) {
        printf("OnTriggerUInt16:%x!\n", u16Info);
        CThread::Quit(-29);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerUInt32(void* pUserData, PInterface pSender, UInt32 u32Info)
{
    if (u32Info != 4294967295u) {
        printf("OnTriggerUInt32:%x!\n", u32Info);
        CThread::Quit(-30);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerUInt64(void* pUserData, PInterface pSender, UInt64 u64Info)
{
    if (u64Info != 18446744073709551615ULL) {
        printf("OnTriggerUInt64:%x!\n", u64Info);
        CThread::Quit(-31);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerInt8Array(void* pUserData,
        PInterface pSender, const BufferOf<Int8> & i8aInfo)
{
    if (i8aInfo[0] != 0 || i8aInfo[1] != 1 || i8aInfo[2] != -1 ||
            i8aInfo[3] != -128 || i8aInfo[4] != 127) {
        printf("OnTriggerInt8Array 1:%d\t2:%d\t3:%d\t4:%d\t5:%d\n\n",
                i8aInfo[0], i8aInfo[1], i8aInfo[2],
                i8aInfo[3], i8aInfo[4]);
        CThread::Quit(-32);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerUInt16Array(void* pUserData,
        PInterface pSender, const BufferOf<UInt16> & u16aInfo)
{
    if (u16aInfo[0] != 0 || u16aInfo[1] != 1 || u16aInfo[2] != 32767 ||
            u16aInfo[3] != 32768 || u16aInfo[4] != 65535) {
        printf("OnTriggerUInt16Array 1:%d\t2:%d\t3:%d\t4:%d\t5:%d\n\n",
                u16aInfo[0], u16aInfo[1], u16aInfo[2],
                u16aInfo[3], u16aInfo[4]);
        CThread::Quit(-33);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerUInt32Array(void* pUserData,
        PInterface pSender, const BufferOf<UInt32> & u32aInfo)
{
    if (u32aInfo[0] != 0 || u32aInfo[1] != 1 || u32aInfo[2] != 2147483647 ||
            u32aInfo[3] != 2147483648u || u32aInfo[4] != 4294967295u) {
        printf("OnTriggerUInt32Array 1:%d\t2:%d\t3:%d\t4:%d\t5:%d\n\n",
                u32aInfo[0], u32aInfo[1], u32aInfo[2],
                u32aInfo[3], u32aInfo[4]);
        CThread::Quit(-34);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerUInt64Array(void* pUserData,
        PInterface pSender, const BufferOf<UInt64> & u64aInfo)
{
    if (u64aInfo[0] != 0 || u64aInfo[1] != 1 ||
            u64aInfo[2] != 9223372036854775807LL ||
            u64aInfo[3] != 9223372036854775808ULL ||
            u64aInfo[4] != 18446744073709551615ULL) {
        printf("OnTriggerUInt64Array "
                "1:%I64d\t2:%I64d\t3:%I64d\t4:%I64d\t5:%I64d\n\n",
                u64aInfo[0], u64aInfo[1], u64aInfo[2],
                u64aInfo[3], u64aInfo[4]);
        CThread::Quit(-35);
    }
    g_count++;
    return NOERROR;
}

ECode OnTriggerAllLocal(void* pUserData,
        PInterface pSender,
        Int8 i8Info,
        UInt16 u16Info,
        UInt32 u32Info,
        UInt64 u64Info,
        const BufferOf<Int8> & i8aInfo,
        const BufferOf<UInt16> & u16aInfo,
        const BufferOf<UInt32> & u32aInfo,
        const BufferOf<UInt64> & u64aInfo)
{
    printf("OnTriggerAllLocal!\n");

    OnTriggerInt8(pUserData, pSender, i8Info);
    OnTriggerUInt16(pUserData, pSender, u16Info);
    OnTriggerUInt32(pUserData, pSender, u32Info);
    OnTriggerUInt64(pUserData, pSender, u64Info);
    OnTriggerInt8Array(pUserData, pSender, i8aInfo);
    OnTriggerUInt16Array(pUserData, pSender, u16aInfo);
    OnTriggerUInt32Array(pUserData, pSender, u32aInfo);
    OnTriggerUInt64Array(pUserData, pSender, u64aInfo);

    return NOERROR;
}

struct RoutineArg
{
    int         nIndex;
    CTest*      pCTest;
    IInterface*    apIObj[5];
};
CTest*  g_pcTest = NULL;
int     g_nIndex = 0;
typedef int (__stdcall *FUNC)(void *pThis);
typedef int (__stdcall CTest::*TestFuncPrototype)();

ECode CTest::SubRoutine(void *pArg)
{
    RoutineArg raArg = *(RoutineArg*)pArg;
    ECode ec = NOERROR;
    QuitCode qcRet = 0;

    switch (raArg.nIndex) {
        case 1:
            ec = CCallbackClass::AddOnTriggerClassCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger1);
            if (ec != NOERROR) {
                printf("CCallbackClass Add Callback Failed! ec = %x\n", ec);
                qcRet = 1;
                goto Exit;
            }
            ec = CCallbackClass::AddOnTriggerClassCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger1);
            if (ec != NOERROR) {
                printf("CCallbackClass Add Callback Failed! ec = %x\n", ec);
                qcRet = 22;
                goto Exit;
            }
            break;
        case 2:
            ec = ACallbackAspect::AddOnTriggerAspectCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger2);
            if (ec != NOERROR) {
                printf("ACallbackAspect Add Callback Failed! ec = %x\n", ec);
                qcRet = 2;
                goto Exit;
            }
            ec = CCallbackClass::AddOnTriggerClassCallback(
                raArg.apIObj[1], raArg.pCTest, &CTest::OnTrigger1);
            if (ec != NOERROR) {
                printf("CCallbackClass Add Callback Failed! ec = %x\n", ec);
                qcRet = 3;
                goto Exit;
            }
            break;
        case 3:
            ec = KCallbackContext::AddOnTriggerContextCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger3);
            if (ec != NOERROR) {
                printf("KCallbackContext Add Callback Failed! ec = %x\n", ec);
                qcRet = 4;
                goto Exit;
            }
            ec = ACallbackAspect::AddOnTriggerAspectCallback(
                raArg.apIObj[1], raArg.pCTest, &CTest::OnTrigger2);
            if (ec != NOERROR) {
                printf("ACallbackAspect Add Callback Failed! ec = %x\n", ec);
                qcRet = 5;
                goto Exit;
            }
            ec = CCallbackClass::AddOnTriggerClassCallback(
                raArg.apIObj[2], raArg.pCTest, &CTest::OnTrigger1);
            if (ec != NOERROR) {
                printf("CCallbackClass Add Callback Failed! ec = %x\n", ec);
                qcRet = 6;
                goto Exit;
            }
            break;
        case 4:
            ec = CCallback::AddOnTriggerLocalCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger4);
            if (ec != NOERROR) {
                printf("CCallback Add Callback Failed! ec = %x\n", ec);
                qcRet = 7;
                goto Exit;
            }
            ec = CCallback::AddOnTriggerLocalCallback(
                raArg.apIObj[1], raArg.pCTest, &CTest::OnTrigger4);
            if (E_NO_INTERFACE != ec) {
                printf("CCallback Add Callback error! ec = %x\n", ec);
                qcRet = 8;
                goto Exit;
            }
            ec = CCallback::AddOnTriggerLocalCallback(
                raArg.apIObj[0], (IHelloClient*)(raArg.apIObj[2]),
                &IHelloClient::OnTrigger);
            if (ec != NOERROR) {
                printf("CCallback Add Callback error! ec = %x\n", ec);
                qcRet = 9;
                goto Exit;
            }
            break;
        case 5:
            ec = CCallback31::AddOnTrigger31Callback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger5);
            if (ec != NOERROR) {
                printf("TCallback31 Add Callback Failed! ec = %x\n", ec);
                qcRet = 10;
                goto Exit;
            }
            break;
        case 6:
            ec = AAspect41::AddOnTrigger41Callback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger6);
            if (ec != NOERROR) {
                printf("AAspect41 Add Callback Failed! ec = %x\n", ec);
                qcRet = 11;
                goto Exit;
            }
            break;
        case 7:
            ec = KContext51::AddOnTrigger51Callback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger7);
            if (ec != NOERROR) {
                printf("KContext51 Add Callback Failed! ec = %x\n", ec);
                qcRet = 12;
                goto Exit;
            }
            break;
        case 8:
            ec = CCallback61::AddOnTrigger61Callback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger8);
            if (ec != NOERROR) {
                printf("CCallback61 Add Callback Failed! ec = %x\n", ec);
                qcRet = 13;
                goto Exit;
            }
            ec = CCallback61::AddOnTrigger62Callback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger9);
            if (ec != NOERROR) {
                printf("CCallback61 Add Callback Failed! ec = %x\n", ec);
                qcRet = 14;
                goto Exit;
            }
            break;
        case 9:
            ec = CClass71::AddOnTrigger71Callback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger10);
            if (ec != NOERROR) {
                printf("CClass71 Add Callback Failed! ec = %x\n", ec);
                qcRet = 15;
                goto Exit;
            }
            ec = AAspect72::AddOnTrigger71Callback(
                raArg.apIObj[1], raArg.pCTest, &CTest::OnTrigger11);
            if (ec != NOERROR) {
                printf("AAspect72 Add Callback Failed! ec = %x\n", ec);
                qcRet = 16;
                goto Exit;
            }
            ec = KContext73::AddOnTrigger71Callback(
                raArg.apIObj[2], raArg.pCTest, &CTest::OnTrigger12);
            if (ec != NOERROR) {
                printf("KContext73 Add Callback Failed! ec = %x\n", ec);
                qcRet = 17;
                goto Exit;
            }
            break;
//        case 10: // 不再支持，删除测试用例
//            ec = CCallback81::AddOnTrigger81Callback(
//                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger13);
//            if (ec != NOERROR) {
//                printf("CCallback81 Add Callback Failed! ec = %x\n", ec);
//                qcRet = 18;
//                goto Exit;
//            }
//            break;
        case 11:
        case 12:
        case 13:
            ec = CCallback::AddOnTriggerLocalCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger14);
            if (ec != NOERROR) {
                printf("CCallback Add Callback Failed! ec = %x\n", ec);
                qcRet = 19;
                goto Exit;
            }
            break;
        case 14:
            ec = CCallback91::AddOnTrigger0Callback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTrigger15);
            if (ec != NOERROR) {
                printf("CCallback91 Add Callback Failed! ec = %x\n", ec);
                qcRet = 20;
                goto Exit;
            }
            ec = CCallback91::AddOnTrigger15Callback(
                raArg.apIObj[1], raArg.pCTest, &CTest::OnTrigger16);
            if (ec != NOERROR) {
                printf("CCallback91 Add Callback Failed! ec = %x\n", ec);
                qcRet = 21;
                goto Exit;
            }
            break;
        case 15:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 23;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 24;
                goto Exit;
            }
            break;
        case 16:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 25;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 26;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 27;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 28;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 29;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 30;
                goto Exit;
            }
            break;
        case 17:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 31;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 32;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 33;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 34;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 35;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 36;
                goto Exit;
            }
            break;
        case 18:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 37;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 38;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 39;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 40;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 41;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 42;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 43;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 44;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 45;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 46;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 47;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 48;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 49;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 50;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], &::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 51;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 52;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 53;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], &::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 54;
                goto Exit;
            }
            break;
        case 20:
        case 21:
        case 22:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 55;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 56;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], &::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 57;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 58;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 59;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 60;
                goto Exit;
            }
            break;
        case 23:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 61;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 62;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 63;
                goto Exit;
            }
            break;
        case 19:
        case 24:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 64;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], &::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 65;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], &::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 66;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 67;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerBCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerB);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 68;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerCCallback(
                raArg.apIObj[0], raArg.pCTest, &CTest::OnTriggerC);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 69;
                goto Exit;
            }

            ec = CCBServer::AddOnTriggerDCallback(
                raArg.apIObj[0], &::OnTriggerD);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 70;
                goto Exit;
            }
            break;
        case 25:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 71;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 72;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 73;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerDCallback(
                raArg.apIObj[0], &::OnTriggerD);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 74;
                goto Exit;
            }
            break;
        case 26:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[1], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 75;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 76;
                goto Exit;
            }
            break;
        case 27:
            ec = CCBServer::AddOnTriggerCallback(
                raArg.apIObj[0], &::OnTrigger);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 77;
                goto Exit;
            }
            break;
        case 28:
            ec = CCBServer::AddOnTriggerACallback(
                raArg.apIObj[0], &::OnTriggerA);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 78;
                goto Exit;
            }
            ec = CCBServer::AddOnTriggerDCallback(
                raArg.apIObj[0], &::OnTriggerD);
            if (ec != NOERROR) {
                printf("CCBServer Add Callback Failed! ec = %x\n", ec);
                qcRet = 79;
                goto Exit;
            }
            break;
        case 29:
            ec = CCallback121::AddOnTriggerEnumCallback(
                raArg.apIObj[0], &::OnTriggerEnum);
            if (ec != NOERROR) {
                printf("CCallback121 AddEnumCallback Failed! ec = %x\n", ec);
                qcRet = 80;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerStructCallback(
                raArg.apIObj[0], &::OnTriggerStruct);
            if (ec != NOERROR) {
                printf("CCallback121 AddStructCallback Failed! ec = %x\n", ec);
                qcRet = 81;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerByteCallback(
                raArg.apIObj[0], &::OnTriggerByte);
            if (ec != NOERROR) {
                printf("CCallback121 AddByteCallback Failed! ec = %x\n", ec);
                qcRet = 82;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerInt16Callback(
                raArg.apIObj[0], &::OnTriggerInt16);
            if (ec != NOERROR) {
                printf("CCallback121 AddInt16Callback Failed! ec = %x\n", ec);
                qcRet = 83;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerInt32Callback(
                raArg.apIObj[0], &::OnTriggerInt32);
            if (ec != NOERROR) {
                printf("CCallback121 AddInt32Callback Failed! ec = %x\n", ec);
                qcRet = 84;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerInt64Callback(
                raArg.apIObj[0], &::OnTriggerInt64);
            if (ec != NOERROR) {
                printf("CCallback121 AddInt64Callback Failed! ec = %x\n", ec);
                qcRet = 85;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerFloatCallback(
                raArg.apIObj[0], &::OnTriggerFloat);
            if (ec != NOERROR) {
                printf("CCallback121 AddFloatCallback Failed! ec = %x\n", ec);
                qcRet = 86;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerDoubleCallback(
                raArg.apIObj[0], &::OnTriggerDouble);
            if (ec != NOERROR) {
                printf("CCallback121 AddDoubleCallback Failed! ec = %x\n", ec);
                qcRet = 87;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerBooleanCallback(
                raArg.apIObj[0], &::OnTriggerBoolean);
            if (ec != NOERROR) {
                printf("CCallback121 AddBooleanCallback Failed! ec = %x\n", ec);
                qcRet = 88;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerChar8Callback(
                raArg.apIObj[0], &::OnTriggerChar8);
            if (ec != NOERROR) {
                printf("CCallback121 AddChar8Callback Failed! ec = %x\n", ec);
                qcRet = 89;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerChar16Callback(
                raArg.apIObj[0], &::OnTriggerChar16);
            if (ec != NOERROR) {
                printf("CCallback121 AddChar16Callback Failed! ec = %x\n", ec);
                qcRet = 90;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerAStringCallback(
                raArg.apIObj[0], &::OnTriggerAString);
            if (ec != NOERROR) {
                printf("CCallback121 AddAStringCallback Failed! ec = %x\n", ec);
                qcRet = 91;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerStringCallback(
                raArg.apIObj[0], &::OnTriggerString);
            if (ec != NOERROR) {
                printf("CCallback121 AddStringCallback Failed! ec = %x\n", ec);
                qcRet = 92;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerACArrayCallback(
                raArg.apIObj[0], &::OnTriggerACArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddACArrayCallback Failed! ec = %x\n", ec);
                qcRet = 93;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerWCArrayCallback(
                raArg.apIObj[0], &::OnTriggerWCArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddWCArrayCallback Failed! ec = %x\n", ec);
                qcRet = 94;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerASArrayCallback(
                raArg.apIObj[0], &::OnTriggerASArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddASArrayCallback Failed! ec = %x\n", ec);
                qcRet = 95;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerWSArrayCallback(
                raArg.apIObj[0], &::OnTriggerWSArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddWSArrayCallback Failed! ec = %x\n", ec);
                qcRet = 96;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerByteArrayCallback(
                raArg.apIObj[0], &::OnTriggerByteArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOf<Byte>Callback Failed!"
                        " ec = %x\n", ec);
                qcRet = 97;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerInt16ArrayCallback(
                raArg.apIObj[0], &::OnTriggerInt16Array);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOf<Int16>Callback Failed!"
                        " ec = %x\n", ec);
                qcRet = 98;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerInt32ArrayCallback(
                raArg.apIObj[0], &::OnTriggerInt32Array);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOf<Int32>Callback Failed!"
                        " ec = %x\n", ec);
                qcRet = 99;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerInt64ArrayCallback(
                raArg.apIObj[0], &::OnTriggerInt64Array);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOf<Int64>Callback Failed!"
                        " ec = %x\n", ec);
                qcRet = 100;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerFloatArrayCallback(
                raArg.apIObj[0], &::OnTriggerFloatArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOf<Float>Callback Failed!"
                        " ec = %x\n", ec);
                qcRet = 101;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerDoubleArrayCallback(
                raArg.apIObj[0], &::OnTriggerDoubleArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOf<Double>Callback Failed!"
                        " ec = %x\n", ec);
                qcRet = 102;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerObjectArrayCallback(
                raArg.apIObj[0], &::OnTriggerObjectArray);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOf<PInterface>Callback Failed!"
                        " ec = %x\n", ec);
                qcRet = 103;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerArrayOfCallback(
                raArg.apIObj[0], &::OnTriggerArrayOf);
            if (ec != NOERROR) {
                printf("CCallback121 AddArrayOfCallback Failed!"
                        " ec = %x\n", ec);
                qcRet = 104;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerInterfaceCallback(
                raArg.apIObj[0], &::OnTriggerInterface);
            if (ec != NOERROR) {
                printf("CCallback121 AddInterfaceCallback Failed!"
                        " ec = %x\n", ec);
                qcRet = 105;
                goto Exit;
            }
            ec = CCallback121::AddOnTriggerAllCallback(
                raArg.apIObj[0], &::OnTriggerAll);
            if (ec != NOERROR) {
                printf("CCallback121 AddAllCallback Failed!"
                        " ec = %x\n", ec);
                qcRet = 106;
                goto Exit;
            }
            // ec = CCallback121::AddOnTriggerInt8Callback(
                // raArg.apIObj[0], &::OnTriggerInt8);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddInt8Callback Failed! ec = %x\n", ec);
                // qcRet = 107;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerUInt16Callback(
                // raArg.apIObj[0], &::OnTriggerUInt16);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddUInt16Callback Failed! ec = %x\n", ec);
                // qcRet = 108;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerUInt32Callback(
                // raArg.apIObj[0], &::OnTriggerUInt32);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddUInt32Callback Failed! ec = %x\n", ec);
                // qcRet = 109;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerUInt64Callback(
                // raArg.apIObj[0], &::OnTriggerUInt64);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddUInt64Callback Failed! ec = %x\n", ec);
                // qcRet = 110;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerInt8ArrayCallback(
                // raArg.apIObj[0], &::OnTriggerInt8Array);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddArrayOf<Int8>Callback Failed!"
                        // " ec = %x\n", ec);
                // qcRet = 111;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerUInt16ArrayCallback(
                // raArg.apIObj[0], &::OnTriggerUInt16Array);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddArrayOf<UInt16>Callback Failed!"
                        // " ec = %x\n", ec);
                // qcRet = 112;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerUInt32ArrayCallback(
                // raArg.apIObj[0], &::OnTriggerUInt32Array);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddArrayOf<UInt32>Callback Failed!"
                        // " ec = %x\n", ec);
                // qcRet = 113;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerUInt64ArrayCallback(
                // raArg.apIObj[0], &::OnTriggerUInt64Array);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddArrayOf<UInt64>Callback Failed!"
                        // " ec = %x\n", ec);
                // qcRet = 114;
                // goto Exit;
            // }
            // ec = CCallback121::AddOnTriggerAllLocalCallback(
                // raArg.apIObj[0], &::OnTriggerAllLocal);
            // if (ec != NOERROR) {
                // printf("CCallback121 AddAllLocalCallback Failed!"
                        // " ec = %x\n", ec);
                // qcRet = 115;
                // goto Exit;
            // }
        default:
            break;
    }
    return NOERROR;
Exit:
    return qcRet;
}

/*
 * 测试简述:class包含callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 1
 *
 * 输入文件:
 *
 * 输出文件: cbclient1.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test1(int argc, char *argv[])
{
    IHello1* pIHello = NULL;
    ECode ec;
    ec = CCallbackClass::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallbackClass::New Error, ec = %x\n", ec);
        return -1;
    }

    RoutineArg raArg;
    raArg.nIndex = 1;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;

    m_apIObj[0] = pIHello;
    pIHello->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello->Hello1(1);
    CThread::Sleep(500, NULL);
    CCallbackClass::RemoveAllCallbacks(pIHello);
    pIHello->Release();
    if (m_nCount != 0x2) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
    }
    else {
        TPOK;
    }

    return 0;

Exit:
    CCallbackClass::RemoveAllCallbacks(pIHello);
    pIHello->Release();
    return ec;
}

/*
 * 测试简述:aspect包含callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 2
 *
 * 输入文件:
 *
 * 输出文件: cbclient2.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test2(int argc, char *argv[])
{
    ECode ec;
    IHello1* pIServer = NULL;
    IHello2* pIAspect = NULL;

    ec = CCallbackClass::New(&pIServer);
    if (ec != NOERROR) {
        TPERR("CCallbackClass::New Error, ec = %x\n", ec);
        goto Exit;
    }
    ec = ACallbackAspect::Attach(pIServer);
    if (ec != NOERROR) {
        TPERR("ACallbackAspect::Attach failed! ec = %x\n", ec);
        goto Exit;
    }
    pIAspect = IHello2::Probe(pIServer);
    if (!pIAspect) {
        ec = E_NO_INTERFACE;
        TPERR("IHello2::Probe failed! ec = %x\n", ec);
        goto Exit;
    }
    RoutineArg raArg;
    raArg.nIndex = 2;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIAspect;
    raArg.apIObj[1] = pIServer;

    m_apIObj[0] = pIAspect;
    m_apIObj[1] = pIServer;
    pIAspect->AddRef();
    pIServer->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIServer->Hello1(1);
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x1) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    pIServer->Hello1(2);
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x11) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    pIAspect->Hello2();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x21) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    CCallbackClass::RemoveAllCallbacks(pIServer);
    ACallbackAspect::RemoveAllCallbacks(pIAspect);
    if (pIServer) {
        pIServer->Release();
    }
    TPOK;
    return 0;

Exit:
    CCallbackClass::RemoveAllCallbacks(pIServer);
    ACallbackAspect::RemoveAllCallbacks(pIAspect);
    if (pIServer) {
        pIServer->Release();
    }
    return ec;
}

/*
 * 测试简述:context包含callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 3
 *
 * 输入文件:
 *
 * 输出文件: cbclient3.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test3(int argc, char *argv[])
{
    ECode ec;
    IHello1* pIServer = NULL;
    IHello2* pIAspect = NULL;
    IHello3* pIContext = NULL;
    ec = CCallbackClass::New(&pIServer);
    if (ec != NOERROR) {
        TPERR("CCallbackClass::New Failed! ec = %x\n", ec);
        goto Exit;
    }
    ec = KCallbackContext::New(&pIContext);
    if (ec != NOERROR) {
        TPERR("KCallbackContext::New Failed! ec = %x\n", ec);
        goto Exit;
    }

    ec = CObject::EnterContext(pIServer, pIContext);
    if (ec != NOERROR) {
        TPERR("EnterContext failed! ec = %x\n", ec);
        goto Exit;
    }

    pIAspect = IHello2::Probe(pIServer);
    if (!pIAspect) {
        ec = E_NO_INTERFACE;
        TPERR("Probe failed! ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 3;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIContext;
    raArg.apIObj[1] = pIAspect;
    raArg.apIObj[2] = pIServer;

    m_apIObj[0] = pIContext;
    m_apIObj[1] = pIServer;
    pIContext->AddRef();
    pIServer->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIServer->Hello1(1);
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x1) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    pIServer->Hello1(2);
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x11) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    pIContext->Hello3();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x111) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    ACallbackAspect::RemoveAllCallbacks(pIAspect);
    ec = CObject::LeaveContext(pIServer, pIContext);
    if (ec != NOERROR) {
        TPERR("LeaveContext failed!\n");
    }

    KCallbackContext::RemoveAllCallbacks(pIContext);
    CCallbackClass::RemoveAllCallbacks(pIServer);
    if (pIServer) {
        pIServer->Release();
    }
    if (pIContext) {
        pIContext->Release();
    }
    TPOK;
    return 0;

Exit:
    ACallbackAspect::RemoveAllCallbacks(pIAspect);
    KCallbackContext::RemoveAllCallbacks(pIContext);
    CCallbackClass::RemoveAllCallbacks(pIServer);
    if (pIServer) {
        pIServer->Release();
    }
    if (pIContext) {
        pIContext->Release();
    }
    return ec;
}

/*
 * 测试简述:用callback修饰local的interface
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 4
 *
 * 输入文件:
 *
 * 输出文件: cbclient4.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test4(int argc, char *argv[])
{
    //callback 不支持local属性，先注掉相关测试
    return NOERROR;

    IFoo* pIFoo = NULL;
    IHello* pIHello = NULL;
    IHello* pIHelloDiff = NULL;
    IHelloClient* pIHelloClient = NULL;
    Int32 nCount = 0;

    ECode ec = CCallback::New(&pIFoo);
    if (ec != NOERROR) {
        TPERR("CCallback::New Error, ec = %x\n", ec);
        goto Exit;
    }

    ec = CCallback::NewInContext(CTX_DIFF_DOMAIN, &pIHelloDiff);
    if (ec != NOERROR) {
        TPERR("CCallback::New in diff domain Error, ec = %x\n", ec);
        goto Exit;
    }
    ec = CHelloClient::NewInContext(CTX_DIFF_DOMAIN, &pIHelloClient);
    if (ec != NOERROR) {
        TPERR("CHelloClient::New in diff domain Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 4;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIFoo;
    raArg.apIObj[1] = pIHelloDiff;
    raArg.apIObj[2] = pIHelloClient;

    m_apIObj[0] = pIFoo;
    m_apIObj[1] = pIHelloDiff;
    m_apIObj[2] = pIHelloClient;
    pIFoo->AddRef();
    pIHelloDiff->AddRef();
    pIHelloClient->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello = IHello::Probe(pIFoo);
    if (!pIHello) {
        TPERR("IHello::Probe Failed! ec = %x\n", E_NO_INTERFACE);
        goto Exit;
    }
    pIFoo->Foo();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x1000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIHello->GetCount(&nCount);
    if (nCount != 0x10001) {
        TPERR("ERROR! Callback be triggered error! nCount:%x\n", nCount);
        goto Exit;
    }
    pIHelloDiff->Bar();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x1000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIHelloDiff->GetCount(&nCount);
    if (nCount != 0) {
        TPERR("ERROR! Callback be triggered error! nCount:%x\n", nCount);
        goto Exit;
    }

    if (pIFoo) {
        pIFoo->Release();
    }
    if (pIHelloDiff) {
        pIHelloDiff->Release();
    }
    if (pIHelloClient) {
        pIHelloClient->Release();
    }
    TPOK;
    return 0;

Exit:
    if (pIFoo) {
        pIFoo->Release();
    }
    if (pIHelloDiff) {
        pIHelloDiff->Release();
    }
    if (pIHelloClient) {
        pIHelloClient->Release();
    }
    return ec;
}

/*
 * 测试简述:singleton, main, aggregate, synchronized修饰的class包含callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 5
 *
 * 输入文件:
 *
 * 输出文件: cbclient5.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test5(int argc, char *argv[])
{
    IHello31* pIHello = NULL;
//    IApplet* pApplet = NULL;

    ECode ec = CCallback31::AcquireSingleton(&pIHello);
    if (ec != NOERROR) {
        TPERR("TCallback31::AcquireSingleton Error, ec = %x\n", ec);
        goto Exit;
    }
/*    
    pApplet = IApplet::Probe(pIHello);
    if (!pApplet) {
        TPERR("IApplet::Query Error, ec = %x\n", ec);
        goto Exit;
    }
    pApplet->Start(NULL_BUFFEROF(String));

    RoutineArg raArg;
    raArg.nIndex = 5;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;

    m_apIObj[0] = pIHello;
    pIHello->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello->Hello31();
    CThread::Sleep(100, NULL);
    ec = TCallback31::RemoveOnTrigger31Callback(
        pIHello, this, &CTest::OnTrigger5);
    if (ec != NOERROR) {
        printf("TCallback31 Remove Callback Failed! ec = %x\n", ec);
        goto Exit;
    }
    if (pApplet) {
        pApplet->Finish(AppletFinish_Nice);
    }
    pIHello->Release();

    if (m_nCount != 0x10000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
    }
    else {
        TPOK;
    }
    return 0;
*/
Exit:
//    if (pApplet) {
//        pApplet->Finish(AppletFinish_Nice);
//    }
    if (pIHello) {
        pIHello->Release();
    }
    return ec;
}

/*
 * 测试简述: aspect包含callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 6
 *
 * 输入文件:
 *
 * 输出文件: cbclient6.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test6(int argc, char *argv[])
{
    IHello41* pIHello = NULL;
    IHello42* pIServer = NULL;

    ECode ec = CAspect42::New(&pIServer);
    if (ec != NOERROR) {
        TPERR("CAspect42::New Error, ec = %x\n", ec);
        goto Exit;
    }
    ec = AAspect41::Attach(pIServer);
    if (ec != NOERROR) {
        TPERR("AAspect41::Attach Error, ec = %x\n", ec);
        goto Exit;
    }
    pIHello = IHello41::Probe(pIServer);
    if (!pIHello) {
        ec = E_NO_INTERFACE;
        TPERR("IHello41::Probe Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 6;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;

    m_apIObj[0] = pIHello;
    pIHello->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello->Hello41();
    CThread::Sleep(100, NULL);
    AAspect41::RemoveAllCallbacks(pIHello);
    pIServer->Release();

    if (m_nCount != 0x100000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
    }
    else {
        TPOK;
    }
    return 0;

Exit:
    if (pIServer) {
        pIServer->Release();
    }
    return ec;
}

/*
 * 测试简述:singleton, aggregate, aspect, sequenced修饰的context包含callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 7
 *
 * 输入文件:
 *
 * 输出文件: cbclient7.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test7(int argc, char *argv[])
{
    IHello51* pIHello = NULL;

    ECode ec = KContext51::AcquireSingleton(&pIHello);
    if (ec != NOERROR) {
        TPERR("KContext51::AcquireSingleton Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 7;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;

    m_apIObj[0] = pIHello;
    pIHello->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello->Hello51();
    CThread::Sleep(100, NULL);

    KContext51::RemoveOnTrigger51Callback(pIHello, this, &CTest::OnTrigger7);
    pIHello->Release();

    if (m_nCount != 0x1000000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
    }
    else {
        TPOK;
    }
    return 0;

Exit:
    KContext51::RemoveOnTrigger51Callback(pIHello, this, &CTest::OnTrigger7);
    if (pIHello) {
        pIHello->Release();
    }
    return ec;
}

/*
 * 测试简述:一个class包含两个以上callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 8
 *
 * 输入文件:
 *
 * 输出文件: cbclient8.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test8(int argc, char *argv[])
{
    IHello61* pIHello = NULL;

    ECode ec = CCallback61::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallback61::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 8;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;

    m_apIObj[0] = pIHello;
    pIHello->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello->Hello61(1);
    CThread::Sleep(500, NULL);
    if (m_nCount != 0x2) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIHello->Hello61(2);
    CThread::Sleep(500, NULL);
    if (m_nCount != 0x22) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIHello->Hello61(3);
    CThread::Sleep(500, NULL);

    CCallback61::RemoveAllCallbacks(pIHello);
    pIHello->Release();
    if (m_nCount != 0x44) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    TPOK;
    return 0;

Exit:
    CCallback61::RemoveAllCallbacks(pIHello);
    if (pIHello) {
        pIHello->Release();
    }
    return ec;
}

/*
 * 测试简述:一个class，一个aspect和一个context都包含同一个callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 9
 *
 * 输入文件:
 *
 * 输出文件: cbclient9.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test9(int argc, char *argv[])
{
    IHello71* pIHello = NULL;
    IHello72* pIAspect = NULL;
    IHello73* pIContext = NULL;

    ECode ec = CClass71::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallbackClass::New Failed, ec = %x\n", ec);
        goto Exit;
    }

    ec = AAspect72::Attach(pIHello);
    if (ec != NOERROR) {
        TPERR("AAspect72::Attach Failed, ec = %x\n", ec);
        goto Exit;
    }
    pIAspect = IHello72::Probe(pIHello);
    if (!pIAspect) {
        ec = E_NO_INTERFACE;
        TPERR("IHello72::Probe Failed, ec = %x\n", ec);
        goto Exit;
    }

    ec = KContext73::New(&pIContext);
    if (ec != NOERROR) {
        TPERR("KContext73::New Failed, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 9;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;
    raArg.apIObj[1] = pIAspect;
    raArg.apIObj[2] = pIContext;

    m_apIObj[0] = pIHello;
    m_apIObj[1] = pIAspect;
    m_apIObj[2] = pIContext;
    pIHello->AddRef();
    pIAspect->AddRef();
    pIContext->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello->Hello71();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x200) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIAspect->Hello72();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x2200) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIContext->Hello73();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x22200) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    CClass71::RemoveAllCallbacks(pIHello);
    AAspect72::RemoveAllCallbacks(pIAspect);
    KContext73::RemoveAllCallbacks(pIContext);
    if (pIHello) {
        pIHello->Release();
    }
    if (pIContext) {
        pIContext->Release();
    }
    TPOK;
    return 0;

Exit:
    CClass71::RemoveAllCallbacks(pIHello);
    AAspect72::RemoveAllCallbacks(pIAspect);
    KContext73::RemoveAllCallbacks(pIContext);
    if (pIHello) {
        pIHello->Release();
    }
    if (pIContext) {
        pIContext->Release();
    }
    return ec;
}

/*
 * 测试简述:两个class包含一个相同的接口，一个是普通接口，一个是callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 10
 *
 * 输入文件:
 *
 * 输出文件: cbclient10.out
 *
 * 结果检查: Manual
 *
 * 补充说明: 不再支持这种用法，删除本测试用例。
 *
 */
int CTest::Test10(int argc, char *argv[])
{
#if 0
    IHello81* pIHello = NULL;
    IHello82* pIHello2 = NULL;
    ICallback81* pICallback = NULL;
    Int32 nCount = 0;

    ECode ec = CCallback81::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallback81::New Error, ec = %x\n", ec);
        goto Exit;
    }

    ec = CCallback82::New(&pIHello2);
    if (ec != NOERROR) {
        TPERR("CCallback82::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 10;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;

    m_apIObj[0] = pIHello;
    m_apIObj[1] = pIHello2;
    pIHello->AddRef();
    pIHello2->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    pIHello->Hello81();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x200000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIHello2->Hello82();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x200000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIHello2->GetCount(&nCount);
    if (nCount != 1) {
        TPERR("ERROR! Callback be triggered error! nCount:%d\n", nCount);
        goto Exit;
    }

    pICallback = ICallback81::Probe(pIHello2);
    if (!pICallback) {
        TPERR("ICallback81::Probe Error, ec = %x\n", E_NO_INTERFACE);
        goto Exit;
    }

    pICallback->OnTrigger81();
    CThread::Sleep(100, NULL);
    if (m_nCount != 0x200000) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    pIHello2->GetCount(&nCount);
    if (nCount != 2) {
        TPERR("ERROR! Callback be triggered error! nCount:%d\n", nCount);
        goto Exit;
    }

    if (pIHello) {
        pIHello->Release();
    }
    TPOK;
    return 0;

Exit:
    if (pIHello) {
        pIHello->Release();
    }
#endif
    return 0;
}

/*
 * 测试简述:循环创建对象500次，最后销毁
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 11
 *
 * 输入文件:
 *
 * 输出文件: cbclient11.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test11(int argc, char *argv[])
{
    IFoo* pIHello = NULL;
    RoutineArg raArg;
    raArg.nIndex = 11;
    raArg.pCTest = this;
    ECode ec;

    for (int i = 0; i < 500; i++) {
        ec = CCallback::New(&pIHello);
        if (ec != NOERROR) {
            TPERR("CCallback::New Error, ec = %x\n", ec);
            goto Exit;
        }
        raArg.apIObj[0] = pIHello;

        m_apIObj[0] = pIHello;
        pIHello->AddRef();

        ec = SubRoutine(&raArg);
        if (ec != NOERROR) {
            TPERR("SubRoutine Failed! ec = %x\n", ec);
            goto Exit;
        }

        pIHello->Foo();

        CThread::Sleep(100, NULL);
        CCallback::RemoveAllCallbacks(pIHello);
        if (pIHello) {
            pIHello->Release();
            pIHello = NULL;
        }
    }

    CThread::Sleep(500, NULL);
    if (m_nCount == 500) {
        TPOK;
    }
    else {
        TPERR("ERROR! callback times: %d\n", m_nCount);
    }
    return 0;

Exit:
    CCallback::RemoveAllCallbacks(pIHello);
    if (pIHello) {
        pIHello->Release();
    }
    return ec;
}

/*
 * 测试简述:多线程同时创建对象，最后销毁
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 12
 *
 * 输入文件:
 *
 * 输出文件: cbclient12.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
ECode TestThread(void *pArg)
{
    IFoo* pIHello = NULL;
    RoutineArg raArg;
    int i;
    ECode ec = CCallback::New(&pIHello);
    if (ec != NOERROR) {
        printf("CCallback::New Error, ec = %x\n", ec);
        goto Exit;
    }

    raArg.nIndex = 11;
    raArg.pCTest = (CTest*)pArg;
    raArg.apIObj[0] = pIHello;

    ec = raArg.pCTest->SubRoutine(&raArg);
    if (ec != NOERROR) {
        printf("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    for (i = 0; i < 1000; i++) {
        pIHello->Foo();
    }

    CThread::Sleep(1000, NULL);
    CCallback::RemoveAllCallbacks(pIHello);
    pIHello->Release();
    return NOERROR;
Exit:
    CCallback::RemoveAllCallbacks(pIHello);
    if (pIHello) {
        pIHello->Release();
    }
    return -1;
}

int CTest::Test12(int argc, char *argv[])
{
    IThread* pThread = NULL;
    RoutineArg raArg;
    ECode ec;
    int i;

    IFoo* pIHello = NULL;
    ec = CCallback::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallback::New Error, ec = %x\n", ec);
        return 1;
    }

    ec = CThread::New(TestThread, this, 0, &pThread);
    if (ec != NOERROR) {
        TPERR("*ERROR* Failed to create thread(ec = %x)\n", ec);
        pIHello->Release();
        return 1;
    }
    assert(pThread);

    raArg.nIndex = 12;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;
    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    for (i = 0; i < 1000; i++) {
        pIHello->Foo();
    }

    CThread::Sleep(1000, NULL);
    pThread->Join(INFINITE, NULL);
    pThread->GetQuitCode(&ec);
    pThread->Release();
    CCallback::RemoveAllCallbacks(pIHello);
    pIHello->Release();
    if (ec != NOERROR) {
        TPERR("child thread Error!\n");
    }
    else if (m_nCount != 2000) {
        TPERR("ERROR! callback times: %d\n", m_nCount);
    }
    else {
        TPOK;
    }
    return 0;

Exit:
    CCallback::RemoveAllCallbacks(pIHello);
    if (pThread) {
        pThread->Release();
    }
    if (pIHello) {
        pIHello->Release();
    }
    return 0;
}

/*
 * 测试简述:多进程同时创建对象，最后销毁
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 13
 *
 * 输入文件:
 *
 * 输出文件: cbclient13.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test13(int argc, char *argv[])
{
    IHello* pIHello = NULL;
    IFoo* pIFoo = NULL;
    int i;
    ECode ec = CCallback::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallback::New Error, ec = %x\n", ec);
        return 1;
    }

    ec = CSystem::RegisterRunningObject("cbtestserver", pIHello);
    if (ec != NOERROR) {
        TPERR("ERROR Failed to CSystem::RegisterRunningObject(ec = %x)\n", ec);
        pIHello->Release();
        return 1;
    }

    IProcess* pProc = NULL;

    ec = CProcess::New("cbclient1.exe", NULL, &pProc);
    if (ec != NOERROR) {
        TPERR("*ERROR* Failed to create Process(ec = %x)\n", ec);
        CSystem::UnregisterRunningObject("cbtestserver");
        pIHello->Release();
        return 1;
    }
    assert(pProc);

    pIFoo = IFoo::Probe(pIHello);
    if (!pIFoo) {
        ec = E_NO_INTERFACE;
        TPERR("IFoo::Probe failed, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 13;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIFoo;
    pIFoo->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    for (i = 0; i < 100; i++) {
        pIFoo->Foo();
    }

    CThread::Sleep(1000, NULL);
    pProc->WaitForExit(INFINITE, NULL);
    pProc->GetExitCode(&ec);
    if (ec < 0) {
        TPERR("child Proc Error!\n");
    }
    else if (ec != 100) {
        TPERR("ERROR! callback times: %d\n", ec);
    }
    else if (m_nCount != 200) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%d\n", m_nCount);
    }

Exit:
    CSystem::UnregisterRunningObject("cbtestserver");

    pProc->WaitForExit(INFINITE, NULL);
    CCallback::RemoveAllCallbacks(pIHello);
    pProc->Release();
    pIHello->Release();
    TPOK;
    return 0;
}

/*
 * 测试简述:一个class中包含16个普通接口和8个callback接口
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 14
 *
 * 输入文件:
 *
 * 输出文件: cbclient14.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test14(int argc, char *argv[])
{
    ICBNormal0* pIHello = NULL;
    ICBNormal1* pIHello1 = NULL;
    RoutineArg raArg;

    ECode ec = CCallback91::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallback91::New Error, ec = %x\n", ec);
        goto Exit;
    }

    pIHello1 = ICBNormal1::Probe(pIHello);
    if (!pIHello1) {
        ec = E_NO_INTERFACE;
        TPERR("ICBNormal1::Probe Error, ec = %x\n", ec);
        goto Exit;
    }

    raArg.nIndex = 14;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;
    raArg.apIObj[1] = pIHello1;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    m_nCount = 14;

    pIHello->Hello0();
    CThread::Sleep(500, NULL);
    if (m_nCount != 30) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }

    m_nCount = 14;
    pIHello1->Hello1();
    CThread::Sleep(500, NULL);
    if (m_nCount != 32) {
        TPERR("ERROR! Callback be triggered error! m_nCount:%x\n", m_nCount);
        goto Exit;
    }
    CCallback91::RemoveAllCallbacks(pIHello);
    pIHello->Release();

    TPOK;
    return 0;

Exit:
    CCallback91::RemoveAllCallbacks(pIHello);
    if (pIHello) {
        pIHello->Release();
    }
    return 0;
}

/*
 * 测试简述:分别注册类方法，全局方法为回调事件处理函数
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 15
 *
 * 输入文件:
 *
 * 输出文件: cbclient15.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test15(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 15;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->TriggerA();
    CThread::Sleep(100, NULL);

    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 1) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    piServer->TriggerB();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_CTest_OnTriggerB
        || g_pObj != piServer || g_n != 2) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 2) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:为同一个事件注册多个不同的回调方法，包括类方法与全局方法
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 16
 *
 * 输入文件:
 *
 * 输出文件: cbclient16.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test16(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 16;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->TriggerA();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_CTest_OnTriggerA
        || g_pObj != piServer || g_n != 1) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    piServer->TriggerB();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_CTest_OnTriggerC
        || g_pObj != piServer || g_n != 2) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 6) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:为多个不同的事件注册同一个回调方法，分别测试了类方法和全局方法；
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 17
 *
 * 输入文件:
 *
 * 输出文件: cbclient17.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test17(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 17;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->TriggerA();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 1) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    piServer->TriggerB();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 2) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    piServer->TriggerC();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 3) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 6) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:为多个不同的事件分别都注册多个不同的回调方法，包括类方法和全局方法；
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 18
 *
 * 输入文件:
 *
 * 输出文件: cbclient18.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test18(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 18;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerC || g_pObj != piServer || g_n != 3) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 18) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:注册回调方法后，中途撤销（RemoveHandler），再进行回调测试；
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 19
 *
 * 输入文件:
 *
 * 输出文件: cbclient19.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test19(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 19;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;
    g_pCTest = this;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_CTest_OnTriggerC || g_pObj != piServer || g_n != 3) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }

    piServer->Trigger6(1);
    CThread::Sleep(100, NULL);
    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerC || g_pObj != piServer || g_n != 3) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }

    piServer->Trigger6(2);
    CThread::Sleep(100, NULL);
    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerB || g_pObj != piServer || g_n != 3) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        TPERR("ERROR!\n");
        goto Exit;
    }
    if (g_count != 16) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        TPERR("ERROR!\n");
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:Client端另起线程，与主线程同时调用Server端的会进行回调的方法；
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 20
 *
 * 输入文件:
 *
 * 输出文件: cbclient20.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
ECode Thread20(void* pArg)
{
    ICBServer* piServer = (ICBServer*)pArg;
    for (int i = 0; i < 20; i++) {
//        printf("\t\tTrigger1::%d", i);
        piServer->Trigger1();
        CThread::Sleep(100, NULL);
    }
    return NOERROR;
}

int CTest::Test20(int argc, char *argv[])
{
    ICBServer* piServer = NULL;
    IThread* pThread = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 20;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }
    ec = CThread::New(Thread20, (void*)piServer, 0, &pThread);
    if (ec != NOERROR) {
        TPERR("CThread::New failed. ec = %x.\n", ec);
        goto Exit;
    }

    CThread::Sleep(100, NULL);
    piServer->Trigger2();

    pThread->Join(INFINITE, NULL);
    CThread::Sleep(500, NULL);

    if (g_flag != MethodFlag_CTest_OnTriggerC
        || g_pObj != piServer || g_n != 3) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 180) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    pThread->Release();
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (pThread) {
        pThread->Join(INFINITE, NULL);
        pThread->Release();
    }
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:调用Server端的方法，该方法会在Server端另起线程，与主线程同时进行回调
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 21
 *
 * 输入文件:
 *
 * 输出文件: cbclient21.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test21(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 21;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->Trigger3();
    CThread::Sleep(100, NULL);

    if (g_flag != MethodFlag_CTest_OnTriggerC
        || g_pObj != piServer || g_n != 33) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 180) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:Client端与Server端都另起线程，同时进行回调
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 22
 *
 * 输入文件:
 *
 * 输出文件: cbclient22.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test22(int argc, char *argv[])
{
    ICBServer* piServer = NULL;
    IThread* pThread = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 22;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }
    ec = CThread::New(Thread20, (void*)piServer, 0, &pThread);
    if (ec != NOERROR) {
        TPERR("CThread::New failed. ec = %x.\n", ec);
        goto Exit;
    }

    CThread::Sleep(100, NULL);
    piServer->Trigger3();
    CThread::Sleep(100, NULL);

    pThread->Join(INFINITE, NULL);

    if (g_count != 300) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    pThread->Release();
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (pThread) {
        pThread->Join(INFINITE, NULL);
        pThread->Release();
    }
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:多进程对回调进行测试
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 23
 *
 * 输入文件:
 *
 * 输出文件: cbclient23.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test23(int argc, char *argv[])
{
    ICBServer* piServer = NULL;
    IProcess* pIProc = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 23;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }
    ec = CSystem::RegisterRunningObject("CBServer", (IInterface*)piServer);
    if (ec != NOERROR) {
        TPERR("RegisterRunningObject failed. ec = %x.\n", ec);
        goto Exit;
    }

    ec = CProcess::New("cbclient2.exe", "", &pIProc);
    if (ec != NOERROR) {
        TPERR("CProcess::New failed. ec = %x.\n", ec);
        CSystem::UnregisterRunningObject("CBServer");
        goto Exit;
    }

    CThread::Sleep(100, NULL);
    piServer->Trigger3();
    CThread::Sleep(100, NULL);

    CSystem::UnregisterRunningObject("CBServer");

    if (g_flag != MethodFlag_CTest_OnTriggerC ||
        g_pObj != piServer || g_n != 33) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    CThread::Sleep(1000, NULL);
    if (g_count != 120) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    pIProc->WaitForExit(INFINITE, NULL);
    CCBServer::RemoveAllCallbacks(piServer);
    pIProc->Release();
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (pIProc) {
        pIProc->WaitForExit(INFINITE, NULL);
        pIProc->Release();
    }
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:注册回调方法后，调用RemoveAllCallbacks方法，再进行回调测试
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 24
 *
 * 输入文件:
 *
 * 输出文件: cbclient24.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test24(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 24;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_CTest_OnTriggerC || g_pObj != piServer || g_n != 3) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 6) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }

    g_flag = (MethodFlag)0;
    g_pObj = NULL;
    g_n = 0;
    g_count = 0;

    piServer->Trigger6(3);
    CThread::Sleep(100, NULL);
    piServer->Trigger1();
    CThread::Sleep(100, NULL);

    if (g_flag != 0 || g_pObj != NULL || g_n != 0 || g_count != 0) {
        TPERR("CallBack error! Should be Removed already.\n");
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:为事件注册多个回调函数，然后一个个的进行Remove，并进行回调测试，
 *          直到全部Remove完，再次Remove；
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 25
 *
 * 输入文件:
 *
 * 输出文件: cbclient25.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test25(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 25;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 1) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 3) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    g_flag = (MethodFlag)0;
    g_pObj = NULL;
    g_n = 0;
    g_count = 0;

    piServer->Trigger6(4);
    CThread::Sleep(100, NULL);
    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 1) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 2) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    g_flag = (MethodFlag)0;
    g_pObj = NULL;
    g_n = 0;
    g_count = 0;

    piServer->Trigger6(4);
    CThread::Sleep(100, NULL);
    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 1) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 1) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    g_flag = (MethodFlag)0;
    g_pObj = NULL;
    g_n = 0;
    g_count = 0;

    piServer->Trigger6(4);
    CThread::Sleep(100, NULL);
    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != 0 || g_pObj != NULL || g_n != 0 || g_count != 0) {
        TPERR("CallBack error! Should be Removed already.\n");
        goto Exit;
    }

    piServer->Trigger6(4);
    CThread::Sleep(100, NULL);
    piServer->Trigger1();
    CThread::Sleep(100, NULL);
    if (g_flag != 0 || g_pObj != NULL || g_n != 0 || g_count != 0) {
        TPERR("CallBack error! Should be Removed already.\n");
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
       piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:对两个对象进行回调的测试
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 26
 *
 * 输入文件:
 *
 * 输出文件: cbclient26.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test26(int argc, char *argv[])
{
    ICBServer* piServer = NULL;
    ICBServer* piServer2 = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }
    ec = CCBServer::NewInContext(CTX_DIFF_DOMAIN, &piServer2);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 26;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;
    raArg.apIObj[1] = piServer2;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer2->Trigger2();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer2 || g_n != 21) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 10) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }

    piServer->Trigger2();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 21) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 20) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    CCBServer::RemoveAllCallbacks(piServer2);
    piServer2->Release();
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    CCBServer::RemoveAllCallbacks(piServer2);
    if (piServer2) {
        piServer2->Release();
    }
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:测试多个参数的回调
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 27
 *
 * 输入文件:
 *
 * 输出文件: cbclient27.out
 *
 * 结果检查: Dailybuild
 *
 * 补充说明:
 *
 */
int CTest::Test27(int argc, char *argv[])
{
    ICBServer* piServer = NULL;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 27;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    piServer->Trigger4();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTrigger || g_pObj != piServer || g_n != 41) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }

    piServer->Trigger5();
    CThread::Sleep(100, NULL);
    if (g_flag != MethodFlag_OnTrigger || g_pObj != piServer || g_n != 92) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }

    if (g_count != 0) {
        TPERR("CallBack error! g_count:%d\n See OnTrigger!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:连续执行AddHandler和RemoveHandler 100000次，验证是否能正常运行完
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 28
 *
 * 输入文件:
 *
 * 输出文件: cbclient28.out
 *
 * 结果检查: Manual
 *
 * 补充说明:
 *
 */
int CTest::Test28(int argc, char *argv[])
{
    ICBServer* piServer = NULL;
    int i;

    ECode ec = CCBServer::New(&piServer);
    if (ec != NOERROR) {
        TPERR("CCBServer::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 28;
    raArg.pCTest = this;
    raArg.apIObj[0] = piServer;

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    for (i = 0; i < 100000; i++) {
        piServer->Trigger1();
        CThread::Sleep(100, NULL);

        piServer->Trigger6(5);
        CThread::Sleep(100, NULL);

        piServer->Trigger1();
        CThread::Sleep(100, NULL);

        piServer->Trigger6(6);
        CThread::Sleep(100, NULL);
    }
    if (g_flag != MethodFlag_OnTriggerA || g_pObj != piServer || g_n != 1) {
        TPERR("CallBack error!\n");
        TPERR("g_flag = %d; g_pObj = %x; g_n = %d.\n", g_flag, g_pObj, g_n);
        goto Exit;
    }
    if (g_count != 100000) {
        TPERR("CallBack count error! g_count = %d!\n", g_count);
        goto Exit;
    }
    CCBServer::RemoveAllCallbacks(piServer);
    piServer->Release();
    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(piServer);
    if (piServer) {
        piServer->Release();
    }
    return 0;
}

/*
 * 测试简述:
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 29
 *
 * 输入文件:
 *
 * 输出文件: cbclient29.out
 *
 * 结果检查: DailyBuild
 *
 * 补充说明:
 *
 */
int CTest::Test29(int argc, char *argv[])
{
    IHello121* pIHello = NULL;
    ECode ec;
    int i = 0;
    g_count = 0;
    ec = CCallback121::New(&pIHello);
    if (ec != NOERROR) {
        TPERR("CCallback121::New Error, ec = %x\n", ec);
        goto Exit;
    }

    RoutineArg raArg;
    raArg.nIndex = 29;
    raArg.pCTest = this;
    raArg.apIObj[0] = pIHello;
    pIHello->AddRef();

    ec = SubRoutine(&raArg);
    if (ec != NOERROR) {
        TPERR("SubRoutine Failed! ec = %x\n", ec);
        goto Exit;
    }

    for (i = 1; i < 28; i++) {
        pIHello->Hello121(i);
    }
    CThread::Sleep(1000, NULL);

    CCBServer::RemoveAllCallbacks(pIHello);
    pIHello->Release();
    if (52 != g_count) {
        TPERR("Callback Count error! g_count:%d!\n", g_count);
    }
    else {
        TPOK;
    }
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(pIHello);
    if (pIHello) {
        pIHello->Release();
    }
    return 0;
}

/*
 * 测试简述:
 *
 * 预期结果:
 *
 * 运行方式: cbclient.exe 30
 *
 * 输入文件:
 *
 * 输出文件: cbclient30.out
 *
 * 结果检查: DailyBuild
 *
 * 补充说明:
 *
 */
int CTest::Test30(int argc, char *argv[])
{
    ICalBakSvr* pIServer = NULL;
    ECode ec = CCalBakSvr::New(&pIServer);
    if (ec != NOERROR) {
        TPERR("CCalBakSvr::New Error, ec = %x\n", ec);
        goto Exit;
    }
    ec = CCalBakSvr::AddOnEnterCallback(pIServer, this, &CTest::OnEnter);
    if (ec != NOERROR) {
        TPERR("CCalBakSvr Add Callback Failed! ec = %x\n", ec);
        goto Exit;
    }
    CThread::Sleep(100, NULL);
    pIServer->Trigger(0, NULL);
    CThread::Sleep(1000, NULL);
    CCBServer::RemoveAllCallbacks(pIServer);
    pIServer->Release();

    TPOK;
    return 0;

Exit:
    CCBServer::RemoveAllCallbacks(pIServer);
    if (pIServer) {
        pIServer->Release();
    }
    return NOERROR;
}
