
#ifndef __NUMBERKEYLISTENER_H__
#define __NUMBERKEYLISTENER_H__

#include "text/method/BaseKeyListener.h"
#include <elastos/ElRefBase.h>

/**
 * For numeric text entry
 */
class NumberKeyListener
        : public BaseKeyListener
        , public IInputFilter
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

    virtual CARAPI Filter(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ISpanned* dest,
        /* [in] */ Int32 dstart,
        /* [in] */ Int32 dend,
        /*[out] */ ICharSequence** cs);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

protected:
    /**
     * You can say which characters you can accept.
     */
    virtual CARAPI_(ArrayOf<Char32>*) GetAcceptedChars() = 0;

    virtual CARAPI_(Int32) Lookup(
        /* [in] */ IKeyEvent* event,
        /* [in] */ ISpannable* content);

    static CARAPI_(Boolean) Ok(
        /* [in] */ ArrayOf<Char32>* accept,
        /* [in] */ Char32 c);
};

#endif
