
#include "CCrashReporter.h"
#include <elastos/Math.h>
#include "GeckoRunnable.h"
#include <stdio.h>

using namespace Elastos::Core;

#define MAX_INT_VALUE   2147483647
const String CCrashReporter::kMiniDumpPathKey("upload_file_minidump");
const String CCrashReporter::kPageURLKey("URL");
const String CCrashReporter::kNotesKey("Notes");

class CrashReporterClikListener : public ElRefBase
                                , public IViewOnClickListener
{
public:
    CrashReporterClikListener(
        /* [in] */ CCrashReporter* pHost)
        : mHost(pHost)
    {
    }

    UInt32 AddRef()
    {
        return ElRefBase::AddRef();
    }

    UInt32 Release()
    {
        return ElRefBase::Release();
    }

    PInterface Probe(
        /* [in] */ REIID riid)
    {
        if (riid == EIID_IInterface) {
            return (PInterface)(IViewOnClickListener*)this;
        }
        else if (riid == EIID_IViewOnClickListener) {
            return (IViewOnClickListener*)this;
        }

        return NULL;
    }

    ECode GetInterfaceID(
        /* [in] */ IInterface* pObject,
        /* [in] */ InterfaceID* pIID)
    {
        return E_NOT_IMPLEMENTED;
    }

    ECode OnClick(
        /* [in] */ IView* pView)
    {
        if (!CObject::Compare(pView, mHost->mCloseButton)) {
            return mHost->OnCloseClick(pView);
        }
        else if (!CObject::Compare(pView, mHost->mRestartButton)) {
            return mHost->OnRestartClick(pView);
        }
        return NOERROR;
    }

protected:
    CCrashReporter* mHost;
};


CCrashReporter::CCrashReporter()
    : mHandler(NULL)
    //, mProgressDialog(NULL)
    , mPendingMinidumpFile(NULL)
    , mPendingExtrasFile(NULL)
    , mRestartButton(NULL)
    , mCloseButton(NULL)
{
}

CCrashReporter::~CCrashReporter()
{
    if (mPendingExtrasFile) {
        mPendingExtrasFile->Release();
    }
    if (mPendingMinidumpFile) {
        mPendingMinidumpFile->Release();
    }
    /*if (mProgressDialog) {
        mProgressDialog->Release();
    }*/
    if (mHandler) {
        mHandler->Release();
    }
}

ECode CCrashReporter::MoveFile(
    /* [in] */ IFile* pInFile,
    /* [in] */ IFile* pOutFile)
{
    Boolean succeeded;
    Int64 position = 0;
    ECode ec = pInFile->RenameTo(pOutFile, &succeeded);
    if (FAILED(ec)) return ec;
    if (succeeded) {
        return NOERROR;
    }
    //try {
    ec = pOutFile->CreateNewFile(&succeeded);
    if (FAILED(ec)) return ec;
    printf("GeckoCrashReporter couldn't rename minidump file\n");

    // so copy it instead
    IFileInputStream* pIns;
    IFileOutputStream* pOuts;
    IFileChannel *pInChannel = NULL, *pOutChannel = NULL;
    ec = CFileInputStream::New(pInFile, &pIns);
    if (FAILED(ec)) return ec;
    ec = pIns->GetChannel(&pInChannel);
    pIns->Release();
    if (FAILED(ec)) return ec;

    ec = CFileOutputStream::New(pOutFile, &pOuts);
    if (FAILED(ec)) goto exit;
    ec = pOuts->GetChannel(&pOutChannel);
    pOuts->Release();
    if (FAILED(ec)) goto exit;

    Int64 size, transferred;
    pInChannel->Size(&size);
    ec = pInChannel->TransferTo(position, 
                size, IWritableByteChannel::Probe(pOutChannel), &transferred);
    if (FAILED(ec)) goto exit;
    pInChannel->Close();
    pOutChannel->Close();

    if (transferred > 0) {
      pInFile->Delete(&succeeded);
    }
    //} catch (Exception e) {
    //  Log.e("GeckoCrashReporter",
    //        "exception while copying minidump file: ", e);
    //  return false;
    //}
exit:
    pInChannel->Release();
    if (pOutChannel) {
        pOutChannel->Release();
    }
    return ec;
}

class FinishRunnable : public GeckoRunnable
{
public:
    FinishRunnable(
        /* [in] */ CCrashReporter* pHost)
        : GeckoRunnable((Void*)pHost)
    {
    }

    ECode Run()
    {
        ((CCrashReporter*)mHost)->Finish();
        return NOERROR;
    }
};

ECode CCrashReporter::DoFinish()
{
    if (!mHandler) {
        FinishRunnable* pRunnable = new FinishRunnable(this);
        if (!pRunnable) return E_OUT_OF_MEMORY;

        //return mHandler->Post(IRunnable::Probe(pRunnable));
    }

    return NOERROR;
}

ECode CCrashReporter::Finish()
{
    //mProgressDialog->Dismiss();
    //return Activity::Finish();
    return E_NOT_IMPLEMENTED;
}

ECode CCrashReporter::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Activity::OnCreate(savedInstanceState);
    // mHandler is created here so runnables can be run on the main thread
    ECode ec;
    //ec = CHandler::New(&mHandler);
    //if (FAILED(ec)) return ec;

    SetContentView(0x7f030000);//R.layout.crash_reporter

    /*ec = CProgressDialog::New(this, &mProgressDialog);
    if (FAILED(ec)) return ec;
    String report;
    GetString(0x7f05000c, &report); //R.string.sending_crash_report
    mProgressDialog->SetMessage(report);*/

    mRestartButton = FindViewById(0x7f070003);//R.id.restart
    assert(mRestartButton);
    mCloseButton = FindViewById(0x7f070002);//R.id.close
    assert(mCloseButton);
    CrashReporterClikListener* pListener = new CrashReporterClikListener(this);
    if (!pListener) return E_OUT_OF_MEMORY;
    mRestartButton->SetOnClickListener(IViewOnClickListener::Probe(pListener));
    mCloseButton->SetOnClickListener(IViewOnClickListener::Probe(pListener));

    AutoPtr<IIntent> pIIntent = GetIntent();
    assert(pIIntent);
    String passedMinidumpPath;
    ec = pIIntent->GetStringExtra(String("minidumpPath"), &passedMinidumpPath);
    if (FAILED(ec)) return ec;

    IFile* pPassedMinidumpFile;
    ec = CFile::New(passedMinidumpPath, &pPassedMinidumpFile);
    if (FAILED(ec)) return ec;

    String name;
    String path;
    IFile* pPendingDir = NULL;
    ec = CFile::New(
        String("/data/data/org.mozilla.fennec_unofficial/mozilla/Crash Reports/pending"),
        &pPendingDir);
    if (FAILED(ec)) goto exit;

    Boolean succeeded;
    pPendingDir->Mkdirs(&succeeded);

    pPassedMinidumpFile->GetName(&name);
    ec = CFile::New(pPendingDir, name, &mPendingMinidumpFile);
    if (FAILED(ec)) goto exit;
    MoveFile(pPassedMinidumpFile, mPendingMinidumpFile);

    IFile* pExtrasFile;
    ec = CFile::New(passedMinidumpPath.Replace(".dmp", ".extra"), &pExtrasFile);
    if (FAILED(ec)) goto exit;
    pExtrasFile->GetName(&name);
    ec = CFile::New(pPendingDir, name, &mPendingExtrasFile);
    if (FAILED(ec)) {
        pExtrasFile->Release();
        goto exit;
    }
    MoveFile(pExtrasFile, mPendingExtrasFile);
    pExtrasFile->Release();

    mPendingExtrasFile->GetPath(&path);
    ec = ReadStringsFromFile(path, &mExtrasStringMap);

exit:
    pPassedMinidumpFile->Release();
    if (pPendingDir) {
        pPendingDir->Release();
    }
    return ec;
}

class SendReportRunnable : public GeckoRunnable
{
public:
    SendReportRunnable(
        /* [in] */ CCrashReporter* pHost)
        : GeckoRunnable((Void*)pHost)
    {
    }

    ECode Run()
    {
        CCrashReporter* pHost = (CCrashReporter*)mHost;
        pHost->SendReport(pHost->mPendingMinidumpFile,
             pHost->mExtrasStringMap, pHost->mPendingExtrasFile);
        return NOERROR;
    }
};

ECode CCrashReporter::BackgroundSendReport()
{
    AutoPtr<IView> pView = FindViewById(
                                0x7f070000); //R.id.send_report
    assert(pView);

    ICheckBox* pSendReportCheckbox = ICheckBox::Probe(pView);

    Boolean isChecked;
    //pSendReportCheckbox->IsChecked(&isChecked);
    if (!isChecked) {
        return DoFinish();
    }

    //mProgressDialog->Show();
    SendReportRunnable* pRunnable = new SendReportRunnable(this);
    if (!pRunnable) return E_OUT_OF_MEMORY;

    IThread* pIThread;
    ECode ec = CThread::New(IRunnable::Probe(pRunnable), &pIThread);
    if (FAILED(ec)) {
        delete pRunnable;
        return ec;
    }
    pIThread->Start();
    pIThread->Release();
    return NOERROR;
}

ECode CCrashReporter::OnCloseClick(
    /* [in] */ IView* pIView)
{
    return BackgroundSendReport();
}

ECode CCrashReporter::OnRestartClick(
    /* [in] */ IView* pIView)
{
    DoRestart();
    return BackgroundSendReport();
}

ECode CCrashReporter::ReadStringsFromFile(
    /* [in] */ const String& filePath,
    /* [in] */ HashMap<String, String>* stringMap)
{
    //try {
    IInputStreamReader* pIFileReader;
    ECode ec = CFileReader::New(filePath, &pIFileReader);
    if (FAILED(ec)) return ec;

    IBufferedReader* pReader;
    ec = CBufferedReader::New(pIFileReader, &pReader);
    pIFileReader->Release();
    if (FAILED(ec)) return ec;

    ec = ReadStringsFromReader(pReader, stringMap);
    pReader->Release();
    return ec;
    //} catch (Exception e) {
    //  Log.e("GeckoCrashReporter", "exception while reading strings: ", e);
    //  return false;
    //}
}

ECode CCrashReporter::ReadStringsFromReader(
    /* [in] */ IBufferedReader* pReader,
    /* [in] */ HashMap<String, String>* stringMap)
{
    String line, key, val;
    ECode ec = pReader->ReadLine(&line);
    while (SUCCEEDED(ec) && !line.IsNull()) {
        Int32 equalsPos = -1;
        if ((equalsPos = line.IndexOf('=')) != -1) {
            key = line.Substring(0, equalsPos);
            Unescape(line.Substring(equalsPos + 1), &val);
            (*stringMap)[key] = val;
        }
        ec = pReader->ReadLine(&line);
    }
    pReader->Close();
    return NOERROR;
}

ECode CCrashReporter::GenerateBoundary(
    /* [out] */ String* boundary)
{
    // Generate some random numbers to fill out the boundary
    Int32 r0 = (Int32)((Double)MAX_INT_VALUE * Math::Random());
    Int32 r1 = (Int32)((Double)MAX_INT_VALUE * Math::Random());

    //return String.format("---------------------------%08X%08X", r0, r1);
    return E_NOT_IMPLEMENTED;
}

ECode CCrashReporter::SendPart(
    /* [in] */ IOutputStream* pOs,
    /* [in] */ const String& boundary,
    /* [in] */ const String& name,
    /* [in] */ const String& data)
{
    //try {
    Int32 bufLen = boundary.GetLength()
                + name.GetLength() + data.GetLength();
    StringBuf* pStringBuf = StringBuf::Alloc(bufLen + 128);
    if (!pStringBuf) return E_OUT_OF_MEMORY;

    pStringBuf->Copy("--");
    pStringBuf->Append(boundary);
    pStringBuf->Append("\r\n");
    pStringBuf->Append("Content-Disposition: form-data; name=\"");
    pStringBuf->Append(name);
    pStringBuf->Append("\"\r\n\r\n");
    pStringBuf->Append(data);
    pStringBuf->Append("\r\n");

    ArrayOf<Byte> buf((Byte*)pStringBuf->GetPayload(), pStringBuf->GetLength());

    ECode ec = pOs->WriteBuffer(buf);
    StringBuf::Free(pStringBuf);
    return ec;
    //} catch (Exception ex) {
    //  Log.e("GeckoCrashReporter", "Exception when sending \"" + name + "\"", ex);
    //}
}

ECode CCrashReporter::SendFile(
    /* [in] */ IOutputStream* pOs,
    /* [in] */ const String& boundary,
    /* [in] */ const String& name,
    /* [in] */ IFile* pFile)
{
    String fileName;
    pFile->GetName(&fileName);
    Int32 bufLen = boundary.GetLength()
                + name.GetLength() + fileName.GetLength();
    StringBuf* pStringBuf = StringBuf::Alloc(bufLen + 128);
    if (!pStringBuf) return E_OUT_OF_MEMORY;

    pStringBuf->Copy("--");
    pStringBuf->Append(boundary);
    pStringBuf->Append("\r\n");
    pStringBuf->Append("Content-Disposition: form-data; ");
    pStringBuf->Append("name=\"");
    pStringBuf->Append(name);
    pStringBuf->Append("\"; ");
    pStringBuf->Append("filename=\"");
    pStringBuf->Append(fileName);
    pStringBuf->Append("\"\r\n");
    pStringBuf->Append("Content-Type: application/octet-stream\r\n");
    pStringBuf->Append("\r\n");

    ArrayOf<Byte> buf((Byte*)pStringBuf->GetPayload(), pStringBuf->GetLength());
    ECode ec = pOs->WriteBuffer(buf);
    StringBuf::Free(pStringBuf);
    if (FAILED(ec)) return ec;

    IFileInputStream* pIns;
    ec = CFileInputStream::New(pFile, &pIns);
    if (FAILED(ec)) return ec;
    IFileChannel* pInFc;
    ec = pIns->GetChannel(&pInFc);
    pIns->Release();
    if (FAILED(ec)) return ec;

    IFileChannel* pOutFc;
    /*ec = Channels::NewChannel(pOs, &pOutFc);
    if (FAILED(ec)) {
        pInFc->Release();
        return ec;
    }*/

    Int64 size, transfered, position = 0;
    pInFc->Size(&size);
    ec = pInFc->TransferTo(position, 
            size, IWritableByteChannel::Probe(pOutFc), &transfered);
    pInFc->Close();
    pInFc->Release();
    pOutFc->Close();
    pOutFc->Release();
    return ec;
}

ECode CCrashReporter::SendReport(
    /* [in] */ IFile* pMinidumpFile,
    /* [in] */ HashMap<String, String> extras,
    /* [in] */ IFile* pExtrasFile)
{
    String path;
    pMinidumpFile->GetPath(&path);
    printf("GeckoCrashReport sendReport: %s\n", path.string());

    HashMap<String, String>::Iterator it = extras.Find(String("ServerURL"));
    if (it != extras.End()) {
        String spec = it->mSecond;
        if (spec.IsNull()) {
            DoFinish();
        }
        printf("GeckoCrashReport server url: %s\n", spec.string());
    }

    //try {
    ECode ec = NOERROR;
    /*IURL* pIUrl;
    String boundary;
    String notes;
    HashMap<String, String> responseMap;
    ec = CURL::New(spec, &pIUrl);
    if (FAILED(ec)) return ec;

    IHttpURLConnection* pConn = NULL;
    IOutputStream* pOs = NULL;
    ec = pIUrl->OpenConnection(&pConn);
    pIUrl->Release();
    if (FAILED(ec)) goto exit;

    ec = pConn->SetRequestMethod("POST");
    if (FAILED(ec)) goto exit;

    ec = GenerateBoundary(&boundary);
    if (FAILED(ec)) goto exit;
    ec = pConn->SetDoOutput(TRUE);
    if (FAILED(ec)) goto exit;

    {
        String request("multipart/form-data; boundary=");
        ec = pConn->SetRequestProperty("Content-Type",  request + boundary);
        if (FAILED(ec)) goto exit;
    }

    ec = pConn->GetOutputStream(&pOs);
    if (FAILED(ec)) goto exit;

    it = extras.Begin();
    for (; it != extras.End(); ++it) {
        String key = it->mFirst;
        if (key.Equals(kPageURLKey)) {
            Boolean isChecked;
            AutoPtr<IView> pView = FindViewById(0x7f070001);
            ICheckBox* pCheckBox = ICheckBox::Probe(pView);
            //pCheckBox->IsChecked(&isChecked); //R.id.include_url
            if (isChecked) {
                SendPart(pOs, boundary, key, it->mSecond);
            }
        }
        else if (!key.Equals("ServerURL") && !key.Equals(kNotesKey)) {
            SendPart(pOs, boundary, key, it->mSecond);
        }
    }

    // Add some extra information to notes so its displayed by
    // crash-stats.mozilla.org. Remove this when bug 607942 is fixed.
    it = extras.Find(kNotesKey);
    if (it != extras.End()) {
        notes = it->mSecond + "\n";
    }
    else {
        notes = "";
    }

    //notes += Build_MANUFACTURER + " ";
    //notes += Build_MODEL + "\n";
    //notes += Build_FINGERPRINT;
    SendPart(pOs, boundary, kNotesKey, notes);

    SendPart(pOs, boundary, String("Min_ARM_Version"), String("7"));
    SendPart(pOs, boundary, String("Android_Manufacturer"), Build_MANUFACTURER);
    SendPart(pOs, boundary, String("Android_Model"), Build_MODEL);
    SendPart(pOs, boundary, String("Android_Board"), Build_BOARD);
    SendPart(pOs, boundary, String("Android_Brand"), Build_BRAND);
    SendPart(pOs, boundary, String("Android_Device"), Build_DEVICE);
    SendPart(pOs, boundary, String("Android_Display"), Build_DISPLAY);
    SendPart(pOs, boundary, String("Android_Fingerprint"), Build_FINGERPRINT);
    SendPart(pOs, boundary, String("Android_CPU_ABI"), Build_CPU_ABI);
    if (1) {//Build_VERSION.SDK_INT >= 8) {
        //try {
        //SendPart(pOs, boundary, String("Android_CPU_ABI2"), Build_CPU_ABI2);
        //SendPart(pOs, boundary, String("Android_Hardware"), Build_HARDWARE);
        //} catch (Exception ex) {
        //  Log.e("GeckoCrashReporter", "Exception while sending SDK version 8 keys", ex);
        //}
    }
    {
        String version(Build_VERSION_SDK_INT);

        version += " (" + Build_VERSION_CODENAME + ")";
        SendPart(pOs, boundary, "Android_Version", version);
    }

    SendFile(pOs, boundary, kMiniDumpPathKey, pMinidumpFile);
    {
        String writeStr("\r\n--");
        writeStr += boundary + "--\r\n";
        ArrayOf<Byte> buf((Byte*)writeStr.string(), writeStr.GetLength());
        ec = pOs->WriteBuffer(buf);
        if (FAILED(ec)) goto exit;
        pOs->Flush();
        pOs->Close();
    }

    IInputStream* pIns;
    ec = pConn->GetInputStream(&pIns);
    if (FAILED(ec)) goto exit;
    IInputStreamReader* pInSReader;
    ec = CInputStreamReader::New(pIns, &pInSReader);
    pIns->Release();
    if (FAILED(ec)) goto exit;

    IBufferedReader* pBr;
    ec = CBufferedReader::New(pInSReader, &pBr);
    pInSReader->Release();
    if (FAILED(ec)) goto exit;

    ec = ReadStringsFromReader(pBr, &responseMap);
    pBr->Release();
    if (FAILED(ec)) goto exit;

    Int32 responseCode;
    pConn->GetResponseCode(&responseCode);
    if (responseCode == pConn->HTTP_OK) {
        IFile* pSubmittedDir;
        ec = CFile::New(
          String("/data/data/org.mozilla.fennec_unofficial/mozilla/Crash Reports/submitted"),
          &pSubmittedDir);
        if (FAILED(ec)) goto exit;
        Boolean succeeded;
        pSubmittedDir->Mkdirs(&succeeded);
        pMinidumpFile->Delete(&succeeded);
        pExtrasFile->Delete(&succeeded);

        String crashid = responseMap[String("CrashID")];
        IFile* pFile;
        ec = CFile::New(pSubmittedDir, crashid + ".txt", &pFile);
        pSubmittedDir->Release();
        if (FAILED(ec)) goto exit;

        IFileOutputStream* pFos;
        ec = CFileOutputStream::New(pFile, &pFos);
        pFile->Release();
        if (FAILED(ec)) goto exit;
        ArrayOf_<Byte, 16> titleBuf;
        titleBuf.Copy((Byte*)"Crash ID: ", strlen("Crash ID: "));
        pFos->WriteBuffer(titleBuf);
        ArrayOf<Byte> idBuf((Byte*)crashid.string(), crashid.GetLength());
        pFos->WriteBuffer(idBuf);
        pFos->Close();
        pFos->Release();
    }
    //} catch (IOException e) {
    //  Log.e("GeckoCrashReporter", "exception during send: ", e);
    //}

exit:
    DoFinish();
    if (pOs) {
        pOs->Release();
    }
    if (pConn) {
        pConn->Release();
    }*/
    return ec;
}

ECode CCrashReporter::DoRestart()
{
    //try {
    String action("android.intent.action.MAIN");
    IIntent* pIIntent;
    ECode ec = CIntent::New(action, &pIIntent);
    if (FAILED(ec)) return ec;
    pIIntent->SetClassName(String("org.mozilla.fennec_unofficial"),
                      String("org.mozilla.fennec_unofficial.App"));
    //printf("GeckoCrashReporter %s\n", pIIntent->ToString());
    ec = StartActivity(pIIntent);
    pIIntent->Release();
    return ec;
    //} catch (Exception e) {
    //  Log.e("GeckoCrashReporter", "error while trying to restart", e);
    //}
}

ECode CCrashReporter::Unescape(
    /* [in] */ const String& string,
    /* [out] */ String* out)
{
    string.Replace("\\\\", "\\");
    string.Replace("\\n", "\n");
    *out = string.Replace("\\t", "\t");
    return NOERROR;
}

