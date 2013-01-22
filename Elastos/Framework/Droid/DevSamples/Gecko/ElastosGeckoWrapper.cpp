
#include "ElastosGeckoWrapper.h"
#include "wrapper.h"
#include "GeckoAppShell.h"
#include "GeckoEvent.h"

using namespace Elastos;

EGEnv* gEGEnv = NULL;

//IPoint
int IPointGetX(eobject eobj)
{
    if (!eobj) return -1;
    IPoint* pPoint = (IPoint*)eobj;
    Int32 x = -1;
    //pPoint->GetX(&x);
    return x;
}

int IPointGetY(eobject eobj)
{
    if (!eobj) return -1;
    IPoint* pPoint = (IPoint*)eobj;
    Int32 y = -1;
    //pPoint->GetX(&y);
    return y;
}

//IRect
int IRectGetBottom(eobject eobj)
{
    if (!eobj) return -1;
    IRect* pRect = (IRect*)eobj;
    Int32 bottom = -1;
    pRect->GetBottom(&bottom);
    return bottom;
}

int IRectGetLeft(eobject eobj)
{
    if (!eobj) return -1;
    IRect* pRect = (IRect*)eobj;
    Int32 left = -1;
    pRect->GetBottom(&left);
    return left;
}

int IRectGetTop(eobject eobj)
{
    if (!eobj) return -1;
    IRect* pRect = (IRect*)eobj;
    Int32 top = -1;
    pRect->GetBottom(&top);
    return top;
}

int IRectGetRight(eobject eobj)
{
    if (!eobj) return -1;
    IRect* pRect = (IRect*)eobj;
    Int32 right = -1;
    pRect->GetBottom(&right);
    return right;
}

//GeckoSurfaceView
int GeckoSurfaceViewBeginDrawing(eobject eobj)
{
    if (!eobj) return -1;
    CGeckoSurfaceView* pSurfaceView = (CGeckoSurfaceView*)eobj;
    Int32 result = -1;
    pSurfaceView->BeginDrawing(&result);
    return result;
}

eobject GeckoSurfaceViewGetSoftwareDrawBitmap(eobject eobj)
{
    if (!eobj) return NULL;
    CGeckoSurfaceView* pSurfaceView = (CGeckoSurfaceView*)eobj;
    IBitmap* pBitmap = NULL;
    pSurfaceView->GetSoftwareDrawBitmap(&pBitmap);
    assert(pBitmap);
    return (void*)pBitmap;
}

eobject GeckoSurfaceViewGetSoftwareDrawBuffer(eobject eobj)
{
    if (!eobj) return NULL;
    CGeckoSurfaceView* pSurfaceView = (CGeckoSurfaceView*)eobj;
    IBuffer* pByteBuffer = NULL;
    pSurfaceView->GetSoftwareDrawBuffer(&pByteBuffer);
    assert(pByteBuffer);
    return (void*)pByteBuffer;
}

void GeckoSurfaceViewEndDrawing(eobject eobj)
{
    if (!eobj) return;
    CGeckoSurfaceView* pSurfaceView = (CGeckoSurfaceView*)eobj;
    pSurfaceView->EndDrawing();
}

void GeckoSurfaceViewDraw2D(eobject eobj, eobject bitmap, int width, int height)
{
    if (!eobj) return;
    CGeckoSurfaceView* pSurfaceView = (CGeckoSurfaceView*)eobj;
    pSurfaceView->Draw2D((IBitmap*)bitmap, width, height);
}

void GeckoSurfaceViewDraw2DEx(eobject eobj, eobject buffer, int stride)
{
    if (!eobj) return;
    CGeckoSurfaceView* pSurfaceView = (CGeckoSurfaceView*)eobj;
    pSurfaceView->Draw2D((IByteBuffer*)buffer, stride);
}

eobject GeckoSurfaceViewGetSurfaceHolder(eobject eobj)
{
    if (!eobj) return NULL;
    CGeckoSurfaceView* pSurfaceView = (CGeckoSurfaceView*)eobj;
    ISurfaceHolder* pHolder = NULL;
    pSurfaceView->GetHolder(&pHolder);
    assert(pHolder);
    return (void*)pHolder;
}

//ILocation
double ILocationGetLatitude(eobject eobj)
{
    if (!eobj) return -1;
    ILocation* pLocation = (ILocation*)eobj;
    double latitude;
    pLocation->GetLatitude(&latitude);
    return latitude;
}

double ILocationGetLongitude(eobject eobj)
{
    if (!eobj) return -1;
    ILocation* pLocation = (ILocation*)eobj;
    double longitude;
    pLocation->GetLongitude(&longitude);
    return longitude;
}

double ILocationGetAltitude(eobject eobj)
{
    if (!eobj) return -1;
    ILocation* pLocation = (ILocation*)eobj;
    double altitude;
    pLocation->GetAltitude(&altitude);
    return altitude;
}

float ILocationGetAccuracy(eobject eobj)
{
    if (!eobj) return -1;
    ILocation* pLocation = (ILocation*)eobj;
    float accuracy;
    pLocation->GetAccuracy(&accuracy);
    return accuracy;
}

float ILocationGetBearing(eobject eobj)
{
    if (!eobj) return -1;
    ILocation* pLocation = (ILocation*)eobj;
    float bearing;
    pLocation->GetBearing(&bearing);
    return bearing;
}

float ILocationGetSpeed(eobject eobj)
{
    if (!eobj) return -1;
    ILocation* pLocation = (ILocation*)eobj;
    float speed;
    pLocation->GetSpeed(&speed);
    return speed;
}

long long ILocationGetTime(eobject eobj)
{
    if (!eobj) return -1;
    ILocation* pLocation = (ILocation*)eobj;
    Int64 time;
    pLocation->GetTime(&time);
    return time;
}

const char* IAddressGetAdminArea(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String area;
    pAddress->GetAdminArea(&area);
    return area.string();
}

const char* IAddressGetCountryCode(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String countryCode;
    pAddress->GetCountryCode(&countryCode);
    return countryCode.string();
}

const char* IAddressGetCountryName(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String countryName;
    pAddress->GetCountryName(&countryName);
    return countryName.string();
}

const char* IAddressGetFeatureName(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String countryName;
    pAddress->GetFeatureName(&countryName);
    return countryName.string();
}

const char* IAddressGetLocality(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String locality;
    pAddress->GetLocality(&locality);
    return locality.string();
}

const char* IAddressGetPostalCode(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String postalCode;
    pAddress->GetPostalCode(&postalCode);
    return postalCode.string();
}

const char* IAddressGetPremises(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String permises;
    pAddress->GetPremises(&permises);
    return permises.string();
}

const char* IAddressGetSubAdminArea(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String subArea;
    pAddress->GetSubAdminArea(&subArea);
    return subArea.string();
}

const char* IAddressGetSubLocality(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String subLocality;
    pAddress->GetSubLocality(&subLocality);
    return subLocality.string();
}

const char* IAddressGetSubThoroughfare(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String subThoroughfare;
    pAddress->GetSubThoroughfare(&subThoroughfare);
    return subThoroughfare.string();
}

const char* IAddressGetThoroughfare(eobject eobj)
{
    if (!eobj) return NULL;
    IAddress* pAddress = (IAddress*)eobj;
    String thoroughfare;
    pAddress->GetThoroughfare(&thoroughfare);
    return thoroughfare.string();
}

//GeckoEvent
int GeckoEventGetAction(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mAction;
}

int GeckoEventGetType(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mType;
}

long long GeckoEventGetTime(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mTime;
}

eobject GeckoEventGetP0(eobject eobj)
{
    if (!eobj) return NULL;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    pGeckoEvent->mP0->AddRef();
    return pGeckoEvent->mP0;
}

eobject GeckoEventGetP1(eobject eobj)
{
    if (!eobj) return NULL;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    pGeckoEvent->mP1->AddRef();
    return pGeckoEvent->mP1;
}

eobject GeckoEventGetRect(eobject eobj)
{
    if (!eobj) return NULL;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    pGeckoEvent->mRect->AddRef();
    return pGeckoEvent->mRect;
}

int GeckoEventGetFlags(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mFlags;
}

int GeckoEventGetMetaState(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mMetaState;
}

int GeckoEventGetKeyCode(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mKeyCode;
}

int GeckoEventGetUnicodeChar(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mUnicodeChar;
}

int GeckoEventGetOffset(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mOffset;
}

int GeckoEventGetCount(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mCount;
}

int GeckoEventGetRangeType(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mRangeType;
}

int GeckoEventGetRangeStyles(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mRangeStyles;
}

int GeckoEventGetRangeForeColor(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mRangeForeColor;
}

int GeckoEventGetRangeBackColor(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mRangeBackColor;
}

double GeckoEventGetAlpha(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mAlpha;
}

double GeckoEventGetBeta(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mBeta;
}

double GeckoEventGetGamma(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mGamma;
}

double GeckoEventGetX(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mX;
}

double GeckoEventGetY(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mY;
}

double GeckoEventGetZ(eobject eobj)
{
    if (!eobj) return -1;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mZ;
}

const char* GeckoEventGetCharacters(eobject eobj)
{
    if (!eobj) return NULL;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    return pGeckoEvent->mCharacters.string();
}

eobject GeckoEventGetLocation(eobject eobj)
{
    if (!eobj) return NULL;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    pGeckoEvent->mLocation->AddRef();
    return pGeckoEvent->mLocation;
}

eobject GeckoEventGetAddress(eobject eobj)
{
    if (!eobj) return NULL;
    GeckoEvent* pGeckoEvent = (GeckoEvent*)eobj;
    pGeckoEvent->mAddress->AddRef();
    return pGeckoEvent->mAddress;
}

//GeckoAppShell
void GeckoAppShellNotifyIME(eobject eobj, int type, int state)
{
    GeckoAppShell::NotifyIME(type, state);
}

void GeckoAppShellNotifyIMEEnabled(eobject eobj,  
                    int state, const char* typeHint,
                    const char* actionHint, bool landscapeFS)
{
    String sTypeHint(typeHint);
    String sActionHint(actionHint);
    GeckoAppShell::NotifyIMEEnabled(state, sTypeHint, sActionHint, landscapeFS);
}

void GeckoAppShellNotifyIMEChange(eobject eobj,
                    const char* text, int start, int end, int newEnd)
{
    String sText(text);
    GeckoAppShell::NotifyIMEChange(sText, start, end, newEnd);
}

void GeckoAppShellAcknowledgeEventSync(eobject eobj)
{
    GeckoAppShell::AcknowledgeEventSync();
}

void GeckoAppShellEnableDeviceMotion(eobject eobj, bool enable)
{
    GeckoAppShell::EnableDeviceMotion(enable);
}

void GeckoAppShellEnableLocation(eobject eobj, bool enable)
{
    GeckoAppShell::EnableLocation(enable);
}

void GeckoAppShellReturnIMEQueryResult(eobject eobj,
                        const char* result, int selStart, int selLen)
{
    String sResult(result);
    GeckoAppShell::ReturnIMEQueryResult(sResult, selStart, selLen);
}

void GeckoAppShellOnAppShellReady(eobject eobj)
{
    GeckoAppShell::OnAppShellReady();
}

void GeckoAppShellScheduleRestart(eobject eobj)
{
    GeckoAppShell::ScheduleRestart();
}

void GeckoAppShellOnXreExit(eobject eobj)
{
    GeckoAppShell::OnXreExit();
}

bool GeckoAppShellOpenUriExternal(eobject eobj, 
                            const char* uriSpec, const char* mimeType,
                            const char* packageName, const char* className,
                            const char* action, const char* title)
{
    String sUriSpec(uriSpec);
    String sMimeType(mimeType);
    String sPackageName(packageName);
    String sClassName(className);
    String sAction(action);
    String sTitle(title);
    ECode ec = GeckoAppShell::OpenUriExternal(sUriSpec, sMimeType,
                 sPackageName, sClassName, sAction, sTitle);
    if (FAILED(ec)) return FALSE;
    else return TRUE;
}

const char* GeckoAppShellGetMimeTypeFromExtensions(eobject eobj, const char* fileExt)
{
    String sFileExt(fileExt);
    String mimeType;
    GeckoAppShell::GetMimeTypeFromExtensions(sFileExt, &mimeType);
    return mimeType.string();
}

const char* GeckoAppShellGetExtensionFromMimeType(eobject eobj, const char* mimeType)
{
    String sMimeType(mimeType);
    String extension;
    GeckoAppShell::GetExtensionFromMimeType(sMimeType, &extension);
    return extension.string();
}

void GeckoAppShellMoveTaskToBack(eobject eobj)
{
    GeckoAppShell::MoveTaskToBack();
}

const char* GeckoAppShellGetClipboardText(eobject eobj)
{
    String text;
    GeckoAppShell::GetClipboardText(&text);
    return text.string();
}

void GeckoAppShellSetClipboardText(eobject eobj, const char* text)
{
    String sText(text);
    GeckoAppShell::SetClipboardText(sText);
}

void GeckoAppShellShowAlertNotification(eobject eobj, const char* imageUrl,
                            const char* alertTitle, const char* alertText,
                            const char* alertCookie, const char* alertName)
{
    String sUri(imageUrl);
    String sTitle(alertTitle);
    String sText(alertText);
    String sCookie(alertCookie);
    String sName(alertName);
    GeckoAppShell::ShowAlertNotification(sUri, sTitle, sText, sCookie, sName);
}

void GeckoAppShellAlertsProgressListener_OnProgress(
                            eobject eobj, const char* alertName, long long progress, 
                            long long progressMax, const char* alertText)

{
    String sText(alertText);
    String sName(alertName);
    GeckoAppShell::AlertsProgressListener_OnProgress(
                    sName, progress, progressMax, sText);
}

void GeckoAppShellAlertsProgressListener_OnCancel(eobject eobj, const char* alertName)
{
    String sName(alertName);
    GeckoAppShell::AlertsProgressListener_OnCancel(sName);
}

int GeckoAppShellGetDPI(eobject eobj)
{
    Int32 dpi = -1;
    GeckoAppShell::GetDpi(&dpi);
    return dpi;
}

const char* GeckoAppShellShowFilePicker(eobject eobj, const char* filters)
{
    String sFilters(filters);
    String filePath;
    GeckoAppShell::ShowFilePicker(sFilters, &filePath);
    return filePath.string();
}

void GeckoAppShellSetFullScreen(eobject eobj, bool fullScreen)
{
    GeckoAppShell::SetFullScreen(fullScreen);
}

void GeckoAppShellHideProgressDialog(eobject eobj)
{
    GeckoAppShell::HideProgressDialog();
}

void GeckoAppShellPerformHapticFeedback(eobject eobj, bool isLongPress)
{
    GeckoAppShell::PerformHapticFeedback(isLongPress);
}

bool GeckoAppShellIsNetworkLinkUp(eobject eobj)
{
    Boolean isLinkUp = FALSE;
    GeckoAppShell::IsNetworkLinkUp(&isLinkUp);
    return isLinkUp;
}

bool GeckoAppShellIsNetworkLinkKnown(eobject eobj)
{
    Boolean isLinkKnown = FALSE;
    GeckoAppShell::IsNetworkLinkKnown(&isLinkKnown);
    return isLinkKnown;
}

void GeckoAppShellSetSelectedLocale(eobject eobj, const char* aLocale)
{
    String locale(aLocale);
    GeckoAppShell::SetSelectedLocale(locale);
}

void GeckoAppShellShowInputMethodPicker(eobject eobj)
{
    GeckoAppShell::ShowInputMethodPicker();
}

void GeckoAppShellSetKeepScreenOn(eobject eobj, bool on)
{
    GeckoAppShell::SetKeepScreenOn(on);
}

void GeckoAppShellScanMedia(eobject eobj, const char* file, const char* mimeType)
{
    String sFile(file);
    String sMimeType(mimeType);
    GeckoAppShell::ScanMedia(sFile, sMimeType);
}

eobject GeckoAppShellGetIconForExtension(eobject eobj, const char* fileExt, int iconSize)
{
    String sFileExt(fileExt);
    ArrayOf<Byte>* pIcons = NULL;
    GeckoAppShell::GetIconForExtension(sFileExt, iconSize, &pIcons);
    return (void*)pIcons;
}

eobject GeckoAppShellGetSystemColors(eobject eobj)
{
    ArrayOf<Int32>* pColors = NULL;
    GeckoAppShell::GetSystemColors(&pColors);
    return (void*)pColors;
}

eobject GeckoAppShellGetHandlersForURL(eobject eobj, const char* URL, const char* action)
{
    String sURL(URL);
    String sAction(action);
    ArrayOf<String>* pHandlers = NULL;
    GeckoAppShell::GetHandlersForURL(sURL, sAction, &pHandlers);
    return (void*)pHandlers;
}

eobject GeckoAppShellGetHandlersForMimeType(eobject eobj,
                            const char* mimeType, const char* action)
{
    String sMimeType(mimeType);
    String sAction(action);
    ArrayOf<String>* pHandlers = NULL;
    GeckoAppShell::GetHandlersForMimeType(sMimeType, sAction, &pHandlers);
    return (void*)pHandlers;
}

void GeckoAppShellInstallWebApplication(eobject eobj, const char* uri,
                        const char* title, const char* iconData)
{
    String sUri(uri);
    String sTitle(title);
    String sIconData(iconData);
    GeckoAppShell::InstallWebApplication(sUri, sTitle, sIconData);
}

//ArrayOf<T>
int ArrayOfByteGetLength(eobject eobj)
{
    if (!eobj) return -1;
    ArrayOf<Byte>* pArray = (ArrayOf<Byte>*)eobj;
    return pArray->GetLength();
}

int ArrayOfIntGetLength(eobject eobj)
{
    if (!eobj) return -1;
    ArrayOf<Int32>* pArray = (ArrayOf<Int32>*)eobj;
    return pArray->GetLength();
}

int ArrayOfStringGetLength(eobject eobj)
{
    if (!eobj) return -1;
    ArrayOf<String>* pArray = (ArrayOf<String>*)eobj;
    return pArray->GetLength();
}

unsigned char* ArrayOfByteGetPayload(eobject eobj)
{
    if (!eobj) return NULL;
    ArrayOf<Byte>* pArray = (ArrayOf<Byte>*)eobj;
    return pArray->GetPayload();
}

int* ArrayOfIntGetPayload(eobject eobj)
{
    if (!eobj) return NULL;
    ArrayOf<Int32>* pArray = (ArrayOf<Int32>*)eobj;
    return pArray->GetPayload();
}

const char* ArrayOfGetContent(eobject eobj, int index)
{
    if (!eobj || index < 0) return NULL;
    ArrayOf<String>* pArray = (ArrayOf<String>*)eobj;
    Int32 len = pArray->GetLength();
    if (index >= len) return NULL;
    return (*pArray)[index].string();
}

void ArrayOfByteFree(eobject eobj)
{
    if (!eobj) return;
    ArrayOf<Byte>* pArray = (ArrayOf<Byte>*)eobj;
    ArrayOf<Byte>::Free(pArray);
}

void ArrayOfIntFree(eobject eobj)
{
    if (!eobj) return;
    ArrayOf<Int32>* pArray = (ArrayOf<Int32>*)eobj;
    ArrayOf<Int32>::Free(pArray);
}

void ArrayOfStringFree(eobject eobj)
{
    if (!eobj) return;
    ArrayOf<String>* pArray = (ArrayOf<String>*)eobj;
    Int32 len = pArray->GetLength();
    for (Int32 i = 0; i < len; i++) {
        (*pArray)[i] = NULL;
    }
    ArrayOf<String>::Free(pArray);
}

void FreeEObject(eobject eobj)
{
    if (!eobj) return;
    ((IInterface*)eobj)->Release();
}

void InitEGEnv()
{
    gEGEnv = new EGEnv();
    assert(gEGEnv != NULL);

    //IPoint
    gEGEnv->IPointGetX = IPointGetX;
    gEGEnv->IPointGetY = IPointGetY;

    //IRect
    gEGEnv->IRectGetBottom = IRectGetBottom;
    gEGEnv->IRectGetLeft = IRectGetLeft;
    gEGEnv->IRectGetTop = IRectGetTop;
    gEGEnv->IRectGetRight = IRectGetRight;

    //GeckoSurfaceView
    gEGEnv->GeckoSurfaceViewBeginDrawing = GeckoSurfaceViewBeginDrawing;
    gEGEnv->GeckoSurfaceViewGetSoftwareDrawBitmap = GeckoSurfaceViewGetSoftwareDrawBitmap;
    gEGEnv->GeckoSurfaceViewGetSoftwareDrawBuffer = GeckoSurfaceViewGetSoftwareDrawBuffer;
    gEGEnv->GeckoSurfaceViewEndDrawing = GeckoSurfaceViewEndDrawing;
    gEGEnv->GeckoSurfaceViewDraw2D = GeckoSurfaceViewDraw2D;
    gEGEnv->GeckoSurfaceViewDraw2DEx = GeckoSurfaceViewDraw2DEx;
    gEGEnv->GeckoSurfaceViewGetSurfaceHolder = GeckoSurfaceViewGetSurfaceHolder;

    //ILocation
    gEGEnv->ILocationGetLatitude = ILocationGetLatitude;
    gEGEnv->ILocationGetLongitude = ILocationGetLongitude;
    gEGEnv->ILocationGetAltitude = ILocationGetAltitude;
    gEGEnv->ILocationGetAccuracy = ILocationGetAccuracy;
    gEGEnv->ILocationGetBearing = ILocationGetBearing;
    gEGEnv->ILocationGetSpeed = ILocationGetSpeed;
    gEGEnv->ILocationGetTime = ILocationGetTime;

    //IAddress
    gEGEnv->IAddressGetAdminArea = IAddressGetAdminArea;
    gEGEnv->IAddressGetCountryCode = IAddressGetCountryCode;
    gEGEnv->IAddressGetCountryName = IAddressGetCountryName;
    gEGEnv->IAddressGetFeatureName = IAddressGetFeatureName;
    gEGEnv->IAddressGetLocality = IAddressGetLocality;
    gEGEnv->IAddressGetPostalCode = IAddressGetPostalCode;
    gEGEnv->IAddressGetPremises = IAddressGetPremises;
    gEGEnv->IAddressGetSubAdminArea = IAddressGetSubAdminArea;
    gEGEnv->IAddressGetSubLocality = IAddressGetSubLocality;
    gEGEnv->IAddressGetSubThoroughfare = IAddressGetSubThoroughfare;
    gEGEnv->IAddressGetThoroughfare = IAddressGetThoroughfare;

    //GeckoEvent
    gEGEnv->GeckoEventGetAction = GeckoEventGetAction;
    gEGEnv->GeckoEventGetType = GeckoEventGetType;
    gEGEnv->GeckoEventGetTime = GeckoEventGetTime;
    gEGEnv->GeckoEventGetP0 = GeckoEventGetP0;
    gEGEnv->GeckoEventGetP1 = GeckoEventGetP1;
    gEGEnv->GeckoEventGetRect = GeckoEventGetRect;
    gEGEnv->GeckoEventGetFlags = GeckoEventGetFlags;
    gEGEnv->GeckoEventGetMetaState = GeckoEventGetMetaState;
    gEGEnv->GeckoEventGetKeyCode = GeckoEventGetKeyCode;
    gEGEnv->GeckoEventGetUnicodeChar = GeckoEventGetUnicodeChar;
    gEGEnv->GeckoEventGetOffset = GeckoEventGetOffset;
    gEGEnv->GeckoEventGetCount = GeckoEventGetCount;
    gEGEnv->GeckoEventGetRangeType = GeckoEventGetRangeType;
    gEGEnv->GeckoEventGetRangeStyles = GeckoEventGetRangeStyles;
    gEGEnv->GeckoEventGetRangeForeColor = GeckoEventGetRangeForeColor;
    gEGEnv->GeckoEventGetRangeBackColor = GeckoEventGetRangeBackColor;
    gEGEnv->GeckoEventGetAlpha = GeckoEventGetAlpha;
    gEGEnv->GeckoEventGetBeta = GeckoEventGetBeta;
    gEGEnv->GeckoEventGetGamma = GeckoEventGetGamma;
    gEGEnv->GeckoEventGetX = GeckoEventGetX;
    gEGEnv->GeckoEventGetY = GeckoEventGetY;
    gEGEnv->GeckoEventGetZ = GeckoEventGetZ;
    gEGEnv->GeckoEventGetCharacters = GeckoEventGetCharacters;
    gEGEnv->GeckoEventGetLocation = GeckoEventGetLocation;
    gEGEnv->GeckoEventGetAddress = GeckoEventGetAddress;

    //GeckoAppShell
    gEGEnv->GeckoAppShellNotifyIME = GeckoAppShellNotifyIME;
    gEGEnv->GeckoAppShellNotifyIMEEnabled = GeckoAppShellNotifyIMEEnabled;
    gEGEnv->GeckoAppShellNotifyIMEChange = GeckoAppShellNotifyIMEChange;
    gEGEnv->GeckoAppShellAcknowledgeEventSync = GeckoAppShellAcknowledgeEventSync;
    gEGEnv->GeckoAppShellEnableDeviceMotion = GeckoAppShellEnableDeviceMotion;
    gEGEnv->GeckoAppShellEnableLocation = GeckoAppShellEnableLocation;
    gEGEnv->GeckoAppShellReturnIMEQueryResult = GeckoAppShellReturnIMEQueryResult;
    gEGEnv->GeckoAppShellOnAppShellReady = GeckoAppShellOnAppShellReady;
    gEGEnv->GeckoAppShellScheduleRestart = GeckoAppShellScheduleRestart;
    gEGEnv->GeckoAppShellOnXreExit = GeckoAppShellOnXreExit;
    gEGEnv->GeckoAppShellOpenUriExternal = GeckoAppShellOpenUriExternal;
    gEGEnv->GeckoAppShellGetMimeTypeFromExtensions = GeckoAppShellGetMimeTypeFromExtensions;
    gEGEnv->GeckoAppShellGetExtensionFromMimeType = GeckoAppShellGetExtensionFromMimeType;
    gEGEnv->GeckoAppShellMoveTaskToBack = GeckoAppShellMoveTaskToBack;
    gEGEnv->GeckoAppShellGetClipboardText = GeckoAppShellGetClipboardText;
    gEGEnv->GeckoAppShellSetClipboardText = GeckoAppShellSetClipboardText;
    gEGEnv->GeckoAppShellShowAlertNotification = GeckoAppShellShowAlertNotification;
    gEGEnv->GeckoAppShellAlertsProgressListener_OnProgress = GeckoAppShellAlertsProgressListener_OnProgress;
    gEGEnv->GeckoAppShellAlertsProgressListener_OnCancel = GeckoAppShellAlertsProgressListener_OnCancel;
    gEGEnv->GeckoAppShellGetDPI = GeckoAppShellGetDPI;
    gEGEnv->GeckoAppShellShowFilePicker = GeckoAppShellShowFilePicker;
    gEGEnv->GeckoAppShellSetFullScreen = GeckoAppShellSetFullScreen;
    gEGEnv->GeckoAppShellHideProgressDialog = GeckoAppShellHideProgressDialog;
    gEGEnv->GeckoAppShellPerformHapticFeedback = GeckoAppShellPerformHapticFeedback;
    gEGEnv->GeckoAppShellIsNetworkLinkUp = GeckoAppShellIsNetworkLinkUp;
    gEGEnv->GeckoAppShellIsNetworkLinkKnown = GeckoAppShellIsNetworkLinkKnown;
    gEGEnv->GeckoAppShellSetSelectedLocale = GeckoAppShellSetSelectedLocale;
    gEGEnv->GeckoAppShellShowInputMethodPicker = GeckoAppShellShowInputMethodPicker;
    gEGEnv->GeckoAppShellSetKeepScreenOn = GeckoAppShellSetKeepScreenOn;
    gEGEnv->GeckoAppShellScanMedia = GeckoAppShellScanMedia;
    gEGEnv->GeckoAppShellGetIconForExtension = GeckoAppShellGetIconForExtension;
    gEGEnv->GeckoAppShellGetSystemColors = GeckoAppShellGetSystemColors;
    gEGEnv->GeckoAppShellGetHandlersForURL = GeckoAppShellGetHandlersForURL;
    gEGEnv->GeckoAppShellGetHandlersForMimeType = GeckoAppShellGetHandlersForMimeType;
    gEGEnv->GeckoAppShellInstallWebApplication = GeckoAppShellInstallWebApplication;

    //ArrayOf<T>
    gEGEnv->ArrayOfByteGetLength = ArrayOfByteGetLength;
    gEGEnv->ArrayOfIntGetLength = ArrayOfIntGetLength;
    gEGEnv->ArrayOfStringGetLength = ArrayOfStringGetLength;
    gEGEnv->ArrayOfByteGetPayload = ArrayOfByteGetPayload;
    gEGEnv->ArrayOfIntGetPayload = ArrayOfIntGetPayload;
    gEGEnv->ArrayOfGetContent = ArrayOfGetContent;
    gEGEnv->ArrayOfByteFree = ArrayOfByteFree;
    gEGEnv->ArrayOfIntFree = ArrayOfIntFree;
    gEGEnv->ArrayOfStringFree = ArrayOfStringFree;
    
    gEGEnv->FreeEObject = FreeEObject;
}
