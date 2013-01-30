
#ifndef __PROCESSMANAGER_H__
#define __PROCESSMANAGER_H__

#include "Elastos.Process_server.h"
#include "cmdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IProcess> >
{
    size_t operator()(AutoPtr<IProcess> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

class ProcessManager
{
public:
    class ProcessImpl : public ElRefBase, public IProcess
    {
    public:
        ProcessImpl(
            /* [in] */ Int32 id,
            /* [in] */ IFileDescriptor* in,
            /* [in] */ IFileDescriptor* out,
            /* [in] */ IFileDescriptor* err);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Destroy();

        CARAPI ExitValue(
            /* [out] */ Int32* value);

        CARAPI GetErrorStream(
            /* [out] */ IInputStream** es);

        CARAPI GetInputStream(
            /* [out] */ IInputStream** is);

        CARAPI GetOutputStream(
            /* [out] */ IOutputStream** os);

        CARAPI WaitFor(
            /* [out] */ Int32* value);

        CARAPI_(void) SetExitValue(
            /* [in] */ Int32 exitValue);

    public:
        /** Process ID. */
        Int32 mId;

        AutoPtr<IInputStream> mErrorStream;

        /** Reads output from process. */
        AutoPtr<IInputStream> mInputStream;

        /** Sends output to process. */
        AutoPtr<IOutputStream> mOutputStream;

        /** The process's exit value. */
        Int32 mExitValue;// = null;
        //Object exitValueMutex = new Object();
        Mutex mExitValueLock;
    };

private:
    class ProcessThread : public ElRefBase, public IRunnable
    {
    public:
        ProcessThread(
            /* [in] */ ProcessManager* pMg);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Run();

    private:
        ProcessManager* mPMg;
    };

    /** Automatically closes fd when collected. */
    class ProcessInputStream : public ElRefBase, public IFileInputStream
    {
    public:
        ProcessInputStream(
            /* [in]  */ IFileDescriptor* fd);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Available(
            /* [out] */ Int32* number);

        CARAPI Close();

        CARAPI Mark(
            /* [in] */ Int32 readLimit);

        CARAPI IsMarkSupported(
            /* [out] */ Boolean* supported);

        CARAPI Read(
            /* [out] */ Int32* value);

        CARAPI ReadBuffer(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [out] */ Int32* number);

        CARAPI ReadBufferEx(
            /* [in] */ Int32 offset,
            /* [in] */ Int32 length,
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [out] */ Int32* number);

        CARAPI Reset();

        CARAPI Skip(
            /* [in] */ Int64 count,
            /* [out] */ Int64* number);

        CARAPI GetChannel(
            /* [out] */ IFileChannel** channel);

        CARAPI GetFD(
            /* [out] */ IFileDescriptor** fd);

    private:
        AutoPtr<IFileDescriptor> mFd;
        AutoPtr<IFileInputStream> mFIn;
        Mutex m_lock;
    };

    /** Automatically closes fd when collected. */
    class ProcessOutputStream : public ElRefBase, public IFileOutputStream
    {
    public:
        ProcessOutputStream(
            /* [in]  */ IFileDescriptor* fd);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Close();

        CARAPI Flush();

        CARAPI Write(
            /* [in] */ Int32 oneByte);

        CARAPI WriteBuffer(
            /* [in] */ const ArrayOf<Byte>& buffer);

        CARAPI WriteBufferEx(
            /* [in] */ Int32 offset,
            /* [in] */ Int32 count,
            /* [in] */ const ArrayOf<Byte>& buffer);

        CARAPI GetChannel(
            /* [out] */ IFileChannel** channel);

        CARAPI GetFD(
            /* [out] */ IFileDescriptor** fd);

    private:
        AutoPtr<IFileDescriptor> mFd;
        AutoPtr<IFileOutputStream> mFOut;
        Mutex m_lock;
    };

public:
    /**
     * Cleans up after garbage collected processes. Requires the lock on the
     * map.
     */
    /* package */ CARAPI_(void) CleanUp();

    /**
     * Listens for signals from processes and calls back to
     * {@link #onExit(int,int)}.
     */
    /* package */ CARAPI_(void) WatchChildren();

    /**
     * Called by {@link #watchChildren()} when a child process exits.
     *
     * @param pid ID of process that exited
     * @param exitValue value the process returned upon exit
     */
    /* package */ CARAPI OnExit(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 exitValue);

    /**
     * Executes a native process. Fills in in, out, and err and returns the
     * new process ID upon success.
     */
    /* package */ static CARAPI Exec(
        /* [in] */ const ArrayOf<String>* command,
        /* [in] */ const ArrayOf<String>* environment,
        /* [in] */ const String& workingDirectory,
        /* [in] */ IFileDescriptor* in,
        /* [in] */ IFileDescriptor* out,
        /* [in] */ IFileDescriptor* err,
        /* [in] */ Boolean redirectErrorStream,
        /* [out] */ Int32* procId);

    /**
     * Executes a process and returns an object representing it.
     */
    /* package */ CARAPI Exec(
        /* [in] */ const ArrayOf<String>* taintedCommand,
        /* [in] */ const ArrayOf<String>* taintedEnvironment,
        /* [in] */ IFile* workingDirectory,
        /* [in] */ Boolean redirectErrorStream,
        /* [out] */ IProcess** proc);

    /** Gets the process manager. */
    static CARAPI_(const ProcessManager*) GetInstance();

private:
    ProcessManager();

    /**
     * Kills the process with the given ID.
     *
     * @parm pid ID of process to kill
     */
    static CARAPI Kill(
        /* [in] */ Int32 pid);

private:
    /**
     * constant communicated from native code indicating that a
     * child died, but it was unable to determine the status
     */
    static const Int32 WAIT_STATUS_UNKNOWN = -1;

    /**
     * constant communicated from native code indicating that there
     * are currently no children to wait for
     */
    static const Int32 WAIT_STATUS_NO_CHILDREN = -2;

    /**
     * constant communicated from native code indicating that a wait()
     * call returned -1 and set an undocumented (and hence unexpected) errno
     */
    static const Int32 WAIT_STATUS_STRANGE_ERRNO = -3;

public:
    static const ProcessManager* mInstance;

private:
    /**
     * Map from pid to Process. We keep weak references to the Process objects
     * and clean up the entries when no more external references are left. The
     * process objects themselves don't require much memory, but file
     * descriptors (associated with stdin/out/err in this case) can be
     * a scarce resource.
     */
    HashMap<Int32, AutoPtr<IProcess> > mProcesses;
    Mutex mProcessesLock;
};

#endif //__PROCESSMANAGER_H__
