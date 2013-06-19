
#include <stdio.h>
#include "CActivityOne.h"
#include "CBroadcastReceiverOne.h"

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnStart()
{
    // TODO: Add your code here
    printf("\n==Ready to send Broadcast message!==\n");
    String text=(String)"com.android.BroadcastDemo.TEST_RECEIVER";
    //ECode ec;
    IIntent* pIntent;
    //ec=CIntent::New(text,&pIntent);
    CIntent::New(text,&pIntent);
 
    //IComponentName* pComponentName;
    //CComponentName::New("BroadcastDemo",,&pComponentName);
    //pIntent->SetComponent(pComponentName);
    
    //pIntent->SetAction(text);

    //IIntentFilter* pIntentFilter;
    //CIntentFilter::New(text,&pIntentFilter);
    //IBroadcastReceiver* pBroadcastReceiver;
    //CBroadcastReceiverOne::New(&pBroadcastReceiver);
    //RegisterReceiver(pBroadcastReceiver,pIntentFilter);
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

