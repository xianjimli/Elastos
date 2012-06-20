
#include <pthread.h>
#include "rot.h"

pthread_mutex_t g_importTableLock;
pthread_mutex_t g_exportTableLock;
pthread_mutex_t g_marshalLock;

#define ROT_HASHTABLE_SIZE 32

static ImportObject s_hashImportObjects[ROT_HASHTABLE_SIZE];
static ExportObject s_hashExportObjects[ROT_HASHTABLE_SIZE];

static UInt32 Hash(String key)
{
    UInt32 val = 0;
    UInt32 pos = 0;
    UInt32 len = (UInt32)key.GetLength();
    
    while(pos < len) {
        val += ((const char*)key)[pos] * pos;
        pos++; 
    }
    
    return val % ROT_HASHTABLE_SIZE;
}

ECode InitROT()
{
    ECode ec = NOERROR;

    for (int n = 0; n < ROT_HASHTABLE_SIZE; n++) {
        s_hashImportObjects[n].Initialize();
        s_hashImportObjects[n].m_pNext = NULL;
        s_hashExportObjects[n].Initialize();
        s_hashExportObjects[n].m_pNext = NULL;
    }
    
    pthread_mutexattr_t recursiveAttr;
    pthread_mutexattr_init(&recursiveAttr);
    pthread_mutexattr_settype(&recursiveAttr, PTHREAD_MUTEX_RECURSIVE);
    if (FAILED(pthread_mutex_init(&g_importTableLock, &recursiveAttr))) {
        ec = E_FAIL;
    }
    else if (FAILED(pthread_mutex_init(&g_exportTableLock, &recursiveAttr))) {
        ec = E_FAIL;
    }
    else if (FAILED(pthread_mutex_init(&g_marshalLock, &recursiveAttr))) {
        ec = E_FAIL;
    }       
    pthread_mutexattr_destroy(&recursiveAttr);

    return ec;
}

void UninitROT()
{
    pthread_mutex_destroy(&g_importTableLock);
    pthread_mutex_destroy(&g_exportTableLock);
    pthread_mutex_destroy(&g_marshalLock);
}

ECode RegisterExportObject(
    /* [in] */ String connName,
    /* [in] */ IInterface *pObject,
    /* [in] */ IStub *pIStub)
{
    ExportObject* pObjNode;

    pObject = pObject->Probe(EIID_IInterface);
    if (!pObject) return E_NO_INTERFACE;

    pObjNode = new ExportObject;
    if (NULL == pObjNode) return E_OUT_OF_MEMORY;
    pObjNode->m_connName = String::Duplicate(connName);
    pObjNode->m_pObject = pObject;
    pObjNode->m_pIStub = pIStub;

    pthread_mutex_lock(&g_exportTableLock);
    s_hashExportObjects[Hash(connName)].InsertFirst(pObjNode);
    pthread_mutex_unlock(&g_exportTableLock);
    return NOERROR;
}

ECode FindExportObject(
    /* [in] */ IInterface *pObject,
    /* [out] */ ExportObject *pExport)
{
    pObject = pObject->Probe(EIID_IInterface);
    if (!pObject) return E_NO_INTERFACE;

    pthread_mutex_lock(&g_exportTableLock);
    ExportObject* pIter;
    for (int n = 0; n < ROT_HASHTABLE_SIZE; n++) {
        pIter = s_hashExportObjects + n;
        for (; NULL != pIter; pIter = (ExportObject*)(pIter->Next())) {
            if (pIter->m_pObject == pObject) {
                memcpy(pExport, pIter, sizeof(ExportObject));
                pExport->m_pIStub->AddRef();
                pthread_mutex_unlock(&g_exportTableLock);
                return NOERROR;
            }
        }
    }

    pthread_mutex_unlock(&g_exportTableLock);
    return E_NO_EXPORT_SERVER;
}

ECode FindExportObject(
    /* [in] */ String connName,
    /* [out] */ ExportObject *pExport)
{
    pthread_mutex_lock(&g_exportTableLock);
    ExportObject* pHead = s_hashExportObjects + Hash(connName);

    for (ExportObject* pIter = pHead; NULL != pIter; \
        pIter = (ExportObject*)(pIter->Next())) {
        if (!pIter->m_connName.Compare(connName)) {
            memcpy(pExport, pIter, sizeof(ExportObject));
            pExport->m_pIStub->AddRef();
            pthread_mutex_unlock(&g_exportTableLock);
            return NOERROR;
        }
    }

    pthread_mutex_unlock(&g_exportTableLock);
    return E_NO_EXPORT_SERVER;
}

ECode UnregisterExportObject(
    /* [in] */ String connName)
{
    pthread_mutex_lock(&g_exportTableLock);
    ExportObject* pHead = s_hashExportObjects + Hash(connName);
    ExportObject* pPrev = NULL;

    for (ExportObject* pIter = pHead; NULL != pIter; \
        pIter = (ExportObject*)(pIter->Next())) {
        if (!pIter->m_connName.Compare(connName)) {
            if ((Int32)(((CObjectStub*)(pIter->m_pIStub))->m_cRef) != 0) {
                pthread_mutex_unlock(&g_exportTableLock);
                return S_FALSE;
            }

            if (pIter != pHead) {
                pIter->Detach(pPrev);
                delete pIter;
            }
            pthread_mutex_unlock(&g_exportTableLock);
            return NOERROR;
        }
        pPrev = pIter;
    }

    pthread_mutex_unlock(&g_exportTableLock);
    return E_NO_EXPORT_SERVER;
}

ECode RegisterImportObject(
    /* [in] */ String stubConnName,
    /* [in] */ IProxy *pIProxy)
{
    ImportObject * pObjNode;

    pObjNode = new ImportObject;
    if (NULL == pObjNode) {
        return E_OUT_OF_MEMORY;
    }
    pObjNode->m_stubConnName = String::Duplicate(stubConnName);
    pObjNode->m_pIProxy = pIProxy;
    pthread_mutex_lock(&g_importTableLock);
    s_hashImportObjects[Hash(stubConnName)].InsertFirst(pObjNode);
    pthread_mutex_unlock(&g_importTableLock);

    return NOERROR;
}

ECode FindImportObject(
    /* [in] */ String stubConnName,
    /* [out] */ ImportObject *pImport)
{
    pthread_mutex_lock(&g_importTableLock);
    ImportObject* pHead = s_hashImportObjects + Hash(stubConnName);

    for (ImportObject* pIter = pHead; NULL != pIter; \
        pIter = (ImportObject*)(pIter->Next())) {
        if (!pIter->m_stubConnName.Compare(stubConnName)) {
            memcpy(pImport, pIter, sizeof(ImportObject));
            pImport->m_pIProxy->AddRef();
            pthread_mutex_unlock(&g_importTableLock);
            return NOERROR;
        }
    }

    pthread_mutex_unlock(&g_importTableLock);
    return E_NO_IMPORT_SERVER;
}

// BUGBUG: must atomic operation with delete proxy
ECode UnregisterImportObject(
    /* [in] */ String stubConnName)
{
    pthread_mutex_lock(&g_importTableLock);
    ImportObject* pHead = s_hashImportObjects + Hash(stubConnName);
    ImportObject* pPrev = NULL;

    for (ImportObject* pIter = pHead; NULL != pIter; \
        pIter = (ImportObject*)(pIter->Next())) {
        if (!pIter->m_stubConnName.Compare(stubConnName)) {
            if ((Int32)(((CObjectProxy *)(pIter->m_pIProxy))->m_cRef) != 0) {
                pthread_mutex_unlock(&g_importTableLock);
                return S_FALSE;
            }

            if (pIter != pHead) {
                pIter->Detach(pPrev);
                delete pIter;
            }

            pthread_mutex_unlock(&g_importTableLock);
            return NOERROR;
        }
        pPrev = pIter;
    }

    pthread_mutex_unlock(&g_importTableLock);
    return E_NO_IMPORT_SERVER;
}
