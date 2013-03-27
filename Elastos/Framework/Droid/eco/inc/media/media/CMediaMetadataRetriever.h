
#ifndef __CMEDIAMETADATARETRIEVER_H__
#define __CMEDIAMETADATARETRIEVER_H__

#include "_CMediaMetadataRetriever.h"

#include "media/MediaMetadataRetriever.h"

CarClass(CMediaMetadataRetriever), public MediaMetadataRetriever
{
public:
    CARAPI SetDataSource(
        /* [in] */ IFileDescriptor * pFd);

    CARAPI SetDataSourceEx(
        /* [in] */ IContext * pContext,
        /* [in] */ IUri * pUri);

    CARAPI GetFrameAtTime(
        /* [in] */ Int64 timeUs,
        /* [in] */ Int32 option,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI GetFrameAtTimeEx(
        /* [in] */ Int64 timeUs,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI GetFrameAtTimeEx2(
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI GetEmbeddedPicture(
        /* [out, callee] */ ArrayOf<Byte> ** ppGraphic);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CMEDIAMETADATARETRIEVER_H__
