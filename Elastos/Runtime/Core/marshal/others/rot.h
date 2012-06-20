
#ifndef __ROT_H__
#define __ROT_H__

#include "prxstub.h"
#include "syslinknode.h"

_ELASTOS_NAMESPACE_USING

typedef struct ExportObject: SLinkNode 
{
    String          m_connName;
    IInterface      *m_pObject;
    IStub           *m_pIStub;
} ExportObject;

typedef struct ImportObject: SLinkNode 
{
    String          m_stubConnName;
    IProxy          *m_pIProxy;
} ImportObject;

extern ECode InitROT();

extern void UninitROT();

extern ECode RegisterExportObject(
        /* [in] */ String connName,
        /* [in] */ IInterface *pObject,
        /* [in] */ IStub *pIStub);

extern ECode UnregisterExportObject(
        /* [in] */ String connName);

extern ECode FindExportObject(
        /* [in] */ IInterface *pObject,
        /* [out] */ ExportObject *pExport);

extern ECode FindExportObject(
        /* [in] */ String connName,
        /* [out] */ ExportObject *pExport);

extern ECode RegisterImportObject(
        /* [in] */ String stubConnName,
        /* [in] */ IProxy *pIProxy);

extern ECode UnregisterImportObject(
        /* [in] */ String stubConnName);

extern ECode FindImportObject(
        /* [in] */ String stubConnName,
        /* [out] */ ImportObject *pImport);

#endif //__ROT_H__
