
#ifndef __CPENDINGINTENT_H__
#define __CPENDINGINTENT_H__

#include "_CPendingIntent.h"

CarClass(CPendingIntent)
{
public:
    /**
     * Flag for use with {@link #getActivity}, {@link #getBroadcast}, and
     * {@link #getService}: this
     * PendingIntent can only be used once.  If set, after
     * {@link #send()} is called on it, it will be automatically
     * canceled for you and any future attempt to send through it will fail.
     */
    static const Int32 FLAG_ONE_SHOT = 1 << 30;

    /**
     * Flag for use with {@link #getActivity}, {@link #getBroadcast}, and
     * {@link #getService}: if the described PendingIntent does not already
     * exist, then simply return null instead of creating it.
     */
    static const Int32 FLAG_NO_CREATE = 1 << 29;

    /**
     * Flag for use with {@link #getActivity}, {@link #getBroadcast}, and
     * {@link #getService}: if the described PendingIntent already exists,
     * the current one is canceled before generating a new one.  You can use
     * this to retrieve a new PendingIntent when you are only changing the
     * extra data in the Intent; by canceling the previous pending intent,
     * this ensures that only entities given the new data will be able to
     * launch it.  If this assurance is not an issue, consider
     * {@link #FLAG_UPDATE_CURRENT}.
     */
    static const Int32 FLAG_CANCEL_CURRENT = 1 << 28;

    /**
     * Flag for use with {@link #getActivity}, {@link #getBroadcast}, and
     * {@link #getService}: if the described PendingIntent already exists,
     * then keep it but its replace its extra data with what is in this new
     * Intent.  This can be used if you are creating intents where only the
     * extras change, and don't care that any entities that received your
     * previous PendingIntent will be able to launch it with your new
     * extras even if they are not explicitly given to it.
     */
    static const Int32 FLAG_UPDATE_CURRENT = 1 << 27;

public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CPENDINGINTENT_H__
