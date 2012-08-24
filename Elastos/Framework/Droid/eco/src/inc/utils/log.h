//==========================================================================
// Copyright (c) 2000-2010,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define FRMDBG_NORMAL           1
#define FRMDBG_WARNING          3
#define FRMDBG_ERROR            4
#define FRMDBG_NONE             100

#define FRMDBG_LEVEL            FRMDBG_NORMAL

#if defined(_DEBUG)

#define LOGD(fmt, ...) \
        if (FRMDBG_NORMAL >= FRMDBG_LEVEL) { \
            printf("==========FRAMEWORK DEBUG============\n"); \
            printf("<func:%s>[line:%d]: ", __FUNCTION__, __LINE__); \
            printf(fmt, ##__VA_ARGS__); }

#define LOGW(fmt, ...) \
        if (FRMDBG_WARNING >= FRMDBG_LEVEL) { \
            printf("==========FRAMEWORK WARNING============\n"); \
            printf("<func:%s>[line:%d]: ", __FUNCTION__, __LINE__); \
            printf(fmt, ##__VA_ARGS__); }

#define LOGE(fmt, ...) \
        if (FRMDBG_ERROR >= FRMDBG_LEVEL) { \
            printf("==========FRAMEWORK ERROR============\n"); \
            printf("<func:%s>[line:%d]: ", __FUNCTION__, __LINE__); \
            printf(fmt, ##__VA_ARGS__); }
#else
#define FRAMEWORK_DBGOUT(level, exp)
#define LOGD(fmt, ...)
#define LOGW(fmt, ...)
#define LOGE(fmt, ...) 
#endif // _DEBUG

#endif /* __LOG_H__ */

