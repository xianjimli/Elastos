
#ifndef __EDITTEXT_H__
#define __EDITTEXT_H__

#include "widget/TextView.h"

/*
 * This is supposed to be a *very* thin veneer over TextView.
 * Do not make any changes here that do anything that a TextView
 * with a key listener and a movement method wouldn't do!
 */

/**
 * EditText is a thin veneer over TextView that configures itself
 * to be editable.
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-formstuff.html">Form Stuff
 * tutorial</a>.</p>
 * <p>
 * <b>XML attributes</b>
 * <p>
 * See {@link android.R.styleable#EditText EditText Attributes},
 * {@link android.R.styleable#TextView TextView Attributes},
 * {@link android.R.styleable#View View Attributes}
 */
class EditText : public TextView
{
public:
    EditText(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x0101006e/*com.android.internal.R.attr.editTextStyle*/);

    //@Override
    CARAPI_(AutoPtr<ICharSequence>) GetText();

    using TextView::SetText;

    //@Override
    CARAPI SetText(
        /* [in] */ ICharSequence* text,
        /* [in] */ BufferType type);

    /**
     * Convenience for {@link Selection#setSelection(Spannable, Int32, Int32)}.
     */
    virtual CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    /**
     * Convenience for {@link Selection#setSelection(Spannable, Int32)}.
     */
    virtual CARAPI SetSelection(
        /* [in] */ Int32 index);

    /**
     * Convenience for {@link Selection#selectAll}.
     */
    virtual CARAPI SelectAll();

    /**
     * Convenience for {@link Selection#extendSelection}.
     */
    virtual CARAPI ExtendSelection(
        /* [in] */ Int32 index);

    //@Override
    CARAPI SetEllipsize(
        /* [in] */ TextUtilsTruncateAt ellipsis);

protected:
    EditText();

    //@Override
    CARAPI_(Boolean) GetDefaultEditable();

    //@Override
    CARAPI_(AutoPtr<IMovementMethod>) GetDefaultMovementMethod();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x0101006e/*com.android.internal.R.attr.editTextStyle*/);
};

#endif
