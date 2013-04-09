//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#ifdef _linux
#include <sys/io.h>
#else
#include <io.h>
#endif
#include "carc.h"
#include "clsutil.h"

#ifdef _linux
#define _stricmp strcasecmp
#define _MAX_PATH 256
#endif

enum InternalReturnValue {
    Ret_Continue        = 0,
    Ret_ContinueOnError = -1,
    Ret_AbortOnError    = -2,
};

enum Properties {
    Prop_deprecated     = 0x0001,
    Prop_local          = 0x0002,
    Prop_private        = 0x0004,
};

static FILE *s_pFile;
static CLSModule *s_pModule = NULL;
static const char *s_pszSourcePath = "";
static const char *s_pszFactoryUrl = "";
static char *s_pszCurrentPath = "";
static char *s_pszCarFullName = "";
ClassAttrib s_DefaultThreadModel = ClassAttrib_freethreaded;
bool s_bLenientNaming = false;
bool s_bInKernel = false;

static const IID EIID_IInterface = \
{0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x66}};

static const IID EIID_IObject = \
{0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x68}};

static const IID EIID_IAspect = \
{0x00010002,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x66}};

static const IID EIID_IClassObject = \
{0x00000001,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};

static const CLSID CLSID_XOR_CallbackSink = \
/* e724df56-e16a-4599-8edd-a97ab245d583 */
{0xe724df56,0xe16a,0x4599,{0x8e,0xdd,0xa9,0x7a,0xb2,0x45,0xd5,0x83}};

void SetFactoryUrl(const char *pszUrl)
{
    s_pszFactoryUrl = pszUrl;
}

void SetSourcePath(const char *pszPath)
{
    s_pszSourcePath = pszPath;
}

void SetDefaultThreadModel(ClassAttrib attrib)
{
    s_DefaultThreadModel = attrib;
}

int GetCurrentCarPath(const char *pszPath, char **ppszDest)
{
    int n = strlen(pszPath) - 1;

    while (n >= 0) {
        if (IS_PATH_SEPARATOR(*(pszPath + n))) {
            if ((*ppszDest) && ('\0' != **ppszDest))
                delete[] *ppszDest;

            *ppszDest = new char[n + 1];
            if (NULL == *ppszDest) {
                ErrorReport(CAR_E_OutOfMemory);
                return Ret_AbortOnError;
            }

            memcpy(*ppszDest, pszPath, n);
            (*ppszDest)[n] = 0;

            break;
        }
        n--;
    }

    return Ret_Continue;
}

const char *GenerateInternalName(const char *s)
{
    static char s_szName[32];
    static int s_nSerial = 1;

    sprintf(s_szName, "__Internal_%s%d", s, s_nSerial++);
    return s_szName;
}

int Hexstr2Int(const char *pszHex)
{
    int nResult = 0;

    assert(pszHex[0] == '0' && (pszHex[1] == 'x' || pszHex[1] == 'X'));

    pszHex += 2; // skip prefix "0x"
    while (*pszHex) {
        nResult *= 16;

        if (isdigit(*pszHex)) {
            nResult += *pszHex - '0';
        }
        else if (*pszHex >= 'a' && *pszHex <= 'f') {
            nResult += *pszHex - 'a' + 10;
        }
        else {
            nResult += *pszHex - 'A' + 10;
        }
        pszHex++;
    }
    return nResult;
}

#define IS_ECODE(t)    ((t).type == Type_ECODE && (t).nPointer == 0)

BOOL IsValidParamTypeAttrib(
    const CLSModule *pModule, const TypeDescriptor *pType,
    const DWORD dwAttribs, const BOOL isLocal, const BOOL isDeprecated)
{
    TypeDescriptor type;

    switch (pType->type) {
        case Type_alias:
            GetOriginalType(pModule, pType, &type);
            return IsValidParamTypeAttrib(pModule, &type, dwAttribs, isLocal, isDeprecated);

        /*
         * local: [in] PVoid ;
         * local: [out] PVoid;
         * local: [out] PVoid *;
         */
        case Type_PVoid:
            if (isLocal) {
                if (dwAttribs & ParamAttrib_in) {
                    if (0 == pType->nPointer) return TRUE;
                    ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have '*' when it modifies [in] parameter.");
                    return FALSE;
                }
                if (dwAttribs & ParamAttrib_out) {
                    if (dwAttribs & ParamAttrib_callee) {
                        ErrorReport(CAR_E_IllegalTypeUsage, "Attribute [callee] is illegal.");
                        return FALSE;
                    }

                    if (1 >= pType->nPointer) return TRUE;

                    ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than "
                            "one '*' when it modifies [out] parameter.");
                    return FALSE;
                }
            }
            ErrorReport(CAR_E_IllegalTypeUsage, "Type is a local type.");
            return FALSE;

        /*
         * local|unlocal: [in] IInterface *;
         * local|unlocal, deprecated: [in] IInterface **; // warning
         * local|unlocal: [out] IInterface **;
         */
        case Type_interface:
            if ((pModule->ppInterfaceDir[pType->sIndex]->pDesc->dwAttribs
                    & InterfaceAttrib_local) && (!isLocal)) {
                ErrorReport(CAR_E_IllegalTypeUsage, "Type is a local type.");
                return FALSE;
            }

            if (dwAttribs & ParamAttrib_in) {
                if (1 == pType->nPointer) return TRUE;
                if (2 == pType->nPointer) {
                    if (isDeprecated) {
                        ErrorReport(CAR_W_DecrepitTypeUsage, "The usage is deprecated, and will be removed.");
                        return TRUE;
                    }
                    ErrorReport(CAR_E_IllegalTypeUsage, "The usage is deprecated, you should use keyword \"deprecated\".");
                    return FALSE;
                }
                ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than two '*' "
                        "when it modifies [in] parameter.");
                return FALSE;
            }

            if (dwAttribs & ParamAttrib_out) {
                if ((2 == pType->nPointer)
                        && !(dwAttribs & ParamAttrib_callee)) return TRUE;
                ErrorReport(CAR_E_IllegalTypeUsage, "Type should have only two '*' "
                        "when it modifies [out] parameter.");
                return FALSE;
            }

            return FALSE;

        /*
         * local|unlocal: [in|out] ArrayOf<Type>;
         * local|unlocal: [in|out] ArrayOf<Type, size>;
         * local|: [out, callee] ArrayOf<Type> *;
         * local|unlocal, deprecated: [in] ArrayOf<Type> *;   //warning
         * local|unlocal, deprecated: [in] ArrayOf<Type, size> *; //warning
         */
        case Type_ArrayOf:
        case Type_BufferOf:
            if (IsLocalCarQuintet(pModule, pType->pNestedType, dwAttribs) && (!isLocal)) {
                ErrorReport(CAR_E_IllegalTypeUsage, "Type is a local type.");
                return FALSE;
            }
            //no break or return here

        case Type_MemoryBuf:
            if (dwAttribs & ParamAttrib_in) {
                if (0 == pType->nPointer) return TRUE;
                if (1 == pType->nPointer) {
                    if (isDeprecated) {
                        ErrorReport(CAR_W_DecrepitTypeUsage, "The usage is deprecated, and will be removed.");
                        return TRUE;
                    }
                    ErrorReport(CAR_E_IllegalTypeUsage, "The usage is deprecated, you should use keyword \"deprecated\".");
                    return FALSE;
                }
                ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than one '*'.");
                return FALSE;
            }
            if (dwAttribs & ParamAttrib_out) {
                if (0 == pType->nPointer) {
                    if (!(dwAttribs & ParamAttrib_callee)) return TRUE;
                    ErrorReport(CAR_E_IllegalTypeUsage, "Attribute [callee] is illegal.");
                    return FALSE;
                }
                if (1 == pType->nPointer) {
                    if (dwAttribs & ParamAttrib_callee) return TRUE;
                    ErrorReport(CAR_E_IllegalTypeUsage, "Attribute should be [callee].");
                    return FALSE;
                }
                ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than one '*'.");
                return FALSE;
            }
            return FALSE;

        /*
         * local|unlocal: [out] StringBuf<>;
         * local|unlocal: [out] StringBuf<5>;
         * local|: [out, callee] StringBuf<>*;
         */
        case Type_StringBuf:
            if (dwAttribs & ParamAttrib_out) {
                if (0 == pType->nPointer) {
                    if (!(dwAttribs & ParamAttrib_callee)) return TRUE;
                    ErrorReport(CAR_E_IllegalTypeUsage, "Attribute [callee] is illegal.");
                    return FALSE;
                }
                if (1 == pType->nPointer) {
                    if (dwAttribs & ParamAttrib_callee) return TRUE;
                    ErrorReport(CAR_E_IllegalTypeUsage, "Attribute should be [callee].");
                    return FALSE;
                }
            }
            ErrorReport(CAR_E_IllegalTypeUsage, "Type can only modify [out] parameter.");
            return FALSE;

        /*
         * local: [in] UInt16;
         * local, deprecated: [in] UInt16 *;   //warning
         * local: [out] UInt16 *;
         * local, deprecated: [out] UInt16 **;   //warning
         */
        case Type_Int8:
        case Type_UInt16:
        case Type_UInt32:
        case Type_UInt64:
            if (isLocal) {
                if (dwAttribs & ParamAttrib_in) {
                    if (0 == pType->nPointer) return TRUE;
                    if (1 == pType->nPointer) {
                        if (isDeprecated) {
                            ErrorReport(CAR_W_DecrepitTypeUsage, "The usage is deprecated, and will be removed.");
                            return TRUE;
                        }
                        ErrorReport(CAR_E_IllegalTypeUsage, "The usage is deprecated, you should use keyword \"deprecated\".");
                        return FALSE;
                    }
                    ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than one '*'.");
                    return FALSE;
                }
                if (dwAttribs & ParamAttrib_out) {
                    if (dwAttribs & ParamAttrib_callee) {
                        ErrorReport(CAR_E_IllegalTypeUsage, "Attribute [callee] is illegal.");
                        return FALSE;
                    }

                    if (1 == pType->nPointer) return TRUE;

                    if (2 == pType->nPointer) {
                        if (isDeprecated) {
                            ErrorReport(CAR_W_DecrepitTypeUsage, "The usage is deprecated, and will be removed.");
                            return TRUE;
                        }
                        ErrorReport(CAR_E_IllegalTypeUsage, "The usage is deprecated, you should use keyword \"deprecated\".");
                        return FALSE;
                    }
                    ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than two '*'.");
                    return FALSE;
                }
            }
            ErrorReport(CAR_E_IllegalTypeUsage, "Type is a local type.");
            return FALSE;

        /*
         * local|unlocal: [in] struct stname;
         * local|unlocal, deprecated: [in] struct stname *;  //warning
         * local|unlocal: [out] struct stname *;
         * local:         [out] struct stname **;
         */
        case Type_struct:
            //check if struct is local based on its elements;
            if (IsLocalStruct(pModule, pModule->ppStructDir[pType->sIndex]->pDesc)
                    && (!isLocal)) {
                ErrorReport(CAR_E_IllegalTypeUsage, "Type is a local type.");
                return FALSE;
            }

            if (dwAttribs & ParamAttrib_in) {
                if (0 == pType->nPointer) return TRUE;
                if (1 == pType->nPointer) {
                    if (isDeprecated) {
                        ErrorReport(CAR_W_DecrepitTypeUsage, "The usage is deprecated, and will be removed.");
                        return TRUE;
                    }
                    ErrorReport(CAR_E_IllegalTypeUsage, "The usage is deprecated, you should use keyword \"deprecated\".");
                    return FALSE;
                }
                ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than one '*'.");
                return FALSE;
            }
            if (dwAttribs & ParamAttrib_out) {
                if (dwAttribs & ParamAttrib_callee) {
                    ErrorReport(CAR_E_IllegalTypeUsage, "Attribute [callee] is illegal.");
                    return FALSE;
                }

                if (1 == pType->nPointer) return TRUE;

                if (2 == pType->nPointer) {
                    if (isLocal) return TRUE;
                    else {
                        ErrorReport(CAR_E_IllegalTypeUsage, "Type is a local type.");
                        return FALSE;
                    }
                }
                ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than two '*'.");
                return FALSE;
            }
            return FALSE;

        /*
         * local|unlocal: [in] type;
         * local|unlocal, deprecated: [in] type *;   //warning
         * local|unlocal: [out] type *;
         * local, deprecated: [out] type **;   //warning
         */
        default:
            if (dwAttribs & ParamAttrib_in) {
                if (0 == pType->nPointer) return TRUE;
                if (1 == pType->nPointer) {
                    if (isDeprecated) {
                        ErrorReport(CAR_W_DecrepitTypeUsage, "The usage is deprecated, and will be removed.");
                        return TRUE;
                    }
                    ErrorReport(CAR_E_IllegalTypeUsage, "The usage is deprecated, you should use keyword \"deprecated\".");
                    return FALSE;
                }
                ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than one '*'.");
                return FALSE;
            }
            if (dwAttribs & ParamAttrib_out) {
                if (dwAttribs & ParamAttrib_callee) {
                    ErrorReport(CAR_E_IllegalTypeUsage, "Attribute [callee] is illegal.");
                    return FALSE;
                }

                if (0 == pType->nPointer) {
                    ErrorReport(CAR_E_IllegalTypeUsage, "Type should have one or two '*'.");
                    return FALSE;
                }

                if (1 == pType->nPointer) return TRUE;

                if (2 == pType->nPointer) {
                    if (!isLocal) {
                        ErrorReport(CAR_E_IllegalTypeUsage, "Type is a local type.");
                        return FALSE;
                    }
                    if (isDeprecated) {
                        ErrorReport(CAR_W_DecrepitTypeUsage, "The usage is deprecated, and will be removed.");
                        return TRUE;
                    }
                    ErrorReport(CAR_E_IllegalTypeUsage, "The usage is deprecated, you should use keyword \"deprecated\".");
                    return FALSE;
                }
                ErrorReport(CAR_E_IllegalTypeUsage, "Type cann't have more than two '*'.");
                return FALSE;
            }
            return FALSE;
    }
}

int P_BaseType(CARToken token, TypeDescriptor *pType);

// TYPE        -> < BASE_TYPE | DEF_TYPE > [ s_star [ s_star ] ]
// DEF_TYPE    -> [ k_struct | k_enum ] ident
//
int P_Type(TypeDescriptor *pType)
{
    int n;
    CARToken token;

    token = GetToken(s_pFile);
    switch (token) {
        case Token_ident:
            if (RetrieveIdentifyType(
                g_szCurrentToken, s_pModule, pType, FALSE) < 0) {
                ErrorReport(CAR_E_UndefType, g_szCurrentToken);
                // continue on error
            }
            break;

        case Token_K_enum:
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_ExpectEnumName);
                return Ret_AbortOnError;
            }
            n = RetrieveEnum(g_szCurrentToken, s_pModule, FALSE);
            if (n < 0) {
                ErrorReport(CAR_E_NotFound, "enum", g_szCurrentToken);
                break; // continue on error
            }
            pType->type = Type_enum;
            pType->sIndex = n;
            break;

        case Token_K_struct:
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_ExpectStructName);
                return Ret_AbortOnError;
            }
            n = RetrieveStruct(g_szCurrentToken, s_pModule, FALSE);
            if (n < 0) {
                ErrorReport(CAR_E_NotFound, "struct", g_szCurrentToken);
                break; // continue on error
            }
            pType->type = Type_struct;
            pType->sIndex = n;
            break;

        default:
            if (P_BaseType(token, pType) == Ret_AbortOnError)
                return Ret_AbortOnError;
            break;
    }

    if (PeekToken(s_pFile) == Token_S_star) {
        GetToken(s_pFile);
        pType->nPointer++;
        if (PeekToken(s_pFile) == Token_S_star) {
            GetToken(s_pFile);
            pType->nPointer++;
        }
    }
    return Ret_Continue;
}

// ArrayOf      -> k_ArrayOf s_langle TYPE, SIZE s_rangle
// BufferOf     -> k_BufferOf s_langle TYPE, SIZE s_rangle
// MemoryBuf    -> k_MemoryBuf s_langle SIZE s_rangle
// StringBuf   -> k_StringBuf s_langle SIZE s_rangle
//
int P_CarQuient(TypeDescriptor *pType)
{
    int s, token;

    pType->nSize = -1;
    if (GetToken(s_pFile) != Token_S_langle) {
        ErrorReport(CAR_E_ExpectSymbol, "<");
        return Ret_AbortOnError;
    }

    if ((Type_ArrayOf == pType->type)
            || (Type_BufferOf == pType->type)
            || (Type_MemoryBuf == pType->type)){

        pType->pNestedType = new TypeDescriptor;
        if (!pType->pNestedType) {
            ErrorReport(CAR_E_OutOfMemory);
            return Ret_AbortOnError;
        }
        memset(pType->pNestedType, 0, sizeof(TypeDescriptor));
        pType->pNestedType->bNested = 1;

        if (Type_MemoryBuf == pType->type) {
            pType->pNestedType->type = Type_Byte;
        }
        else {
            if (P_Type(pType->pNestedType) == Ret_AbortOnError) {
                goto ErrorExit;
            }

            if ((Type_ArrayOf == pType->pNestedType->type)
                    || (Type_BufferOf == pType->pNestedType->type)
                    || (Type_MemoryBuf == pType->pNestedType->type)
                    || (Type_StringBuf == pType->pNestedType->type)) {
                ErrorReport(CAR_E_NestedCARQuient);
                return Ret_AbortOnError;
            }

            if (Token_S_comma == PeekToken(s_pFile)) {
                GetToken(s_pFile);
                token = PeekToken(s_pFile);
                if (!(Token_integer == token) && !(Token_ident == token)){
                    ErrorReport(CAR_E_IlleagalSizeType);    //< n > n must be an integer
                    return Ret_AbortOnError;
                }
            }
        }
    }

    token = GetToken(s_pFile);
    if (token == Token_integer) {
        pType->nSize = atoi(g_szCurrentToken);

        token = GetToken(s_pFile);
    }
    else if (token == Token_ident) {
        s = RetrieveConst(g_szCurrentToken, s_pModule, FALSE);
        if (s < 0) {
            ErrorReport(CAR_E_NotFound, "const", g_szCurrentToken);
            return Ret_AbortOnError;
        }
        if (s_pModule->ppConstDir[s]->type != INTEGER) {
            ErrorReport(CAR_E_NotIntegerConst, g_szCurrentToken);
            return Ret_AbortOnError;
        }
        pType->nSize = s_pModule->ppConstDir[s]->v.intValue.nValue;

        token = GetToken(s_pFile);
    }

    if (token != Token_S_rangle) {
        ErrorReport(CAR_E_ExpectSymbol, ">");
        return Ret_AbortOnError;
    }

    return Ret_Continue;

ErrorExit:
    delete pType->pNestedType;
    pType->pNestedType = NULL;

    return Ret_AbortOnError;
}

// BASE_TYPE   -> C_TYPE | CAR_TYPE
//
int P_BaseType(CARToken token, TypeDescriptor *pType)
{

    switch (token) {
        case Token_K_ECode:
            pType->type = Type_ECode;
            break;
        case Token_K_Byte:
            pType->type = Type_Byte;
            break;
        case Token_K_Boolean:
            pType->type = Type_Boolean;
            break;
        case Token_K_Int8:
            pType->type = Type_Int8;
            break;
        case Token_K_Int16:
            pType->type = Type_Int16;
            break;
        case Token_K_UInt16:
            pType->type = Type_UInt16;
            break;
        case Token_K_Int32:
            pType->type = Type_Int32;
            break;
        case Token_K_UInt32:
            pType->type = Type_UInt32;
            break;
        case Token_K_Int64:
            pType->type = Type_Int64;
            break;
        case Token_K_UInt64:
            pType->type = Type_UInt64;
            break;
        case Token_K_PVoid:
            pType->type = Type_PVoid;
            break;
        case Token_K_Char8:
            pType->type = Type_Char8;
            break;
        case Token_K_Char16:
            pType->type = Type_Char16;
            break;
        case Token_K_Float:
            pType->type = Type_Float;
            break;
        case Token_K_Double:
            pType->type = Type_Double;
            break;
        case Token_K_EMuid:
            pType->type = Type_EMuid;
            break;
        case Token_K_EGuid:
            pType->type = Type_EGuid;
            break;
        case Token_K_DateTime:
            pType->type = Type_DateTime;
            break;
        case Token_K_EventHandler:
            pType->type = Type_EventHandler;
            break;
        case Token_K_CString:
            pType->type = Type_CString;
            break;
        case Token_K_String:
            pType->type = Type_String;
            break;
        case Token_K_ArrayOf:
            pType->type = Type_ArrayOf;
            return P_CarQuient(pType);
        case Token_K_BufferOf:
            pType->type = Type_BufferOf;
            return P_CarQuient(pType);
        case Token_K_MemoryBuf:
            pType->type = Type_MemoryBuf;
            return P_CarQuient(pType);
        case Token_K_StringBuf:
            pType->type = Type_StringBuf;
            return P_CarQuient(pType);
        default:
            ErrorReport(CAR_E_UndefType, g_szCurrentToken);
            // Don't break here
        case Token_Error:
            return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// DTYPE       -> BASE_TYPE | ident
//
int P_DType(TypeDescriptor *pType)
{
    int n;
    CARToken token;

    token = GetToken(s_pFile);
    switch (token) {
        case Token_ident:
            n = RetrieveIdentifyType(g_szCurrentToken, s_pModule, pType, FALSE);
            if (n < 0) {
                ErrorReport(CAR_E_UndefinedSymbol, g_szCurrentToken);
                // continue on error
            }
            break;

        default:
            return P_BaseType(token, pType);
    }

    return Ret_Continue;
}

// ENUM_ELEM   -> ident [ s_assign < integer | hinteger | ident > ]
// The ident must has "EnumName_" as its prefix.
int P_EnumElement(const char *pszEnumName, EnumDescriptor *pDesc, int *pValue)
{
    int r;
    EnumElement *pElement;
    int nEnumNameLen;

    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    nEnumNameLen = strlen(pszEnumName);
    assert(nEnumNameLen);

    if ((g_nCurrentTokenLen <= nEnumNameLen + 1 /* EnumName_ */
        || strncmp(g_szCurrentToken, pszEnumName, nEnumNameLen)
        || g_szCurrentToken[nEnumNameLen] != '_') && !s_bLenientNaming) {
        ErrorReport(CAR_W_IllegalMemberName, g_szCurrentToken);
    }

    r = CreateEnumElement(g_szCurrentToken, s_pModule, pDesc);
    if (r < 0) {
        CreateError(r, "enum member", g_szCurrentToken);
        return Ret_ContinueOnError; // try to continue
    }

    if (PeekToken(s_pFile) == Token_S_assign) {
        GetToken(s_pFile); // skip "="

        switch (GetToken(s_pFile)) {
            case Token_integer:
                pDesc->ppElems[r]->nValue = atoi(g_szCurrentToken);
                break;

            case Token_hinteger:
                pDesc->ppElems[r]->nValue = Hexstr2Int(g_szCurrentToken);
                pDesc->ppElems[r]->bHexFormat = TRUE;
                break;

            case Token_ident:
                pElement = GlobalSelectEnumElement(g_szCurrentToken, s_pModule);
                if (!pElement) {
                    ErrorReport(CAR_E_NotFound, "const", g_szCurrentToken);
                    break; // continue on error
                }
                pDesc->ppElems[r]->nValue = pElement->nValue;
                pDesc->ppElems[r]->bHexFormat = pElement->bHexFormat;
                break;

            default:
                ErrorReport(CAR_E_IllegalValue);
            case Token_Error:
                return Ret_AbortOnError;
        }

        *pValue = pDesc->ppElems[r]->nValue;
    }
    else {
        pDesc->ppElems[r]->nValue = *pValue;
    }

    return Ret_Continue;
}

// ENUM_BODY   -> s_lbrace ENUM_ELEM { s_comma ENUM_ELEM }
//                [ s_comma ] s_rbrace ]
//
int P_EnumBody(const char *pszEnumName, EnumDescriptor *pDesc)
{
    CARToken token;
    int nValue = 0;

    do {
        if (P_EnumElement(pszEnumName, pDesc, &nValue) == Ret_AbortOnError) {
            return Ret_AbortOnError;
        }
        nValue++;

        token = GetToken(s_pFile);
        if (Token_S_comma == token) {
            if (PeekToken(s_pFile) == Token_S_rbrace)
                token = GetToken(s_pFile);
        }
    } while (Token_S_comma == token);

    if (Token_S_rbrace != token) {
        ErrorReport(CAR_E_ExpectSymbol, "}");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// ENUM        -> k_enum ident [ ENUM_BODY | s_semicolon ]
//
int P_Enum()
{
    CARToken token;
    int r;

    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_ExpectEnumName);
        return Ret_AbortOnError;
    }
    r = CreateEnumDirEntry(g_szCurrentToken, s_pModule);

    token = PeekToken(s_pFile);

    if (Token_S_lbrace == token) {
        if (r < 0) {
            CreateError(r, "enum", g_szCurrentToken);
            return Ret_AbortOnError;
        }
        GetToken(s_pFile); // ignore "{"

        if (P_EnumBody(s_pModule->ppEnumDir[r]->pszName,
            s_pModule->ppEnumDir[r]->pDesc) == Ret_AbortOnError) {
            return Ret_AbortOnError;
        }
    }
    else if (Token_S_semicolon == token) {
        if (r < 0 && r != CLSError_DupEntry) {
            CreateError(r, "enum", g_szCurrentToken);
            // continue on error
        }
        GetToken(s_pFile); // ignore ";"
    }
    else {
        ErrorReport(CAR_E_ExpectSymbol, "{ or ;");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// CONST       -> k_const ident s_assign < integer | hinteger > s_semicolon
//
int P_Const()
{
    CARToken token;
    int r;

    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_ExpectConstName);
        return Ret_AbortOnError;
    }

    r = CreateConstDirEntry(g_szCurrentToken, s_pModule);
    if (r < 0) {
        if (r == CLSError_DupEntry) {
            ErrorReport(CAR_E_DupEntry, "const", g_szCurrentToken);
            return Ret_AbortOnError;
        }
        else {
            CreateError(r, "const", g_szCurrentToken);
            // continue on error
        }
    }
    token = GetToken(s_pFile);
    if (Token_S_assign != token) {
        ErrorReport(CAR_E_ExpectSymbol, "=");
        return Ret_AbortOnError;
    }
    switch (GetToken(s_pFile)) {
        case Token_integer:
            s_pModule->ppConstDir[r]->type = INTEGER;
            s_pModule->ppConstDir[r]->v.intValue.nValue = atoi(g_szCurrentToken);
            s_pModule->ppConstDir[r]->v.intValue.bHexFormat = FALSE;
            break;

        case Token_hinteger:
            s_pModule->ppConstDir[r]->type = INTEGER;
            s_pModule->ppConstDir[r]->v.intValue.nValue = Hexstr2Int(g_szCurrentToken);
            s_pModule->ppConstDir[r]->v.intValue.bHexFormat = TRUE;
            break;

        case Token_string:
            {
                s_pModule->ppConstDir[r]->type = STRING;
                char *str = (char*)malloc(strlen(g_szCurrentToken) + 1);
                strcpy(str, g_szCurrentToken);
                s_pModule->ppConstDir[r]->v.strValue.pszValue = str;
                break;
            }
        default:
            ErrorReport(CAR_E_IllegalValue);
        case Token_Error:
            return Ret_AbortOnError;
    }

    token = PeekToken(s_pFile);
    if (Token_S_semicolon == token) {
        GetToken(s_pFile); // ignore ";"
    }
    else {
        ErrorReport(CAR_E_ExpectSymbol, ";");
        return Ret_AbortOnError;
    }
    return Ret_Continue;
}

// ARRAY -> {s_lbracket < Token_hinteger | ident > s_rbracket}+

int P_Array(TypeDescriptor &type)
{
    unsigned short nElements;
    CARToken token, nextToken;
    int r;

    nextToken = PeekToken(s_pFile);
    assert(Token_S_lbracket == nextToken);

    // ignore '['
    token = GetToken(s_pFile);

    token = GetToken(s_pFile);
    if (token != Token_integer
        && token != Token_hinteger) {
        if (token != Token_ident) {
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
        }
        else {
            r = SelectConstDirEntry(g_szCurrentToken, s_pModule);
            if (CLSError_NotFound == r) {
                ErrorReport(CAR_E_NotFound, "const", g_szCurrentToken);
                return Ret_AbortOnError;
            }
            if (s_pModule->ppConstDir[r]->type != INTEGER) {
                ErrorReport(CAR_E_NotIntegerConst, g_szCurrentToken);
                return Ret_AbortOnError;
            }
            nElements = s_pModule->ppConstDir[r]->v.intValue.nValue;
        }
    }
    else {
        if (Token_integer == token) {
            nElements = (unsigned short)atoi(g_szCurrentToken);
        }
        else {
            assert(Token_hinteger == token);
            nElements = (unsigned short)Hexstr2Int(g_szCurrentToken);
        }
    }
    nextToken = PeekToken(s_pFile);
    if (nextToken != Token_S_rbracket) {
        ErrorReport(CAR_E_UnexpectSymbol, nextToken);
        return Ret_AbortOnError;
    }

    if (nElements <= 0) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    // ignore ']'
    token = GetToken(s_pFile);

    nextToken = PeekToken(s_pFile);
    if (Token_S_lbracket == nextToken) { //Handle the case of [m][n]...
        r = P_Array(type);
        if (r < 0)
            return r;
    }

    r = SelectArrayDirEntry(nElements, type, s_pModule);
    if (r < 0) {
        r = CreateArrayDirEntry(s_pModule);
        if (r < 0) {
            ErrorReport(CAR_E_OutOfMemory);
            return Ret_AbortOnError;
        }
        memcpy(&s_pModule->ppArrayDir[r]->type, &type, sizeof(TypeDescriptor));
        s_pModule->ppArrayDir[r]->nElements = nElements;
    }

    memset(&type, 0, sizeof(TypeDescriptor));

    type.type = Type_Array;
    type.sIndex = r;

    return r;
}

// STRUCT_ELEM -> DTYPE ELEM_NAME { s_comma ELEM_NAME } s_semicolon
// ELEM_NAME   -> [ s_star [ s_star] ] ident [ARRAY]
//
int P_StructElement(StructDescriptor *pDesc)
{
    int r;
    TypeDescriptor type, tempType;
    CARToken token, nextToken;
    char szElementName[c_nMaxTokenSize + 1];

    memset(&type, 0, sizeof(TypeDescriptor));
    if (P_DType(&type) == Ret_AbortOnError) return Ret_AbortOnError;

    do {
        memcpy(&tempType, &type, sizeof(type));
        token = GetToken(s_pFile);

        tempType.nPointer = 0;
        if (Token_S_star == token) {
            tempType.nPointer++;
            token = GetToken(s_pFile);
            if (Token_S_star == token) {
                tempType.nPointer++;
                token = GetToken(s_pFile);
            }
        }

        if (Token_ident != token) {
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
        }

        strcpy(szElementName, g_szCurrentToken);

        if (Type_struct == tempType.type
            && s_pModule->ppStructDir[tempType.sIndex]->pDesc == pDesc
            && 0 == tempType.nPointer) {
            ErrorReport(CAR_E_NestedStruct, g_szCurrentToken);
            // continue on error
        }
        // In case of Array
        nextToken = PeekToken(s_pFile);
        if (Token_S_lbracket == nextToken) {
            r = P_Array(tempType);
            if (r < 0) {
                return Ret_AbortOnError;
            }
        }

        r = CreateStructElement(szElementName, pDesc);
        if (r < 0) {
            CreateError(r, "struct member", szElementName);
            // continue on error
        }
        else {
            memcpy(&pDesc->ppElems[r]->type, &tempType, sizeof(tempType));
        }

        token = GetToken(s_pFile);
    } while (Token_S_comma == token);

    if (Token_S_semicolon != token) {
        ErrorReport(CAR_E_ExpectSymbol, ";");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// STRUCT_BODY -> s_lbrace { STRUCT_ELEM { s_semicolon } }+ s_rbrace
//
int P_StructBody(StructDescriptor *pDesc)
{
    do {
        if (P_StructElement(pDesc) == Ret_AbortOnError)
            return Ret_AbortOnError;
        while (PeekToken(s_pFile) == Token_S_semicolon) GetToken(s_pFile);
    } while (PeekToken(s_pFile) != Token_S_rbrace);

    GetToken(s_pFile); // skip "}"
    return Ret_Continue;
}

// STRUCT      -> k_struct ident [ STRUCT_BODY | s_semicolon ]
//
int P_Struct()
{
    CARToken token;
    int r;

    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_ExpectStructName);
        return Ret_AbortOnError;
    }
    r = CreateStructDirEntry(g_szCurrentToken, s_pModule);

    token = PeekToken(s_pFile);

    if (Token_S_lbrace == token) {
        if (r < 0) {
            CreateError(r, "struct", g_szCurrentToken);
            return Ret_AbortOnError;
        }
        GetToken(s_pFile); // ignore "{"

        if (P_StructBody(
            s_pModule->ppStructDir[r]->pDesc) == Ret_AbortOnError) {
            return Ret_AbortOnError;
        }
    }
    else if (Token_S_semicolon == token) {
        if (r < 0 && r != CLSError_DupEntry) {
            CreateError(r, "struct", g_szCurrentToken);
            // continue on error
        }
        GetToken(s_pFile); // ignore ";"
    }
    else {
        ErrorReport(CAR_E_ExpectSymbol, "{ or ;");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// TYPEDEF     -> k_typedef [ k_dummytype ]
//                DTYPE ALIAS { s_comma ALIAS } s_semicolon
// ALIAS       -> [ s_star [ s_star ] ] ident
//
int P_Typedef()
{
    int r;
    CARToken token;
    BOOL bDummyType = FALSE;
    TypeDescriptor type;

    if (PeekToken(s_pFile) == Token_S_lbracket) {
        GetToken(s_pFile); // skip "["
        if (GetToken(s_pFile) != Token_K_dummytype) {
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
        }
        if (GetToken(s_pFile) != Token_S_rbracket) {
            ErrorReport(CAR_E_ExpectSymbol, "]");
            return Ret_AbortOnError;
        }
        bDummyType = TRUE;
    }

    memset(&type, 0, sizeof(type));
    if (P_DType(&type) == Ret_AbortOnError) return Ret_AbortOnError;

    do {
        token = GetToken(s_pFile);

        type.nPointer = 0;
        if (Token_S_star == token) {
            type.nPointer++;
            token = GetToken(s_pFile);
            if (Token_S_star == token) {
                type.nPointer++;
                token = GetToken(s_pFile);
            }
        }

        if (Token_ident != token) {
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
        }

        r = CreateAliasDirEntry(g_szCurrentToken, s_pModule, &type);
        if (r < 0) {
            CreateError(r, "alias", g_szCurrentToken);
            // continue on error
        }
        else {
            s_pModule->ppAliasDir[r]->bDummyType = bDummyType;
        }

        token = GetToken(s_pFile);
    } while (Token_S_comma == token);

    if (Token_S_semicolon != token) {
        ErrorReport(CAR_E_ExpectSymbol, ";");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// PRAGMA      -> k_pragma s_lparen PRAGMA_EXPS s_rparen
// PRAGMA_EXPS -> < k_disalbe | k_enable > s_colon < integer | hinteger >
//
int P_Pragma()
{
    int nWarningNo;
    CARToken token;

    if (GetToken(s_pFile) != Token_S_lparen) {
        ErrorReport(CAR_E_ExpectSymbol, "(");
        return Ret_AbortOnError;
    }

    token = GetToken(s_pFile);
    if (Token_K_disable != token && Token_K_enable != token) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }
    if (GetToken(s_pFile) != Token_S_colon) {
        ErrorReport(CAR_E_ExpectSymbol, ":");
        return Ret_AbortOnError;
    }

    switch (GetToken(s_pFile)) {
        case Token_integer:
            nWarningNo = atoi(g_szCurrentToken);
            break;
        case Token_hinteger:
            nWarningNo = Hexstr2Int(g_szCurrentToken);
            break;
        default:
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
    }

    if (Token_K_enable == token)
        EnableWarning(nWarningNo);
    else
        DisableWarning(nWarningNo);

    if (GetToken(s_pFile) != Token_S_rparen) {
        ErrorReport(CAR_E_ExpectSymbol, ")");
        return Ret_AbortOnError;
    }
    return Ret_Continue;
}


void AddConstClassInterface(
    const char *pszName,
    CLSModule *pModule,
    ClassDescriptor *pDesc)
{
    int n;

    n = RetrieveInterface(pszName, pModule, FALSE);
    if (n < 0) {
        //ErrorReport(CAR_E_NotFound, "interface", pszName);
        return;
    }

    n = CreateClassInterface(n, pDesc);
    if (n < 0) {
        //if (n != CLSError_DupEntry)
        //CreateError(n, "class interface", pszName);
    }
    else {
        pDesc->ppInterfaces[n]->wAttribs |= ClassInterfaceAttrib_autoadded;
        pDesc->dwAttribs |= ClassAttrib_defined;
    }
}

int AddConstClassInterface(const char *pszName, ClassDescriptor *pDesc)
{
    int n;

    n = RetrieveInterface(pszName, s_pModule, FALSE);
    if (n < 0) {
        ErrorReport(CAR_E_NotFound, "interface", pszName);
        return n;
    }

    n = CreateClassInterface(n, pDesc);
    if (n < 0) {
        if (n != CLSError_DupEntry)
            CreateError(n, "class interface", pszName);
    }
    else {
        pDesc->ppInterfaces[n]->wAttribs |= ClassInterfaceAttrib_autoadded;
        pDesc->dwAttribs |= ClassAttrib_defined;
    }
    return n;
}


//
// Generate an IID seed string from interface info.
// The seed string is compose of alphabet, number or '_', we get them
// from name string, the selective order is:
//
//      (interface name) -> (method names) ->
//      (parent name) -> (parent method names) ->
//      (parent's parent name) -> ... and so on,
//
void GenIIDSeedString(
    CLSModule *pModule,
    InterfaceDirEntry *pInterface,
    char *pszBuf)
{
    int n, nSize, nTotalSize = 0;
    InterfaceDescriptor *pDesc = pInterface->pDesc;

    while (1) {
        nSize = strlen(pInterface->pszName);
        if (nSize + nTotalSize + 1 > 256) break;
        memcpy(pszBuf, pInterface->pszName, nSize);
        pszBuf += nSize;
        *pszBuf++ = '/';
        nTotalSize += nSize + 1;

        for (n = 0; n < pDesc->cMethods; n++) {
            nSize = strlen(pDesc->ppMethods[n]->pszName);
            if (nSize + nTotalSize > 256) goto ExitEntry;
            memcpy(pszBuf, pDesc->ppMethods[n]->pszName, nSize);
            pszBuf += nSize;
            nTotalSize += nSize;
        }
        if (0 == pDesc->sParentIndex) break;

        pInterface = pModule->ppInterfaceDir[pDesc->sParentIndex];
        pDesc = pInterface->pDesc;
    }

ExitEntry:
    *pszBuf = 0;
}

void GenerateSinkClass(
    const char *pszName,
    CLSModule *pModule,
    ClassDirEntry *pClass)
{
    int n;
    ClassDescriptor *pSinkDesc;
    char szSinkName[c_nMaxTokenSize + 1];
    char szInterfaceName[c_nMaxTokenSize + 1];
    ClassDescriptor *pDesc = pClass->pDesc;
    CLSID *pClsid, *pSinkClsid;

    strcpy(szSinkName, pszName);
    strcat(szSinkName, "Sink");

    n = CreateClassDirEntry(szSinkName, pModule, ClassAttrib_t_sink);
    if (n < 0) {
        return;
    }
    if (NULL != pClass->pszNameSpace) {
        pModule->ppClassDir[n]->pszNameSpace =
                        (char *)malloc(strlen(pClass->pszNameSpace) + 1);
        if (!pModule->ppClassDir[n]->pszNameSpace) return;
        strcpy(pModule->ppClassDir[n]->pszNameSpace, pClass->pszNameSpace);
    }
    pSinkDesc = pModule->ppClassDir[n]->pDesc;

    pClsid = &pDesc->clsid;
    pSinkClsid = &pSinkDesc->clsid;
    pSinkClsid->Data1 = pClsid->Data1 ^ CLSID_XOR_CallbackSink.Data1;
    pSinkClsid->Data2 = pClsid->Data2 ^ CLSID_XOR_CallbackSink.Data2;
    pSinkClsid->Data3 = pClsid->Data3 ^ CLSID_XOR_CallbackSink.Data3;
    pSinkClsid->Data4[0] = pClsid->Data4[0] ^ CLSID_XOR_CallbackSink.Data4[0];
    pSinkClsid->Data4[1] = pClsid->Data4[1] ^ CLSID_XOR_CallbackSink.Data4[1];
    pSinkClsid->Data4[2] = pClsid->Data4[2] ^ CLSID_XOR_CallbackSink.Data4[2];
    pSinkClsid->Data4[3] = pClsid->Data4[3] ^ CLSID_XOR_CallbackSink.Data4[3];
    pSinkClsid->Data4[4] = pClsid->Data4[4] ^ CLSID_XOR_CallbackSink.Data4[4];
    pSinkClsid->Data4[5] = pClsid->Data4[5] ^ CLSID_XOR_CallbackSink.Data4[5];
    pSinkClsid->Data4[6] = pClsid->Data4[6] ^ CLSID_XOR_CallbackSink.Data4[6];
    pSinkClsid->Data4[7] = pClsid->Data4[7] ^ CLSID_XOR_CallbackSink.Data4[7];

    AddConstClassInterface("IObject", pModule, pSinkDesc);
    AddConstClassInterface("ICallbackSink", pModule, pSinkDesc);
    for (n = 0; n < pDesc->cInterfaces; n++) {
        if (pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback){
            strcpy(szInterfaceName, pModule->ppInterfaceDir[pDesc->ppInterfaces[n]->sIndex]->pszName);
            strcat(szInterfaceName, "Callback");
            int x = SelectInterfaceDirEntry(szInterfaceName, pModule);
            if (x < 0) continue;
            int m = CreateClassInterface(x, pSinkDesc);
            if (m > 0 && (pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_async)) {
                pSinkDesc->ppInterfaces[m]->wAttribs |= ClassInterfaceAttrib_async;
            }
            if (m > 0 && (pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_delegate)) {
                pSinkDesc->ppInterfaces[m]->wAttribs |= ClassInterfaceAttrib_delegate;
            }
            if (!(pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_async)
                && !(pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_delegate)) {
                szInterfaceName[strlen(szInterfaceName)-8] = '\0';
                strcat(szInterfaceName, "Handler");
                x = AddConstClassInterface(szInterfaceName, pSinkDesc);
                if (x > 0) pSinkDesc->ppInterfaces[x]->wAttribs |= ClassInterfaceAttrib_handler;
            }
        }
    }
    pSinkDesc->dwAttribs |= ClassAttrib_t_sink | ClassAttrib_naked;
    pModule->dwAttribs |= CARAttrib_hasSinkObject;
}

int GenerateSinkInterface(
    const char *pszName,
    CLSModule *pModule,
    InterfaceDirEntry *pInterface)
{
    int i=0, n=0, x=0, m=0;
    InterfaceDescriptor *pSinkDesc = NULL;
    char szSinkName[c_nMaxTokenSize + 1] = {0};
    InterfaceDescriptor *pSrcDesc = pInterface->pDesc;
    ParamDescriptor *pParam = NULL;

    strcpy(szSinkName, pszName);
    strcat(szSinkName, "Callback");

    x = SelectInterfaceDirEntry(szSinkName, pModule);
    if (x >= 0)
        return x;
    x = n = CreateInterfaceDirEntry(szSinkName, pModule, InterfaceAttrib_sink);
    if (n < 0) {
        //CreateError(n, "interfcace", szSinkName);
        return 0;
    }
    pSinkDesc = pModule->ppInterfaceDir[n]->pDesc;
    pModule->pDefinedInterfaceIndex[pModule->cDefinedInterfaces++] = n;
    InterfaceDescriptorCopy(pModule, pSrcDesc, pModule, pSinkDesc, TRUE);

    pSinkDesc->dwAttribs |= InterfaceAttrib_sink;
    pSinkDesc->dwAttribs &= ~InterfaceAttrib_t_callbacks;
    pSinkDesc->dwAttribs |= InterfaceAttrib_t_normal;
    for (n = 0; n < pSrcDesc->cMethods; n++) {
        m = CreateMethodParam("cFlags", pSinkDesc->ppMethods[n]);
        if (m < 0) return 0;
        pParam = pSinkDesc->ppMethods[n]->ppParams[m];

        for (i = m; i > 0;--i) {
            pSinkDesc->ppMethods[n]->ppParams[i] = pSinkDesc->ppMethods[n]->ppParams[i-1];
        }
        pSinkDesc->ppMethods[n]->ppParams[0] = pParam;
        pParam->dwAttribs = ParamAttrib_in;
        memset(&pParam->type, 0, sizeof(pParam->type));
        pParam->type.type = Type_Int32;
    }

    return x;
}


int GenerateAsyncCallbackInterface(
    const char *pszName,
    CLSModule *pModule,
    InterfaceDirEntry *pInterface)
{
    int n=0, x=0;
    InterfaceDescriptor *pSinkDesc = NULL;
    char szSinkName[c_nMaxTokenSize + 1] = {0};
    InterfaceDescriptor *pSrcDesc = pInterface->pDesc;
    char szMethodName[c_nMaxTokenSize + 1];

    strcpy(szSinkName, pszName);
    strcat(szSinkName, "Async");

    x = SelectInterfaceDirEntry(szSinkName, pModule);
    if (x >= 0)
        return x;
    x = n = CreateInterfaceDirEntry(szSinkName, pModule, InterfaceAttrib_defined);
    if (n < 0) {
        //CreateError(n, "interfcace", szSinkName);
        return 0;
    }
    pSinkDesc = pModule->ppInterfaceDir[n]->pDesc;
    pModule->pDefinedInterfaceIndex[pModule->cDefinedInterfaces++] = n;
    InterfaceDescriptorCopy(pModule, pSrcDesc, pModule, pSinkDesc, TRUE);

    for (n = 0; n < pSrcDesc->cMethods; n++) {
        strcpy(szMethodName, "begin");
        strcat(szMethodName, pSinkDesc->ppMethods[n]->pszName);
        delete [] pSinkDesc->ppMethods[n]->pszName;
        pSinkDesc->ppMethods[n]->pszName = new char[strlen(szMethodName)+1];
        strcpy(pSinkDesc->ppMethods[n]->pszName, szMethodName);
    }

    return x;
}

int GenerateHandlerInterface(
    const char *pszName,
    CLSModule *pModule,
    InterfaceDirEntry *pInterface)
{
    int i=0, n=0, x=0, m=0, r=0;
    InterfaceDescriptor *pSinkDesc = NULL;
    char szSinkName[c_nMaxTokenSize + 1] = {0};
    InterfaceDescriptor *pSrcDesc = pInterface->pDesc;
    ParamDescriptor *pParam = NULL;
    char szMethodName[c_nMaxTokenSize + 1];

    strcpy(szSinkName, pszName);
    strcat(szSinkName, "Handler");

    x = RetrieveInterface(szSinkName, pModule, TRUE);
    if (x >= 0) return x;
    x = n = CreateInterfaceDirEntry(szSinkName, pModule, InterfaceAttrib_defined);
    if (n < 0) return 0;
    r = RetrieveInterface("ICallbackRendezvous", pModule, FALSE);
    if (r < 0) return 0;

    pSinkDesc = pModule->ppInterfaceDir[n]->pDesc;
    pModule->pDefinedInterfaceIndex[pModule->cDefinedInterfaces++] = n;

    pSinkDesc->dwAttribs |= InterfaceAttrib_defined;
    pSinkDesc->dwAttribs |= InterfaceAttrib_sink;
    pSinkDesc->dwAttribs |= InterfaceAttrib_t_normal;
    for (i = 0; i < pSrcDesc->cMethods; i++) {
        //////// AddXXXCallback(pThis, pFunc, pUserData) ////////
        strcpy(szMethodName, "Add");
        strcat(szMethodName, pSrcDesc->ppMethods[i]->pszName);
        strcat(szMethodName, "Callback");

        n = CreateInterfaceMethod(szMethodName, pSinkDesc);
        if (m < 0) return 0;

        pSinkDesc->ppMethods[n]->type.type = Type_ECode;

        m = CreateMethodParam("handler", pSinkDesc->ppMethods[n]);
        if (m < 0) return 0;
        pParam = pSinkDesc->ppMethods[n]->ppParams[m];

        pSinkDesc->ppMethods[n]->ppParams[0] = pParam;
        pParam->dwAttribs = ParamAttrib_in;
        memset(&pParam->type, 0, sizeof(pParam->type));
        pParam->type.type = Type_EventHandler;
        pParam->type.nPointer = 0;

        //////// RemoveXXXCallback(pThis, pFunc, pUserData) ////////
        strcpy(szMethodName, "Remove");
        strcat(szMethodName, pSrcDesc->ppMethods[i]->pszName);
        strcat(szMethodName, "Callback");

        n = CreateInterfaceMethod(szMethodName, pSinkDesc);
        if (m < 0) return 0;
        pSinkDesc->ppMethods[n]->type.type = Type_ECode;

        m = CreateMethodParam("handler", pSinkDesc->ppMethods[n]);
        if (m < 0) return 0;
        pParam = pSinkDesc->ppMethods[n]->ppParams[m];

        pSinkDesc->ppMethods[n]->ppParams[0] = pParam;
        pParam->dwAttribs = ParamAttrib_in;
        memset(&pParam->type, 0, sizeof(pParam->type));
        pParam->type.type = Type_EventHandler;
        pParam->type.nPointer = 0;

        //////// AcquireXXXRendezvous(pThis, pFunc, pUserData) ////////
        strcpy(szMethodName, "Acquire");
        strcat(szMethodName, pSrcDesc->ppMethods[i]->pszName);
        strcat(szMethodName, "Rendezvous");

        n = CreateInterfaceMethod(szMethodName, pSinkDesc);
        if (m < 0) return 0;
        pSinkDesc->ppMethods[n]->type.type = Type_ECode;

        m = CreateMethodParam("ppRendezvous", pSinkDesc->ppMethods[n]);
        if (m < 0) return 0;
        pParam = pSinkDesc->ppMethods[n]->ppParams[m];
        pSinkDesc->ppMethods[n]->ppParams[0] = pParam;
        pParam->dwAttribs = ParamAttrib_out;
        memset(&pParam->type, 0, sizeof(pParam->type));
        pParam->type.type = Type_interface;
        pParam->type.nPointer = 2;
        pParam->type.sIndex = r;

    }

    return x;
}

static int s_nCallbackEnumValue = 0;
int CreateEnumElems(
    int methods,
    InterfaceDescriptor*pIntf,
    EnumDescriptor* pEnumDesc,
    char* pElemName,
    CLSModule *pModule)
{
    int i, m, n;
    bool flag = false;
    char tmp[c_nMaxTokenSize + 1];

    //if callback interface have parent
    if (pIntf->sParentIndex > 0) {
        CreateEnumElems(pModule->ppInterfaceDir[pIntf->sParentIndex]->pDesc->cMethods,
            pModule->ppInterfaceDir[pIntf->sParentIndex]->pDesc, pEnumDesc, pElemName, pModule);
    }

    // the name of the enum element
    for (i = 0; i < methods; i++) {
        strcpy(tmp, pElemName);
        strcat(tmp, pIntf->ppMethods[i]->pszName);
        for (n = 0; n < pEnumDesc->cElems; n++) {
            if (!strcmp(tmp, pEnumDesc->ppElems[n]->pszName)) {
                flag = true;
                break;
            }
        }
        if (flag == true)
            break;
        m = CreateEnumElement(tmp, pModule, pEnumDesc);
        if (m < 0) {
            //CreateError(m, "enum member", pElemName);
            return -1; // try to continue
        }
        pEnumDesc->ppElems[m]->nValue = s_nCallbackEnumValue++;
    }

    return 0;
}

int CreateEnum(
    ClassDirEntry *pClass,
    InterfaceDescriptor *pIntf,
    char* pIntfName,
    CLSModule *pModule)
{
    char enumToken[c_nMaxTokenSize + 1];
    EnumDescriptor *pEnumDesc;
    int methods;
    char elemName[c_nMaxTokenSize + 1];
   // char szInterfaceName[c_nMaxTokenSize + 1];
    int m;

    //name of the enum entry
    strcpy(enumToken, "_");
    strcat(enumToken, pClass->pszName);
    strcat(enumToken, "Event");

    m = SelectEnumDirEntry(enumToken, pModule);
    if (m < 0) {
        m = CreateEnumDirEntry(enumToken, pModule);
        if (m < 0) {
            //   CreateError(m, "enum Dir", enumToken);
            return -1;
        }
    }

    pEnumDesc = pModule->ppEnumDir[m]->pDesc;

    //begin of the callback interface enum
    strcpy(enumToken, pClass->pszName);
    strcat(enumToken, "Sink_");
    strcat(enumToken, pIntfName);  //interface name
    strcat(enumToken, "Begin");

    m = CreateEnumElement(enumToken, pModule, pEnumDesc);
    if (m < 0) {
        //  CreateError(m, "enum member", enumToken);
        return 0; // try to continue
    }
    pEnumDesc->ppElems[m]->nValue = s_nCallbackEnumValue++;

    methods = pIntf->cMethods;

    strcpy(elemName, pClass->pszName);
    strcat(elemName, "_");

    CreateEnumElems(methods, pIntf, pEnumDesc, elemName, pModule);

    //end of the callback interface enum
    strcpy(enumToken, pClass->pszName);
    strcat(enumToken, "Sink_");
    strcat(enumToken, pIntfName);  //interface name
    strcat(enumToken, "End");

    m = CreateEnumElement(enumToken, pModule, pEnumDesc);
    if (m < 0) {
        //  CreateError(m, "enum member", enumToken);
        return 0; // try to continue
    }
    pEnumDesc->ppElems[m]->nValue = s_nCallbackEnumValue++;

    return 0;
}

int GenerateEnums(CLSModule *pModule)
{
    int j, m, n, k=0;
    ClassDirEntry *pClass=NULL;
    ClassDescriptor *pDesc=NULL;
    InterfaceDirEntry *pIntf=NULL;
    InterfaceDescriptor *pIntfDesc=NULL;
    char szInterfaceName[c_nMaxTokenSize + 1];
    EnumDescriptor *pEnumDesc;
    char enumToken[c_nMaxTokenSize + 1];

    for (k = 0; k < pModule->cClasses; k++) {
        pClass = pModule->ppClassDir[k];
        pDesc = pClass->pDesc;
        if (pDesc->dwAttribs & ClassAttrib_hascallback) {
            for (n = 0; n < pDesc->cInterfaces; n++) {
                if (pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback) {
                    pIntf = pModule->ppInterfaceDir[pDesc->ppInterfaces[n]->sIndex];
                    pIntfDesc = pIntf->pDesc;

                    /*
                     * check callback interface of original class
                     */
                    strcpy(szInterfaceName, pIntf->pszName);
                    j = RetrieveInterface(szInterfaceName, pModule, FALSE);
                    if (j < 0) {
                        return -1;
                    }

                    /*
                     * generate enum and interface for callback
                     */
                    CreateEnum(pClass, pIntfDesc, szInterfaceName, pModule);
                }
            }
            /*
             * generate Sink_MaxEvents enum
             */
            strcpy(enumToken, "_");
            strcat(enumToken, pClass->pszName);
            strcat(enumToken, "Event");

            m = SelectEnumDirEntry(enumToken, pModule);
            if (m < 0) {
                return -2;
            }

            pEnumDesc = pModule->ppEnumDir[m]->pDesc;

            strcpy(enumToken, pClass->pszName);
            strcat(enumToken, "Sink_MaxEvents");

            m = CreateEnumElement(enumToken, pModule, pEnumDesc);
            if (m < 0) {
                return -3;
            }

            pEnumDesc->ppElems[m]->nValue = s_nCallbackEnumValue++;
            s_nCallbackEnumValue = 0;
        }
    }

    return 0;
}

int ExtendCLS(CLSModule *pModule, BOOL bNoElastos)
{
    int j, n, k, x=0;
    ClassDirEntry *pClass=NULL;
    ClassDescriptor *pDesc=NULL;
    InterfaceDirEntry *pIntf=NULL;
    InterfaceDescriptor *pIntfDesc=NULL, *pIntfDescSink=NULL;
    char szInterfaceName[c_nMaxTokenSize + 1];
    char szSeedBuf[c_nMaxTokenSize + 1];
    CLSModule *pModuleTmp;

    for (k = 0; k < pModule->cClasses; k++) {
        pClass = pModule->ppClassDir[k];
        pDesc = pClass->pDesc;
        if ((pDesc->dwAttribs & ClassAttrib_hascallback) &&
                !(pDesc->dwAttribs & ClassAttrib_t_generic) &&
                (pClass->pszNameSpace == NULL)) {
            if (!bNoElastos) {
                if ((CLS_NoError == LoadCLSFromDll("Elastos.Runtime.CarRuntime.eco", &pModuleTmp))
                    || (LoadCLS("ElastosCore.cls", &pModule) == CLS_NoError)) {
                    int nIndex = SelectInterfaceDirEntry("ICallbackSink", pModuleTmp);
                    if (nIndex >= 0) InterfaceCopy(pModuleTmp, nIndex, pModule, FALSE);
                    DisposeFlattedCLS(pModuleTmp);
                }
            }

            for (n = 0; n < pDesc->cInterfaces; n++) {
                if (pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback) {
                    pIntf = pModule->ppInterfaceDir[pDesc->ppInterfaces[n]->sIndex];
                    pIntfDesc = pIntf->pDesc;

                    /*
                     * check callback interface of original class
                     */
                    strcpy(szInterfaceName, pIntf->pszName);
                    j = RetrieveInterface(szInterfaceName, pModule, FALSE);
                    if (j < 0) {
                        return -1;
                    }

                    /*
                     * generate enum and interface for callback
                     */
                    x = GenerateSinkInterface(szInterfaceName, pModule, pModule->ppInterfaceDir[j]);
                    pIntfDescSink = pModule->ppInterfaceDir[x]->pDesc;

                    GenIIDSeedString(pModule, pModule->ppInterfaceDir[x], szSeedBuf);
                    GuidFromSeedString(szSeedBuf, &pModule->ppInterfaceDir[x]->pDesc->iid);
                }
            }
            /*
             * generate sink class for callback
             */
            GenerateSinkClass(pClass->pszName, pModule, pClass);
        }
    }

    return 0;
}


int ImportCAR(const char *pszFile)
{
    FILE *pFile;
    CLSModule *pModule;
    void *pvCtxToken, *pvCtxError;
    CLSModule *pImportCLS;
    char *pszCtxCurrentPath;

    pImportCLS = CreateCLS();
    if (!pImportCLS) {
        ErrorReport(CAR_E_OutOfMemory);
        return Ret_AbortOnError;
    }

    // Save compiling context of current file.
    //
    pvCtxToken = SaveTokenContext();
    pvCtxError = SaveErrorContext();
    pszCtxCurrentPath = new char[strlen(s_pszCurrentPath) + 1];
    strcpy(pszCtxCurrentPath, s_pszCurrentPath);
    pFile = s_pFile;
    pModule = s_pModule;

    DisableErrorCounting();
    ErrorStringFix("import ", "  -> import error ignored.");

    // Compiling imported car file and ignore any errors.
    //
    if (GetCurrentCarPath(s_pszCarFullName, &s_pszCurrentPath) == Ret_AbortOnError) {
        delete[] pszCtxCurrentPath;
        return Ret_AbortOnError;
    }

    if (DoCompiling(pszFile, pImportCLS) == Ret_AbortOnError) {
        DestroyCLS(pImportCLS);
    }
    else {
        AddCLSLibrary(pImportCLS);
    }

    ErrorStringFix(NULL, NULL);
    EnableErrorCounting();

    // Restore compiling context of current file.
    //
    s_pModule = pModule;
    s_pFile = pFile;
    assert(s_pszCurrentPath);
    delete[] s_pszCurrentPath;
    s_pszCurrentPath = pszCtxCurrentPath;
    RestoreErrorContext(pvCtxError);
    RestoreTokenContext(pvCtxToken);

    return Ret_Continue;
}

int ImportProc(char *pszName)
{
    int n, nNameLen;

    nNameLen = strlen(pszName);

    // check suffix and replace .car with .cls
    //
    if (_stricmp(pszName + nNameLen - 4, ".car")) {
        ErrorReport(CAR_W_UnexpectFileType, pszName);
        return Ret_ContinueOnError;
    }
    strcpy(pszName + nNameLen - 4, ".cls");

    // check same entry
    //
    for (n = 0; n < s_pModule->cLibraries; n++) {
        if (!_stricmp(pszName, s_pModule->ppLibNames[n]))
            return Ret_Continue;
    }

    // add library name to module
    //
    if (c_nMaxLibNumber == n) {
        ErrorReport(CAR_W_TooManyLibraries);
        return Ret_Continue;
    }
    s_pModule->ppLibNames[n] = new char[nNameLen + 1];
    if (!s_pModule->ppLibNames[n]) {
        ErrorReport(CAR_E_OutOfMemory);
        return Ret_AbortOnError;
    }
    strcpy(s_pModule->ppLibNames[n], pszName);
    s_pModule->cLibraries++;

    strcpy(pszName + nNameLen - 4, ".car");
    return ImportCAR(pszName);
}

// IMPORT      -> k_import s_lparen string s_rparen s_semicolon
//
int P_Import()
{
    if (GetToken(s_pFile) != Token_S_lparen ||
        GetToken(s_pFile) != Token_string) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    if (ImportProc(g_szCurrentToken) == Ret_AbortOnError)
        return Ret_AbortOnError;

    if (GetToken(s_pFile) != Token_S_rparen ||
        GetToken(s_pFile) != Token_S_semicolon) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

int ImportLibrary(const char *pszName)
{
    int n;
    CLSModule *pModule;

    // skip Elastos.Runtime.CarRuntime.eco
    //
    if (!_stricmp(pszName, "Elastos.Runtime.CarRuntime.eco")) return Ret_Continue;

    // check same entry
    //
    for (n = 0; n < s_pModule->cLibraries; n++) {
        if (!_stricmp(pszName, s_pModule->ppLibNames[n]))
            return Ret_Continue;
    }

    // add library name to module
    //
    if (c_nMaxLibNumber == n) {
        ErrorReport(CAR_W_TooManyLibraries);
        return Ret_Continue;
    }
    s_pModule->ppLibNames[n] = new char[strlen(pszName) + 1];
    if (!s_pModule->ppLibNames[n]) {
        ErrorReport(CAR_E_OutOfMemory);
        return Ret_AbortOnError;
    }
    strcpy(s_pModule->ppLibNames[n], pszName);
    s_pModule->cLibraries++;

    // load it from file and add it to library
    //
    if (LoadCLS(pszName, &pModule) < 0) {
        ErrorReport(CAR_W_LoadLibrary, pszName);
        return Ret_Continue;
    }
    AddCLSLibrary(pModule);

    return Ret_Continue;
}

typedef struct ModuleInfo {
    char        *pszUunm;
    GUID        uuid;
    unsigned char       cMajorVersion;
    unsigned char       cMinorVersion;
    DWORD       dwAttribs;
    char        *pszName;
}   ModuleInfo;

int MergeCAR(const char *pszFile)
{
    int nRet;
    FILE *pFile;
    ModuleInfo *pInfo, info;
    void *pvCtxToken, *pvCtxError;
    char *pszCtxCurrentPath;

    if (_stricmp(pszFile + strlen(pszFile) - 4, ".car")) {
        ErrorReport(CAR_W_UnexpectFileType, pszFile);
        return Ret_ContinueOnError;
    }

    // Save compiling context of current file.
    //
    pvCtxToken = SaveTokenContext();
    pvCtxError = SaveErrorContext();
    pszCtxCurrentPath = new char[strlen(s_pszCurrentPath) + 1];
    strcpy(pszCtxCurrentPath, s_pszCurrentPath);
    pFile = s_pFile;

    // Save module info of current file
    //
    pInfo = (ModuleInfo *)&s_pModule->pszUunm;
    memcpy(&info, pInfo, sizeof(ModuleInfo));
    memset(pInfo, 0, sizeof(ModuleInfo));

    ErrorStringFix("merge ", NULL);
    nRet = DoCompiling(pszFile, s_pModule);
    ErrorStringFix(NULL, NULL);

    // Free allocated resource of merged module, and restore module
    // info of current file
    //
    if (pInfo->pszUunm) delete [] pInfo->pszUunm;
    if (pInfo->pszName) delete [] pInfo->pszName;
    memcpy(pInfo, &info, sizeof(ModuleInfo));

    // Restore compiling context of current file.
    //
    s_pFile = pFile;

    assert(s_pszCurrentPath);
    delete[] s_pszCurrentPath;
    s_pszCurrentPath = pszCtxCurrentPath;
    RestoreErrorContext(pvCtxError);
    RestoreTokenContext(pvCtxToken);

    return nRet;
}

// MERGE       -> k_merge s_lparen string s_rparen s_semicolon
//
int P_Merge()
{
    if (GetToken(s_pFile) != Token_S_lparen ||
        GetToken(s_pFile) != Token_string) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    if (MergeCAR(g_szCurrentToken) == Ret_AbortOnError)
        return Ret_AbortOnError;

    GenCarDependences(s_pszCarFullName, 1);

    if (GetToken(s_pFile) != Token_S_rparen ||
        GetToken(s_pFile) != Token_S_semicolon) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// IMPORTLIB   -> k_importlib s_lparen string s_rparen s_semicolon
//
int P_ImportLibrary()
{
    if (GetToken(s_pFile) != Token_S_lparen ||
        GetToken(s_pFile) != Token_string) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    if (ImportLibrary(g_szCurrentToken) == Ret_AbortOnError)
        return Ret_AbortOnError;

    if (GetToken(s_pFile) != Token_S_rparen ||
        GetToken(s_pFile) != Token_S_semicolon) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

int MergeLibrary(const char *pszName)
{
    CLSModule *pModule;

    if (LoadCLS(pszName, &pModule) < 0) {
        ErrorReport(CAR_E_LoadLibrary, pszName);
        return Ret_ContinueOnError;
    }

    if (MergeCLS(pModule, s_pModule) < 0) {
        ErrorReport(CAR_E_MergeCLS, pszName);
        // continue on error
    }

    char *pszPath = getenv("PATH");
    char szLibFullName[_MAX_PATH];
    SearchFileFromPath(pszPath, pszName, szLibFullName);
    GenCarDependences(szLibFullName, 1);

    return Ret_Continue;
}

// MERGELIB    -> k_merge s_lparen string s_rparen s_semicolon
//
int P_MergeLibrary()
{
    if (GetToken(s_pFile) != Token_S_lparen ||
        GetToken(s_pFile) != Token_string) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    if (MergeLibrary(g_szCurrentToken) == Ret_AbortOnError)
        return Ret_AbortOnError;

    if (GetToken(s_pFile) != Token_S_rparen ||
        GetToken(s_pFile) != Token_S_semicolon) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// ARG_ATTRIBS -> s_lbracket ARG_ATTRIB { s_comma ARG_ATTRIB } s_rbracket
// ARG_ATTRIB  -> k_in | k_out | k_retval
//
int P_ArgAttribs(DWORD *pAttribs)
{
    CARToken token;

    //eat "["
    GetToken(s_pFile);

    do {
        switch (GetToken(s_pFile)) {
            case Token_K_in:
                *pAttribs |= ParamAttrib_in;
                break;
            case Token_K_out:
                *pAttribs |= ParamAttrib_out;
                break;
            case Token_K_callee:
                if (!(*pAttribs & ParamAttrib_out)) {
                    ErrorReport(CAR_E_CalleeDecl);
                    return Ret_AbortOnError;
                }

                *pAttribs |= ParamAttrib_callee;
                break;
            default:
                ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            case Token_Error:
                return Ret_AbortOnError;
        }
        token = GetToken(s_pFile);
    } while (Token_S_comma == token);

    if (token != Token_S_rbracket) {
        ErrorReport(CAR_E_ExpectSymbol, "]");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// METHOD_ARG  -> [ ARG_ATTRIBS ] TYPE ident
//
int P_MethodArg(MethodDescriptor *pMethod, BOOL isLocal, BOOL isDeprecated)
{
    int n;
    DWORD dwAttribs;
    TypeDescriptor type;

    dwAttribs = 0;
    memset(&type, 0, sizeof(type));

    if (PeekToken(s_pFile) != Token_S_lbracket) {
        ErrorReport(CAR_E_ExpectParamAttrib);
    }

    if (P_ArgAttribs(&dwAttribs) == Ret_AbortOnError) {
        return Ret_AbortOnError;
    }

    if (P_Type(&type) == Ret_AbortOnError) {
        return Ret_AbortOnError;
    }

    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_ExpectParamName);
        return Ret_AbortOnError;
    }
    n = CreateMethodParam(g_szCurrentToken, pMethod);
    if (n < 0) {
        CreateError(n, "method parameter", g_szCurrentToken);
        return Ret_Continue;
    }

    //attributes conflict checking
    if (!(dwAttribs & (ParamAttrib_in | ParamAttrib_out))) {
        ErrorReport(CAR_E_ParamNoAttrib, g_szCurrentToken, pMethod->pszName);
        return Ret_AbortOnError;
    }

    if ((dwAttribs & ParamAttrib_in) &&
            ((dwAttribs & ParamAttrib_out) || (dwAttribs & ParamAttrib_callee))) {
        ErrorReport(CAR_E_IllegalParamAttrib, g_szCurrentToken, pMethod->pszName);
        return Ret_AbortOnError;
    }

    if ((dwAttribs & ParamAttrib_callee) && !isLocal &&
        (type.type != Type_ArrayOf && type.type != Type_BufferOf &&
        type.type != Type_MemoryBuf && type.type != Type_StringBuf)) {
        ErrorReport(CAR_E_CalleeParam, g_szCurrentToken, pMethod->pszName);
        return Ret_AbortOnError;
    }

    if (!IsValidParamTypeAttrib(s_pModule, &type, dwAttribs, isLocal, isDeprecated)) {
        ErrorReport(CAR_E_IllegalParamType, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    pMethod->ppParams[n]->dwAttribs = dwAttribs;
    memcpy(&pMethod->ppParams[n]->type, &type, sizeof(type));

    return Ret_Continue;
}

// METHOD      -> ident METHOD_ARGS s_semicolon
// METHOD_ARGS -> s_lparen [ METHOD_ARG { s_comma METHOD_ARG } ] s_rparen
//
int P_Method(InterfaceDirEntry *pItfDirEntry, BOOL isDeprecated)
{
    int n;
    CARToken token;
    InterfaceDescriptor *pDesc = pItfDirEntry->pDesc;

    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_ExpectMethodName);
        return Ret_AbortOnError;
    }
    n = CreateInterfaceMethod(g_szCurrentToken, pDesc);
    if (n < 0) {
        CreateError(n, "interface method", g_szCurrentToken);
        return Ret_AbortOnError;
    }
    pDesc->ppMethods[n]->type.type = Type_ECode;

    if (GetToken(s_pFile) != Token_S_lparen) {
        ErrorReport(CAR_E_ExpectSymbol, "(");
        return Ret_AbortOnError;
    }
    if (PeekToken(s_pFile) != Token_S_rparen) {
        BOOL isLocal = (pDesc->dwAttribs & InterfaceAttrib_local);
        do {
            if (P_MethodArg(pDesc->ppMethods[n], isLocal, isDeprecated) == \
                Ret_AbortOnError) {
                return Ret_AbortOnError;
            }
            token = GetToken(s_pFile);
        } while (Token_S_comma == token);

        if (Token_S_rparen != token) {
            ErrorReport(CAR_E_ExpectSymbol, ")");
            return Ret_AbortOnError;
        }
    }
    else {
        GetToken(s_pFile); // skip ")"
    }

    if (GetToken(s_pFile) != Token_S_semicolon) {
        ErrorReport(CAR_E_ExpectSymbol, ";");
        return Ret_AbortOnError;
    }
    return Ret_Continue;
}

void AddInterfaceParent(const char *pszName, InterfaceDescriptor *pDesc)
{
    int n;
    InterfaceDescriptor *pParent;

    n = RetrieveInterface(pszName, s_pModule, FALSE);
    if (n < 0) {
        ErrorReport(CAR_E_NotFound, "interface", pszName);
        return;
    }
    pParent = s_pModule->ppInterfaceDir[n]->pDesc;

    if (pParent == pDesc) {
        ErrorReport(CAR_E_NestedInherit, "interface", pszName);
        return;
    }
    if (!(pParent->dwAttribs & InterfaceAttrib_defined)) {
        ErrorReport(CAR_E_NotFound, "interface",
                s_pModule->ppInterfaceDir[n]->pszName);
        return;
    }
    pDesc->sParentIndex = n;

    if (!(pDesc->dwAttribs & InterfaceAttrib_local)) {
        if (pParent->dwAttribs & InterfaceAttrib_local) {
            ErrorReport(CAR_W_LocalParent, pszName);
            fputs("Treated as local automatically.\n", stderr);
            pDesc->dwAttribs |= InterfaceAttrib_local;
        }
    }
}

// ITF_BODY    -> [ k_extends ident ]
//                s_lbrace { METHOD { s_semicolon } } s_rbrace
//
int P_InterfaceBody(InterfaceDirEntry *pDirEntry, BOOL isDeprecated)
{
    CARToken token;
    InterfaceDescriptor *pDesc = pDirEntry->pDesc;

    token = GetToken(s_pFile);
    if (Token_K_extends == token) {
        if (GetToken(s_pFile) != Token_ident) {
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
        }
        AddInterfaceParent(g_szCurrentToken, pDesc);

        token = GetToken(s_pFile);
    }

    if (Token_S_lbrace != token) {
        ErrorReport(CAR_E_ExpectSymbol, "{ or ;");
        return Ret_AbortOnError;
    }
    while (PeekToken(s_pFile) != Token_S_rbrace) {
        if (P_Method(pDirEntry, isDeprecated) == Ret_AbortOnError)
            return Ret_AbortOnError;
        while (PeekToken(s_pFile) == Token_S_semicolon) GetToken(s_pFile);
    }
    GetToken(s_pFile); // skip "}"

    pDesc->dwAttribs |= InterfaceAttrib_defined;

    if (0 == pDesc->cMethods) ErrorReport(CAR_W_NoMethods);
    return Ret_Continue;
}

// INTERFACE   -> [ ITF_PROPERTIES ] ITF_ID [ ITF_BODY | s_semicolon ]
// ITF_ID      -> interface ident
//
int P_Interface(CARToken token, DWORD properties)
{
    int r, ret;
    DWORD dwAttribs = 0;
    InterfaceDescriptor *pDesc;
    BOOL isDeprecated = FALSE;

    //set properties
    if (properties & Prop_deprecated) isDeprecated = TRUE;
    if (properties & Prop_local) dwAttribs |= InterfaceAttrib_local;
    if ((properties & Prop_private)) {
        //error
        ErrorReport(CAR_E_IllegalInterfaceProperties);
    }

    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_ExpectInterfaceName);
        return Ret_AbortOnError;
    }

    if (Token_K_interface == token) {
        if (g_szCurrentToken[0] != 'I') {
            ErrorReport(CAR_E_IllegalInterfaceName, g_szCurrentToken);
            return Ret_AbortOnError;
        }
        dwAttribs |= InterfaceAttrib_t_normal;
    }
    else {
        if (g_szCurrentToken[0] != 'J') {
            ErrorReport(CAR_E_IllegalCallbacksOrDelegatesName, g_szCurrentToken);
            return Ret_AbortOnError;
        }
        if (Token_K_callbacks == token) dwAttribs |= InterfaceAttrib_t_callbacks;
        if (Token_K_delegates == token) dwAttribs |= InterfaceAttrib_t_delegates;
    }

    if (((dwAttribs & InterfaceAttrib_t_callbacks) || (dwAttribs & InterfaceAttrib_t_delegates))
            && (dwAttribs & InterfaceAttrib_local)) {
        ErrorReport(CAR_E_LocalConflict);
    }

    r = CreateInterfaceDirEntry(g_szCurrentToken, s_pModule, dwAttribs);

    if (Token_S_semicolon == PeekToken(s_pFile)) {
        GetToken(s_pFile); // ignore ";"
        if (INTERFACE_ATTR(dwAttribs) != 0) {
            ErrorReport(CAR_E_DeclaredInterfaceProp);
            return Ret_ContinueOnError;
        }
        if (r < 0) {
            if (r != CLSError_DupEntry)
                CreateError(r, "interface", g_szCurrentToken);
        }
        else {
            s_pModule->ppInterfaceDir[r]->pDesc->dwAttribs |= dwAttribs;
        }
        return Ret_Continue;
    }
    if (r < 0) {
        CreateError(r, "interface", g_szCurrentToken);
        return Ret_AbortOnError;
    }
    pDesc = s_pModule->ppInterfaceDir[r]->pDesc;
    pDesc->dwAttribs = dwAttribs;
    s_pModule->pDefinedInterfaceIndex[s_pModule->cDefinedInterfaces++] = r;

    ret = P_InterfaceBody(s_pModule->ppInterfaceDir[r], isDeprecated);
    if (ret != Ret_Continue) return ret;

    if ((dwAttribs & InterfaceAttrib_t_callbacks)
            || (dwAttribs & InterfaceAttrib_t_delegates)) {
        GenerateHandlerInterface(
            s_pModule->ppInterfaceDir[r]->pszName,
            s_pModule,
            s_pModule->ppInterfaceDir[r]);
    }

    return Ret_Continue;
}

/*
void GenerateDispatchInterface(ClassDescriptor *pDesc)
{
    int n, nDispIndex;
    const char *pszDispName;
    InterfaceDescriptor *pDisp, *pIntf;
    ClassInterface *pClsIntf;

    n = RetrieveInterface("IDispatch", s_pModule, FALSE);
    if (n < 0) {
        ErrorReport(CAR_E_NotFound, "interface", "IDispatch");
        return;
    }

    pszDispName = GenerateInternalName("Dispatch");
    nDispIndex = CreateInterfaceDirEntry(pszDispName, s_pModule, 0);
    if (nDispIndex < 0) {
        CreateError(nDispIndex, "interface", pszDispName);
        return;
    }

    pDisp = s_pModule->ppInterfaceDir[nDispIndex]->pDesc;
    pDisp->sParentIndex = n;
    pDisp->dwAttribs = InterfaceAttrib_dual;

    for (n = 0; n < pDesc->cInterfaces; n++) {
        pIntf = s_pModule-> \
            ppInterfaceDir[pDesc->ppInterfaces[n]->sIndex]->pDesc;
        if (!(pDesc->ppInterfaces[n]->\
            wAttribs & ClassInterfaceAttrib_callback)) {
            if (InterfaceMethodsAppend(s_pModule, pIntf, pDisp) < 0) {
                ErrorReport(CAR_E_GenDisp);
                return;
            }
        }
    }

    n = CreateClassInterface(nDispIndex, pDesc);
    if (n < 0) {
        CreateError(n, "class dispatch interface", pszDispName);
        return;
    }
    pDesc->ppInterfaces[n]->wAttribs |= ClassInterfaceAttrib_autoadded;

    // Interchange the dispatch interface to first position.
    //
    if (0 != n) {
        pClsIntf = pDesc->ppInterfaces[0];
        pDesc->ppInterfaces[0] = pDesc->ppInterfaces[n];
        pDesc->ppInterfaces[n] = pClsIntf;
    }
}
*/

typedef struct TempClassAttribs {
    DWORD       dwAttribs;
    USHORT      cAggregates;
    USHORT      cAspects;
    USHORT      cClasses;
    USHORT      aggrIndexs[c_nMaxClassAspects];
    USHORT      aspectIndexs[c_nMaxClassAspects];
    USHORT      classIndexs[c_nMaxAggrClassesOfAspect];
}   TempClassAttribs;

void CheckInterfaceDupMethodNameEx(InterfaceDirEntry *pientry)
{
    if (!strcmp(pientry->pszName, "IInterface")) return;

    int n, m;
    InterfaceDescriptor *pDesc = pientry->pDesc;

    //Check dup method name with the interface "IInterface"
    for (n = 0; n < pDesc->cMethods; n++) {
        if (!strcmp(pDesc->ppMethods[n]->pszName, "Probe")
            || !strcmp(pDesc->ppMethods[n]->pszName, "AddRef")
            || !strcmp(pDesc->ppMethods[n]->pszName, "Release")
            || !strcmp(pDesc->ppMethods[n]->pszName, "GetInterfaceID")) {
            ErrorReport(CAR_E_DupMethodName, pDesc->ppMethods[n]->pszName);
        }
    }

    //Check dup method name with the parent interfaces without "IInterface"
    if (0 != pDesc->sParentIndex) {
        int nParentIndex = pDesc->sParentIndex;
        do {
            for (n = 0; n < pDesc->cMethods; n++) {
                for (m = 0; m < s_pModule->ppInterfaceDir[nParentIndex]->\
                    pDesc->cMethods; m++) {
                    if (!strcmp(pDesc->ppMethods[n]->pszName,
                        s_pModule->ppInterfaceDir[nParentIndex]->\
                        pDesc->ppMethods[m]->pszName)) {
                        ErrorReport(CAR_E_DupMethodName,
                                    pDesc->ppMethods[n]->pszName);
                    }
                }
            }
            nParentIndex = s_pModule->ppInterfaceDir[nParentIndex]->\
                           pDesc->sParentIndex;
        } while (nParentIndex != 0);
    }
}

void CheckInterfaceDupMethodName(
    InterfaceDescriptor *pSrc, InterfaceDescriptor *pDest)
{
    int n, m;

    //same parent
    if (pSrc == pDest)
        return;

    if (0 != pSrc->sParentIndex) {
        CheckInterfaceDupMethodName(
            s_pModule->ppInterfaceDir[pSrc->sParentIndex]->pDesc, pDest);
    }
    if (0 != pDest->sParentIndex) {
        CheckInterfaceDupMethodName(
            pSrc, s_pModule->ppInterfaceDir[pDest->sParentIndex]->pDesc);
    }

    for (n = 0; n < pSrc->cMethods; n++)
        for (m = 0; m < pDest->cMethods; m++) {
            if (!strcmp(pSrc->ppMethods[n]->pszName,
                pDest->ppMethods[m]->pszName)) {
                ErrorReport(CAR_E_DupMethodName, pSrc->ppMethods[n]->pszName);
            }
        }
}

void CheckClassDupMethodName(ClassDescriptor *pDesc)
{
    int n, m;

    for (n = 0; n < pDesc->cInterfaces; n++)
        for (m = n + 1; m < pDesc->cInterfaces; m++)
            CheckInterfaceDupMethodName(
                s_pModule->ppInterfaceDir \
                    [pDesc->ppInterfaces[n]->sIndex]->pDesc,
                s_pModule->ppInterfaceDir \
                    [pDesc->ppInterfaces[m]->sIndex]->pDesc);
}

void CheckClassAttribs(ClassDescriptor *pDesc)
{
    int j=0;
    char szAttribName[2][20];

    if (pDesc->dwAttribs & ClassAttrib_t_aspect) {
        if (pDesc->dwAttribs & ClassAttrib_singleton) {
            ErrorReport(CAR_E_AttribConflict, "singleton", "aspect type");
        }
    }

    if (j >= 2) {
        ErrorReport(CAR_E_AttribConflict, szAttribName[0], szAttribName[1]);
    }

    if (pDesc->dwAttribs & ClassAttrib_aspect) {
        if (!(pDesc->dwAttribs & ClassAttrib_t_regime)) {
            ErrorReport(CAR_E_AspectUse);
        }
    }

    if (!(pDesc->dwAttribs & ClassAttrib_t_aspect)) {
        AddConstClassInterface("IObject", pDesc);
    }
    else {
        AddConstClassInterface("IAspect", pDesc);
    }

    if (pDesc->dwAttribs & ClassAttrib_t_regime) {
        AddConstClassInterface("IRegime", pDesc);
    }

    CheckClassDupMethodName(pDesc);
}

// CLASS     -> k_class s_lparen ident { s_comma ident } s_rparen
//
int GetClasses(USHORT *pClassIndexs)
{
    int n, cClasses;
    CARToken token;

    cClasses = 0;
    do {
        if (GetToken(s_pFile) != Token_ident) {
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
        }
        n = RetrieveClass(g_szCurrentToken, s_pModule, FALSE);
        if (n < 0) {
            ErrorReport(CAR_E_NotFound, "class", g_szCurrentToken);
            goto ErrorSkip;
        }

        if (s_pModule->ppClassDir[n]->pDesc->dwAttribs & \
            (ClassAttrib_t_aspect | ClassAttrib_t_generic
             | ClassAttrib_t_sink | ClassAttrib_t_clsobj)) {
            ErrorReport(CAR_E_NotClass, g_szCurrentToken);
            goto ErrorSkip;
        }
        if (cClasses >= c_nMaxAggrClassesOfAspect) {
            ErrorReport(CAR_E_FullEntry, "classes");
            goto ErrorSkip;
        }

        pClassIndexs[cClasses++] = n;

ErrorSkip:
        token = PeekToken(s_pFile);
        if (Token_S_comma == token) token = GetToken(s_pFile);

    } while (Token_S_comma == token);

    return cClasses;
}

inline int P_ClassForAspect(TempClassAttribs *pAttr)
{
    int n;

    n = GetClasses(pAttr->classIndexs);
    if (n <= 0) return n;

    pAttr->cClasses= n;
    return Ret_Continue;
}

// AGGREGATE  -> k_aggregate s_lparen ident { s_comma ident } s_rparen
// ASPECT     -> k_aspect s_lparen ident { s_comma ident } s_rparen
//
int GetAspects(USHORT *pAspectIndexs)
{
    int n, cAspects;
    CARToken token;

    cAspects = 0;
    do {
        token = PeekToken(s_pFile);

        if (Token_ident != token) {
            if ((0 == cAspects)
                    || ((Token_K_pertainsto != token)
                    && (Token_K_substitutes != token))){
                ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
                return Ret_AbortOnError;
            }
            else break;
        }
        else token = GetToken(s_pFile);

        n = RetrieveClass(g_szCurrentToken, s_pModule, FALSE);
        if (n < 0) {
            ErrorReport(CAR_E_NotFound, "aspect", g_szCurrentToken);
            goto ErrorSkip;
        }

        if (!(s_pModule->ppClassDir[n]->pDesc->dwAttribs & \
            ClassAttrib_t_aspect)) {
            ErrorReport(CAR_E_NotAspect, g_szCurrentToken);
            goto ErrorSkip;
        }
        if (cAspects >= c_nMaxClassAspects) {
            ErrorReport(CAR_E_FullEntry, "aspects");
            goto ErrorSkip;
        }

        pAspectIndexs[cAspects++] = n;

ErrorSkip:
        token = PeekToken(s_pFile);
        if (Token_S_comma == token) token = GetToken(s_pFile);

    } while (Token_S_comma == token);

    return cAspects;
}

inline int P_Aggregate(TempClassAttribs *pAttr)
{
    int n;

    n = GetAspects(pAttr->aggrIndexs);
    if (n <= 0) return n;

    pAttr->cAggregates = n;
    return Ret_Continue;
}

inline int P_Aspect(TempClassAttribs *pAttr)
{
    int n;

    n = GetAspects(pAttr->aspectIndexs);
    if (n <= 0) return n;

    pAttr->cAspects = n;
    return Ret_Continue;
}

BOOL IsInterfaceParent(InterfaceDescriptor *pDesc, int nIndex)
{
    while (0 != pDesc->sParentIndex) {
        if (nIndex == pDesc->sParentIndex) return TRUE;
        pDesc = s_pModule->ppInterfaceDir[pDesc->sParentIndex]->pDesc;
    }
    return FALSE;
}

BOOL IsClsIntfParent(ClassDescriptor *pDesc, int nIndex)
{
    int n;

    if (0 == nIndex) return TRUE;

    for (n = 0; n < pDesc->cInterfaces; n++) {
        if (IsInterfaceParent(s_pModule->ppInterfaceDir
            [pDesc->ppInterfaces[n]->sIndex]->pDesc, nIndex))
            return TRUE;
    }
    return FALSE;
}

int TryGetParentClsIntf(ClassDescriptor *pDesc, InterfaceDescriptor *pIntf)
{
    int n;

    if (0 == pIntf->sParentIndex) return -1;

    for (n = 0; n < pDesc->cInterfaces; n++) {
        if (IsInterfaceParent(pIntf, pDesc->ppInterfaces[n]->sIndex))
            return n;
    }
    return -1;
}

BOOL IsClassInterfaceVirtuallyImplementedByAnyone(
    ClassInterface* pClsIntf, int clsIndex)
{
    assert(pClsIntf && s_pModule->ppClassDir[clsIndex]);
    ClassDescriptor* pDesc = s_pModule->ppClassDir[clsIndex]->pDesc;

    int index = pClsIntf->sIndex;

    for (int n = 0; n < pDesc->cInterfaces; n++) {
        ClassInterface* pClsIntfn = pDesc->ppInterfaces[n];
        if (index == pClsIntfn->sIndex) {
            return (pClsIntfn->wAttribs & ClassInterfaceAttrib_virtual);
        }
    }

    return FALSE;
}

// CLS_ITF     -> [ k_virtual | k_hidden | k_callback ]
//                k_interface ident s_semicolon
//
int P_ClassInterface(ClassDirEntry *pClass)
{
    int n, m, x, nIndexOfInterface;
    CARToken token;
    WORD wAttribs = 0;
    ClassDescriptor *pDesc;
    InterfaceDescriptor *pIntf;
    char szInterfaceName[c_nMaxTokenSize + 1];

    pDesc = pClass->pDesc;

    // [ k_virtual | k_hidden | k_privileged | k_asynchronous | k_filtering ] k_interface
    //
    token = GetToken(s_pFile);
    switch (token) {
        case Token_K_virtual:
            pDesc->dwAttribs |= ClassAttrib_hasvirtual;
            wAttribs |= ClassInterfaceAttrib_virtual;
            token = GetToken(s_pFile);
            break;
        case Token_K_hidden:
            wAttribs |= ClassInterfaceAttrib_hidden;
            token = GetToken(s_pFile);
            break;
        case Token_K_privileged:
            wAttribs |= ClassInterfaceAttrib_privileged;
            token = GetToken(s_pFile);
            break;
        case Token_K_asynchronous:
            pDesc->dwAttribs |= ClassAttrib_hasasynchronous;
            wAttribs |= ClassInterfaceAttrib_async;
            token = GetToken(s_pFile);
            break;
        case Token_K_filtering:
            pDesc->dwAttribs |= ClassAttrib_hasfilter;
            wAttribs |= ClassInterfaceAttrib_filter;
            token = GetToken(s_pFile);
            break;
        default:
            break;
    }

    if (Token_K_callbacks == token) {
        if (!(wAttribs & ClassInterfaceAttrib_filter)) {
            wAttribs |= ClassInterfaceAttrib_callback;
            pDesc->dwAttribs |= ClassAttrib_hascallback;
        }
    }
    else if (Token_K_callbacksink == token) {
        wAttribs |= ClassInterfaceAttrib_callbacksink;
    }
    else if (Token_K_delegates == token) {
        if (!(wAttribs & ClassInterfaceAttrib_filter)) {
            wAttribs |= ClassInterfaceAttrib_delegate;
            wAttribs |= ClassInterfaceAttrib_callback;
            pDesc->dwAttribs |= ClassAttrib_hascallback;
        }
    }
    else if (Token_K_delegatesink == token) {
        wAttribs |= ClassInterfaceAttrib_delegatesink;
    }
    else if (Token_K_interface != token) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }

    // ident
    //
    if (GetToken(s_pFile) != Token_ident) {
        ErrorReport(CAR_E_ExpectInterfaceName);
        return Ret_AbortOnError;
    }

    nIndexOfInterface = n = RetrieveInterface(g_szCurrentToken, s_pModule, FALSE);
    if (n < 0) {
        ErrorReport(CAR_E_NotFound, "interface", g_szCurrentToken);
        goto ErrorSkip;
    }

    strcpy(szInterfaceName, s_pModule->ppInterfaceDir[n]->pszName);
    pIntf = s_pModule->ppInterfaceDir[n]->pDesc;

    //checking whether the interfaces declared in class and the interfaces
    //defined in module match.
    if (pIntf->dwAttribs & InterfaceAttrib_t_callbacks) {
        if (!(wAttribs & ClassInterfaceAttrib_callback)
                && !(wAttribs & ClassInterfaceAttrib_callbacksink)
                && !(wAttribs & ClassInterfaceAttrib_filter))
            ErrorReport(CAR_E_UnmatchedInterface, g_szCurrentToken);
    }
    else if (pIntf->dwAttribs & InterfaceAttrib_t_delegates) {
        if (!(wAttribs & ClassInterfaceAttrib_delegate)
                && !(wAttribs & ClassInterfaceAttrib_delegatesink)
                && !(wAttribs & ClassInterfaceAttrib_filter))
            ErrorReport(CAR_E_UnmatchedInterface, g_szCurrentToken);
    }
    else {
        if ((wAttribs & ClassInterfaceAttrib_callback)
                || (wAttribs & ClassInterfaceAttrib_callbacksink)
                || (wAttribs & ClassInterfaceAttrib_delegate)
                || (wAttribs & ClassInterfaceAttrib_delegatesink))
            ErrorReport(CAR_E_UnmatchedInterface, g_szCurrentToken);
    }

    //checking only aspect can have privileged interface;
    if (!(pDesc->dwAttribs & ClassAttrib_t_aspect) &&
            (wAttribs & ClassInterfaceAttrib_privileged)) {
        ErrorReport(CAR_E_IllegalPrivilegedInterface,
                pClass->pszName, s_pModule->ppInterfaceDir[n]->pszName);
        return Ret_AbortOnError;
    }

    if ((pDesc->dwAttribs & ClassAttrib_t_generic) &&
            (pDesc->dwAttribs & ClassAttrib_hasvirtual)) {
        ErrorReport(CAR_E_GenericCouldntHaveVirtualInterface,
                pClass->pszName, s_pModule->ppInterfaceDir[n]->pszName);
        return Ret_AbortOnError;
    }

    if (!(pIntf->dwAttribs & InterfaceAttrib_defined)) {
        ErrorReport(CAR_E_NotFound, "interface", g_szCurrentToken);
        goto ErrorSkip;
    }

    // Eliminate parent-son relation of class interfaces.
    //
    m = TryGetParentClsIntf(pDesc, pIntf);
    if (m >= 0) {
        ErrorReport(CAR_E_InheritanceConflict, g_szCurrentToken, pClass->pszName);
        goto ErrorSkip; // parent interface is replaced
    }
    else if (IsClsIntfParent(pDesc, n)) {
        ErrorReport(CAR_E_InheritanceConflict, g_szCurrentToken, pClass->pszName);
        goto ErrorSkip; // ignore parent interface of others
    }

    n = CreateClassInterface(n, pDesc);
    if (n < 0) {
        ErrorReport(CAR_E_DupEntry, "class interface", g_szCurrentToken);
        goto ErrorSkip;
    }
    if (!(wAttribs & ClassInterfaceAttrib_callback)) {
        if (!(pIntf->dwAttribs & InterfaceAttrib_local))
            pDesc->dwAttribs &= ~ClassAttrib_classlocal;
        pDesc->dwAttribs |= ClassAttrib_defined;
    }

    if (pDesc->dwAttribs & ClassAttrib_hasparent) {
        BOOL inherit = IsClassInterfaceVirtuallyImplementedByAnyone(
            pDesc->ppInterfaces[n], pDesc->sParentIndex);
        if (inherit) wAttribs |= ClassInterfaceAttrib_inherited;
    }

    pDesc->ppInterfaces[n]->wAttribs = wAttribs;

    if ((wAttribs & ClassInterfaceAttrib_callback)
        && !(wAttribs & ClassInterfaceAttrib_delegate)) {
        if (s_pModule->ppInterfaceDir[nIndexOfInterface]->pDesc->dwAttribs & InterfaceAttrib_local) {
            ErrorReport(CAR_E_LocalConflict);
            goto ErrorSkip;
        }
        GenerateHandlerInterface(szInterfaceName, s_pModule, s_pModule->ppInterfaceDir[nIndexOfInterface]);
    }
    else if (wAttribs & ClassInterfaceAttrib_async) {
        if (s_pModule->ppInterfaceDir[nIndexOfInterface]->pDesc->dwAttribs & InterfaceAttrib_local) {
            ErrorReport(CAR_E_LocalConflict);
            goto ErrorSkip;
        }
        x = GenerateAsyncCallbackInterface(
                szInterfaceName,
                s_pModule,
                s_pModule->ppInterfaceDir[nIndexOfInterface]);

        if (x > 0) s_pModule->ppInterfaceDir[x]->pDesc->dwAttribs |= InterfaceAttrib_defined;
        strcat(szInterfaceName, "Async");
        x = AddConstClassInterface(szInterfaceName, pDesc);
        if (x > 0) {
            pDesc->ppInterfaces[x]->wAttribs |= ClassInterfaceAttrib_callback;
            pDesc->ppInterfaces[x]->wAttribs |= ClassInterfaceAttrib_async;
            pDesc->dwAttribs |= ClassAttrib_hascallback;
        }
    }

    if (!strcmp("IActivity", szInterfaceName)) {
        pClass->pDesc->dwAttribs |= ClassAttrib_activity;
    }
    else if (!strcmp("IService", szInterfaceName)) {
        pClass->pDesc->dwAttribs |= ClassAttrib_service;
    }
    else if (!strcmp("IBroadcastReceiver", szInterfaceName)) {
        pClass->pDesc->dwAttribs |= ClassAttrib_receiver;
    }
    else if (!strcmp("IContentProvider", szInterfaceName)) {
        pClass->pDesc->dwAttribs |= ClassAttrib_provider;
    }

ErrorSkip:
    if (GetToken(s_pFile) != Token_S_semicolon) {
        ErrorReport(CAR_E_ExpectSymbol, ";");
        return Ret_AbortOnError;
    }
    return Ret_Continue;
}

int GenerateClassObject(ClassDirEntry *pClass)
{
    ClassDescriptor *pDesc;

    pDesc = pClass->pDesc;

    assert(0 == pDesc->sCtorIndex);

    int r, n;
    char szName[c_nMaxTokenSize + 1];
    int attr = InterfaceAttrib_defined | InterfaceAttrib_clsobj | InterfaceAttrib_t_normal;
    int classAttr = ClassAttrib_defined | ClassAttrib_t_clsobj;
    if (pDesc->dwAttribs & ClassAttrib_t_generic) {
        attr |= InterfaceAttrib_generic;
        classAttr |= ClassAttrib_t_generic;
    }
    if (pDesc->dwAttribs & ClassAttrib_naked) {
        classAttr |= ClassAttrib_naked;
    }

    pDesc->dwAttribs |= ClassAttrib_hasctor;

    sprintf(szName, "I%sClassObject", pClass->pszName);
    r = CreateInterfaceDirEntry(szName, s_pModule, 0);
    if (r < 0) {
        CreateError(r, "interface", szName);
        return Ret_AbortOnError;
    }
    pDesc->sCtorIndex = r;

    s_pModule->ppInterfaceDir[pDesc->sCtorIndex]->pDesc->dwAttribs  = attr;

    n = RetrieveInterface("IClassObject", s_pModule, TRUE);
    if (n < 0) {
        return Ret_AbortOnError;
    }
    s_pModule->ppInterfaceDir[pDesc->sCtorIndex]->pDesc->sParentIndex = n;

    s_pModule->pDefinedInterfaceIndex[s_pModule->cDefinedInterfaces++] = r;

    sprintf(szName, "%sClassObject", pClass->pszName);
    r = CreateClassDirEntry(szName, s_pModule, 0);

    n = CreateClassInterface(pDesc->sCtorIndex, s_pModule->ppClassDir[r]->pDesc);
    if (n < 0) {
        if (n != CLSError_DupEntry)
            CreateError(n, "class interface",
                s_pModule->ppInterfaceDir[pDesc->sCtorIndex]->pszName);
        return Ret_AbortOnError;
    }

    if (r < 0) {
        if (r != CLSError_DupEntry)
            CreateError(r, "class", szName);
    }
    else {
        s_pModule->ppClassDir[r]->pDesc->dwAttribs = classAttr;
    }
    return Ret_Continue;
}

void AddClassParent(
    const char *pszName, ClassDirEntry* pClass)
{
    ClassDescriptor *pDesc = pClass->pDesc;
    int n;
    ClassDescriptor *pParent;

    n = RetrieveClass(pszName, s_pModule, FALSE);
    if (n < 0) {
        ErrorReport(CAR_E_NotFound, "class", pszName);
        return;
    }
    pParent = s_pModule->ppClassDir[n]->pDesc;

    if (pDesc == pParent) {
        ErrorReport(CAR_E_NestedInherit,
                "class", s_pModule->ppClassDir[n]->pszName);
        return;
    }
    if (!(pParent->dwAttribs & ClassAttrib_defined)) {
        ErrorReport(CAR_E_NotFound, "class", pszName);
        return;
    }
    if (pParent->dwAttribs & ClassAttrib_t_generic) {
        ErrorReport(CAR_E_CouldNotInheritGeneric, pszName);
        return;
    }
    if (pParent->dwAttribs & ClassAttrib_final) {
        ErrorReport(CAR_E_CouldNotInheritFinalClass, pszName);
        return;
    }

    pDesc->sParentIndex = n;
    pDesc->dwAttribs |= ClassAttrib_hasparent;
}

void AddGenericParent(
    const char *pszName, ClassDirEntry* pClass)
{
    int n;
    ClassDescriptor *pParent;
    ClassDescriptor *pDesc = pClass->pDesc;

    n = RetrieveClass(pszName, s_pModule, FALSE);
    if (n < 0) {
        ErrorReport(CAR_E_NotFound, "Generic", pszName);
        return;
    }
    pParent = s_pModule->ppClassDir[n]->pDesc;

    if (pDesc == pParent) {
        ErrorReport(CAR_E_NestedInherit,
                "Generic", s_pModule->ppClassDir[n]->pszName);
        return;
    }
    if (!(pParent->dwAttribs & ClassAttrib_defined)) {
        ErrorReport(CAR_E_NotFound, "Generic", pszName);
        return;
    }
    if (!(pParent->dwAttribs & ClassAttrib_t_generic)) {
        ErrorReport(CAR_E_NotGeneric, pszName);
        return;
    }
    pDesc->sParentIndex = n;
    pDesc->dwAttribs |= ClassAttrib_hasparent;
}

// CLS_CTOR    -> k_constructor s_lparen [ METHOD_ARG { s_comma METHOD_ARG } ] s_rparen s_semicolon
// METHOD_ARG  -> [ ARG_ATTRIBS ] TYPE ident
//
int P_ClassCtorMethod(ClassDirEntry *pClass, BOOL isDeprecated)
{
    InterfaceDescriptor *pIntfDesc = NULL;
    ClassDescriptor *pClsDesc;
    pClsDesc = pClass->pDesc;

    if (0 == pClsDesc->sCtorIndex) {
        if (GenerateClassObject(pClass) == Ret_AbortOnError)
            return Ret_AbortOnError;
    }

    pIntfDesc = s_pModule->ppInterfaceDir[pClsDesc->sCtorIndex]->pDesc;

    int n;
    CARToken token;

    GetToken(s_pFile); // Skip k_constructor

    n = CreateInterfaceMethod("Method", pIntfDesc);
    if (n < 0) {
        CreateError(n, "interface method", g_szCurrentToken);
        return Ret_AbortOnError;
    }
    pIntfDesc->ppMethods[n]->type.type = Type_ECode;

    if (GetToken(s_pFile) != Token_S_lparen) {
        ErrorReport(CAR_E_ExpectSymbol, "(");
        return Ret_AbortOnError;
    }
    if (pClass->pDesc->dwAttribs & ClassAttrib_local) {
        pIntfDesc->dwAttribs |= InterfaceAttrib_local;
    }
    if (PeekToken(s_pFile) != Token_S_rparen) {
        if (pClass->pDesc->dwAttribs & ClassAttrib_singleton) {
            ErrorReport(CAR_E_ParameterInSingletonCtor);
            return Ret_AbortOnError;
        }
        if (pClass->pDesc->dwAttribs & ClassAttrib_t_aspect) {
            ErrorReport(CAR_E_ContructorWithAspect);
            return Ret_AbortOnError;
        }
        BOOL isLocal = (pClsDesc->dwAttribs & ClassAttrib_local);
        do {
            if (P_MethodArg(pIntfDesc->ppMethods[n], isLocal, isDeprecated) == \
                Ret_AbortOnError) {
                return Ret_AbortOnError;
            }
            token = GetToken(s_pFile);
        } while (Token_S_comma == token);

        if (Token_S_rparen != token) {
            ErrorReport(CAR_E_ExpectSymbol, ")");
            return Ret_AbortOnError;
        }

        pIntfDesc->ppMethods[n]->dwAttribs |= MethodAttrib_NonDefaultCtor;
    }
    else {
        GetToken(s_pFile); // skip ")"
        pIntfDesc->ppMethods[n]->dwAttribs |= MethodAttrib_DefaultCtor;
        pClass->pDesc->dwAttribs |= ClassAttrib_hasDefaultCtor;
    }

    if (GetToken(s_pFile) != Token_S_semicolon) {
        ErrorReport(CAR_E_ExpectSymbol, ";");
        return Ret_AbortOnError;
    }

    //Create the method name combined with paramters' name
    char szMethodName[c_nMaxTokenSize + 1] = {'\0'};
    strcpy(szMethodName, "CreateObjectWith");
    int nCurLength = strlen("CreateObjectWith");
    if (pIntfDesc->ppMethods[n]->cParams != 0) {
        int nMeth;
        for (nMeth = 0; nMeth < pIntfDesc->ppMethods[n]->cParams; nMeth++) {
            if (pIntfDesc->ppMethods[n]->ppParams[nMeth]->dwAttribs & ParamAttrib_out) {
                ErrorReport(CAR_E_OutParameterInCtor);
                return Ret_AbortOnError;
            }
            strcat(szMethodName, pIntfDesc->ppMethods[n]->ppParams[nMeth]->pszName);
            if (isalpha(szMethodName[nCurLength]) && islower(szMethodName[nCurLength])) {
                szMethodName[nCurLength] = toupper(szMethodName[nCurLength]);
            }
            nCurLength = strlen(szMethodName);
        }
    }
    else {
        strcat(szMethodName, "DefaultCtor");
        nCurLength = strlen(szMethodName);
    }
    delete pIntfDesc->ppMethods[n]->pszName;
    pIntfDesc->ppMethods[n]->pszName = NULL;
    pIntfDesc->ppMethods[n]->pszName = new char[nCurLength + 1];
    if (pIntfDesc->ppMethods[n]->pszName == NULL) {
        return Ret_AbortOnError;
    }
    strcpy(pIntfDesc->ppMethods[n]->pszName, szMethodName);

    // Append "[out] IInterface **ppNewObj" Param
    TypeDescriptor type;
    int i;

    memset(&type, 0, sizeof(type));
    type.type = Type_interface;
    type.sIndex = SelectInterfaceDirEntry("IInterface", s_pModule);
    type.nPointer = 2;

    i = CreateMethodParam("newObj", pIntfDesc->ppMethods[n]);
    if (i < 0) {
        CreateError(n, "method parameter", "newObj");
        return Ret_AbortOnError;
    }
    pIntfDesc->ppMethods[n]->ppParams[i]->dwAttribs = ParamAttrib_out;
    memcpy(&pIntfDesc->ppMethods[n]->ppParams[i]->type, &type, sizeof(type));

    return Ret_Continue;
}

int AddTrivialClassCtorMethod(ClassDirEntry *pClass)
{
    InterfaceDescriptor *pIntfDesc = NULL;
    ClassDescriptor *pClsDesc;
    pClsDesc = pClass->pDesc;

    if (0 == pClsDesc->sCtorIndex) {
        if (GenerateClassObject(pClass) == Ret_AbortOnError)
            return Ret_AbortOnError;
    }

    pIntfDesc = s_pModule->ppInterfaceDir[pClsDesc->sCtorIndex]->pDesc;

    int n;

    n = CreateInterfaceMethod("Method", pIntfDesc);
    if (n < 0) {
        CreateError(n, "interface method", g_szCurrentToken);
        return Ret_AbortOnError;
    }
    pIntfDesc->ppMethods[n]->type.type = Type_ECode;

    pIntfDesc->ppMethods[n]->dwAttribs |= MethodAttrib_TrivialCtor | MethodAttrib_DefaultCtor;
    pClass->pDesc->dwAttribs |= ClassAttrib_hasTrivialCtor | ClassAttrib_hasDefaultCtor;

    //Create the method name combined with "DefaultCtor"
    char szMethodName[c_nMaxTokenSize + 1] = {'\0'};
    strcpy(szMethodName, "CreateObjectWith");
    int nCurLength = strlen("CreateObjectWith");
    strcat(szMethodName, "DefaultCtor");
    nCurLength = strlen(szMethodName);
    delete pIntfDesc->ppMethods[n]->pszName;
    pIntfDesc->ppMethods[n]->pszName = NULL;
    pIntfDesc->ppMethods[n]->pszName = new char[nCurLength + 1];
    if (pIntfDesc->ppMethods[n]->pszName == NULL) {
        return Ret_AbortOnError;
    }
    strcpy(pIntfDesc->ppMethods[n]->pszName, szMethodName);

    // Append "[out] IInterface **ppNewObj" Param
    TypeDescriptor type;
    int i;

    memset(&type, 0, sizeof(type));
    type.type = Type_interface;
    type.sIndex = SelectInterfaceDirEntry("IInterface", s_pModule);
    type.nPointer = 2;

    i = CreateMethodParam("newObj", pIntfDesc->ppMethods[n]);
    if (i < 0) {
        CreateError(n, "method parameter", "newObj");
        return Ret_AbortOnError;
    }
    pIntfDesc->ppMethods[n]->ppParams[i]->dwAttribs = ParamAttrib_out;
    memcpy(&pIntfDesc->ppMethods[n]->ppParams[i]->type, &type, sizeof(type));

    return Ret_Continue;
}

int AddCallbackCoalesceMethod(char* pszMethodName, ClassDirEntry *pClass)
{
    InterfaceDescriptor *pIntfDesc = NULL;
    ClassDescriptor *pClsDesc;
    MethodDescriptor *pMethod;
    pClsDesc = pClass->pDesc;
    int n, x;

    for (n = 0; n < pClsDesc->cInterfaces; n++) {
        if (pClsDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback) {
            x = RetrieveMethod(pszMethodName, s_pModule,
                                                    pClsDesc->ppInterfaces[n]);
            if (x < 0) continue;

            pIntfDesc = s_pModule->ppInterfaceDir[pClsDesc->ppInterfaces[n]->sIndex]->pDesc;
            pMethod = pIntfDesc->ppMethods[x];

            if (0 == pMethod->cParams) {
                ErrorReport(CAR_E_NoParameters, pMethod->pszName);
                return Ret_AbortOnError;
            }

            pMethod->dwAttribs |= MethodAttrib_WithCoalescence;
            return x;
        }
    }

    return Ret_AbortOnError;
}

// CLS_BODY    -> [ k_inherits ident] [ k_aggregates ident [, ident]] [k_substitutes ident]
//                s_lbrace { < CLS_CTOR | CLS_ITF > { s_semicolon } } s_rbrace
//
int P_ClassBody(ClassDirEntry *pClass, BOOL isDeprecated)
{
    CARToken token;
    ClassDescriptor *pDesc;
    int nCtorCount = 0;

    pDesc = pClass->pDesc;
    pDesc->dwAttribs |= ClassAttrib_classlocal;

    token = GetToken(s_pFile);

    if (pDesc->dwAttribs & ClassAttrib_t_normalClass) {
        if (Token_K_inherits == token) {
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
                return Ret_AbortOnError;
            }

            AddClassParent(g_szCurrentToken, pClass);

            token = GetToken(s_pFile);
            if (Token_S_lbrace != token) {
                //should be ','
                if (Token_S_comma != token) {
                    ErrorReport(CAR_E_ExpectSymbol, ",");
                    return Ret_AbortOnError;
                }
                token = GetToken(s_pFile);
            }
        }

        if (Token_K_aggregates == token) {
            //some class can't aggregate aspect;
            if ((pDesc->dwAttribs & ClassAttrib_t_aspect)
                    || (pDesc->dwAttribs & ClassAttrib_t_generic)) {
                ErrorReport(CAR_E_AggregateUse);
                return Ret_AbortOnError;
            }

            TempClassAttribs attr;
            if (P_Aggregate(&attr) == Ret_AbortOnError)
                return Ret_AbortOnError;
            pDesc->dwAttribs |= ClassAttrib_aggregate;

            memcpy(pDesc->pAggrIndexs, attr.aggrIndexs,
                    attr.cAggregates * sizeof(USHORT));
            pDesc->cAggregates = attr.cAggregates;

            token = GetToken(s_pFile);
        }

        if (Token_K_substitutes == token) {
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
                return Ret_AbortOnError;
            }

            AddGenericParent(g_szCurrentToken, pClass);

            token = GetToken(s_pFile);
        }

    }
    else if (pDesc->dwAttribs & ClassAttrib_t_regime) {
        if (Token_K_aggregates == token) {
            //some class can't aggregate aspect;
            if ((pDesc->dwAttribs & ClassAttrib_t_aspect)
                    || (pDesc->dwAttribs & ClassAttrib_t_generic)) {
                ErrorReport(CAR_E_AggregateUse);
                return Ret_AbortOnError;
            }

            TempClassAttribs attr;
            if (P_Aggregate(&attr) == Ret_AbortOnError)
                return Ret_AbortOnError;
            pDesc->dwAttribs |= ClassAttrib_aggregate;

            memcpy(pDesc->pAggrIndexs, attr.aggrIndexs,
                    attr.cAggregates * sizeof(USHORT));
            pDesc->cAggregates = attr.cAggregates;

            token = GetToken(s_pFile);
        }

        if (Token_K_pertainsto == token) {
            TempClassAttribs attr;
            if (P_Aspect(&attr) == Ret_AbortOnError)
                return Ret_AbortOnError;
            pDesc->dwAttribs |= ClassAttrib_aspect;

            memcpy(pDesc->pAspectIndexs, attr.aspectIndexs,
                    attr.cAspects * sizeof(USHORT));
            pDesc->cAspects = attr.cAspects;

            token = GetToken(s_pFile);
        }
    }
    else if (pDesc->dwAttribs & ClassAttrib_t_aspect) {
        if (Token_K_affiliates == token) {
            TempClassAttribs attr;
            if (P_ClassForAspect(&attr) == Ret_AbortOnError)
                return Ret_AbortOnError;
            pDesc->dwAttribs |= ClassAttrib_specialAspect;

            memcpy(pDesc->pClassIndexs, attr.classIndexs,
                    attr.cClasses* sizeof(USHORT));
            pDesc->cClasses = attr.cClasses;

            token = GetToken(s_pFile);
        }
    }

    if (Token_S_lbrace != token) {
        ErrorReport(CAR_E_ExpectSymbol, "{ or ;");
        return Ret_AbortOnError;
    }

    while (PeekToken(s_pFile) != Token_S_rbrace) {
        token = PeekToken(s_pFile);
        if (token == Token_K_constructor) {
            if (pDesc->dwAttribs & ClassAttrib_t_generic) {
                ErrorReport(CAR_E_CouldntHasCtor, "generic");
                return Ret_AbortOnError;
            }

            nCtorCount++;
            if (P_ClassCtorMethod(pClass, isDeprecated) == Ret_AbortOnError) {
                return Ret_AbortOnError;
            }
        }
        else if (token == Token_K_coalesce) {
            GetToken(s_pFile); // peek 'coalesce'
            GetToken(s_pFile); // peek 'callback'
            if (strcmp(g_szCurrentToken, "callback")) {
                ErrorReport(CAR_E_UndefinedSymbol, g_szCurrentToken);
                return Ret_AbortOnError;
            }

            GetToken(s_pFile); // peek method name

            if (AddCallbackCoalesceMethod(g_szCurrentToken, pClass)
                               == Ret_AbortOnError) {
                ErrorReport(CAR_E_InvalidMemberName, g_szCurrentToken);
                return Ret_AbortOnError;
            }
            pDesc->dwAttribs |= ClassAttrib_hascoalescence;
        }
        else
        {
            if (P_ClassInterface(pClass) == Ret_AbortOnError)
                return Ret_AbortOnError;
        }

        while (PeekToken(s_pFile) == Token_S_semicolon) GetToken(s_pFile);
    }
    GetToken(s_pFile); // skip "}"

    if (nCtorCount == 0 && !s_bInKernel) {
        if (AddTrivialClassCtorMethod(pClass) == Ret_AbortOnError)
            return Ret_AbortOnError;
    }

    if (pDesc->dwAttribs & ClassAttrib_hasparent) {
        ClassDirEntry* pParent = s_pModule->ppClassDir[pDesc->sParentIndex];
        if (!(pParent->pDesc->dwAttribs & ClassAttrib_t_generic)) {
            if ((pParent->pDesc->dwAttribs & ClassAttrib_hasctor) &&
                !(pParent->pDesc->dwAttribs & ClassAttrib_hasDefaultCtor)) {
                if (!(pDesc->dwAttribs & ClassAttrib_hasctor) ||
                     (pDesc->dwAttribs & ClassAttrib_hasTrivialCtor)) {
                    ErrorReport(CAR_E_NoConstructor, pParent->pszName, pClass->pszName);
                    return Ret_AbortOnError;
                }
            }
            if (!(pParent->pDesc->dwAttribs & ClassAttrib_hasvirtual)) {
                ErrorReport(CAR_E_NoVirtualInterface, pParent->pszName);
                return Ret_AbortOnError;
            }
        }
    }

    CheckClassAttribs(pDesc);
    if (!(pDesc->dwAttribs & ClassAttrib_defined)) {
        ErrorReport(CAR_E_NoClassInterfaces);
    }
    return Ret_Continue;
}

// CLASS       -> [ CLS_PROPERTIES ] [CLS_ATTRIBS] [k_singleton] [k_final] CLS_ID [ CLS_BODY | s_semicolon ]
// CLS_ID      -> CLS_TYPE ident
// CLS_TYPE    -> k_class | k_category | k_aspect | k_regime | k_domain
//
int P_Class(CARToken token, DWORD properties)
{
    int r;
    //TempClassAttribs attr;
    DWORD dwAttribs = 0;
    ClassDirEntry * pClass;
    ClassDescriptor *pDesc;
    BOOL isDeprecated = FALSE;

    if (properties & Prop_deprecated) isDeprecated = TRUE;
    if (properties & Prop_local) dwAttribs |= ClassAttrib_local;
    if (properties & Prop_private) dwAttribs |= ClassAttrib_private;

    //check properties
    dwAttribs |= s_DefaultThreadModel;

    //[k_singleton]
    if (Token_K_singleton == token) {
        dwAttribs |= ClassAttrib_singleton;

        token = GetToken(s_pFile);
    }

    //[k_final]
    if (Token_K_final == token) {
        dwAttribs |= ClassAttrib_final;

        token = GetToken(s_pFile);
    }

    switch (token) {
        case Token_K_generic:
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_ExpectClassName);
                return Ret_AbortOnError;
            }
            if (g_szCurrentToken[0] != 'G' && !s_bLenientNaming) {
                ErrorReport(CAR_E_IllegalGenericName, g_szCurrentToken);
                return Ret_AbortOnError;
            }
            if (dwAttribs & ClassAttrib_final) {
                ErrorReport(CAR_E_IllegalFinalUsage);
                return Ret_AbortOnError;
            }

            dwAttribs |= ClassAttrib_t_generic;
            break;

        case Token_K_aspect:
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_ExpectClassName);
                return Ret_AbortOnError;
            }
            if (g_szCurrentToken[0] != 'A' && !s_bLenientNaming) {
                ErrorReport(CAR_E_IllegalAspectName, g_szCurrentToken);
                return Ret_AbortOnError;
            }
            if (dwAttribs & ClassAttrib_local) {
                ErrorReport(CAR_E_LocalUse);
                return Ret_AbortOnError;
            }
            if (dwAttribs & ClassAttrib_aggregate) {
                ErrorReport(CAR_E_AggregateUse);
                return Ret_AbortOnError;
            }
            if (dwAttribs & ClassAttrib_final) {
                ErrorReport(CAR_E_IllegalFinalUsage);
                return Ret_AbortOnError;
            }

            dwAttribs |= ClassAttrib_t_aspect;
            break;

        case Token_K_regime:
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_ExpectClassName);
                return Ret_AbortOnError;
            }
            if (g_szCurrentToken[0] != 'R' && !s_bLenientNaming) {
                ErrorReport(CAR_E_IllegalRegimeName, g_szCurrentToken);
                return Ret_AbortOnError;
            }
            if (dwAttribs & ClassAttrib_final) {
                ErrorReport(CAR_E_IllegalFinalUsage);
                return Ret_AbortOnError;
            }

            dwAttribs |= ClassAttrib_t_regime;
            break;

        case Token_K_class:
            if (GetToken(s_pFile) != Token_ident) {
                ErrorReport(CAR_E_ExpectClassName);
                return Ret_AbortOnError;
            }
            if (g_szCurrentToken[0] != 'C' && !s_bLenientNaming) {
                ErrorReport(CAR_E_IllegalClassName, g_szCurrentToken);
                return Ret_AbortOnError;
            }
            dwAttribs |= ClassAttrib_t_normalClass;
            break;
        default:
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
    }

    r = CreateClassDirEntry(g_szCurrentToken, s_pModule, dwAttribs);

    if (Token_S_semicolon == PeekToken(s_pFile)) {
        GetToken(s_pFile); // ignore ";"
        if (CLASS_ATTR(dwAttribs) != 0) {
            ErrorReport(CAR_E_DeclaredClassProp);
            return Ret_ContinueOnError;
        }
        if (r < 0) {
            if (r != CLSError_DupEntry)
                CreateError(r, "class", g_szCurrentToken);
        }
        else {
            s_pModule->ppClassDir[r]-> \
                        pDesc->dwAttribs |= dwAttribs;
        }
        return Ret_Continue;
    }
    if (r < 0) {
        CreateError(r, "class", g_szCurrentToken);
        return Ret_AbortOnError;
    }

    pClass = s_pModule->ppClassDir[r];
    pDesc = pClass->pDesc;
    pDesc->dwAttribs = dwAttribs;

    return P_ClassBody(s_pModule->ppClassDir[r], isDeprecated);
}

// PROPERTY  -> k_scriptable | k_monitor | k_singleton
//                k_sequenced | k_synchronized
//                k_local
int P_InterfaceAndClass(CARToken token)
{
    DWORD props = 0;

    if (Token_S_lbracket == token) {
        //parse properties
        if (Token_S_rbracket == PeekToken(s_pFile)) {
            //warning
            ErrorReport(CAR_W_NoProperties);
        }
        else {
            do {
                token = GetToken(s_pFile);
                switch(token) {
                    case Token_K_deprecated:
                        props |= Prop_deprecated;
                        break;

                    case Token_K_local:
                        props |= Prop_local;
                        break;

                    case Token_K_private:
                        props |= Prop_private;
                        break;

                    default:
                        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
                }

                token = GetToken(s_pFile);
            }while(Token_S_comma == token);

            if (Token_S_rbracket != token) {
                ErrorReport(CAR_E_ExpectSymbol, ", or ]");
            }
        }

        token = GetToken(s_pFile);
    }

    switch(token) {
        case Token_K_interface:
        case Token_K_callbacks:
        case Token_K_delegates:
            return P_Interface(token, props);

        default:
            return P_Class(token, props);
    }
}

// CAR_ELEM    -> INTERFACE | CLASS | ENUM | STRUCT | TYPEDEF |
//                PRAGMA | IMPORT | IMPORTLIB | MERGE
//
int P_CARElement()
{
    CARToken token;

    token = GetToken(s_pFile);
    switch (token) {
        case Token_K_enum:
            return P_Enum();

        case Token_K_struct:
            return P_Struct();

        case Token_K_typedef:
            return P_Typedef();

        case Token_K_const:
            return P_Const();

        case Token_K_pragma:
            return P_Pragma();

        case Token_K_import:
            return P_Import();

        case Token_K_importlib:
            return P_ImportLibrary();

        case Token_K_merge:
            return P_Merge();

        case Token_K_mergelib:
            return P_MergeLibrary();

        default:
            return P_InterfaceAndClass(token);

        case Token_Error:
            return Ret_AbortOnError;
    }
}

// CAR_BODY    -> s_lbrace { CAR_ELEM { s_semicolon } }+ s_rbrace
//
int P_CARBody()
{
    CARToken token;

    // import IInterface first and ignore any errors
    //
    RetrieveInterface("IInterface", s_pModule, TRUE);
    RetrieveInterface("IObject", s_pModule, TRUE);
    RetrieveInterface("IAspect", s_pModule, TRUE);
    RetrieveInterface("IClassObject", s_pModule, TRUE);

    token = GetToken(s_pFile);
    if (Token_S_lbrace == token) {
        do {
            if (P_CARElement() == Ret_AbortOnError)
                return Ret_AbortOnError;
            token = PeekToken(s_pFile);
        } while (Token_S_rbrace != token);

        GetToken(s_pFile); // skip ")"
        return Ret_Continue;
    }
    else {
        ErrorReport(CAR_E_ExpectSymbol, "{");
        return Ret_AbortOnError;
    }
}

int P_CARVersion()
{
    CARToken token;
    int version;

    if (GetToken(s_pFile) != Token_S_lparen) {
        ErrorReport(CAR_E_ExpectSymbol, "(");
        return Ret_AbortOnError;
    }

    token = GetToken(s_pFile);
    if (token != Token_decimal && token != Token_integer) {
        ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        return Ret_AbortOnError;
    }
    if (0 == s_pModule->cMajorVersion && 0 == s_pModule->cMinorVersion) {
        int len = strlen(g_szCurrentToken);
        int dotIdx = -1;
        for (int k = 0; k < len; k++) {
            if (g_szCurrentToken[k] == '.') {
                dotIdx = k;
                break;
            }
        }
        version = atoi(g_szCurrentToken);
        if (version < 0 || version > 255) {
            ErrorReport(CAR_E_IllegalVersion);
            return Ret_AbortOnError;
        }
        if (dotIdx == -1 || dotIdx == len - 1) {
            s_pModule->cMajorVersion = (unsigned char)version;
            s_pModule->cMinorVersion = 0;
        }
        else {
            s_pModule->cMajorVersion = (unsigned char)version;
            char* digits = (g_szCurrentToken + dotIdx + 1);
            s_pModule->cMinorVersion = (unsigned char)atoi(digits);
        }
    }

    if (GetToken(s_pFile) != Token_S_rparen) {
        ErrorReport(CAR_E_ExpectSymbol, ")");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// CAR_ATTRIB  -> [VERSION | k_console | SERVICE | GRAPHICS
//                  | k_project ]
// VERSION     -> k_version s_lparen < integer | decimal > s_rparen
// SERVICE     -> k_service s_lparen string s_rparen
// GRAPHICS    -> k_graphics [ s_lparen k_litegraphics s_rparen ]
//
int P_CARAttrib()
{
    CARToken token;

    token = GetToken(s_pFile);

    switch (token) {
        case Token_K_version:
            if (P_CARVersion() == Ret_AbortOnError) return Ret_AbortOnError;
            break;

        case Token_EOF:
            ErrorReport(CAR_E_UnexpectEOF);
        case Token_Error:
            return Ret_AbortOnError;

        default:
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
            return Ret_AbortOnError;
    }

    return Ret_Continue;
}

// CAR_ATTRIBS -> s_lbracket CAR_ATTRIB { s_comma CAR_ATTRIB } s_rbracket
//
int P_CARAttribs()
{
    CARToken token;

    do {
        if (P_CARAttrib() == Ret_AbortOnError)
            return Ret_AbortOnError;
        token = GetToken(s_pFile);
    } while (token == Token_S_comma);

    if (token != Token_S_rbracket) {
        ErrorReport(CAR_E_ExpectSymbol, "]");
        return Ret_AbortOnError;
    }

    return Ret_Continue;
}

void EnumLastCheck(EnumDirEntry *pEnum)
{
    // Try to retrieve declared but undefined enum from imported libraries.
    //
    if (0 == pEnum->pDesc->cElems) {
        if (RetrieveEnum(pEnum->pszName, s_pModule, TRUE) < 0) {
            ErrorReport(CAR_E_NotFound, "enum", pEnum->pszName);
        }
    }
}

void StructLastCheck(StructDirEntry *pStruct)
{
    // Try to retrieve declared but undefined struct from imported libraries.
    //
    if (0 == pStruct->pDesc->cElems) {
        if (RetrieveStruct(pStruct->pszName, s_pModule, TRUE) < 0) {
            ErrorReport(CAR_E_NotFound, "struct", pStruct->pszName);
        }
    }
}

//
// Generate an IID seed string from interface info.
// The seed string is compose of alphabet, number or '_', we get them
// from name string, the selective order is:
//
//      (interface name) -> (method names) ->
//      (parent name) -> (parent method names) ->
//      (parent's parent name) -> ... and so on,
//
void GenIIDSeedString(InterfaceDirEntry *pInterface, char *pszBuf)
{
    int n, nSize, nTotalSize = 0;
    InterfaceDescriptor *pDesc = pInterface->pDesc;

    while (1) {
        nSize = strlen(pInterface->pszName);
        if (nSize + nTotalSize + 1 > c_nMaxSeedSize) break;
        memcpy(pszBuf, pInterface->pszName, nSize);
        pszBuf += nSize;
        *pszBuf++ = '/';
        nTotalSize += nSize + 1;

        for (n = 0; n < pDesc->cMethods; n++) {
            nSize = strlen(pDesc->ppMethods[n]->pszName);
            if (nSize + nTotalSize > c_nMaxSeedSize) goto ExitEntry;
            memcpy(pszBuf, pDesc->ppMethods[n]->pszName, nSize);
            pszBuf += nSize;
            nTotalSize += nSize;
        }
        if (0 == pDesc->sParentIndex) break;

        pInterface = s_pModule->ppInterfaceDir[pDesc->sParentIndex];
        pDesc = pInterface->pDesc;
    }

ExitEntry:
    *pszBuf = 0;
}

void InterfaceLastCheck(InterfaceDirEntry *pInterface)
{
    char szSeedBuf[c_nMaxSeedSize + 1];

    if (NULL != pInterface->pszNameSpace) return;

    CheckInterfaceDupMethodNameEx(pInterface);

    // Try to retrieve declared but undefined interface from imported
    // libraries.
    //
    if (!(pInterface->pDesc->dwAttribs & InterfaceAttrib_defined)) {
        if (RetrieveInterface(pInterface->pszName, s_pModule, TRUE) < 0) {
            ErrorReport(CAR_E_NotFound, "interface", pInterface->pszName);
            // continue on error
        }
    }

    // Auto generate iid, the seed string of iid is generated base on
    // interface name, methods' name, parent name, parent methods' name, etc.
    //

    if (0 == strcmp(pInterface->pszName, "IInterface")) {
        pInterface->pDesc->iid = EIID_IInterface;
    }
    else if (0 == strcmp(pInterface->pszName, "IObject")) {
        pInterface->pDesc->iid = EIID_IObject;
    }
    else if (0 == strcmp(pInterface->pszName, "IAspect")) {
        pInterface->pDesc->iid = EIID_IAspect;
    }
    else if (0 == strcmp(pInterface->pszName, "IClassObject")) {
        pInterface->pDesc->iid = EIID_IClassObject;
    }
    else {
        GenIIDSeedString(pInterface, szSeedBuf);
        GuidFromSeedString(szSeedBuf, &pInterface->pDesc->iid);
    }
}

void ClassLastCheck(ClassDirEntry *pClass)
{
    int n,m,i;
    ClassDescriptor *pDesc = pClass->pDesc;
    ClassInterface *pClsIntf;
    InterfaceDescriptor *pIntfDesc;
    MethodDescriptor *pMethod;
    char szSeedBuf[c_nMaxSeedSize + 1];

    if (NULL != pClass->pszNameSpace) return;

    //Void class name being duplicated with module name.
    if (strcmp(pClass->pszName, s_pModule->pszName) == 0) {
        ErrorReport(CAR_E_DupNameWithModule);
        return ;
    }

    // Try to retrieve declared but undefined class.
    //
    if (!(pDesc->dwAttribs & ClassAttrib_defined)) {
        if (RetrieveClass(pClass->pszName, s_pModule, TRUE) < 0) {
            ErrorReport(CAR_E_NotFound, "class", pClass->pszName);
            // continue on error
            return;
        }
    }

    // Auto generate clsid base on class name and module name
    //
    if (!(pDesc->dwAttribs & ClassAttrib_t_sink)) {
        strcpy(szSeedBuf, pClass->pszName);
        strcat(szSeedBuf, "/");
        strcat(szSeedBuf, s_pModule->pszName);
        GuidFromSeedString(szSeedBuf, &pDesc->clsid);
    }

    // check if all interfaces of generic have implemented in 'child'.
    //
    if (pDesc->dwAttribs & ClassAttrib_hasparent) {
        ClassDirEntry* pParent = s_pModule->ppClassDir[pDesc->sParentIndex];
        if (pParent->pDesc->dwAttribs & ClassAttrib_t_generic) {
            // the 'parent' is generic
            // the interfaces of generic all must have implemented in 'child'
            for (int i = 0; i < pParent->pDesc->cInterfaces; ++i) {
                bool bFound = FALSE;
                for (int j = 0; j < pDesc->cInterfaces; ++j) {
                    if (pDesc->ppInterfaces[j]->sIndex ==
                            pParent->pDesc->ppInterfaces[i]->sIndex) {
                        bFound = TRUE;
                        break;
                    }
                }
                if (!bFound) {
                    ErrorReport(CAR_E_NoImplIntfOfGeneric,
                        s_pModule->ppInterfaceDir[pParent->pDesc->
                        ppInterfaces[i]->sIndex]->pszName, pClass->pszName);
                    return;
                }
            }
        }
    }

    // Generate sink class if current class has callback interface.
    // Callback interface can't support method with [out] parameter.
    //
    if (pDesc->dwAttribs & ClassAttrib_hascallback) {
        for (n = 0; n < pDesc->cInterfaces; n++) {
            if ((pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback)
                && !(pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_delegate)) {
                pIntfDesc = s_pModule->ppInterfaceDir\
                            [pDesc->ppInterfaces[n]->sIndex]->pDesc;
                for (m = 0; m < pIntfDesc->cMethods; m++) {
                    pMethod = pIntfDesc->ppMethods[m];
                    for (i = 0; i < pMethod->cParams; i++) {
                        if (pMethod->ppParams[i]->dwAttribs & ParamAttrib_out) {
                            ErrorReport(CAR_E_OutParameterWithCallback,
                                        pMethod->pszName);
                            return;
                        }
                    }
                }
            }
        }
    }

    if ((pDesc->dwAttribs & ClassAttrib_classlocal)
        && (pDesc->dwAttribs & ClassAttrib_hasctor)) {
        pDesc->dwAttribs |= ClassAttrib_local;
        s_pModule->ppInterfaceDir[pDesc->sCtorIndex]-> \
            pDesc->dwAttribs |= InterfaceAttrib_local;
    }

    if (pDesc->dwAttribs & ClassAttrib_t_clsobj) {
        if (s_pModule->ppInterfaceDir[pDesc->ppInterfaces[0]->\
            sIndex]->pDesc->dwAttribs & InterfaceAttrib_local) {
            pDesc->dwAttribs |= ClassAttrib_classlocal;
        }
    }

    // If class is not local, assure the first class interface is not local,
    //
    if (!(pDesc->dwAttribs & ClassAttrib_classlocal)) {
        InterfaceDirEntry *pIntf;

        pClsIntf = pDesc->ppInterfaces[0];
        pIntf = s_pModule->ppInterfaceDir[pClsIntf->sIndex];

        if ((pIntf->pDesc->dwAttribs & InterfaceAttrib_local)
            || (pClsIntf->wAttribs & ClassInterfaceAttrib_callback)) {
            for (n = 1; n < pDesc->cInterfaces; n++) {
                if (!(s_pModule->ppInterfaceDir
                    [pDesc->ppInterfaces[n]->sIndex]->pDesc->\
                    dwAttribs & InterfaceAttrib_local)
                    && !(pDesc->ppInterfaces[n]->wAttribs &
                    ClassInterfaceAttrib_callback)) {
                    pDesc->ppInterfaces[0] = pDesc->ppInterfaces[n];
                    pDesc->ppInterfaces[n] = pClsIntf;
                    break;
                }
            }
            assert(n != pDesc->cInterfaces);
        }
    }
}
void CalcStructAlignedSize(const CLSModule *pModule, StructDescriptor *pDesc);
void LastCheck()
{
    int n;
    DWORD dwAttribs = s_pModule->dwAttribs;

    // Auto generate uuid base on uunm or name.
    //
    if (s_pModule->pszUunm) {
        GuidFromSeedString(s_pModule->pszUunm, &s_pModule->uuid);
    }
    else {
        GuidFromSeedString(s_pModule->pszName, &s_pModule->uuid);
    }

    if (0 == s_pModule->cClasses && !(dwAttribs & CARAttrib_library)) {
        ErrorReport(CAR_E_NoClasses);
    }

    for (n = 0; n < s_pModule->cEnums; n++) {
        EnumLastCheck(s_pModule->ppEnumDir[n]);
    }

    for (n = 0; n < s_pModule->cStructs; n++) {
        StructLastCheck(s_pModule->ppStructDir[n]);
        CalcStructAlignedSize(s_pModule, s_pModule->ppStructDir[n]->pDesc);
    }

    for (n = 0; n < s_pModule->cInterfaces; n++) {
        InterfaceLastCheck(s_pModule->ppInterfaceDir[n]);
    }

    for (n = 0; n < s_pModule->cClasses; n++) {
        ClassLastCheck(s_pModule->ppClassDir[n]);
    }

    GenerateEnums(s_pModule);
}

int LoadSystemLibrary(BOOL bNoElastos)
{
    CLSModule *pModule;

    // Load CLS from current module(carc.exe), system types defined here.
    //
    if (LoadCLSFromDll(NULL, &pModule) < 0) {
        ErrorReport(CAR_E_LoadSystemLib);
        return -1;
    }
    AddCLSLibrary(pModule);

    // Load Elastos.Runtime.CarRuntime.eco.
    //
    if (!bNoElastos) {
        if ((LoadCLSFromDll("Elastos.Runtime.CarRuntime.eco", &pModule) == CLS_NoError)
            || (LoadCLS("ElastosCore.cls", &pModule) == CLS_NoError)) {
            AddCLSLibrary(pModule);
        }
        else {
            fprintf(stderr, "[ERROR] carc (0x1005) : %s\n", CLSLastErrorMessage());
            DestroyAllLibraries();
            return -1;
        }
    }

    return 0;
}

int GenerateModuleName(const char *pszModuleName)
{
    s_pModule->pszName = new char[strlen(pszModuleName) + 1];
    if (!s_pModule->pszName) goto OutOfMemoryError;
    strcpy(s_pModule->pszName, pszModuleName);

    if (NULL != s_pModule->pszUunm) return Ret_Continue;
    if (s_pModule->dwAttribs & CARAttrib_library) return Ret_Continue;

        s_pModule->pszUunm = new char
                [strlen(s_pszFactoryUrl) + strlen(pszModuleName) + 6];
    if (!s_pModule->pszUunm) goto OutOfMemoryError;
    if (0 != *s_pszFactoryUrl) {
        strcpy(s_pModule->pszUunm, s_pszFactoryUrl);
        strcat(s_pModule->pszUunm, "/");
    }
    else {
        *s_pModule->pszUunm = 0;
    }

    strcat(s_pModule->pszUunm, pszModuleName);
    strcat(s_pModule->pszUunm, ".eco");
    return Ret_Continue;

OutOfMemoryError:
    ErrorReport(CAR_E_OutOfMemory);
    return Ret_AbortOnError;
}

// CAR         -> [ CAR_ATTRIBS ] CAR_ID CAR_BODY
// CAR_ID      -> < k_module | k_library > [ uunm ]
//
int P_CAR(const char *pszModuleName, BOOL bNested)
{
    CARToken token;
    char szNakedUunm[c_nMaxTokenSize];
    char* pszUunm;
    int nUunmlen;

    token = GetToken(s_pFile);

    // CAR_ATTRIBS
    //
    if (Token_S_lbracket == token) {
        if (P_CARAttribs() == Ret_AbortOnError) {
            return Ret_AbortOnError;
        }
        token = GetToken(s_pFile);
    }

    // CAR_ID
    //
    if (Token_K_library == token) {
        s_pModule->dwAttribs |= CARAttrib_library;
    }
    else if (Token_K_module != token) {
        if (token != Token_Error) {
            ErrorReport(CAR_E_UnexpectSymbol, g_szCurrentToken);
        }
        return Ret_AbortOnError;
    }

    token = PeekToken(s_pFile);
    /*
     * If there are uunm, token will be Token_ident else token will be
     * Token_S_lbrace (grammar correct) or Token_Nothing (grammar error)
     */
    if (Token_S_lbrace != token && Token_Nothing != token) {
        token = GetUunm(s_pFile); // must be uunm in the next!
        if (Token_uunm == token) {
            if (!s_pModule->pszUunm) {
                GetNakedFileName(g_szCurrentToken, szNakedUunm);
                if (strcmp("elastos", szNakedUunm)) {
                    if (strcmp(pszModuleName, szNakedUunm)) {
                        ErrorReport(CAR_E_UunmDifferFromCarName);
                        return Ret_AbortOnError;
                    }
                    else pszUunm = g_szCurrentToken;
                }
                else {
                    //if g_szCurrentToken is "Elastos:System:Elastos.Runtime.CarRuntime.eco",
                    //we delete prefix "Elastos:System:" in order to
                    //make function IsElastosUunm in locmod.cpp work correctly.
                    pszUunm = "Elastos.Runtime.CarRuntime.eco";
                }

                nUunmlen = strlen(pszUunm);
                s_pModule->pszUunm = new char[nUunmlen + 1];
                if (!s_pModule->pszUunm) {
                    ErrorReport(CAR_E_OutOfMemory);
                    return Ret_AbortOnError;
                }
                strcpy(s_pModule->pszUunm, pszUunm);
            }
        }
        else { // Token_Error == token
            return Ret_AbortOnError;
        }
    }
//    else ErrorReport(CAR_W_CARUuidUndef);

    if (!bNested) {
        if (GenerateModuleName(pszModuleName) == Ret_AbortOnError) {
            return Ret_AbortOnError;
        }
    }

    // CAR_BODY
    //
    if (P_CARBody() == Ret_AbortOnError) {
        return Ret_AbortOnError;
    }

    if (!bNested) LastCheck();
    return Ret_Continue;
}

//
// Note: the DoCompiling will be nested invoked if the .car source file
// include merge() or import() statements.
//
int DoCompiling(const char *pszName, CLSModule *pModule)
{
    char *pszPath;
    BOOL bNested = FALSE;
    int n1, n2, n3, n4, nRet;
    char szModuleName[c_nMaxTokenSize];

    n1 = strlen(s_pszCurrentPath);
    n2 = strlen(s_pszSourcePath);
    n3 = strlen(pszName);
    n4 = n1 > n2? n1 + n3: n2 + n3;
    pszPath = new char[n4 + 2];
    if (NULL == pszPath) {
        ErrorReport(CAR_E_OutOfMemory);
        return Ret_AbortOnError;
    }

    if (SearchFileFromPath(s_pszCurrentPath, pszName, pszPath) < 0) {
        if (SearchFileFromPath(s_pszSourcePath, pszName, pszPath) < 0) {
            ErrorReport(CAR_E_OpenFile, pszName);
            return Ret_ContinueOnError;
        }
    }

    if (GetCurrentCarPath(pszPath, &s_pszCurrentPath) == Ret_AbortOnError) {
        delete [] pszPath;
        return Ret_AbortOnError;
    }

    GetNakedFileName(pszName, szModuleName);

    s_pFile = fopen(pszPath, "rt");
    if (!s_pFile) {
        ErrorReport(CAR_E_OpenFile, pszName);
        return Ret_ContinueOnError;
    }
    if (s_pModule == pModule) {
        bNested = TRUE;
    }
    else {
        s_pModule = pModule;
    }

    TokenInit();
    ErrorReporterInit(pszName);

    nRet = P_CAR(szModuleName, bNested);

    if ((s_pszCarFullName) && ('\0' != *s_pszCarFullName))
		delete[] s_pszCarFullName;
    s_pszCarFullName = new char[strlen(pszPath) + 1];
    if (NULL == s_pszCarFullName) {
        ErrorReport(CAR_E_OutOfMemory);
        return Ret_AbortOnError;
    }
    strcpy(s_pszCarFullName, pszPath);
    delete[] pszPath;

    fclose(s_pFile);
    return nRet;
}
