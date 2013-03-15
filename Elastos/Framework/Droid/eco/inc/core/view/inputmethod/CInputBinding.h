
#ifndef  _CINPUTBINDING_H__
#define  _CINPUTBINDING_H__

#include "_CInputBinding.h"
#include <elastos/AutoPtr.h>

/**
 * Information given to an {@link InputMethod} about a client connecting
 * to it.
 */
CarClass(CInputBinding)
{
public:
    CARAPI constructor();

    /**
     * Constructor.
     *
     * @param conn The interface for communicating back with the application.
     * @param connToken A remoteable token for communicating across processes.
     * @param uid The user id of the client of this binding.
     * @param pid The process id of where the binding came from.
     */
    CARAPI constructor(
        /* [in] */ IInputConnection* conn,
        /* [in] */ IBinder* connToken,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 pid);

    /**
     * Constructor from an existing InputBinding taking a new local input
     * connection interface.
     *
     * @param conn The new connection interface.
     * @param binding Existing binding to copy.
     */
    CARAPI constructor(
        /* [in] */ IInputConnection* conn,
        /* [in] */ IInputBinding* binding);

    /**
     * Return the connection for interacting back with the application.
     */
    CARAPI GetConnection(
        /* [out] */ IInputConnection** inputConnection);

    /**
     * Return the token for the connection back to the application.  You can
     * not use this directly, it must be converted to a {@link InputConnection}
     * for you.
     */
    CARAPI GetConnectionToken(
        /* [out] */ IBinder** binder);

    /**
     * Return the user id of the client associated with this binding.
     */
    CARAPI GetUid(
        /* [out] */ Int32* uid);

    /**
     * Return the process id where this binding came from.
     */
    CARAPI GetPid(
        /* [out] */ Int32* pid);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    //static final String TAG = "InputBinding";

    /**
     * The connection back to the client.
     */
    AutoPtr<IInputConnection> mConnection;

    /**
     * A remotable token for the connection back to the client.
     */
    AutoPtr<IBinder> mConnectionToken;

    /**
     * The UID where this binding came from.
     */
    Int32 mUid;

    /**
     * The PID where this binding came from.
     */
    Int32 mPid;
};

#endif  //_CINPUTBINDING_H__
