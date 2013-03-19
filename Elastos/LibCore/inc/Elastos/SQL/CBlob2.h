#ifndef __CBLOB2_H__
#define __CBLOB2_H__

#include "Blob2.h"
#include "_CBlob2.h"

CarClass(CBlob2), public Blob2
{
public:
    CARAPI constructor();

    CARAPI GetInputStream(
        /** [out] **/IInputStream** stream);

    CARAPI GetOutputStream(
        /** [out] **/IOutputStream** stream);

    CARAPI Close();

    CARAPI Write(
        /** [in] **/Int32 off, 
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 len,
        /** [in] **/const ArrayOf<Byte>& b, 
        /** [out] **/Int32* result);

    CARAPI Read(
        /** [in] **/Int32 off, 
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 len,
        /** [in] **/ArrayOf<Byte>* b, 
        /** [out] **/Int32* result);

    CARAPI Finalize();
};

#endif //__CBLOB2_H__