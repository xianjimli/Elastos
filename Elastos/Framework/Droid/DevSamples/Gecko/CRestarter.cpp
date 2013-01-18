
#include "CRestarter.h"
#include <elastos/Thread.h>
#include <stdio.h>
#include "GeckoAppShell.h"

ECode CRestarter::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    printf("Restarter trying to restart fennec\n");
    //try {
        Int32 countdown = 40;
        Boolean result;
        ECode ec = GeckoAppShell::CheckForGeckoProcs(&result);
        while (SUCCEEDED(ec) && result &&  --countdown > 0) {
            // Wait for the old process to die before we continue
            //try {
            AutoPtr<IThread> pThread = Thread::GetCurrentThread();
            assert(pThread);
            //pIThread->Sleep(100);
            //} catch (InterruptedException ie) {}
            ec = GeckoAppShell::CheckForGeckoProcs(&result);
        }

        if (countdown <= 0) {
            // if the countdown expired, something is hung
            GeckoAppShell::KillAnyZombies();
            countdown = 10;
            // wait for the kill to take effect
            ec = GeckoAppShell::CheckForGeckoProcs(&result);
            while (SUCCEEDED(ec) && result &&  --countdown > 0) {
                //try {
                AutoPtr<IThread> pThread = Thread::GetCurrentThread();
                assert(pThread);
                //pThread->Sleep(100);
                //} catch (InterruptedException ie) {}
                ec = GeckoAppShell::CheckForGeckoProcs(&result);
            }
        }
    //} catch (Exception e) {
    //    Log.i("Restarter", e.toString());
    //}
    //try {
        String action("android.intent.action.MAIN");
        IIntent* pNewIntent;
        AutoPtr<IIntent> pIIntent = GetIntent();
        assert(pIIntent);

        IBundle* pIBundle = NULL;
        ec = pIIntent->GetExtras(&pIBundle);
        if (FAILED(ec)) return ec;

        ec = CIntent::New(&pNewIntent);
        if (FAILED(ec)) {
            if (pIBundle) pIBundle->Release();
            return ec;
        }

        pNewIntent->SetClassName(String("org.mozilla.fennec_unofficial"),
                            String("org.mozilla.fennec_unofficial.App"));
        if (pIBundle) {
            pNewIntent->PutExtras(pIBundle);
            pIBundle->Release();
        }
        pNewIntent->SetFlags(Intent_FLAG_ACTIVITY_NEW_TASK);
        //printf("GeckoAppJava %s\n", pNewIntent->ToString());
        StartActivity(pNewIntent);
        pNewIntent->Release();
    //} catch (Exception e) {
    //    Log.i("Restarter", e.toString());
    //}
    // Give the new process time to start before we die
    GeckoAppShell::WaitForAnotherGeckoProc();
    //System::Exit(0);
    return NOERROR;
}

