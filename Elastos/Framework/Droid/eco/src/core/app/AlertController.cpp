
#include "app/AlertController.h"
#include "os/CApartment.h"
#include "view/CViewGroupLayoutParams.h"
#include "widget/CLinearLayoutLayoutParams.h"
#include "text/TextUtils.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

AlertController::AlertParams::AlertParams(
    /* [in] */ IContext* context)
    : mContext(context)
    , mIconId(0)
    , mCancelable(TRUE)
    , mItems(NULL)
    , mViewSpacingSpecified(FALSE)
    , mCheckedItems(NULL)
    , mIsMultiChoice(FALSE)
    , mIsSingleChoice(FALSE)
    , mCheckedItem(-1)
    , mForceInverseBackground(FALSE)
    , mOnPrepareListViewListener(NULL)
    , mRecycleOnMeasure(TRUE)
{
    context->GetSystemService(
        Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
}

void AlertController::AlertController::AlertParams::Apply(
    /* [in] */ AlertController* dialog)
{
    if (mCustomTitleView != NULL) {
        dialog->SetCustomTitle(mCustomTitleView);
    }
    else {
        if (mTitle != NULL) {
            dialog->SetTitle(mTitle);
        }
        if (mIcon != NULL) {
            dialog->SetIcon(mIcon);
        }
        if (mIconId >= 0) {
            dialog->SetIcon(mIconId);
        }
    }

    if (mMessage != NULL) {
        dialog->SetMessage(mMessage);
    }
    if (mPositiveButtonText != NULL) {
        dialog->SetButton(DialogInterface_BUTTON_POSITIVE, mPositiveButtonText,
                mPositiveButtonListener);
    }
    if (mNegativeButtonText != NULL) {
        dialog->SetButton(DialogInterface_BUTTON_NEGATIVE, mNegativeButtonText,
                mNegativeButtonListener);
    }
    if (mNeutralButtonText != NULL) {
        dialog->SetButton(DialogInterface_BUTTON_NEUTRAL, mNeutralButtonText,
                mNeutralButtonListener);
    }
    if (mForceInverseBackground) {
        dialog->SetInverseBackgroundForced(TRUE);
    }
    // For a list, the client can either supply an array of items or an
    // adapter or a cursor
    if ((mItems != NULL) || (mCursor != NULL) || (mAdapter != NULL)) {
        CreateListView(dialog);
    }
    if (mView != NULL) {
        if (mViewSpacingSpecified) {
            dialog->SetView(mView, mViewSpacingLeft, mViewSpacingTop, mViewSpacingRight,
                    mViewSpacingBottom);
        }
        else {
            dialog->SetView(mView);
        }
    }

    /*
    dialog->SetCancelable(mCancelable);
    dialog->SetOnCancelListener(mOnCancelListener);
    if (mOnKeyListener != NULL) {
        dialog->SetOnKeyListener(mOnKeyListener);
    }
    */
}

void AlertController::AlertController::AlertParams::CreateListView(
    /* [in] */ AlertController* dialog)
{
//    final RecycleListView listView = (RecycleListView)
//            mInflater.inflate(R.layout.select_dialog, NULL);
//    ListAdapter adapter;

//    if (mIsMultiChoice) {
//        if (mCursor == NULL) {
//            adapter = new ArrayAdapter<CharSequence>(
//                    mContext, R.layout.select_dialog_multichoice, R.id.text1, mItems) {
//                @Override
//                AutoPtr<IView> getView(Int32 position, View convertView, ViewGroup parent) {
//                    /* [in] */ IView* view = super.getView(position, convertView, parent);
//                    if (mCheckedItems != NULL) {
//                        Boolean isItemChecked = mCheckedItems[position];
//                        if (isItemChecked) {
//                            listView->SetItemChecked(position, TRUE);
//                        }
//                    }
//                    return view;
//                }
//            };
//        } else {
//            adapter = new CursorAdapter(mContext, mCursor, FALSE) {
//                private final Int32 mLabelIndex;
//                private final Int32 mIsCheckedIndex;

//                {
//                    final Cursor cursor = getCursor();
//                    mLabelIndex = cursor.getColumnIndexOrThrow(mLabelColumn);
//                    mIsCheckedIndex = cursor.getColumnIndexOrThrow(mIsCheckedColumn);
//                }

//                @Override
//                public void AlertController::bindView(/* [in] */ IView* view, Context context, Cursor cursor) {
//                    CheckedTextView text = (CheckedTextView) view.findViewById(R.id.text1);
//                    text->SetText(cursor.getString(mLabelIndex));
//                    listView->SetItemChecked(cursor.getPosition(),
//                            cursor.getInt(mIsCheckedIndex) == 1);
//                }

//                @Override
//                AutoPtr<IView> newView(Context context, Cursor cursor, ViewGroup parent) {
//                    return mInflater.inflate(R.layout.select_dialog_multichoice,
//                            parent, FALSE);
//                }

//            };
//        }
//    } else {
//        Int32 layout = mIsSingleChoice
//                ? R.layout.select_dialog_singlechoice : R.layout.select_dialog_item;
//        if (mCursor == NULL) {
//            adapter = (mAdapter != NULL) ? mAdapter
//                    : new ArrayAdapter<CharSequence>(mContext, layout, R.id.text1, mItems);
//        } else {
//            adapter = new SimpleCursorAdapter(mContext, layout,
//                    mCursor, new String[]{mLabelColumn}, new Int32[]{R.id.text1});
//        }
//    }

//    if (mOnPrepareListViewListener != NULL) {
//        mOnPrepareListViewListener.onPrepareListView(listView);
//    }

//    /* Don't directly set the adapter on the ListView as we might
//     * want to add a footer to the ListView later.
//     */
//    dialog.mAdapter = adapter;
//    dialog.mCheckedItem = mCheckedItem;

//    if (mOnClickListener != NULL) {
//        listView->SetOnItemClickListener(new OnItemClickListener() {
//            public void AlertController::onItemClick(AdapterView parent, View v, Int32 position, long id) {
//                mOnClickListener.onClick(dialog.mDialogInterface, position);
//                if (!mIsSingleChoice) {
//                    dialog.mDialogInterface.dismiss();
//                }
//            }
//        });
//    } else if (mOnCheckboxClickListener != NULL) {
//        listView->SetOnItemClickListener(new OnItemClickListener() {
//            public void AlertController::onItemClick(AdapterView parent, View v, Int32 position, long id) {
//                if (mCheckedItems != NULL) {
//                    mCheckedItems[position] = listView.isItemChecked(position);
//                }
//                mOnCheckboxClickListener.onClick(
//                        dialog.mDialogInterface, position, listView.isItemChecked(position));
//            }
//        });
//    }

//    // Attach a given OnItemSelectedListener to the ListView
//    if (mOnItemSelectedListener != NULL) {
//        listView->SetOnItemSelectedListener(mOnItemSelectedListener);
//    }

//    if (mIsSingleChoice) {
//        listView->SetChoiceMode(ListView.CHOICE_MODE_SINGLE);
//    } else if (mIsMultiChoice) {
//        listView->SetChoiceMode(ListView.CHOICE_MODE_MULTIPLE);
//    }
//    listView.mRecycleOnMeasure = mRecycleOnMeasure;
//    dialog.mListView = listView;
}

AlertController::ButtonOnClickListener::ButtonOnClickListener(
    /* [in] */ AlertController* host)
    : mHost(host)
{
}

PInterface AlertController::ButtonOnClickListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnClickListener*)this;
    }
    else if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 AlertController::ButtonOnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AlertController::ButtonOnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AlertController::ButtonOnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnClickListener*)this) {
        *pIID = EIID_IViewOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

AlertController::ButtonOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    AutoPtr<IDialogInterfaceOnClickListener> listener;
    Int32 whichButton;
    if (v == mHost->mButtonPositive) {
        listener = mHost->mButtonPositiveListener;
        whichButton = DialogInterface_BUTTON_POSITIVE;
    }
    else if (v == mHost->mButtonNegative) {
        listener = mHost->mButtonNegativeListener;
        whichButton = DialogInterface_BUTTON_NEGATIVE;
    }
    else if (v == mHost->mButtonNeutral) {
        listener = mHost->mButtonNeutralListener;
        whichButton = DialogInterface_BUTTON_NEUTRAL;
    }

    if (listener != NULL) {
        ECode (STDCALL IDialogInterfaceOnClickListener::*pHandlerFunc)(
            IDialogInterface*, Int32);
        pHandlerFunc = &IDialogInterfaceOnClickListener::OnClick;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(mHost->mDialogInterface.Get());
        params->WriteInt32(whichButton);

        mHost->mApartment->PostCppCallback(
            (Handle32)listener.Get(), *(Handle32*)&pHandlerFunc, params, 0);
    }

    // Post a message so we dismiss after the above handlers are executed
    ECode (STDCALL IDialogInterface::*pHandlerFunc)();
    pHandlerFunc = &IDialogInterface::Dismiss;

    mHost->mApartment->PostCppCallback(
        (Handle32)mHost->mDialogInterface.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);

    return NOERROR;
}

AlertController::AlertController(
    /* [in] */ IContext* context,
    /* [in] */ IDialogInterface* di,
    /* [in] */ IWindow* window)
    : mContext(context)
    , mDialogInterface(di)
    , mWindow(window)
    , mViewSpacingSpecified(FALSE)
    , mIconId(-1)
    , mCheckedItem(-1)
{
    assert(SUCCEEDED(CApartment::GetDefaultApartment((IApartment**)&mApartment))
        && (mApartment != NULL));

    mButtonHandler = new ButtonOnClickListener(this);
}

Boolean AlertController::CanTextInput(
    /* [in] */ IView* v)
{
    Boolean isTextEditor;
    v->OnCheckIsTextEditor(&isTextEditor);
    if (isTextEditor) {
        return TRUE;
    }

    if (IViewGroup::Probe(v) == NULL) {
        return FALSE;
    }

    IViewGroup* vg = (IViewGroup*)v->Probe(EIID_IViewGroup);
    Int32 i;
    vg->GetChildCount(&i);
    while (i > 0) {
        i--;
        AutoPtr<IView> cv;
        vg->GetChildAt(i, (IView**)&cv);
        if (CanTextInput(cv)) {
            return TRUE;
        }
    }

    return FALSE;
}

void AlertController::InstallContent()
{
    /* We use a custom title so never request a window title */
    Boolean res;
    mWindow->RequestFeature(Window_FEATURE_NO_TITLE, &res);

    if (mView == NULL || !CanTextInput(mView)) {
        mWindow->SetFlags(WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM,
                WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM);
    }
    mWindow->SetContentView(0x01090016/*com.android.internal.R.layout.alert_dialog*/);
    SetupView();
}

void AlertController::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    if (mTitleView != NULL) {
        mTitleView->SetText(title);
    }
}

/**
 * @see AlertDialog.Builder#setCustomTitle(View)
 */
void AlertController::SetCustomTitle(
    /* [in] */ IView* customTitleView)
{
    mCustomTitleView = customTitleView;
}

void AlertController::SetMessage(
    /* [in] */ ICharSequence* message)
{
    mMessage = message;
    if (mMessageView != NULL) {
        mMessageView->SetText(message);
    }
}

/**
 * Set the view to display in the dialog.
 */
void AlertController::SetView(
    /* [in] */ IView* view)
{
    mView = view;
    mViewSpacingSpecified = FALSE;
}

/**
 * Set the view to display in the dialog along with the spacing around that view
 */
void AlertController::SetView(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSpacingLeft,
    /* [in] */ Int32 viewSpacingTop,
    /* [in] */ Int32 viewSpacingRight,
    /* [in] */ Int32 viewSpacingBottom)
{
    mView = view;
    mViewSpacingSpecified = TRUE;
    mViewSpacingLeft = viewSpacingLeft;
    mViewSpacingTop = viewSpacingTop;
    mViewSpacingRight = viewSpacingRight;
    mViewSpacingBottom = viewSpacingBottom;
}

/**
 * Sets a click listener or a message to be sent when the button is clicked.
 * You only need to pass one of {@code listener} or {@code msg}.
 *
 * @param whichButton Which button, can be one of
 *            {@link DialogInterface#BUTTON_POSITIVE},
 *            {@link DialogInterface#BUTTON_NEGATIVE}, or
 *            {@link DialogInterface#BUTTON_NEUTRAL}
 * @param text The text to display in positive button.
 * @param listener The {@link DialogInterface_OnClickListener} to use.
 * @param msg The {@link Message} to be sent when clicked.
 */
ECode AlertController::SetButton(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    switch (whichButton) {
        case DialogInterface_BUTTON_POSITIVE:
            mButtonPositiveText = text;
            mButtonPositiveListener = listener;
            break;
        case DialogInterface_BUTTON_NEGATIVE:
            mButtonNegativeText = text;
            mButtonNegativeListener = listener;
            break;
        case DialogInterface_BUTTON_NEUTRAL:
            mButtonNeutralText = text;
            mButtonNeutralListener = listener;
            break;
        default:
            Logger::E("AlertController", "Button does not exist");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

/**
 * Set resId to 0 if you don't want an icon.
 * @param resId the resourceId of the drawable to use as the icon or 0
 * if you don't want an icon.
 */
void AlertController::SetIcon(
    /* [in] */ Int32 resId)
{
    mIconId = resId;
    if (mIconView != NULL) {
        if (resId > 0) {
            mIconView->SetImageResource(mIconId);
        }
        else if (resId == 0) {
            mIconView->SetVisibility(View_GONE);
        }
    }
}

void AlertController::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIcon = icon;
    if ((mIconView != NULL) && (mIcon != NULL)) {
        mIconView->SetImageDrawable(icon);
    }
}

void AlertController::SetInverseBackgroundForced(
    /* [in] */ Boolean forceInverseBackground)
{
    mForceInverseBackground = forceInverseBackground;
}

AutoPtr<IListView> AlertController::GetListView()
{
    return mListView;
}

AutoPtr<IButton> AlertController::GetButton(
    /* [in] */ Int32 whichButton)
{
    switch (whichButton) {
    case DialogInterface_BUTTON_POSITIVE:
        return mButtonPositive;
    case DialogInterface_BUTTON_NEGATIVE:
        return mButtonNegative;
    case DialogInterface_BUTTON_NEUTRAL:
        return mButtonNeutral;
    default:
        return NULL;
    }
}

//@SuppressWarnings({"UnusedDeclaration"})
Boolean AlertController::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean res = FALSE;
    if (mScrollView != NULL) {
        mScrollView->ExecuteKeyEvent(event, &res);
    }

    return res;
}

//@SuppressWarnings({"UnusedDeclaration"})
Boolean AlertController::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean res = FALSE;
    if (mScrollView != NULL) {
        mScrollView->ExecuteKeyEvent(event, &res);
    }

    return res;
}

static Int32 R_Styleable_AlertDialog[] = {
    0x010100c6, 0x010100c7, 0x010100c8, 0x010100c9,
    0x010100ca, 0x010100cb, 0x010100cc, 0x010100cd,
    0x010100ce, 0x010100cf
};

void AlertController::SetupView()
{
    AutoPtr<ILinearLayout> contentPanel;
    mWindow->FindViewById(0x0102017f/*R.id.contentPanel*/, (IView**)&contentPanel);
    SetupContent(contentPanel);
    Boolean hasButtons = SetupButtons();

    AutoPtr<ILinearLayout> topPanel;
    mWindow->FindViewById(0x0102017b/*R.id.topPanel*/, (IView**)&topPanel);
    AutoPtr<ITypedArray> a;
    mContext->ObtainStyledAttributesEx3(
        NULL, ArrayOf<Int32>(R_Styleable_AlertDialog,/*com.android.internal.R.styleable.AlertDialog*/
        sizeof(R_Styleable_AlertDialog) / sizeof(Int32)),
        0x0101005d/*com.android.internal.R.attr.alertDialogStyle*/, 0,
        (ITypedArray**)&a);
    Boolean hasTitle = SetupTitle(topPanel);

    AutoPtr<IView> buttonPanel;
    mWindow->FindViewById(0x01020182/*R.id.buttonPanel*/, (IView**)&buttonPanel);
    if (!hasButtons) {
        buttonPanel->SetVisibility(View_GONE);
    }

    AutoPtr<IFrameLayout> customPanel;
    if (mView != NULL) {
        mWindow->FindViewById(0x01020181/*R.id.customPanel*/, (IView**)&customPanel);
        AutoPtr<IFrameLayout> custom;
        mWindow->FindViewById(0x0102002b/*R.id.custom*/, (IView**)&custom);

        AutoPtr<IViewGroupLayoutParams> lp;
        CViewGroupLayoutParams::New(
            ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_MATCH_PARENT,
            (IViewGroupLayoutParams**)&lp);
        custom->AddViewEx3(mView, lp);
        if (mViewSpacingSpecified) {
            custom->SetPadding(
                mViewSpacingLeft, mViewSpacingTop,
                mViewSpacingRight, mViewSpacingBottom);
        }

        if (mListView != NULL) {
            AutoPtr<ILinearLayoutLayoutParams> l;
            customPanel->GetLayoutParams((IViewGroupLayoutParams**)&l);
            ((CLinearLayoutLayoutParams*)l.Get())->mWeight = 0;
        }
    }
    else {
        mWindow->FindViewById(0x01020181/*R.id.customPanel*/, (IView**)&customPanel);
        customPanel->SetVisibility(View_GONE);
        customPanel = NULL;
    }

    /* Only display the divider if we have a title and a
     * custom view or a message.
     */
    if (hasTitle && ((mMessage != NULL) || (mView != NULL))) {
        AutoPtr<IView> divider;
        mWindow->FindViewById(0x0102017e/*R.id.titleDivider*/, (IView**)&divider);
        divider->SetVisibility(View_VISIBLE);
    }

    SetBackground(topPanel, contentPanel, customPanel, hasButtons, a, hasTitle, buttonPanel);
    a->Recycle();
}

Boolean AlertController::SetupTitle(
    /* [in] */ ILinearLayout* topPanel)
{
    Boolean hasTitle = TRUE;

    if (mCustomTitleView != NULL) {
        // Add the custom title view directly to the topPanel layout
        AutoPtr<ILinearLayoutLayoutParams> lp;
        CLinearLayoutLayoutParams::New(
            ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_WRAP_CONTENT,
            (ILinearLayoutLayoutParams**)&lp);

        topPanel->AddViewEx3(mCustomTitleView, (IViewGroupLayoutParams*)lp.Get());

        // Hide the title template
        AutoPtr<IView> titleTemplate;
        mWindow->FindViewById(
            0x0102017c/*R.id.title_template*/, (IView**)&titleTemplate);
        titleTemplate->SetVisibility(View_GONE);
    }
    else {
        Boolean hasTextTitle = !TextUtils::IsEmpty(mTitle);

        mWindow->FindViewById(0x01010002/*R.id.icon*/, (IView**)&mIconView);
        if (hasTextTitle) {
            /* Display the title if a title is supplied, else hide it */
            mWindow->FindViewById(0x0102017d/*R.id.alertTitle*/, (IView**)&mTitleView);

            mTitleView->SetText(mTitle);

            /* Do this last so that if the user has supplied any
             * icons we use them instead of the default ones. If the
             * user has specified 0 then make it disappear.
             */
            if (mIconId > 0) {
                mIconView->SetImageResource(mIconId);
            }
            else if (mIcon != NULL) {
                mIconView->SetImageDrawable(mIcon);
            }
            else if (mIconId == 0) {
                /* Apply the padding from the icon to ensure the
                 * title is aligned correctly.
                 */
                Int32 left, top, right, bottom;
                mIconView->GetPaddingLeft(&left);
                mIconView->GetPaddingTop(&top);
                mIconView->GetPaddingRight(&right);
                mIconView->GetPaddingBottom(&bottom);
                mTitleView->SetPadding(left, top, right, bottom);
                mIconView->SetVisibility(View_GONE);
            }
        }
        else {
            // Hide the title template
            AutoPtr<IView> titleTemplate;
            mWindow->FindViewById(
                0x0102017c/*R.id.title_template*/, (IView**)&titleTemplate);
            titleTemplate->SetVisibility(View_GONE);
            mIconView->SetVisibility(View_GONE);
            hasTitle = FALSE;
        }
    }
    return hasTitle;
}

void AlertController::SetupContent(
    /* [in] */ ILinearLayout* contentPanel)
{
    mWindow->FindViewById(0x01020180/*R.id.scrollView*/, (IView**)&mScrollView);
    mScrollView->SetFocusable(FALSE);

    // Special case for users that only want to display a String
    mWindow->FindViewById(0x0102000b/*R.id.message*/, (IView**)&mMessageView);
    if (mMessageView == NULL) {
        return;
    }

    if (mMessage != NULL) {
        mMessageView->SetText(mMessage);
    }
    else {
        mMessageView->SetVisibility(View_GONE);
        IViewManager::Probe(mScrollView)->RemoveView(mMessageView);

        if (mListView != NULL) {
            IViewManager::Probe(contentPanel)->RemoveView(mScrollView.Get());
            AutoPtr<ILinearLayoutLayoutParams> lp;
            CLinearLayoutLayoutParams::New(
                ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_MATCH_PARENT,
                (ILinearLayoutLayoutParams**)&lp);
            contentPanel->AddViewEx3(mListView, (IViewGroupLayoutParams*)lp.Get());

            lp = NULL;
            CLinearLayoutLayoutParams::New(
                ViewGroupLayoutParams_MATCH_PARENT, 0, 1.0f,
                (ILinearLayoutLayoutParams**)&lp);
            contentPanel->SetLayoutParams((IViewGroupLayoutParams*)lp.Get());
        }
        else {
            contentPanel->SetVisibility(View_GONE);
        }
    }
}

Boolean AlertController::SetupButtons()
{
    Int32 BIT_BUTTON_POSITIVE = 1;
    Int32 BIT_BUTTON_NEGATIVE = 2;
    Int32 BIT_BUTTON_NEUTRAL = 4;
    Int32 whichButtons = 0;
    mWindow->FindViewById(0x01020019/*R.id.button1*/, (IView**)&mButtonPositive);
    mButtonPositive->SetOnClickListener(mButtonHandler);

    if (TextUtils::IsEmpty(mButtonPositiveText)) {
        mButtonPositive->SetVisibility(View_GONE);
    }
    else {
        mButtonPositive->SetText(mButtonPositiveText);
        mButtonPositive->SetVisibility(View_VISIBLE);
        whichButtons = whichButtons | BIT_BUTTON_POSITIVE;
    }

    mWindow->FindViewById(0x0102001a/*R.id.button2*/, (IView**)&mButtonNegative);
    mButtonNegative->SetOnClickListener(mButtonHandler);

    if (TextUtils::IsEmpty(mButtonNegativeText)) {
        mButtonNegative->SetVisibility(View_GONE);
    }
    else {
        mButtonNegative->SetText(mButtonNegativeText);
        mButtonNegative->SetVisibility(View_VISIBLE);

        whichButtons = whichButtons | BIT_BUTTON_NEGATIVE;
    }

    mWindow->FindViewById(0x0102001b/*R.id.button3*/, (IView**)&mButtonNeutral);
    mButtonNeutral->SetOnClickListener(mButtonHandler);

    if (TextUtils::IsEmpty(mButtonNeutralText)) {
        mButtonNeutral->SetVisibility(View_GONE);
    }
    else {
        mButtonNeutral->SetText(mButtonNeutralText);
        mButtonNeutral->SetVisibility(View_VISIBLE);

        whichButtons = whichButtons | BIT_BUTTON_NEUTRAL;
    }

    /*
     * If we only have 1 button it should be centered on the layout and
     * expand to fill 50% of the available space.
     */
    if (whichButtons == BIT_BUTTON_POSITIVE) {
        CenterButton(mButtonPositive);
    }
    else if (whichButtons == BIT_BUTTON_NEGATIVE) {
        CenterButton(mButtonNeutral);
    }
    else if (whichButtons == BIT_BUTTON_NEUTRAL) {
        CenterButton(mButtonNeutral);
    }

    return whichButtons != 0;
}

void AlertController::CenterButton(
    /* [in] */ IButton* button)
{
    AutoPtr<ILinearLayoutLayoutParams> params;
    button->GetLayoutParams((IViewGroupLayoutParams**)&params);
    ((CLinearLayoutLayoutParams*)params.Get())->mGravity = Gravity_CENTER_HORIZONTAL;
    ((CLinearLayoutLayoutParams*)params.Get())->mWeight = 0.5f;
    button->SetLayoutParams(params);
    AutoPtr<IView> leftSpacer;
    mWindow->FindViewById(0x01020183/*R.id.leftSpacer*/, (IView**)&leftSpacer);
    leftSpacer->SetVisibility(View_VISIBLE);
    AutoPtr<IView> rightSpacer;
    mWindow->FindViewById(0x01020184/*R.id.rightSpacer*/, (IView**)&rightSpacer);
    rightSpacer->SetVisibility(View_VISIBLE);
}

void AlertController::SetBackground(
    /* [in] */ ILinearLayout* topPanel,
    /* [in] */ ILinearLayout* contentPanel,
    /* [in] */ IView* customPanel,
    /* [in] */ Boolean hasButtons,
    /* [in] */ ITypedArray* a,
    /* [in] */ Boolean hasTitle,
    /* [in] */ IView* buttonPanel)
{
    /* Get all the different background required */
    Int32 fullDark;
    a->GetResourceId(
        0/*R.styleable.AlertDialog_fullDark*/,
        0x01080249/*R.drawable.popup_full_dark*/, &fullDark);
    Int32 topDark;
    a->GetResourceId(
        1/*R.styleable.AlertDialog_topDark*/,
        0x0108024d/*R.drawable.popup_top_dark*/, &topDark);
    Int32 centerDark;
    a->GetResourceId(
        2/*R.styleable.AlertDialog_centerDark*/,
        0x01080246/*R.drawable.popup_center_dark*/, &centerDark);
    Int32 bottomDark;
    a->GetResourceId(
        3/*R.styleable.AlertDialog_bottomDark*/,
        0x01080243/*R.drawable.popup_bottom_dark*/, &bottomDark);
    Int32 fullBright;
    a->GetResourceId(
        4/*R.styleable.AlertDialog_fullBright*/,
        0x01080248/*R.drawable.popup_full_bright*/, &fullBright);
    Int32 topBright;
    a->GetResourceId(
        5/*R.styleable.AlertDialog_topBright*/,
        0x0108024c/*R.drawable.popup_top_bright*/, &topBright);
    Int32 centerBright;
    a->GetResourceId(
        6/*R.styleable.AlertDialog_centerBright*/,
        0x01080245/*R.drawable.popup_center_bright*/, &centerBright);
    Int32 bottomBright;
    a->GetResourceId(
        7/*R.styleable.AlertDialog_bottomBright*/,
        0x01080242/*R.drawable.popup_bottom_bright*/, &bottomBright);
    Int32 bottomMedium;
    a->GetResourceId(
        8/*R.styleable.AlertDialog_bottomMedium*/,
        0x01080244/*R.drawable.popup_bottom_medium*/, &bottomMedium);

    /*
     * We now set the background of all of the sections of the alert.
     * First collect together each section that is being displayed along
     * with whether it is on a light or dark background, then run through
     * them setting their backgrounds.  This is complicated because we need
     * to correctly use the full, top, middle, and bottom graphics depending
     * on how many views they are and where they appear.
     */
    ArrayOf_<IView*, 4> views;
    Boolean light[4];
    IView* lastView;
    Boolean lastLight = FALSE;

    Int32 pos = 0;
    if (hasTitle) {
        views[pos] = topPanel;
        light[pos] = FALSE;
        pos++;
    }

    /* The contentPanel displays either a custom text message or
     * a ListView. If it's text we should use the dark background
     * for ListView we should use the light background. If neither
     * are there the contentPanel will be hidden so set it as NULL.
     */
    Int32 visibility;
    contentPanel->GetVisibility(&visibility);
    views[pos] = (visibility == View_GONE) ? NULL : contentPanel;
    light[pos] = mListView != NULL;
    pos++;
    if (customPanel != NULL) {
        views[pos] = customPanel;
        light[pos] = mForceInverseBackground;
        pos++;
    }
    if (hasButtons) {
        views[pos] = buttonPanel;
        light[pos] = TRUE;
    }

    Boolean setView = FALSE;
    for (pos = 0; pos < views.GetLength(); pos++) {
        IView* v = views[pos];
        if (v == NULL) {
            continue;
        }
        if (lastView != NULL) {
            if (!setView) {
                lastView->SetBackgroundResource(lastLight ? topBright : topDark);
            }
            else {
                lastView->SetBackgroundResource(lastLight ? centerBright : centerDark);
            }
            setView = TRUE;
        }
        lastView = v;
        lastLight = light[pos];
    }

    if (lastView != NULL) {
        if (setView) {

            /* ListViews will use the Bright background but buttons use
             * the Medium background.
             */
            lastView->SetBackgroundResource(
                    lastLight ? (hasButtons ? bottomMedium : bottomBright) : bottomDark);
        }
        else {
            lastView->SetBackgroundResource(lastLight ? fullBright : fullDark);
        }
    }

    /* TODO: uncomment section below. The logic for this should be if
     * it's a Contextual menu being displayed AND only a Cancel button
     * is shown then do this.
     */
//        if (hasButtons && (mListView != NULL)) {

        /* Yet another *special* case. If there is a ListView with buttons
         * don't put the buttons on the bottom but instead put them in the
         * footer of the ListView this will allow more items to be
         * displayed.
         */

        /*
        contentPanel->SetBackgroundResource(bottomBright);
        buttonPanel->SetBackgroundResource(centerMedium);
        ViewGroup parent = (ViewGroup) mWindow.findViewById(R.id.parentPanel);
        parent.removeView(buttonPanel);
        AbsListView.LayoutParams params = new AbsListView.LayoutParams(
                AbsListView.LayoutParams.MATCH_PARENT,
                AbsListView.LayoutParams.MATCH_PARENT);
        buttonPanel->SetLayoutParams(params);
        mListView.addFooterView(buttonPanel);
        */
//        }

    if ((mListView != NULL) && (mAdapter != NULL)) {
        mListView->SetAdapter(mAdapter);
        if (mCheckedItem > -1) {
            mListView->SetItemChecked(mCheckedItem, TRUE);
            mListView->SetSelection(mCheckedItem);
        }
    }
}
