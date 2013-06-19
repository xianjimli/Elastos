
#include <stdio.h>
#include <unistd.h>
#include "CActivityOne.h"
#include "CBroadcastReceiverOne.h"

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    printf("\n== CActivityOne::OnCreate ==\n");
    const String text=(String)"com.android.BroadcastDemo.TEST_RECEIVER";
    CBroadcastReceiverOne::New((IBroadcastReceiver**)&mReceiver);
    CIntentFilter::New(text, &mFilter);
    printf("\n==Ready to Register Broadcast Receiver!==\n");
    IIntent* pIntent;
    RegisterReceiver(mReceiver,mFilter,&pIntent);
    return NOERROR;
}

ECode CActivityOne::OnStart()
{
    printf("\n==Ready to send Broadcast message!==\n");
    String text=(String)"com.android.BroadcastDemo.TEST_RECEIVER";
    IIntent* pIntent;
    CIntent::New(text,&pIntent);

    SendBroadcast(pIntent);
    sleep(1);
    printf("\n==Ready to Unregister Receiver!==\n");
    UnregisterReceiver(mReceiver);  
    sleep(1);
    printf("\n==Ready to send Broadcast message 2 !==\n");  
    SendBroadcast(pIntent);
    return NOERROR;
}

ECode CActivityOne::OnRestart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

