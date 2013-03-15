
#ifndef __XML_H__
#define __XML_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#include "Elastos.Framework.Core.h"
#endif

#include <elastos/AutoPtr.h>

using namespace Elastos;

class Xml
{
public:
    /**
     * Return an AttributeSet interface for use with the given XmlPullParser.
     * If the given parser itself implements AttributeSet, that implementation
     * is simply returned.  Otherwise a wrapper class is
     * instantiated on top of the XmlPullParser, as a proxy for retrieving its
     * attributes, and returned to you.
     *
     * @param parser The existing parser for which you would like an
     *               AttributeSet.
     *
     * @return An AttributeSet you can use to retrieve the
     *         attribute values at each of the tags as the parser moves
     *         through its XML document.
     *
     * @see AttributeSet
     */
    static CARAPI_(AutoPtr<IAttributeSet>) AsAttributeSet(
        /* [in] */ IXmlPullParser* parser);

    static CARAPI_(AutoPtr<IXmlPullParser>) NewPullParser();
};

#endif //__XML_H__