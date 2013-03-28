#include "webkit/WebHistoryItem.h"

Int32 WebHistoryItem::sNextId = 0;

WebHistoryItem::WebHistoryItem()
{
	if(TRUE) {//JAVA:   synchronized (WebHistoryItem.class)
        Elastos::Core::Threading::Mutex::Autolock lock(sLock);
        mId = sNextId++;
    }
}

WebHistoryItem::WebHistoryItem(
    /* [in] */ ArrayOf<Byte>* data)
{
	mUrl = NULL; // This will be updated natively
    mFlattenedData = data;
    if(TRUE){//JAVA:   synchronized (WebHistoryItem.class)
        Elastos::Core::Threading::Mutex::Autolock lock(sLock);
        mId = sNextId++;
    }
}

WebHistoryItem::WebHistoryItem(
    /* [in] */ WebHistoryItem* item)
{
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

void WebHistoryItem::SetCustomData(
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

WebHistoryItem* WebHistoryItem::Clone()
{
    Elastos::Core::Threading::Mutex::Autolock lock(mLock);
    return new WebHistoryItem(this);
}

void WebHistoryItem::Inflate(
        /* [in] */ Int32 nativeFrame, 
        /* [in] */ ArrayOf<Byte>* data)
{//=0(virtual)
    return;    
}

void WebHistoryItem::Update(
    /* [in] */ CString url, 
    /* [in] */ CString originalUrl, 
    /* [in] */ String title,
    /* [in] */ IBitmap* favicon,
    /* [in] */ ArrayOf<Byte>* data)
{
    mUrl = url;
    mOriginalUrl = originalUrl;
    mTitle = title;
    mFavicon = favicon;
    mFlattenedData = data;
}