
#include "app/CContentProviderHolder.h"

ECode CContentProviderHolder::GetContentProvider(
    /* [out] */ IContentProvider** provider)
{
    if (provider == NULL) return E_INVALID_ARGUMENT;
    
    *provider = mProvider;
    if (*provider) (*provider)->AddRef();
    return NOERROR;
}
        
ECode CContentProviderHolder::GetContentProviderInfo(
    /* [out] */ IContentProviderInfo** info)
{
    if (info == NULL) return E_INVALID_ARGUMENT;
    
    *info = (IContentProviderInfo*)(CContentProviderInfo*)mInfo;
    if (*info) (*info)->AddRef();
    return NOERROR;
}

ECode CContentProviderHolder::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    AutoPtr<IInterface> info, provider;
    source->ReadInterfacePtrPtr((Handle32*)&info);
    mInfo = (info != NULL)? (CContentProviderInfo*)(IContentProviderInfo*)
            info->Probe(EIID_IContentProviderInfo) : NULL;
    source->ReadInterfacePtrPtr((Handle32*)&provider);
    mProvider = (provider != NULL) ? (IContentProvider*)
            provider->Probe(EIID_IContentProvider) : NULL;
    return NOERROR;
}

ECode CContentProviderHolder::WriteToParcel(
    /* [in] */ IParcel *dest)
{  
    dest->WriteInterfacePtr((IInterface*)(IContentProviderInfo*)mInfo);
    dest->WriteInterfacePtr((IInterface*)(IContentProvider*)mProvider);
    return NOERROR;
}
