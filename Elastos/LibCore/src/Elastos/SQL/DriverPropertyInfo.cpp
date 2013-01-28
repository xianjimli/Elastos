#include "DriverPropertyInfo.h"
#include "cmdef.h"

using namespace Elastos::System;

DriverPropertyInfo::DriverPropertyInfo():mChoices(NULL),mDescription(NULL),mRequired(false){
}

DriverPropertyInfo::~DriverPropertyInfo(){
}

ECode DriverPropertyInfo::Init(
    /**[in]**/String name, 
    /**[in]**/String value)
{
    mName = name;
    mValue = value;
    mChoices = NULL;
    mDescription = NULL;
    mRequired = false;
    return NOERROR;
}