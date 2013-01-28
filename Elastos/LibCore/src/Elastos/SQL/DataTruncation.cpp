
#include "DataTruncation.h"
#include "cmdef.h"

using namespace Elastos::System;

const Int64  DataTruncation::serialVersionUID = 6464298989504059473L;
const String DataTruncation::THE_REASON = "Data truncation";
const String DataTruncation::THE_SQLSTATE_READ = "01004";
const String DataTruncation::THE_SQLSTATE_WRITE = "22001";
const Int32  DataTruncation::THE_ERROR_CODE = 0;


DataTruncation::DataTruncation():mIndex(0),mParameter(false),mRead(false),mDataSize(0),mTransferSize(0) {
}

DataTruncation::~DataTruncation(){
}

ECode DataTruncation::Init(
    /**[in]**/Int32 index, 
    /**[in]**/Boolean parameter, 
    /**[in]**/Boolean read,
    /**[in]**/Int32 dataSize, 
    /**[in]**/Int32 transferSize)
{
    //super(THE_REASON, THE_SQLSTATE_READ, THE_ERROR_CODE);
    mIndex = index;
    mParameter = parameter;
    mRead = read;
    mDataSize = dataSize;
    mTransferSize = transferSize;
    return NOERROR;
}

ECode DataTruncation::GetDataSize(
    /**[out]**/Int32* size)
{
    *size = mDataSize;
    return NOERROR;
}


ECode DataTruncation::GetIndex(
    /**[out]**/Int32* index)
{
    *index = mIndex;
    return NOERROR;
}


ECode DataTruncation::GetParameter(
    /**[out]**/Boolean* result)
{
    *result = mParameter;
    return NOERROR;
}


ECode DataTruncation::GetRead(
    /**[out]**/Boolean* result)
{
    *result = mRead;
    return NOERROR;
}


ECode DataTruncation::GetTransferSize(
    /**[out]**/Int32* size)
{
    *size = mTransferSize;
    return NOERROR;
}