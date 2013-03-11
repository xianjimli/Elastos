
#include "webkit/CURLUtil.h"

#include <Logger.h>
//#include "net/CWebAddress.h"
#include <StringBuffer.h>
#include <Uri.h>
#include "webkit/CMimeTypeMap.h"

// to refer to bar.png under your package's asset/foo/ directory, use
// "file:///android_asset/foo/bar.png".
const char* CURLUtil::ASSET_BASE = "file:///android_asset/";
// to refer to bar.png under your package's res/drawable/ directory, use
// "file:///android_res/drawable/bar.png". Use "drawable" to refer to
// "drawable-hdpi" directory as well.
const char* CURLUtil::RESOURCE_BASE = "file:///android_res/";
const char* CURLUtil::FILE_BASE = "file://";
const char* CURLUtil::PROXY_BASE = "file:///cookieless_proxy/";

const char* CURLUtil::LOGTAG = "webkit";

//const AutoPtr<CPattern> CURLUtil::CONTENT_DISPOSITION_PATTERN = CPattern::Compile("attachment;\\s*filename\\s*=\\s*(\"?)([^\"]*)\\1\\s*$",CPattern::CASE_INSENSITIVE);

#if 0

ECode CURLUtil::ParseHex(
        /* [in] */ Byte b,
        /* [out] */ Int32* retVal)
{
    if (b >= '0' && b <= '9')
    {
        *retVal = (b - '0');
        return NOERROR;
    }
    if (b >= 'A' && b <= 'F') 
    {
        *retVal = ((b - 'A') + 10);
        return NOERROR;
    }        
    if (b >= 'a' && b <= 'f')
    {
        *retVal = ((b - 'a') + 10);
        return NOERROR;
    }

    Utility::Logging::Logger::E(LOGTAG, String("Invalid hex char '") + b + String("'") );
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CURLUtil::GuessUrl(
    /* [in] */ const String& inUrl,
    /* [out] */ String * pOutUrl)
{    
    String retVal = inUrl;
    //IWebAddress * webAddress;

    Utility::Logging::Logger::V(LOGTAG, String("guessURL before queueRequest: ") + inUrl );

    if(inUrl.GetLength() == 0)
    {
        *pOutUrl = inUrl;
        return NOERROR;
    }
    if(inUrl.Substring(0,6) == String("about:"))
    {
        *pOutUrl = inUrl;
        return NOERROR;
    }
    // Do not try to interpret data scheme URLs
    if (inUrl.Substring(0,5) == String("data:"))
    {
        *pOutUrl = inUrl;
        return NOERROR;
    }
    // Do not try to interpret file scheme URLs
    if (inUrl.Substring(0,5) == String("file:"))
    {
        *pOutUrl = inUrl;
        return NOERROR;
    }
    // Do not try to interpret javascript scheme URLs
    if (inUrl.Substring(0,11) == String("javascript:"))
    {
        *pOutUrl = inUrl;
        return NOERROR;
    }

    // bug 762454: strip period off end of url
    String tInUrl;
    if (inUrl.Substring(inUrl.GetLength()-1,inUrl.GetLength()) == String(".") ) 
    {
        tInUrl = inUrl.Substring(0, inUrl.GetLength() - 1);
    }

    /*
    ECode ec = CWebAddress::New(tInUrl,&webAddress);
    if(FAILED(ec))
    {        
        if (DebugFlags::URL_UTIL) 
        {
            Utility::Logging::Logger::V(LOGTAG, String("smartUrlFilter: failed to parse url = ") + tInUrl );
        }        
        *pOutUrl = retVal;
        return ec;
    }
    

    // Check host
    if ((webAddress -> mHost).Find(".") == -1) 
    {
        // no dot: user probably entered a bare domain.  try .com
        webAddress -> mHost = String("www.") + (webAddress -> mHost) + String(".com");
    }
    webAddress -> ToString(pOutUrl);
    */

    return NOERROR;
}

ECode CURLUtil::ComposeSearchUrl(
    /* [in] */ const String& inQuery,
    /* [in] */ const String& strTemplate,
    /* [in] */ const String& queryPlaceHolder,
    /* [out] */ String * pUrl)
{
    Int32 placeHolderIndex = strTemplate.IndexOf(queryPlaceHolder);
    if (placeHolderIndex < 0) 
    {
        *pUrl = String(NULL);
        return NOERROR;
    }

    String query;
    StringBuffer buffer(NULL);
    buffer = strTemplate.Substring(0, placeHolderIndex);

    /*
    ECode ec = Elastos::URLEncoder::Encode(inQuery, "utf-8", &query);
    if(FAILED(ec))
    {
        //JAVA:catch (UnsupportedEncodingException ex)        
        *pUrl = String(NULL);
        return ec;
    }
    */
    
    buffer += query;
    buffer += strTemplate.Substring(placeHolderIndex + queryPlaceHolder.GetLength());

    *pUrl = (const char*)buffer;

    return NOERROR;
}

ECode CURLUtil::Decode(
    /* [in] */ const ArrayOf<Byte> & url,
    /* [out] */ ArrayOf<Byte> * pOutUrl)
{
    if(url.GetLength() == 0)
    {
        pOutUrl = NULL;
        return NOERROR;
    }

    // Create a new byte array with the same length to ensure capacity
    ArrayOf<Byte> * tempData = ArrayOf<Byte>::Alloc(url.GetLength());

    Int32 tempCount = 0;
    Int32 tHexH = 0,tHexL = 0;
    ECode ecH,ecL;
    for(Int32 i = 0; i < url.GetLength(); i++)
    {
        Byte b = url[i];
        if(b == '%')
        {
            if(url.GetLength() - i > 2)
            {
                ecH = ParseHex(url[i+1],&tHexH);
                if( FAILED(ecH))
                {
                    return ecH;
                }
                ecL = ParseHex(url[i+2],&tHexL);                
                if( FAILED(ecL))
                {
                    return ecL;
                }
                b = (byte) (ecH * 16 + ecL);
                i += 2;
            }
            else
            {
                ArrayOf<Byte>::Free(tempData);
                //JAVA:throw new IllegalArgumentException("Invalid format");
                Utility::Logging::Logger::E(LOGTAG, String("Invalid format") );
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }            
        }
        (*tempData)[tempCount++] = b;
    }
    for(Int32 i = 0; i < tempCount; i++)
    {
        (*pOutUrl)[i] = (*tempData)[i];
    }
    ArrayOf<Byte>::Free(tempData);

    return NOERROR;
}

ECode CURLUtil::IsAssetUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    *pFlag = ((NULL != url) && (url.Substring(0,String(ASSET_BASE).GetLength()) == String(ASSET_BASE) ) );
    return NOERROR;
}

ECode CURLUtil::IsResourceUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    *pFlag = ((NULL != url ) && (url.Substring(0,String(RESOURCE_BASE).GetLength()) == String(RESOURCE_BASE) ) );
    return NOERROR;
}

ECode CURLUtil::IsCookielessProxyUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    *pFlag = ((NULL != url ) && (url.Substring(0,String(PROXY_BASE).GetLength()) == String(PROXY_BASE) ) );
    return NOERROR;
}

ECode CURLUtil::IsFileUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    *pFlag = (NULL != url );
    *pFlag = (*pFlag) && (url.Substring(0,String(FILE_BASE).GetLength()) == String(FILE_BASE) );
    *pFlag = (*pFlag) && (!(url.Substring(0,String(ASSET_BASE).GetLength()) == String(ASSET_BASE) ) );
    *pFlag = (*pFlag) && (!(url.Substring(0,String(PROXY_BASE).GetLength()) == String(PROXY_BASE) ) );
    return NOERROR;
}

ECode CURLUtil::IsAboutUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    String strUrlFlag("about:");
    *pFlag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return NOERROR;
}

ECode CURLUtil::IsDataUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    String strUrlFlag("data:");
    *pFlag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return NOERROR;
}

ECode CURLUtil::IsJavaScriptUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{    
    String strUrlFlag("javascript:");
    *pFlag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return NOERROR;
}

ECode CURLUtil::IsHttpUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    *pFlag = (NULL != url);
    *pFlag = (*pFlag) && ( url.GetLength() > 6 );
    *pFlag = (*pFlag) && ( url.Substring(0,7).EqualsIgnoreCase("http://") );
    return NOERROR;
}

ECode CURLUtil::IsHttpsUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    *pFlag = (NULL != url);
    *pFlag = (*pFlag) && ( url.GetLength() > 7 );
    *pFlag = (*pFlag) && ( url.Substring(0,8).EqualsIgnoreCase("https://") );
    return NOERROR;
}

ECode CURLUtil::IsNetworkUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    if (url == NULL || url.GetLength() == 0) 
    {
        *pFlag = FALSE;
        return NOERROR;
    }
    Boolean bHttp,bHttps;
    IsHttpUrl(url,&bHttp);
    IsHttpsUrl(url,&bHttps);
    *pFlag =  bHttp || bHttps;
    return NOERROR;
}

ECode CURLUtil::IsContentUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    String strUrlFlag("content:");
    *pFlag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return NOERROR;
}

ECode CURLUtil::IsValidUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * pFlag)
{
    if (url == NULL || url.GetLength() == 0) 
    {
        *pFlag = FALSE;
        return NOERROR;
    }
    Boolean bAssetUrl,bResourceUrl,bFileUrl,bAboutUrl,bHttpUrl,bHttpsUrl,bJavaScriptUrl,bContentUrl;
    IsAssetUrl(url,&bAssetUrl);
    IsResourceUrl(url,&bResourceUrl);
    IsFileUrl(url,&bFileUrl);
    IsAboutUrl(url,&bAboutUrl);
    IsHttpUrl(url,&bHttpUrl);
    IsHttpsUrl(url,&bHttpsUrl);
    IsJavaScriptUrl(url,&bJavaScriptUrl);
    IsContentUrl(url,&bContentUrl);
    *pFlag = bAssetUrl || bResourceUrl || bFileUrl || bAboutUrl || bHttpUrl || bHttpsUrl || bJavaScriptUrl || bContentUrl;
    return NOERROR;
}

ECode CURLUtil::StripAnchor(
    /* [in] */ const String& url,
    /* [out] */ String * pOutUrl)
{
    Int32 anchorIndex = url.IndexOf('#');
    if (anchorIndex != -1) 
    {
        *pOutUrl = url.Substring(0, anchorIndex);
        return NOERROR;
    }
    *pOutUrl = url;
    return NOERROR;
}

ECode CURLUtil::GuessFileName(
    /* [in] */ const String& url,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimeType,
    /* [out] */ String * pName)
{
    String filename(NULL);
    String extension(NULL);

    // If we couldn't do anything with the hint, move toward the content disposition
    if (filename.IsNullOrEmpty() && (!contentDisposition.IsNullOrEmpty()) ) 
    {        
        ParseContentDisposition(contentDisposition,&filename);
        if (!filename.IsNullOrEmpty()) 
        {
            Int32 index = filename.LastIndexOf('/') + 1;
            if (index > 0) 
            {
                filename = filename.Substring(index);
            }
        }
    }

    // If all the other http-related approaches failed, use the plain uri
    if (filename.IsNullOrEmpty()) 
    {
        String decodedUrl(NULL);
        Uri::Decode(url,&decodedUrl);
        if (decodedUrl.IsNullOrEmpty()) 
        {
            Int32 queryIndex = decodedUrl.IndexOf('?');
            // If there is a query string strip it, same as desktop browsers
            if (queryIndex > 0) 
            {
                decodedUrl = decodedUrl.Substring(0, queryIndex);
            }
            if (!decodedUrl.EndWith("/")) 
            {
                Int32 index = decodedUrl.LastIndexOf('/') + 1;
                if (index > 0) 
                {
                    filename = decodedUrl.Substring(index);
                }
            }
        }
    }

    // Finally, if couldn't get filename from URI, get a generic filename
    if (filename.IsNullOrEmpty())
    {
        filename = "downloadfile";
    }

    // Split filename between base and extension
    // Add an extension if filename does not have one
    Int32 dotIndex = filename.IndexOf('.');
    if (dotIndex < 0) 
    {
        if (!mimeType.IsNullOrEmpty()) 
        {
            //(CMimeTypeMap::GetSingleton()) -> GetExtensionFromMimeType(mimeType,&extension);
            if (extension.IsNullOrEmpty()) 
            {
                extension = String(".") + extension;
            }
        }
        if (extension.IsNullOrEmpty()) 
        {
            if ((!mimeType.IsNullOrEmpty()) && ((mimeType.Substring(0,5)).EqualsIgnoreCase("text/")) ) 
            {
                if (mimeType.EqualsIgnoreCase("text/html")) 
                {
                    extension = ".html";
                } 
                else 
                {
                    extension = ".txt";
                }
            } 
            else 
            {
                extension = ".bin";
            }
        }
    } 
    else 
    {
        if (!mimeType.IsNullOrEmpty()) 
        {
            // Compare the last segment of the extension against the mime type.
            // If there's a mismatch, discard the entire extension.            
            String typeFromExt(NULL);
            //Int32 lastDotIndex = filename.LastIndexOf('.');
            //(CMimeTypeMap::GetSingleton()) -> GetMimeTypeFromExtension(filename.substring(lastDotIndex + 1),&typeFromExt);
            if ((!typeFromExt.IsNullOrEmpty()) && (!typeFromExt.EqualsIgnoreCase(mimeType)) ) 
            {
                //(CMimeTypeMap::GetSingleton()) -> GetExtensionFromMimeType(mimeType,&extension);
                if (!extension.IsNullOrEmpty()) 
                {
                    extension = String(".") + extension;
                }
            }
        }
        if (extension.IsNullOrEmpty()) 
        {
            extension = filename.Substring(dotIndex);
        }
        filename = filename.Substring(0, dotIndex);
    }

    *pName = filename + extension;    

    return NOERROR;
}

ECode CURLUtil::VerifyURLEncoding(
        /* [in] */ String url,
        /* [out] */ Boolean * retVal)
{
    Int32 count = url.GetLength();
    if(count == 0)
    {
        *retVal = FALSE;
        return NOERROR;
    }

    int index = url.IndexOf('%');
    ECode ec;
    Int32 nT=0;
    while (index >= 0 && index < count) 
    {
        if (index < (count - 2) ) 
        {
            ec = ParseHex((Byte) url.GetChar(++index),&nT);
            ec = ParseHex((Byte) url.GetChar(++index),&nT);
            if(FAILED(ec))
            {
                //JAVA:catch (IllegalArgumentException e)
                *retVal = FALSE;
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }
        else
        {
            *retVal = FALSE;
            return NOERROR;
        }
        index = url.IndexOf('%', index + 1);
    }
    *retVal = TRUE;
    return NOERROR;    
}

ECode CURLUtil::ParseContentDisposition(
        /* [in] */ String contentDisposition,
        /* [out] */ String * retVal)
{
    /*
    CMatcher m = CONTENT_DISPOSITION_PATTERN -> Matcher(contentDisposition);
    Boolean bMatchFound;
    ECode ec = m -> Find(&bMatchFound);
    if(FAILED(ec))
    {
        retVal = NULL;
        return ec;  //return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (bMatchFound) 
    {
        ec = m -> Group(retVal);
        if(FAILED(ec))
        {
            retVal = NULL;
            return ec;  //return E_ILLEGAL_STATE_EXCEPTION;
        }
        return NOERROR;
    }
    retVal = NULL;
    */
    return NOERROR;
}

#endif