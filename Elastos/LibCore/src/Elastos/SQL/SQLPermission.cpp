#include "SQLPermission.h"
#include "cmdef.h"

using namespace Elastos::System;

const Int64  SQLPermission::serialVersionUID = -1439323187199563495L;

SQLPermission::SQLPermission(){}

SQLPermission::~SQLPermission(){}

ECode SQLPermission::Init(
    /**[in]**/String name)
{
    //super(name);
    BasicPermission::BasicPermission(name);
}

ECode SQLPermission::Init(
    /**[in]**/String name, 
    /**[in]**/String actions)
{
    //super(name, null);
    BasicPermission::BasicPermission(name,NULL);
}