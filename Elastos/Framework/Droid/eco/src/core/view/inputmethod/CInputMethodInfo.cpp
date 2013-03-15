
#include "ext/frameworkext.h"
#include "capsule/CServiceInfo.h"
#include "view/inputmethod/CInputMethodInfo.h"

#include "content/CComponentName.h"
#include "utils/Xml.h"


CString CInputMethodInfo::TAG = "InputMethodInfo";

CInputMethodInfo::CInputMethodInfo()
    : mIsDefaultResId(0)
{}

static Int32 R_Styleable_InputMethod[] = {
    0x01010221, 0x01010225
};

ECode CInputMethodInfo::constructor()
{
    return NOERROR;
}

ECode CInputMethodInfo::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IResolveInfo* service)
{
    mService = (CResolveInfo*)service;
    CServiceInfo* si = mService->mServiceInfo;
    AutoPtr<CComponentName> componentName;
    CComponentName::NewByFriend(si->mCapsuleName, si->mName, (CComponentName**)&componentName);
    componentName->FlattenToShortString(&mId);

    AutoPtr<ILocalCapsuleManager> pm;
    context->GetCapsuleManager((ILocalCapsuleManager**)&pm);
    String settingsActivityComponent;
    Int32 isDefaultResId = 0;

    // AutoPtr<IXmlResourceParser> parser;
    // // try {
    // si->LoadXmlMetaData(pm, String(InputMethod_SERVICE_META_DATA), (IXmlResourceParser**)&parser);
    // if (parser == NULL) {
    //        assert(0);
    //     // throw new XmlPullParserException("No "
    //     //         + InputMethod.SERVICE_META_DATA + " meta-data");
    //     return E_XML_PULL_PARSER_EXCEPTION;
    // }

    // AutoPtr<IResources> res;
    // pm->GetResourcesForApplication(si->mApplicationInfo, (IResources**)&res);

    // AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    // Int32 type = 0;
    // while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
    //         && type != IXmlPullParser_START_TAG) {
    // }

    // String nodeName;
    // parser->GetName(&nodeName);
    // if (!CString("input-method").Equals(nodeName)) {
    //        assert(0);
    //     // throw new XmlPullParserException(
    //     //         "Meta-data does not start with input-method tag");
    //     return E_XML_PULL_PARSER_EXCEPTION;
    // }

    // AutoPtr<ITypedArray> sa;
    // res->ObtainAttributes(attrs,
    //         ArrayOf<Int32>(R_Styleable_InputMethod, sizeof(R_Styleable_InputMethod) / sizeof(Int32))
    //         /*com.android.internal.R.styleable.InputMethod*/,
    //         (ITypedArray**) &sa);

    // sa->GetString(
    //         1 /*com.android.internal.R.styleable.InputMethod_settingsActivity*/,
    //         &settingsActivityComponent);

    // sa->GetResourceId(
    //         0 /*com.android.internal.R.styleable.InputMethod_isDefault*/, 0,
    //         &isDefaultResId);

    // sa->Recycle();
    // // } catch (NameNotFoundException e) {
    // //     throw new XmlPullParserException(
    // //             "Unable to create context for: " + si.packageName);
    // // } finally {
    // //     if (parser != NULL) parser.close();
    // // }

    // if (parser != NULL) {
    //     parser->Close();
    // }

    mSettingsActivityName = settingsActivityComponent;
    mIsDefaultResId = isDefaultResId;
    return NOERROR;
}

ECode CInputMethodInfo::constructor(
    /* [in] */ const String& capsuleName,
    /* [in] */ const String& className,
    /* [in] */ ICharSequence* label,
    /* [in] */ const String& settingsActivity)
{
    AutoPtr<CResolveInfo> ri;
    CResolveInfo::NewByFriend((CResolveInfo**)&ri);
    AutoPtr<CServiceInfo> si;
    CServiceInfo::NewByFriend((CServiceInfo**)&si);
    AutoPtr<CApplicationInfo> ai;
    CApplicationInfo::NewByFriend((CApplicationInfo**)&ai);
    ai->mCapsuleName = capsuleName;
    ai->mEnabled = TRUE;
    si->mApplicationInfo = ai;
    si->mEnabled = TRUE;
    si->mCapsuleName = capsuleName;
    si->mName = className;
    si->mExported = TRUE;
    si->mNonLocalizedLabel = label;
    ri->mServiceInfo = si;
    mService = ri;
    AutoPtr<CComponentName> componentName;
    CComponentName::NewByFriend(si->mCapsuleName,
            si->mName, (CComponentName**)&componentName);
    componentName->FlattenToShortString(&mId);
    mSettingsActivityName = settingsActivity;
    mIsDefaultResId = 0;
    return NOERROR;
}

ECode CInputMethodInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInputMethodInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInputMethodInfo::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CInputMethodInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mService->mServiceInfo->mCapsuleName;
    return NOERROR;
}

ECode CInputMethodInfo::GetServiceName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mService->mServiceInfo->mName;
    return NOERROR;
}

ECode CInputMethodInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mService->mServiceInfo;
    if (*info != NULL) {
        (*info)->AddRef();
    }

    return NOERROR;
}

ECode CInputMethodInfo::GetComponent(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    return CComponentName::New(mService->mServiceInfo->mCapsuleName,
            mService->mServiceInfo->mName, name);
}

ECode CInputMethodInfo::LoadLabel(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** str)
{
    VALIDATE_NOT_NULL(str);
    assert(0);
    //TODO
    //return mService->LoadLabel(pm, str);
    return NOERROR;
}

ECode CInputMethodInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    assert(0);
    //TODO
    //return mService->LoadIcon(pm, drawable);
    return NOERROR;
}

ECode CInputMethodInfo::GetSettingsActivity(
    /* [out] */ String* settings)
{
    VALIDATE_NOT_NULL(settings);
    *settings = mSettingsActivityName;
    return NOERROR;
}

ECode CInputMethodInfo::GetIsDefaultResourceId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mIsDefaultResId;
    return NOERROR;
}
