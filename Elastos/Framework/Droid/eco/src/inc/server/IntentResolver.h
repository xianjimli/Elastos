
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

    CARAPI_(List<R*>*) QueryIntentFromList(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Boolean defaultOnly,
        /* [in] */ List<List<F*>*>* listCut);

    CARAPI_(List<R*>*) QueryIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Boolean defaultOnly);

    /**
     * Returns a read-only set of the filters.
     */
    List<F*>* FilterSet() {
//	        return Collections.unmodifiableSet(mFilters);
        return NULL;
    }

    List<F*>* FilterIterator() {
        return NULL;
    }

protected:
    virtual CARAPI_(String) CapsuleForFilter(
        /* [in] */ F* filter);

    virtual CARAPI DumpFilter(
        /* [in] */ IPrintWriter* out,
        /* [in] */ String prefix,
        /* [in] */ F* filter);

private:
    virtual CARAPI NewResult(
        /* [in] */ F* f,
        /* [in] */ Int32 match,
        /* [out] */ R** ret);

    virtual CARAPI_(void) SortResults(
        /* [in] */ List<R*>* results);

    CARAPI_(void) RemoveFilterInternal(
        /* [in] */ F* f);

    CARAPI_(Int32) RegisterMimeTypes(
        /* [in] */ F* f,
        /* [in] */ String prefix);

    CARAPI_(Int32) UnregisterMimeTypes(
        /* [in] */ F* f,
        /* [in] */ String prefix);

    CARAPI_(Int32) RegisterIntentFilter(
        /* [in] */ F* f,
        /* [in] */ List<String>* slist,
        /* [in] */ HashMap<String, List<F*>*>* dest,
        /* [in] */ String prefix);

    CARAPI_(Int32) UnregisterIntentFilter(
        /* [in] */ F* f,
        /* [in] */ List<String>* slist,
        /* [in] */ HashMap<String, List<F*>*>* dest,
        /* [in] */ String prefix);

    CARAPI_(Boolean) RemoveAllObjects(
        /* [in] */ List<F*>* list,
        /* [in] */ F* f);

    CARAPI_(void) BuildResolveList(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean debug,
        /* [in] */ Boolean defaultOnly,
        /* [in] */ String resolvedType,
        /* [in] */ String scheme,
        /* [in] */ List<F*>* src,
        /* [in] */ List<R*>* dest);

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

    Int32 numS = RegisterIntentFilter(
            f,
            f->mFilter->GetSchemes(),
            &mSchemeToFilter,
            "    Scheme: ");
    Int32 numT = RegisterMimeTypes(f, "    Type: ");
    if (numS == 0 && numT == 0) {
        RegisterIntentFilter(
                f,
                f->mFilter->GetActions(),
                &mActionToFilter,
                "    Action: ");
    }
    if (numT != 0) {
        RegisterIntentFilter(
                f,
                f->mFilter->GetActions(),
                &mTypedActionToFilter,
                "    Action: ");
    }
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
    Int32 numS = UnregisterIntentFilter(
            f,
            f->mFilter->GetSchemes(),
            &mSchemeToFilter,
            "      Scheme: ");
    Int32 numT = UnregisterMimeTypes(f, "      Type: ");
    if (numS == 0 && numT == 0) {
        UnregisterIntentFilter(
                f,
                f->mFilter->GetActions(),
                &mActionToFilter,
                "      Action: ");
    }
    if (numT != 0) {
        UnregisterIntentFilter(
                f,
                f->mFilter->GetActions(),
                &mTypedActionToFilter,
                "      TypedAction: ");
    }
}

template <typename F, typename R>
List<R*>* IntentResolver<F, R>::QueryIntentFromList(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
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
    String::Free(scheme);
    return resultList;
}

template <typename F, typename R>
List<R*>* IntentResolver<F, R>::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ Boolean defaultOnly)
{
    String scheme = NULL;
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
        firstTypeCut = mActionToFilter[action];
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
    return NULL;
}

template <typename F, typename R>
ECode IntentResolver<F, R>::DumpFilter(
    /* [in] */ IPrintWriter* out,
    /* [in] */ String prefix,
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
    /* [in] */ String prefix)
{
    List<String>* types = f->mFilter->GetTypes();
    if (types == NULL) return 0;

    List<String>::Iterator it1 = types->Begin();
    List<String>::Iterator it2 = types->End();
    if (it1 == it2) {
        return 0;
    }

    Int32 num = 0;
    for(; it1 != it2; ++it1) {
        const char* str = (const char*)*it1;
        num++;
        char* name = NULL;
        char* baseName = NULL;
        Int32 slashpos = (strchr(str, '/') - str);
        if (slashpos > 0) {
            name = (char*)malloc(strlen(str) + 1);
            strcpy(name, str);
            baseName = (char*)malloc(slashpos + 1);
            strncpy(baseName, name, slashpos);
        } else {
            baseName = (char*)malloc(strlen(str) + 1);
            strcpy(baseName, str);
            name = (char*)malloc(strlen(str) + strlen("/*") + 1);
            strcpy(name, str);
            strcat(name, "/*");
        }

        List<F*>* array = mTypeToFilter[String(name)];
        if (array == NULL) {
            array = new List<F*>();
            mTypeToFilter[String::Duplicate(String(name))] = array;
        }
        array->PushBack(f);

        if (slashpos > 0) {
            array = mBaseTypeToFilter[String(baseName)];
            if (array == NULL) {
                array = new List<F*>();
                mBaseTypeToFilter[String::Duplicate(String(baseName))] = array;
            }
            array->PushBack(f);
        } else {
            array = mWildTypeToFilter[String(baseName)];
            if (array == NULL) {
                array = new List<F*>();
                mWildTypeToFilter[String::Duplicate(String(baseName))] = array;
            }
            array->PushBack(f);
        }

        free(name);
        free(baseName);
    }
    return num;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::UnregisterMimeTypes(
    /* [in] */ F* f,
    /* [in] */ String prefix)
{
    List<String>* types = f->mFilter->GetTypes();
    if (types == NULL) return 0;

    List<String>::Iterator it1 = types->Begin();
    List<String>::Iterator it2 = types->End();
    if (it1 == it2) {
        return 0;
    }

    Int32 num = 0;
    for(; it1 != it2; ++it1) {
        const char* str = (const char*)*it1;
        num++;
        char* name = NULL;
        char* baseName = NULL;
        Int32 slashpos = (strchr(str, '/') - str);
        if (slashpos > 0) {
            name = (char*)malloc(strlen(str) + 1);
            strcpy(name, str);
            baseName = (char*)malloc(slashpos + 1);
            strncpy(baseName, name, slashpos);
        } else {
            baseName = (char*)malloc(strlen(str) + 1);
            strcpy(baseName, str);
            name = (char*)malloc(strlen(str) + strlen("/*") + 1);
            strcpy(name, str);
            strcat(name, "/*");
        }

        if (!RemoveAllObjects(mTypeToFilter[String(name)], f)) {
            mTypeToFilter.Erase(String(name));
        }

        if (slashpos > 0) {
            if (!RemoveAllObjects(mBaseTypeToFilter[String(baseName)], f)) {
                mBaseTypeToFilter.Erase(String(baseName));
            }
        } else {
            if (!RemoveAllObjects(mWildTypeToFilter[String(baseName)], f)) {
                mWildTypeToFilter.Erase(String(baseName));
            }
        }

        free(name);
        free(baseName);
    }
    return num;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::RegisterIntentFilter(
    /* [in] */ F* f,
    /* [in] */ List<String>* slist,
    /* [in] */ HashMap<String, List<F*>*>* dest,
    /* [in] */ String prefix)
{
    if (slist == NULL) return 0;

    List<String>::Iterator itBegin = slist->Begin();
    List<String>::Iterator itEnd = slist->End();
    if (itBegin == itEnd) {
        return 0;
    }

    Int32 num = 0;
    for(; itBegin != itEnd; ++itBegin) {
        String name = *itBegin;
        num++;
        List<F*>* array = (*dest)[name];
        if (array == NULL) {
            array = new List<F*>();
            (*dest)[String::Duplicate(name)] = array;
        }
        array->PushBack(f);
    }
    return num;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::UnregisterIntentFilter(
    /* [in] */ F* f,
    /* [in] */ List<String>* slist,
    /* [in] */ HashMap<String, List<F*>*>* dest,
    /* [in] */ String prefix)
{
    if (slist == NULL) return 0;

    List<String>::Iterator itBegin = slist->Begin();
    List<String>::Iterator itEnd = slist->End();
    if (itBegin == itEnd) {
        return 0;
    }

    Int32 num = 0;
    for(; itBegin != itEnd; ++itBegin) {
        String name = *itBegin;
        num++;
        if (!RemoveAllObjects((*dest)[name], f)) {
            dest->Erase(name);
            itEnd = slist->End();
        }
    }
    return num;
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
    return false;
}

template <typename F, typename R>
void IntentResolver<F, R>::BuildResolveList(
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean debug,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ String resolvedType,
    /* [in] */ String scheme,
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
//            if (!allowFilterResult(filter, dest)) {
//                if (debug) {
//                    Log.v(TAG, "  Filter's target already added");
//                }
//                continue;
//            }

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
                        Intent_CATEGORY_DEFAULT, &hasCategory), hasCategory)) {
                    R* oneResult;
                    NewResult(f, match, &oneResult);
                    if (oneResult != NULL) {
                        dest->PushBack(oneResult);
                    }
                } else {
                    hasNonDefaults = TRUE;
                }
            } else {
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

#endif //__INTENTRESOLVER_H__
