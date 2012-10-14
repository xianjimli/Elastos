
#include "utils/Xml.h"

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