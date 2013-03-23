
#include "view/accessibility/CAccessibilityEvent.h"

const Int32 CAccessibilityEvent::MAX_POOL_SIZE;
const Int32 CAccessibilityEvent::CHECKED;
const Int32 CAccessibilityEvent::ENABLED;
const Int32 CAccessibilityEvent::PASSWORD;
const Int32 CAccessibilityEvent::FULL_SCREEN;

AutoPtr<IAccessibilityEvent> CAccessibilityEvent::sPool;
Mutex CAccessibilityEvent::sPoolLock;
Int32 CAccessibilityEvent::sPoolSize = 0;

CAccessibilityEvent::CAccessibilityEvent()
    : mEventType(0)
    , mBooleanProperties(0)
    , mCurrentItemIndex(AccessibilityEvent_INVALID_POSITION)
    , mItemCount(0)
    , mFromIndex(0)
    , mAddedCount(0)
    , mRemovedCount(0)
    , mEventTime(0)
    , mIsInPool(FALSE)
{}

CAccessibilityEvent::~CAccessibilityEvent()
{
    for (Int32 i = 0; i < mText->GetLength(); ++i) {
        (*mText)[i] = NULL;
    }

    ArrayOf<AutoPtr<ICharSequence> >::Free(mText);
}

/**
 * Gets if the source is checked.
 *
 * @return True if the view is checked, false otherwise.
 */
ECode CAccessibilityEvent::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    VALIDATE_NOT_NULL(isChecked);
    *isChecked = GetBooleanProperty(CHECKED);
    return NOERROR;
}

/**
 * Sets if the source is checked.
 *
 * @param isChecked True if the view is checked, false otherwise.
 */
ECode CAccessibilityEvent::SetChecked(
    /* [in] */ Boolean isChecked)
{
    SetBooleanProperty(CHECKED, isChecked);
    return NOERROR;
}

/**
 * Gets if the source is enabled.
 *
 * @return True if the view is enabled, false otherwise.
 */
ECode CAccessibilityEvent::IsEnabled(
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);
    *isEnabled = GetBooleanProperty(ENABLED);
    return NOERROR;
}

/**
 * Sets if the source is enabled.
 *
 * @param isEnabled True if the view is enabled, false otherwise.
 */
ECode CAccessibilityEvent::SetEnabled(
    /* [in] */ Boolean isEnabled)
{
    SetBooleanProperty(ENABLED, isEnabled);
    return NOERROR;
}

/**
 * Gets if the source is a password field.
 *
 * @return True if the view is a password field, false otherwise.
 */
ECode CAccessibilityEvent::IsPassword(
    /* [out] */ Boolean* isPassword)
{
    VALIDATE_NOT_NULL(isPassword);
    *isPassword = GetBooleanProperty(PASSWORD);
    return NOERROR;
}

/**
 * Sets if the source is a password field.
 *
 * @param isPassword True if the view is a password field, false otherwise.
 */
ECode CAccessibilityEvent::SetPassword(
    /* [in] */ Boolean isPassword)
{
    SetBooleanProperty(PASSWORD, isPassword);
    return NOERROR;
}

/**
 * Sets if the source is taking the entire screen.
 *
 * @param isFullScreen True if the source is full screen, false otherwise.
 */
ECode CAccessibilityEvent::SetFullScreen(
    /* [in] */ Boolean isFullScreen)
{
    SetBooleanProperty(FULL_SCREEN, isFullScreen);
    return NOERROR;
}

/**
 * Gets if the source is taking the entire screen.
 *
 * @return True if the source is full screen, false otherwise.
 */
ECode CAccessibilityEvent::IsFullScreen(
    /* [out] */ Boolean* isFullScreen)
{
    VALIDATE_NOT_NULL(isFullScreen);
    *isFullScreen = GetBooleanProperty(FULL_SCREEN);
    return NOERROR;
}

/**
 * Gets the event type.
 *
 * @return The event type.
 */
ECode CAccessibilityEvent::GetEventType(
    /* [out] */ Int32* eventType)
{
    VALIDATE_NOT_NULL(eventType);
    *eventType = mEventType;
    return NOERROR;
}

/**
 * Sets the event type.
 *
 * @param eventType The event type.
 */
ECode CAccessibilityEvent::SetEventType(
    /* [in] */ Int32 eventType)
{
    mEventType = eventType;
    return NOERROR;
}

/**
 * Gets the number of items that can be visited.
 *
 * @return The number of items.
 */
ECode CAccessibilityEvent::GetItemCount(
    /* [out] */ Int32* itemCount)
{
    VALIDATE_NOT_NULL(itemCount);
    *itemCount = mItemCount;
    return NOERROR;
}

/**
 * Sets the number of items that can be visited.
 *
 * @param itemCount The number of items.
 */
ECode CAccessibilityEvent::SetItemCount(
    /* [in] */ Int32 itemCount)
{
    mItemCount = itemCount;
    return NOERROR;
}

/**
 * Gets the index of the source in the list of items the can be visited.
 *
 * @return The current item index.
 */
ECode CAccessibilityEvent::GetCurrentItemIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mCurrentItemIndex;
    return NOERROR;
}

/**
 * Sets the index of the source in the list of items that can be visited.
 *
 * @param currentItemIndex The current item index.
 */
ECode CAccessibilityEvent::SetCurrentItemIndex(
    /* [in] */ Int32 currentItemIndex)
{
    mCurrentItemIndex = currentItemIndex;
    return NOERROR;
}

/**
 * Gets the index of the first character of the changed sequence.
 *
 * @return The index of the first character.
 */
ECode CAccessibilityEvent::GetFromIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mFromIndex;
    return NOERROR;
}

/**
 * Sets the index of the first character of the changed sequence.
 *
 * @param fromIndex The index of the first character.
 */
ECode CAccessibilityEvent::SetFromIndex(
    /* [in] */ Int32 fromIndex)
{
    mFromIndex = fromIndex;
    return NOERROR;
}

/**
 * Gets the number of added characters.
 *
 * @return The number of added characters.
 */
ECode CAccessibilityEvent::GetAddedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mAddedCount;
    return NOERROR;
}

/**
 * Sets the number of added characters.
 *
 * @param addedCount The number of added characters.
 */
ECode CAccessibilityEvent::SetAddedCount(
    /* [in] */ Int32 addedCount)
{
    mAddedCount = addedCount;
    return NOERROR;
}

/**
 * Gets the number of removed characters.
 *
 * @return The number of removed characters.
 */
ECode CAccessibilityEvent::GetRemovedCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mRemovedCount;
    return NOERROR;
}

/**
 * Sets the number of removed characters.
 *
 * @param removedCount The number of removed characters.
 */
ECode CAccessibilityEvent::SetRemovedCount(
    /* [in] */ Int32 removedCount)
{
    mRemovedCount = removedCount;
    return NOERROR;
}

/**
 * Gets the time in which this event was sent.
 *
 * @return The event time.
 */
ECode CAccessibilityEvent::GetEventTime(
    /* [out] */ Int64* eventTime)
{
    VALIDATE_NOT_NULL(eventTime);
    *eventTime = mEventTime;
    return NOERROR;
}

/**
 * Sets the time in which this event was sent.
 *
 * @param eventTime The event time.
 */
ECode CAccessibilityEvent::SetEventTime(
    /* [in] */ Int64 eventTime)
{
    mEventTime = eventTime;
    return NOERROR;
}

/**
 * Gets the class name of the source.
 *
 * @return The class name.
 */
ECode CAccessibilityEvent::GetClassName(
    /* [out] */ ICharSequence** className)
{
    VALIDATE_NOT_NULL(className);
    *className = mClassName;
    return NOERROR;
}

/**
 * Sets the class name of the source.
 *
 * @param className The lass name.
 */
ECode CAccessibilityEvent::SetClassName(
    /* [in] */ ICharSequence* className)
{
    mClassName = className;
    return NOERROR;
}

/**
 * Gets the package name of the source.
 *
 * @return The package name.
 */
ECode CAccessibilityEvent::GetCapsuleName(
    /* [out] */ ICharSequence** capsuleName)
{
    VALIDATE_NOT_NULL(capsuleName);
    *capsuleName = mCapsuleName;
    return NOERROR;
}

/**
 * Sets the package name of the source.
 *
 * @param packageName The package name.
 */
ECode CAccessibilityEvent::SetCapsuleName(
    /* [in] */ ICharSequence* capsuleName)
{
    mCapsuleName = capsuleName;
    return NOERROR;
}

/**
 * Gets the text of the event. The index in the list represents the priority
 * of the text. Specifically, the lower the index the higher the priority.
 *
 * @return The text.
 */
ECode CAccessibilityEvent::GetText(
    /* [out] */ IObjectContainer** container)
{
    VALIDATE_NOT_NULL(container);

    AutoPtr<IObjectContainer> c;
    CObjectContainer::New((IObjectContainer**)&c);
    List<AutoPtr<ICharSequence> >::Iterator it;
    for (Int32 i = 0; i < mText->GetLength(); ++i) {
        c->Add((*mText)[i]);
    }
    *container = c;

    return NOERROR;
}

/**
 * Sets the text before a change.
 *
 * @return The text before the change.
 */
ECode CAccessibilityEvent::GetBeforeText(
    /* [out] */ ICharSequence** beforeText)
{
    VALIDATE_NOT_NULL(beforeText);
    *beforeText = mBeforeText;
    return NOERROR;
}

/**
 * Sets the text before a change.
 *
 * @param beforeText The text before the change.
 */
ECode CAccessibilityEvent::SetBeforeText(
    /* [in] */ ICharSequence* beforeText)
{
    mBeforeText = beforeText;
    return NOERROR;
}

/**
 * Gets the description of the source.
 *
 * @return The description.
 */
ECode CAccessibilityEvent::GetContentDescription(
    /* [out] */ ICharSequence** contentDescription)
{
    VALIDATE_NOT_NULL(contentDescription);
    *contentDescription = mContentDescription;
    return NOERROR;
}

/**
 * Sets the description of the source.
 *
 * @param contentDescription The description.
 */
ECode CAccessibilityEvent::SetContentDescription(
    /* [in] */ ICharSequence* contentDescription)
{
    mContentDescription = contentDescription;
    return NOERROR;
}

/**
 * Gets the {@link Parcelable} data.
 *
 * @return The parcelable data.
 */
ECode CAccessibilityEvent::GetParcelableData(
    /* [out] */ IParcelable** parcelableData)
{
    VALIDATE_NOT_NULL(parcelableData);
    *parcelableData = mParcelableData;
    return NOERROR;
}

/**
 * Sets the {@link Parcelable} data of the event.
 *
 * @param parcelableData The parcelable data.
 */
ECode CAccessibilityEvent::SetParcelableData(
    /* [in] */ IParcelable* parcelableData)
{
    mParcelableData = parcelableData;
    return NOERROR;
}

/**
 * Return an instance back to be reused.
 * <p>
 * <b>Note: You must not touch the object after calling this function.</b>
 */
ECode CAccessibilityEvent::Recycle()
{
    if (mIsInPool) {
        return NOERROR;
    }

    Clear();
    Mutex::Autolock lock(sPoolLock);
    if (sPoolSize <= MAX_POOL_SIZE) {
        mNext = sPool;
        sPool = (IAccessibilityEvent*)this->Probe(EIID_IAccessibilityEvent);
        mIsInPool = TRUE;
        sPoolSize++;
    }
    return NOERROR;
}

/**
 * Clears the state of this instance.
 */
void CAccessibilityEvent::Clear()
{
    mEventType = 0;
    mBooleanProperties = 0;
    mCurrentItemIndex = AccessibilityEvent_INVALID_POSITION;
    mItemCount = 0;
    mFromIndex = 0;
    mAddedCount = 0;
    mRemovedCount = 0;
    mEventTime = 0;
    mClassName = NULL;
    mCapsuleName = NULL;
    mContentDescription = NULL;
    mBeforeText = NULL;
    mParcelableData = NULL;
    for (Int32 i = 0; i < mText->GetLength(); ++i) {
        (*mText)[i] = NULL;
    }
    ArrayOf<AutoPtr<ICharSequence> >::Free(mText);
}

/**
 * Gets the value of a boolean property.
 *
 * @param property The property.
 * @return The value.
 */
Boolean CAccessibilityEvent::GetBooleanProperty(
    /* [in] */ Int32 property)
{
    return (mBooleanProperties & property) == property;
}

/**
 * Sets a boolean property.
 *
 * @param property The property.
 * @param value The value.
 */
void CAccessibilityEvent::SetBooleanProperty(
    /* [in] */ Int32 property,
    /* [in] */ Boolean value)
{
    if (value) {
        mBooleanProperties |= property;
    }
    else {
        mBooleanProperties &= ~property;
    }
}

/**
 * Creates a new instance from a {@link Parcel}.
 *
 * @param parcel A parcel containing the state of a {@link AccessibilityEvent}.
 */
ECode CAccessibilityEvent::InitFromParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->ReadInt32(&mEventType);
    parcel->ReadInt32(&mBooleanProperties);
    parcel->ReadInt32(&mCurrentItemIndex);
    parcel->ReadInt32(&mItemCount);
    parcel->ReadInt32(&mFromIndex);
    parcel->ReadInt32(&mAddedCount);
    parcel->ReadInt32(&mRemovedCount);
    parcel->ReadInt64(&mEventTime);
    // mClassName = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    // mCapsuleName = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    // mContentDescription = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    // mBeforeText = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
    AutoPtr<IInterface> obj;
    parcel->ReadInterfacePtrPtr((Handle32*)&obj);
    mClassName = obj != NULL ?
            (ICharSequence*)obj->Probe(EIID_ICharSequence) : NULL;
    parcel->ReadInterfacePtrPtr((Handle32*)&obj);
    mCapsuleName = obj != NULL ?
            (ICharSequence*)obj->Probe(EIID_ICharSequence) : NULL;
    parcel->ReadInterfacePtrPtr((Handle32*)&obj);
    mContentDescription = obj != NULL ?
            (ICharSequence*)obj->Probe(EIID_ICharSequence) : NULL;
    parcel->ReadInterfacePtrPtr((Handle32*)&obj);
    mBeforeText = obj != NULL ?
            (ICharSequence*)obj->Probe(EIID_ICharSequence) : NULL;
    parcel->ReadInterfacePtrPtr((Handle32*)&obj);
    mParcelableData = obj != NULL ?
            (IParcelable*)obj->Probe(EIID_IParcelable) : NULL;
    parcel->ReadArrayOf((Handle32*)&mText);
    // mParcelableData = parcel.ReadInterfacePtr(null);
    // parcel.readList(mText, null);
    return NOERROR;
}

ECode CAccessibilityEvent::DescribeContents(
    /* [out] */ Int32* describeContents)
{
    VALIDATE_NOT_NULL(describeContents);
    *describeContents = 0;
    return NOERROR;
}

ECode CAccessibilityEvent::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return NOERROR;
}

ECode CAccessibilityEvent::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInt32(mEventType);
    dest->WriteInt32(mBooleanProperties);
    dest->WriteInt32(mCurrentItemIndex);
    dest->WriteInt32(mItemCount);
    dest->WriteInt32(mFromIndex);
    dest->WriteInt32(mAddedCount);
    dest->WriteInt32(mRemovedCount);
    dest->WriteInt64(mEventTime);
    // TextUtils.writeToParcel(mClassName, parcel, 0);
    // TextUtils.writeToParcel(mPackageName, parcel, 0);
    // TextUtils.writeToParcel(mContentDescription, parcel, 0);
    // TextUtils.writeToParcel(mBeforeText, parcel, 0);
    dest->WriteInterfacePtr(mClassName);
    dest->WriteInterfacePtr(mCapsuleName);
    dest->WriteInterfacePtr(mContentDescription);
    dest->WriteInterfacePtr(mBeforeText);
    dest->WriteInterfacePtr(mBeforeText);
    dest->WriteInterfacePtr(mParcelableData);
    dest->WriteArrayOf((Handle32)mText);
}
