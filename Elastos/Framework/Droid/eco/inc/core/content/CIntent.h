
#ifndef __CINTENT_H__
#define __CINTENT_H__

#include "ext/frameworkdef.h"
#include "_CIntent.h"
#include "content/CComponentName.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>

CarClass(CIntent)
{
public:
    /**
     * Wrapper class holding an Intent and implementing comparisons on it for
     * the purpose of filtering.  The class implements its
     * {@link #equals equals()} and {@link #hashCode hashCode()} methods as
     * simple calls to {@link Intent#filterEquals(Intent)}  filterEquals()} and
     * {@link android.content.Intent#filterHashCode()}  filterHashCode()}
     * on the wrapped Intent.
     */
    class FilterComparison
    {
    public:
        FilterComparison(
            /* [in] */ IIntent* intent);

        /**
         * Return the Intent that this FilterComparison represents.
         * @return Returns the Intent held by the FilterComparison.  Do
         * not modify!
         */
        CARAPI_(IIntent*) GetIntent();

        CARAPI_(Int32) GetHashCode();

        CARAPI_(Boolean) Equals(
            /* [in] */ FilterComparison* obj);

        CARAPI GetDescription(
            /* [out] */ String* description);

    private:
        AutoPtr<IIntent> mIntent;
        Int32 mHashCode;
    };

public:
    CIntent();

    ~CIntent();

    CARAPI CloneFilter(
        /* [out] */ IIntent** intent);

    CARAPI GetComponent(
        /* [out] */ IComponentName** component);

    CARAPI SetComponent(
        /* [in] */ IComponentName* component);

    CARAPI GetCapsule(
        /* [out] */ String* capsuleName);

    CARAPI SetCapsule(
        /* [in] */ const String& capsuleName);

    CARAPI GetAction(
        /* [out] */ String *pAction);

    CARAPI SetAction(
        /* [in] */ const String& action);

    CARAPI GetData(
        /* [out] */ IUri **pData);

    CARAPI SetData(
        /* [in] */ IUri *pData);
	
    CARAPI SetType(
        /*[in]*/ const String& type);

    CARAPI SetDataAndType(
	/*[in]*/ IUri* data,
	/*[in]*/ const String& type);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI AddFlags(
        /* [in] */ Int32 flags);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetClassName(
        /* [in] */ const String& capsuleName,
        /* [in] */ const String& className);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetType(
        /* [out] */ String* type);

    CARAPI AddCategory(
        /* [in] */ const String& category);

    CARAPI HasCategory(
        /* [in] */ const String& category,
        /* [out] */ Boolean* hasCategory);

    CARAPI GetCategories(
        /* [out, callee] */ ArrayOf<String>** categories);

    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* hasFD);

    CARAPI PutBooleanExtra(
        /* [in] */ const String& name,
        /* [out] */ Boolean value);

    CARAPI GetBooleanExtra(
        /* [in] */ const String& name,
        /* [out] */ Boolean *pValue);

    CARAPI PutByteExtra(
        /* [in] */ const String& name,
        /* [in] */ Byte value);

    CARAPI GetByteExtra(
        /* [in] */ const String& name,
        /* [out] */ Byte * pValue);

    CARAPI PutCharExtra(
        /* [in] */ const String& name,
        /* [in] */ Char16 value);

    CARAPI GetCharExtra(
        /* [in] */ const String& name,
        /* [out] */ Char16 * pValue);

    CARAPI PutInt16Extra(
        /* [in] */ const String& name,
        /* [in] */ Int16 value);

    CARAPI GetInt16Extra(
        /* [in] */ const String& name,
        /* [out] */ Int16 * pValue);

    CARAPI PutInt32Extra(
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    CARAPI GetInt32Extra(
        /* [in] */ const String& name,
        /* [out] */ Int32 *pValue);

    CARAPI PutInt64Extra(
        /* [in] */ const String& name,
        /* [in] */ Int64 value);

    CARAPI GetInt64Extra(
        /* [in] */ const String& name,
        /* [out] */ Int64 * pValue);

    CARAPI PutFloatExtra(
        /* [in] */ const String& name,
        /* [in] */ Float value);

    CARAPI GetFloatExtra(
        /* [in] */ const String& name,
        /* [out] */ Float * pValue);

    CARAPI PutDoubleExtra(
        /* [in] */ const String& name,
        /* [in] */ Double value);

    CARAPI GetDoubleExtra(
        /* [in] */ const String& name,
        /* [out] */ Double * pValue);

    CARAPI PutStringExtra(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    CARAPI GetStringExtra(
        /* [in] */ const String& name,
        /* [out] */ String * pValue);

    CARAPI PutExtras(
        /* [in] */ IBundle * pExtras);

    CARAPI GetExtras(
        /* [out] */ IBundle ** ppValue);

    CARAPI PutParcelableExtra(
        /* [in] */ const String& name,
        /* [in] */ IParcelable* value);

    CARAPI GetParcelableExtra(
        /* [in] */ const String& name,
        /* [out] */ IParcelable** value);

    CARAPI GetStringArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<String>** array);

    CARAPI ResolveActivityInfo(
        /* [in] */ ICapsuleManager* pm,
        /* [in] */ Int32 flags,
        /* [out] */ IActivityInfo** info);

    CARAPI ResolveType(
        /* [in] */ IContentResolver* resolver,
        /* [out] */ String* type);

    CARAPI ResolveTypeIfNeeded(
        /* [in] */ IContentResolver* resolver,
        /* [out] */ String* type);

    /**
     * Completely replace the extras in the Intent with the extras in the
     * given Intent.
     *
     * @param src The exact extras contained in this Intent are copied
     * into the target intent, replacing any that were previously there.
     */
    CARAPI ReplaceExtras(
        /* [in] */ IIntent* src);

    CARAPI FilterEquals(
        /* [in] */ IIntent* other,
        /* [out] */ Boolean* isEqual);

    CARAPI GetFilterHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IIntent* intent);

    CARAPI constructor(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean all);

    CARAPI constructor(
        /* [in] */ const String& action);

    CARAPI constructor(
        /* [in] */ const String& action,
        /* [in] */ IUri* uri);

    CARAPI constructor(
        /* [in] */ Handle32 intent);

private:
/*
    android intent:

    private String mAction;
    private Uri mData;
    private String mType;
    private String mPackage;
    private ComponentName mComponent;
    private Int32 mFlags;
    private HashSet<String> mCategories;
    private Bundle mExtras;
*/

    String mAction;
    AutoPtr<IUri> mData;
    String mType;
    String mCapsule;
    AutoPtr<CComponentName> mComponent;
    Int32 mFlags;
    Set<String>* mCategories;
    AutoPtr<IBundle> mExtras;
};

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<CIntent::FilterComparison*>
{
    size_t operator()(CIntent::FilterComparison* filter) const
    {
        assert(filter != NULL);
        return (size_t)filter->GetHashCode();
    }
};

template<> struct EqualTo<CIntent::FilterComparison*>
{
    Boolean operator()(CIntent::FilterComparison* x,
                       CIntent::FilterComparison* y) const
    {
        assert(x != NULL);
        return x->Equals(y);
    }
};

_ELASTOS_NAMESPACE_END

#endif // __CINTENT_H__
