
#ifndef __HH_GRAPHICS_UTILS_H
#define __HH_GRAPHICS_UTILS_H

#include <elastos.h>
#include <skia/core/SkStream.h>
#include <utils/Asset.h>
#include <stdio.h>
#include <unistd.h>

namespace Elastos {

class AssetStreamAdaptor : public SkStream
{
public:
    AssetStreamAdaptor(android::Asset* a) : mAsset(a) {}
    virtual bool rewind();
    virtual size_t read(void* buffer, size_t size);

private:
    static const String TAG;

    android::Asset*  mAsset;
};

/** Restore the file descriptor's offset in our destructor
 */
class AutoFDSeek {
public:
    AutoFDSeek(int fd) : mFD(fd) {
        mCurr = lseek(fd, 0, SEEK_CUR);
    }
    ~AutoFDSeek() {
        if (mCurr >= 0) {
            lseek(mFD, mCurr, SEEK_SET);
        }
    }
private:
    int     mFD;
    off_t   mCurr;
};

}; // namespace Elastos

#endif // __HH_GRAPHICS_UTILS_H

