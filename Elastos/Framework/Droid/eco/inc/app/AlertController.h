
#ifndef __ALERTCONTROLLER_H__
#define __ALERTCONTROLLER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class AlertController : public ElRefBase
{
public:
    //class RecycleListView
    //    : public ElRefBase
    //    , public IListView
    //    , public ITextWatcher
    //    , public IOnGlobalLayoutListener
    //    , public IOnTouchModeChangeListener
    //    , public IFilterListener
    //    , public IViewParent
    //    , public IViewManager
    //    , public IDrawableCallback
    //    , public IKeyEventCallback
    //    , public IAccessibilityEventSource
    //{
    //public:
    //    RecycleListView(
    //        /* [in] */ IContext* context)
    //        : ListView(context)
    //        , mRecycleOnMeasure(TRUE)
    //    {
    //    }

    //    RecycleListView(
    //        /* [in] */ IContext* context,
    //        /* [in] */ IAttributeSet* attrs)
    //        : ListView(context, attrs)
    //        , mRecycleOnMeasure(TRUE)
    //    {
    //    }

    //    RecycleListView(
    //        /* [in] */ IContext* context,
    //        /* [in] */ IAttributeSet* attrs,
    //        /* [in] */ Int32 defStyle)
    //        : ListView(context)
    //        , mRecycleOnMeasure(TRUE)
    //    {
    //    }

    //protected:
    //    //@Override
    //    Boolean RecycleOnMeasure()
    //    {
    //        return mRecycleOnMeasure;
    //    }

    //protected:
    //    Boolean mRecycleOnMeasure;
    //};

    class AlertParams : public ElRefBase
    {
    public:
        /**
         * Interface definition for a callback to be invoked before the ListView
         * will be bound to an adapter.
         */
        interface IOnPrepareListViewListener : public IInterface
        {
            /**
             * Called before the ListView is bound to an adapter.
             * @param listView The ListView that will be shown in the dialog.
             */
            virtual CARAPI_(void) onPrepareListView(
                /* [in] */ IListView* listView) = 0;
        };

    public:
        AlertParams(
            /* [in] */ IContext* context);

        CARAPI_(void) Apply(
            /* [in] */ AlertController* dialog);

    private:
        CARAPI_(void) CreateListView(
            /* [in] */ AlertController* dialog);

    public:
        AutoPtr<IContext> mContext;
        AutoPtr<ILayoutInflater> mInflater;

        Int32 mIconId;
        AutoPtr<IDrawable> mIcon;
        AutoPtr<ICharSequence> mTitle;
        AutoPtr<IView> mCustomTitleView;
        AutoPtr<ICharSequence> mMessage;
        AutoPtr<ICharSequence> mPositiveButtonText;
        AutoPtr<IDialogInterfaceOnClickListener> mPositiveButtonListener;
        AutoPtr<ICharSequence> mNegativeButtonText;
        AutoPtr<IDialogInterfaceOnClickListener> mNegativeButtonListener;
        AutoPtr<ICharSequence> mNeutralButtonText;
        AutoPtr<IDialogInterfaceOnClickListener> mNeutralButtonListener;
        Boolean mCancelable;
        AutoPtr<IDialogInterfaceOnCancelListener> mOnCancelListener;
        AutoPtr<IDialogInterfaceOnKeyListener> mOnKeyListener;
        ArrayOf<ICharSequence*>* mItems;
        AutoPtr<IListAdapter> mAdapter;
        AutoPtr<IDialogInterfaceOnClickListener> mOnClickListener;
        AutoPtr<IView> mView;
        Int32 mViewSpacingLeft;
        Int32 mViewSpacingTop;
        Int32 mViewSpacingRight;
        Int32 mViewSpacingBottom;
        Boolean mViewSpacingSpecified;
        ArrayOf<Boolean>* mCheckedItems;
        Boolean mIsMultiChoice;
        Boolean mIsSingleChoice;
        Int32 mCheckedItem;
        AutoPtr<IDialogInterfaceOnMultiChoiceClickListener> mOnCheckboxClickListener;
        AutoPtr<ICursor> mCursor;
        String mLabelColumn;
        String mIsCheckedColumn;
        Boolean mForceInverseBackground;
        AutoPtr<IOnItemSelectedListener> mOnItemSelectedListener;
        AutoPtr<IOnPrepareListViewListener> mOnPrepareListViewListener;
        Boolean mRecycleOnMeasure;
    };

protected:
    class ButtonOnClickListener : public ElRefBase, public IViewOnClickListener
    {
    public:
        ButtonOnClickListener(
            /* [in] */ AlertController* host);

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
        AlertController* mHost;
    };

public:
    AlertController(
        /* [in] */ IContext* context,
        /* [in] */ IDialogInterface* di,
        /* [in] */ IWindow* window);

    CARAPI_(void) InstallContent();

    CARAPI_(void) SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI_(void) SetCustomTitle(
        /* [in] */ IView* customTitleView);

    CARAPI_(void) SetMessage(
        /* [in] */ ICharSequence* message);

    CARAPI_(void) SetView(
        /* [in] */ IView* view);

    CARAPI_(void) SetView(
        /* [in] */ IView* view,
        /* [in] */ Int32 viewSpacingLeft,
        /* [in] */ Int32 viewSpacingTop,
        /* [in] */ Int32 viewSpacingRight,
        /* [in] */ Int32 viewSpacingBottom);

    CARAPI SetButton(
        /* [in] */ Int32 whichButton,
        /* [in] */ ICharSequence* text,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI_(void) SetIcon(
        /* [in] */ Int32 resId);

    CARAPI_(void) SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI_(void) SetInverseBackgroundForced(
        /* [in] */ Boolean forceInverseBackground);

    CARAPI_(AutoPtr<IListView>) GetListView();

    CARAPI_(AutoPtr<IButton>) GetButton(
        /* [in] */ Int32 whichButton);

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

protected:
    static Boolean CanTextInput(
        /* [in] */ IView* v);

private:
    CARAPI_(void) SetupView();

    CARAPI_(Boolean) SetupTitle(
        /* [in] */ ILinearLayout* topPanel);

    CARAPI_(void) SetupContent(
        /* [in] */ ILinearLayout* contentPanel);

    CARAPI_(Boolean) SetupButtons();

    CARAPI_(void) CenterButton(
        /* [in] */ IButton* button);

    CARAPI_(void) SetBackground(
        /* [in] */ ILinearLayout* topPanel,
        /* [in] */ ILinearLayout* contentPanel,
        /* [in] */ IView* customPanel,
        /* [in] */ Boolean hasButtons,
        /* [in] */ ITypedArray* a,
        /* [in] */ Boolean hasTitle,
        /* [in] */ IView* buttonPanel);

protected:
    AutoPtr<ButtonOnClickListener> mButtonHandler;

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IDialogInterface> mDialogInterface;
    AutoPtr<IWindow> mWindow;

    AutoPtr<ICharSequence> mTitle;

    AutoPtr<ICharSequence> mMessage;

    AutoPtr<IListView> mListView;

    AutoPtr<IView> mView;

    Int32 mViewSpacingLeft;

    Int32 mViewSpacingTop;

    Int32 mViewSpacingRight;

    Int32 mViewSpacingBottom;

    Boolean mViewSpacingSpecified;

    AutoPtr<IButton> mButtonPositive;

    AutoPtr<ICharSequence> mButtonPositiveText;

    //Message mButtonPositiveMessage;
    AutoPtr<IDialogInterfaceOnClickListener> mButtonPositiveListener;

    AutoPtr<IButton> mButtonNegative;

    AutoPtr<ICharSequence> mButtonNegativeText;

    //Message mButtonNegativeMessage;
    AutoPtr<IDialogInterfaceOnClickListener> mButtonNegativeListener;

    AutoPtr<IButton> mButtonNeutral;

    AutoPtr<ICharSequence> mButtonNeutralText;

    //Message mButtonNeutralMessage;
    AutoPtr<IDialogInterfaceOnClickListener> mButtonNeutralListener;

    AutoPtr<IScrollView> mScrollView;

    Int32 mIconId;

    AutoPtr<IDrawable> mIcon;

    AutoPtr<IImageView> mIconView;

    AutoPtr<ITextView> mTitleView;

    AutoPtr<ITextView> mMessageView;

    AutoPtr<IView> mCustomTitleView;

    Boolean mForceInverseBackground;

    AutoPtr<IListAdapter> mAdapter;

    Int32 mCheckedItem;

    //Handler mHandler;
    AutoPtr<IApartment> mApartment;
};
#endif //__ALERTCONTROLLER_H__