
#ifndef __HH_XMLUTILS_H
#define __HH_XMLUTILS_H

#include <ext/frameworkext.h>
#include <elastos.h>

using namespace Elastos;

class XmlUtils
{
public:
    static CARAPI_(void) SkipCurrentTag(
        /* [in] */ IXmlPullParser* parser);

    static CARAPI_(Int32) ConvertValueToList(
        /* [in] */ ICharSequence* value,
        /* [in] */ const ArrayOf<String>& options,
        /* [in] */ Int32 defaultValue);

    static CARAPI_(Boolean) ConvertValueToBoolean(
        /* [in] */ ICharSequence* value,
        /* [in] */ Boolean defaultValue);

    static CARAPI_(Int32) ConvertValueToInt32(
        /* [in] */ ICharSequence* charSeq,
        /* [in] */ Int32 defaultValue);
};

#endif // __HH_XMLUTILS_H
