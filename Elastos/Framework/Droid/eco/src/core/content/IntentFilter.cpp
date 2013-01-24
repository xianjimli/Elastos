
#include "content/IntentFilter.h"
#ifdef _FRAMEWORK_CORE
#include "content/CAuthorityEntry.h"
#include "os/CPatternMatcher.h"
#elif defined(_FRAMEWORK_SERVER)
#include "Elastos.Framework.Core.h"
#endif
#include "utils/AutoStringArray.h"
#include "utils/XmlUtils.h"
#include <elastos/Algorithm.h>


const CString IntentFilter::SGLOB_STR = "sglob";
const CString IntentFilter::PREFIX_STR = "prefix";
const CString IntentFilter::LITERAL_STR = "literal";
const CString IntentFilter::PATH_STR = "path";
const CString IntentFilter::PORT_STR = "port";
const CString IntentFilter::HOST_STR = "host";
const CString IntentFilter::AUTH_STR = "auth";
const CString IntentFilter::SCHEME_STR = "scheme";
const CString IntentFilter::TYPE_STR = "type";
const CString IntentFilter::CAT_STR = "cat";
const CString IntentFilter::NAME_STR = "name";
const CString IntentFilter::ACTION_STR = "action";

/**
 * New empty IntentFilter.
 */
IntentFilter::IntentFilter()
    : mPriority(0)
    , mCategories(NULL)
    , mDataSchemes(NULL)
    , mDataAuthorities(NULL)
    , mDataPaths(NULL)
    , mDataTypes(NULL)
    , mHasPartialTypes(FALSE)
{
    mActions = new List<String>();
}

/**
 * New IntentFilter that matches a single action with no data.  If
 * no data characteristics are subsequently specified, then the
 * filter will only match intents that contain no data.
 *
 * @param action The action to match, i.e. Intent.ACTION_MAIN.
 */
IntentFilter::IntentFilter(
    /* [in] */ const String& action)
    : mPriority(0)
    , mCategories(NULL)
    , mDataSchemes(NULL)
    , mDataAuthorities(NULL)
    , mDataPaths(NULL)
    , mDataTypes(NULL)
    , mHasPartialTypes(FALSE)
{
    mActions = new List<String>();
    AddAction(action);
}

/**
 * New IntentFilter that matches a single action and data type.
 *
 * <p><em>Note: MIME type matching in the Android framework is
 * case-sensitive, unlike formal RFC MIME types.  As a result,
 * you should always write your MIME types with lower case letters,
 * and any MIME types you receive from outside of Android should be
 * converted to lower case before supplying them here.</em></p>
 *
 * <p>Throws {@link MalformedMimeTypeException} if the given MIME type is
 * not syntactically correct.
 *
 * @param action The action to match, i.e. Intent.ACTION_VIEW.
 * @param dataType The type to match, i.e. "vnd.android.cursor.dir/person".
 *
 */
IntentFilter::IntentFilter(
    /* [in] */ const String& action,
    /* [in] */ const String& dataType)
    : mPriority(0)
    , mCategories(NULL)
    , mDataSchemes(NULL)
    , mDataAuthorities(NULL)
    , mDataPaths(NULL)
    , mDataTypes(NULL)
    , mHasPartialTypes(FALSE)
{
    mActions = new List<String>();
    AddAction(action);
    AddDataType(dataType);
}

/**
 * New IntentFilter containing a copy of an existing filter.
 *
 * @param o The original filter to copy.
 */
IntentFilter::IntentFilter(
    /* [in] */ IIntentFilter* o)
    : mActions(NULL)
    , mCategories(NULL)
    , mDataSchemes(NULL)
    , mDataAuthorities(NULL)
    , mDataPaths(NULL)
    , mDataTypes(NULL)
    , mHasPartialTypes(FALSE)
{
//    mPriority = o.mPriority;
//    mActions = new ArrayList<String>(o.mActions);
//    if (o.mCategories != null) {
//        mCategories = new ArrayList<String>(o.mCategories);
//    }
//    if (o.mDataTypes != null) {
//        mDataTypes = new ArrayList<String>(o.mDataTypes);
//    }
//    if (o.mDataSchemes != null) {
//        mDataSchemes = new ArrayList<String>(o.mDataSchemes);
//    }
//    if (o.mDataAuthorities != null) {
//        mDataAuthorities = new ArrayList<AuthorityEntry>(o.mDataAuthorities);
//    }
//    if (o.mDataPaths != null) {
//        mDataPaths = new ArrayList<PatternMatcher>(o.mDataPaths);
//    }
//    mHasPartialTypes = o.mHasPartialTypes;
}

IntentFilter::~IntentFilter()
{
    if (mActions != NULL) {
        mActions->Clear();
        delete mActions;
    }
    if (mCategories != NULL) {
        mCategories->Clear();
        delete mCategories;
    }
    if (mDataSchemes != NULL) {
        mDataSchemes->Clear();
        delete mDataSchemes;
    }
    if (mDataAuthorities != NULL) {
        mDataAuthorities->Clear();
        delete mDataAuthorities;
    }
    if (mDataPaths != NULL) {
        mDataPaths->Clear();
        delete mDataPaths;
    }
    if (mDataTypes != NULL) {
        mDataTypes->Clear();
        delete mDataTypes;
    }
}

/**
 * Modify priority of this filter.  The default priority is 0. Positive
 * values will be before the default, lower values will be after it.
 * Applications must use a value that is larger than
 * {@link #SYSTEM_LOW_PRIORITY} and smaller than
 * {@link #SYSTEM_HIGH_PRIORITY} .
 *
 * @param priority The new priority value.
 *
 * @see #getPriority
 * @see #SYSTEM_LOW_PRIORITY
 * @see #SYSTEM_HIGH_PRIORITY
 */
ECode IntentFilter::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

/**
 * Return the priority of this filter.
 *
 * @return The priority of the filter.
 *
 * @see #setPriority
 */
Int32 IntentFilter::GetPriority()
{
    return mPriority;
}

/**
 * Add a new Intent action to match against.  If any actions are included
 * in the filter, then an Intent's action must be one of those values for
 * it to match.  If no actions are included, the Intent action is ignored.
 *
 * @param action Name of the action to match, i.e. Intent.ACTION_VIEW.
 */
ECode IntentFilter::AddAction(
    /* [in] */ const String& action)
{
    VALIDATE_STRING_NOT_NULL(action);

    List<String>::Iterator it = Find(mActions->Begin(), mActions->End(), action);
    if (it == mActions->End()) {
        mActions->PushBack(action);
    }

    return NOERROR;
}

/**
 * Return the number of actions in the filter.
 */
Int32 IntentFilter::CountActions()
{
    return (Int32)mActions->GetSize();
}

/**
 * Return an action in the filter.
 */
String IntentFilter::GetAction(
    /* [in] */ Int32 index)
{
    if (index < 0) return String(NULL);

    List<String>::Iterator it = mActions->Begin();
    for (; it != mActions->End(), index > 0; ++it) {}
    if (it != mActions->End()) {
        return *it;
    }
    return String(NULL);
}

Boolean IntentFilter::HasAction(
    /* [in] */ const String& action)
{
    return Find(mActions->Begin(), mActions->End(), action)
            != mActions->End();
}

Boolean IntentFilter::MatchAction(
    /* [in] */ const String& action)
{
    if (action.IsNull() || mActions == NULL ||
            mActions->Begin() == mActions->End()) {
        return FALSE;
    }
    return Find(mActions->Begin(), mActions->End(), action)
            != mActions->End();
}

ECode IntentFilter::GetActions(
    /* [out, callee] */ ArrayOf<String>** actions)
{
    if (mActions == NULL) {
        *actions = NULL;
        return NOERROR;
    }

    Int32 size = mActions->GetSize();
    *actions = ArrayOf<String>::Alloc(size);

    List<String>::Iterator it = mActions->Begin();
    Int32 i = 0;
    for (; it != mActions->End(); ++it, ++i) {
        (**actions)[i] = *it;
    }
    return NOERROR;
}

/**
 * Add a new Intent data type to match against.  If any types are
 * included in the filter, then an Intent's data must be <em>either</em>
 * one of these types <em>or</em> a matching scheme.  If no data types
 * are included, then an Intent will only match if it specifies no data.
 *
 * <p><em>Note: MIME type matching in the Android framework is
 * case-sensitive, unlike formal RFC MIME types.  As a result,
 * you should always write your MIME types with lower case letters,
 * and any MIME types you receive from outside of Android should be
 * converted to lower case before supplying them here.</em></p>
 *
 * <p>Throws {@link MalformedMimeTypeException} if the given MIME type is
 * not syntactically correct.
 *
 * @param type Name of the data type to match, i.e. "vnd.android.cursor.dir/person".
 *
 * @see #matchData
 */
ECode IntentFilter::AddDataType(
    /* [in] */ const String& type)
{
    Int32 slashpos = type.IndexOf('/');
    Int32 typelen = type.GetLength();
    if (slashpos > 0 && typelen >= slashpos + 2) {
        if (mDataTypes == NULL) mDataTypes = new List<String>();
        if (typelen == slashpos + 2 && type.GetChar(slashpos + 1) == '*') {
            String str = type.Substring(0, slashpos);
            List<String>::Iterator it =
                    Find(mDataTypes->Begin(), mDataTypes->End(), str);
            if (it == mDataTypes->End()) {
                mDataTypes->PushBack(str);
            }
            mHasPartialTypes = TRUE;
        }
        else {
            List<String>::Iterator it =
                    Find(mDataTypes->Begin(), mDataTypes->End(), type);
            if (it == mDataTypes->End()) {
                mDataTypes->PushBack(type);
            }
        }
        return NOERROR;
    }

    return E_MALFORMED_MIME_TYPE_EXCEPTION;
}

Boolean IntentFilter::HasDataType(
    /* [in] */ const String& type)
{
    return mDataTypes != NULL && FindMimeType(type);
}

/**
 * Return the number of data types in the filter.
 */
Int32 IntentFilter::CountDataTypes()
{
    return mDataTypes != NULL ? mDataTypes->GetSize() : 0;
}

String IntentFilter::GetDataType(
    /* [in] */ Int32 index)
{
    if (index < 0) return String(NULL);

    List<String>::Iterator it = mDataTypes->Begin();
    for (; it != mDataTypes->End(), index > 0; ++it) {}
    if (it != mDataTypes->End()) {
        return *it;
    }
    return String(NULL);
}

ECode IntentFilter::GetTypes(
        /* [out, callee] */ ArrayOf<String>** types)
{
    if (mDataTypes == NULL) {
        *types = NULL;
        return NOERROR;
    }

    Int32 size = mDataTypes->GetSize();
    *types = ArrayOf<String>::Alloc(size);

    List<String>::Iterator it = mDataTypes->Begin();
    Int32 i = 0;
    for (; it != mDataTypes->End(); ++it, ++i) {
        (**types)[i] = *it;
    }
    return NOERROR;
}

/**
 * Add a new Intent data scheme to match against.  If any schemes are
 * included in the filter, then an Intent's data must be <em>either</em>
 * one of these schemes <em>or</em> a matching data type.  If no schemes
 * are included, then an Intent will match only if it includes no data.
 *
 * <p><em>Note: scheme matching in the Android framework is
 * case-sensitive, unlike formal RFC schemes.  As a result,
 * you should always write your schemes with lower case letters,
 * and any schemes you receive from outside of Android should be
 * converted to lower case before supplying them here.</em></p>
 *
 * @param scheme Name of the scheme to match, i.e. "http".
 *
 * @see #matchData
 */
ECode IntentFilter::AddDataScheme(
    /* [in] */ const String& scheme)
{
    if (mDataSchemes == NULL) mDataSchemes = new List<String>();
    List<String>::Iterator it =
            Find(mDataSchemes->Begin(), mDataSchemes->End(), scheme);
    if (it == mDataSchemes->End()) {
        mDataSchemes->PushBack(scheme);
    }
    return NOERROR;
}

/**
 * Return the number of data schemes in the filter.
 */
Int32 IntentFilter::CountDataSchemes()
{
    return mDataSchemes != NULL ? mDataSchemes->GetSize() : 0;
}

String IntentFilter::GetDataScheme(
    /* [in] */ Int32 index)
{
    if (index < 0) return String(NULL);

    List<String>::Iterator it = mDataSchemes->Begin();
    for (; it != mDataSchemes->End(), index > 0; ++it) {}
    if (it != mDataSchemes->End()) {
        return *it;
    }
    return String(NULL);
}

Boolean IntentFilter::HasDataScheme(
    /* [in] */ const String& scheme)
{
    return mDataSchemes != NULL && Find(mDataSchemes->Begin(),
            mDataSchemes->End(), scheme) != mDataSchemes->End();
}

ECode IntentFilter::GetSchemes(
        /* [out, callee] */ ArrayOf<String>** schemes)
{
    if (mDataSchemes == NULL) {
        *schemes = NULL;
        return NOERROR;
    }

    Int32 size = mDataSchemes->GetSize();
    *schemes = ArrayOf<String>::Alloc(size);

    List<String>::Iterator it = mDataSchemes->Begin();
    Int32 i = 0;
    for (; it != mDataSchemes->End(); ++it, ++i) {
        (**schemes)[i] = *it;
    }
    return NOERROR;
}

/**
 * Add a new Intent data authority to match against.  The filter must
 * include one or more schemes (via {@link #addDataScheme}) for the
 * authority to be considered.  If any authorities are
 * included in the filter, then an Intent's data must match one of
 * them.  If no authorities are included, then only the scheme must match.
 *
 * <p><em>Note: host name in the Android framework is
 * case-sensitive, unlike formal RFC host names.  As a result,
 * you should always write your host names with lower case letters,
 * and any host names you receive from outside of Android should be
 * converted to lower case before supplying them here.</em></p>
 *
 * @param host The host part of the authority to match.  May start with a
 *             single '*' to wildcard the front of the host name.
 * @param port Optional port part of the authority to match.  If null, any
 *             port is allowed.
 *
 * @see #matchData
 * @see #addDataScheme
 */
ECode IntentFilter::AddDataAuthority(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    if (mDataAuthorities == NULL) {
        mDataAuthorities = new List< AutoPtr<IAuthorityEntry> >();
    }
    AutoPtr<IAuthorityEntry> authEntry;
    CAuthorityEntry::New(host, port, (IAuthorityEntry**)&authEntry);
    mDataAuthorities->PushBack(authEntry);
    return NOERROR;
}

/**
 * Return the number of data authorities in the filter.
 */
Int32 IntentFilter::CountDataAuthorities()
{
    return mDataAuthorities != NULL ? mDataAuthorities->GetSize() : 0;
}

AutoPtr<IAuthorityEntry> IntentFilter::GetDataAuthority(
    /* [in] */ Int32 index)
{
    if (index < 0) return NULL;

    List< AutoPtr<IAuthorityEntry> >::Iterator it = mDataAuthorities->Begin();
    for (; it != mDataAuthorities->End(), index > 0; ++it) {}
    if (it != mDataAuthorities->End()) {
        return *it;
    }
    return NULL;
}

Boolean IntentFilter::HasDataAuthority(
    /* [in] */ IUri* data)
{
    return MatchDataAuthority(data) >= 0;
}

/**
 * Add a new Intent data oath to match against.  The filter must
 * include one or more schemes (via {@link #addDataScheme}) <em>and</em>
 * one or more authorities (via {@link #addDataAuthority}) for the
 * path to be considered.  If any paths are
 * included in the filter, then an Intent's data must match one of
 * them.  If no paths are included, then only the scheme/authority must
 * match.
 *
 * <p>The path given here can either be a literal that must directly
 * match or match against a prefix, or it can be a simple globbing pattern.
 * If the latter, you can use '*' anywhere in the pattern to match zero
 * or more instances of the previous character, '.' as a wildcard to match
 * any character, and '\' to escape the next character.
 *
 * @param path Either a raw string that must exactly match the file
 * path, or a simple pattern, depending on <var>type</var>.
 * @param type Determines how <var>path</var> will be compared to
 * determine a match: either {@link PatternMatcher#PATTERN_LITERAL},
 * {@link PatternMatcher#PATTERN_PREFIX}, or
 * {@link PatternMatcher#PATTERN_SIMPLE_GLOB}.
 *
 * @see #matchData
 * @see #addDataScheme
 * @see #addDataAuthority
 */
ECode IntentFilter::AddDataPath(
    /* [in] */ const String& path,
    /* [in] */ Int32 type)
{
    if (mDataPaths == NULL) {
        mDataPaths = new List< AutoPtr<IPatternMatcher> >();
    }
    AutoPtr<IPatternMatcher> pm;
    CPatternMatcher::New(path, type, (IPatternMatcher**)&pm);
    mDataPaths->PushBack(pm);
    return NOERROR;
}

/**
 * Return the number of data paths in the filter.
 */
Int32 IntentFilter::CountDataPaths()
{
    return mDataPaths != NULL ? mDataPaths->GetSize() : 0;
}

AutoPtr<IPatternMatcher> IntentFilter::GetDataPath(
    /* [in] */ Int32 index)
{
    if (index < 0) return NULL;

    List< AutoPtr<IPatternMatcher> >::Iterator it = mDataPaths->Begin();
    for (; it != mDataPaths->End(), index > 0; ++it) {}
    if (it != mDataPaths->End()) {
        return *it;
    }
    return NULL;
}

Boolean IntentFilter::HasDataPath(
    /* [in] */ const String& data)
{
    if (mDataPaths == NULL) {
        return FALSE;
    }
    List< AutoPtr<IPatternMatcher> >::Iterator it = mDataPaths->Begin();
    while (it != mDataPaths->End()) {
        Boolean isMatched;
        IPatternMatcher* pe = *it;
        pe->Match(data, &isMatched);
        if (isMatched) {
            return TRUE;
        }
        ++it;
    }
    return FALSE;
}

Int32 IntentFilter::MatchDataAuthority(
    /* [in] */ IUri* data)
{
    if (mDataAuthorities == NULL) {
        return IntentFilter_NO_MATCH_DATA;
    }
    List< AutoPtr<IAuthorityEntry> >::Iterator it = mDataAuthorities->Begin();
    while (it != mDataAuthorities->End()) {
        IAuthorityEntry* ae = *it;
        Int32 match;
        ae->Match(data, &match);
        if (match >= 0) {
            return match;
        }
        ++it;
    }
    return IntentFilter_NO_MATCH_DATA;
}

Int32 IntentFilter::MatchData(
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data)
{
    List<String>* types = mDataTypes;
    List<String>* schemes = mDataSchemes;
    List< AutoPtr<IAuthorityEntry> >* authorities = mDataAuthorities;
    List< AutoPtr<IPatternMatcher> >* paths = mDataPaths;

    Int32 match = IntentFilter_MATCH_CATEGORY_EMPTY;

    if (types == NULL && schemes == NULL) {
        return ((type.IsNull() && data == NULL)
            ? (IntentFilter_MATCH_CATEGORY_EMPTY + IntentFilter_MATCH_ADJUSTMENT_NORMAL)
            : IntentFilter_NO_MATCH_DATA);
    }

    if (schemes != NULL) {
        if (Find(schemes->Begin(), schemes->End(),
                !scheme.IsNull() ? scheme : String("")) != schemes->End()) {
            match = IntentFilter_MATCH_CATEGORY_SCHEME;
        }
        else {
            return IntentFilter_NO_MATCH_DATA;
        }

        if (authorities != NULL) {
            Int32 authMatch = MatchDataAuthority(data);
            if (authMatch >= 0) {
                if (paths == NULL) {
                    match = authMatch;
                }
                else {
                    String path;
                    data->GetPath(&path);
                    if (HasDataPath(path)) {
                        match = IntentFilter_MATCH_CATEGORY_PATH;
                    }
                    else {
                        return IntentFilter_NO_MATCH_DATA;
                    }
                }
            }
            else {
                return IntentFilter_NO_MATCH_DATA;
            }
        }
    } else {
        // Special case: match either an Intent with no data URI,
        // or with a scheme: URI.  This is to give a convenience for
        // the common case where you want to deal with data in a
        // content provider, which is done by type, and we don't want
        // to force everyone to say they handle content: or file: URIs.
        if (!scheme.IsNull() && !scheme.Equals("")
                && !scheme.Equals("content")
                && !scheme.Equals("file")) {
            return IntentFilter_NO_MATCH_DATA;
        }
    }

    if (types != NULL) {
        if (FindMimeType(type)) {
            match = IntentFilter_MATCH_CATEGORY_TYPE;
        }
        else {
            return IntentFilter_NO_MATCH_TYPE;
        }
    }
    else {
        // If no MIME types are specified, then we will only match against
        // an Intent that does not have a MIME type.
        if (!type.IsNull()) {
            return IntentFilter_NO_MATCH_TYPE;
        }
    }

    return match + IntentFilter_MATCH_ADJUSTMENT_NORMAL;
}

/**
 * Add a new Intent category to match against.  The semantics of
 * categories is the opposite of actions -- an Intent includes the
 * categories that it requires, all of which must be included in the
 * filter in order to match.  In other words, adding a category to the
 * filter has no impact on matching unless that category is specified in
 * the intent.
 *
 * @param category Name of category to match, i.e. Intent.CATEGORY_EMBED.
 */
ECode IntentFilter::AddCategory(
    /* [in] */ const String& category)
{
    VALIDATE_STRING_NOT_NULL(category);

    if (mCategories == NULL) {
        mCategories = new List<String>();
    }

    List<String>::Iterator it =
            Find(mCategories->Begin(), mCategories->End(), category);
    if (it == mCategories->End()) {
        mCategories->PushBack(category);
    }

    return NOERROR;
}

/**
 * Return the number of categories in the filter.
 */
Int32 IntentFilter::CountCategories()
{
    return mCategories != NULL ? mCategories->GetSize() : 0;
}

/**
 * Return a category in the filter.
 */
String IntentFilter::GetCategory(
    /* [in] */ Int32 index)
{
    if (index < 0) return String(NULL);

    List<String>::Iterator it = mCategories->Begin();
    for (; it != mCategories->End(), index > 0; ++it) {}
    if (it != mCategories->End()) {
        return *it;
    }
    return String(NULL);
}

/**
 * Is the given category included in the filter?
 *
 * @param category The category that the filter supports.
 *
 * @return True if the category is explicitly mentioned in the filter.
 */
Boolean IntentFilter::HasCategory(
    /* [in] */ const String& category)
{
    return mCategories != NULL && Find(mCategories->Begin(),
            mCategories->End(), category) != mCategories->End();
}

String IntentFilter::MatchCategories(
    /* [in] */ ArrayOf<String>* categories)
{
    if (categories == NULL) {
        return String(NULL);
    }

    if (mCategories == NULL) {
        return categories->GetLength() > 0 ? (*categories)[0] : String(NULL);
    }

    for (Int32 i = 0; i < categories->GetLength(); ++i) {
        const String& category = (*categories)[i];
        if (Find(mCategories->Begin(), mCategories->End(), category)
                == mCategories->End()) {
            return category;
        }
    }

    return String(NULL);
}

Int32 IntentFilter::Match(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean resolve,
    /* [in] */ CString logTag)
{
    String action, type, scheme;
    intent->GetAction(&action);
    if (resolver != NULL) {
        intent->ResolveTypeEx(resolver, &type);
    }
    else {
        intent->GetType(&type);
    }
    intent->GetScheme(&scheme);
    AutoPtr<IUri> data;
    intent->GetData((IUri**)&data);
    AutoStringArray categories;
    intent->GetCategories((ArrayOf<String>**)&categories);
    return Match(action, type, scheme, data, categories, logTag);
}

/**
 * Test whether this filter matches the given intent data.  A match is
 * only successful if the actions and categories in the Intent match
 * against the filter, as described in {@link IntentFilter}; in that case,
 * the match result returned will be as per {@link #matchData}.
 *
 * @param action The intent action to match against (Intent.getAction).
 * @param type The intent type to match against (Intent.resolveType()).
 * @param scheme The data scheme to match against (Intent.getScheme()).
 * @param data The data URI to match against (Intent.getData()).
 * @param categories The categories to match against
 *                   (Intent.getCategories()).
 * @param logTag Tag to use in debugging messages.
 *
 * @return Returns either a valid match constant (a combination of
 * {@link #MATCH_CATEGORY_MASK} and {@link #MATCH_ADJUSTMENT_MASK}),
 * or one of the error codes {@link #NO_MATCH_TYPE} if the type didn't match,
 * {@link #NO_MATCH_DATA} if the scheme/path didn't match,
 * {@link #NO_MATCH_ACTION if the action didn't match, or
 * {@link #NO_MATCH_CATEGORY} if one or more categories didn't match.
 *
 * @see #matchData
 * @see Intent#getAction
 * @see Intent#resolveType
 * @see Intent#getScheme
 * @see Intent#getData
 * @see Intent#getCategories
 */
Int32 IntentFilter::Match(
    /* [in] */ const String& action,
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ CString logTag)
{
    if (!action.IsNull() && !MatchAction(action)) {
//        if (Config.LOGV) Log.v(
//            logTag, "No matching action " + action + " for " + this);
        return IntentFilter_NO_MATCH_ACTION;
    }

    Int32 dataMatch = MatchData(type, scheme, data);
    if (dataMatch < 0) {
//        if (Config.LOGV) {
//            if (dataMatch == NO_MATCH_TYPE) {
//                Log.v(logTag, "No matching type " + type
//                      + " for " + this);
//            }
//            if (dataMatch == NO_MATCH_DATA) {
//                Log.v(logTag, "No matching scheme/path " + data
//                      + " for " + this);
//            }
//        }
        return dataMatch;
    }

    String categoryMatch = MatchCategories(categories);
    if (!categoryMatch.IsNull()) {
//        if (Config.LOGV) Log.v(
//            logTag, "No matching category "
//            + categoryMatch + " for " + this);
        return IntentFilter_NO_MATCH_CATEGORY;
    }

    // It would be nice to treat container activities as more
    // important than ones that can be embedded, but this is not the way...
//    if (FALSE) {
//        if (categories != NULL) {
//            dataMatch -= mCategories.size() - categories.size();
//        }
//    }

    return dataMatch;
}

/**
 * Write the contents of the IntentFilter as an XML stream.
 */
ECode IntentFilter::WriteToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    List<String>::Iterator it;
    for (it = mActions->Begin(); it != mActions->End(); ++it) {
        FAIL_RETURN(serializer->WriteStartTag(NULL, ACTION_STR));
        FAIL_RETURN(serializer->WriteAttribute(NULL, NAME_STR, *it));
        FAIL_RETURN(serializer->WriteEndTag(NULL, ACTION_STR));
    }
    for (it = mCategories->Begin(); it != mCategories->End(); ++it) {
        FAIL_RETURN(serializer->WriteStartTag(NULL, CAT_STR));
        FAIL_RETURN(serializer->WriteAttribute(NULL, NAME_STR, *it));
        FAIL_RETURN(serializer->WriteEndTag(NULL, CAT_STR));
    }
    for (it = mDataTypes->Begin(); it != mDataTypes->End(); ++it) {
        FAIL_RETURN(serializer->WriteStartTag(NULL, TYPE_STR));
        String type = *it;
        if (type.IndexOf('/') < 0) type = type + "/*";
        FAIL_RETURN(serializer->WriteAttribute(NULL, NAME_STR, *it));
        FAIL_RETURN(serializer->WriteEndTag(NULL, TYPE_STR));
    }
    for (it = mDataSchemes->Begin(); it != mDataSchemes->End(); ++it) {
        FAIL_RETURN(serializer->WriteStartTag(NULL, SCHEME_STR));
        FAIL_RETURN(serializer->WriteAttribute(NULL, NAME_STR, *it));
        FAIL_RETURN(serializer->WriteEndTag(NULL, SCHEME_STR));
    }

    List< AutoPtr<IAuthorityEntry> >::Iterator daIt;
    for (daIt = mDataAuthorities->Begin(); daIt != mDataAuthorities->End(); ++daIt) {
        FAIL_RETURN(serializer->WriteStartTag(NULL, AUTH_STR));
        IAuthorityEntry* ae = *daIt;
        String host;
        Int32 port;
        ae->GetHost(&host);
        ae->GetPort(&port);
        FAIL_RETURN(serializer->WriteAttribute(NULL, HOST_STR, host));
        if (port >= 0) {
            FAIL_RETURN(serializer->WriteAttribute(NULL, PORT_STR, String::FromInt32(port)));
        }
        FAIL_RETURN(serializer->WriteEndTag(NULL, AUTH_STR));
    }

    List< AutoPtr<IPatternMatcher> >::Iterator dpIt;
    for (dpIt = mDataPaths->Begin(); dpIt != mDataPaths->End(); ++dpIt) {
        FAIL_RETURN(serializer->WriteStartTag(NULL, PATH_STR));
        IPatternMatcher* pe = *dpIt;
        Int32 type;
        pe->GetType(&type);
        String path;
        switch (type) {
            case PatternMatcher_PATTERN_LITERAL:
                pe->GetPath(&path);
                FAIL_RETURN(serializer->WriteAttribute(NULL, LITERAL_STR, path));
                break;
            case PatternMatcher_PATTERN_PREFIX:
                pe->GetPath(&path);
                FAIL_RETURN(serializer->WriteAttribute(NULL, PREFIX_STR, path));
                break;
            case PatternMatcher_PATTERN_SIMPLE_GLOB:
                pe->GetPath(&path);
                FAIL_RETURN(serializer->WriteAttribute(NULL, SGLOB_STR, path));
                break;
        }
        FAIL_RETURN(serializer->WriteEndTag(NULL, PATH_STR));
    }
    return NOERROR;
}

ECode IntentFilter::ReadFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type, depth;
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        FAIL_RETURN(parser->GetName(&tagName));
        if (tagName.Equals(ACTION_STR)) {
            String name;
            FAIL_RETURN(parser->GetAttributeValueEx(NULL, NAME_STR, &name));
            if (!name.IsNull()) {
                AddAction(name);
            }
        }
        else if (tagName.Equals(CAT_STR)) {
            String name;
            FAIL_RETURN(parser->GetAttributeValueEx(NULL, NAME_STR, &name));
            if (!name.IsNull()) {
                AddCategory(name);
            }
        }
        else if (tagName.Equals(TYPE_STR)) {
            String name;
            FAIL_RETURN(parser->GetAttributeValueEx(NULL, NAME_STR, &name));
            if (!name.IsNull()) {
//                try {
                AddDataType(name);
//                } catch (MalformedMimeTypeException e) {
//                }
            }
        }
        else if (tagName.Equals(SCHEME_STR)) {
            String name;
            FAIL_RETURN(parser->GetAttributeValueEx(NULL, NAME_STR, &name));
            if (!name.IsNull()) {
                AddDataScheme(name);
            }
        }
        else if (tagName.Equals(AUTH_STR)) {
            String host, port;
            FAIL_RETURN(parser->GetAttributeValueEx(NULL, HOST_STR, &host));
            FAIL_RETURN(parser->GetAttributeValueEx(NULL, PORT_STR, &port));
            if (!host.IsNull()) {
                AddDataAuthority(host, port);
            }
        }
        else if (tagName.Equals(PATH_STR)) {
            String path;
            FAIL_RETURN(parser->GetAttributeValueEx(NULL, LITERAL_STR, &path));
            if (!path.IsNull()) {
                AddDataPath(path, PatternMatcher_PATTERN_LITERAL);
            }
            else {
                FAIL_RETURN(parser->GetAttributeValueEx(NULL, PREFIX_STR, &path));
                if (!path.IsNull()) {
                    AddDataPath(path, PatternMatcher_PATTERN_PREFIX);
                }
                else {
                    FAIL_RETURN(parser->GetAttributeValueEx(NULL, SGLOB_STR, &path));
                    if (!path.IsNull()) {
                        AddDataPath(path, PatternMatcher_PATTERN_SIMPLE_GLOB);
                    }
                }
            }
        }
        else {
//            Log.w("IntentFilter", "Unknown tag parsing IntentFilter: " + tagName);
        }
        XmlUtils::SkipCurrentTag(parser);
    }
    return NOERROR;
}

ECode IntentFilter::ReadFromParcel(
    /* [in] */ IParcel *source)
{
//    mActions = new ArrayList<String>();
//    source.readStringList(mActions);
//    if (source.readInt() != 0) {
//        mCategories = new ArrayList<String>();
//        source.readStringList(mCategories);
//    }
//    if (source.readInt() != 0) {
//        mDataSchemes = new ArrayList<String>();
//        source.readStringList(mDataSchemes);
//    }
//    if (source.readInt() != 0) {
//        mDataTypes = new ArrayList<String>();
//        source.readStringList(mDataTypes);
//    }
//    int N = source.readInt();
//    if (N > 0) {
//        mDataAuthorities = new ArrayList<AuthorityEntry>();
//        for (int i=0; i<N; i++) {
//            mDataAuthorities.add(new AuthorityEntry(source));
//        }
//    }
//    N = source.readInt();
//    if (N > 0) {
//        mDataPaths = new ArrayList<PatternMatcher>();
//        for (int i=0; i<N; i++) {
//            mDataPaths.add(new PatternMatcher(source));
//        }
//    }
//    mPriority = source.readInt();
//    mHasPartialTypes = source.readInt() > 0;
    return E_NOT_IMPLEMENTED;
}

ECode IntentFilter::WriteToParcel(
    /* [in] */ IParcel *dest)
{
//    dest.writeStringList(mActions);
//    if (mCategories != null) {
//        dest.writeInt(1);
//        dest.writeStringList(mCategories);
//    } else {
//        dest.writeInt(0);
//    }
//    if (mDataSchemes != null) {
//        dest.writeInt(1);
//        dest.writeStringList(mDataSchemes);
//    } else {
//        dest.writeInt(0);
//    }
//    if (mDataTypes != null) {
//        dest.writeInt(1);
//        dest.writeStringList(mDataTypes);
//    } else {
//        dest.writeInt(0);
//    }
//    if (mDataAuthorities != null) {
//        final int N = mDataAuthorities.size();
//        dest.writeInt(N);
//        for (int i=0; i<N; i++) {
//            mDataAuthorities.get(i).writeToParcel(dest);
//        }
//    } else {
//        dest.writeInt(0);
//    }
//    if (mDataPaths != null) {
//        final int N = mDataPaths.size();
//        dest.writeInt(N);
//        for (int i=0; i<N; i++) {
//            mDataPaths.get(i).writeToParcel(dest, 0);
//        }
//    } else {
//        dest.writeInt(0);
//    }
//    dest.writeInt(mPriority);
//    dest.writeInt(mHasPartialTypes ? 1 : 0);
    return E_NOT_IMPLEMENTED;
}

ECode IntentFilter::Init(
    /* [in] */ const String& action)
{
    return AddAction(action);
}

ECode IntentFilter::Init(
    /* [in] */ const String& action,
    /* [in] */ const String& dataType)
{
    FAIL_RETURN(AddAction(action));
    return AddDataType(dataType);
}

ECode IntentFilter::Init(
    /* [in] */ IIntentFilter* o)
{
    return E_NOT_IMPLEMENTED;
}

Boolean IntentFilter::FindMimeType(
    /* [in] */ const String& type)
{
    List<String>* t = mDataTypes;

    if (type.IsNull()) {
        return FALSE;
    }

    if (Find(t->Begin(), t->End(), type) != t->End()) {
        return TRUE;
    }

    // Deal with an Intent wanting to match every type in the IntentFilter.
    const Int32 typeLength = type.GetCharCount();
    if (typeLength == 3 && type.Equals("*/*")) {
        return t->Begin() != t->End();
    }

    // Deal with this IntentFilter wanting to match every Intent type.
    if (mHasPartialTypes &&
            Find(t->Begin(), t->End(), String("*")) != t->End()) {
        return TRUE;
    }

    const Int32 slashpos = type.IndexOf('/');
    if (slashpos > 0) {
        if (mHasPartialTypes &&
                Find(t->Begin(), t->End(), type.Substring(0, slashpos))
                != t->End()) {
            return TRUE;
        }
        if (typeLength == slashpos + 2 && type.GetChar(slashpos + 1) == '*') {
            // Need to look through all types for one that matches
            // our base...
            List<String>::Iterator it = t->Begin();
            while (it != t->End()) {
                const String& v = *it;
                if (type.IndexOf(v.Substring(0, slashpos + 1)) == 0) { //type.RegionMatches(0, v, 0, slashpos + 1)
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}
