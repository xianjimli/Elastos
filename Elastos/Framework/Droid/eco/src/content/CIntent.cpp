
#include "content/CIntent.h"
#include "utils/CBundle.h"
#include "net/CStringUri.h"
#include <elastos/Algorithm.h>

const String CIntent::EXTRA_CLIENT_LABEL = "android.intent.extra.client_label";
const String CIntent::EXTRA_CLIENT_INTENT = "android.intent.extra.client_intent";

CIntent::FilterComparison::FilterComparison(
    /* [in] */ IIntent* intent)
{
    assert(intent != NULL);
    mIntent = intent;
    intent->GetFilterHashCode(&mHashCode);
}

IIntent*
CIntent::FilterComparison::GetIntent()
{
    return mIntent;
}

Int32
CIntent::FilterComparison::GetHashCode()
{
    return mHashCode;
}

Boolean
CIntent::FilterComparison::Equals(
    /* [in] */ FilterComparison* obj)
{
    if (obj == NULL) return FALSE;
    AutoPtr<IIntent> other = obj->mIntent;
    Boolean isEqual;
    mIntent->FilterEquals(other, &isEqual);
    return isEqual;
}

ECode
CIntent::FilterComparison::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

CIntent::CIntent() : mCategories(NULL)
{
}

CIntent::~CIntent()
{
}

ECode CIntent::constructor()
{
    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ IIntent* intent)
{
    AutoPtr<CIntent> o = (CIntent*)intent;
    mAction = String::Duplicate(o->mAction);
    mData = o->mData;
    mType = String::Duplicate(o->mType);
    mCapsule = String::Duplicate(o->mCapsule);
    mFlags = o->mFlags;
    AutoPtr<IComponentName> componentName;
    o->GetComponent((IComponentName**)&componentName);
    if (componentName) {
        String capsuleName;
        String className;
        componentName->GetCapsuleName(&capsuleName);
        componentName->GetClassName(&className);
        CComponentName::NewByFriend(capsuleName, className, (CComponentName**)&mComponent);
        String::Free(capsuleName);
        String::Free(className);
    }

//    if (o.mCategories != null) {
//        this.mCategories = new HashSet<String>(o.mCategories);
//    }
    if (o->mExtras != NULL) {
        CBundle::New(o->mExtras, (IBundle**)&mExtras);
    }

    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean all)
{
    AutoPtr<CIntent> o = (CIntent*)intent;
    mAction = String::Duplicate(o->mAction);
    mData = o->mData;
    mType = String::Duplicate(o->mType);
    mCapsule = String::Duplicate(o->mCapsule);
    AutoPtr<IComponentName> componentName;
    o->GetComponent((IComponentName**)&componentName);
    if (componentName) {
        String capsuleName;
        String className;
        componentName->GetCapsuleName(&capsuleName);
        componentName->GetClassName(&className);
        CComponentName::NewByFriend(capsuleName, className, (CComponentName**)&mComponent);
        String::Free(capsuleName);
        String::Free(className);
    }

//    if (o.mCategories != null) {
//        this.mCategories = new HashSet<String>(o.mCategories);
//    }

    return NOERROR;
}

ECode CIntent::constructor(
    /* [in] */ String action)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::constructor(
    /* [in] */ String action,
    /* [in] */ IUri* uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::constructor(
    /* [in] */ Handle32 intent)
{
    return NOERROR;
}

/**
 * Make a clone of only the parts of the Intent that are relevant for
 * filter matching: the action, data, type, component, and categories.
 */
ECode CIntent::CloneFilter(
    /* [out] */ IIntent** intent)
{
    if (intent == NULL) return E_INVALID_ARGUMENT;

    return CIntent::New(this, FALSE, intent);
}

ECode CIntent::GetComponent(
    /* [out] */ IComponentName** component)
{
    if (component == NULL) return E_INVALID_ARGUMENT;

    *component = (IComponentName*)(CComponentName*)mComponent;
    if (*component != NULL) (*component)->AddRef();

    return NOERROR;
}

ECode CIntent::SetComponent(
    /* [in] */ IComponentName* component)
{
    mComponent = (CComponentName*)component;

    return NOERROR;
}

ECode CIntent::GetAction(
    /* [out] */ String *pAction)
{
    *pAction = String::Duplicate(mAction);

    return NOERROR;
}

ECode CIntent::SetAction(
    /* [in] */ String action)
{
    mAction = String::Duplicate(action);

    return NOERROR;
}

ECode CIntent::GetData(
    /* [out] */ IUri** data)
{
    *data = mData;
    if (*data != NULL) (*data)->AddRef();

    return NOERROR;
}

ECode CIntent::SetData(
    /* [in] */ IUri* data)
{
    mData = data;

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

ECode CIntent::GetFlags(
    /* [out] */ Int32* flags)
{
    if (flags == NULL) return E_INVALID_ARGUMENT;

    *flags = mFlags;

    return NOERROR;
}

ECode CIntent::GetScheme(
    /* [out] */ String* scheme)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::AddCategory(
    /* [in] */ String category)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::HasCategory(
    /* [in] */ String category,
    /* [out] */ Boolean* hasCategory)
{
    if (hasCategory == NULL) return E_INVALID_ARGUMENT;

    *hasCategory = FALSE;
    if (mCategories != NULL) {
        Set<String>::Iterator it;
        for (it = mCategories->Begin(); it != mCategories->End(); ++it) {
            if (!(*it).Compare(category)) {
                *hasCategory = TRUE;
                break;
            }
        }
    }
    return NOERROR;
}

ECode CIntent::GetCategories(
    /* [out, callee] */ ArrayOf<String>** categories)
{
    if (categories == NULL) return E_INVALID_ARGUMENT;

    *categories = NULL;
    if (mCategories != NULL) {
        *categories = ArrayOf<String>::Alloc(mCategories->GetSize());
        Set<String>::Iterator it;
        Int32 idx = 0;
        for (it = mCategories->Begin();
             it != mCategories->End(); ++it, ++idx) {
            (**categories)[idx] = *it;
        }
    }
    return NOERROR;
}

ECode CIntent::HasFileDescriptors(
    /* [out] */ Boolean* hasFD)
{
    if (hasFD == NULL) return E_INVALID_ARGUMENT;

    *hasFD = FALSE;
    if (mExtras != NULL) {
        return mExtras->HasFileDescriptors(hasFD);
    }

    return NOERROR;
}

ECode CIntent::PutBooleanExtra(
    /* [in] */ String name,
    /* [in] */ Boolean value)
{
    ECode ec = NOERROR;

    if (mExtras == NULL) {
        ec = CBundle::New((IBundle**)&mExtras);
    }

    ec = mExtras->PutBoolean(name, value);

    return ec;
}

ECode CIntent::GetBooleanExtra(
    /* [in] */ String name,
    /* [out] */ Boolean *pValue)
{
    ECode ec = NOERROR;

    if (mExtras == NULL) return E_DOES_NOT_EXIST;
    ec = mExtras->GetBoolean(name, pValue);

    return ec;
}

ECode CIntent::PutByteExtra(
    /* [in] */ String name,
    /* [in] */ Byte value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetByteExtra(
    /* [in] */ String name,
    /* [out] */ Byte * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutCharExtra(
    /* [in] */ String name,
    /* [in] */ Char16 value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetCharExtra(
    /* [in] */ String name,
    /* [out] */ Char16 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutInt16Extra(
    /* [in] */ String name,
    /* [in] */ Int16 value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetInt16Extra(
    /* [in] */ String name,
    /* [out] */ Int16 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutInt32Extra(
    /* [in] */ String name,
    /* [in] */ Int32 value)
{
    ECode ec = NOERROR;

    if (mExtras == NULL) {
        ec = CBundle::New((IBundle**)&mExtras);
    }

    ec = mExtras->PutInt32(name, value);

    return ec;
}

ECode CIntent::GetInt32Extra(
    /* [in] */ String name,
    /* [out] */ Int32 *pValue)
{
    ECode ec = NOERROR;

    if (mExtras == NULL) return E_DOES_NOT_EXIST;
    ec = mExtras->GetInt32(name, pValue);

    return ec;
}

ECode CIntent::PutInt64Extra(
    /* [in] */ String name,
    /* [in] */ Int64 value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetInt64Extra(
    /* [in] */ String name,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutFloatExtra(
    /* [in] */ String name,
    /* [in] */ Float value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetFloatExtra(
    /* [in] */ String name,
    /* [out] */ Float * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutDoubleExtra(
    /* [in] */ String name,
    /* [in] */ Double value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetDoubleExtra(
    /* [in] */ String name,
    /* [out] */ Double * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutStringExtra(
    /* [in] */ String name,
    /* [in] */ String value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetStringExtra(
    /* [in] */ String name,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutExtras(
    /* [in] */ IBundle * pExtras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetExtras(
    /* [out] */ IBundle ** ppValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::PutParcelableExtra(
    /* [in] */ String name,
    /* [in] */ IParcelable* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetParcelableExtra(
    /* [in] */ String name,
    /* [out] */ IParcelable** value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetStringArrayExtra(
    /* [in] */ String name,
    /* [out, callee] */ ArrayOf<String>** array)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::GetCapsule(
    /* [out] */ String* capsuleName)
{
    if (capsuleName == NULL) return E_INVALID_ARGUMENT;

    *capsuleName = String::Duplicate(mCapsule);

    return NOERROR;
}

ECode CIntent::SetCapsule(
    /* [in] */ String capsuleName)
{
    mCapsule = String::Duplicate(capsuleName);

    return NOERROR;
}

ECode CIntent::SetClassName(
    /* [in] */ String capsuleName,
    /* [in] */ String className)
{
    return CComponentName::NewByFriend(
            capsuleName, className, (CComponentName**)&mComponent);
}

ECode CIntent::ResolveActivityInfo(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** info)
{
    return E_NOT_IMPLEMENTED;
}

/**
 * Return the MIME data type of this intent.  If the type field is
 * explicitly set, that is simply returned.  Otherwise, if the data is set,
 * the type of that data is returned.  If neither fields are set, a null is
 * returned.
 *
 * @param resolver A ContentResolver that can be used to determine the MIME
 *                 type of the intent's data.
 *
 * @return The MIME type of this intent.
 *
 * @see #getType
 * @see #resolveType(Context)
 */
ECode CIntent::ResolveType(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ String* type)
{
    if (type == NULL) return E_INVALID_ARGUMENT;

    if (!mType.IsNull()) {
        *type = String::Duplicate(mType);
        return NOERROR;
    }
    if (mData != NULL) {
//        if ("content".equals(mData.getScheme())) {
//            return resolver.getType(mData);
//        }
    }
    *type = NULL;
    return NOERROR;
}

/**
 * Return the MIME data type of this intent, only if it will be needed for
 * intent resolution.  This is not generally useful for application code;
 * it is used by the frameworks for communicating with back-end system
 * services.
 *
 * @param resolver A ContentResolver that can be used to determine the MIME
 *                 type of the intent's data.
 *
 * @return The MIME type of this intent, or null if it is unknown or not
 *         needed.
 */
ECode CIntent::ResolveTypeIfNeeded(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ String* type)
{
    if (type == NULL) return E_INVALID_ARGUMENT;

    if (mComponent != NULL) {
        *type = String::Duplicate(mType);
        return NOERROR;
    }

    return ResolveType(resolver, type);
}

/**
 * Determine if two intents are the same for the purposes of intent
 * resolution (filtering). That is, if their action, data, type,
 * class, and categories are the same.  This does <em>not</em> compare
 * any extra data included in the intents.
 *
 * @param other The other Intent to compare against.
 *
 * @return Returns true if action, data, type, class, and categories
 *         are the same.
 */
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
//            if (!mCategories.equals(other.mCategories)) {
//                return false;
//            }
//        } else {
//            if (!other.mCategories.equals(mCategories)) {
//                return false;
//            }
//        }
//    }

    *isEqual = TRUE;
    return NOERROR;
}

/**
 * Generate hash code that matches semantics of IsFilterEquals().
 *
 * @return Returns the hash value of the action, data, type, class, and
 *         categories.
 *
 * @see #IsFilterEquals
 */
ECode CIntent::GetFilterHashCode(
    /* [out] */ Int32* hashCode)
{
    if (hashCode == NULL) return E_INVALID_ARGUMENT;

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
        mComponent->GetHashCode(hashCode);
        code += *hashCode;
    }
//    if (mCategories != null) {
//        code += mCategories.hashCode();
//    }
    *hashCode = code;
    return NOERROR;
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

    Int32 size;
    source->ReadInt32(&size);
    if (size > 0) {
        CBundle::ReadFromParcel(source, size, (IBundle**)&mExtras);
    }

    return NOERROR;
}

ECode CIntent::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteString(mCapsule);
    CComponentName::WriteToParcel(mComponent, dest);
    dest->WriteString(mAction);

    if (mExtras != NULL) {
        CBundle::WriteToParcel(mExtras, dest);
    }
    else {
        dest->WriteInt32(-1);
    }

    return NOERROR;
}

ECode CIntent::CloneFilter(
    /* [out] */ CIntent** intent)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntent::ReplaceExtras(
    /* [in] */ CIntent* src)
{
    return E_NOT_IMPLEMENTED;
}
