
#ifndef  _ABSTRACTINPUTMETHODSESSIONIMPL_H__
#define  _ABSTRACTINPUTMETHODSESSIONIMPL_H__

#include <elastos/ElRefBase.h>
#include "inputmethodservice/CAbstractInputMethodService.h"


/**
 * Base class for derived classes to implement their {@link InputMethodSession}
 * interface.  This takes care of basic maintenance of the session,
 * but most behavior must be implemented in a derived class.
 */
class AbstractInputMethodSessionImpl:
	public ElRefBase,
    public IAbstractInputMethodSessionImpl,
	public ILocalInputMethodSession
{
public:
	AbstractInputMethodSessionImpl(
		/* [in] */ CAbstractInputMethodService* host);

	~AbstractInputMethodSessionImpl();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Check whether this session has been enabled by the system.  If not
     * enabled, you should not execute any calls on to it.
     */
    CARAPI IsEnabled(
    	/* [out] */ Boolean* enabled);

    /**
     * Check whether this session has been revoked by the system.  Revoked
     * session is also always disabled, so there is generally no need to
     * explicitly check for this.
     */
    CARAPI IsRevoked(
    	/* [out] */ Boolean* revoked);

    /**
     * Change the enabled state of the session.  This only works if the
     * session has not been revoked.
     */
    CARAPI SetEnabled(
    	/* [in] */ Boolean enabled);

    /**
     * Revoke the session from the client.  This disabled the session, and
     * prevents it from ever being enabled again.
     */
    CARAPI RevokeSelf();

    /**
     * Take care of dispatching incoming key events to the appropriate
     * callbacks on the service, and tell the client when this is done.
     */
    CARAPI DispatchKeyEvent(
    	/* [in] */ Int32 seq,
    	/* [in] */ IKeyEvent* event,
    	/* [in] */ ILocalInputMethodSessionEventCallback* callback);

    /**
     * Take care of dispatching incoming trackball events to the appropriate
     * callbacks on the service, and tell the client when this is done.
     */
    CARAPI DispatchTrackballEvent(
    	/* [in] */ Int32 seq,
    	/* [in] */ IMotionEvent* event,
    	/* [in] */ ILocalInputMethodSessionEventCallback* callback);

    CARAPI FinishInput();

    CARAPI UpdateExtractedText(
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    CARAPI UpdateSelection(
        /* [in] */ Int32 oldSelStart,
        /* [in] */ Int32 oldSelEnd,
        /* [in] */ Int32 newSelStart,
        /* [in] */ Int32 newSelEnd,
        /* [in] */ Int32 candidatesStart,
        /* [in] */ Int32 candidatesEnd);

    CARAPI UpdateCursor(
        /* [in] */ IRect* newCursor);

    CARAPI DisplayCompletions(
        /* [in] */ ArrayOf<ICompletionInfo*>* completions);

    CARAPI AppPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI ToggleSoftInput(
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    CARAPI FinishSession();

private:
    Boolean mEnabled;
    Boolean mRevoked;

protected:
    CAbstractInputMethodService* mHost;
};




#endif  //_ABSTRACTINPUTMETHODSESSIONIMPL_H__
