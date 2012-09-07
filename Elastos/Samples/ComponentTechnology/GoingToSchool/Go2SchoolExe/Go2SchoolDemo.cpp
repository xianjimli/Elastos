//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <stdlib.h>
#include <stdio.h>
#include "Go2SchoolDemo.h"

using namespace Elastos;

int main (int argc, char *argv[])
{

    // There are three components used in this example.
    //
    IChild* pChild1 = NULL;
    IChild* pChild2 = NULL;
    ISchool* pSchool = NULL;

    // CFoo::New(...) allocates a component on the heap.
    //
    ECode ec = CChild::New(String("Sophie"), &pChild1);
    if (FAILED(ec)) {
        goto MainExit;
    }
    pChild1->Play();

    // IFoo::Probe() does not impose a reference count, so
    // the lifespan of the interim pointer, pStudent, has to be
    // shorter than that of the component pointer, pChild1.
    //
    IStudent* pStudent;
    pStudent = IStudent::Probe(pChild1);
    if (!pStudent) {
        String name;
        pChild1->GetName(&name);
        name += "'s not a student yet.";
        printf("%s\n", (const char*)name);
    }

    // A regime is a special kind of component, which may have its own
    // member variables and methods, e.g., RSchool::Open().
    //
    ec = RSchool::New(&pSchool);
    if (FAILED(ec)) {
        goto MainExit;
    }
    pSchool->Open();

    // A regime usually has one or more aspects affiliated with it.
    // In this case, the regime, RSchool, has an aspect, AStudent.
    // An aspect is also a special kind of component, which may never
    // be created directly by an application.  Instead, an aspect is
    // created implicitly either when a prime component enters a regime
    // or via an attach operation, i.e., AFoo::Attach(pBar), where pBar
    // refers to a prime component.  As soon as an aspect is created,
    // it is aggregated with the prime component to form an indistinguishable
    // composite component.
    //
    ec = CObject::EnterRegime(pChild1, pSchool);
    if (FAILED(ec)) {
        goto MainExit;
    }

    // After a child enters a school, she has acquired a student aspect,
    // so she suddenly possesses an IStudent interface.
    //
    pStudent = IStudent::Probe(pChild1);
    if (pStudent) {
        pStudent->Study();

        String name;
        pChild1->GetName(&name);
        name += "'s student ID is ";

        Int32 id;
        pStudent->GetID(&id);
        name += String::FromInt32(id);
        printf("%s\n", (const char*)name);
    }

        // As soon as a prime component leaves a regime, the aspects,
    // imposed by the regime, are stripped off implicitly.
    //
    ec = CObject::LeaveRegime(pChild1, pSchool);
    if (FAILED(ec)) {
        goto MainExit;
    }

    // When a child leaves school, she does not have
    // IStudent interface any more.
    //
    pStudent = IStudent::Probe(pChild1);
    if (!pStudent) {
        String name;
        pChild1->GetName(&name);
        name += "'s not a student anymore.";
        printf("%s\n", (const char*)name);
    }

    // A prime component may be created in a regime directly,
    // via the CFoo::NewInRegime() operation.  In fact, a regime
    // is a component, which has some kind of "container" capability.
    // For example, a regime may be a remote process or a special
    // security domain where all member components are being watched
    // by the "big brother".  In other words, CFoo::NewInRegime() is not
    // only useful for aspect oriented programming (AOP), it is also
    // a mechanism for implementing distributed systems.
    //
    ec = CChild::NewInRegime(pSchool, String("Laura"), &pChild2);
    if (FAILED(ec)) {
        goto MainExit;
    }
    pChild2->Play();
//    pStudent = IStudent::Probe(pChild2);
//    if (pStudent) {
//        pStudent->Study();
//
//        StringBuf_<40> strBuf;
//        pChild2->GetName(&strBuf);
//        strBuf.Append("'s student ID is ");
//
//        Int32 id;
//        pStudent->GetID(&id);
//        strBuf.Append(id);
//        printf("%s\n", strBuf.GetPayload());
//    }

MainExit:
    if (pChild1) pChild1->Release();
    if (pChild2) pChild2->Release();
    if (pSchool) pSchool->Release();

    printf("=========>  End. <==========\n");
    return 0;
}
