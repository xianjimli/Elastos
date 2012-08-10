
#include "app/AlertDialog.h"

AlertDialog::AlertDialog()
{}

AlertDialog::AlertDialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    Init(context, theme);
}

AlertDialog::AlertDialog(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    Init(context, cancelable, cancelListener);
}

/**
 * Gets one of the buttons used in the dialog.
 * <p>
 * If a button does not exist in the dialog, NULL will be returned.
 *
 * @param whichButton The identifier of the button that should be returned.
 *            For example, this can be
 *            {@link DialogInterface#BUTTON_POSITIVE}.
 * @return The button from the dialog, or NULL if a button does not exist.
 */
AutoPtr<IButton> AlertDialog::GetButton(
    /* [in] */ Int32 whichButton)
{
    return mAlert->GetButton(whichButton);
}

/**
 * Gets the list view used in the dialog.
 *
 * @return The {@link ListView} from the dialog.
 */
AutoPtr<IListView> AlertDialog::GetListView()
{
    return mAlert->GetListView();
}

//@Override
ECode AlertDialog::SetTitle(
    /* [in] */ ICharSequence* title)
{
    Dialog::SetTitle(title);
    mAlert->SetTitle(title);

    return NOERROR;
}

/**
 * @see Builder#setCustomTitle(View)
 */
ECode AlertDialog::SetCustomTitle(
    /* [in] */ IView* customTitleView)
{
    mAlert->SetCustomTitle(customTitleView);

    return NOERROR;
}

ECode AlertDialog::SetMessage(
    /* [in] */ ICharSequence* message)
{
    mAlert->SetMessage(message);

    return NOERROR;
}

/**
 * Set the view to display in that dialog.
 */
ECode AlertDialog::SetView(
    /* [in] */ IView* view)
{
    mAlert->SetView(view);

    return NOERROR;
}

/**
 * Set the view to display in that dialog, specifying the spacing to appear around that
 * view.
 *
 * @param view The view to show in the content area of the dialog
 * @param viewSpacingLeft Extra space to appear to the left of {@code view}
 * @param viewSpacingTop Extra space to appear above {@code view}
 * @param viewSpacingRight Extra space to appear to the right of {@code view}
 * @param viewSpacingBottom Extra space to appear below {@code view}
 */
ECode AlertDialog::SetView(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSpacingLeft,
    /* [in] */ Int32 viewSpacingTop,
    /* [in] */ Int32 viewSpacingRight,
    /* [in] */ Int32 viewSpacingBottom)
{
    mAlert->SetView(view, viewSpacingLeft, viewSpacingTop, viewSpacingRight, viewSpacingBottom);

    return NOERROR;
}

/**
 * Set a message to be sent when a button is pressed.
 *
 * @param whichButton Which button to set the message for, can be one of
 *            {@link DialogInterface#BUTTON_POSITIVE},
 *            {@link DialogInterface#BUTTON_NEGATIVE}, or
 *            {@link DialogInterface#BUTTON_NEUTRAL}
 * @param text The text to display in positive button.
 * @param msg The {@link Message} to be sent when clicked.
 */
ECode AlertDialog::SetButton(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text)
{
    mAlert->SetButton(whichButton, text, NULL);

    return NOERROR;
}

/**
 * Set a listener to be invoked when the positive button of the dialog is pressed.
 *
 * @param whichButton Which button to set the listener on, can be one of
 *            {@link DialogInterface#BUTTON_POSITIVE},
 *            {@link DialogInterface#BUTTON_NEGATIVE}, or
 *            {@link DialogInterface#BUTTON_NEUTRAL}
 * @param text The text to display in positive button.
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 */
ECode AlertDialog::SetButton(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mAlert->SetButton(whichButton, text, listener);

    return NOERROR;
}

/**
 * Set resId to 0 if you don't want an icon.
 * @param resId the resourceId of the drawable to use as the icon or 0
 * if you don't want an icon.
 */
ECode AlertDialog::SetIcon(
    /* [in] */ Int32 resId)
{
    mAlert->SetIcon(resId);

    return NOERROR;
}

ECode AlertDialog::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mAlert->SetIcon(icon);

    return NOERROR;
}

ECode AlertDialog::SetInverseBackgroundForced(
    /* [in] */ Boolean forceInverseBackground)
{
    mAlert->SetInverseBackgroundForced(forceInverseBackground);

    return NOERROR;
}

//@Override
void AlertDialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Dialog::OnCreate(savedInstanceState);
    mAlert->InstallContent();
}

//@Override
Boolean AlertDialog::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mAlert->OnKeyDown(keyCode, event)) {
        return TRUE;
    }

    return Dialog::OnKeyDown(keyCode, event);
}

//@Override
Boolean AlertDialog::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mAlert->OnKeyUp(keyCode, event)) {
        return TRUE;
    }

    return Dialog::OnKeyUp(keyCode, event);
}

ECode AlertDialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    FAIL_RETURN(Dialog::Init(context, theme));

    mAlert = new AlertController(
        context, (IDialogInterface*)this->Probe(EIID_IDialogInterface), GetWindow());

    return NOERROR;
}

ECode AlertDialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    FAIL_RETURN(Dialog::Init(context, 0x010300a3/*com.android.internal.R.style.Theme_Dialog_Alert*/));

    SetCancelable(cancelable);
    SetOnCancelListener(cancelListener);
    mAlert = new AlertController(
        context, (IDialogInterface*)this->Probe(EIID_IDialogInterface), GetWindow());

    return NOERROR;
}
