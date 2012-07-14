
#ifndef __CPARAGRAPH_H__
#define __CPARAGRAPH_H__

#include "_CParagraph.h"

CarClass(CParagraph)
{
public:
    CParagraph();

    ~CParagraph();

    CARAPI constructor(
        /* [in] */ Int32 size);

    CARAPI Append(
        /* [in] */ const String& text,
        /* [in] */ Int32 length);

    CARAPI GetData(
        /* [out] */ Handle32* data);

    CARAPI GetLength(
        /* [out] */ Int32* size);

private:
    static const String INDENT;

    ArrayOf<Byte>* mText;
    Int32 mIndex;
};

#endif //__CPARAGRAPH_H__