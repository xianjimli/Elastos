#include "FunctionContext.h"

FunctionContext::FunctionContext()
    :mHandle(0)
{
}

ECode FunctionContext::Set_result(
    /* [in] */ const String& r)
{/*
    hfunc *f = getfunc(env, obj);

    if (f && f->sf) {
#if HAVE_BOTH_SQLITE
    if (!f->is3) {
        transstr retstr;
        jthrowable exc;

        trans2iso(env, f->h->haveutf, f->h->enc, ret, &retstr);
        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
        }
        sqlite_set_result_string((sqlite_func *) f->sf,
                     retstr.result, -1);
        transfree(&retstr);
    } else if (ret) {
        jsize len = (*env)->GetStringLength(env, ret) * sizeof (jchar);
        const jchar *str = (*env)->GetStringChars(env, ret, 0);

        sqlite3_result_text16((sqlite3_context *) f->sf, str, len,
                  SQLITE_TRANSIENT);
        (*env)->ReleaseStringChars(env, ret, str);
    } else {
        sqlite3_result_null((sqlite3_context *) f->sf);
    }
#else
#if HAVE_SQLITE2
    transstr retstr;
    jthrowable exc;

    trans2iso(env, f->h->haveutf, f->h->enc, ret, &retstr);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
    }
    sqlite_set_result_string((sqlite_func *) f->sf, retstr.result, -1);
    transfree(&retstr);
#endif
#if HAVE_SQLITE3
    if (ret) {
        jsize len = (*env)->GetStringLength(env, ret) * sizeof (jchar);
        const jchar *str = (*env)->GetStringChars(env, ret, 0);

        sqlite3_result_text16((sqlite3_context *) f->sf, str, len,
                  SQLITE_TRANSIENT);
        (*env)->ReleaseStringChars(env, ret, str);
    } else {
        sqlite3_result_null((sqlite3_context *) f->sf);
    }
#endif
#endif
    }*/
    return NOERROR;
}

ECode FunctionContext::Set_resultEx(
    /* [in] */ Int32 r)
{/*
    hfunc *f = getfunc(env, obj);

    if (f && f->sf) {
#if HAVE_BOTH_SQLITE
    if (f->is3) {
        sqlite3_result_int((sqlite3_context *) f->sf, (int) i);
    } else {
        sqlite_set_result_int((sqlite_func *) f->sf, (int) i);
    }
#else
#if HAVE_SQLITE2
    sqlite_set_result_int((sqlite_func *) f->sf, (int) i);
#endif
#if HAVE_SQLITE3
    sqlite3_result_int((sqlite3_context *) f->sf, (int) i);
#endif
#endif
    }*/
    return NOERROR;
}

ECode FunctionContext::Set_resultEx2(
    /* [in] */ Double r)
{/*
    hfunc *f = getfunc(env, obj);

    if (f && f->sf) {
#if HAVE_BOTH_SQLITE
    if (f->is3) {
        sqlite3_result_double((sqlite3_context *) f->sf, (double) d);
    } else {
        sqlite_set_result_double((sqlite_func *) f->sf, (double) d);
    }
#else
#if HAVE_SQLITE2
    sqlite_set_result_double((sqlite_func *) f->sf, (double) d);
#endif
#if HAVE_SQLITE3
    sqlite3_result_double((sqlite3_context *) f->sf, (double) d);
#endif
#endif
    }*/
    return NOERROR;
}

ECode FunctionContext::Set_error(
    /* [in] */ const String& r)
{/*
    hfunc *f = getfunc(env, obj);

    if (f && f->sf) {
#if HAVE_BOTH_SQLITE
    if (!f->is3) {
        transstr errstr;
        jthrowable exc;

        trans2iso(env, f->h->haveutf, f->h->enc, err, &errstr);
        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
        }
        sqlite_set_result_error((sqlite_func *) f->sf,
                    errstr.result, -1);
        transfree(&errstr);
    } else if (err) {
        jsize len = (*env)->GetStringLength(env, err) * sizeof (jchar);
        const jchar *str = (*env)->GetStringChars(env, err, 0);

        sqlite3_result_error16((sqlite3_context *) f->sf, str, len);
        (*env)->ReleaseStringChars(env, err, str);
    } else {
        sqlite3_result_error((sqlite3_context *) f->sf,
                 "null error text", -1);
    }
#else
#if HAVE_SQLITE2
    transstr errstr;
    jthrowable exc;

    trans2iso(env, f->h->haveutf, f->h->enc, err, &errstr);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        return;
    }
    sqlite_set_result_error((sqlite_func *) f->sf, errstr.result, -1);
    transfree(&errstr);
#endif
#if HAVE_SQLITE3
    if (err) {
        jsize len = (*env)->GetStringLength(env, err) * sizeof (jchar);
        const jchar *str = (*env)->GetStringChars(env, err, 0);

        sqlite3_result_error16((sqlite3_context *) f->sf, str, len);
        (*env)->ReleaseStringChars(env, err, str);
    } else {
        sqlite3_result_error((sqlite3_context *) f->sf,
                 "null error text", -1);
    }
#endif
#endif
    }*/
    return NOERROR;
}

ECode FunctionContext::Set_resultEx3(
    /* [in] */ const ArrayOf<Byte>& r)
{/*
    #if HAVE_SQLITE3
    hfunc *f = getfunc(env, obj);

    if (f && f->sf) {
#if HAVE_BOTH_SQLITE
    if (!f->is3) {
        // silently ignored 
        return;
    }
#endif
    if (b) {
        jsize len;
        jbyte *data;

        len = (*env)->GetArrayLength(env, b);
        data = (*env)->GetByteArrayElements(env, b, 0);
        sqlite3_result_blob((sqlite3_context *) f->sf,
                data, len, SQLITE_TRANSIENT);
        (*env)->ReleaseByteArrayElements(env, b, data, 0);
    } else {
        sqlite3_result_null((sqlite3_context *) f->sf);
    }
    }
#endif*/
    return NOERROR;
}

ECode FunctionContext::Set_result_zeroblob(
    /* [in] */ Int32 n)
{/*
    #if HAVE_SQLITE3 && HAVE_SQLITE3_RESULT_ZEROBLOB
    hfunc *f = getfunc(env, obj);

    if (f && f->sf) {
#if HAVE_BOTH_SQLITE
    if (!f->is3) {
        // silently ignored 
        return;
    }
#endif
    sqlite3_result_zeroblob((sqlite3_context *) f->sf, n);
    }
#endif*/
    return NOERROR;
}

ECode FunctionContext::Count(
    /* [out] */ Int32* number)
{/*
    hfunc *f = getfunc(env, obj);
    jint r = 0;

    if (f && f->sf) {
#if HAVE_SQLITE_BOTH
    if (f->is3) {
        r = (jint) sqlite3_aggregate_count((sqlite3_context *) f->sf);
    } else {
        r = (jint) sqlite_aggregate_count((sqlite_func *) f->sf);
    }
#else
#if HAVE_SQLITE2
    r = (jint) sqlite_aggregate_count((sqlite_func *) f->sf);
#endif
#if HAVE_SQLITE3
    r = (jint) sqlite3_aggregate_count((sqlite3_context *) f->sf);
#endif
#endif
    }
    return r;*/
    return NOERROR;
}

ECode FunctionContext::Internal_init()
{
    return NOERROR;
}

