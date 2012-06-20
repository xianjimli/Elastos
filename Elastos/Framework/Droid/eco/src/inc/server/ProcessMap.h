
#ifndef __PROCESSMAP_H__
#define __PROCESSMAP_H__

#include "ext/frameworkext.h"
#include <elastos/HashMap.h>
#include <elastos/Map.h>

template <typename E>
class ProcessMap
{
public:
    ProcessMap();

    ~ProcessMap();

    CARAPI_(E) Get(
        /* [in] */ String name,
        /* [in] */ Int32 uid);

    CARAPI_(E) Put(
        /* [in] */ String name,
        /* [in] */ Int32 uid,
        /* [in] */ E value);

    CARAPI_(void) Remove(
        /* [in] */ String name,
        /* [in] */ Int32 uid);

    HashMap<String, Map<Int32, E>*>* GetMap();

public:
    HashMap<String, Map<Int32, E>*> mMap;
};

template <typename E>
ProcessMap<E>::ProcessMap() : mMap(11)
{}

template <typename E>
ProcessMap<E>::~ProcessMap()
{
    mMap.Clear();
}

template <typename E>
E ProcessMap<E>::Get(
    /* [in] */ String name,
    /* [in] */ Int32 uid)
{
    Map<Int32, E>* uids = mMap[name];
    if (uids == NULL) return NULL;
    typename Map<Int32, E>::Iterator it = uids->Find(uid);
    if (it != uids->End()) return it->mSecond;
    else return NULL;
}

template <typename E>
E ProcessMap<E>::Put(
    /* [in] */ String name,
    /* [in] */ Int32 uid,
    /* [in] */ E value)
{
    Map<Int32, E>* uids = mMap[name];
    if (uids == NULL) {
        uids = new Map<Int32, E>;
        mMap[String::Duplicate(name)] = uids;
    }
    (*uids)[uid] = value;
    return value;
}

template <typename E>
void ProcessMap<E>::Remove(
    /* [in] */ String name,
    /* [in] */ Int32 uid)
{
    Map<Int32, E>* uids = mMap[name];
    if (uids != NULL) {
        typename Map<Int32, E>::Iterator it = uids->Find(uid);
        if (it != uids->End()) uids->Erase(it);
        if (uids->GetSize() == 0) {
            mMap.Erase(name);
        }
    }
}

template <typename E>
HashMap<String, Map<Int32, E>*>* ProcessMap<E>::GetMap()
{
    return &mMap;
}

#endif //__PROCESSMAP_H__
