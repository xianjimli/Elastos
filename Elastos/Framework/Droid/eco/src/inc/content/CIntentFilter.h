
#ifndef __CINTENTFILTER_H__
#define __CINTENTFILTER_H__

#include "_CIntentFilter.h"

#include "content/IntentFilter.h"
#include "ext/frameworkdef.h"
#include <elastos/List.h>

CarClass(CIntentFilter), public IntentFilter
{
public:
    CIntentFilter();

    ~CIntentFilter();

    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    CARAPI AddAction(
        /* [in] */ String action);

    CARAPI CountActions(
        /* [out] */ Int32 *count);

    CARAPI GetAction(
        /* [in] */ Int32 index,
        /* [out] */ String *action);

    CARAPI AddDataType(
        /* [in] */ String type);

    CARAPI AddDataScheme(
        /* [in] */ String scheme);

    CARAPI AddDataAuthority(
        /* [in] */ String host,
        /* [in] */ String port);

    CARAPI AddDataPath(
        /* [in] */ String path,
        /* [in] */ Int32 type);

    CARAPI AddCategory(
        /* [in] */ String category);

    CARAPI HasCategory(
        /* [in] */ String category,
        /* [out] */ Boolean* hasCategory);

    CARAPI Match(
        /* [in] */ String action,
        /* [in] */ String type,
        /* [in] */ String scheme,
        /* [in] */ IUri* data,
        /* [in] */ ArrayOf<String>* categories,
        /* [in] */ String logTag,
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ String action);

    CARAPI constructor(
        /* [in] */ String action,
        /* [in] */ String dataType);

    CARAPI constructor(
        /* [in] */ IIntentFilter* o);

public:
    CARAPI_(Boolean) MatchAction(
        /* [in] */ String action);

    CARAPI_(Int32) MatchData(
        /* [in] */ String type,
        /* [in] */ String scheme,
        /* [in] */ IUri* data);

    CARAPI_(String) MatchCategories(
        /* [in] */ ArrayOf<String>* categories);

    CARAPI_(List<String>*) GetActions();

    CARAPI_(List<String>*) GetSchemes();

    CARAPI_(List<String>*) GetTypes();

public:
    /**
     * Quality adjustment applied to the category of match that signifies
     * the default, base value; higher numbers improve the quality while
     * lower numbers reduce it.
     */
    static const Int32 Match_Adjustment_Normal = 0x8000;

    /**
     * The filter matched an intent that had no data specified.
     */
    static const Int32 Match_Category_Empty = 0x0100000;
    /**
     * The filter matched an intent with the same data URI scheme.
     */
    static const Int32 Match_Category_Scheme = 0x0200000;
    /**
     * The filter matched an intent with the same data URI scheme and
     * authority host.
     */
    static const Int32 Match_Category_Host = 0x0300000;
    /**
     * The filter matched an intent with the same data URI scheme and
     * authority host and port.
     */
    static const Int32 Match_Category_Port = 0x0400000;
    /**
     * The filter matched an intent with the same data URI scheme,
     * authority, and path.
     */
    static const Int32 Match_Category_Path = 0x0500000;
    /**
     * The filter matched an intent with the same data MIME type.
     */
    static const Int32 Match_Category_Type = 0x0600000;

    /**
     * The filter didn't match due to different MIME types.
     */
    static const Int32 No_Match_Type = -1;
    /**
     * The filter didn't match due to different data URIs.
     */
    static const Int32 No_Match_Data = -2;
    /**
     * The filter didn't match due to different actions.
     */
    static const Int32 No_Match_Action = -3;
    /**
     * The filter didn't match because it required one or more categories
     * that were not in the Intent.
     */
    static const Int32 No_Match_Category = -4;

private:
    Int32 mPriority;
    List<String>* mActions;
    List<String>* mCategories;
    List<String>* mDataSchemes;

    List<String>* mDataTypes;
};

#endif // __CINTENTFILTER_H__
