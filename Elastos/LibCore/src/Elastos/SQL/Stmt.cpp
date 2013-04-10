#include "Stmt.h"
#include <sqlite3.h>
#include <stdio.h>

Stmt::Stmt()
    :mHandle(0)
    ,mError_code(0)
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

typedef struct hvm {
    struct hvm *next;       /* next vm handle */
    void *vm;           /* SQLite 2/3 VM/statement */
    char *tail;         /* tail SQL string */
    int tail_len;       /* only for SQLite3/prepare */
    handle *h;          /* SQLite database handle */
    handle hh;          /* fake SQLite database handle */
} hvm;

ECode Stmt::Prepare(
    /** [out] **/Boolean* result)
{
#if HAVE_SQLITE3
    hvm *v = (hvm *)mHandle;
    void *svm = 0;
    char *tail;
    Int32 ret;

    if (v && v->vm) {
        sqlite3_finalize((sqlite3_stmt *) v->vm);
        v->vm = 0;
    }
    if (v && v->h && v->h->sqlite) {
        if (!v->tail) {
            *result = FALSE;
            return NOERROR;
        }
#if HAVE_SQLITE3_PREPARE16_V2
        ret = sqlite3_prepare16_v2((sqlite3 *) v->h->sqlite,
                       v->tail, -1, (sqlite3_stmt **) &svm,
                       (const void **) &tail);
#else
        ret = sqlite3_prepare16((sqlite3 *) v->h->sqlite,
                    v->tail, -1, (sqlite3_stmt **) &svm,
                    (const void **) &tail);
#endif
        if (ret != SQLITE_OK) {
            if (svm) {
                sqlite3_finalize((sqlite3_stmt *) svm);
                svm = 0;
            }
        }
        if (ret != SQLITE_OK) {
            //const char *err = sqlite3_errmsg(v->h->sqlite);
            //setstmterr(env, obj, ret);
            v->tail = 0;
            //throwex(env, err ? err : "error in compile/prepare");
            *result = FALSE
            return E_SQLITE_EXCEPTION;
        }
        if (!svm) {
            v->tail = 0;
            *result = FALSE;
            return NOERROR;
        }
        v->vm = svm;
        v->tail = (char *) tail;
        v->hh.row1 = 1;
        *result = TRUE;
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *result = FALSE;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *result = FALSE;
    return E_SQLITE_EXCEPTION;
#endif
}
 
ECode Stmt::Step(
    /** [out] **/Boolean* result)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ret;

        ret = sqlite3_step((sqlite3_stmt *) v->vm);
        if (ret == SQLITE_ROW) {
            *result = TRUE;
            return NOERROR;
        }
        *result = FALSE;
        if (ret != SQLITE_DONE) {
            //const char *err = sqlite3_errmsg(v->h->sqlite);
            //setstmterr(env, obj, ret);
            //throwex(env, err ? err : "error in step");
            return E_SQLITE_EXCEPTION;
        }
        return NOERROR;
    }
    //throwex(env, "stmt already closed");

#else
    //throwex(env, "unsupported");
    *result = FALSE;
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::Close()
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ret;
        ret = sqlite3_finalize((sqlite3_stmt *) v->vm);
        v->vm = 0;
        if (ret != SQLITE_OK) {
            //const char *err = sqlite3_errmsg(v->h->sqlite);
            //setstmterr(env, obj, ret);
            //throwex(env, err ? err : "error in close");
            return E_SQLITE_EXCEPTION;
        }
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::Reset()
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        sqlite3_reset((sqlite3_stmt *) v->vm);
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::Clear_bindings()
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_CLEAR_BINDINGS
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        sqlite3_clear_bindings((sqlite3_stmt *) v->vm);
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::Bind(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 value)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        Int32 ret;
        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            return E_SQLITE_EXCEPTION;
        }
        ret = sqlite3_bind_int((sqlite3_stmt *) v->vm, pos, val);
        if (ret != SQLITE_OK) {
            //setstmterr(env, obj, ret);
            //throwex(env, "bind failed");
            return E_SQLITE_EXCEPTION;
        }
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::BindEx(
    /** [in] **/Int32 pos, 
    /** [in] **/Int64 value)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        Int32 ret;

        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            return E_SQLITE_EXCEPTION;
        }
        ret = sqlite3_bind_int64((sqlite3_stmt *) v->vm, pos, val);
        if (ret != SQLITE_OK) {
            //setstmterr(env, obj, ret);
            //throwex(env, "bind failed");
            return E_SQLITE_EXCEPTION;
        }
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::BindEx2(
    /** [in] **/Int32 pos, 
    /** [in] **/Double value)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        Int32 ret;

        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            return E_SQLITE_EXCEPTION;
        }
        ret = sqlite3_bind_double((sqlite3_stmt *) v->vm, pos, val);
        if (ret != SQLITE_OK) {
            //setstmterr(env, obj, ret);
            //throwex(env, "bind failed");
            return E_SQLITE_EXCEPTION;
        }
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::BindEx3(
    /** [in] **/Int32 pos, 
    /** [in] **/const ArrayOf<Byte>& val)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        Int32 ret;
        Int32 len;
        Byte *data = 0;

        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            return E_SQLITE_EXCEPTION;
        }
        if (val.GetPayload()!=NULL) {
            len = val.GetLength();
            if (len > 0) {
                data = (Byte *)sqlite3_malloc(len);
                if (!data) {
                    //throwoom(env, "unable to get blob parameter");
                    return E_SQLITE_EXCEPTION;
                }
                data = val.GetPayload();
                ret = sqlite3_bind_blob((sqlite3_stmt *) v->vm,
                            pos, data, len, sqlite3_free);
            } else {
                ret = sqlite3_bind_blob((sqlite3_stmt *) v->vm,
                        pos, "", 0, SQLITE_STATIC);
            }
        } else {
            ret = sqlite3_bind_null((sqlite3_stmt *) v->vm, pos);
        }
        if (ret != SQLITE_OK) {
            if (data) {
                sqlite3_free(data);
            }
            //setstmterr(env, obj, ret);
            //throwex(env, "bind failed");
            return E_SQLITE_EXCEPTION;
        }
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQLITE_EXCEPTION;
#endif
}

ECode Stmt::BindEx4(
    /** [in] **/Int32 pos, 
    /** [in] **/const String& val)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        Int32 ret;
        Int32 len, count;
        char *data = 0;

        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            return E_SQLITE_EXCEPTION;
        }
        if (!val.IsNull()) {
            count = val.GetLength();
            len = count * sizeof (char);
            if (len > 0) {
#ifndef JNI_VERSION_1_2
            const char *ch;
#endif
            data = (char *)sqlite3_malloc(len);
            if (!data) {
                //throwoom(env, "unable to get blob parameter");
                return E_SQLITE_EXCEPTION;
            }
#ifndef JNI_VERSION_1_2
            ch = (const char *)val;
            memcpy(data, ch, len);
#else
            (*env)->GetStringRegion(env, val, 0, count, (jchar *) data);
            data = (const char *)val;
#endif
            ret = sqlite3_bind_text16((sqlite3_stmt *) v->vm,
                          pos, data, len, sqlite3_free);
            } else {
                ret = sqlite3_bind_text16((sqlite3_stmt *) v->vm, pos, "", 0,
                          SQLITE_STATIC);
            }
        } else {
            ret = sqlite3_bind_null((sqlite3_stmt *) v->vm, pos);
        }
        if (ret != SQLITE_OK) {
            if (data) {
                sqlite3_free(data);
            }
            //setstmterr(env, obj, ret);
            //throwex(env, "bind failed");
            return E_SQLITE_EXCEPTION;
        }
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    // throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::BindEx5(
    /** [in] **/Int32 pos)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        Int32 ret;

        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            return E_SQLITE_EXCEPTION;
        }
        ret = sqlite3_bind_null((sqlite3_stmt *) v->vm, pos);
        if (ret != SQLITE_OK) {
            //setstmterr(env, obj, ret);
            //throwex(env, "bind failed");
            return E_SQLITE_EXCEPTION;
        }
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Bind_zeroblob(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 length)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_BIND_ZEROBLOB
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        Int32 ret;

        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            return E_SQLITE_EXCEPTION;
        }
        ret = sqlite3_bind_zeroblob((sqlite3_stmt *) v->vm, pos, len);
        if (ret != SQLITE_OK) {
            //setstmterr(env, obj, ret);
            //throwex(env, "bind failed");
            return E_SQLITE_EXCEPTION;
        }
    } else {
        //throwex(env, "stmt already closed");
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Bind_parameter_count(
    /** [out] **/Int32* count)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        *count = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        return MOERROR;
    }
    //throwex(env, "stmt already closed");
    *count = 0;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *count = 0;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Bind_parameter_name(
    /** [in] **/Int32 pos,
    /** [out] **/String* str)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_BIND_PARAMETER_NAME
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
        const char *name;

        if (pos < 1 || pos > npar) {
            //throwex(env, "parameter position out of bounds");
            *str = "";
            return NOERROR;
        }
        name = sqlite3_bind_parameter_name((sqlite3_stmt *) v->vm, pos);
        if (name) {
            *str = String(name);
            return NOERROR;
        }
    } else {
        //throwex(env, "stmt already closed");
        *str = "";
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    *str = "";
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Bind_parameter_index(
    /** [in] **/const String& name,
    /** [out] **/Int32* index)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_BIND_PARAMETER_INDEX
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 pos = -1;
        const char *n;
        transstr namestr;
        //????????????????????????????????????
        //n = trans2iso(env, 1, 0, name, &namestr);
        //pos = sqlite3_bind_parameter_index((sqlite3_stmt *) v->vm, n);
        *index = pos;
        return NOERROR;
    } else {
        //throwex(env, "stmt already closed");
        *index = -1;
        return E_SQLITE_EXCEPTION;
    }
#else
    //throwex(env, "unsupported");
    *index = -1;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_int(
    /** [in] **/Int32 col,
    /** [out] **/Int32* result)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *result = 0;
            return E_SQLITE_EXCEPTION;
        }
        *result = sqlite3_column_int((sqlite3_stmt *) v->vm, col);
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *result = 0;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *result = 0;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_long(
    /** [in] **/Int32 col,
    /** [out] **/Int64* result)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *result = 0;
            return E_SQLITE_EXCEPTION;
        }
        *result = sqlite3_column_int64((sqlite3_stmt *) v->vm, col);
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *result = 0;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *result = 0;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_double(
    /** [in] **/Int32 col,
    /** [out] **/Double* result)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *result = 0;
            return E_SQLITE_EXCEPTION;
        }
        *result = sqlite3_column_double((sqlite3_stmt *) v->vm, col);
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *result = 0;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *result = 0;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_bytes(
    /** [in] **/Int32 col,
    /** [out] **/ArrayOf<Byte>** bt)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);
        Int32 nbytes;
        const Byte *data;
        //jbyteArray b = 0;
        ArrayOf<Byte>* b;

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            bt = NULL;
            return E_SQLITE_EXCEPTION;
        }
        data = sqlite3_column_blob((sqlite3_stmt *) v->vm, col);
        if (data) {
            nbytes = sqlite3_column_bytes((sqlite3_stmt *) v->vm, col);
        } else {
            bt = NULL;
            return NOERROR;
        }
        //b = (*env)->NewByteArray(env, nbytes);
        b = ArrayOf<Byte>::Alloc(nbytes);
        if (!b) {
            //throwoom(env, "unable to get blob column data");
            bt = NULL;
            return E_SQLITE_EXCEPTION;
        }
        //(*env)->SetByteArrayRegion(env, b, 0, nbytes, data);
        for(Int32 i=0;i<nbytes;i++){
            (*b)[i] = (*data)[i];
        }
        *bt = b;
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    bt = NULL;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    bt = NULL;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_string(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);
        Int32 nbytes;
        const char *data;
        String b = "";

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *str = "";
            return E_SQLITE_EXCEPTION;
        }
        data = sqlite3_column_text16((sqlite3_stmt *) v->vm, col);
        if (data) {
            nbytes = sqlite3_column_bytes16((sqlite3_stmt *) v->vm, col);
        } else {
            *str = "";
            return NOERROR;
        }
        nbytes /= sizeof (char);
        //b = (*env)->NewString(env, data, nbytes);
        b = String(data, nbytes);
        if (!b) {
            //throwoom(env, "unable to get string column data");
            *str = "";
            return E_SQLITE_EXCEPTION;
        }
        *str = b;
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *str = "";
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *str = "";
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_type(
    /** [in] **/Int32 col,
    /** [out] **/Int32* type)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *type = 0;
            return E_SQLITE_EXCEPTION;
        }
        *type = sqlite3_column_type((sqlite3_stmt *) v->vm, col);
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *type = 0;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *type = 0;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_count(
    /** [out] **/Int32* count)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        *count = sqlite3_column_count((sqlite3_stmt *) v->vm);
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *count = 0;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *count = 0;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column(
    /** [in] **/Int32 col,
    /** [out] **/IInterface** obj)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_table_name(
    /** [in] **/Int32 col,
    /** [out] **/String* mstr)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_COLUMN_TABLE_NAME16
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
        const char *str;

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *mstr = "";
            return E_SQLITE_EXCEPTION;
        }
        str = sqlite3_column_table_name16((sqlite3_stmt *) v->vm, col);
        if (str) {
            *mstr = String(str, strlen(str));
            return NOERROR;
        }
        *mstr = "";
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *mstr = "";
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *mstr = "";
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_database_name(
    /** [in] **/Int32 col,
    /** [out] **/String* mstr)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_COLUMN_DATABASE_NAME16
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
        const char *str;

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *mstr = "";
            return E_SQLITE_EXCEPTION;
        }
        str = sqlite3_column_database_name16((sqlite3_stmt *) v->vm, col);
        if (str) {
            *mstr = String(str, strlen(str));
            return NOERROR;
        }
        *mstr = "";
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *mstr = "";
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *mstr = "";
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_decltype(
    /** [in] **/Int32 col,
    /** [out] **/String* mstr)
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
        const char *str;

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *mstr = "";
            return E_SQLITE_EXCEPTION;
        }
        str = sqlite3_column_decltype16((sqlite3_stmt *) v->vm, col);
        if (str) {
            *mstr = String(str, strlen(str));
            return NOERROR;
        }
        *mstr = "";
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *mstr = "";
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *mstr = "";
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Column_origin_name(
    /** [in] **/Int32 col,
    /** [out] **/String* mstr)
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_COLUMN_ORIGIN_NAME16
    hhvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
        const char *str;

        if (col < 0 || col >= ncol) {
            //throwex(env, "column out of bounds");
            *mstr = "";
            return E_SQLITE_EXCEPTION;
        }
        str = sqlite3_column_origin_name16((sqlite3_stmt *) v->vm, col);
        if (str) {
            *mstr = String(str, strlen(str));
            return NOERROR;
        }
        *mstr = "";
        return NOERROR;
    }
    //throwex(env, "stmt already closed");
    *mstr = "";
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *mstr = "";
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Stmt::Finalize()
{
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hhvm *v = (hvm *)mHandle;

    if (v) {
        if (v->h) {
            handle *h = v->h;
            hvm *vv, **vvp;

            vvp = &h->vms;
            vv = *vvp;
            while (vv) {
            if (vv == v) {
                *vvp = vv->next;
                break;
            }
            vvp = &vv->next;
            vv = *vvp;
            }
        }
        if (v->vm) {
            sqlite3_finalize((sqlite3_stmt *) v->vm);
        }
        v->vm = 0;
        free(v);
        mHandle = 0;
    }
#endif
    return NOERROR;
}

ECode Stmt::Internal_init()
{
    return E_NOT_IMPLEMENTED;
}