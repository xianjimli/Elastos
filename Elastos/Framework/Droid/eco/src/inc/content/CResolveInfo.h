
#ifndef __CRESOLVERINFO_H__
#define __CRESOLVERINFO_H__

#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include "_CResolveInfo.h"
#include "capsule/CActivityInfo.h"
#include "capsule/CServiceInfo.h"
#include "content/CIntentFilter.h"

using namespace Elastos;

CarClass(CResolveInfo)
{
public:
    CResolveInfo()
//	        : mPriority(0)
//	        , mPreferredOrder(0)
//	        , mMatch(0)
//	        , mSpecificIndex(-1)
//	        , mIsDefault(FALSE)
//	        , mLabelRes(0)
//	        , mIcon(0)
    {}

    ~CResolveInfo() {}

public:
    CARAPI GetActivityInfo(
        /* [out] */ IActivityInfo** activityInfo);

    CARAPI GetServiceInfo(
        /* [out] */ IServiceInfo** serviceInfo);

    CARAPI GetIntentFilter(
        /* [out] */ IIntentFilter** intentFilter);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

public:
    /**
     * The activity that corresponds to this resolution match, if this
     * resolution is for an activity.  One and only one of this and
     * serviceInfo must be non-null.
     */
    AutoPtr<CActivityInfo> mActivityInfo;

     /**
     * The service that corresponds to this resolution match, if this
     * resolution is for a service. One and only one of this and
     * activityInfo must be non-null.
     */
    AutoPtr<CServiceInfo> mServiceInfo;

    /**
     * The IntentFilter that was matched for this ResolveInfo.
     */
    AutoPtr<CIntentFilter> mFilter;

    /**
     * The declared priority of this match.  Comes from the "priority"
     * attribute or, if not set, defaults to 0.  Higher values are a higher
     * priority.
     */
    Int32 mPriority;

    /**
     * Order of result according to the user's preference.  If the user
     * has not set a preference for this result, the value is 0; higher
     * values are a higher priority.
     */
    Int32 mPreferredOrder;

    /**
     * The system's evaluation of how well the activity matches the
     * IntentFilter.  This is a match constant, a combination of
     * {@link IntentFilter#MATCH_CATEGORY_MASK IntentFilter.MATCH_CATEGORY_MASK}
     * and {@link IntentFilter#MATCH_ADJUSTMENT_MASK IntentFiler.MATCH_ADJUSTMENT_MASK}.
     */
    Int32 mMatch;

    /**
     * Only set when returned by
     * {@link PackageManager#queryIntentActivityOptions}, this tells you
     * which of the given specific intents this result came from.  0 is the
     * first in the list, < 0 means it came from the generic Intent query.
     */
    Int32 mSpecificIndex;

    /**
     * This filter has specified the Intent.CATEGORY_DEFAULT, meaning it
     * would like to be considered a default action that the user can
     * perform on this data.
     */
    Boolean mIsDefault;

    /**
     * A string resource identifier (in the package's resources) of this
     * match's label.  From the "label" attribute or, if not set, 0.
     */
    Int32 mLabelRes;

    /**
     * The actual string retrieve from <var>labelRes</var> or null if none
     * was provided.
     */
    AutoPtr<ICharSequence> mNonLocalizedLabel;

    /**
     * A drawable resource identifier (in the package's resources) of this
     * match's icon.  From the "icon" attribute or, if not set, 0.
     */
    Int32 mIcon;

    /**
     * Optional -- if non-null, the {@link #labelRes} and {@link #icon}
     * resources will be loaded from this package, rather than the one
     * containing the resolved component.
     */
    String mResolvePackageName;

//	    public static final Creator<ResolveInfo> CREATOR
//	            = new Creator<ResolveInfo>() {
};

#endif //__CRESOLVERINFO_H__
