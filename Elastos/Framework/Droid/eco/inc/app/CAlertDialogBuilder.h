
#ifndef __CALERTDIALOGBUILDER_H__
#define __CALERTDIALOGBUILDER_H__

#include "_CAlertDialogBuilder.h"
#include "app/AlertController.h"

CarClass(CAlertDialogBuilder)
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SetTitle(
        /* [in] */ Int32 titleId);

    CARAPI SetTitleEx(
        /* [in] */ ICharSequence* title);

    CARAPI SetCustomTitle(
        /* [in] */ IView* customTitleView);

    CARAPI SetMessage(
        /* [in] */ Int32 messageId);

    CARAPI SetMessageEx(
        /* [in] */ ICharSequence* message);

    CARAPI SetIcon(
        /* [in] */ Int32 iconId);

    CARAPI SetIconEx(
        /* [in] */ IDrawable* icon);

    CARAPI SetPositiveButton(
        /* [in] */ Int32 textId,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetPositiveButtonEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetNegativeButton(
        /* [in] */ Int32 textId,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetNegativeButtonEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetNeutralButton(
        /* [in] */ Int32 textId,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetNeutralButtonEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetCancelable(
        /* [in] */ Boolean cancelable);

    CARAPI SetOnCancelListener(
        /* [in] */ IDialogInterfaceOnCancelListener* onCancelListener);

    CARAPI SetOnKeyListener(
        /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener);

    CARAPI SetItems(
        /* [in] */ Int32 itemsId,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetItemsEx(
        /* [in] */ const ArrayOf<ICharSequence*>& items,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetCursor(
        /* [in] */ ICursor* cursor,
        /* [in] */ IDialogInterfaceOnClickListener* listener,
        /* [in] */ const String& labelColumn);

    CARAPI SetMultiChoiceItems(
        /* [in] */ Int32 itemsId,
        /* [in] */ const ArrayOf<Boolean>& checkedItems,
        /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener);

    CARAPI SetMultiChoiceItemsEx(
        /* [in] */ const ArrayOf<ICharSequence*>& items,
        /* [in] */ const ArrayOf<Boolean>& checkedItems,
        /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener);

    CARAPI SetMultiChoiceItemsEx2(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& isCheckedColumn,
        /* [in] */ const String& labelColumn,
        /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener);

    CARAPI SetSingleChoiceItems(
        /* [in] */ Int32 itemsId,
        /* [in] */ Int32 checkedItem,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetSingleChoiceItemsEx(
        /* [in] */ ICursor* cursor,
        /* [in] */ Int32 checkedItem,
        /* [in] */ const String& labelColumn,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetSingleChoiceItemsEx2(
        /* [in] */ const ArrayOf<ICharSequence*>& items,
        /* [in] */ Int32 checkedItem,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetSingleChoiceItemsEx3(
        /* [in] */ IListAdapter* adapter,
        /* [in] */ Int32 checkedItem,
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI SetOnItemSelectedListener(
        /* [in] */ IOnItemSelectedListener* listener);

    CARAPI SetView(
        /* [in] */ IView* view);

    CARAPI SetViewEx(
        /* [in] */ IView* view,
        /* [in] */ Int32 viewSpacingLeft,
        /* [in] */ Int32 viewSpacingTop,
        /* [in] */ Int32 viewSpacingRight,
        /* [in] */ Int32 viewSpacingBottom);

    CARAPI SetInverseBackgroundForced(
        /* [in] */ Boolean useInverseBackground);

    CARAPI SetRecycleOnMeasureEnabled(
        /* [in] */ Boolean enabled);

    CARAPI Create(
        /* [out] */ IAlertDialog** dialog);

    CARAPI Show(
        /* [out] */ IAlertDialog** dialog);

private:
    AutoPtr<AlertController::AlertParams> mP;
};

#endif // __CALERTDIALOGBUILDER_H__
