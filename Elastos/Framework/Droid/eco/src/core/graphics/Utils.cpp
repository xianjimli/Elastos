
#include "graphics/Utils.h"
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

namespace Elastos {

const char* AssetStreamAdaptor::TAG = "AssetStreamAdaptor";

bool AssetStreamAdaptor::rewind()
{
    off_t pos = mAsset->seek(0, SEEK_SET);
    if (pos == (off_t)-1) {
        Logger::E(TAG, "----- mAsset->seek(rewind) failed");
        return false;
    }
    return true;
}

size_t AssetStreamAdaptor::read(void* buffer, size_t size)
{
    ssize_t amount;

    if (NULL == buffer) {
        if (0 == size) {  // caller is asking us for our total length
            return mAsset->getLength();
        }
        // asset->seek returns new total offset
        // we want to return amount that was skipped

        off_t oldOffset = mAsset->seek(0, SEEK_CUR);
        if (-1 == oldOffset) {
            Logger::E(TAG, "---- mAsset->seek(oldOffset) failed");
            return 0;
        }
        off_t newOffset = mAsset->seek(size, SEEK_CUR);
        if (-1 == newOffset) {
            Logger::E(TAG,
                StringBuffer("---- mAsset->seek(%d) failed\n") + (Int32)size);
            return 0;
        }
        amount = newOffset - oldOffset;
    } else {
        amount = mAsset->read(buffer, size);
        if (amount <= 0) {
            Logger::E(TAG,
                StringBuffer("---- mAsset->read(%d) returned %d\n")
                + (Int32)size + (Int32)amount);
        }
    }

    if (amount < 0) {
        amount = 0;
    }
    return amount;
}

}; // namespace Elastos
