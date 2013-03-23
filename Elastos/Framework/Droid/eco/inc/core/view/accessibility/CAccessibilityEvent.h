
#ifndef __CACCESSIBILITYEVENT_H__
#define __CACCESSIBILITYEVENT_H__

#include "_CAccessibilityEvent.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/List.h>

using namespace Elastos::Core::Threading;

class CAccessibilityEventHelper;

CarClass(CAccessibilityEvent)
{
public:
    friend class CAccessibilityEventHelper;

    CAccessibilityEvent();

    ~CAccessibilityEvent();

    /**
     * Gets if the source is checked.
     *
     * @return True if the view is checked, false otherwise.
     */
    CARAPI IsChecked(
        /* [out] */ Boolean* isChecked);

    /**
     * Sets if the source is checked.
     *
     * @param isChecked True if the view is checked, false otherwise.
     */
    CARAPI SetChecked(
        /* [in] */ Boolean isChecked);

    /**
     * Gets if the source is enabled.
     *
     * @return True if the view is enabled, false otherwise.
     */
    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnabled);

    /**
     * Sets if the source is enabled.
     *
     * @param isEnabled True if the view is enabled, false otherwise.
     */
    CARAPI SetEnabled(
        /* [in] */ Boolean isEnabled);

    /**
     * Gets if the source is a password field.
     *
     * @return True if the view is a password field, false otherwise.
     */
    CARAPI IsPassword(
        /* [out] */ Boolean* isPassword);

    /**
     * Sets if the source is a password field.
     *
     * @param isPassword True if the view is a password field, false otherwise.
     */
    CARAPI SetPassword(
        /* [in] */ Boolean isPassword);

    /**
     * Sets if the source is taking the entire screen.
     *
     * @param isFullScreen True if the source is full screen, false otherwise.
     */
    CARAPI SetFullScreen(
        /* [in] */ Boolean isFullScreen);

    /**
     * Gets if the source is taking the entire screen.
     *
     * @return True if the source is full screen, false otherwise.
     */
    CARAPI IsFullScreen(
        /* [out] */ Boolean* isFullScreen);

    /**
     * Gets the event type.
     *
     * @return The event type.
     */
    CARAPI GetEventType(
        /* [out] */ Int32* eventType);

    /**
     * Sets the event type.
     *
     * @param eventType The event type.
     */
    CARAPI SetEventType(
        /* [in] */ Int32 eventType);

    /**
     * Gets the number of items that can be visited.
     *
     * @return The number of items.
     */
    CARAPI GetItemCount(
        /* [out] */ Int32* itemCount);

    /**
     * Sets the number of items that can be visited.
     *
     * @param itemCount The number of items.
     */
    CARAPI SetItemCount(
        /* [in] */ Int32 itemCount);

    /**
     * Gets the index of the source in the list of items the can be visited.
     *
     * @return The current item index.
     */
    CARAPI GetCurrentItemIndex(
        /* [out] */ Int32* index);

    /**
     * Sets the index of the source in the list of items that can be visited.
     *
     * @param currentItemIndex The current item index.
     */
    CARAPI SetCurrentItemIndex(
        /* [in] */ Int32 currentItemIndex);

    /**
     * Gets the index of the first character of the changed sequence.
     *
     * @return The index of the first character.
     */
    CARAPI GetFromIndex(
        /* [out] */ Int32* index);

    /**
     * Sets the index of the first character of the changed sequence.
     *
     * @param fromIndex The index of the first character.
     */
    CARAPI SetFromIndex(
        /* [in] */ Int32 fromIndex);

    /**
     * Gets the number of added characters.
     *
     * @return The number of added characters.
     */
    CARAPI GetAddedCount(
        /* [out] */ Int32* count);

    /**
     * Sets the number of added characters.
     *
     * @param addedCount The number of added characters.
     */
    CARAPI SetAddedCount(
        /* [in] */ Int32 addedCount);

    /**
     * Gets the number of removed characters.
     *
     * @return The number of removed characters.
     */
    CARAPI GetRemovedCount(
        /* [out] */ Int32* count);

    /**
     * Sets the number of removed characters.
     *
     * @param removedCount The number of removed characters.
     */
    CARAPI SetRemovedCount(
        /* [in] */ Int32 removedCount);

    /**
     * Gets the time in which this event was sent.
     *
     * @return The event time.
     */
    CARAPI GetEventTime(
        /* [out] */ Int64* eventTime);

    /**
     * Sets the time in which this event was sent.
     *
     * @param eventTime The event time.
     */
    CARAPI SetEventTime(
        /* [in] */ Int64 eventTime);

    /**
     * Gets the class name of the source.
     *
     * @return The class name.
     */
    CARAPI GetClassName(
        /* [out] */ ICharSequence** className);

    /**
     * Sets the class name of the source.
     *
     * @param className The lass name.
     */
    CARAPI SetClassName(
        /* [in] */ ICharSequence* className);

    /**
     * Gets the package name of the source.
     *
     * @return The package name.
     */
    CARAPI GetCapsuleName(
        /* [out] */ ICharSequence** capsuleName);

    /**
     * Sets the package name of the source.
     *
     * @param packageName The package name.
     */
    CARAPI SetCapsuleName(
        /* [in] */ ICharSequence* capsuleName);

    /**
     * Gets the text of the event. The index in the list represents the priority
     * of the text. Specifically, the lower the index the higher the priority.
     *
     * @return The text.
     */
    CARAPI GetText(
        /* [out] */ IObjectContainer** container);

    /**
     * Sets the text before a change.
     *
     * @return The text before the change.
     */
    CARAPI GetBeforeText(
        /* [out] */ ICharSequence** beforeText);

    /**
     * Sets the text before a change.
     *
     * @param beforeText The text before the change.
     */
    CARAPI SetBeforeText(
        /* [in] */ ICharSequence* beforeText);

    /**
     * Gets the description of the source.
     *
     * @return The description.
     */
    CARAPI GetContentDescription(
        /* [out] */ ICharSequence** contentDescription);

    /**
     * Sets the description of the source.
     *
     * @param contentDescription The description.
     */
    CARAPI SetContentDescription(
        /* [in] */ ICharSequence* contentDescription);

    /**
     * Gets the {@link Parcelable} data.
     *
     * @return The parcelable data.
     */
    CARAPI GetParcelableData(
        /* [out] */ IParcelable** parcelableData);
    /**
     * Sets the {@link Parcelable} data of the event.
     *
     * @param parcelableData The parcelable data.
     */
    CARAPI SetParcelableData(
        /* [in] */ IParcelable* parcelableData);

    /**
     * Return an instance back to be reused.
     * <p>
     * <b>Note: You must not touch the object after calling this function.</b>
     */
    CARAPI Recycle();

    /**
     * Creates a new instance from a {@link Parcel}.
     *
     * @param parcel A parcel containing the state of a {@link AccessibilityEvent}.
     */
    CARAPI InitFromParcel(
        /* [in] */ IParcel* parcel);

    CARAPI DescribeContents(
        /* [out] */ Int32* describeContents);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    /**
     * Clears the state of this instance.
     */
    CARAPI_(void) Clear();

    /**
     * Gets the value of a boolean property.
     *
     * @param property The property.
     * @return The value.
     */
    CARAPI_(Boolean) GetBooleanProperty(
        /* [in] */ Int32 property);

    /**
     * Sets a boolean property.
     *
     * @param property The property.
     * @param value The value.
     */
    CARAPI_(void) SetBooleanProperty(
        /* [in] */ Int32 property,
        /* [in] */ Boolean value);

private:
    static const Int32 MAX_POOL_SIZE = 2;
    static const Int32 CHECKED = 0x00000001;
    static const Int32 ENABLED = 0x00000002;
    static const Int32 PASSWORD = 0x00000004;
    static const Int32 FULL_SCREEN = 0x00000080;

private:
    static AutoPtr<IAccessibilityEvent> sPool;
    static Mutex sPoolLock;
    static Int32 sPoolSize;

    AutoPtr<IAccessibilityEvent> mNext;

    Int32 mEventType;
    Int32 mBooleanProperties;
    Int32 mCurrentItemIndex;
    Int32 mItemCount;
    Int32 mFromIndex;
    Int32 mAddedCount;
    Int32 mRemovedCount;

    Int64 mEventTime;

    AutoPtr<ICharSequence> mClassName;
    AutoPtr<ICharSequence> mCapsuleName;
    AutoPtr<ICharSequence> mContentDescription;
    AutoPtr<ICharSequence> mBeforeText;

    AutoPtr<IParcelable> mParcelableData;

    ArrayOf<AutoPtr<ICharSequence> >* mText;

    Boolean mIsInPool;
};

#endif //__CACCESSIBILITYEVENT_H__
