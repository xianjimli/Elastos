#ifndef __WEBSYNCMANAGER_H__
#define __WEBSYNCMANAGER_H__

#include "ext/frameworkext.h"

class WebSyncManager// : public Runnable 
{    

public:
	virtual CARAPI_(void) Run();

    /**
     * sync() forces sync manager to sync now
     */
    virtual CARAPI_(void) Sync();

    /**
     * resetSync() resets sync manager's timer
     */
    virtual CARAPI_(void) ResetSync();

    /**
     * startSync() requests sync manager to start sync
     */
    virtual CARAPI_(void) StartSync();

    /**
     * stopSync() requests sync manager to stop sync. remove any SYNC_MESSAGE in
     * the queue to break the sync loop
     */
    virtual CARAPI_(void) StopSync();

    virtual CARAPI_(void) SyncFromRamToFlash() = 0;

protected:
	WebSyncManager(
		/* [in] */ IContext* context, 
		/* [in] */ const String& name);

	virtual CARAPI_(void) onSyncInit();
	virtual CARAPI_(IInterface*) Clone();

protected:
	// handler of the sync thread
	IHandler* mHandler;
	// database for the persistent storage
	IWebViewDatabase* mDataBase;

	// log tag
	static const char* LOGTAG;// = "websync";

private:
	class SyncHandler// : public IHandler 
	{
	public:
    	//@Override
    	CARAPI_(void) HandleMessage(
    		/* [in] */ IMessage* msg);
	};

private:
	// message code for sync message
	static const Int32 SYNC_MESSAGE = 101;
	// time delay in millisec for a sync (now) message
	static const Int32 SYNC_NOW_INTERVAL = 100; // 100 millisec
	// time delay in millisec for a sync (later) message
	static const Int32 SYNC_LATER_INTERVAL = 5 * 60 * 1000; // 5 minutes

	// thread for syncing
//	Thread mSyncThread;
	// Name of thread
	CString mThreadName;
	// Ref count for calls to start/stop sync
	Int32 mStartSyncRefCount;
};

#endif //__WEBSYNCMANAGER_H__