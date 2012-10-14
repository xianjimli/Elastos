
#ifndef __INTENTRESOLVER_H__
#define __INTENTRESOLVER_H__

#include "ext/frameworkdef.h"
#include <elastos/Set.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>

using namespace Elastos;

template <typename F, typename R>
class IntentResolver
{
public:
    IntentResolver();

    ~IntentResolver();

    CARAPI_(void) AddFilter(F* filter);

    CARAPI_(void) RemoveFilter(
        /* [in] */ F* f);

    /**
     * Returns a read-only set of the filters.
     */
    Set<F*>* FilterSet() {
//	        return Collections.unmodifiableSet(mFilters);
        return mFilters;
    }

    CARAPI_(List<R*>*) QueryIntentFromList(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Boolean defaultOnly,
        /* [in] */ List<List<F*>*>* listCut);

    CARAPI_(List<R*>*) QueryIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Boolean defaultOnly);

protected:
    /**
     * Control whether the given filter is allowed to go into the result
     * list.  Mainly intended to prevent adding multiple filters for the
     * same target object.
     */
    virtual CARAPI_(Boolean) AllowFilterResult(
        /* [in] */ F* filter,
        /* [in] */ List<R*>* dest)
    {
        return TRUE;
    }

    virtual CARAPI_(String) CapsuleForFilter(
        /* [in] */ F* filter);

    virtual CARAPI NewResult(
        /* [in] */ F* f,
        /* [in] */ Int32 match,
        /* [out] */ R** ret);

    virtual CARAPI_(void) SortResults(
        /* [in] */ List<R*>* results);


    virtual CARAPI DumpFilter(
        /* [in] */ IPrintWriter* out,
        /* [in] */ const String& prefix,
        /* [in] */ F* filter);

private:
    CARAPI_(void) RemoveFilterInternal(
        /* [in] */ F* f);

    CARAPI_(Int32) RegisterMimeTypes(
        /* [in] */ F* f,
        /* [in] */ const char* prefix);

    CARAPI_(Int32) UnregisterMimeTypes(
        /* [in] */ F* f,
        /* [in] */ const char* prefix);

    CARAPI_(Int32) RegisterIntentFilter(
        /* [in] */ F* f,
        /* [in] */ ArrayOf<String>* slist,
        /* [in] */ HashMap<String, List<F*>*>* dest,
        /* [in] */ const char* prefix);

    CARAPI_(Int32) UnregisterIntentFilter(
        /* [in] */ F* f,
        /* [in] */ ArrayOf<String>* slist,
        /* [in] */ HashMap<String, List<F*>*>* dest,
        /* [in] */ const char* prefix);

    CARAPI_(Boolean) RemoveAllObjects(
        /* [in] */ List<F*>* list,
        /* [in] */ F* f);

    CARAPI_(void) BuildResolveList(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean debug,
        /* [in] */ Boolean defaultOnly,
        /* [in] */ const String& resolvedType,
        /* [in] */ const String& scheme,
        /* [in] */ List<F*>* src,
        /* [in] */ List<R*>* dest);

    CARAPI_(void) FreeStringArray(
        /* [in] */ ArrayOf<String>* strings);

private:
    /**
     * All filters that have been registered.
     */
    Set<F*>* mFilters;

    /**
     * All of the MIME types that have been registered, such as "image/jpeg",
     * "image/ *", or "{@literal *}/ *".
     */
    HashMap<String, List<F*>*> mTypeToFilter;

    /**
     * The base names of all of all fully qualified MIME types that have been
     * registered, such as "image" or "*".  Wild card MIME types such as
     * "image/ *" will not be here.
     */
    HashMap<String, List<F*>*> mBaseTypeToFilter;

     /**
     * The base names of all of the MIME types with a sub-type wildcard that
     * have been registered.  For example, a filter with "image/ *" will be
     * included here as "image" but one with "image/jpeg" will not be
     * included here.  This also includes the "*" for the "{@literal *}/ *"
     * MIME type.
     */
    HashMap<String, List<F*>*> mWildTypeToFilter;

    /**
     * All of the URI schemes (such as http) that have been registered.
     */
    HashMap<String, List<F*>*> mSchemeToFilter;

    /**
     * All of the actions that have been registered, but only those that did
     * not specify data.
     */
    HashMap<String, List<F*>*> mActionToFilter;

    /**
     * All of the actions that have been registered and specified a MIME type.
     */
    HashMap<String, List<F*>*> mTypedActionToFilter;
};

template <typename F, typename R>
IntentResolver<F, R>::IntentResolver() :
    mTypeToFilter(5),
    mBaseTypeToFilter(5),
    mWildTypeToFilter(5),
    mSchemeToFilter(5),
    mActionToFilter(5),
    mTypedActionToFilter(5)
{
    mFilters = new Set<F*>();
}

template <typename F, typename R>
IntentResolver<F, R>::~IntentResolver()
{
    mFilters->Clear();
    delete mFilters;

    mTypeToFilter.Clear();
    mBaseTypeToFilter.Clear();
    mWildTypeToFilter.Clear();
    mSchemeToFilter.Clear();
    mActionToFilter.Clear();
    mTypedActionToFilter.Clear();
}

template <typename F, typename R>
void IntentResolver<F, R>::AddFilter(
    /* [in] */ F* f)
{
    mFilters->Insert(f);

    ArrayOf<String>* schemes = NULL;
    ArrayOf<String>* actions = NULL;
    f->mFilter->GetSchemes(&schemes);
    Int32 numS = RegisterIntentFilter(
            f,
            schemes,
            &mSchemeToFilter,
            "    Scheme: ");
    Int32 numT = RegisterMimeTypes(f, "    Type: ");
    if (numS == 0 && numT == 0) {
        f->mFilter->GetActions(&actions);
        RegisterIntentFilter(
                f,
                actions,
                &mActionToFilter,
                "    Action: ");
    }
    if (numT != 0) {
        f->mFilter->GetActions(&actions);
        RegisterIntentFilter(
                f,
                actions,
                &mTypedActionToFilter,
                "    TypedAction: ");
    }
    FreeStringArray(schemes);
    FreeStringArray(actions);
}

template <typename F, typename R>
void IntentResolver<F, R>::RemoveFilter(
    /* [in] */ F* f)
{
    RemoveFilterInternal(f);
    mFilters->Erase(f);
}

template <typename F, typename R>
void IntentResolver<F, R>::RemoveFilterInternal(
    /* [in] */ F* f)
{
    ArrayOf<String>* schemes = NULL;
    ArrayOf<String>* actions = NULL;
    f->mFilter->GetSchemes(&schemes);
    Int32 numS = UnregisterIntentFilter(
            f,
            schemes,
            &mSchemeToFilter,
            "      Scheme: ");
    Int32 numT = UnregisterMimeTypes(f, "      Type: ");
    if (numS == 0 && numT == 0) {
        f->mFilter->GetActions(&actions);
        UnregisterIntentFilter(
                f,
                actions,
                &mActionToFilter,
                "      Action: ");
    }
    if (numT != 0) {
        f->mFilter->GetActions(&actions);
        UnregisterIntentFilter(
                f,
                actions,
                &mTypedActionToFilter,
                "      TypedAction: ");
    }
    FreeStringArray(schemes);
    FreeStringArray(actions);
}

template <typename F, typename R>
List<R*>* IntentResolver<F, R>::QueryIntentFromList(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ List<List<F*>*>* listCut)
{
    List<R*>* resultList = new List<R*>();

    Boolean debug = FALSE;
//    Boolean debug = localLOGV ||
//                ((intent.getFlags() & Intent_Flag_Debug_LogResolution) != 0);

    String scheme;
    intent->GetScheme(&scheme);
    typename List<List<F*>*>::Iterator it;
    for (it = listCut->Begin(); it != listCut->End(); ++it) {
        BuildResolveList(intent, debug, defaultOnly, resolvedType,
                scheme, *it, resultList);
    }
    SortResults(resultList);
    return resultList;
}

template <typename F, typename R>
List<R*>* IntentResolver<F, R>::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly)
{
    String scheme(NULL);
    intent->GetScheme(&scheme);

    List<R*>* finalList = new List<R*>();

    Boolean debug = FALSE;
//    Boolean debug = localLOGV ||
//                ((intent.getFlags() & Intent.FLAG_DEBUG_LOG_RESOLUTION) != 0);

//    if (debug) Log.v(
//            TAG, "Resolving type " + resolvedType + " scheme " + scheme
//            + " of intent " + intent);

    List<F*>* firstTypeCut = NULL;

    String action;
    intent->GetAction(&action);
    if (resolvedType.IsNull() && !action.IsNull()) {
        typename HashMap<String, List<F*>*>::Iterator it = mActionToFilter.Find(action);
        if (it != mActionToFilter.End()) {
            firstTypeCut = it->mSecond;
        }
    }

    if (firstTypeCut != NULL) {
        BuildResolveList(intent, debug, defaultOnly, resolvedType,
                scheme, firstTypeCut, finalList);
    }
    SortResults(finalList);

    return finalList;
}

template <typename F, typename R>
String IntentResolver<F, R>::CapsuleForFilter(
    /* [in] */ F* filter)
{
    return String(NULL);
}

template <typename F, typename R>
ECode IntentResolver<F, R>::DumpFilter(
    /* [in] */ IPrintWriter* out,
    /* [in] */ const String& prefix,
    /* [in] */ F* filter)
{
//	    out.print(prefix); out.println(filter);
    return E_NOT_IMPLEMENTED;
}

template <typename F, typename R>
ECode IntentResolver<F, R>::NewResult(
    /* [in] */ F* f,
    /* [in] */ Int32 match,
    /* [out] */ R** ret)
{
    assert(ret != NULL);
    *ret = (R*)f;
    return NOERROR;
}

template <typename F, typename R>
void IntentResolver<F, R>::SortResults(
    /* [in] */ List<R*>* results)
{
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::RegisterMimeTypes(
    /* [in] */ F* f,
    /* [in] */ const char* prefix)
{
    ArrayOf<String>* types = NULL;
    f->mFilter->GetTypes(&types);
    if (types == NULL) return 0;

    if (types->GetLength() == 0) {
        return 0;
    }

    Int32 i;
    for(i = 0; i < types->GetLength(); ++i) {
        const String& str = (*types)[i];
        String name, baseName;
        Int32 slashpos = str.IndexOf('/');
        if (slashpos > 0) {
            name = str;
            baseName = name.Substring(0, slashpos);
        }
        else {
            baseName = str;
            name = baseName + "/*";
        }

        List<F*>* array = NULL;
        typename HashMap<String, List<F*>*>::Iterator it = mTypeToFilter.Find(name);
        if (it != mTypeToFilter.End()) {
            array = it->mSecond;
        }
        if (array == NULL) {
            array = new List<F*>();
            mTypeToFilter[name] = array;
        }
        array->PushBack(f);

        if (slashpos > 0) {
            array = NULL;
            it = mBaseTypeToFilter.Find(baseName);
            if (it != mBaseTypeToFilter.End()) {
                array = it->mSecond;
            }
            if (array == NULL) {
                array = new List<F*>();
                mBaseTypeToFilter[baseName] = array;
            }
            array->PushBack(f);
        }
        else {
            array = NULL;
            it = mWildTypeToFilter.Find(baseName);
            if (it != mWildTypeToFilter.End()) {
                array = it->mSecond;
            }
            if (array == NULL) {
                array = new List<F*>();
                mWildTypeToFilter[baseName] = array;
            }
            array->PushBack(f);
        }
    }
    FreeStringArray(types);
    return i;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::UnregisterMimeTypes(
    /* [in] */ F* f,
    /* [in] */ const char* prefix)
{
    ArrayOf<String>* types = NULL;
    f->mFilter->GetTypes(&types);
    if (types == NULL) return 0;

    if (types->GetLength() == 0) {
        return 0;
    }

    Int32 i;
    for(i = 0; i < types->GetLength(); ++i) {
        const String& str = (*types)[i];
        String name, baseName;
        Int32 slashpos = str.IndexOf('/');
        if (slashpos > 0) {
            name = str;
            baseName = name.Substring(0, slashpos);
        }
        else {
            baseName = str;
            name = baseName + "/*";
        }

        List<F*>* array = NULL;
        typename HashMap<String, List<F*>*>::Iterator it = mTypeToFilter.Find(name);
        if (it != mTypeToFilter.End()) {
            array = it->mSecond;
        }
        if (!RemoveAllObjects(array, f)) {
            mTypeToFilter.Erase(name);
        }

        if (slashpos > 0) {
            array = NULL;
            it = mBaseTypeToFilter.Find(baseName);
            if (it != mBaseTypeToFilter.End()) {
                array = it->mSecond;
            }
            if (!RemoveAllObjects(array, f)) {
                mBaseTypeToFilter.Erase(baseName);
            }
        }
        else {
            array = NULL;
            it = mWildTypeToFilter.Find(baseName);
            if (it != mWildTypeToFilter.End()) {
                array = it->mSecond;
            }
            if (!RemoveAllObjects(array, f)) {
                mWildTypeToFilter.Erase(baseName);
            }
        }
    }
    FreeStringArray(types);
    return i;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::RegisterIntentFilter(
    /* [in] */ F* f,
    /* [in] */ ArrayOf<String>* slist,
    /* [in] */ HashMap<String, List<F*>*>* dest,
    /* [in] */ const char* prefix)
{
    if (slist == NULL) return 0;

    if (slist->GetLength() == 0) {
        return 0;
    }

    Int32 i;
    for(i = 0; i < slist->GetLength(); ++i) {
        const String& name = (*slist)[i];
        List<F*>* array = NULL;
        typename HashMap<String, List<F*>*>::Iterator it = dest->Find(name);
        if (it != dest->End()) {
            array = it->mSecond;
        }
        if (array == NULL) {
            array = new List<F*>();
            (*dest)[name] = array;
        }
        array->PushBack(f);
    }
    return i;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::UnregisterIntentFilter(
    /* [in] */ F* f,
    /* [in] */ ArrayOf<String>* slist,
    /* [in] */ HashMap<String, List<F*>*>* dest,
    /* [in] */ const char* prefix)
{
    if (slist == NULL) return 0;

    if (slist->GetLength() == 0) {
        return 0;
    }

    Int32 i;
    for(i = 0; i < slist->GetLength(); ++i) {
        const String& name = (*slist)[i];
        List<F*>* array = NULL;
        typename HashMap<String, List<F*>*>::Iterator it = dest->Find(name);
        if (it != dest->End()) {
            array = it->mSecond;
        }
        if (!RemoveAllObjects(array, f)) {
            dest->Erase(name);
        }
    }
    return i;
}

template <typename F, typename R>
Boolean IntentResolver<F, R>::RemoveAllObjects(
    /* [in] */ List<F*>* list,
    /* [in] */ F* f)
{
    if (list != NULL) {
        typename List<F*>::Iterator it1 = list->Begin();
        typename List<F*>::Iterator it2 = list->End();
        for(;it1 != it2; ++it1) {
            if (*it1 == f) {
                list->Erase(it1);
                it2 = list->End();
            }
        }
        return list->GetSize() > 0;
    }
    return FALSE;
}

template <typename F, typename R>
void IntentResolver<F, R>::BuildResolveList(
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean debug,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ const String& resolvedType,
    /* [in] */ const String& scheme,
    /* [in] */ List<F*>* src,
    /* [in] */ List<R*>* dest)
{
    ArrayOf<String>* categories;
    intent->GetCategories(&categories);

    Boolean hasNonDefaults = FALSE;
    if (src != NULL) {
        for(typename List<F*>::Iterator next = src->Begin();
            next != src->End(); ++next) {

            F* f = *next;
//            if (debug) Log.v(TAG, "Matching against filter " + filter);

            // Do we already have this one?
            if (!AllowFilterResult(f, dest)) {
//                if (debug) {
//                    Log.v(TAG, "  Filter's target already added");
//                }
                continue;
            }

            String action;
            intent->GetAction(&action);
            AutoPtr<IUri> data;
            intent->GetData((IUri**)&data);

            Int32 match;
            f->mFilter->Match(action, resolvedType,
                    scheme, data, categories, "IntentResolver", &match);
            if (match >= 0) {
//                if (debug) Log.v(TAG, "  Filter matched!  match=0x" +
//                        Integer.toHexString(match));
                Boolean hasCategory;
                if (!defaultOnly || (f->mFilter->HasCategory(
                        String(Intent_CATEGORY_DEFAULT), &hasCategory), hasCategory)) {
                    R* oneResult;
                    NewResult(f, match, &oneResult);
                    if (oneResult != NULL) {
                        dest->PushBack(oneResult);
                    }
                }
                else {
                    hasNonDefaults = TRUE;
                }
            }
            else {
//                if (debug) {
//                    String reason;
//                    switch (match) {
//                        case IntentFilter.NO_MATCH_ACTION: reason = "action"; break;
//                        case IntentFilter.NO_MATCH_CATEGORY: reason = "category"; break;
//                        case IntentFilter.NO_MATCH_DATA: reason = "data"; break;
//                        case IntentFilter.NO_MATCH_TYPE: reason = "type"; break;
//                        default: reason = "unknown reason"; break;
//                    }
//                    Log.v(TAG, "  Filter did not match: " + reason);
//                }
            }
        }
    }

    if (dest->GetSize() == 0 && hasNonDefaults) {
//        Log.w(TAG, "resolveIntent failed: found match, but none with Intent.CATEGORY_DEFAULT");
    }
}

template <typename F, typename R>
void IntentResolver<F, R>::FreeStringArray(
    /* [in] */ ArrayOf<String>* strings)
{
    if (strings == NULL) return;

    for (Int32 i = 0; i < strings->GetLength(); ++i) {
        (*strings)[i].~String();
    }
    ArrayOf<String>::Free(strings);
}

#endif //__INTENTRESOLVER_H__
