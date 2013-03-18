
#include "JDBCDriver.h"

Boolean JDBCDriver::mSharedCache = FALSE;

JDBCDriver::JDBCDriver()
{
}

ECode JDBCDriver::AcceptsURL(
    /* [in] */CString url,
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = url.StartWith("sqlite:/") ||
        url.StartWith("jdbc:sqlite:/");
    return NOERROR;
}

//ECode JDBCDriver::Connect(
//        /* [in] */String url, 
//        /* [in] */Properties info,
//        /* [out] */IConnection** con)
//{
//}

ECode JDBCDriver::GetMajorVersion(
    /* [out] */Int32* version)
{
    assert(version != NULL);
    *version =IJDBCDriver_MAJORVERSION;
    return NOERROR;
}

ECode JDBCDriver::GetMinorVersion(
    /* [out] */Int32* version)
{
    assert(version != NULL);
    *version = IConstants_drv_minor;
    return NOERROR;
}

//ECode JDBCDriver::GetPropertyInfo(
//        /* [in] */String url, 
//        /* [in] */IProperties* info,
//        /* [out] */ArrayOf<IDriverPropertyInfo*>* info)
//{
//}

ECode JDBCDriver::IsJdbcCompliant(
    /* [out] */Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}