#include "Vm.h"

Vm::Vm()
    :mHandle(0)
    ,mError_code(0)
{
}

ECode Vm::Step(
    /** [in] **/ICallback* cb,
    /** [out] **/Boolean* result)
{/*
#if HAVE_SQLITE_COMPILE
    hvm *v = gethvm(env, obj);

    if (v && v->vm && v->h) {
    jthrowable exc;
    int ret, tmp;
    long ncol = 0;
#if HAVE_SQLITE3
    freemem *freeproc = 0;
    const char **blob = 0;
#endif
    const char **data = 0, **cols = 0;

    v->h->env = env;
#if HAVE_BOTH_SQLITE
    if (v->is3) {
        ret = sqlite3_step((sqlite3_stmt *) v->vm);
        if (ret == SQLITE_DONE && v->hh.row1) {
        ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
        if (ncol > 0) {
            data = calloc(ncol * 3 + 3 + 1, sizeof (char *));
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
            cols[i] =
                sqlite3_column_name((sqlite3_stmt *) v->vm, i);
            }
        }
        } else if (ret == SQLITE_ROW) {
        ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);
        if (ncol > 0) {
            data = calloc(ncol * 3 + 3 + 1, sizeof (char *));
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
            cols[i] =
                sqlite3_column_name((sqlite3_stmt *) v->vm, i);
            if (sqlite3_column_type((sqlite3_stmt *) v->vm, i)
                == SQLITE_BLOB) {
                unsigned char *src = (unsigned char *)
                sqlite3_column_blob((sqlite3_stmt *) v->vm, i);
                int n =
                sqlite3_column_bytes((sqlite3_stmt *) v->vm,
                             i);

                if (src) {
                data[i] = malloc(n * 2 + 4);
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
                data[i] = (const char *)
                sqlite3_column_text((sqlite3_stmt *) v->vm, i);
            }
            }
        }
        }
    } else {
        tmp = 0;
        ret = sqlite_step((sqlite_vm *) v->vm, &tmp, &data, &cols);
        ncol = tmp;
    }
#else
#if HAVE_SQLITE2
    tmp = 0;
    ret = sqlite_step((sqlite_vm *) v->vm, &tmp, &data, &cols);
    ncol = tmp;
#endif
#if HAVE_SQLITE3
    ret = sqlite3_step((sqlite3_stmt *) v->vm);
    if (ret == SQLITE_DONE && v->hh.row1) {
        ncol = sqlite3_column_count((sqlite3_stmt *) v->vm);
        if (ncol > 0) {
        data = calloc(ncol * 3 + 3 + 1, sizeof (char *));
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
            cols[i] =
            sqlite3_column_name((sqlite3_stmt *) v->vm, i);
        }
        }
    } else if (ret == SQLITE_ROW) {
        ncol = sqlite3_data_count((sqlite3_stmt *) v->vm);
        if (ncol > 0) {
        data = calloc(ncol * 3 + 3 + 1, sizeof (char *));
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
            if (sqlite3_column_type((sqlite3_stmt *) v->vm, i)
            == SQLITE_BLOB) {
            unsigned char *src = (unsigned char *)
                sqlite3_column_blob((sqlite3_stmt *) v->vm, i);
            int n =
                sqlite3_column_bytes((sqlite3_stmt *) v->vm, i);

            if (src) {
                data[i] = malloc(n * 2 + 4);
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
#endif
#endif
    if (ret == SQLITE_ROW) {
        v->hh.cb = cb;
        v->hh.env = env;
#if HAVE_BOTH_SQLITE
        if (v->is3) {
        v->hh.stmt = (sqlite3_stmt *) v->vm;
        }
#else
#if HAVE_SQLITE3
        v->hh.stmt = (sqlite3_stmt *) v->vm;
#endif
#endif
        callback((void *) &v->hh, ncol, (char **) data, (char **) cols);
#if HAVE_SQLITE3
        if (data && freeproc) {
        freeproc((void *) data);
        }
#endif
        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
        (*env)->DeleteLocalRef(env, exc);
        goto dofin;
        }
        return JNI_TRUE;
    } else if (ret == SQLITE_DONE) {
dofin:
        if (v->hh.row1 && cols) {
        v->hh.cb = cb;
        v->hh.env = env;
#if HAVE_BOTH_SQLITE
        if (v->is3) {
            v->hh.stmt = (sqlite3_stmt *) v->vm;
        }
#else
#if HAVE_SQLITE3
        v->hh.stmt = (sqlite3_stmt *) v->vm;
#endif
#endif
        callback((void *) &v->hh, ncol, (char **) 0, (char **) cols);
#if HAVE_SQLITE3
        if (data && freeproc) {
            freeproc((void *) data);
        }
#endif
        exc = (*env)->ExceptionOccurred(env);
        if (exc) {
            (*env)->DeleteLocalRef(env, exc);
        }
        }
#if HAVE_BOTH_SQLITE
        if (v->is3) {
        sqlite3_finalize((sqlite3_stmt *) v->vm);
        } else {
        sqlite_finalize((sqlite_vm *) v->vm, 0);
        }
#else
#if HAVE_SQLITE2
        sqlite_finalize((sqlite_vm *) v->vm, 0);
#endif
#if HAVE_SQLITE3
        sqlite3_finalize((sqlite3_stmt *) v->vm);
#endif
#endif
        v->vm = 0;
        return JNI_FALSE;
    }
#if HAVE_BOTH_SQLITE
    if (v->is3) {
        sqlite3_finalize((sqlite3_stmt *) v->vm);
    } else {
        sqlite_finalize((sqlite_vm *) v->vm, 0);
    }
#else
#if HAVE_SQLITE2
    sqlite_finalize((sqlite_vm *) v->vm, 0);
#endif
#if HAVE_SQLITE3
    sqlite3_finalize((sqlite3_stmt *) v->vm);
#endif
#endif
    setvmerr(env, obj, ret);
    v->vm = 0;
    throwex(env, "error in step");
    return JNI_FALSE;
    }
    throwex(env, "vm already closed");
#else
    throwex(env, "unsupported");
#endif
    return JNI_FALSE;*/
    return NOERROR;
}

ECode Vm::Compile(
    /** [out] **/Boolean* result)
{/*
#if HAVE_SQLITE_COMPILE
    hvm *v = gethvm(env, obj);
    void *svm = 0;
    char *err = 0;
#ifdef HAVE_SQLITE2
    char *errfr = 0;
#endif
    const char *tail;
    int ret;

    if (v && v->vm) {
#if HAVE_BOTH_SQLITE
    if (v->is3) {
        sqlite3_finalize((sqlite3_stmt *) v->vm);
    } else {
        sqlite_finalize((sqlite_vm *) v->vm, 0);
    }
#else
#if HAVE_SQLITE2
    sqlite_finalize((sqlite_vm *) v->vm, 0);
#endif
#if HAVE_SQLITE3
    sqlite3_finalize((sqlite3_stmt *) v->vm);
#endif
#endif
    v->vm = 0;
    }
    if (v && v->h && v->h->sqlite) {
    if (!v->tail) {
        return JNI_FALSE;
    }
    v->h->env = env;
#if HAVE_BOTH_SQLITE
    if (v->is3) {
#if HAVE_SQLITE3_PREPARE_V2
        ret = sqlite3_prepare_v2((sqlite3 *) v->h->sqlite, v->tail, -1,
                     (sqlite3_stmt **) &svm, &tail);
#else
        ret = sqlite3_prepare((sqlite3 *) v->h->sqlite, v->tail, -1,
                  (sqlite3_stmt **) &svm, &tail);
#endif
        if (ret != SQLITE_OK) {
        if (svm) {
            sqlite3_finalize((sqlite3_stmt *) svm);
            svm = 0;
        }
        err = (char *) sqlite3_errmsg((sqlite3 *) v->h->sqlite);
        }
    } else {
        ret = sqlite_compile((sqlite *) v->h->sqlite, v->tail,
                 &tail, (sqlite_vm **) &svm, &errfr);
        if (ret != SQLITE_OK) {
        err = errfr;
        if (svm) {
            sqlite_finalize((sqlite_vm *) svm, 0);
            svm = 0;
        }
        }
    }
#else
#if HAVE_SQLITE2
    ret = sqlite_compile((sqlite *) v->h->sqlite, v->tail,
                 &tail, (sqlite_vm **) &svm, &errfr);
    if (ret != SQLITE_OK) {
        err = errfr;
        if (svm) {
        sqlite_finalize((sqlite_vm *) svm, 0);
        svm = 0;
        }
    }
#endif
#if HAVE_SQLITE3
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
#endif
#endif
    if (ret != SQLITE_OK) {
        setvmerr(env, obj, ret);
        v->tail = 0;
        throwex(env, err ? err : "error in compile/prepare");
#if HAVE_SQLITE2
        if (errfr) {
        sqlite_freemem(errfr);
        }
#endif
        return JNI_FALSE;
    }
#if HAVE_SQLITE2
    if (errfr) {
        sqlite_freemem(errfr);
    }
#endif
    if (!svm) {
        v->tail = 0;
        return JNI_FALSE;
    }
    v->vm = svm;
    v->tail = (char *) tail;
    v->hh.row1 = 1;
    return JNI_TRUE;
    }
    throwex(env, "vm already closed");
#else
    throwex(env, "unsupported");
#endif
    return JNI_FALSE;*/
    return NOERROR;
}

ECode Vm::Stop()
{
  //  dovmfinal(env, obj, 0);

    return NOERROR;
}

ECode Vm::Finalize()
{
   // dovmfinal(env, obj, 1);

    return NOERROR;
}

ECode Vm::Internal_init()
{
    return NOERROR;
}

/*
static void dovmfinal(JNIEnv *env, jobject obj, int final)
{
    hvm *v = gethvm(env, obj);

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
#if HAVE_BOTH_SQLITE
        if (v->is3) {
        sqlite3_finalize((sqlite3_stmt *) v->vm);
        } else {
        sqlite_finalize((sqlite_vm *) v->vm, 0);
        }
#else
#if HAVE_SQLITE2
        sqlite_finalize((sqlite_vm *) v->vm, 0);
#endif
#if HAVE_SQLITE3
        sqlite3_finalize((sqlite3_stmt *) v->vm);
#endif
#endif
        v->vm = 0;
    }
    free(v);
    (*env)->SetLongField(env, obj, F_SQLite_Vm_handle, 0);
    return;
    }
    if (!final) {
    throwex(env, "vm already closed");
    }
}*/