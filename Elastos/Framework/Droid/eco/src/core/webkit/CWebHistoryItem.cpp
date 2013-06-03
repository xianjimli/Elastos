
#include "webkit/CWebHistoryItem.h"

ECode CWebHistoryItem::constructor()
{
    return NOERROR;
}

ECode CWebHistoryItem::constructor(
        /* [in] */ ArrayOf<byte>* data)
{
    WebHistoryItem::Init(data);
    return NOERROR;
}

ECode CWebHistoryItem::constructor(
    /* [in] */ IWebHistoryItem* item)
{
    WebHistoryItem::Init((CWebHistoryItem*)item);
    return NOERROR;
}

ECode CWebHistoryItem::GetId(
    /* [out] */ Int32 * id)
{
    VALIDATE_NOT_NULL(id);
    *id = WebHistoryItem::GetId();
    return NOERROR;
}

ECode CWebHistoryItem::GetUrl(
    /* [out] */ String * url)
{
    VALIDATE_NOT_NULL(url);
    *url = WebHistoryItem::GetUrl();
    return NOERROR;
}

ECode CWebHistoryItem::GetOriginalUrl(
    /* [out] */ String * originalUrl)
{
    VALIDATE_NOT_NULL(originalUrl);
    *originalUrl = WebHistoryItem::GetOriginalUrl();
    return NOERROR;
}

ECode CWebHistoryItem::GetTitle(
    /* [out] */ String * title)
{
    VALIDATE_NOT_NULL(title);
    *title = WebHistoryItem::GetTitle();
    return NOERROR;
}

ECode CWebHistoryItem::GetFavicon(
    /* [out] */ IBitmap** favicon)
{    
    VALIDATE_NOT_NULL(favicon);
    *favicon = WebHistoryItem::GetFavicon();
    return NOERROR;
}

ECode CWebHistoryItem::GetTouchIconUrl(
    /* [out] */ String * touchIconUrl)
{
    VALIDATE_NOT_NULL(touchIconUrl);
    *touchIconUrl = WebHistoryItem::GetTouchIconUrl();
    return NOERROR;
}

ECode CWebHistoryItem::GetCustomData(
    /* [out] */ IInterface ** customData)
{
    VALIDATE_NOT_NULL(customData);
    *customData = WebHistoryItem::GetCustomData();
    return NOERROR;
}

ECode CWebHistoryItem::SetCustomData(
    /* [in] */ IInterface * data)
{
    WebHistoryItem::SetCustomData(data);
    return NOERROR;
}

ECode CWebHistoryItem::Clone(
    /* [out] */ IWebHistoryItem** item)
{
    VALIDATE_NOT_NULL(item);
    Mutex::Autolock lock(_m_syncLock);

    CWebHistoryItem::New(item);    
    String url;
    String title;
    ArrayOf<byte>* data;
    AutoPtr<IBitmap> favicon;
    Int32 id;    
    GetUrl(&url);
    GetTitle(&title);
    data=WebHistoryItem::GetFlattenedData();
    GetFavicon((IBitmap**)&favicon);
    GetId(&id);
    ((CWebHistoryItem*)(*item))->Update(url,String(""),title,favicon,data);
    ((CWebHistoryItem*)(*item))->SetId(id);

    return NOERROR;
}