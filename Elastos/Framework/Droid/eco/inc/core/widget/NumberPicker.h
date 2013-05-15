
#ifndef __NUMBERPICKER_H__
#define __NUMBERPICKER_H__

#include "widget/LinearLayout.h"
#include "widget/NumberKeyListener.h"
//#include <elastos/ElRefBase.h>
#include <StringBuffer.h>

/**
 * A view for selecting a number
 *
 * For a dialog using this view, see {@link android.app.TimePickerDialog}.
 * @hide
 */

class NumberPicker : public LinearLayout
{
private:
    class OnLongClickRunnable
            : public IRunnable
            , public ElRefBase
    {
    public:
        OnLongClickRunnable(
            /* [in] */ NumberPicker* host);

        UInt32 AddRef();

        UInt32 Release();

        PInterface Probe(
            /* [in] */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        ECode Run();

    private:
        NumberPicker* mHost;
    };

    class ClickListener
            : public IViewOnClickListener
            , public ElRefBase
    {
    public:
        ClickListener(
            /* [in] */ NumberPicker* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        NumberPicker* mHost;
    };

    class FocusListener
            : public IViewOnFocusChangeListener
            , public ElRefBase
    {
    public:
        FocusListener(
            /* [in] */ NumberPicker* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnFocusChange(
            /* [in] */ IView* v,
            /* [in] */ Boolean hasFocus);

    private:
        NumberPicker* mHost;
    };

    class LongClickListener
            : public IViewOnLongClickListener
            , public ElRefBase
    {
    public:
        LongClickListener(
            /* [in] */ NumberPicker* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnLongClick(
            /* [in] */ IView* v,
            /* [out] */ Boolean* result);

    private:
        NumberPicker* mHost;
    };

public:
    NumberPicker();

    /**
     * Create a new number picker
     * @param context the application environment
     */
    NumberPicker(
        /* [in] */ IContext* context);

    /**
     * Create a new number picker
     * @param context the application environment
     * @param attrs a collection of attributes
     */
    NumberPicker(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ~NumberPicker();

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Set the enabled state of this view. The interpretation of the enabled
     * state varies by subclass.
     *
     * @param enabled True if this view is enabled, FALSE otherwise.
     */
    virtual CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Set the callback that indicates the number has been adjusted by the user.
     * @param listener the callback, should not be NULL.
     */
    virtual CARAPI SetOnChangeListener(
        /* [in] */ IOnChangedListener* listener);

    /**
     * Set the formatter that will be used to format the number for presentation
     * @param formatter the formatter object.  If formatter is NULL, String.valueOf()
     * will be used
     */
    virtual CARAPI SetFormatter(
        /* [in] */ IFormatter* formatter);

    /**
     * Set the range of numbers allowed for the number picker. The current
     * value will be automatically set to the start.
     *
     * @param start the start of the range (inclusive)
     * @param end the end of the range (inclusive)
     */
    virtual CARAPI SetRange(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Set the range of numbers allowed for the number picker. The current
     * value will be automatically set to the start. Also provide a mapping
     * for values used to display to the user.
     *
     * @param start the start of the range (inclusive)
     * @param end the end of the range (inclusive)
     * @param displayedValues the values displayed to the user.
     */
    virtual CARAPI SetRangeEx(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<String>* displayedValues);

    /**
     * Set the current value for the number picker.
     *
     * @param current the current value the start of the range (inclusive)
     * @throws IllegalArgumentException when current is not within the range
     *         of of the number picker
     */
    virtual CARAPI SetCurrent(
        /* [in] */ Int32 current);

    /**
     * Sets the speed at which the numbers will scroll when the +/-
     * buttons are longpressed
     *
     * @param speed The speed (in milliseconds) at which the numbers will scroll
     * default 300ms
     */
    virtual CARAPI SetSpeed(
        /* [in] */ Int64 speed);

    /**
     * @hide
     */
    virtual CARAPI CancelIncrement();

    /**
     * @hide
     */
    virtual CARAPI CancelDecrement();

    /**
     * Returns the current value of the NumberPicker
     * @return the current value.
     */
    virtual CARAPI_(Int32) GetCurrent();

protected:
    /**
     * Sets the current value of this NumberPicker, and sets mPrevious to the previous
     * value.  If current is greater than mEnd less than mStart, the value of mCurrent
     * is wrapped around.
     *
     * Subclasses can override this to change the wrapping behavior
     *
     * @param current the new value of the NumberPicker
     */
    virtual CARAPI_(void) ChangeCurrent(
        /* [in] */ Int32 current);

    /**
     * Returns the upper value of the range of the NumberPicker
     * @return the uppper number of the range.
     */
    virtual CARAPI_(Int32) GetEndRange();

    /**
     * Returns the lower value of the range of the NumberPicker
     * @return the lower number of the range.
     */
    virtual CARAPI_(Int32) GetBeginRange();


private:
    CARAPI_(String) FormatNumber(
        /* [in] */ Int32 value);

    /**
     * Notifies the listener, if registered, of a change of the value of this
     * NumberPicker.
     */
    CARAPI_(void) NotifyChange();

    /**
     * Updates the view of this NumberPicker.  If displayValues were specified
     * in {@link #setRange}, the string corresponding to the index specified by
     * the current value will be returned.  Otherwise, the formatter specified
     * in {@link setFormatter} will be used to format the number.
     */
    CARAPI_(void) UpdateView();

    CARAPI_(void) ValidateCurrentView(
        /* [in] */ ICharSequence* str);

    CARAPI_(void) ValidateInput(
        /* [in] */ IView* v);

public:
    class NumberPickerFormatter
            : public IFormatter
            , public ElRefBase
    {
    public:
        UInt32 AddRef();

        UInt32 Release();

        PInterface Probe(
            /* [in] */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI ToString(
            /* [in] */ Int32 val,
            /* [out] */ String* str);

    public:
        StringBuffer mBuf;// = new StringBuilder();
        //final java.util.Formatter mFmt = new java.util.Formatter(mBuilder);
        //final Object[] mArgs = new Object[1];
    };
    /*
     * Use a custom NumberPicker formatting callback to use two-digit
     * minutes strings like "01".  Keeping a static formatter etc. is the
     * most efficient way to do this; it avoids creating temporary objects
     * on every call to format().
     */
    static const AutoPtr<IFormatter> TWO_DIGIT_FORMATTER;/* =
            new NumberPicker.Formatter() {
                final StringBuilder mBuilder = new StringBuilder();
                final java.util.Formatter mFmt = new java.util.Formatter(mBuilder);
                final Object[] mArgs = new Object[1];
                public String toString(Int32 value) {
                    mArgs[0] = value;
                    mBuilder.delete(0, mBuilder.length());
                    mFmt.format("%02d", mArgs);
                    return mFmt.toString();
                }
        };*/

private:
    static const ArrayOf<Char32>* DIGIT_CHARACTERS;/* = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };*/
    static CARAPI_(const ArrayOf<Char32>*) InitDigitCharacters();

    class NumberPickerInputFilter
            : public IInputFilter
            , public ElRefBase
    {
    public:
        NumberPickerInputFilter(
            /* [in] */ NumberPicker* host);

        UInt32 AddRef();

        UInt32 Release();

        PInterface Probe(
            /* [in] */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Filter(
            /* [in] */ ICharSequence* source,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ ISpanned* dest,
            /* [in] */ Int32 dstart,
            /* [in] */ Int32 dend,
            /*[out] */ ICharSequence** cs);

    private:
        NumberPicker* mHost;
    };

    class NumberRangeKeyListener : public NumberKeyListener
    {
    public:
        NumberRangeKeyListener(
            /* [in] */ NumberPicker* host);

        // XXX This doesn't allow for range limits when controlled by a
        // soft input method!
        CARAPI_(Int32) GetInputType();

        CARAPI Filter(
            /* [in] */ ICharSequence* source,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ ISpanned* dest,
            /* [in] */ Int32 dstart,
            /* [in] */ Int32 dend,
            /*[out] */ ICharSequence** cs);

        CARAPI_(Boolean) OnKeyDown(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event);

    protected:
        CARAPI_(ArrayOf<Char32>*) GetAcceptedChars();

        CARAPI_(Int32) Lookup(
            /* [in] */ IKeyEvent* event,
            /* [in] */ ISpannable* content);

    private:
        NumberPicker* mHost;
    };

    CARAPI_(Int32) GetSelectedPos(
        /* [in] */ String str);

    AutoPtr<IApartment> mHandler;
    AutoPtr<IRunnable> mRunnable;

    AutoPtr<IEditText> mText;
    AutoPtr<IInputFilter> mNumberInputFilter;

    ArrayOf<String>* mDisplayedValues;

    /**
     * Lower value of the range of numbers allowed for the NumberPicker
     */
    Int32 mStart;

    /**
     * Upper value of the range of numbers allowed for the NumberPicker
     */
    Int32 mEnd;

    /**
     * Current value of this NumberPicker
     */
    Int32 mCurrent;

    /**
     * Previous value of this NumberPicker.
     */
    Int32 mPrevious;
    AutoPtr<IOnChangedListener> mListener;
    AutoPtr<IFormatter> mFormatter;
    Int64 mSpeed;

    Boolean mIncrement;
    Boolean mDecrement;

    AutoPtr<INumberPickerButton> mIncrementButton;
    AutoPtr<INumberPickerButton> mDecrementButton;
};

#endif
