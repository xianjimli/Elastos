
#ifndef _JDBCDRIVER_H_
#define _JDBCDRIVER_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class JDBCDriver : public IDriver
{
public:
    static Boolean mSharedCache;// = FALSE;

    static String mVfs;// = NULL;

private:
  //static java.lang.reflect.Constructor makeConn = null;

protected:
   // AutoPtr<IConnection> mConn;

public:
    JDBCDriver();
    
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

//    CARAPI GetPropertyInfo(
//        /* [in] */String url, 
//        /* [in] */IProperties* info,
//        /* [out] */ArrayOf<IDriverPropertyInfo*>* info);

    CARAPI IsJdbcCompliant(
        /* [out] */Boolean* result);
};

#endif// _JDBCDRIVER_H_