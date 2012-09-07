//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "RSchool.h"
#include <stdio.h>

ECode RSchool::OnObjectEntering(
    /* [in] */ PInterface pObject)
{
    printf("-> RSchool::OnObjectEntering()...\n");

    // Make sure that only a child can enter the school.
    //
    IChild* pChild;
    pChild = IChild::Probe(pObject);
    if (pChild) return NOERROR;
    else return E_NO_INTERFACE;
}

ECode RSchool::OnObjectEntered(
    /* [in] */ PInterface pObject)
{
    printf("-> RSchool::OnObjectEntered()...\n");

    // The aspect of the RSchool context, i.e., AStudent, has a "protected"
    // interface, i.e., IStudentAdmin.  The protected keyword, in this case,
    // means that the interface may only be queried by its context or
    // its peer component or aspect.  In addition, a protected interface
    // may only be queried inside one of the following functions:
    // OnObjectEntering(), OnObjectEntered(),
    // OnAspectAttaching(), OnAspectDetaching(),
    // OnObjectLeaving(), OnObjectLeft().
    //
    IStudentAdmin* pStudentAdmin;
    pStudentAdmin = IStudentAdmin::Probe(pObject);
    if (!pStudentAdmin) {
        return E_NO_INTERFACE;
    }

    static Int32 s_totalStudents = 0;
    s_totalStudents++;
    pStudentAdmin->SetID(s_totalStudents+100);  // student ID starts with 101

    return NOERROR;
}

ECode RSchool::OnObjectLeaving(
    /* [in] */ PInterface pObject)
{
    printf("-> RSchool::OnObjectLeaving()...\n");
    return NOERROR;
}

ECode RSchool::OnObjectLeft(
    /* [in] */ PInterface pObject)
{
    printf("-> RSchool::OnObjectLeft()...\n");
    return NOERROR;
}

ECode RSchool::Open()
{
    printf("The school opens.\n");
    return NOERROR;
}
