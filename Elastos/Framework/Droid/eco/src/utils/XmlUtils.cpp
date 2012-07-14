
#include "utils/XmlUtils.h"

void XmlUtils::SkipCurrentTag(
    /* [in] */ IXmlPullParser* parser)
{
    Int32 outerDepth = 0;
    parser->GetDepth(&outerDepth);

    Int32 type = 0;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth) > outerDepth)) {
    }
}

Int32 XmlUtils::ConvertValueToList(
    /* [in] */ ICharSequence* value,
    /* [in] */ const ArrayOf<String>& options,
    /* [in] */ Int32 defaultValue)
{
//	    if (null != value) {
//	        for (int i = 0; i < options.length; i++) {
//	            if (value.equals(options[i]))
//	                return i;
//	        }
//	    }
//
//	    return defaultValue;
    return -1;
}

Boolean XmlUtils::ConvertValueToBoolean(
        /* [in] */ ICharSequence* value,
        /* [in] */ Boolean defaultValue)
{
//	    boolean result = false;
//
//	    if (null == value)
//	        return defaultValue;
//
//	    if (value.equals("1")
//	    ||  value.equals("true")
//	    ||  value.equals("TRUE"))
//	        result = true;
//
//	    return result;
    return FALSE;
}

Int32 XmlUtils::ConvertValueToInt32(
    /* [in] */ ICharSequence* charSeq,
    /* [in] */ Int32 defaultValue)
{
//	    if (null == charSeq)
//	        return defaultValue;
//
//	    String nm = charSeq.toString();
//
//	    // XXX This code is copied from Integer.decode() so we don't
//	    // have to instantiate an Integer!
//
//	    int value;
//	    int sign = 1;
//	    int index = 0;
//	    int len = nm.length();
//	    int base = 10;
//
//	    if ('-' == nm.charAt(0)) {
//	        sign = -1;
//	        index++;
//	    }
//
//	    if ('0' == nm.charAt(index)) {
//	        //  Quick check for a zero by itself
//	        if (index == (len - 1))
//	            return 0;
//
//	        char    c = nm.charAt(index + 1);
//
//	        if ('x' == c || 'X' == c) {
//	            index += 2;
//	            base = 16;
//	        } else {
//	            index++;
//	            base = 8;
//	        }
//	    }
//	    else if ('#' == nm.charAt(index))
//	    {
//	        index++;
//	        base = 16;
//	    }
//
//	    return Integer.parseInt(nm.substring(index), base) * sign;
    return -1;
}

void XmlUtils::BeginDocument(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& firstElementName)
{
}

void XmlUtils::NextElement(
    /* [in] */ IXmlPullParser* parser)
{
}

