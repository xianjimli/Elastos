
#include "webkit/CWebHistoryItem.h"

Int32 CWebHistoryItem::sNextId = 0;
Core::Threading::Mutex CWebHistoryItem::sLock;

ECode CWebHistoryItem::constructor()
{
    if(TRUE) {//JAVA:   synchronized (WebHistoryItem.class)
        Mutex::Autolock lock(sLock);
        mId = sNextId++;
    }
    return NOERROR;
}

ECode CWebHistoryItem::constructor(
        /* [in] */ ArrayOf<byte>* data)
{
    mUrl = NULL; // This will be updated natively
    mFlattenedData = data;
    if(TRUE){//JAVA:   synchronized (WebHistoryItem.class)
        Mutex::Autolock lock(sLock);
        mId = sNextId++;
    }
    return NOERROR;
}

//ECode CWebHistoryItem::constructor(IWebHistoryItem* item)
ECode CWebHistoryItem::constructor(
    /* [in] */ IWebHistoryItem* item)
{
    if(item != NULL){
        CWebHistoryItem* itemC = (CWebHistoryItem*)item;
        itemC->GetUrl(&mUrl);
        itemC->GetTitle(&mTitle);
        itemC->GetFlattenedData((ArrayOf<byte>**)&mFlattenedData);
        itemC->GetFavicon((IBitmap**)&mFavicon);
        itemC->GetId(&mId);
    }
    return NOERROR;
}

ECode CWebHistoryItem::GetId(
    /* [out] */ Int32 * id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CWebHistoryItem::GetUrl(
    /* [out] */ String * url)
{
    VALIDATE_NOT_NULL(url);
    *url = mUrl;
    return NOERROR;
}

ECode CWebHistoryItem::GetOriginalUrl(
    /* [out] */ String * originalUrl)
{
    VALIDATE_NOT_NULL(originalUrl);
    *originalUrl = mOriginalUrl;
    return NOERROR;
}

ECode CWebHistoryItem::GetTitle(
    /* [out] */ String * title)
{
    VALIDATE_NOT_NULL(title);
    *title = mTitle;
    return NOERROR;
}

ECode CWebHistoryItem::GetFavicon(
    /* [out] */ IBitmap** favicon)
{    
    VALIDATE_NOT_NULL(favicon);
    *favicon = mFavicon.Get();
    return NOERROR;
}

ECode CWebHistoryItem::GetTouchIconUrl(
    /* [out] */ String * touchIconUrl)
{
    VALIDATE_NOT_NULL(touchIconUrl);
    *touchIconUrl = mTouchIconUrl;
    return NOERROR;
}

ECode CWebHistoryItem::GetCustomData(
    /* [out] */ IInterface ** customData)
{
    VALIDATE_NOT_NULL(customData);
    *customData = mCustomData;
    return NOERROR;
}

ECode CWebHistoryItem::SetCustomData(
    /* [in] */ IInterface * data)
{
    mCustomData = data;
    return NOERROR;
}

ECode CWebHistoryItem::SetFavicon(
    /* [in] */ IBitmap* icon)
{
    mFavicon = icon;
    return NOERROR;
}

ECode CWebHistoryItem::SetTouchIconUrl(
        /* [in] */ String url)
{
    mTouchIconUrl = url;
    return NOERROR;
}

ECode CWebHistoryItem::GetFlattenedData(
    /* [out] */ ArrayOf<byte>** data)
{
    VALIDATE_NOT_NULL(data);
    *data = mFlattenedData.Get();
    return NOERROR;
}

ECode CWebHistoryItem::Inflate(
    /* [in] */ Int32 nativeFrame)
{
    Inflate(nativeFrame, mFlattenedData.Get());
    return NOERROR;
}

ECode CWebHistoryItem::Clone(
    /* [out] */ IWebHistoryItem** item)
{
    Mutex::Autolock lock(_m_syncLock);

    VALIDATE_NOT_NULL(item);       

    CWebHistoryItem::New(item);    
    String url;
    String title;
    ArrayOf<byte>* data;
    AutoPtr<IBitmap> favicon;
    Int32 id;    
    GetUrl(&url);
    GetTitle(&title);
    GetFlattenedData(&data);
    GetFavicon((IBitmap**)&favicon);
    GetId(&id);
    ((CWebHistoryItem*)(*item))->Update(url,String(""),title,favicon,data);
    ((CWebHistoryItem*)(*item))->SetId(id);

    return NOERROR;
}

ECode CWebHistoryItem::Inflate(
    /* [in] */ Int32 nativeFrame, 
    /* [in] */ ArrayOf<byte>* data)
{//=0(virtual)
    return NOERROR;
}

ECode CWebHistoryItem::Update(
    /* [in] */ String url, 
    /* [in] */ String originalUrl, 
    /* [in] */ String title,
    /* [in] */ IBitmap* favicon,
    /* [in] */ ArrayOf<byte>* data)
{
    mUrl = url;
    mOriginalUrl = originalUrl;
    mTitle = title;
    mFavicon = favicon;
    mFlattenedData = data;
    return NOERROR;
}

ECode CWebHistoryItem::SetId(
    /* [in] */ Int32 id)
{
    mId = id;
    return NOERROR;
}