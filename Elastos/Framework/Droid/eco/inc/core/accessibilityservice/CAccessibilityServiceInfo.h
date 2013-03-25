
#ifndef __CACCESSIBILITYSERVICEINFO_H__
#define __CACCESSIBILITYSERVICEINFO_H__

#include "_CAccessibilityServiceInfo.h"
#include "utils/AutoStringArray.h"

CarClass(CAccessibilityServiceInfo)
{
public:
    CAccessibilityServiceInfo();

    ~CAccessibilityServiceInfo();

    CARAPI SetEventTypes (
        /* [in] */ Int32 types);

    CARAPI GetEventTypes (
        /* [out] */ Int32* types);

    CARAPI SetPackageNames (
        /* [in] */ const ArrayOf<String>& names);

    CARAPI GetPackageNames (
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI SetFeedbackType (
        /* [in] */ Int32 type);

    CARAPI GetFeedbackType (
        /* [out] */ Int32* type);

    CARAPI SetNotificationTimeout (
        /* [in] */ Int64 timeout);

    CARAPI GetNotificationTimeout (
        /* [out] */ Int64* timeout);

    CARAPI SetFlags (
        /* [in] */ Int32 flags);

    CARAPI GetFlags (
        /* [out] */ Int32* flags);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    /**
     * The event types an {@link AccessibilityService} is interested in.
     *
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_CLICKED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_LONG_CLICKED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_FOCUSED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_SELECTED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_TEXT_CHANGED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_WINDOW_STATE_CHANGED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_NOTIFICATION_STATE_CHANGED
     */
    Int32 mEventTypes;

    /**
     * The package names an {@link AccessibilityService} is interested in. Setting
     * to null is equivalent to all packages.
     */
    AutoStringArray mPackageNames;

    /**
     * The feedback type an {@link AccessibilityService} provides.
     *
     * @see #FEEDBACK_AUDIBLE
     * @see #FEEDBACK_GENERIC
     * @see #FEEDBACK_HAPTIC
     * @see #FEEDBACK_SPOKEN
     * @see #FEEDBACK_VISUAL
     */
    Int32 mFeedbackType;

    /**
     * The timeout after the most recent event of a given type before an
     * {@link AccessibilityService} is notified.
     * <p>
     * Note: The event notification timeout is useful to avoid propagating events to the client
     *       too frequently since this is accomplished via an expensive interprocess call.
     *       One can think of the timeout as a criteria to determine when event generation has
     *       settled down
     */
    Int64 mNotificationTimeout;

    /**
     * This field represents a set of flags used for configuring an
     * {@link AccessibilityService}.
     *
     * @see #DEFAULT
     */
    Int32 mFlags;
};

#endif //__CACCESSIBILITYSERVICEINFO_H__
