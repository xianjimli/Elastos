
#ifndef __MARSHAL_H__
#define __MARSHAL_H__

#include <elastos.h>

using namespace Elastos;

typedef struct MarshalHeader 
{
    UInt32              m_uMagic;           // magic number
    oid_t               m_oid;              // server oid_t
    Int16               m_hInterfaceIndex;  // interface index in object
    Int16               m_hMethodIndex;     // method index in interface
    PContext            m_pContext;         // context hint
    UInt32              m_uInSize;          // size of in data
    UInt32              m_uOutSize;         // size of out data
}   MarshalHeader;

typedef struct InterfacePack
{
    EMuid               m_clsid;    // clsid of object
    UInt32              m_uIndex;   // interface index in class
#ifdef _MSVC
    Char8               m_stubConnName[255];
#else
    android::sp<android::IBinder> m_pBinder; // dbus unique connection name of stub
#endif
}   InterfacePack;

#define MARSHAL_MAGIC   0x7a79636f    

#define MSH_MAX_METHODS (128 + 3)

#define MSH_ALIGN_4(n)  RoundUp2((n), 4)

#define MSH_NULL        ((UInt32)0)
#define MSH_NOT_NULL    ((UInt32)-2)

#define GET_LENGTH(a) ((a) & 0x3f)

#define BY_REFERENCE(base, offset)  ((UInt32*)base + offset)
#define BY_VALUE(base, offset) \
        (*(UInt32*)BY_REFERENCE(base,offset))  

#endif //__MARSHAL_H__
