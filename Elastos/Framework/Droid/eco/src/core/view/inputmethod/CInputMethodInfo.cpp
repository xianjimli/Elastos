
#include "view/inputmethod/CInputMethodInfo.h"
#include "content/CResolveInfo.h"
#include "capsule/CServiceInfo.h"
#include "content/CComponentName.h"
#include "utils/Xml.h"


static Int32 R_Styleable_InputMethod[] = {
    0x01010221, 0x01010225
};


CString CInputMethodInfo::TAG = "InputMethodInfo";

CInputMethodInfo::CInputMethodInfo():
	mIsDefaultResId(0)
{

}

CInputMethodInfo::~CInputMethodInfo()
{

}

ECode CInputMethodInfo::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IResolveInfo* service)
{
    mService = service;
    CServiceInfo* si = ((CResolveInfo*)service)->mServiceInfo;

    AutoPtr<CComponentName> componentName;
    CComponentName::NewByFriend(si->mCapsuleName, si->mName, (CComponentName**) &componentName);
    componentName->FlattenToShortString(&mId);

    AutoPtr<ICapsuleManager> pm;
    // context->GetPackageManager();
    AutoPtr<IServiceManager> serviceManager;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    assert(serviceManager != NULL);
    serviceManager->GetService(String("capsule"),
            (IInterface**)(ICapsuleManager**)&pm);

    String settingsActivityComponent;
    Int32 isDefaultResId = 0;

    AutoPtr<IXmlResourceParser> parser;
    // try {
    si->LoadXmlMetaData(pm, String(InputMethod_SERVICE_META_DATA), (IXmlResourceParser**) &parser);
    if (parser == NULL) {
    	assert(0);
        // throw new XmlPullParserException("No "
        //         + InputMethod.SERVICE_META_DATA + " meta-data");
    }

    AutoPtr<IResources> res;
    //TODO
    assert(0);
    //pm->GetResourcesForApplication(si->mApplicationInfo, (IResources**) &res);

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type = 0;
    parser->Next(&type);
    while (type != IXmlPullParser_END_DOCUMENT
            && type != IXmlPullParser_START_TAG) {
    	parser->Next(&type);
    }

    String nodeName;
    parser->GetName(&nodeName);
    if (! CString("input-method").Equals(nodeName)) {
    	assert(0);
        // throw new XmlPullParserException(
        //         "Meta-data does not start with input-method tag");
    }

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs,
    		ArrayOf<Int32>(R_Styleable_InputMethod, sizeof(R_Styleable_InputMethod) / sizeof(Int32))
            /*com.android.internal.R.styleable.InputMethod*/,
            (ITypedArray**) &sa);

    sa->GetString(
            1 /*com.android.internal.R.styleable.InputMethod_settingsActivity*/,
            &settingsActivityComponent);

    sa->GetResourceId(
            0 /*com.android.internal.R.styleable.InputMethod_isDefault*/, 0,
            &isDefaultResId);

    sa->Recycle();
    // } catch (NameNotFoundException e) {
    //     throw new XmlPullParserException(
    //             "Unable to create context for: " + si.packageName);
    // } finally {
    //     if (parser != NULL) parser.close();
    // }

    if (parser != NULL) {
    	parser->Close();
    }

    mSettingsActivityName = settingsActivityComponent;
    mIsDefaultResId = isDefaultResId;

    return NOERROR;
}

ECode CInputMethodInfo::constructor(
    /* [in] */ IParcel* source)
{
	assert(source != NULL);
    source->ReadString(&mId);
    source->ReadString(&mSettingsActivityName);
    source->ReadInt32(&mIsDefaultResId);
    return CResolveInfo::New(source, (IResolveInfo**) &mService);
}

ECode CInputMethodInfo::constructor(
    /* [in] */ String packageName,
    /* [in] */ String className,
    /* [in] */ ICharSequence* label,
    /* [in] */ String settingsActivity)
{
    AutoPtr<IResolveInfo> ri;
    CResolveInfo::New((IResolveInfo**) &ri);

    AutoPtr<IServiceInfo> si;
    CServiceInfo::New((IServiceInfo**) &si);

    AutoPtr<IApplicationInfo> ai;
    CApplicationInfo::New((IApplicationInfo**) &ai);
    ((CApplicationInfo*)ai.Get())->mCapsuleName = packageName;
    ((CApplicationInfo*)ai.Get())->mEnabled = TRUE;
    ((CServiceInfo*)si.Get())->mApplicationInfo = (CApplicationInfo*)ai.Get();
    ((CServiceInfo*)si.Get())->mEnabled = TRUE;
    ((CServiceInfo*)si.Get())->mCapsuleName = packageName;
    ((CServiceInfo*)si.Get())->mName = className;
    ((CServiceInfo*)si.Get())->mExported = TRUE;
    ((CServiceInfo*)si.Get())->mNonLocalizedLabel = label;
    ((CResolveInfo*)ri.Get())->mServiceInfo = (CServiceInfo*)si.Get();

    mService = ri;

    AutoPtr<CComponentName> componentName;
    CComponentName::NewByFriend(((CServiceInfo*)si.Get())->mCapsuleName,
    		((CServiceInfo*)si.Get())->mName, (CComponentName**) &componentName);
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
	assert(id != NULL);
	*id = mId;
	return NOERROR;
}

ECode CInputMethodInfo::GetCapsuleName(
    /* [out] */ String* name)
{
	assert(name != NULL);
	*name = ((CResolveInfo*)mService.Get())->mServiceInfo->mCapsuleName;
	return NOERROR;
}

ECode CInputMethodInfo::GetServiceName(
    /* [out] */ String* name)
{
	assert(name != NULL);
	*name = ((CResolveInfo*)mService.Get())->mServiceInfo->mName;
	return NOERROR;
}

ECode CInputMethodInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** info)
{
	assert(info != NULL);
	*info = ((CResolveInfo*)mService.Get())->mServiceInfo;
	if (*info != NULL) {
		(*info)->AddRef();
	}

	return NOERROR;
}

ECode CInputMethodInfo::GetComponent(
    /* [out] */ IComponentName** name)
{
	assert(name != NULL);
    return CComponentName::New(((CResolveInfo*)mService.Get())->mServiceInfo->mCapsuleName,
            ((CResolveInfo*)mService.Get())->mServiceInfo->mName, name);
}

ECode CInputMethodInfo::LoadLabel(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ ICharSequence** str)
{
	assert(mService != NULL);
	assert(0);
	//TODO
	//return mService->LoadLabel(pm, str);
	return NOERROR;
}

ECode CInputMethodInfo::LoadIcon(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** drawable)
{
	assert(mService != NULL);
	assert(0);
	//TODO
	//return mService->LoadIcon(pm, drawable);
	return NOERROR;
}

ECode CInputMethodInfo::GetSettingsActivity(
    /* [out] */ String* settings)
{
	assert(settings != NULL);
	*settings = mSettingsActivityName;
	return NOERROR;
}

ECode CInputMethodInfo::GetIsDefaultResourceId(
    /* [out] */ Int32* id)
{
	assert(id != NULL);
	*id = mIsDefaultResId;
	return NOERROR;
}
