#ifndef __DebugFlags_H__
#define __DebugFlags_H__


#include "ext/frameworkext.h"

//namespace Elastos {
    /*
     * Copyright (C) 2009 The Android Open Source Project
     *
     * Licensed under the Apache License, Version 2.0 (the "License");
     * you may not use this file except in compliance with the License.
     * You may obtain a copy of the License at
     *
     *      http://www.apache.org/licenses/LICENSE-2.0
     *
     * Unless required by applicable law or agreed to in writing, software
     * distributed under the License is distributed on an "AS IS" BASIS,
     * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
     * See the License for the specific language governing permissions and
     * limitations under the License.
     */

    //package android.webkit;

    /**
     * This class is a container for all of the debug flags used in the Java
     * components of webkit.  These flags must be final in order to ensure that
     * the compiler optimizes the code that uses them out of the final executable.
     *
     * The name of each flags maps directly to the name of the class in which that
     * flag is used.
     *
     */
    class DebugFlags 
    {
        public: static const Boolean sBROWSER_FRAME = FALSE;
        public: static const Boolean sCACHE_MANAGER = FALSE;
        public: static const Boolean sCALLBACK_PROXY = FALSE;
        public: static const Boolean sCOOKIE_MANAGER = FALSE;
        public: static const Boolean sCOOKIE_SYNC_MANAGER = FALSE;
        public: static const Boolean sDRAG_TRACKER = FALSE;
        public: static const CString sDRAG_TRACKER_LOGTAG;// = "skia";
        public: static const Boolean sFRAME_LOADER = FALSE;
        public: static const Boolean sJ_WEB_CORE_JAVA_BRIDGE = FALSE;// HIGHLY VERBOSE
        public: static const Boolean sLOAD_LISTENER = FALSE;
        public: static const Boolean sNETWORK = FALSE;
        public: static const Boolean sSSL_ERROR_HANDLER = FALSE;
        public: static const Boolean sSTREAM_LOADER = FALSE;
        public: static const Boolean sURL_UTIL = FALSE;
        public: static const Boolean sWEB_BACK_FORWARD_LIST = FALSE;
        public: static const Boolean sWEB_SETTINGS = FALSE;
        public: static const Boolean sWEB_SYNC_MANAGER = FALSE;
        public: static const Boolean sWEB_TEXT_VIEW = FALSE;
        public: static const Boolean sWEB_VIEW = FALSE;
        public: static const Boolean sWEB_VIEW_CORE = FALSE;
    };

//};

#endif // __DebugFlags_H__