
#ifndef __QWERTYKEYLISTENER_H__
#define __QWERTYKEYLISTENER_H__

#include "text/method/BaseKeyListener.h"
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>

extern "C" const InterfaceID EIID_Replaced;

class QwertyKeyListener : public BaseKeyListener
{
    friend class TextKeyListener;
protected:
    class Replaced : public ElRefBase, public INoCopySpan
    {
    public:
        Replaced(
            /* [in] */ ArrayOf<Char8>* text);

        ~Replaced();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    public:
        ArrayOf<Char8>* mText;
    };

public:
    QwertyKeyListener();

    static CARAPI_(AutoPtr<IQwertyKeyListener>) GetInstance(
        /* [in] */ Boolean autotext,
        /* [in] */ Capitalize cap);

    CARAPI_(Int32) GetInputType();
    
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    static CARAPI_(void) MarkAsReplaced(
        /* [in] */ ISpannable* content,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ String original);

protected:
    CARAPI Init(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

private:
    CARAPI_(String) GetReplacement(
        /* [in] */ ICharSequence* src,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IView* view);

    CARAPI_(Boolean) ShowCharacterPicker(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Char32 c,
        /* [in] */ Boolean insert,
        /* [in] */ Int32 count);

    static CARAPI_(String) ToTitleCase(
        /* [in] */ String src);

private:
    static const Int32 CAPITALIZELENGTH = 4;
    static AutoPtr<IQwertyKeyListener> sInstance[CAPITALIZELENGTH * 2];
    static HashMap<Char32, String> PICKER_SETS;

private:
    Capitalize mAutoCap;
    Boolean mAutoText;
};
#endif //__QWERTYKEYLISTENER_H__
