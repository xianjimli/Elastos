#ifndef _CJDBCDRIVER_H_
#define _CJDBCDRIVER_H_

#include "JDBCDriver.h"
#include "_CJDBCDriver.h"

CarClass(CJDBCDriver), public JDBCDriver
{
public:
    CARAPI constructor();

    CARAPI AcceptsURL(
        /* [in] */CString url,
        /* [out] */Boolean* result);

//    CARAPI Connect(
//        /* [in] */String url, 
//        /* [in] */Properties info,
//        /* [out] */IConnection** con);
        
    CARAPI GetMajorVersion(
        /* [out] */Int32* version);

    CARAPI GetMinorVersion(
        /* [out] */Int32* version);

//  CARAPI GetPropertyInfo(
//        /* [in] */String url, 
//        /* [in] */IProperties* info,
//        /* [out] */ArrayOf<IDriverPropertyInfo*>* info);

    CARAPI IsJdbcCompliant(
        /* [out] */Boolean* result);
};

#endif //_CJDBCDRIVER_H_