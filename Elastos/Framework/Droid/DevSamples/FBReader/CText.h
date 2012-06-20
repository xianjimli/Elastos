
#ifndef __CTEXT_H__
#define __CTEXT_H__

#include "_CText.h"

CarClass(CText)
{
public:
    CText();

    ~CText();

    constructor();

    constructor(
        /* [in] */ String text,
        /* [in] */ Int32 index);

private:
    String mText;
    Int32 mIndex;
};

#endif //__CTEXT_H__
