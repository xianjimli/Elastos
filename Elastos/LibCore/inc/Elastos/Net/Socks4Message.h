#ifndef __SOCKS4MESSAGE_H__
#define __SOCKE4MESSAGE_H__

#include <elastos.h>

using namespace Elastos;

class Socks4Message
{
public:
    static const Int32 COMMAND_CONNECT = 1;
    static const Int32 COMMAND_BIND = 2;
    static const Int32 RETURN_SUCCESS = 90;
    static const Int32 RETURN_FAILURE = 91;
    static const Int32 RETURN_CANNOT_CONNECT_TO_IDENTD = 92;
    static const Int32 RETURN_DIFFERENT_USER_IDS = 93;
    static const Int32 REPLY_LENGTH = 8;
    static const Int32 INDEX_VERSION = 0;

private:
    static const Int32 SOCKS_VERSION = 4;
    static const Int32 INDEX_COMMAND = 1;
    static const Int32 INDEX_PORT = 2;
    static const Int32 INDEX_IP = 4;
    static const Int32 INDEX_USER_ID = 8;
    static const Int32 BUFFER_LENGTH = 256;
    static const Int32 MAX_USER_ID_LENGTH = BUFFER_LENGTH - INDEX_USER_ID;

protected:
    ArrayOf<Byte>  *mBuffer;

public:
    Socks4Message();

    Int32 GetCommandOrResult();

    void SetCommandOrResult(
        /* [in] */ Int32 command);

    Int32 GetPort();

    void SetPort(
        /* [in] */ Int32 port);

    Int32 GetIP();

    void SetIP(
        /* [in] */ const ArrayOf<Byte>& ip);

    String GetUserId();

    void SetUserId(
        /* [in] */ const String& id);

    String ToString();

    Int32 GetLength();

    String GetErrorString(
        /* [in] */ Int32 error);

    ArrayOf<Byte>* GetBytes();

private:
    Int32 GetInt16(
        /* [in] */ Int32 offset);

    Int32 GetInt32(
        /* [in] */ Int32 offset);

    String GetString(
         /* [in] */ Int32 offset,
         /* [in] */ Int32 maxLength);

    Int32 GetVersionNumber();

    void SetInt16(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 value);

    void SetString(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 maxLength,
        /* [in] */ const String& theString);

    void SetVersionNumber(int number);
};

#endif //__SOCKS4MESSAGE_H__