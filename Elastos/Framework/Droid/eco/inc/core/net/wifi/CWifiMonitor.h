
#ifndef __CWIFIMONITOR_H__
#define __CWIFIMONITOR_H__

#include "_CWifiMonitor.h"

/**
 * Listens for events from the wpa_supplicant server, and passes them on
 * to the {@link WifiStateTracker} for handling. Runs in its own thread.
 *
 * @hide
 */
CarClass(CWifiMonitor)
{
protected:
    class MonitorThread
    {
    public:
        MonitorThread(
            /* [in] */ CWifiMonitor* owner);

        ~MonitorThread();

        CARAPI Start();

        CARAPI Run();

    protected:
        /**
         * Handle all supplicant events except STATE-CHANGE
         * @param event the event type
         * @param remainder the rest of the string following the
         * event name and &quot;&#8195;&#8212;&#8195;&quot;
         */
        CARAPI_(void) HandleEvent(
            /* [in] */ Int32 event,
            /* [in] */ const String& remainder);

    private:
        CARAPI_(Boolean) ConnectToSupplicant();

        CARAPI_(void) HandlePasswordKeyMayBeIncorrect();

        CARAPI_(void) HandleDriverEvent(
            /* [in] */ const String& state);

        /**
         * Handle the supplicant STATE-CHANGE event
         * @param dataString New supplicant state string in the format:
         * id=network-id state=new-state
         */
        CARAPI_(void) HandleSupplicantStateChange(
            /* [in] */ const String& dataString);

        static void* EntryRoutine(void *arg);

    private:
        pthread_t mThread;
        CWifiMonitor* mOwner;
    };

public:
    CWifiMonitor();

    CARAPI constructor(
        /* [in] */ IWifiStateTracker* tracker);

    CARAPI StartMonitoring();

    CARAPI GetNetworkStateTracker(
        /* [out] */ INetworkStateTracker** tracker);

private:
    CARAPI_(void) HandleNetworkStateChange(
        /* [in] */ NetworkInfo.DetailedState newState,
        /* [in] */ const String& data);

    /**
     * Sleep for a period of time.
     * @param secs the number of seconds to sleep
     */
    static CARAPI_(void) Nap(
        /* [in] */ Int32 secs);

private:
    static const CString TAG;

    /** Events we receive from the supplicant daemon */

    static const Int32 CONNECTED    = 1;
    static const Int32 DISCONNECTED = 2;
    static const Int32 STATE_CHANGE = 3;
    static const Int32 SCAN_RESULTS = 4;
    static const Int32 LINK_SPEED   = 5;
    static const Int32 TERMINATING  = 6;
    static const Int32 DRIVER_STATE = 7;
    static const Int32 UNKNOWN      = 8;

    /** All events coming from the supplicant start with this prefix */
    static const CString eventPrefix;
    static const Int32 eventPrefixLen = eventPrefix.length();

    /** All WPA events coming from the supplicant start with this prefix */
    static const CString wpaEventPrefix;
    static const CString passwordKeyMayBeIncorrectEvent;

    /**
     * Names of events from wpa_supplicant (minus the prefix). In the
     * format descriptions, * &quot;<code>x</code>&quot;
     * designates a dynamic value that needs to be parsed out from the event
     * string
     */
    /**
     * <pre>
     * CTRL-EVENT-CONNECTED - Connection to xx:xx:xx:xx:xx:xx completed
     * </pre>
     * <code>xx:xx:xx:xx:xx:xx</code> is the BSSID of the associated access point
     */
    static const CString connectedEvent;
    /**
     * <pre>
     * CTRL-EVENT-DISCONNECTED - Disconnect event - remove keys
     * </pre>
     */
    static const CString disconnectedEvent;
    /**
     * <pre>
     * CTRL-EVENT-STATE-CHANGE x
     * </pre>
     * <code>x</code> is the numerical value of the new state.
     */
    static const CString stateChangeEvent;
    /**
     * <pre>
     * CTRL-EVENT-SCAN-RESULTS ready
     * </pre>
     */
    static const CString scanResultsEvent;

    /**
     * <pre>
     * CTRL-EVENT-LINK-SPEED x Mb/s
     * </pre>
     * {@code x} is the link speed in Mb/sec.
     */
    static const CString linkSpeedEvent;
    /**
     * <pre>
     * CTRL-EVENT-TERMINATING - signal x
     * </pre>
     * <code>x</code> is the signal that caused termination.
     */
    static const CString terminatingEvent;
    /**
     * <pre>
     * CTRL-EVENT-DRIVER-STATE state
     * </pre>
     * <code>state</code> is either STARTED or STOPPED
     */
    static const CString driverStateEvent;

    /**
     * Regex pattern for extracting an Ethernet-style MAC address from a string.
     * Matches a strings like the following:<pre>
     * CTRL-EVENT-CONNECTED - Connection to 00:1e:58:ec:d5:6d completed (reauth) [id=1 id_str=]</pre>
     */
    // private static Pattern mConnectedEventPattern =
    //     Pattern.compile("((?:[0-9a-f]{2}:){5}[0-9a-f]{2}) .* \\[id=([0-9]+) ");

    /**
     * This indicates the supplicant connection for the monitor is closed
     */
    static const CString monitorSocketClosed;

    /**
     * This indicates a read error on the monitor socket conenction
     */
    static const CString wpaRecvError;

    /**
     * Max errors before we close supplicant connection
     */
    static const Int32 MAX_RECV_ERRORS    = 10;


    AutoPtr<IWifiStateTracker> mWifiStateTracker;

    /**
     * Tracks consecutive receive errors
     */
    Int32 mRecvErrors = 0;
};

#endif __CWIFIMONITOR_H__
