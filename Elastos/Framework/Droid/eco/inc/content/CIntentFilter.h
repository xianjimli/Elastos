
#ifndef __CINTENTFILTER_H__
#define __CINTENTFILTER_H__

#include "_CIntentFilter.h"
#include "content/IntentFilter.h"


CarClass(CIntentFilter), public IntentFilter
{
public:
    using IntentFilter::MatchAction;

    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    CARAPI AddAction(
        /* [in] */ const String& action);

    CARAPI CountActions(
        /* [out] */ Int32 *count);

    CARAPI GetAction(
        /* [in] */ Int32 index,
        /* [out] */ String *action);

    CARAPI HasAction(
        /* [in] */ const String& action,
        /* [out] */ Boolean* hasAction);

    CARAPI MatchAction(
        /* [in] */ const String& action,
        /* [out] */ Boolean* isMatched);

    CARAPI AddDataType(
        /* [in] */ const String& type);

    CARAPI HasDataType(
        /* [in] */ const String& type,
        /* [out] */ Boolean* hasDataType);

    CARAPI CountDataTypes(
        /* [out] */ Int32* count);

    CARAPI GetDataType(
        /* [in] */ Int32 index,
        /* [out] */ String* type);

    CARAPI AddDataScheme(
        /* [in] */ const String& scheme);

    CARAPI CountDataSchemes(
        /* [out] */ Int32* count);

    CARAPI GetDataScheme(
        /* [in] */ Int32 index,
        /* [out] */ String* scheme);

    CARAPI HasDataScheme(
        /* [in] */ const String& scheme,
        /* [out] */ Boolean* result);

    CARAPI AddDataAuthority(
        /* [in] */ const String& host,
        /* [in] */ const String& port);

    CARAPI CountDataAuthorities(
        /* [out] */ Int32* count);

    CARAPI GetDataAuthority(
        /* [in] */ Int32 index,
        /* [out] */ IAuthorityEntry** authority);

    CARAPI HasDataAuthority(
        /* [in] */ IUri* data,
        /* [out] */ Boolean* result);

    CARAPI AddDataPath(
        /* [in] */ const String& path,
        /* [in] */ Int32 type);

    CARAPI CountDataPaths(
        /* [out] */ Int32* count);

    CARAPI GetDataPath(
        /* [in] */ Int32 index,
        /* [out] */ IPatternMatcher** path);

    CARAPI HasDataPath(
        /* [in] */ const String& data,
        /* [out] */ Boolean* result);

    CARAPI MatchDataAuthority(
        /* [in] */ IUri* data,
        /* [out] */ Int32* result);

    CARAPI MatchData(
        /* [in] */ const String& type,
        /* [in] */ const String& scheme,
        /* [in] */ IUri* data,
        /* [out] */ Int32* result);

    CARAPI AddCategory(
        /* [in] */ const String& category);

    CARAPI CountCategories(
        /* [out] */ Int32* count);

    CARAPI GetCategory(
        /* [in] */ Int32 index,
        /* [out] */ String* category);

    CARAPI HasCategory(
        /* [in] */ const String& category,
        /* [out] */ Boolean* hasCategory);

    CARAPI MatchCategories(
        /* [in] */ ArrayOf<String>* categories,
        /* [out] */ String* result);

    CARAPI Match(
        /* [in] */ const String& action,
        /* [in] */ const String& type,
        /* [in] */ const String& scheme,
        /* [in] */ IUri* data,
        /* [in] */ ArrayOf<String>* categories,
        /* [in] */ CString logTag,
        /* [out] */ Int32* result);

    CARAPI MatchEx(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean resolve,
        /* [in] */ CString logTag,
        /* [out] */ Int32* result);

    CARAPI WriteToXml(
        /* [in] */ IXmlSerializer* serializer);

    CARAPI ReadFromXml(
        /* [in] */ IXmlPullParser* parser);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& action);

    CARAPI constructor(
        /* [in] */ const String& action,
        /* [in] */ const String& dataType);

    CARAPI constructor(
        /* [in] */ IIntentFilter* o);

public:
    CARAPI_(List<String>*) GetActions();

    CARAPI_(List<String>*) GetSchemes();

    CARAPI_(List<String>*) GetTypes();
};

#endif // __CINTENTFILTER_H__
