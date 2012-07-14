
#ifndef __ALERTDIALOG_H__
#define __ALERTDIALOG_H__

#include "app/Dialog.h"
#include "app/AlertController.h"

class AlertDialog : public Dialog
{
    friend class CAlertDialogBuilder;
public:
    AlertDialog();

    AlertDialog(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = 0x010300a3/*com.android.internal.R.style.Theme_Dialog_Alert*/);

    AlertDialog(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

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
    CARAPI_(AutoPtr<IButton>) GetButton(
        /* [in] */ Int32 whichButton);

    /**
     * Gets the list view used in the dialog.
     *
     * @return The {@link ListView} from the dialog.
     */
    CARAPI_(AutoPtr<IListView>) GetListView();

    //@Override
    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetTitle(
        /* [in] */ Int32 titleId);

    /**
     * @see Builder#setCustomTitle(View)
     */
    CARAPI SetCustomTitle(
        /* [in] */ IView* customTitleView);

    CARAPI SetMessage(
        /* [in] */ ICharSequence* message);

    /**
     * Set the view to display in that dialog.
     */
    CARAPI SetView(
        /* [in] */ IView* view);

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
    CARAPI SetView(
        /* [in] */ IView* view,
        /* [in] */ Int32 viewSpacingLeft,
        /* [in] */ Int32 viewSpacingTop,
        /* [in] */ Int32 viewSpacingRight,
        /* [in] */ Int32 viewSpacingBottom);

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
    CARAPI SetButton(
        /* [in] */ Int32 whichButton,
        /* [in] */ ICharSequence* text);

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
    CARAPI SetButton(
        /* [in] */ Int32 whichButton,
        /* [in] */ ICharSequence* text,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    /**
     * Set resId to 0 if you don't want an icon.
     * @param resId the resourceId of the drawable to use as the icon or 0
     * if you don't want an icon.
     */
    CARAPI SetIcon(
        /* [in] */ Int32 resId);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetInverseBackgroundForced(
        /* [in] */ Boolean forceInverseBackground);

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

protected:
    void OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = 0x010300a3/*com.android.internal.R.style.Theme_Dialog_Alert*/);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

private:
    AutoPtr<AlertController> mAlert;
};
#endif //__ALERTDIALOG_H__
