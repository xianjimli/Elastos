
#ifndef __CLICKABLESPAN_H__
#define __CLICKABLESPAN_H__

#include "ext/frameworkext.h"
#include "text/style/CharacterStyle.h"

/**
 * If an object of this type is attached to the text of a TextView
 * with a movement method of LinkMovementMethod, the affected spans of
 * text can be selected.  If clicked, the {@link #onClick} method will
 * be called.
 */
class ClickableSpan : public CharacterStyle
{
public:
    /**
     * Performs the click action associated with this span.
     */
    virtual CARAPI OnClick(
        /* [in] */ IView* widget) = 0;

    /**
     * Makes the text underlined and in the link color.
     */
    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);
};

#endif //__CLICKABLESPAN_H__
