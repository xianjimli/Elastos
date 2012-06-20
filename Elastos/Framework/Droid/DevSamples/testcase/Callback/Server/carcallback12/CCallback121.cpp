#include "CCallback121.h"
#include "stdlib.h"
#include "stdio.h"
#include "limits.h"
//#include "CharacterSetTest.h"

ECode CCallback121::Hello121(
    /* [in] */ Int32 nType)
{
    IEvent * pIEvent = NULL;
    IMutex * pIMutex = NULL;
    IThread* pIThread = NULL;

    switch (nType) {
        case 1: {
            TestEnum emInfo = TestEnum_Three;
            Callback::OnTriggerEnum(emInfo);
            break;
        }
        case 2: {
            TestStruct stInfo;
            stInfo.nType = nType;
            stInfo.nx = 6;
            stInfo.ny = 12;

            Callback::OnTriggerStruct(stInfo);
            break;
        }
        case 3: {
            Byte byInfo = 255;
            Callback::OnTriggerByte(byInfo);
            break;
        }
        case 4: {
            Int16 i16Info = -32768;
            Callback::OnTriggerInt16(i16Info);
            break;
        }
        case 5: {
            Int32 i32Info = 2147483647;
            Callback::OnTriggerInt32(i32Info);
            break;
        }
        case 6: {
            Int64 i64Info = 9223372036854775807LL;
            Callback::OnTriggerInt64(i64Info);
            break;
        }
        case 7: {
            Float fInfo = 1111.1111f;
            Callback::OnTriggerFloat(fInfo);
            break;
        }
        case 8: {
            Double dInfo = 2222.2222;
            Callback::OnTriggerDouble(dInfo);
            break;
        }
        case 9: {
            Boolean bInfo = TRUE;
            Callback::OnTriggerBoolean(bInfo);
            break;
        }
        case 10: {
            Char8 acInfo = '@';
            Callback::OnTriggerChar8(acInfo);
            break;
        }
        case 11: {
            Char16 wcInfo = '中';
            Callback::OnTriggerChar16(wcInfo);
            break;
        }
        case 12: {
            String asInfo = "AString";
            Callback::OnTriggerAString(asInfo);
            break;
        }
        case 13: {
            String wsInfo = "ＷＳｔｒｉｎｇ";
            Callback::OnTriggerString(wsInfo);
            break;
        }
        case 14: {
            BufferOf_<Char8,10> acaInfo;
            acaInfo.SetUsed(0);
            Char8 acTmp[5] = {'a', 'b', 'x', 'y', 'z'};
            acaInfo.Append(acTmp, 5);

            Callback::OnTriggerACArray(acaInfo);
            break;
        }
        case 15: {
            BufferOf_<Char16,10> wcaInfo;
            wcaInfo.SetUsed(0);
            Char16 wcTmp[5] = {
                    '一',
                    '二',
                    '三',
                    '四',
                    '五'};
            wcaInfo.Append(wcTmp, 5);

            Callback::OnTriggerWCArray(wcaInfo);
            break;
        }
        case 16: {
            BufferOf_<String,10> asaInfo;
            asaInfo.SetUsed(0);
            String asTmp[5] = {"1", "22", "333", "4444", "55555"};
            asaInfo.Append(asTmp, 5);

            Callback::OnTriggerASArray(asaInfo);
            break;
        }
        case 17: {
            BufferOf_<String,10> wsaInfo;
            wsaInfo.SetUsed(0);
            String wsTmp[5] = {
                    "伟大",
                    "渺小",
                    "中庸",
                    "可怜",
                    "欢乐"};
            wsaInfo.Append(wsTmp, 5);

            Callback::OnTriggerWSArray(wsaInfo);
            break;
        }
        case 18: {
            BufferOf_<Byte,10> baInfo;
            baInfo.SetUsed(0);
            Byte byTmp[5] = {0, 1, 127, 128, 255};
            baInfo.Append(byTmp, 5);

            Callback::OnTriggerByteArray(baInfo);
            break;
        }
        case 19: {
            BufferOf_<Int16, 10> i16aInfo;
            i16aInfo.SetUsed(0);
            Int16 i16Tmp[5] = {0, 1, -1, -32768, 32767};
            i16aInfo.Append(i16Tmp, 5);

            Callback::OnTriggerInt16Array(i16aInfo);
            break;
        }
        case 20: {
            BufferOf_<Int32,10> i32aInfo;
            i32aInfo.SetUsed(0);
            Int32 i32Tmp[5] = {0, 1, -1, (Int32)0x80000000, 2147483647};
            i32aInfo.Append(i32Tmp, 5);

            Callback::OnTriggerInt32Array(i32aInfo);
            break;
        }
        case 21: {
            BufferOf_<Int64, 10> i64aInfo;
            i64aInfo.SetUsed(0);
            Int64 i64Tmp[5] = {0, 1, -1,
                    -9223372036854775808LL, 9223372036854775807LL};
            i64aInfo.Append(i64Tmp, 5);

            Callback::OnTriggerInt64Array(i64aInfo);
            break;
        }
        case 22: {
            BufferOf_<Float,10> faInfo;
            faInfo.SetUsed(0);
            Float fTmp[5] = {0.0f, -0.000000001f, 0.0000001f,
                    1111.1111f, -9999.9999f};
            faInfo.Append(fTmp, 5);

            Callback::OnTriggerFloatArray(faInfo);
            break;
        }
        case 23: {
            BufferOf_<Double, 10> daInfo;
            daInfo.SetUsed(0);
            Double dTmp[5] = {0.0, -0.000000001, 0.0000001,
                    1111.1111, -9999.9999};
            daInfo.Append(dTmp, 5);

            Callback::OnTriggerDoubleArray(daInfo);
            break;
        }
        case 24: {
            BufferOf_<PInterface, 10> oaInfo;
            ECode ec = NOERROR;

            ec = CEvent::New(FALSE, 0, &pIEvent);
            if (NOERROR != ec) {
                printf("New CEvent error! ec=%x\n", ec);
                goto Exit;
            }
            ec = CMutex::New(&pIMutex);
            if (NOERROR != ec) {
                printf("New CMutex error! ec=%x\n", ec);
                goto Exit;
            }

            pIThread = CThread::GetCurrent();

            oaInfo.SetUsed(0);

            IInterface* ioTmp[5] = {(IObject*)this, pIEvent, pIMutex, pIThread, NULL};
            oaInfo.Append(ioTmp, 5);
            pIEvent->AddRef();
            pIMutex->AddRef();
            pIThread->AddRef();

            Callback::OnTriggerObjectArray(oaInfo);

            pIEvent->Release();
            pIMutex->Release();
            break;
        }
        case 25: {
            BufferOf_<TestStruct, 10> saInfo;
            saInfo.SetUsed(0);
            TestStruct st[5] = {{1, 1, 1}, {2, 2, 2},
                    {3, 3, 3}, {4, 4, 4}, {5, 5, 5}};
            saInfo.Append(st, 5);
            Callback::OnTriggerArrayOf(saInfo);
            break;
        }
        case 26: {
            Callback::OnTriggerInterface(this);
            break;
        }
        case 27: {
            TestEnum emInfo = TestEnum_Three;

            TestStruct stInfo;
            stInfo.nType = nType;
            stInfo.nx = 6;
            stInfo.ny = 12;

            Byte byInfo = 255;

            Int16 i16Info = -32768;

            Int32 i32Info = 2147483647;

            Int64 i64Info = 9223372036854775807LL;

            Float fInfo = 1111.1111f;

            Double dInfo = 2222.2222;

            Boolean bInfo = TRUE;

            Char8 acInfo = '@';

            Char16 wcInfo = '中';

            String asInfo = "AString";

            String wsInfo = "ＷＳｔｒｉｎｇ";

            BufferOf_<Char8,10> acaInfo;
            acaInfo.SetUsed(0);
            Char8 acTmp[5] = {'a', 'b', 'x', 'y', 'z'};
            acaInfo.Append(acTmp, 5);

            BufferOf_<Char16,10> wcaInfo;
            wcaInfo.SetUsed(0);
            Char16 wcTmp[5] = {
                    '一',
                    '二',
                    '三',
                    '四',
                    '五'};
            wcaInfo.Append(wcTmp, 5);

            BufferOf_<String,10> asaInfo;
            asaInfo.SetUsed(0);
            String asTmp[5] = {"1", "22", "333", "4444", "55555"};
            asaInfo.Append(asTmp, 5);

            BufferOf_<String,10> wsaInfo;
            wsaInfo.SetUsed(0);
            String wsTmp[5] = {
                    "伟大",
                    "渺小",
                    "中庸",
                    "可怜",
                    "欢乐"};
            wsaInfo.Append(wsTmp, 5);

            BufferOf_<Byte,10> baInfo;
            baInfo.SetUsed(0);
            Byte byTmp[5] = {0, 1, 127, 128, 255};
            baInfo.Append(byTmp, 5);

            BufferOf_<Int16, 10> i16aInfo;
            i16aInfo.SetUsed(0);
            Int16 i16Tmp[5] = {0, 1, -1, -32768, 32767};
            i16aInfo.Append(i16Tmp, 5);

            BufferOf_<Int32,10> i32aInfo;
            i32aInfo.SetUsed(0);
            int i32Tmp[5] = {0, 1, -1, (Int32)0x80000000, 2147483647};
            i32aInfo.Append(i32Tmp, 5);

            BufferOf_<Int64, 10> i64aInfo;
            i64aInfo.SetUsed(0);
            Int64 i64Tmp[5] = {0, 1, -1,
                    -9223372036854775808LL, 9223372036854775807LL};
            i64aInfo.Append(i64Tmp, 5);

            BufferOf_<Float, 10> faInfo;
            faInfo.SetUsed(0);
            Float fTmp[5] = {0.0f, -0.000000001f, 0.0000001f,
                    1111.1111f, -9999.9999f};
            faInfo.Append(fTmp, 5);

            BufferOf_<Double, 10> daInfo;
            daInfo.SetUsed(0);
            Double dTmp[5] = {0.0, -0.000000001, 0.0000001,
                    1111.1111, -9999.9999};
            daInfo.Append(dTmp, 5);

            BufferOf_<TestStruct, 10> saInfo;
            saInfo.SetUsed(0);
            TestStruct st[5] = {{1, 1, 1}, {2, 2, 2},
                    {3, 3, 3}, {4, 4, 4}, {5, 5, 5}};
            saInfo.Append(st, 5);

            BufferOf_<IInterface*, 10> oaInfo;
            ECode ec = NOERROR;

            ec = CEvent::New(FALSE, 0, &pIEvent);
            if (NOERROR != ec) {
                printf("New CEvent error! ec=%x\n", ec);
                goto Exit;
            }
            ec = CMutex::New(&pIMutex);
            if (NOERROR != ec) {
                printf("New CMutex error! ec=%x\n", ec);
                goto Exit;
            }

            pIThread = CThread::GetCurrent();

            oaInfo.SetUsed(0);

            IInterface* ioTmp[5] = {(IObject*)this, pIEvent, pIMutex, pIThread, NULL};
            oaInfo.Append(ioTmp, 5);
            pIEvent->AddRef();
            pIMutex->AddRef();
            pIThread->AddRef();

            Callback::OnTriggerAll(emInfo, stInfo, byInfo, i16Info, i32Info, i64Info,
                    fInfo, dInfo, bInfo, acInfo, wcInfo, asInfo, wsInfo,
                    acaInfo, wcaInfo, asaInfo, wsaInfo, baInfo, i16aInfo,
                    i32aInfo, i64aInfo, faInfo, daInfo, oaInfo, saInfo, this);

            pIEvent->Release();
            pIMutex->Release();
            break;
        }
        // case 28: {
            // Int8 i8Info = -128;
            // OnTriggerInt8(i8Info);
            // break;
        // }
        // case 29: {
            // UInt16 u16Info = 65535;
            // OnTriggerUInt16(u16Info);
            // break;
        // }
        // case 30: {
            // UInt32 u32Info = 4294967295u;
            // OnTriggerUInt32(u32Info);
            // break;
        // }
        // case 31: {
            // UInt64 u64Info = ULLONG(18446744073709551615);
            // OnTriggerUInt64(u64Info);
            // break;
        // }
        // case 32: {
            // BufferOf_<Int8, 10> i8aInfo;
            // i8aInfo.SetUsed(0);
            // Int8 i8Tmp[5] = {0, 1, -1, -128, 127};
            // i8aInfo.Append(i8Tmp, 5);

            // OnTriggerInt8Array(i8aInfo);
            // break;
        // }
        // case 33: {
            // BufferOf_<UInt16, 10> u16aInfo;
            // u16aInfo.SetUsed(0);
            // UInt16 u16Tmp[5] = {0, 1, 32767, 32768, 65535};
            // u16aInfo.Append(u16Tmp, 5);

            // OnTriggerUInt16Array(u16aInfo);
            // break;
        // }
        // case 34: {
            // BufferOf_<UInt32,10> u32aInfo;
            // u32aInfo.SetUsed(0);
            // UInt32 u32Tmp[5] = {0, 1, 2147483647, 2147483648u, 4294967295u};
            // u32aInfo.Append(u32Tmp, 5);

            // OnTriggerUInt32Array(u32aInfo);
            // break;
        // }
        // case 35: {
            // BufferOf_<UInt64, 10> u64aInfo;
            // u64aInfo.SetUsed(0);
            // UInt64 u64Tmp[5] = {0, 1, LLONG(9223372036854775807),
                    // ULLONG(9223372036854775808), ULLONG(18446744073709551615)};
            // u64aInfo.Append(u64Tmp, 5);

            // OnTriggerUInt64Array(u64aInfo);
            // break;
        // }
        // case 36: {
            // Int8 i8Info = -128;

            // UInt16 u16Info = 65535;

            // UInt32 u32Info = 4294967295u;

            // UInt64 u64Info = ULLONG(18446744073709551615);

            // BufferOf_<Int8, 10> i8aInfo;
            // i8aInfo.SetUsed(0);
            // Int8 i8Tmp[5] = {0, 1, -1, -128, 127};
            // i8aInfo.Append(i8Tmp, 5);

            // BufferOf_<UInt16, 10> u16aInfo;
            // u16aInfo.SetUsed(0);
            // UInt16 u16Tmp[5] = {0, 1, 32767, 32768, 65535};
            // u16aInfo.Append(u16Tmp, 5);

            // BufferOf_<UInt32,10> u32aInfo;
            // u32aInfo.SetUsed(0);
            // UInt32 u32Tmp[5] = {0, 1, 2147483647, 2147483648u, 4294967295u};
            // u32aInfo.Append(u32Tmp, 5);

            // BufferOf_<UInt64, 10> u64aInfo;
            // u64aInfo.SetUsed(0);
            // UInt64 u64Tmp[5] = {0, 1, LLONG(9223372036854775807),
                    // ULLONG(9223372036854775808), ULLONG(18446744073709551615)};
            // u64aInfo.Append(u64Tmp, 5);

            // OnTriggerAllLocal(i8Info, u16Info, u32Info, u64Info,
                    // i8aInfo, u16aInfo, u32aInfo, u64aInfo);
            // break;
        // }
        default:
            printf("CANNOT TOUCH! nType:%d\n", nType);
            break;
    }

    return NOERROR;

Exit:
    if (pIEvent) pIEvent->Release();
    if (pIMutex) pIMutex->Release();
    return 1;
}
