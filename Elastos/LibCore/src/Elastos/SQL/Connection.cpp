
#include "Connection.h"

using namespace Elastos::System;

const Int32 Connection::IConnection_TRANSACTION_NONE = 0;
const Int32 Connection::IConnection_TRANSACTION_READ_COMMITTED = 2;
const Int32 Connection::IConnection_TRANSACTION_READ_UNCOMMITTED = 1;
const Int32 Connection::IConnection_TRANSACTION_REPEATABLE_READ = 4;
const Int32 Connection::IConnection_TRANSACTION_SERIALIZABLE = 8;