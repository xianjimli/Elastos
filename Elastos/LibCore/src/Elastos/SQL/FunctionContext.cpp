#include "FunctionContext.h"
#include <sqlite3.h>

FunctionContext::FunctionContext()
    :mHandle(0)
{
}

/* internal handle for SQLite database */

typedef struct {
    void *sqlite;       /* SQLite handle */
    int ver;            /* version code */
//    jobject bh;         /* BusyHandler object */
//    jobject cb;         /* Callback object */
//    jobject ai;         /* Authorizer object */
//    jobject tr;         /* Trace object */
//    jobject ph;         /* ProgressHandler object */
//    JNIEnv *env;        /* Java environment for callbacks */
    int row1;           /* true while processing first row */
    // int haveutf;        /* true for SQLite UTF-8 support */
//    jstring enc;        /* encoding or 0 */
    struct hfunc *funcs;    /* SQLite user defined function handles */
#if HAVE_SQLITE_COMPILE
    struct hvm *vms;        /* Compiled SQLite VMs */
#endif
    sqlite3_stmt *stmt;     /* For callback() */
    struct hbl *blobs;        /* SQLite3 blob handles */
} handle;

/* internal handle for SQLite user defined function */

typedef struct hfunc {
    struct hfunc *next;     /* next function */
    // jobject fc;         /* FunctionContext object */
    // jobject fi;         /* Function object */
    // jobject db;         /* Database object */
    handle *h;          /* SQLite database handle */
    void *sf;           /* SQLite function handle */
    // JNIEnv *env;        /* Java environment for callbacks */
} hfunc;

ECode FunctionContext::Set_result(
    /* [in] */ const String& r)
{
    hfunc *f = (hfunc *)mHandle;

    if (f && f->sf) {
        if (!r.IsNull()) {
            Int32 len = r.GetLength() * sizeof (char);
            const char *str = (const char *)r;

            sqlite3_result_text16((sqlite3_context *) f->sf, str, len,
                      SQLITE_TRANSIENT);
        } else {
            sqlite3_result_null((sqlite3_context *) f->sf);
        }
    }
    return NOERROR;
}

ECode FunctionContext::Set_resultEx(
    /* [in] */ Int32 r)
{
    hfunc *f = (hfunc *)mHandle;

    if (f && f->sf) {
        sqlite3_result_int((sqlite3_context *) f->sf, (int) r);
    }
    return NOERROR;
}

ECode FunctionContext::Set_resultEx2(
    /* [in] */ Double r)
{
    hfunc *f = (hfunc *)mHandle;

    if (f && f->sf) {
        sqlite3_result_double((sqlite3_context *) f->sf, (double) r);
    }
    return NOERROR;
}

ECode FunctionContext::Set_error(
    /* [in] */ const String& r)
{
    hfunc *f = (hfunc *)mHandle;

    if (f && f->sf) {
        if (!r.IsNull()) {
            Int32 len = r.GetLength() * sizeof (char);
            const char *str = (const char *)r;
            sqlite3_result_error16((sqlite3_context *) f->sf, str, len);
        } else {
            sqlite3_result_error((sqlite3_context *) f->sf,
                     "null error text", -1);
        }
    }
    return NOERROR;
}

ECode FunctionContext::Set_resultEx3(
    /* [in] */ const ArrayOf<Byte>& r)
{
    hfunc *f = (hfunc *)mHandle;

    if (f && f->sf) {
        if (r.GetPayload()!=NULL) {
            Int32 len;
            Byte *data;

            len = r.GetLength();
            data = r.GetPayload();
            sqlite3_result_blob((sqlite3_context *) f->sf,
                    data, len, SQLITE_TRANSIENT);
        } else {
            sqlite3_result_null((sqlite3_context *) f->sf);
        }
    }
    return NOERROR;
}

ECode FunctionContext::Set_result_zeroblob(
    /* [in] */ Int32 n)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_RESULT_ZEROBLOB
    hfunc *f = (hfunc *)mHandle;

    if (f && f->sf) {
        sqlite3_result_zeroblob((sqlite3_context *) f->sf, n);
    }
#endif
    return NOERROR;
}

ECode FunctionContext::Count(
    /* [out] */ Int32* number)
{
    assert(number == NULL);
    hfunc *f = (hfunc *)mHandle;
    Int32 r = 0;

    if (f && f->sf) {
        r = (Int32) sqlite3_aggregate_count((sqlite3_context *) f->sf);
    }
    *number = r;
    return NOERROR;
}

ECode FunctionContext::Internal_init()
{
    return NOERROR;
}

