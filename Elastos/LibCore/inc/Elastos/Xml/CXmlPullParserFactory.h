
#ifndef __CXMLPULLPARSERFACTORY_H__
#define __CXMLPULLPARSERFACTORY_H__

#include "_CXmlPullParserFactory.h"

CarClass(CXmlPullParserFactory)
{
public:
    CARAPI NewPullParser(
        /* [out] */ IXmlPullParser ** ppParser);

private:
    // TODO: Add your private member variables here.
};

#endif // __CXMLPULLPARSERFACTORY_H__
