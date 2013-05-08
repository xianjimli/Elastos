
#include "net/CTrafficStats.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>


// Returns an ASCII decimal number read from the specified file, -1 on error.
Int64 ReadNumber(
    /* [in] */Char8 const* filename) 
{
#ifdef HAVE_ANDROID_OS
    Char8 buf[80];
    Int32 fd = open(filename, O_RDONLY);
    if (fd < 0) {
        if (errno != ENOENT){
            //LOGE("Can't open %s: %s", filename, strerror(errno));
        }
        return -1;
    }

    Int32 len = read(fd, buf, sizeof(buf) - 1);
    if (len < 0) {
        //LOGE("Can't read %s: %s", filename, strerror(errno));
        close(fd);
        return -1;
    }

    close(fd);
    buf[len] = '\0';
    return ((Int64)(buf));
#else  // Simulator
    return -1;
#endif
}


// Return the number from the first file which exists and contains data
Int64 TryBoth(
    /* [in] */const Char8* a,
    /* [in] */const Char8* b) 
{
    Int64 num = ReadNumber(a);
    return num >= 0 ? num : ReadNumber(b);
}


// Returns the sum of numbers from the specified path under /sys/class/net/*,
// -1 if no such file exists.
static Int64 ReadTotal(
    /* [in] */Char8 const* suffix) 
{
#ifdef HAVE_ANDROID_OS
    Char8 filename[PATH_MAX] = "/sys/class/net/";
    DIR *dir = opendir(filename);
    if (dir == NULL) {
        //LOGE("Can't list %s: %s", filename, strerror(errno));
        return -1;
    }

    Int32 len = strlen(filename);
    Int64 total = -1;
    while (struct dirent *entry = readdir(dir)) {
        // Skip ., .., and localhost interfaces.
        if (entry->d_name[0] != '.' && strncmp(entry->d_name, "lo", 2) != 0) {
            strlcpy(filename + len, entry->d_name, sizeof(filename) - len);
            strlcat(filename, suffix, sizeof(filename));
            Int64 num = ReadNumber(filename);
            if (num >= 0) total = total < 0 ? num : total + num;
        }
    }

    closedir(dir);
    return total;
#else  // Simulator
    return -1;
#endif
}

CTrafficStats::CTrafficStats()
{}

CTrafficStats::~CTrafficStats()
{}


ECode CTrafficStats::TrafficStatsGetMobileTxPackets(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = TryBoth("/sys/class/net/rmnet0/statistics/tx_packets","/sys/class/net/ppp0/statistics/tx_packets");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetMobileRxPackets(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = TryBoth("/sys/class/net/rmnet0/statistics/rx_packets","/sys/class/net/ppp0/statistics/rx_packets");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetMobileTxBytes(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = TryBoth("/sys/class/net/rmnet0/statistics/tx_bytes","/sys/class/net/ppp0/statistics/tx_bytes");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetMobileRxBytes(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = TryBoth("/sys/class/net/rmnet0/statistics/rx_bytes","/sys/class/net/ppp0/statistics/rx_bytes");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetTotalTxPackets(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = ReadTotal("/statistics/tx_packets");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetTotalRxPackets(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = ReadTotal("/statistics/rx_packets");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetTotalTxBytes(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = ReadTotal("/statistics/tx_bytes");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetTotalRxBytes(
    /* [out] */ Int64* result)
{
//    VALIDATE_NOT_NULL(result);
    *result = ReadTotal("/statistics/rx_bytes");
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetUidTxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    char filename[80];
//    sprintf(filename, "/proc/uid_stat/%d/tcp_rcv", uid);
    *result = ReadNumber(filename);
    return NOERROR;
}

ECode CTrafficStats::TrafficStatsGetUidRxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    char filename[80];
//    sprintf(filename, "/proc/uid_stat/%d/tcp_snd", uid);
    *result = ReadNumber(filename);
    return NOERROR;
}


