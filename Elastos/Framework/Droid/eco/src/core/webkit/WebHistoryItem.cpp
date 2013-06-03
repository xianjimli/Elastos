#include "webkit/WebHistoryItem.h"

Int32 WebHistoryItem::sNextId = 0;
Elastos::Core::Threading::Mutex WebHistoryItem::mMutexClass;

WebHistoryItem::WebHistoryItem()
{
	if(TRUE) {//JAVA:   synchronized (WebHistoryItem.class)
        Elastos::Core::Threading::Mutex::Autolock lock(mMutexClass);
        mId = sNextId++;
    }
}

WebHistoryItem::WebHistoryItem(
    /* [in] */ ArrayOf<Byte>* data)
{
	Init(data);
    if(TRUE){//JAVA:   synchronized (WebHistoryItem.class)
        Elastos::Core::Threading::Mutex::Autolock lock(mMutexClass);
        mId = sNextId++;
    }
}

void WebHistoryItem::Init(
    /* [in] */ ArrayOf<Byte>* data)
{    
    mUrl = NULL; // This will be updated natively
    mFlattenedData = data;
}

WebHistoryItem::WebHistoryItem(
    /* [in] */ WebHistoryItem* item)
{
    if(TRUE){
        Elastos::Core::Threading::Mutex::Autolock lock(mMutexClass);
        sNextId++;
    }
    Init(item);
}

void WebHistoryItem::Init(
    /* [in] */ WebHistoryItem* item)
{
    if(TRUE){
        Elastos::Core::Threading::Mutex::Autolock lock(mMutexClass);
        sNextId--;
    }
    if(item != NULL){
        mUrl = item -> GetUrl();
        mTitle = item -> GetTitle();
        mFlattenedData = item -> GetFlattenedData();
        mFavicon = item -> GetFavicon();
        mId = item -> GetId();
    }
}

Int32 WebHistoryItem::GetId()
{
    return mId;
}

String WebHistoryItem::GetUrl()
{
    return mUrl;
}

String WebHistoryItem::GetOriginalUrl()
{
    return mOriginalUrl;
}

String WebHistoryItem::GetTitle()
{
    return mTitle;
}

IBitmap* WebHistoryItem::GetFavicon()
{
    return mFavicon.Get();
}

String WebHistoryItem::GetTouchIconUrl()
{
    return mTouchIconUrl;
}

IInterface* WebHistoryItem::GetCustomData()
{
    return mCustomData;
}

ECode WebHistoryItem::SetCustomData(
    /* [in] */ IInterface* data)
{
    mCustomData = data;
}

void WebHistoryItem::SetFavicon(
    /* [in] */ IBitmap* icon)
{
    mFavicon = icon;
}

void WebHistoryItem::SetTouchIconUrl(
    /* [in] */ CString url)
{
    mTouchIconUrl = url;
}

ArrayOf<Byte>* WebHistoryItem::GetFlattenedData()
{
    return mFlattenedData.Get();
}

void WebHistoryItem::Inflate(
    /* [in] */ Int32 nativeFrame)
{
    Inflate(nativeFrame, mFlattenedData.Get());
}

// WebHistoryItem* WebHistoryItem::Clone()
// {
//     Elastos::Core::Threading::Mutex::Autolock lock(mMutex);
//     return new WebHistoryItem(this);
// }

void WebHistoryItem::Inflate(
        /* [in] */ Int32 nativeFrame, 
        /* [in] */ ArrayOf<Byte>* data)
{//=0(virtual)
    return;    
}

void WebHistoryItem::Update(
    /* [in] */ CString url, 
    /* [in] */ CString originalUrl, 
    /* [in] */ const String& title,
    /* [in] */ IBitmap* favicon,
    /* [in] */ ArrayOf<Byte>* data)
{
    mUrl = url;
    mOriginalUrl = originalUrl;
    mTitle = title;
    mFavicon = favicon;
    mFlattenedData = data;
}

void WebHistoryItem::SetId(
    /* [in] */ Int32 id)
{
    mId = id;
}