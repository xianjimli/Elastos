#include "Stmt.h"

Stmt::Stmt()
{
}

ECode Stmt::Prepare(
    /** [out] **/Boolean* result)
{/*
#if HAVE_SQLITE3
    hvm *v = gethstmt(env, obj);
    void *svm = 0;
    char *tail;
    int ret;

    if (v && v->vm) {
    sqlite3_finalize((sqlite3_stmt *) v->vm);
    v->vm = 0;
    }
    if (v && v->h && v->h->sqlite) {
    if (!v->tail) {
        return JNI_FALSE;
    }
    v->h->env = env;
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
        const char *err = sqlite3_errmsg(v->h->sqlite);

        setstmterr(env, obj, ret);
        v->tail = 0;
        throwex(env, err ? err : "error in compile/prepare");
        return JNI_FALSE;
    }
    if (!svm) {
        v->tail = 0;
        return JNI_FALSE;
    }
    v->vm = svm;
    v->tail = (char *) tail;
    v->hh.row1 = 1;
    return JNI_TRUE;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return JNI_FALSE;*/
    return E_NOT_IMPLEMENTED;
}
 
ECode Stmt::Step(
    /** [out] **/Boolean* result)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ret;

    ret = sqlite3_step((sqlite3_stmt *) v->vm);
    if (ret == SQLITE_ROW) {
        return JNI_TRUE;
    }
    if (ret != SQLITE_DONE) {
        const char *err = sqlite3_errmsg(v->h->sqlite);

        setstmterr(env, obj, ret);
        throwex(env, err ? err : "error in step");
    }
    return JNI_FALSE;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return JNI_FALSE;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Close()
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ret;

    ret = sqlite3_finalize((sqlite3_stmt *) v->vm);
    v->vm = 0;
    if (ret != SQLITE_OK) {
        const char *err = sqlite3_errmsg(v->h->sqlite);

        setstmterr(env, obj, ret);
        throwex(env, err ? err : "error in close");
    }
    return;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Reset()
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    sqlite3_reset((sqlite3_stmt *) v->vm);
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Clear_bindings()
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_CLEAR_BINDINGS
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    sqlite3_clear_bindings((sqlite3_stmt *) v->vm);
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 value)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    int ret;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return;
    }
    ret = sqlite3_bind_int((sqlite3_stmt *) v->vm, pos, val);
    if (ret != SQLITE_OK) {
        setstmterr(env, obj, ret);
        throwex(env, "bind failed");
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx(
    /** [in] **/Int32 pos, 
    /** [in] **/Int64 value)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    int ret;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return;
    }
    ret = sqlite3_bind_int64((sqlite3_stmt *) v->vm, pos, val);
    if (ret != SQLITE_OK) {
        setstmterr(env, obj, ret);
        throwex(env, "bind failed");
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx2(
    /** [in] **/Int32 pos, 
    /** [in] **/Double value)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    int ret;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return;
    }
    ret = sqlite3_bind_double((sqlite3_stmt *) v->vm, pos, val);
    if (ret != SQLITE_OK) {
        setstmterr(env, obj, ret);
        throwex(env, "bind failed");
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx3(
    /** [in] **/Int32 pos, 
    /** [in] **/const ArrayOf<Byte>& value)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    int ret;
    jint len;
    char *data = 0;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return;
    }
    if (val) {
        len = (*env)->GetArrayLength(env, val);
        if (len > 0) {
        data = sqlite3_malloc(len);
        if (!data) {
            throwoom(env, "unable to get blob parameter");
            return;
        }
        (*env)->GetByteArrayRegion(env, val, 0, len, (jbyte *) data);
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
        setstmterr(env, obj, ret);
        throwex(env, "bind failed");
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx4(
    /** [in] **/Int32 pos, 
    /** [in] **/const String& value)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    int ret;
    jsize len, count;
    char *data = 0;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return;
    }
    if (val) {
        count = (*env)->GetStringLength(env, val);
        len = count * sizeof (jchar);
        if (len > 0) {
#ifndef JNI_VERSION_1_2
        const jchar *ch;
#endif
        data = sqlite3_malloc(len);
        if (!data) {
            throwoom(env, "unable to get blob parameter");
            return;
        }
#ifndef JNI_VERSION_1_2
        ch = (*env)->GetStringChars(env, val, 0);
        memcpy(data, ch, len);
        (*env)->ReleaseStringChars(env, val, ch);
#else
        (*env)->GetStringRegion(env, val, 0, count, (jchar *) data);
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
        setstmterr(env, obj, ret);
        throwex(env, "bind failed");
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::BindEx5(
    /** [in] **/Int32 pos)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    int ret;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return;
    }
    ret = sqlite3_bind_null((sqlite3_stmt *) v->vm, pos);
    if (ret != SQLITE_OK) {
        setstmterr(env, obj, ret);
        throwex(env, "bind failed");
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_zeroblob(
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 length)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_BIND_ZEROBLOB
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    int ret;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return;
    }
    ret = sqlite3_bind_zeroblob((sqlite3_stmt *) v->vm, pos, len);
    if (ret != SQLITE_OK) {
        setstmterr(env, obj, ret);
        throwex(env, "bind failed");
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_parameter_count(
    /** [out] **/Int32* count)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    return sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_parameter_name(
    /** [in] **/Int32 pos,
    /** [out] **/String* str)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_BIND_PARAMETER_NAME
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int npar = sqlite3_bind_parameter_count((sqlite3_stmt *) v->vm);
    const char *name;

    if (pos < 1 || pos > npar) {
        throwex(env, "parameter position out of bounds");
        return 0;
    }
    name = sqlite3_bind_parameter_name((sqlite3_stmt *) v->vm, pos);
    if (name) {
        return (*env)->NewStringUTF(env, name);
    }
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Bind_parameter_index(
    /** [in] **/const String& name,
    /** [out] **/Int32* index)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_BIND_PARAMETER_INDEX
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int pos;
    const char *n;
    transstr namestr;
    jthrowable exc;

    n = trans2iso(env, 1, 0, name, &namestr);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return -1;
    }
    pos = sqlite3_bind_parameter_index((sqlite3_stmt *) v->vm, n);
    transfree(&namestr);
    return pos;
    } else {
    throwex(env, "stmt already closed");
    }
#else
    throwex(env, "unsupported");
#endif
    return -1;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_int(
    /** [in] **/Int32 col,
    /** [out] **/Int32* result)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    return sqlite3_column_int((sqlite3_stmt *) v->vm, col);
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_long(
    /** [in] **/Int32 col,
    /** [out] **/Int64* result)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    return sqlite3_column_int64((sqlite3_stmt *) v->vm, col);
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_double(
    /** [in] **/Int32 col,
    /** [out] **/Double* result)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    return sqlite3_column_double((sqlite3_stmt *) v->vm, col);
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_bytes(
    /** [in] **/Int32 col,
    /** [out] **/ArrayOf<Byte>** bt)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);
    int nbytes;
    const jbyte *data;
    jbyteArray b = 0;

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    data = sqlite3_column_blob((sqlite3_stmt *) v->vm, col);
    if (data) {
        nbytes = sqlite3_column_bytes((sqlite3_stmt *) v->vm, col);
    } else {
        return 0;
    }
    b = (*env)->NewByteArray(env, nbytes);
    if (!b) {
        throwoom(env, "unable to get blob column data");
        return 0;
    }
    (*env)->SetByteArrayRegion(env, b, 0, nbytes, data);
    return b;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_string(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);
    int nbytes;
    const jchar *data;
    jstring b = 0;

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    data = sqlite3_column_text16((sqlite3_stmt *) v->vm, col);
    if (data) {
        nbytes = sqlite3_column_bytes16((sqlite3_stmt *) v->vm, col);
    } else {
        return 0;
    }
    nbytes /= sizeof (jchar);
    b = (*env)->NewString(env, data, nbytes);
    if (!b) {
        throwoom(env, "unable to get string column data");
        return 0;
    }
    return b;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_type(
    /** [in] **/Int32 col,
    /** [out] **/Int32* type)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    return sqlite3_column_type((sqlite3_stmt *) v->vm, col);
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_count(
    /** [out] **/Int32* count)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    return sqlite3_column_count((sqlite3_stmt *) v->vm);
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column(
    /** [in] **/Int32 col,
    /** [out] **/IInterface** obj)
{
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_table_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_COLUMN_TABLE_NAME16
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
    const jchar *str;

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    str = sqlite3_column_table_name16((sqlite3_stmt *) v->vm, col);
    if (str) {
        return (*env)->NewString(env, str, jstrlen(str));
    }
    return 0;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_database_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_COLUMN_DATABASE_NAME16
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
    const jchar *str;

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    str = sqlite3_column_database_name16((sqlite3_stmt *) v->vm, col);
    if (str) {
        return (*env)->NewString(env, str, jstrlen(str));
    }
    return 0;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_decltype(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
    const jchar *str;

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    str = sqlite3_column_decltype16((sqlite3_stmt *) v->vm, col);
    if (str) {
        return (*env)->NewString(env, str, jstrlen(str));
    }
    return 0;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Column_origin_name(
    /** [in] **/Int32 col,
    /** [out] **/String* str)
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE3_COLUMN_ORIGIN_NAME16
    hvm *v = gethstmt(env, obj);

    if (v && v->vm && v->h) {
    int ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
    const jchar *str;

    if (col < 0 || col >= ncol) {
        throwex(env, "column out of bounds");
        return 0;
    }
    str = sqlite3_column_origin_name16((sqlite3_stmt *) v->vm, col);
    if (str) {
        return (*env)->NewString(env, str, jstrlen(str));
    }
    return 0;
    }
    throwex(env, "stmt already closed");
#else
    throwex(env, "unsupported");
#endif
    return 0;*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Finalize()
{/*
#if HAVE_SQLITE3 && HAVE_SQLITE_COMPILE
    dostmtfinal(env, obj);
#endif*/
    return E_NOT_IMPLEMENTED;
}

ECode Stmt::Internal_init()
{
    return E_NOT_IMPLEMENTED;
}