
#include "net/wifi/CWifiMonitor.h"
#include "net/wifi/WifiNative.h"


ECode WifiMonitor::MonitorThread::Start()
{
    if (pthread_create(&mThread, NULL, EntryRoutine, (void*)this)) {
        return E_THREAD_ABORTED;
    }
    return NOERROR;
}

ECode WifiMonitor::MonitorThread::Run()
{
    if (ConnectToSupplicant()) {
        // Send a message indicating that it is now possible to send commands
        // to the supplicant
        mOwner->mWifiStateTracker->NotifySupplicantConnection();
    }
    else {
        mOwner->mWifiStateTracker->NotifySupplicantLost();
        return NOERROR;
    }

    //noinspection InfiniteLoopStatement
    for (;;) {
        String eventStr = WifiNative::WaitForEvent();

        // Skip logging the common but mostly uninteresting scan-results event
        // if (Config.LOGD && eventStr.indexOf(scanResultsEvent) == -1) {
        //     Log.v(TAG, "Event [" + eventStr + "]");
        // }
        if (!eventStr.StartWith(eventPrefix)) {
            if (eventStr.StartWith(wpaEventPrefix) &&
                    0 < eventStr.IndexOf(passwordKeyMayBeIncorrectEvent)) {
                HandlePasswordKeyMayBeIncorrect();
            }
            continue;
        }

        String eventName = eventStr.Substring(eventPrefixLen);
        Int nameEnd = eventName.IndexOf(' ');
        if (nameEnd != -1) {
            eventName = eventName.Substring(0, nameEnd);
        }
        if (eventName.GetLength() == 0) {
            // if (Config.LOGD) Log.i(TAG, "Received wpa_supplicant event with empty event name");
            continue;
        }
        /*
         * Map event name into event enum
         */
        Int32 event;
        if (eventName.Equals(connectedEvent)) {
            event = CONNECTED;
        }
        else if (eventName.Equals(disconnectedEvent)) {
            event = DISCONNECTED;
        }
        else if (eventName.Equals(stateChangeEvent)) {
            event = STATE_CHANGE;
        }
        else if (eventName.Equals(scanResultsEvent)) {
            event = SCAN_RESULTS;
        }
        else if (eventName.Equals(linkSpeedEvent)) {
            event = LINK_SPEED;
        }
        else if (eventName.Equals(terminatingEvent)) {
            event = TERMINATING;
        }
        else if (eventName.Equals(driverStateEvent)) {
            event = DRIVER_STATE;
        }
        else {
            event = UNKNOWN;
        }

        String eventData = eventStr;
        if (event == DRIVER_STATE || event == LINK_SPEED) {
            eventData = eventData.split(" ")[1];
        }
        else if (event == STATE_CHANGE) {
            Int32 ind = eventStr.IndexOf(" ");
            if (ind != -1) {
                eventData = eventStr.Substring(ind + 1);
            }
        }
        else {
            Int32 ind = eventStr.IndexOf(" - ");
            if (ind != -1) {
                eventData = eventStr.Substring(ind + 3);
            }
        }

        if (event == STATE_CHANGE) {
            HandleSupplicantStateChange(eventData);
        }
        else if (event == DRIVER_STATE) {
            HandleDriverEvent(eventData);
        }
        else if (event == TERMINATING) {
            /**
             * If monitor socket is closed, we have already
             * stopped the supplicant, simply exit the monitor thread
             */
            if (eventData.StartWith(monitorSocketClosed)) {
                // if (Config.LOGD) {
                //     Log.d(TAG, "Monitor socket is closed, exiting thread");
                // }
                break;
            }

            /**
             * Close the supplicant connection if we see
             * too many recv errors
             */
            if (eventData.StartWith(wpaRecvError)) {
                if (++mRecvErrors > MAX_RECV_ERRORS) {
                    // if (Config.LOGD) {
                    //     Log.d(TAG, "too many recv errors, closing connection");
                    // }
                }
                else {
                    continue;
                }
            }

            // notify and exit
            mOwner->mWifiStateTracker->NotifySupplicantLost();
            break;
        }
        else {
            HandleEvent(event, eventData);
        }
        mRecvErrors = 0;
    }

    return NOERROR;
}

Boolean WifiMonitor::MonitorThread::ConnectToSupplicant()
{
    Int32 connectTries = 0;

    while (TRUE) {
        Boolean connected;
        mOwner->mWifiStateTracker->ConnectToSupplicant(&connected);
        if (connected) {
            return TRUE;
        }
        if (ConnectTries++ < 3) {
            mOwner->Nap(5);
        }
        else {
            break;
        }
    }
    return FALSE;
}

void WifiMonitor::MonitorThread::HandlePasswordKeyMayBeIncorrect()
{
    mOwner->mWifiStateTracker->NotifyPasswordKeyMayBeIncorrect();
}

void WifiMonitor::MonitorThread::HandleDriverEvent(
    /* [in] */ const String& state)
{
    if (state.IsNull()) {
        return;
    }
    if (state.Equals("STOPPED")) {
        mOwner->mWifiStateTracker->NotifyDriverStopped();
    }
    else if (state.Equals("STARTED")) {
        mOwner->mWifiStateTracker->NotifyDriverStarted();
    }
    else if (state.Equals("HANGED")) {
        mOwner->mWifiStateTracker->NotifyDriverHung();
    }
}

void WifiMonitor::MonitorThread::HandleEvent(
    /* [in] */ Int32 event,
    /* [in] */ const String& remainder)
{
    switch (event) {
        case DISCONNECTED:
            HandleNetworkStateChange(NetworkInfo.DetailedState.DISCONNECTED, remainder);
            break;

        case CONNECTED:
            HandleNetworkStateChange(NetworkInfo.DetailedState.CONNECTED, remainder);
            break;

        case SCAN_RESULTS:
            mOwner->mWifiStateTracker.NotifyScanResultsAvailable();
            break;

        case UNKNOWN:
            break;
    }
}

void WifiMonitor::MonitorThread::HandleSupplicantStateChange(
    /* [in] */ const String& dataString)
{
    String[] dataTokens = dataString.split(" ");

    String BSSID;
    int networkId = -1;
    int newState  = -1;
    for (String token : dataTokens) {
        String[] nameValue = token.split("=");
        if (nameValue.length != 2) {
            continue;
        }

        if (nameValue[0].equals("BSSID")) {
            BSSID = nameValue[1];
            continue;
        }

        int value;
        try {
            value = Integer.parseInt(nameValue[1]);
        } catch (NumberFormatException e) {
            Log.w(TAG, "STATE-CHANGE non-integer parameter: " + token);
            continue;
        }

        if (nameValue[0].equals("id")) {
            networkId = value;
        } else if (nameValue[0].equals("state")) {
            newState = value;
        }
    }

    if (newState == -1) return;

    SupplicantState newSupplicantState = SupplicantState.INVALID;
    for (SupplicantState state : SupplicantState.values()) {
        if (state.ordinal() == newState) {
            newSupplicantState = state;
            break;
        }
    }
    if (newSupplicantState == SupplicantState.INVALID) {
        Log.w(TAG, "Invalid supplicant state: " + newState);
    }
    mWifiStateTracker.notifyStateChange(networkId, BSSID, newSupplicantState);
}

void* WifiMonitor::MonitorThread::EntryRoutine(void* arg)
{
    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    ECode ec = ((WifiMonitor::MonitorThread*)arg)->Run();

    return (void*)ec;
}


const CString WifiMonitor::TAG = "WifiMonitor";

const Int32 WifiMonitor::CONNECTED;
const Int32 WifiMonitor::DISCONNECTED;
const Int32 WifiMonitor::STATE_CHANGE;
const Int32 WifiMonitor::SCAN_RESULTS;
const Int32 WifiMonitor::LINK_SPEED;
const Int32 WifiMonitor::TERMINATING;
const Int32 WifiMonitor::DRIVER_STATE;
const Int32 WifiMonitor::UNKNOWN;

const CString WifiMonitor::eventPrefix = "CTRL-EVENT-";
const Int32 WifiMonitor::eventPrefixLen = eventPrefix.GetLength();

/** All WPA events coming from the supplicant start with this prefix */
const CString WifiMonitor::wpaEventPrefix = "WPA:";
const CString WifiMonitor::passwordKeyMayBeIncorrectEvent = "pre-shared key may be incorrect";

const CString WifiMonitor::connectedEvent =    "CONNECTED";
const CString WifiMonitor::disconnectedEvent = "DISCONNECTED";
const CString WifiMonitor::stateChangeEvent =  "STATE-CHANGE";
const CString WifiMonitor::scanResultsEvent =  "SCAN-RESULTS";
const CString WifiMonitor::linkSpeedEvent = "LINK-SPEED";
const CString WifiMonitor::terminatingEvent =  "TERMINATING";
const CString WifiMonitor::driverStateEvent = "DRIVER-STATE";

const CString WifiMonitor::monitorSocketClosed = "connection closed";
const CString WifiMonitor::wpaRecvError = "recv error";

const Int32 WifiMonitor::MAX_RECV_ERRORS;

CWifiMonitor::CWifiMonitor()
{}

ECode CWifiMonitor::constructor(
    /* [in] */ IWifiStateTracker* tracker)
{
    mWifiStateTracker = tracker;
    return NOERROR;
}

ECode CWifiMonitor::StartMonitoring()
{
    new MonitorThread().start();
}

ECode CWifiMonitor::GetNetworkStateTracker(
    /* [out] */ INetworkStateTracker** tracker)
{
    return mWifiStateTracker;
}

void CWifiMonitor::HandleNetworkStateChange(
    /* [in] */ NetworkInfo.DetailedState newState,
    /* [in] */ const String& data)
{
    String BSSID = null;
    int networkId = -1;
    if (newState == NetworkInfo.DetailedState.CONNECTED) {
        Matcher match = mConnectedEventPattern.matcher(data);
        if (!match.find()) {
            if (Config.LOGD) Log.d(TAG, "Could not find BSSID in CONNECTED event string");
        } else {
            BSSID = match.group(1);
            try {
                networkId = Integer.parseInt(match.group(2));
            } catch (NumberFormatException e) {
                networkId = -1;
            }
        }
    }
    mWifiStateTracker.notifyStateChange(newState, BSSID, networkId);
}

void CWifiMonitor::Nap(
    /* [in] */ Int32 secs)
{
    // try {
    Thread.sleep(secs * 1000);
    // } catch (InterruptedException ignore) {
    // }
}
