
#include "server/CDiskStatsService.h"
#include "ext/frameworkext.h"
#include "Elastos.Framework.Core.h"
#include "Elastos.IO.h"
#include <Slogger.h>
#include "os/Environment.h"
#include "os/SystemClock.h"

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

/**
 * This service exists only as a "dumpsys" target which reports
 * statistics about the status of the disk.
 */
ECode CDiskStatsService::constructor(
    /* [in] */ IContext *context)
{
    mContext = context;
    return NOERROR;
}

ECode CDiskStatsService::GetDescription(
    /* [out] */ String * description)
{
    return E_NOT_IMPLEMENTED;
}

//@Override
void CDiskStatsService::Dump(
    /* [in] */ IFileDescriptor * fd,
    /* [in] */ IPrintWriter *pw,
    /* [in] */ const ArrayOf<String> & args)
{
    // This data is accessible to any app -- no permission check needed.
    // Run a quick-and-dirty performance test: write 512 bytes

    ArrayOf<Byte> * junk = ArrayOf<Byte>::Alloc(512);
    for (Int32 i = 0; i < (Int32) junk->GetLength(); i++) {
        (*junk)[i] = (Byte) i;  // Write nonzero bytes
    }

    AutoPtr<IFile> tmp;
    CFile::New(Environment::GetDataDirectory(), String("system/perftest.tmp"),(IFile **) &tmp);
    AutoPtr<IFileOutputStream> fos;
    ECode error = NOERROR;
    Int64 before = SystemClock::GetUptimeMillis();

    CFileOutputStream::New(tmp, (IFileOutputStream **)&fos);
    //AutoPtr<IOutputStream> os = IOutputStream->Probe(fos);
    error = fos->WriteBuffer(*junk);
    fos->Close();

    Int64 after = SystemClock::GetUptimeMillis();
    Boolean exists;
    tmp->Exists(&exists);
    Boolean tmpret;
    if (exists) tmp->Delete(&tmpret);
    String log = String("");
    if (FAILED(error)) {
        log = String("Test-Error: ") +
              String::FromInt32(error);

    } else {
        log = String("Latency: ") +
              String::FromInt64(after - before) +
              String(" ms [512B Data Write]");
    }

    Slogger::D(CString("CDiskStatsService"),CString(log.string()));

    ReportFreeSpace(Environment::GetDataDirectory(), String("Data"), pw);
    ReportFreeSpace(Environment::GetDownloadCacheDirectory(), String("Cache"), pw);
    AutoPtr<IFile> tmp1;
    CFile::New(String("/system"), (IFile **) &tmp1);
    ReportFreeSpace((IFile *)tmp1, String("System"), pw);

    // TODO: Read /proc/yaffs and report interesting values;
    // add configurable (through args) performance test parameters.

}

//private
void CDiskStatsService::ReportFreeSpace(
    /* [in] */ IFile * path,
    /* [in] */ const String& name,
    /* [in] */ IPrintWriter * pw)
{
    //?? lack of android.os.StatFs class
    /*
    StatFs statfs = new StatFs(path.getPath());
    long bsize = statfs.getBlockSize();
    long avail = statfs.getAvailableBlocks();
    long total = statfs.getBlockCount();
    if (bsize <= 0 || total <= 0) {
        throw new IllegalArgumentException(
                "Invalid stat: bsize=" + bsize + " avail=" + avail + " total=" + total);
    }
    */
    Int64 bsize;
    Int64 avail;
    Int64 total;
    String log = name +
            String("-Free: ")+
            String::FromInt64(avail * bsize / 1024)+
            String("K / ");
            String::FromInt64(total * bsize / 1024)+
            String("K total = ");
            String::FromInt64(avail * 100 / total)+
            String("% free");
    Slogger::D(CString("CDiskStatsService"), CString(log.string()));

}

