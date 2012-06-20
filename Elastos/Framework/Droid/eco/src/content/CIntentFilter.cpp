
#include "content/CIntentFilter.h"


const Int32 CIntentFilter::Match_Adjustment_Normal;
const Int32 CIntentFilter::Match_Category_Empty;
const Int32 CIntentFilter::Match_Category_Scheme;
const Int32 CIntentFilter::Match_Category_Host;
const Int32 CIntentFilter::Match_Category_Port;
const Int32 CIntentFilter::Match_Category_Path;
const Int32 CIntentFilter::Match_Category_Type;
const Int32 CIntentFilter::No_Match_Type;
const Int32 CIntentFilter::No_Match_Data;
const Int32 CIntentFilter::No_Match_Action;
const Int32 CIntentFilter::No_Match_Category;

CIntentFilter::CIntentFilter()
{
}

CIntentFilter::~CIntentFilter()
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

    if (mDataTypes != NULL) {
        mDataTypes->Clear();
        delete mDataTypes;
    }
}

ECode CIntentFilter::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

ECode CIntentFilter::GetPriority(
    /* [out] */ Int32* priority)
{
    if (priority == NULL) return E_INVALID_ARGUMENT;

    *priority = mPriority;
    return NOERROR;
}

ECode CIntentFilter::AddAction(
    /* [in] */ String action)
{
    if (action.IsNull() || action.IsEmpty()) return NOERROR;

    if (mActions == NULL) {
        mActions = new List<String>;
    }

    List<String>::Iterator it;
    for (it = mActions->Begin(); it != mActions->End(); ++it) {
        if (!(*it).Compare(action)) return NOERROR;
    }
    mActions->PushBack(String::Duplicate(action));

    return NOERROR;
}

ECode CIntentFilter::CountActions(
    /* [out] */ Int32 *count)
{
    if (NULL == count) return E_INVALID_ARGUMENT;

    *count = mActions != NULL ?
            (Int32)mActions->GetSize() : -1;

    return NOERROR;
}

ECode CIntentFilter::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String *action)
{
    if (NULL == action || mActions == NULL ||
        (mActions != NULL && index >= (Int32)mActions->GetSize())) {
            return E_INVALID_ARGUMENT;
    }

    List<String>::Iterator it;
    for(it = mActions->Begin(); index > 0; --index) ++it;
    *action = String::Duplicate(*it);

    return NOERROR;
}

ECode CIntentFilter::AddDataType(
    /* [in] */ String type)
{
//		Int32 slashpos = type.IndexOf('/');
//		Int32 typelen = type->GetLength();
//		if (slashpos > 0 && typelen >= slashpos + 2) {
//			if (mDataTypes == NULL) mDataTypes = new ArrayList<String>();
//			if (typelen == slashpos+2 && type->CharAt(slashpos+1) == '*') {
//				String str = type->Substring(0, slashpos);
//				if (!mDataTypes->Contains(str)) {
//					mDataTypes->Add(str->Intern());
//				}
//				mHasPartialTypes = true;
//			} else {
//				if (!mDataTypes->Contains(type)) {
//					mDataTypes->Add(type->Intern());
//				}
//			}
//			return;
//		}

//		throw new MalformedMimeTypeException(type);
	return E_NOT_IMPLEMENTED;
}

ECode CIntentFilter::AddDataScheme(
	/* [in] */ String scheme)
{
	return E_NOT_IMPLEMENTED;
}

ECode CIntentFilter::AddDataAuthority(
	/* [in] */ String host,
	/* [in] */ String port)
{
	return E_NOT_IMPLEMENTED;
}

ECode CIntentFilter::AddDataPath(
	/* [in] */ String path,
	/* [in] */ Int32 type)
{
	return E_NOT_IMPLEMENTED;
}

ECode CIntentFilter::AddCategory(
    /* [in] */ String category)
{
    if (category.IsNull() || category.IsEmpty()) return NOERROR;

    if (mCategories == NULL) {
        mCategories = new List<String>;
    }

    List<String>::Iterator it;
    for (it = mCategories->Begin(); it != mCategories->End(); ++it) {
        if (!(*it).Compare(category)) return NOERROR;
    }
    mCategories->PushBack(String::Duplicate(category));

    return NOERROR;
}

ECode CIntentFilter::HasCategory(
    /* [in] */ String category,
    /* [out] */ Boolean* hasCategory)
{
    if (hasCategory == NULL) return E_INVALID_ARGUMENT;

    *hasCategory = FALSE;
    if (mCategories != NULL) {
        List<String>::Iterator it;
        for (it = mCategories->Begin(); it != mCategories->End(); ++it) {
            if (!(*it).Compare(category)) {
                *hasCategory = TRUE;
                break;
            }
        }
    }
    return NOERROR;
}

/**
 * Match this filter against an Intent's action.  If the filter does not
 * specify any actions, the match will always fail.
 *
 * @param action The desired action to look for.
 *
 * @return True if the action is listed in the filter or the filter does
 *         not specify any actions.
 */
Boolean CIntentFilter::MatchAction(
    /* [in] */ String action)
{
    if (action.IsNull() || mActions == NULL ||
        (mActions != NULL && mActions->GetSize() == 0)) {
        return FALSE;
    }

    List<String>::Iterator it;
    for (it = mActions->Begin(); it != mActions->End(); ++it) {
        if (!(*it).Compare(action)) return TRUE;
    }
    return FALSE;
}

/**
 * Match this filter against an Intent's data (type, scheme and path). If
 * the filter does not specify any types and does not specify any
 * schemes/paths, the match will only succeed if the intent does not
 * also specify a type or data.
 *
 * <p>Be aware that to match against an authority, you must also specify a base
 * scheme the authority is in.  To match against a data path, both a scheme
 * and authority must be specified.  If the filter does not specify any
 * types or schemes that it matches against, it is considered to be empty
 * (any authority or data path given is ignored, as if it were empty as
 * well).
 *
 * <p><em>Note: MIME type, Uri scheme, and host name matching in the
 * Android framework is case-sensitive, unlike the formal RFC definitions.
 * As a result, you should always write these elements with lower case letters,
 * and normalize any MIME types or Uris you receive from
 * outside of Android to ensure these elements are lower case before
 * supplying them here.</em></p>
 *
 * @param type The desired data type to look for, as returned by
 *             Intent.resolveType().
 * @param scheme The desired data scheme to look for, as returned by
 *               Intent.getScheme().
 * @param data The full data string to match against, as supplied in
 *             Intent.data.
 *
 * @return Returns either a valid match constant (a combination of
 * {@link #MATCH_CATEGORY_MASK} and {@link #MATCH_ADJUSTMENT_MASK}),
 * or one of the error codes {@link #NO_MATCH_TYPE} if the type didn't match
 * or {@link #NO_MATCH_DATA} if the scheme/path didn't match.
 *
 * @see #match
 */
Int32 CIntentFilter::MatchData(
    /* [in] */ String type,
    /* [in] */ String scheme,
    /* [in] */ IUri* data)
{
//    return Match_Category_Empty;

    List<String>* types = mDataTypes;
    List<String>* schemes = mDataSchemes;
//    List<AuthorityEntry>* authorities = mDataAuthorities;
//    List<PatternMatcher>* paths = mDataPaths;

    Int32 match = Match_Category_Empty;

    if (types == NULL && schemes == NULL) {
        return ((type.IsNull() && data == NULL)
            ? (Match_Category_Empty + Match_Adjustment_Normal) : No_Match_Data);
    }

    if (schemes != NULL) {
        List<String>::Iterator it;
        for (it = schemes->Begin(); it != schemes->End(); ++it) {
            if (!(*it).Compare(!scheme.IsNull() ? scheme : "")) {
                match = Match_Category_Scheme;
                break;
            }
        }
        if (it == schemes->End()) return No_Match_Data;

//        if (authorities != null) {
//            int authMatch = matchDataAuthority(data);
//            if (authMatch >= 0) {
//                if (paths == null) {
//                    match = authMatch;
//                } else if (hasDataPath(data.getPath())) {
//                    match = MATCH_CATEGORY_PATH;
//                } else {
//                    return NO_MATCH_DATA;
//                }
//            } else {
//                return NO_MATCH_DATA;
//            }
//        }
    } else {
        // Special case: match either an Intent with no data URI,
        // or with a scheme: URI.  This is to give a convenience for
        // the common case where you want to deal with data in a
        // content provider, which is done by type, and we don't want
        // to force everyone to say they handle content: or file: URIs.
        if (!scheme.IsNull() && scheme.Compare("")
                && scheme.Compare("content")
                && scheme.Compare("file")) {
            return No_Match_Data;
        }
    }

//    if (types != NULL) {
//        if (findMimeType(type)) {
//            match = Match_Category_Type;
//        } else {
//            return No_Match_Type;
//        }
//    } else {
//        // If no MIME types are specified, then we will only match against
//        // an Intent that does not have a MIME type.
//        if (!type.IsNull()) {
//            return No_Match_Type;
//        }
//    }

    return match + Match_Adjustment_Normal;
}

/**
 * Match this filter against an Intent's categories.  Each category in
 * the Intent must be specified by the filter; if any are not in the
 * filter, the match fails.
 *
 * @param categories The categories included in the intent, as returned by
 *                   Intent.getCategories().
 *
 * @return If all categories match (success), null; else the name of the
 *         first category that didn't match.
 */
String CIntentFilter::MatchCategories(
    /* [in] */ ArrayOf<String>* categories)
{
    if (categories == NULL || categories->GetLength() <= 0) {
        return NULL;
    }

    if (mCategories == NULL) {
        return (*categories)[0];
    }

    for (Int32 i = 0; i < categories->GetLength(); i++) {
        String category = (*categories)[i];
        List<String>::Iterator it;
        for (it = mCategories->Begin(); it != mCategories->End(); ++it) {
            if ((*it).Compare(category)) return category;
        }
    }

    return NULL;

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
ECode CIntentFilter::Match(
    /* [in] */ String action,
    /* [in] */ String type,
    /* [in] */ String scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ String logTag,
    /* [out] */ Int32* result)
{
    if (!action.IsNull() && !MatchAction(action)) {
//        if (Config.LOGV) Log.v(
//            logTag, "No matching action " + action + " for " + this);
        *result = No_Match_Action;
        return NOERROR;
    }

//    return MATCH_CATEGORY_EMPTY + MATCH_ADJUSTMENT_NORMAL;

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
        *result = dataMatch;
        return NOERROR;
    }

    String categoryMatch = MatchCategories(categories);
    if (!categoryMatch.IsNull()) {
//        if (Config.LOGV) Log.v(
//            logTag, "No matching category "
//            + categoryMatch + " for " + this);
        *result = No_Match_Category;
        return NOERROR;
    }

    // It would be nice to treat container activities as more
    // important than ones that can be embedded, but this is not the way...
//    if (FALSE) {
//        if (categories != NULL) {
//            dataMatch -= mCategories.size() - categories.size();
//        }
//    }

    *result = dataMatch;
    return NOERROR;
}

ECode CIntentFilter::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntentFilter::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntentFilter::constructor()
{
    return NOERROR;
}

ECode CIntentFilter::constructor(
    /* [in] */ String action)
{
    return NOERROR;
}

ECode CIntentFilter::constructor(
    /* [in] */ String action,
    /* [in] */ String dataType)
{
    return NOERROR;
}

ECode CIntentFilter::constructor(
    /* [in] */ IIntentFilter* o)
{
    return NOERROR;
}

List<String>* CIntentFilter::GetActions()
{
    return mActions;
}

List<String>* CIntentFilter::GetSchemes()
{
    return mDataSchemes;
}

List<String>* CIntentFilter::GetTypes()
{
    return mDataTypes;
}
