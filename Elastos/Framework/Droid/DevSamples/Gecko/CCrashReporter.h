
#ifndef __CCRASHREPORTER_H__
#define __CCRASHREPORTER_H__

#include "Activity.h"
#include "_CCrashReporter.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>

using namespace Elastos;

class CCrashReporter : public Activity
{
public:
    CCrashReporter();

    ~CCrashReporter();

    ECode MoveFile(
        /* [in] */ IFile* pInFile,
        /* [in] */ IFile* pOutFile);

    ECode DoFinish();

    CARAPI Finish();

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    ECode BackgroundSendReport();

    ECode OnCloseClick(
        /* [in] */ IView* pIView);

    ECode OnRestartClick(
        /* [in] */ IView* pIView);

    ECode ReadStringsFromFile(
        /* [in] */ const String& filePath,
        /* [in] */ HashMap<String, String>* stringMap);

    ECode ReadStringsFromReader(
        /* [in] */ IBufferedReader* pReader,
        /* [in] */ HashMap<String, String>* stringMap);

    ECode GenerateBoundary(
        /* [out] */ String* boundary);

    ECode SendPart(
        /* [in] */ IOutputStream* pOs,
        /* [in] */ const String& boundary,
        /* [in] */ const String& name,
        /* [in] */ const String& data);

    ECode SendFile(
        /* [in] */ IOutputStream* pOs,
        /* [in] */ const String& boundary,
        /* [in] */ const String& name,
        /* [in] */ IFile* pFile);

    ECode SendReport(
        /* [in] */ IFile* pMinidumpFile,
        /* [in] */ HashMap<String, String> extras,
        /* [in] */ IFile* pExtrasFile);

    ECode DoRestart();

    ECode Unescape(
        /* [in] */ const String& string,
        /* [out] */ String* out);

public:
    static const String kMiniDumpPathKey;
    static const String kPageURLKey;
    static const String kNotesKey;

    IHandler* mHandler;
    //IProgressDialog* mProgressDialog;
    IFile* mPendingMinidumpFile;
    IFile* mPendingExtrasFile;
    HashMap<String, String> mExtrasStringMap;

    AutoPtr<IView> mRestartButton;
    AutoPtr<IView> mCloseButton;
};

#endif //__CCRASHREPORTER_H__

