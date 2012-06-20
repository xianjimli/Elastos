//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <elastos.h>
#include <frameworkext.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <elastos/AutoPtr.h>
#include <dlfcn.h>
#include <unistd.h>
#ifdef _MSVC
#include <sys/platform.h>
#include <KD/kd_KT_extentions.h>
#endif

#include "SystemServer.h"

_ELASTOS_NAMESPACE_USING

static Boolean s_bDebugFlag = FALSE;
Int32 option = 0;                       //For Framework App Testing

void Usage()
{
    printf("SuperExe.ecx [-debug] <Option> [args ...] \n");
    printf("Options: \n");
    printf("\t-debug\t\tFor debugging\n");
    printf("\t-app\tRun an application\n");
    printf("\t-test\t\tRun a test\n");
    printf("\t-?\t\t\tFor Help\n");
}

int LaunchApp(void *handle, char *appName);
int LaunchTest(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if (argc == 4){
        option = atoi(argv[3]);
    }

    if (argc < 2) {
        Usage();
        return 0;
    }

    if (!strcmp(argv[1], "-debug")) {
        s_bDebugFlag = TRUE;
        argv += 2;
        argc -= 3;
    }
    else {
        argv += 1;
        argc -= 2;
    }

    if (argv[0][0] == '-') {
        if (!strcmp(argv[0] + 1, "app")) {
            return LaunchApp(NULL, argv[1]);
        }
        else if (!strcmp(argv[0] + 1, "test")) {
            return LaunchTest(argc, argv + 1);
        }
        else {
            Usage();
            return 0;
        }
    }
    else {
        Usage();
        return 0;
    }
}

int LaunchApp(void *handle, char *appName)
{
    ECode ec = NOERROR;
    Int32 pid;
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IActivityManager> activityManagerService;
    AutoPtr<ICapsuleManager> capsuleManager;

    if (s_bDebugFlag == TRUE) {
#ifdef _MSVC
        DebugBreak();
#endif
    }

#ifdef _MSVC
    pid = kdCreateProcessKT("/native/ServiceManager.ecx");
    if (pid == -1) {
        printf("Create ServiceManager process fail!\n");
        return -1;
    }
#else
    pid = fork();
    if(pid == 0){
        char * argv[] = { (char*)"ServiceManager", NULL };
        if(execv("/data/data/com.elastos.runtime/elastos/ServiceManager", argv) < 0){
            printf("Execute ServiceManager.ecx fail!\n");
        }
        return 0;
    }
    else if(pid == -1) {
        printf("Create ServiceManager process fail!\n");
        return -1;
    }
#endif

    int count = 0;
    while ( FAILED(SystemServer::Init()) ) {
        usleep(100 * 1000);
        if (count++ < 8)
            printf("init system server failed, retry...\n");
    }

    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    serviceManager->GetService("capsule", (IInterface**)&capsuleManager);

    serviceManager->GetService("ActivityManagerService", (IInterface**)&activityManagerService);

    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetCapsule(appName);
    intent->SetAction("elastos.intent.action.MAIN");
    if (option != 0){
        intent->PutInt32Extra("ARG",option);
    }
    Int32 status;
    ec = activityManagerService->StartActivity(NULL, (IIntent*)intent, NULL,
            NULL, 0, NULL, NULL, -1, FALSE, FALSE, &status);

    if (FAILED(ec)) goto Exit;

    sem_t m_event;
    sem_init(&m_event, 0, 0);
    sem_wait(&m_event);

Exit:

    return ec;
}

#define TESTMAIN_FUNC "main"
typedef int (*TestMain_t)(int argc, char *argv[]);
typedef UInt32 (*Service)();

int LaunchTest(int argc, char *argv[])
{
    if (s_bDebugFlag == TRUE) {
#ifdef _MSVC
        DebugBreak();
#endif
    }

    void *elServicesHandle = dlopen("ElServices.dll", RTLD_NOW);
    Service startService = (Service)dlsym(elServicesHandle, "StartServiceCentral");
    if (NULL == startService) {
        printf("Cannot load symbol StartServiceCentral. ErrorCode: %s \n", dlerror());
        dlclose(elServicesHandle);
        return 0;
    }
    startService();

    char path[256] = {0};
    char appDir[256] = {0};
#ifdef _MSVC
    if (!kdGetcwdKT(appDir, 256)) {
#else
    if (!getcwd(appDir, 256)) {
#endif
        printf("Cann't Get appDir\n", __FILE__, __LINE__);
        return E_FAIL;
    }

    strcpy(path, appDir);
    strcat(path, "\\");
    strcat(path, argv[0]);

    void *testEcoHandle = dlopen(path, RTLD_NOW);
    if(NULL == testEcoHandle){
        printf("Can't dlopen %s. ErrorCode: %s\n", argv[0], dlerror());
        dlclose(elServicesHandle);
        return 0;
    }

    TestMain_t testMain = (TestMain_t)dlsym(testEcoHandle, TESTMAIN_FUNC);
    if (NULL == testMain) {
        printf("Cannot load symbol %s. ErrorCode: %s \n",
                TESTMAIN_FUNC, dlerror());
        dlclose(elServicesHandle);
        dlclose(testEcoHandle);
        return 0;
    }

    int ret = testMain(argc, argv);

    dlclose(testEcoHandle);

    Service stopService = (Service)dlsym(elServicesHandle, "StopServiceCentral");
    if (NULL == stopService) {
        printf("Cannot load symbol StopServiceCentral. ErrorCode: %s \n", dlerror());
        dlclose(elServicesHandle);
        return 0;
    }
    stopService();

    dlclose(elServicesHandle);

    return ret;
}
