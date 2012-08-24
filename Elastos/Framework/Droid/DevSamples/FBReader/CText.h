
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
        /* [in] */ const String& text,
        /* [in] */ Int32 index);

    CARAPI GetText(
        /* [out] */ String* text);

	CARAPI GetOrder(
		/* [out] */  Int32* order);

private:
    String mText;
    Int32 mIndex;
};

#endif //__CTEXT_H__
