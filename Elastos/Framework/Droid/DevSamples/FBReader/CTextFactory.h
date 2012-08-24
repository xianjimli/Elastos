
#ifndef __CTEXTFACTORY_H__
#define __CTEXTFACTORY_H__


#include <elastos/AutoPtr.h>

#include "_CTextFactory.h"

CarClass(CTextFactory)
{
public:
    CARAPI ProcessText(
        /* [in] */ ArrayOf<Byte>* text,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 lineHeight,
	    /* [in] */ ITextPaint* textPaint,
        /* [out] */ IObjectContainer** texts);

private:
	Boolean IsValidNodeText(
		/* [in] */ const String& text);

private:
	Int32					mWidth;
	Int32					mHeight;
	AutoPtr<ILayout>		mLayout;
	AutoPtr<ITextPaint> 	mTextPaint;
	Float					mSpacingMult;
	Float					mSpacingAdd;

    Boolean                 mIncludePad;
};

#endif //__CTEXTFACTORY_H__
