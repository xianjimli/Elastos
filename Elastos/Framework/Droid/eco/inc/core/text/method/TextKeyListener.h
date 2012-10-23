
#ifndef __TEXTKEYLISTENER_H__
#define __TEXTKEYLISTENER_H__

#include "text/method/BaseKeyListener.h"
#include "database/ContentObserver.h"
#include <elastos/ElRefBase.h>

class TextKeyListener : public BaseKeyListener
{
    friend class QwertyKeyListener;
private:
    class NullKeyListener : public ElRefBase, public IKeyListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetInputType(
            /* [out] */ Int32* inputType);

        CARAPI OnKeyDown(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI OnKeyUp(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI OnKeyOther(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* res);

        CARAPI ClearMetaKeyState(
            /* [in] */ IView* view,
            /* [in] */ IEditable* content,
            /* [in] */ Int32 states);

        static AutoPtr<NullKeyListener> GetInstance();

    private:
        static AutoPtr<NullKeyListener> sInstance;
    };

    class SettingsObserver : public ElRefBase
        , public ContentObserver, public IContentObserver
    {
    public:
        SettingsObserver(
            /* [in] */ TextKeyListener* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        TextKeyListener* mHost;
    };

public:
    TextKeyListener();

    static CARAPI_(AutoPtr<ITextKeyListener>) GetInstance(
        /* [in] */ Boolean autotext,
        /* [in] */ Capitalize cap);

    static CARAPI_(AutoPtr<ITextKeyListener>) GetInstance();

    static CARAPI_(Boolean) ShouldCap(
        /* [in] */ Capitalize cap,
        /* [in] */ ICharSequence* cs,
        /* [in] */ Int32 off);

    CARAPI_(Int32) GetInputType();

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnKeyOther(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ IKeyEvent* event);

    static CARAPI_(void) Clear(
        /* [in] */ IEditable* e);

    CARAPI OnSpanAdded(
        /* [in] */ ISpannable* s,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanRemoved(
        /* [in] */ ISpannable* s,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanChanged(
        /* [in] */ ISpannable* s,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 st,
        /* [in] */ Int32 en);

    CARAPI ReleaseListener();

    CARAPI_(Int32) GetPrefs(
        /* [in] */ IContext* context);

protected:
    CARAPI Init(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

private:
    CARAPI_(AutoPtr<IKeyListener>) GetKeyListener(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) InitPrefs(
        /* [in] */ IContext* context);

    CARAPI_(void) UpdatePrefs(
        /* [in] */ IContentResolver* resolver);

protected:
    static const AutoPtr<IInterface> ACTIVE;
    static const AutoPtr<IInterface> CAPPED;
    static const AutoPtr<IInterface> INHIBIT_REPLACEMENT;
    static const AutoPtr<IInterface> LAST_TYPED;

    static const Int32 AUTO_CAP = 1;
    static const Int32 AUTO_TEXT = 2;
    static const Int32 AUTO_PERIOD = 4;
    static const Int32 SHOW_PASSWORD = 8;

private:
    static const Int32 CAPITALIZELENGTH = 4;
    static AutoPtr<ITextKeyListener> sInstance[CAPITALIZELENGTH * 2];

private:
    Capitalize mAutoCap;
    Boolean mAutoText;

    Mutex mPrefsLock;
    Int32 mPrefs;
    Boolean mPrefsInited;

    //WeakReference<ContentResolver> mResolver;
    AutoPtr<IContentResolver> mResolver;
    AutoPtr<SettingsObserver> mObserver;
};
#endif //__TEXTKEYLISTENER_H__
