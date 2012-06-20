
#include "content/CComponentName.h"
#include <StringBuffer.h>

using namespace Elastos::System;

CComponentName::CComponentName()
{}

CComponentName::~CComponentName()
{
    String::Free(mCapsule);
    String::Free(mClass);
}

ECode CComponentName::constructor()
{
    return NOERROR;
}

ECode CComponentName::constructor(
    /* [in] */ String capsuleName,
    /* [in] */ String className)
{
    mCapsule = String::Duplicate(capsuleName);
    mClass = String::Duplicate(className);

    return NOERROR;
}

ECode CComponentName::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    if (capsuleName == NULL) return E_INVALID_ARGUMENT;

    *capsuleName = String::Duplicate(mCapsule);

    return NOERROR;
}

ECode CComponentName::GetClassName(
    /* [out] */ String* className)
{
    if (className == NULL) return E_INVALID_ARGUMENT;

    *className = String::Duplicate(mClass);

    return NOERROR;
}

/**
 * Return the class name, either fully qualified or in a shortened form
 * (with a leading '.') if it is a suffix of the package.
 */
String CComponentName::GetShortClassName()
{
    if (strstr(mClass, mCapsule) != NULL) {
        Int32 PN = mCapsule.GetLength();
        Int32 CN = mClass.GetLength();
        if (CN > PN && mClass.GetChar(PN) == '.') {
            return mClass.Substring(PN + 1);
        }
    }

    return mClass;
}

/**
 * Return a String that unambiguously describes both the package and
 * class names contained in the ComponentName.  You can later recover
 * the ComponentName from this string through
 * {@link #unflattenFromString(String)}.
 *
 * @return Returns a new String holding the package and class names.  This
 * is represented as the package name, concatenated with a '/' and then the
 * class name.
 *
 * @see #unflattenFromString(String)
 */
ECode CComponentName::FlattenToString(
    /* [out] */ String* name)
{
    StringBuffer sb;
    sb = sb + mCapsule + "/" + mClass;
    *name = String::Duplicate(sb);
    return NOERROR;
}

/**
 * The same as {@link #flattenToString()}, but abbreviates the class
 * name if it is a suffix of the package.  The result can still be used
 * with {@link #unflattenFromString(String)}.
 *
 * @return Returns a new String holding the package and class names.  This
 * is represented as the package name, concatenated with a '/' and then the
 * class name.
 *
 * @see #unflattenFromString(String)
 */
ECode CComponentName::FlattenToShortString(
    /* [out] */ String* name)
{
    if (name == NULL) return E_INVALID_ARGUMENT;

    String shortName = GetShortClassName();
    char* s = (char*)malloc(mCapsule.GetLength() +
            1 /* "/" */ + shortName.GetLength() + 1);
    strcpy(s, mCapsule);
    strcat(s, "/");
    strcat(s, shortName);
    *name = s;
    return NOERROR;
}

ECode CComponentName::Equals(
    /* [in] */ IComponentName* other,
    /* [out] */ Boolean* isEqual)
{
    if (isEqual == NULL) return E_INVALID_ARGUMENT;

    *isEqual = FALSE;
    if (other != NULL) {
        *isEqual = (!mCapsule.Compare(((CComponentName*)other)->mCapsule) &&
                    !mClass.Compare(((CComponentName*)other)->mClass));
    }

    return NOERROR;
}

ECode CComponentName::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    if (hashCode == NULL) return E_INVALID_ARGUMENT;

    *hashCode = (Int32)HashString(mCapsule) * 5 +
            (Int32)HashString(mClass);
    return NOERROR;
}

ECode CComponentName::GetShortDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CComponentName::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CComponentName::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    String capsuleName, className;

    source->ReadString(&capsuleName);
    if (!capsuleName.IsNull()) {
        source->ReadString(&className);
    }

    return NOERROR;
}

ECode CComponentName::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteString(mCapsule);
    dest->WriteString(mClass);

    return NOERROR;
}

ECode CComponentName::ReadFromParcel(
    /* [in] */ IParcel *source,
    /* [out] */ CComponentName** component)
{
    String capsuleName, className;

    assert(component != NULL);
    source->ReadString(&capsuleName);
    if (!capsuleName.IsNull()) {
        source->ReadString(&className);
        CComponentName::NewByFriend(capsuleName, className, component);
    }
    else {
        *component = NULL;
    }

    return NOERROR;
}

ECode CComponentName::WriteToParcel(
    /* [in] */ CComponentName* component,
    /* [in] */ IParcel *dest)
{
    if (component != NULL) {
        component->WriteToParcel(dest);
    }
    else {
        dest->WriteString(NULL);
    }

    return NOERROR;
}
