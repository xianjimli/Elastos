
#ifdef _FRAMEWORK_CORE
#include "utils/Xml.h"
#else
#include "Xml.h"
#endif

AutoPtr<IAttributeSet> Xml::AsAttributeSet(
    /* [in] */ IXmlPullParser* parser)
{
    AutoPtr<IAttributeSet> attrSet = (IAttributeSet*)parser->Probe(EIID_IAttributeSet);
    if (attrSet != NULL) {
        return attrSet;
    }
    else {
//        new XmlPullAttributes(parser);
        return AutoPtr<IAttributeSet>(NULL);
    }
}

AutoPtr<IXmlPullParser> Xml::NewPullParser()
{
    return NULL;
}