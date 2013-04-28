#include "Vm.h"
#include <sqlite3.h>

static const char xdigits[] = "0123456789ABCDEF";

Vm::Vm()
    :mHandle(0)
    ,mError_code(0)
{
}

/* free memory proc */

typedef void (freemem)(void *);

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

static void free_tab(void *mem)
{
    char **p = (char **) mem;
    int i, n;

    if (!p) {
        return;
    }
    p -= 1;
    mem = (void *) p;
    n = ((int *) p)[0];
    p += n * 2 + 2 + 1;
    for (i = 0; i < n; i++) {
        if (p[i]) {
            free(p[i]);
        }
    }
    free(mem);
}

static int callback(void *udata, int ncol, char **data, char **cols)
{
    return 0;
}

ECode Vm::Step(
    /** [in] **/ICallback* cb,
    /** [out] **/Boolean* result)
{
#if HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;

    if (v && v->vm && v->h) {
        Int32 ret;
        Int64 ncol = 0;
        freemem *freeproc = 0;
        const char **blob = 0;
        const char **data = 0, **cols = 0;
        ret = sqlite3_step((sqlite3_stmt *) v->vm);

        if (ret == SQLITE_DONE && v->hh.row1) {
            ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
            if (ncol > 0) {
                data = (const char **)calloc(ncol * 3 + 3 + 1, sizeof (char *));
                if (data) {
                    data[0] = (const char *) ncol;
                    ++data;
                    cols = data + ncol + 1;
                    blob = cols + ncol + 1;
                    freeproc = free_tab;
                } else {
                    ret = SQLITE_NOMEM;
                }
            }
            if (ret != SQLITE_NOMEM) {
                Int32 i;
                for (i = 0; i < ncol; i++) {
                    cols[i] =
                    sqlite3_column_name((sqlite3_stmt *) v->vm, i);
                }
            }
        } else if (ret == SQLITE_ROW) {
            ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);
            if (ncol > 0) {
                data = (const char **)calloc(ncol * 3 + 3 + 1, sizeof (char *));
                if (data) {
                    data[0] = (const char *) ncol;
                    ++data;
                    cols = data + ncol + 1;
                    blob = cols + ncol + 1;
                    freeproc = free_tab;
                } else {
                    ret = SQLITE_NOMEM;
                }
            }
            if (ret != SQLITE_NOMEM) {
                int i;

                for (i = 0; i < ncol; i++) {
                    cols[i] = sqlite3_column_name((sqlite3_stmt *) v->vm, i);
                    if (sqlite3_column_type((sqlite3_stmt *) v->vm, i)== SQLITE_BLOB) {
                        unsigned char *src = (unsigned char *)
                            sqlite3_column_blob((sqlite3_stmt *) v->vm, i);
                        int n =
                            sqlite3_column_bytes((sqlite3_stmt *) v->vm, i);

                        if (src) {
                            data[i] = (char *)malloc(n * 2 + 4);
                            if (data[i]) {
                                int k;
                                char *p = (char *) data[i];

                                blob[i] = data[i];
                                *p++ = 'X';
                                *p++ = '\'';
                                for (k = 0; k < n; k++) {
                                    *p++ = xdigits[src[k] >> 4];
                                    *p++ = xdigits[src[k] & 0x0F];
                                }
                                *p++ = '\'';
                                *p++ = '\0';
                            }
                        }
                    } else {
                        data[i] = (char *)
                            sqlite3_column_text((sqlite3_stmt *) v->vm, i);
                    }
                }
            }
        }

        if (ret == SQLITE_ROW) {
            v->hh.stmt = (sqlite3_stmt *) v->vm;

            callback((void *) &v->hh, ncol, (char **) data, (char **) cols);

            if (data && freeproc) {
                freeproc((void *) data);
            }

            *result = TRUE;
            return NOERROR;
        } else if (ret == SQLITE_DONE) {
    dofin:
            if (v->hh.row1 && cols) {
                v->hh.stmt = (sqlite3_stmt *) v->vm;


                callback((void *) &v->hh, ncol, (char **) 0, (char **) cols);

                if (data && freeproc) {
                    freeproc((void *) data);
                }
            }

            sqlite3_finalize((sqlite3_stmt *) v->vm);

            v->vm = 0;
            *result = FALSE;
            return NOERROR;
        }

        sqlite3_finalize((sqlite3_stmt *) v->vm);
        //setvmerr(env, obj, ret);
        v->vm = 0;
        //throwex(env, "error in step");
        *result = FALSE;
        return E_SQLITE_EXCEPTION;
    }
    //throwex(env, "vm already closed");
    *result = FALSE;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *result = FALSE;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}

ECode Vm::Compile(
    /** [out] **/Boolean* result)
{
#if HAVE_SQLITE_COMPILE
    hvm *v = (hvm *)mHandle;
    void *svm = 0;
    char *err = 0;
    const char *tail;
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

#if HAVE_SQLITE3_PREPARE_V2
        ret = sqlite3_prepare_v2((sqlite3 *) v->h->sqlite,
                     v->tail, -1, (sqlite3_stmt **) &svm, &tail);
#else
        ret = sqlite3_prepare((sqlite3 *) v->h->sqlite,
                      v->tail, -1, (sqlite3_stmt **) &svm, &tail);
#endif
        if (ret != SQLITE_OK) {
            if (svm) {
                sqlite3_finalize((sqlite3_stmt *) svm);
                svm = 0;
            }
            err = (char *) sqlite3_errmsg((sqlite3 *) v->h->sqlite);
        }


        if (ret != SQLITE_OK) {
            //setvmerr(env, obj, ret);
            v->tail = 0;
            //throwex(env, err ? err : "error in compile/prepare");
            *result = FALSE;
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
    //throwex(env, "vm already closed");
    *result = FALSE;
    return E_SQLITE_EXCEPTION;
#else
    //throwex(env, "unsupported");
    *result = FALSE;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
}


ECode Vm::Dovmfinal(
    /** [in] **/Int32 final)
{
    hvm *v = (hvm *)mHandle;

    if (v) {
        if (v->h) {
            //handle *h = v->h;
            hvm *vv, **vvp;
#if HAVE_SQLITE_COMPILE
            vvp = &h->vms;
#else
            vvp = NULL;
            *vvp = NULL;
#endif
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
            v->vm = 0;
        }
        free(v);
        mHandle = 0;
        return NOERROR;
    }
    if (!final) {
        //throwex(env, "vm already closed");
        return E_SQLITE_EXCEPTION;
    }
    return NOERROR;
}


ECode Vm::Stop()
{
    return Dovmfinal(0);
}

ECode Vm::Finalize()
{
    return Dovmfinal(1);
}

ECode Vm::Internal_init()
{
    return NOERROR;
}