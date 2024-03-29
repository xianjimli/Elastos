
#include "content/CIntent.h"
#include "os/CBundle.h"
#include "net/CStringUri.h"
#include "graphics/CRect.h"
#include <elastos/Algorithm.h>


CIntent::FilterComparison::FilterComparison(
    /* [in] */ IIntent* intent)
{
    assert(intent != NULL);
    mIntent = intent;
    intent->FilterHashCode(&mHashCode);
}

IIntent* CIntent::FilterComparison::GetIntent()
{
    return mIntent;
}

Int32 CIntent::FilterComparison::GetHashCode()
{
    return mHashCode;
}

Boolean CIntent::FilterComparison::Equals(
    /* [in] */ FilterComparison* obj)
{
    if (obj == NULL) return FALSE;
    AutoPtr<IIntent> other = obj->mIntent;
    Boolean isEqual;
    mIntent->FilterEquals(other, &isEqual);
    return isEqual;
}

ECode CIntent::FilterComparison::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

CIntent::CIntent() : mCategories(NULL)
{
}

CIntent::~CIntent()
{
    if (mCategories) {
        delete mCategories;
    }
}

ECode CIntent::constructor()
{
    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ IIntent* intent)
{
    AutoPtr<CIntent> o = (CIntent*)intent;
    mAction = o->mAction;
    mData = o->mData;
    mType = o->mType;
    mCapsule = o->mCapsule;
    mFlags = o->mFlags;
    AutoPtr<IComponentName> componentName;
    o->GetComponent((IComponentName**)&componentName);
    if (componentName) {
        String capsuleName;
        String className;
        componentName->GetCapsuleName(&capsuleName);
        componentName->GetClassName(&className);
        CComponentName::New(
            capsuleName, className, (IComponentName**)&mComponent);
    }

    if (o->mCategories != NULL) {
        mCategories = new HashSet<String>();
        if (!mCategories) {
            return E_OUT_OF_MEMORY;
        }

        HashSet<String>::Iterator it;
        for (it = o->mCategories->Begin(); it != o->mCategories->End(); ++it) {
            String s = *it;
            mCategories->Insert(s);
        }
    }
    if (o->mExtras != NULL) {
        CBundle::New(o->mExtras, (IBundle**)&mExtras);
    }
    if (o->mSourceBounds != NULL) {
        CRect::New(o->mSourceBounds, (IRect**)&mSourceBounds);
    }

    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean all)
{
    AutoPtr<CIntent> o = (CIntent*)intent;
    mAction = o->mAction;
    mData = o->mData;
    mType = o->mType;
    mCapsule = o->mCapsule;
    AutoPtr<IComponentName> componentName;
    o->GetComponent((IComponentName**)&componentName);
    if (componentName) {
        String capsuleName;
        String className;
        componentName->GetCapsuleName(&capsuleName);
        componentName->GetClassName(&className);
        CComponentName::NewByFriend(
            capsuleName, className, (CComponentName**)&mComponent);
    }


    if (o->mCategories != NULL) {
        mCategories = new HashSet<String>();
        if (!mCategories) {
            return E_OUT_OF_MEMORY;
        }
        HashSet<String>::Iterator it;
        for (it = o->mCategories->Begin(); it != o->mCategories->End(); ++it) {
            String s = *it;
            mCategories->Insert(s);
        }
    }

    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ const String& action)
{
    mAction = action;
    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ const String& action,
    /* [in] */ IUri* uri)
{
    mAction = action;
    mData = uri;
    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ Handle32 intent)
{
    return NOERROR;
}

ECode CIntent::CloneFilter(
    /* [out] */ IIntent** result)
{
    VALIDATE_NOT_NULL(result);

    return CIntent::New(this, FALSE, result);
}

ECode CIntent::GetAction(
    /* [out] */ String* action)
{
    VALIDATE_NOT_NULL(action);
    *action = mAction;

    return NOERROR;
}

ECode CIntent::GetData(
    /* [out] */ IUri** data)
{
    VALIDATE_NOT_NULL(data);
    *data = mData;
    if (mData != NULL) mData->AddRef();

    return NOERROR;
}

ECode CIntent::GetDataString(
    /* [out] */ String* dataString)
{
    VALIDATE_NOT_NULL(dataString);
    if (mData != NULL) {
        // TODO: ALEX
        // return mData->ToString(dataString);
        return E_NOT_IMPLEMENTED;
    }
    else {
        *dataString = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    if (mData != NULL) {
        return mData->GetScheme(scheme);
    }
    else {
        *scheme = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;

	return NOERROR;
}

ECode CIntent::ResolveType(
    /* [in] */ IContext* context,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(type);

    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver));

    return ResolveTypeEx(resolver, type);
}

ECode CIntent::ResolveTypeEx(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(resolver);
    VALIDATE_NOT_NULL(type);

    if (!mType.IsNull()) {
        *type = mType;
        return NOERROR;
    }

    if (mData != NULL) {
        String scheme;
        FAIL_RETURN(mData->GetScheme(&scheme));
        if (scheme.Equals("content")) {
            return resolver->GetType(mData, type);
        }
    }

    *type = NULL;

    return NOERROR;
}

ECode CIntent::ResolveTypeIfNeeded(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(resolver);
    VALIDATE_NOT_NULL(type);

    if (mComponent != NULL) {
        *type = mType;
        return NOERROR;
    }

    return ResolveTypeEx(resolver, type);
}

ECode CIntent::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    if (mCategories != NULL) {
        HashSet<String>::Iterator it;
        for (it = mCategories->Begin(); it != mCategories->End(); ++it) {
            if (!(*it).Compare(category)) {
                *result = TRUE;
                break;
            }
        }
    }

    return NOERROR;
}

ECode CIntent::GetCategories(
    /* [out, callee] */ ArrayOf<String>** categories)
{
    VALIDATE_NOT_NULL(categories);

    *categories = NULL;
    if (mCategories != NULL) {
        *categories = ArrayOf<String>::Alloc(mCategories->GetSize());
        if (!*categories) {
            return E_OUT_OF_MEMORY;
        }

        HashSet<String>::Iterator it;
        Int32 idx = 0;
        for (it = mCategories->Begin(); it != mCategories->End(); ++it, ++idx) {
            (**categories)[idx] = *it;
        }
    }

    return NOERROR;
}

ECode CIntent::SetExtrasClassLoader(
    /* [in] */ IClassLoader* loader)
{
    if (mExtras != NULL) {
        return mExtras->SetClassLoader(loader);
    }
    else {
        return NOERROR;
    }
}

ECode CIntent::HasExtra(
    /* [in] */ const String& name,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mExtras != NULL) {
        Boolean containsKey;
        FAIL_RETURN(mExtras->ContainsKey(name, &containsKey));
        *result = containsKey;
    }
    else {
        *result = FALSE;
    }

    return NOERROR;
}

ECode CIntent::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mExtras != NULL) {
        return mExtras->HasFileDescriptors(result);
    }
    else {
        *result = FALSE;
        return NOERROR;
    }
}

/**
 * Retrieve extended data from the intent.
 *
 * @param name The name of the desired item.
 *
 * @return the value of an item that previously added with putExtra()
 * or null if none was found.
 *
 * @deprecated
 * @hide
 */
// @Deprecated
// public Object getExtra(String name);

ECode CIntent::GetBooleanExtra(
    /* [in] */ const String& name,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetBooleanEx(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetByteExtra(
    /* [in] */ const String& name,
    /* [in] */ Byte defaultValue,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetByteEx(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetInt16Extra(
    /* [in] */ const String& name,
    /* [in] */ Int16 defaultValue,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetInt16Ex(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetCharExtra(
    /* [in] */ const String& name,
    /* [in] */ Char32 defaultValue,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetCharEx(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetInt32Extra(
    /* [in] */ const String& name,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetInt32Ex(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetInt64Extra(
    /* [in] */ const String& name,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetInt64Ex(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetFloatExtra(
    /* [in] */ const String& name,
    /* [in] */ Float defaultValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetFloatEx(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetDoubleExtra(
    /* [in] */ const String& name,
    /* [in] */ Double defaultValue,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetDoubleEx(name, defaultValue, value);
    }
    else {
        *value = defaultValue;
        return NOERROR;
    }
}

ECode CIntent::GetStringExtra(
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetString(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetCharSequenceExtra(
    /* [in] */ const String& name,
    /* [out] */ ICharSequence** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetCharSequence(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetParcelableExtra(
    /* [in] */ const String& name,
    /* [out] */ IParcelable** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetParcelable(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetParcelableArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<IParcelable*>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetParcelableArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetParcelableArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetParcelableArrayList(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

/**
 * Retrieve extended data from the intent.
 *
 * @param name The name of the desired item.
 *
 * @return the value of an item that previously added with putExtra()
 * or null if no Serializable value was found.
 *
 * @see #putExtra(String, Serializable)
 */
// GetSerializableExtra(
//     /* [in] */ const String& name,
//     /* [out] */ ISerializable** value);

ECode CIntent::GetIntegerArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetIntegerArrayList(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetStringArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetStringArrayList(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetCharSequenceArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetCharSequenceArrayList(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetBooleanArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Boolean>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetBooleanArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetByteArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetByteArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetInt16ArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Int16>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetInt16Array(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetCharArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Char32>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetCharArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetInt32ArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetInt32Array(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetInt64ArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Int64>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetInt64Array(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetFloatArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Float>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetFloatArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetDoubleArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Double>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetDoubleArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetStringArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<String>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetStringArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetCharSequenceArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<ICharSequence*>** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetCharSequenceArray(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetBundleExtra(
    /* [in] */ const String& name,
    /* [out] */ IBundle** value)
{
    VALIDATE_NOT_NULL(value);

    if (mExtras != NULL) {
        return mExtras->GetBundle(name, value);
    }
    else {
        *value = NULL;
        return NOERROR;
    }
}

/**
 * Retrieve extended data from the intent.
 *
 * @param name The name of the desired item.
 *
 * @return the value of an item that previously added with putExtra()
 * or null if no IBinder value was found.
 *
 * @see #putExtra(String, IBinder)
 *
 * @deprecated
 * @hide
 */
// @Deprecated
// public IBinder getIBinderExtra(String name);

/**
 * Retrieve extended data from the intent.
 *
 * @param name The name of the desired item.
 * @param defaultValue The default value to return in case no item is
 * associated with the key 'name'
 *
 * @return the value of an item that previously added with putExtra()
 * or defaultValue if none was found.
 *
 * @see #putExtra
 *
 * @deprecated
 * @hide
 */
// @Deprecated
// public Object getExtra(String name, Object defaultValue);

ECode CIntent::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);

    if (mExtras != NULL) {
        return CBundle::New(mExtras, extras);
    }
    else {
        *extras = NULL;
        return NOERROR;
    }
}

ECode CIntent::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;

    return NOERROR;
}

ECode CIntent::GetCapsule(
    /* [out] */ String* capsuleName)
{
    VALIDATE_NOT_NULL(capsuleName);
    *capsuleName = mCapsule;

    return NOERROR;
}

ECode CIntent::GetComponent(
    /* [out] */ IComponentName** result)
{
    VALIDATE_NOT_NULL(result);

    *result = (IComponentName*)(CComponentName*)mComponent;
    if (*result != NULL) (*result)->AddRef();

    return NOERROR;
}

ECode CIntent::GetSourceBounds(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mSourceBounds;
    if (mSourceBounds != NULL) mSourceBounds->AddRef();

    return NOERROR;
}

ECode CIntent::ResolveActivity(
    /* [in] */ ICapsuleManager* cm,
    /* [out] */ IComponentName** result)
{
    // TODO: ALEX
    return E_NOT_IMPLEMENTED;
#if 0
    VALIDATE_NOT_NULL(cm);
    VALIDATE_NOT_NULL(result);

    if (mComponent != NULL) {
        *result = mComponent;
        mComponent->AddRef();
        return NOERROR;
    }

    AutoPtr<IResolveInfo> info;
    FAIL_RETURN(cm->ResolveActivity(
        this,
        CapsuleManager_MATCH_DEFAULT_ONLY,
        (IResolveInfo**)&info));

    AutoPtr<IActivityInfo> activityInfo;
    FAIL_RETURN(info->GetActivityInfo((IActivityInfo**)&activityInfo));

    AutoPtr<IApplicationInfo> applicationInfo;
    FAIL_RETURN(activityInfo->GetApplicationInfo(
        (IApplicationInfo**)&applicationInfo));

    String packageName;
    FAIL_RETURN(applicationInfo->GetCapsuleName(&packageName));

    String name;
    FAIL_RETURN(activityInfo->GetName(&name));

    return CComponentName::New(packageName, name, result);
#endif
}

ECode CIntent::ResolveActivityInfo(
    /* [in] */ ICapsuleManager* cm,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** result)
{
    // TODO: ALEX
    return E_NOT_IMPLEMENTED;
#if 0
    VALIDATE_NOT_NULL(cm);
    VALIDATE_NOT_NULL(result);

    *result = NULL;
    if (mComponent != NULL) {
        // try {
        cm->GetActivityInfo(mComponent, flags, &result);
        return NOERROR;
        // } catch (PackageManager.NameNotFoundException e) {
        //     // ignore
        // }
    }
    else {
        AutoPtr<IResolveInfo> info;
        cm->ResolveActivity(
            this,
            CapsuleManager_MATCH_DEFAULT_ONLY | flags,
            (IResolveInfo**)&info);
        if (info != NULL) {
            info->GetActivityInfo(result);
        }
    }

    return NOERROR;
#endif
}

ECode CIntent::SetAction(
    /* [in] */ const String& action)
{
    mAction = action;
    return NOERROR;
}

ECode CIntent::SetData(
    /* [in] */ IUri* data)
{
    mData = data;
    mType = NULL;
    return NOERROR;
}

ECode CIntent::SetType(
  /* [in] */ const String& type)
{
    mData = NULL;
    mType = type;
    return NOERROR;
}

ECode CIntent::SetDataAndType(
    /* [in] */ IUri* data,
    /* [in] */ const String& type)
{
    mData = data;
    mType = type;
    return NOERROR;
}

ECode CIntent::AddCategory(
    /* [in] */ const String& category)
{
    if (mCategories == NULL) {
        mCategories = new HashSet<String>();
        if (!mCategories) {
            return E_OUT_OF_MEMORY_ERROR;
        }
    }
    mCategories->Insert(category);
    return NOERROR;
}

ECode CIntent::RemoveCategory(
    /* [in] */ const String& category)
{
    if (mCategories != NULL) {
        HashSet<String>::Iterator it = mCategories->Find(category);
        if (it == mCategories->End()) {
            return NOERROR;
        }
        mCategories->Erase(it);
        if (mCategories->Begin() == mCategories->End()) {
            delete mCategories;
            mCategories = NULL;
        }
    }
    return NOERROR;
}

ECode CIntent::PutBooleanExtra(
    /* [in] */ const String& name,
    /* [in] */ Boolean value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutBoolean(name, value);
    return NOERROR;
}

ECode CIntent::PutByteExtra(
    /* [in] */ const String& name,
    /* [in] */ Byte value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutByte(name, value);
    return NOERROR;
}

ECode CIntent::PutCharExtra(
    /* [in] */ const String& name,
    /* [in] */ Char32 value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutChar(name, value);
    return NOERROR;
}

ECode CIntent::PutInt16Extra(
    /* [in] */ const String& name,
    /* [in] */ Int16 value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutInt16(name, value);
    return NOERROR;
}

ECode CIntent::PutInt32Extra(
    /* [in] */ const String& name,
    /* [in] */ Int32 value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutInt32(name, value);
    return NOERROR;
}

ECode CIntent::PutInt64Extra(
    /* [in] */ const String& name,
    /* [in] */ Int64 value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutInt64(name, value);
    return NOERROR;
}

ECode CIntent::PutFloatExtra(
    /* [in] */ const String& name,
    /* [in] */ Float value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutFloat(name, value);
    return NOERROR;
}

ECode CIntent::PutDoubleExtra(
    /* [in] */ const String& name,
    /* [in] */ Double value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutDouble(name, value);
    return NOERROR;
}

ECode CIntent::PutStringExtra(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutString(name, value);
    return NOERROR;
}

ECode CIntent::PutCharSequenceExtra(
    /* [in] */ const String& name,
    /* [in] */ ICharSequence* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutCharSequence(name, value);
    return NOERROR;
}

ECode CIntent::PutParcelableExtra(
    /* [in] */ const String& name,
    /* [in] */ IParcelable* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutParcelable(name, value);
    return NOERROR;
}

ECode CIntent::PutParcelableArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<IParcelable*>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutParcelableArray(name, value);
    return NOERROR;
}

ECode CIntent::PutParcelableArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutParcelableArrayList(name, value);
    return NOERROR;
}

ECode CIntent::PutIntegerArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutIntegerArrayList(name, value);
    return NOERROR;
}

ECode CIntent::PutStringArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutStringArrayList(name, value);
    return NOERROR;
}

ECode CIntent::PutCharSequenceArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutCharSequenceArrayList(name, value);
    return NOERROR;
}


// PutSerializableExtra(
//     /* [in] */ const String& name,
//     /* [in] */ ISerializable* value,
//     /* [out] */ IIntent** result);

ECode CIntent::PutBooleanArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Boolean>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutBooleanArray(name, value);
    return NOERROR;
}

ECode CIntent::PutByteArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Byte>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutByteArray(name, value);
    return NOERROR;
}

ECode CIntent::PutInt16ArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Int16>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutInt16Array(name, value);
    return NOERROR;
}

ECode CIntent::PutCharArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Char32>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutCharArray(name, value);
    return NOERROR;
}

ECode CIntent::PutInt32ArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Int32>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutInt32Array(name, value);
    return NOERROR;
}

ECode CIntent::PutInt64ArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Int64>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutInt64Array(name, value);
    return NOERROR;
}

ECode CIntent::PutFloatArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Float>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutFloatArray(name, value);
    return NOERROR;
}

ECode CIntent::PutDoubleArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Double>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutDoubleArray(name, value);
    return NOERROR;
}

ECode CIntent::PutStringArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<String>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutStringArray(name, value);
    return NOERROR;
}

ECode CIntent::PutCharSequenceArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<ICharSequence*>* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutCharSequenceArray(name, value);
    return NOERROR;
}

ECode CIntent::PutBundleExtra(
    /* [in] */ const String& name,
    /* [in] */ IBundle* value)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutBundle(name, value);
    return NOERROR;
}

/**
 * Add extended data to the intent.  The name must include a package
 * prefix, for example the app com.android.contacts would use names
 * like "com.android.contacts.ShowAll".
 *
 * @param name The name of the extra data, with package prefix.
 * @param value The IBinder data value.
 *
 * @return Returns the same Intent object, for chaining multiple calls
 * into a single statement.
 *
 * @see #putExtras
 * @see #removeExtra
 * @see #getIBinderExtra(String)
 *
 * @deprecated
 * @hide
 */
// @Deprecated
// public Intent putExtra(String name, IBinder value);

ECode CIntent::PutExtras(
    /* [in] */ IIntent* src)
{
    VALIDATE_NOT_NULL(src);

    CIntent* intent = (CIntent*)src;
    if (intent->mExtras != NULL) {
        if (mExtras == NULL) {
            FAIL_RETURN(CBundle::New(intent->mExtras, (IBundle**)&mExtras));
        }
        else {
            mExtras->PutAll(intent->mExtras);
        }
    }
    return NOERROR;
}

ECode CIntent::PutExtrasEx(
    /* [in] */ IBundle* extras)
{
    if (mExtras == NULL) {
        FAIL_RETURN(CBundle::New((IBundle**)&mExtras));
    }
    mExtras->PutAll(extras);
    return NOERROR;
}

ECode CIntent::ReplaceExtras(
    /* [in] */ IIntent* src)
{
    VALIDATE_NOT_NULL(src);

    CIntent* intent = (CIntent*)src;
    mExtras = NULL;
    if (intent->mExtras != NULL) {
        FAIL_RETURN(CBundle::New(intent->mExtras, (IBundle**)&mExtras));
    }
    return NOERROR;
}

ECode CIntent::ReplaceExtrasEx(
    /* [in] */ IBundle* extras)
{
    VALIDATE_NOT_NULL(extras);

    mExtras = NULL;
    if (extras != NULL) {
        FAIL_RETURN(CBundle::New(extras, (IBundle**)&mExtras));
    }
    return NOERROR;
}

ECode CIntent::RemoveExtra(
    /* [in] */ const String& name)
{
    if (mExtras != NULL) {
        mExtras->Remove(name);
        Int32 size;
        FAIL_RETURN(mExtras->GetSize(&size));
        if (size == 0) {
            mExtras = NULL;
        }
    }

    return NOERROR;
}

ECode CIntent::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CIntent::AddFlags(
    /* [in] */ Int32 flags)
{
    mFlags |= flags;
    return NOERROR;
}

ECode CIntent::SetCapsule(
    /* [in] */ const String& capsuleName)
{
    mCapsule = capsuleName;
    return NOERROR;
}

ECode CIntent::SetComponent(
    /* [in] */ IComponentName* component)
{
    mComponent = (CComponentName*)component;
    return NOERROR;
}

ECode CIntent::SetClassName(
    /* [in] */ IContext* capsuleContext,
    /* [in] */ const String& className)
{
    // TODO: ALEX
    return E_NOT_IMPLEMENTED;
#if 0
    VALIDATE_NOT_NULL(packageContext);

    mComponent = NULL;
    FAIL_RETURN(CComponentName::New(
        packageContext,
        className,
        (IComponentName**)&mComponent));
    return NOERROR;
#endif
}

ECode CIntent::SetClassNameEx(
    /* [in] */ const String& capsuleName,
    /* [in] */ const String& className)
{
    return CComponentName::New(
            capsuleName, className, (IComponentName**)&mComponent);
}

// TODO: ALEX
// public Intent setClass(Context packageContext, Class<?> cls);

ECode CIntent::SetSourceBounds(
    /* [in] */ IRect* r)
{
    mSourceBounds = r;

    return NOERROR;
}

ECode CIntent::FillIn(
    /* [in] */ IIntent* other,
    /* [in] */ Int32 flags,
    /* [out] */ Int32* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::FilterEquals(
    /* [in] */ IIntent* other,
    /* [out] */ Boolean* isEqual)
{
    if (isEqual == NULL) return E_INVALID_ARGUMENT;

    *isEqual = FALSE;
    if (other == NULL) {
        return NOERROR;
    }

    AutoPtr<CIntent> intent = (CIntent*)other;
    if (mAction.IsNullOrEmpty()) {
        if (!intent->mAction.IsNullOrEmpty()) {
            return NOERROR;
        }
    }
    else {
        if (mAction.Compare(intent->mAction)) {
            return NOERROR;
        }
    }
    if (mData != intent->mData) {
        if (mData != NULL) {
            if (SUCCEEDED(mData->Equals(intent->mData, isEqual)) && !isEqual) {
                return NOERROR;
            }
        } else {
            if (SUCCEEDED(intent->mData->Equals(mData, isEqual)) && !isEqual) {
                return NOERROR;
            }
        }
    }
    if (mType.IsNullOrEmpty()) {
        if (!intent->mType.IsNullOrEmpty()) {
            return NOERROR;
        }
    }
    else {
        if (mType.Compare(intent->mType)) {
            return NOERROR;
        }
    }
    if (mCapsule.IsNullOrEmpty()) {
        if (!intent->mCapsule.IsNullOrEmpty()) {
            return NOERROR;
        }
    }
    else {
        if (mCapsule.Compare(intent->mCapsule)) {
            return NOERROR;
        }
    }
    if (mComponent != intent->mComponent) {
        if (mComponent != NULL) {
            if (SUCCEEDED(mComponent->Equals(intent->mComponent, isEqual)) && !isEqual) {
                return NOERROR;
            }
        } else {
            if (SUCCEEDED(intent->mComponent->Equals(mComponent, isEqual)) && !isEqual) {
                return NOERROR;
            }
        }
    }
//    if (mCategories != other.mCategories) {
//        if (mCategories != null) {
//            if (!mCategories->equals(other.mCategories)) {
//                return false;
//            }
//        } else {
//            if (!other.mCategories->equals(mCategories)) {
//                return false;
//            }
//        }
//    }

    *isEqual = TRUE;
    return NOERROR;
}

ECode CIntent::FilterHashCode(
    /* [out] */ Int32* result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;

    Int32 code = 0;
    if (!mAction.IsNull()) {
        code += mAction.GetHashCode();
    }
//    if (mData != null) {
//        code += mData.hashCode();
//    }
    if (!mType.IsNull()) {
        code += mType.GetHashCode();
    }
    if (!mCapsule.IsNull()) {
        code += mCapsule.GetHashCode();
    }
    if (mComponent != NULL) {
        Int32 hashCode;
        mComponent->GetHashCode(&hashCode);
        code += hashCode;
    }
//    if (mCategories != null) {
//        code += mCategories->hashCode();
//    }
    *result = code;

    return NOERROR;
}

// @Override
// public String toString();

/** @hide */
ECode CIntent::ToShortString(
    /* [in] */ Boolean comp,
    /* [in] */ Boolean extras,
    /* [out] */ String* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/** @hide */
ECode CIntent::ToShortStringEx(
    /* [in] */ const String& b,
    /* [in] */ Boolean comp,
    /* [in] */ Boolean extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

/**
 * Call {@link #toUri} with 0 flags.
 * @deprecated Use {@link #toUri} instead.
 */
// @Deprecated
// public String toURI();

/**
 * Convert this Intent into a String holding a URI representation of it.
 * The returned URI string has been properly URI encoded, so it can be
 * used with {@link Uri#parse Uri.parse(String)}.  The URI contains the
 * Intent's data as the base URI, with an additional fragment describing
 * the action, categories, type, flags, package, component, and extras.
 *
 * <p>You can convert the returned string back to an Intent with
 * {@link #getIntent}.
 *
 * @param flags Additional operating flags.  Either 0 or
 * {@link #URI_INTENT_SCHEME}.
 *
 * @return Returns a URI encoding URI string describing the entire contents
 * of the Intent.
 */
ECode CIntent::ToUri(
    /* [in] */ Int32 flags,
    /* [out] */ String* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    source->ReadString(&mCapsule);
    CComponentName::ReadFromParcel(source, (CComponentName**)&mComponent);
    source->ReadString(&mAction);

    return CBundle::ReadFromParcel(source, (IBundle**)&mExtras);
}

ECode CIntent::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteString(mCapsule);
    CComponentName::WriteToParcel(mComponent, dest);
    dest->WriteString(mAction);

    return CBundle::WriteToParcel(mExtras, dest);
}
