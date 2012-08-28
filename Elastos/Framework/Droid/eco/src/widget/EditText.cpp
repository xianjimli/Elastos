
#include "widget/EditText.h"

static const Int32 R_Attr_EditTextStyle = 0x0101006e;

EditText::EditText()
{

}

EditText::EditText(
    /* [in] */ IContext* context) : TextView(context, NULL, R_Attr_EditTextStyle)
{

}

EditText::EditText(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : TextView(context, attrs, R_Attr_EditTextStyle)
{

}

EditText::EditText(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle) : TextView(context, attrs, defStyle)
{

}

Boolean EditText::GetDefaultEditable() 
{
    return TRUE;
}

AutoPtr<IMovementMethod> EditText::GetDefaultMovementMethod()
{
    //return ArrowKeyMovementMethod.getInstance();
    return NULL;
}

AutoPtr<ICharSequence> EditText::GetText()
{
    return TextView::GetText();
}

//ECode EditText::SetText(
//    /* [in] */ ICharSequence* text, 
//    /* [in] */ BufferType type)
//{
//    return SetText(text, BufferType_EDITABLE);
//}

/**
 * Convenience for {@link Selection#setSelection(Spannable, Int32, Int32)}.
 */
ECode EditText::SetSelection(
    /* [in] */ Int32 start, 
    /* [in] */ Int32 stop) 
{
    //Selection.setSelection(getText(), start, stop);
    return NOERROR;
}

/**
 * Convenience for {@link Selection#setSelection(Spannable, Int32)}.
 */
ECode EditText::SetSelection(
    /* [in] */ Int32 index)
{
    //Selection.setSelection(getText(), index);
    return NOERROR;
}

/**
 * Convenience for {@link Selection#selectAll}.
 */
ECode EditText::SelectAll()
{
    //Selection.selectAll(getText());
    return NOERROR;
}

/**
 * Convenience for {@link Selection#extendSelection}.
 */
ECode EditText::ExtendSelection(
    /* [in] */ Int32 index)
{
    //Selection.extendSelection(getText(), index);
    return NOERROR;
}

ECode EditText::SetEllipsize(
    /* [in] */ TextUtilsTruncateAt ellipsis)
{
    if (ellipsis == TextUtilsTruncateAt_MARQUEE) {
        /*throw new IllegalArgumentException("EditText cannot use the ellipsize mode "
                + "TextUtils.TruncateAt.MARQUEE");*/
    }

    return TextView::SetEllipsize(ellipsis);
}
