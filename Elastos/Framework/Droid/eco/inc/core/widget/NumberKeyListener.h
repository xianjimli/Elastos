
#ifndef __NUMBERKEYLISTENER_H__
#define __NUMBERKEYLISTENER_H__

#include "text/method/BaseKeyListener.h"

/**
 * For numeric text entry
 */
class NumberKeyListener : public BaseKeyListener
{
public:
    virtual CARAPI_(AutoPtr<ICharSequence>) Filter(
        /* [in] */ ICharSequence* source, 
        /* [in] */ Int32 start, 
        /* [in] */ Int32 end,
        /* [in] */ ISpanned* dest, 
        /* [in] */ Int32 dstart, 
        /* [in] */ Int32 dend);
    
    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view, 
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode, 
        /* [in] */ IKeyEvent* event);

protected:
    /**
     * You can say which characters you can accept.
     */
    virtual CARAPI_(Char16*) GetAcceptedChars() = 0;

    virtual CARAPI_(Int32) Lookup(
        /* [in] */ IKeyEvent* event, 
        /* [in] */ ISpannable* content);

    static CARAPI_(Boolean) Ok(
        /* [in] */ ArrayOf<Char16>* accept, 
        /* [in] */ Char16 c);
};

#endif
