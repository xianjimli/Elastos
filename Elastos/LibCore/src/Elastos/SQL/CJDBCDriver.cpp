
#include "cmdef.h"
#include "CJDBCDriver.h"

ECode CJDBCDriver::AcceptsURL(
    /* [in] */CString url,
    /* [out] */Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    
    return JDBCDriver::AcceptsURL(url, result);
}

//ECode CJDBCDriver::Connect(
//    /* [in] */String url, 
//    /* [in] */Properties info,
//    /* [out] */IConnection** con)
//{
//    return JDBCDriver::Connect(url, info, con);
//}

ECode CJDBCDriver::GetMajorVersion(
    /* [out] */Int32* version)
{
    VALIDATE_NOT_NULL(version);

    return JDBCDriver::GetMajorVersion(version);
}

ECode CJDBCDriver::GetMinorVersion(
    /* [out] */Int32* version)
{
    VALIDATE_NOT_NULL(version);

    return JDBCDriver::GetMinorVersion(version);
}

//ECode CJDBCDriver::GetPropertyInfo(
//    /* [in] */String url, 
//    /* [in] */IProperties* info,
//    /* [out] */ArrayOf<IDriverPropertyInfo*>* info)
//{
//}

ECode CJDBCDriver::IsJdbcCompliant(
    /* [out] */Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return JDBCDriver::IsJdbcCompliant(result);
}

ECode CJDBCDriver::constructor()
{
    return NOERROR;
}