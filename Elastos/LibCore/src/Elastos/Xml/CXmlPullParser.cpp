
#include <Elastos.Core.h>
#include "CXmlPullParser.h"
#include <elastos/AutoFree.h>
#include <StringBuffer.h>
#include <Elastos.IO.h>

using namespace Elastos::Core;

#define FAIL_RETURN(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return ec; \
    } while(0);

#define XPP_ERROR(desc) \
    do { \
        if (mRelaxed) { \
            if (mError.IsNull()) { \
                String error("ERR: "); \
                error += desc;\
                mError = error; \
            } \
        } \
        else { \
            return Exception(desc); \
        } \
    } while(0);

//#define STRING_ASSIGN(dst, src) \
//    do { \
//        String::Free(dst); \
//        dst = String::Duplicate(src); \
//    } while(0);


const String CXmlPullParser::UNEXPECTED_EOF = String("Unexpected EOF");
const String CXmlPullParser::ILLEGAL_TYPE = String("Wrong event type");

const String CXmlPullParser::TYPES[TYPES_LENGTH] = {
    String("IXmlPullParser_START_DOCUMENT"),
    String("IXmlPullParser_END_DOCUMENT"),
    String("IXmlPullParser_START_TAG"),
    String("IXmlPullParser_END_TAG"),
    String("IXmlPullParser_TEXT"),
    String("IXmlPullParser_CDSECT"),
    String("IXmlPullParser_ENTITY_REF"),
    String("IXmlPullParser_IGNORABLE_WHITESPACE"),
    String("IXmlPullParser_PROCESSING_INSTRUCTION"),
    String("IXmlPullParser_COMMENT"),
    String("IXmlPullParser_DOCDECL")
};

CXmlPullParser::CXmlPullParser()
{
}

CXmlPullParser::~CXmlPullParser()
{
    for (Int32 i = 0; i < 16; i++) ((*mAttributes)[i]).~String();
    ArrayOf<String>::Free(mAttributes);
}

// BEGIN android-added
/**
 * Retains namespace attributes like {@code xmlns="http://foo"} or {@code
 * xmlns:foo="http:foo"} in pulled elements. Most applications will only be
 * interested in the effective namespaces of their elements, so these
 * attributes aren't useful. But for structure preserving wrappers like DOM,
 * it is necessary to keep the namespace data around.
 */
void CXmlPullParser::KeepNamespaceAttributes()
{
    mKeepNamespaceAttributes = TRUE;
}
// END android-added

Boolean CXmlPullParser::IsProp(
    /* [in] */ const String& n1,
    /* [in] */ Boolean prop,
    /* [in] */ const char* n2)
{
    if (!n1.StartWith("http://xmlpull.org/v1/doc/")) {
        return FALSE;
    }

    if (prop) {
        return !n1.Substring(42).Compare(n2);
    }
    else {
        return !n1.Substring(40).Compare(n2);
    }
}

ECode CXmlPullParser::AdjustNsp(
    /* [in] */ Boolean* result)
{
    AutoFree<StringBuf> buf1, buf2, buf3;
    Boolean any = FALSE;

    for (Int32 i = 0; i < mAttributeCount << 2; i += 4) {
        // * 4 - 4; i >= 0; i -= 4) {

        String& attrName = (*mAttributes)[i + 2];
        Int32 cut = attrName.IndexOf(':');
        String prefix;

        if (cut != -1) {
            prefix = attrName.Substring(0, cut);
            attrName = attrName.Substring(cut + 1);
        }
        else if (!attrName.Compare("xmlns")) {
            prefix = attrName;
            attrName = NULL;
        }
        else continue;

        if (prefix.Compare("xmlns")) {
            any = TRUE;
        }
        else {
            Int32 j = ((*mNspCounts)[mDepth]++) << 1;

            EnsureCapacity(mNspStack, j + 2, &mNspStack);
            (*mNspStack)[j] = attrName;
            (*mNspStack)[j + 1] = (*mAttributes)[i + 3];

            if (!attrName.IsNull() && (*mAttributes)[i + 3].IsEmpty()) {
                XPP_ERROR("illegal empty namespace");
            }

            //  prefixMap = new PrefixMap (prefixMap, attrName, attr.getValue ());

            //System.out.println (prefixMap);

            // BEGIN android-changed
            if (mKeepNamespaceAttributes) {
                // explicitly set the namespace for unprefixed attributes
                // such as xmlns="http://foo"
                (*mAttributes)[i] = "http://www.w3.org/2000/xmlns/";
                any = TRUE;
            }
            else {
                Int32 length = ((--mAttributeCount) << 2) - i;
                for (Int32 j = 0; j < length; j++) {
                    (*mAttributes)[i + j] = (*mAttributes)[i + 4 + j];
                }

                i -= 4;
            }
            // END android-changed
        }
    }

    if (any) {
        for (Int32 i = (mAttributeCount << 2) - 4; i >= 0; i -= 4) {
            String attrName = (*mAttributes)[i + 2];
            Int32 cut = attrName.IndexOf(':');

            if (cut == 0 && !mRelaxed) {
//                throw new RuntimeException(
//                    "illegal attribute name: " + attrName + " at " + this);
                return E_RUNTIME_EXCEPTION;
            }
            else if (cut != -1) {
                String attrPrefix = attrName.Substring(0, cut);

                attrName = attrName.Substring(cut + 1);

                String attrNs;
                GetNamespaceEx(attrPrefix, &attrNs);

                if (attrNs.IsNull() && !mRelaxed) {
//                    throw new RuntimeException(
//                        "Undefined Prefix: " + attrPrefix + " in " + this);
                    return E_RUNTIME_EXCEPTION;
                }

                (*mAttributes)[i] = attrNs;
                (*mAttributes)[i + 1] = attrPrefix;
                (*mAttributes)[i + 2] = attrName;

                /*
                                    if (!relaxed) {
                                        for (int j = (attributeCount << 2) - 4; j > i; j -= 4)
                                            if (attrName.equals(attributes[j + 2])
                                                && attrNs.equals(attributes[j]))
                                                exception(
                                                    "Duplicate Attribute: {"
                                                        + attrNs
                                                        + "}"
                                                        + attrName);
                                    }
                    */
            }
        }
    }

    Int32 cut = mName.IndexOf(':');

    if (cut == 0) {
        String error("illegal tag name: ");
        error += mName;
        XPP_ERROR(error);
    }

    if (cut != -1) {
        mPrefix = mName.Substring(0, cut);
        mName = mName.Substring(cut + 1);
    }

    GetNamespaceEx(mPrefix, &mNamespace);

    if (mNamespace.IsNull()) {
        if (!mPrefix.IsNull()) {
            String error("undefined prefix: ");
            error += mPrefix;
            XPP_ERROR(error);
        }
        mNamespace = IXmlPullParser_NO_NAMESPACE;
    }

    return any;
}

void CXmlPullParser::EnsureCapacity(
    /* [in] */ ArrayOf<String>* oldArr,
    /* [in] */ Int32 required,
    /* [out] */ ArrayOf<String>** newArr)
{
    Int32 length = oldArr->GetLength();
    if (length >= required) {
        *newArr = oldArr;
        return;
    }
    ArrayOf<String>* bigger = ArrayOf<String>::Alloc(required + 16);

    for (Int32 i = 0; i < length; i++) {
        (*bigger)[i] = (*oldArr)[i];
    }
    ArrayOf<String>::Free(oldArr);
    *newArr = bigger;
}

ECode CXmlPullParser::Exception(
    /* [in] */ const char* desc)
{
//    throw new XmlPullParserException(
//        desc.length() < 100 ? desc : desc.substring(0, 100) + "\n",
//        this,
//        null);
    return E_XML_PULL_PARSER_EXCEPTION;
}

/**
 * common base for next and nextToken. Clears the state, except from
 * txtPos and whitespace. Does not set the type variable */
ECode CXmlPullParser::NextImpl()
{
    if (mReader == NULL) {
        return Exception(String("No Input specified"));
    }

    if (mType == IXmlPullParser_END_TAG) mDepth--;

    while (TRUE) {
        mAttributeCount = -1;

        // degenerated needs to be handled before error because of possible
        // processor expectations(!)

        if (mDegenerated) {
            mDegenerated = FALSE;
            mType = IXmlPullParser_END_TAG;
            return NOERROR;
        }


        if (!mError.IsNull()) {
            Int32 count = mError.GetCharCount();
            for (Int32 i = 0; i < count; i++) {
                Push(mError.GetChar(i));
            }
            //text = error;
            mError = NULL;
            mType = IXmlPullParser_COMMENT;
            return NOERROR;
        }


//        if (relaxed
//            && (stackMismatch > 0 || (peek(0) == -1 && depth > 0))) {
//            int sp = (depth - 1) << 2;
//            type = IXmlPullParser_END_TAG;
//            namespace = elementStack[sp];
//            prefix = elementStack[sp + 1];
//            name = elementStack[sp + 2];
//            if (stackMismatch != 1)
//                error = "missing end tag /" + name + " inserted";
//            if (stackMismatch > 0)
//                stackMismatch--;
//            return;
//        }

        mPrefix = NULL;
        mName = NULL;
        mNamespace = NULL;
        //            text = null;

        FAIL_RETURN(PeekType(&mType));

        switch (mType) {
            case IXmlPullParser_ENTITY_REF :
                return PushEntity();

            case IXmlPullParser_START_TAG :
                return ParseStartTag(FALSE);

            case IXmlPullParser_END_TAG :
                return ParseEndTag();

            case IXmlPullParser_END_DOCUMENT :
                return NOERROR;

            case IXmlPullParser_TEXT :
                // BEGIN android-changed: distinguish attribute values from normal text.
                FAIL_RETURN(PushText('<', !mToken, FALSE));
                // END android-changed
                if (mDepth == 0) {
                    if (mIsWhitespace)
                        mType = IXmlPullParser_IGNORABLE_WHITESPACE;
                    // make exception switchable for instances.chg... !!!!
                    // else
                    //    exception ("text '"+getText ()+"' not allowed outside root element");
                }
                return NOERROR;

            default :
                FAIL_RETURN(ParseLegacy(mToken, &mType));
                if (mType != XML_DECL) return NOERROR;
        }
    }
}

ECode CXmlPullParser::ParseLegacy(
    /* [in] */ Boolean push,
    /* [out] */ Int32* value)
{
    String req;
    Int32 term;
    Int32 result;
    Int32 prev = 0;
    Int32 c;

    FAIL_RETURN(Read(&c)); // <
    FAIL_RETURN(Read(&c));

    if (c == '?') {
        Int32 p0, p1;

        FAIL_RETURN(Peek(0, &p0));
        FAIL_RETURN(Peek(1, &p1));
        if ((p0 == 'x' || p0 == 'X')
            && (p1 == 'm' || p1 == 'M')) {
            if (push) {
                Push(p0);
                Push(p1);
            }
            FAIL_RETURN(Read(&c));
            FAIL_RETURN(Read(&c));

            FAIL_RETURN(Peek(0, &p0));
            FAIL_RETURN(Peek(1, &p1));
            if ((p0 == 'l' || p0 == 'L') && p1 <= ' ') {
                if (mLine != 1 || mColumn > 4) {
                    XPP_ERROR("PI must not start with xml");
                }

                FAIL_RETURN(ParseStartTag(TRUE));

                if (mAttributeCount < 1 || ((*mAttributes)[2]).Compare("version")) {
                    XPP_ERROR("version expected");
                }

                mVersion = (*mAttributes)[3];

                Int32 pos = 1;

                if (pos < mAttributeCount
                    && !((*mAttributes)[2 + 4]).Compare("encoding")) {
                    mEncoding = (*mAttributes)[3 + 4];
                    pos++;
                }

                if (pos < mAttributeCount
                    && !((*mAttributes)[4 * pos + 2]).Compare("standalone")) {
                    String st = (*mAttributes)[3 + 4 * pos];
                    if (!st.Compare("yes")) {
                        mStandalone = TRUE;
                    }
                    else if (!st.Compare("no")) {
                        mStandalone = FALSE;
                    }
                    else {
                        String error("illegal standalone value: ");
                        error += st;
                        XPP_ERROR(error);
                    }
                    pos++;
                }

                if (pos != mAttributeCount) {
                    XPP_ERROR("illegal xmldecl");
                }

                mIsWhitespace = TRUE;
                mTxtPos = 0;

                *value = XML_DECL;
                return NOERROR;
            }
        }

        /*            int c0 = read ();
                    int c1 = read ();
                    int */

        term = '?';
        result = IXmlPullParser_PROCESSING_INSTRUCTION;
    }
    else if (c == '!') {
        Int32 p0;
        FAIL_RETURN(Peek(0, &p0));
        if (p0 == '-') {
            result = IXmlPullParser_COMMENT;
            req = "--";
            term = '-';
        }
        else if (p0 == '[') {
            result = IXmlPullParser_CDSECT;
            req = "[CDATA[";
            term = ']';
            push = TRUE;
        }
        else {
            result = IXmlPullParser_DOCDECL;
            req = "DOCTYPE";
            term = -1;
        }
    }
    else {
        StringBuffer sb("illegal: <");
        sb += c;
        XPP_ERROR((const char*)sb);
        *value = IXmlPullParser_COMMENT;
        return NOERROR;
    }

    Int32 count = req.GetCharCount();
    for (Int32 i = 0; i < count; i++) {
        FAIL_RETURN(Read(req.GetChar(i)));
    }

    if (result == IXmlPullParser_DOCDECL) {
        FAIL_RETURN(ParseDoctype(push));
    }
    else {
        while (TRUE) {
            FAIL_RETURN(Read(&c));
            if (c == -1){
                XPP_ERROR(UNEXPECTED_EOF);
                *value = IXmlPullParser_COMMENT;
                return NOERROR;
            }

            if (push) {
                Push(c);
            }

            Int32 p0, p1;
            FAIL_RETURN(Peek(0, &p0));
            FAIL_RETURN(Peek(1, &p1));
            if ((term == '?' || c == term)
                && p0 == term
                && p1 == '>') {
                break;
            }

            prev = c;
        }

        if (term == '-' && prev == '-' && !mRelaxed) {
            XPP_ERROR("illegal comment delimiter: --->");
        }

        FAIL_RETURN(Read(&c));
        FAIL_RETURN(Read(&c));

        if (push && term != '?') {
            mTxtPos--;
        }

    }
    *value = result;
    return NOERROR;
}

/** precondition: &lt! consumed */

ECode CXmlPullParser::ParseDoctype(
    /* [in] */ Boolean push)
{
    Int32 nesting = 1;
    Boolean quoted = FALSE;

    // read();

    while (TRUE) {
        Int32 i;
        FAIL_RETURN(Read(&i));
        switch (i) {
            case -1 :
                XPP_ERROR(UNEXPECTED_EOF);
                return NOERROR;

            case '\'' :
                quoted = !quoted;
                break;

            case '<' :
                if (!quoted) {
                    nesting++;
                }
                break;

            case '>' :
                if (!quoted) {
                    if ((--nesting) == 0)
                        return NOERROR;
                }
                break;
        }
        if (push) {
            Push(i);
        }
    }
}

/* precondition: &lt;/ consumed */

ECode CXmlPullParser::ParseEndTag()
{
    Int32 c;

    FAIL_RETURN(Read(&c)); // '<'
    FAIL_RETURN(Read(&c)); // '/'
    FAIL_RETURN(ReadName(&mName));
    FAIL_RETURN(Skip());
    FAIL_RETURN(Read('>'));

    Int32 sp = (mDepth - 1) << 2;

    if (mDepth == 0) {
        XPP_ERROR("element stack empty");
        mType = IXmlPullParser_COMMENT;
        return NOERROR;
    }

    if (!mRelaxed) {
      if (mName.Compare((*mElementStack)[sp + 3])) {
        StringBuffer sb("expected: /");
        sb += (*mElementStack)[sp + 3];
        sb += StringBuffer(" read: ") + mName;

        XPP_ERROR((const char*)sb);

          // become case insensitive in relaxed mode

//        int probe = sp;
//        while (probe >= 0 && !name.toLowerCase().equals(elementStack[probe + 3].toLowerCase())) {
//            stackMismatch++;
//            probe -= 4;
//        }
//
//        if (probe < 0) {
//            stackMismatch = 0;
//            // text = "unexpected end tag ignored";
//            type = IXmlPullParser_COMMENT;
//            return;
//        }
        }

        mNamespace = (*mElementStack)[sp];
        mPrefix = (*mElementStack)[sp + 1];
        mName = (*mElementStack)[sp + 2];
    }
    return NOERROR;
}

ECode CXmlPullParser::PeekType(
    /* [out] */ Int32* type)
{
    Int32 c;
    FAIL_RETURN(Peek(0, &c));
    switch (c) {
        case -1 :
            *type = IXmlPullParser_END_DOCUMENT;
            return NOERROR;
        case '&' :
            *type = IXmlPullParser_ENTITY_REF;
            return NOERROR;
        case '<' :
            FAIL_RETURN(Peek(1, &c));
            switch (c) {
                case '/' :
                    *type = IXmlPullParser_END_TAG;
                    return NOERROR;
                case '?' :
                case '!' :
                    *type = LEGACY;
                    return NOERROR;
                default :
                    *type = IXmlPullParser_START_TAG;
                    return NOERROR;
            }
        default :
            *type = IXmlPullParser_TEXT;
            return NOERROR;
    }
}

void CXmlPullParser::Get(
    /* [in] */ Int32 pos,
    /* [out] */ String* str)
{
    char* ss = (char*)malloc(mTxtPos - pos);
    strncpy(ss, mTxtBuf->GetPayload() + pos, mTxtPos - pos);
    *str = ss;
}

/*
private final String pop (int pos) {
String result = new String (txtBuf, pos, txtPos - pos);
txtPos = pos;
return result;
}
*/
void CXmlPullParser::Push(
    /* [in] */ Int32 c)
{
    mIsWhitespace &= (c <= ' ');

    if (mTxtPos == mTxtBuf->GetLength()) {
        ArrayOf<Char8>* bigger = ArrayOf<Char8>::Alloc(mTxtPos * 4 / 3 + 4);
        for (Int32 i = 0; i < mTxtPos; i++) {
            (*bigger)[i] = (*mTxtBuf)[i];
        }
        ArrayOf<Char8>::Free(mTxtBuf);
        mTxtBuf = bigger;
    }

    (*mTxtBuf)[mTxtPos++] = (char)c;
}

/** Sets name and attributes */
ECode CXmlPullParser::ParseStartTag(
    /* [in] */ Boolean xmldecl)
{
    Int32 cc;

    if (!xmldecl) {
        FAIL_RETURN(Read(&cc));
    }
    FAIL_RETURN(ReadName(&mName));
    mAttributeCount = 0;

    while (TRUE) {
        FAIL_RETURN(Skip());

        Int32 c;
        FAIL_RETURN(Peek(0, &c));

        if (xmldecl) {
            if (c == '?') {
                FAIL_RETURN(Read(&cc));
                FAIL_RETURN(Read('>'));
                return NOERROR;
            }
        }
        else {
            if (c == '/') {
                mDegenerated = TRUE;
                FAIL_RETURN(Read(&cc));
                FAIL_RETURN(Skip());
                FAIL_RETURN(Read('>'));
                break;
            }
            else if (c == '>' && !xmldecl) {
                FAIL_RETURN(Read(&cc));
                break;
            }
        }

        if (c == -1) {
            XPP_ERROR(UNEXPECTED_EOF);
            //type = IXmlPullParser_COMMENT;
            return NOERROR;
        }

        String attrName;
        FAIL_RETURN(ReadName(&attrName));

        if (attrName.GetLength() == 0) {
            XPP_ERROR("attr name expected");
           //type = IXmlPullParser_COMMENT;
            break;
        }

        Int32 i = (mAttributeCount++) << 2;

        EnsureCapacity(mAttributes, i + 4, &mAttributes);

        (*mAttributes)[i++] = "";
        (*mAttributes)[i++] = NULL;
        (*mAttributes)[i++] = attrName;

        FAIL_RETURN(Skip());

        FAIL_RETURN(Peek(0, &c));
        if (c != '=') {
            if(!mRelaxed){
                String error("Attr.value missing f. ");
                error += attrName;
                XPP_ERROR(error);
            }
            (*mAttributes)[i] = attrName;
        }
        else {
            FAIL_RETURN(Read('='));
            FAIL_RETURN(Skip());
            Int32 delimiter;
            FAIL_RETURN(Peek(0, &delimiter));

            if (delimiter != '\'' && delimiter != '"') {
                if(!mRelaxed){
                    XPP_ERROR("attr value delimiter missing!");
                }
                delimiter = ' ';
            }
            else {
                FAIL_RETURN(Read(&cc));
            }

            Int32 p = mTxtPos;
            // BEGIN android-changed: distinguish attribute values from normal text.
            FAIL_RETURN(PushText(delimiter, TRUE, TRUE));
            // END android-changed

            Get(p, &((*mAttributes)[i]));
            mTxtPos = p;

            if (delimiter != ' ') {
                FAIL_RETURN(Read(&cc)); // skip endquote
            }
        }
    }

    Int32 sp = mDepth++ << 2;

    EnsureCapacity(mElementStack, sp + 4, &mElementStack);
    (*mElementStack)[sp + 3] = mName;

    if (mDepth >= mNspCounts->GetLength()) {
        ArrayOf<Int32>* bigger = ArrayOf<Int32>::Alloc(mDepth + 16);

        for (Int32 i = 0; i < mNspCounts->GetLength(); i++) {
            (*bigger)[i] = (*mNspCounts)[i];
        }
        ArrayOf<Int32>::Free(mNspCounts);
        mNspCounts = bigger;
    }

    (*mNspCounts)[mDepth] = (*mNspCounts)[mDepth - 1];

    /*
        if(!relaxed){
            for (int i = attributeCount - 1; i > 0; i--) {
                for (int j = 0; j < i; j++) {
                    if (getAttributeName(i).equals(getAttributeName(j)))
                        exception("Duplicate Attribute: " + getAttributeName(i));
                }
            }
        }
    */
    if (mProcessNsp) {
        Boolean adjusted;
        FAIL_RETURN(AdjustNsp(&adjusted));
    }
    else {
        mNamespace = "";
    }

    (*mElementStack)[sp] = mNamespace;
    (*mElementStack)[sp + 1] = mPrefix;
    (*mElementStack)[sp + 2] = mName;
    return NOERROR;
}

/**
 * result: isWhitespace; if the setName parameter is set,
 * the name of the entity is stored in "name" */

ECode CXmlPullParser::PushEntity()
{
    Int32 cc;

    FAIL_RETURN(Read(&cc)); // &
    Push(cc);

    Int32 pos = mTxtPos;

    while (TRUE) {
        Int32 c;
        FAIL_RETURN(Peek(0, &c));
        if (c == ';') {
          FAIL_RETURN(Read(&cc));
          break;
        }
        if (c < 128 && (c < '0' || c > '9') && (c < 'a' || c > 'z') && (c < 'A' || c > 'Z')
                && c != '_' && c != '-' && c != '#') {
            if(!mRelaxed){
                XPP_ERROR("unterminated entity ref");
            }

            // BEGIN android-removed: avoid log spam.
            // System.out.println("broken entitiy: "+get(pos-1));
            // END android-removed

            //; ends with:"+(char)c);
//            if (c != -1)
//                push(c);
            return NOERROR;
        }

        FAIL_RETURN(Read(&cc));
        Push(cc);
    }

    String code;
    Get(pos, &code);
    mTxtPos = pos - 1;
    if (mToken && mType == IXmlPullParser_ENTITY_REF){
        mName = code;
    }

    if (code.GetChar(0) == '#') {
        Int32 c =
            (code.GetChar(1) == 'x'
                ? strtol(code.Substring(2), NULL, 16)
                : strtol(code.Substring(1), NULL, 10));
        Push(c);
        return NOERROR;
    }

    String result = (String)(*mEntityMap)[code];

    mUnresolved = result.IsNull();

    if (mUnresolved) {
        if (!mToken) {
            StringBuffer sb("unresolved: &");
            sb += code;
            sb += ";";
            XPP_ERROR((const char*)sb);
        }
    }
    else {
        Int32 count = result.GetCharCount();
        for (Int32 i = 0; i < count; i++) {
            Push(result.GetChar(i));
        }
    }
    return NOERROR;
}

/** types:
'<': parse to any token (for nextToken ())
'"': parse to quote
' ': parse to whitespace or '>'
*/

ECode CXmlPullParser::PushText(
    /* [in] */ Int32 delimiter,
    /* [in] */ Boolean resolveEntities,
    /* [in] */ Boolean inAttributeValue)
{
    Int32 next;
    FAIL_RETURN(Peek(0, &next));
    Int32 cbrCount = 0;
    Int32 c;

    while (next != -1 && next != delimiter) { // covers eof, '<', '"'
        if (delimiter == ' ') {
            if (next <= ' ' || next == '>') {
                break;
            }
        }

        // BEGIN android-changed: "<" is not allowed in attribute values.
        if (next == '&') {
            if (!resolveEntities) {
                break;
            }

            FAIL_RETURN(PushEntity());
        }
        else if (next == '<' && inAttributeValue) {
            XPP_ERROR("Illegal: \"<\" inside attribute value");
        }
        else if (next == '\n' && mType == IXmlPullParser_START_TAG) {
            FAIL_RETURN(Read(&c));
            Push(' ');
        }
        else {
            FAIL_RETURN(Read(&c));
            Push(c);
        }
        // END android-changed

        // BEGIN android-changed: "]]>" *is* allowed in attribute values, but
        // is not allowed in regular text between markup.
        Boolean allowCloseCdata = inAttributeValue;
        if (!allowCloseCdata && (next == '>' && cbrCount >= 2 && delimiter != ']')) {
            XPP_ERROR("Illegal: \"]]>\" outside CDATA section");
        }
        // END android-changed

        if (next == ']') {
            cbrCount++;
        }
        else {
            cbrCount = 0;
        }

        FAIL_RETURN(Peek(0, &next));
    }
    return NOERROR;
}

ECode CXmlPullParser::Read(
    /* [in] */ Char8 c)
{
    Int32 a;
    FAIL_RETURN(Read(&a));
    if (a != c) {
        StringBuffer sb("expected: '");
        sb += c;
        sb += "' actual: '";
        sb += (char)a;
        sb += "'";
        XPP_ERROR((const char*)sb);
    }
    return NOERROR;
}

ECode CXmlPullParser::Read(
    /* [out] */ Int32* value)
{
    if (mPeekCount == 0) {
        FAIL_RETURN(Peek(0, value));
    }
    else {
        *value = mPeek[0];
        mPeek[0] = mPeek[1];
    }
    // else {
    //     result = peek[0];
    //     System.arraycopy (peek, 1, peek, 0, peekCount-1);
    // }
    mPeekCount--;

    mColumn++;

    if (*value == '\n') {

        mLine++;
        mColumn = 1;
    }

    return NOERROR;
}

/** Does never read more than needed */
ECode CXmlPullParser::Peek(
    /* [in] */ Int32 pos,
    /* [out] */ Int32* value)
{
    while (pos >= mPeekCount) {
        Int32 nw;

        if (mSrcBuf->GetLength() <= 1) {
            FAIL_RETURN(mReader->Read(&nw));
        }
        else if (mSrcPos < mSrcCount) {
            nw = (*mSrcBuf)[mSrcPos++];
        }
        else {
            FAIL_RETURN(mReader->ReadBufferEx(0, mSrcBuf->GetLength(), mSrcBuf, &mSrcCount));
            if (mSrcCount <= 0) {
                nw = -1;
            }
            else {
                nw = (*mSrcBuf)[0];
            }

            mSrcPos = 1;
        }

        if (nw == '\r') {
            mWasCR = TRUE;
            mPeek[mPeekCount++] = '\n';
        }
        else {
            if (nw == '\n') {
                if (!mWasCR) {
                    mPeek[mPeekCount++] = '\n';
                }
            }
            else {
                mPeek[mPeekCount++] = nw;
            }

            mWasCR = FALSE;
        }
    }

    *value = mPeek[pos];
    return NOERROR;
}

ECode CXmlPullParser::ReadName(
    /* [out] */ String* name)
{
    Int32 pos = mTxtPos;
    Int32 c;
    FAIL_RETURN(Peek(0, &c));

    if ((c < 'a' || c > 'z')
        && (c < 'A' || c > 'Z')
        && c != '_'
        && c != ':'
        && c < 0x0c0
        && !mRelaxed) {
        XPP_ERROR("name expected");
    }

    do {
        FAIL_RETURN(Read(&c));
        Push(c);
        FAIL_RETURN(Peek(0, &c));
    }
    while ((c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '_'
        || c == '-'
        || c == ':'
        || c == '.'
        || c >= 0x0b7);

    Get(pos, name);
    mTxtPos = pos;
    return NOERROR;
}

ECode CXmlPullParser::Skip()
{
    while (TRUE) {
        Int32 c;
        FAIL_RETURN(Peek(0, &c));
        if (c > ' ' || c == -1) return NOERROR;
        FAIL_RETURN(Read(&c));
    }
}

//  public part starts here...

ECode CXmlPullParser::SetInput(
    /* [in]*/ IReader* reader)
{
    mReader = reader;

    mLine = 1;
    mColumn = 0;
    mType = IXmlPullParser_START_DOCUMENT;
    mName = NULL;
    mNamespace = NULL;
    mDegenerated = FALSE;
    mAttributeCount = -1;
    mEncoding = NULL;
    mVersion = NULL;
//    standalone = null;

    if (reader == NULL) return NOERROR;

    mSrcPos = 0;
    mSrcCount = 0;
    mPeekCount = 0;
    mDepth = 0;

    mEntityMap = new HashMap<String, String>(50);
    (*mEntityMap)[String("amp")] = "&";
    (*mEntityMap)[String("apos")] = "'";
    (*mEntityMap)[String("gt")] = ">";
    (*mEntityMap)[String("lt")] = "<";
    (*mEntityMap)[String("quot")] = "\"";
    return NOERROR;
}

ECode CXmlPullParser::SetInputEx(
    /* [in] */ IInputStream* is,
    /* [in] */ const String& inputEncoding)
{
    AutoFree<StringBuf> buf1;
    mSrcPos = 0;
    mSrcCount = 0;
    String enc = inputEncoding;

    if (is == NULL) return E_INVALID_ARGUMENT;

//    try {

    if (enc.IsNull()) {
        // read four bytes

        Int32 chk = 0;

        while (mSrcCount < 4) {
            Int32 b;
            FAIL_RETURN(is->Read(&b));
            if (b == -1) break;
            chk = (chk << 8) | b;
            (*mSrcBuf)[mSrcCount++] = b;
        }

        if (mSrcCount == 4) {
            switch(chk) {
                case 0x00000FEFF :
                    enc = "UTF-32BE";
                    mSrcCount = 0;
                    break;

                case 0x0FFFE0000 :
                    enc = "UTF-32LE";
                    mSrcCount = 0;
                    break;

                case 0x03c :
                    enc = "UTF-32BE";
                    (*mSrcBuf)[0] = '<';
                    mSrcCount = 1;
                    break;

                case 0x03c000000 :
                    enc = "UTF-32LE";
                    (*mSrcBuf)[0] = '<';
                    mSrcCount = 1;
                    break;

                case 0x0003c003f :
                    enc = "UTF-16BE";
                    (*mSrcBuf)[0] = '<';
                    (*mSrcBuf)[1] = '?';
                    mSrcCount = 2;
                    break;

                case 0x03c003f00 :
                    enc = "UTF-16LE";
                    (*mSrcBuf)[0] = '<';
                    (*mSrcBuf)[1] = '?';
                    mSrcCount = 2;
                    break;

                case 0x03c3f786d :
                    while(TRUE) {
                        Int32 b;
                        FAIL_RETURN(is->Read(&b));
                        if (b == -1) break;
                        (*mSrcBuf)[mSrcCount++] = b;
                        if (b == '>') {
                            String s((char*)mSrcBuf->GetPayload(), mSrcCount);
                            Int32 i0 = s.IndexOf("encoding");
                            if (i0 != -1) {
                                while ((Char8)s.GetChar(i0) != '"'
                                    && (Char8)s.GetChar(i0) != '\'') {
                                    i0++;
                                }
                                Char8 deli = (Char8)s.GetChar(i0++);
                                Int32 i1 = s.Substring(i0).IndexOf(deli);
                                enc = s.Substring(i0, i1);
                            }
                            break;
                        }
                    }

                default :
                    if ((chk & 0x0ffff0000) == 0x0FEFF0000) {
                        enc = "UTF-16BE";
                        (*mSrcBuf)[0] = (Byte)(((*mSrcBuf)[2] << 8) | (*mSrcBuf)[3]);
                        mSrcCount = 1;
                    }
                    else if ((chk & 0x0ffff0000) == 0x0fffe0000) {
                        enc = "UTF-16LE";
                        (*mSrcBuf)[0] = (Byte)(((*mSrcBuf)[3] << 8) | (*mSrcBuf)[2]);
                        mSrcCount = 1;
                    }
                    else if ((chk & 0x0ffffff00) == 0x0EFBBBF00) {
                        enc = "UTF-8";
                        (*mSrcBuf)[0] = (*mSrcBuf)[3];
                        mSrcCount = 1;
                    }
            }
        }
    }

    if (enc.IsNull()) enc = "UTF-8";

    Int32 sc = mSrcCount;
    AutoPtr<IInputStreamReader> reader;
    CInputStreamReader::New(is, enc, (IInputStreamReader**)&reader);
    SetInput((IReader*)reader.Get());
    mEncoding = inputEncoding;
    mSrcCount = sc;

//    }
//    catch (Exception e) {
//        throw new XmlPullParserException(
//            "Invalid stream or encoding: " + e.toString(),
//            this,
//            e);
//    }

    return NOERROR;
}

ECode CXmlPullParser::GetFeature(
    /* [in] */ const String& name,
    /* [out] */ Boolean* value)
{
    if (value == NULL) return E_INVALID_ARGUMENT;

    if (!String(IXmlPullParser_FEATURE_PROCESS_NAMESPACES).Compare(name)) {
        *value = mProcessNsp;
    }
    else if (IsProp(name, FALSE, "relaxed")) {
        *value = mRelaxed;
    }
    else {
        *value = FALSE;
    }

    return NOERROR;
}

ECode CXmlPullParser::GetInputEncoding(
    /* [out] */ String* inputEncoding)
{
    if (inputEncoding == NULL) return E_INVALID_ARGUMENT;

    *inputEncoding = mEncoding;
    return NOERROR;
}

ECode CXmlPullParser::DefineEntityReplacementText(
    /* [in] */ const String& entityName,
    /* [in] */ const String& replacementText)
{
    if (mEntityMap == NULL) {
//        throw new RuntimeException("entity replacement text must be defined after setInput!");
        return E_RUNTIME_EXCEPTION;
    }
    (*mEntityMap)[entityName] = replacementText;
    return NOERROR;
}

ECode CXmlPullParser::GetProperty(
    /* [in] */ const String& name,
    /* [out] */ IInterface** value)
{
//    if (value == NULL) return E_INVALID_ARGUMENT;
//
//    if (isProp(name, TRUE, "xmldecl-version")) {
//        return version;
//    }
//
//    if (isProp(name, true, "xmldecl-standalone")) {
//        return standalone;
//    }
//
//    if (isProp(name, true, "location")) {
//        return location != null ? location : reader.toString();
//    }
//
//    return null;
    return E_NOT_IMPLEMENTED;
}

ECode CXmlPullParser::GetNamespaceCount(
    /* [in] */ Int32 depth,
    /* [out] */ Int32* count)
{
    if (count == NULL || depth > mDepth) return E_INVALID_ARGUMENT;

    *count = (*mNspCounts)[depth];
    return NOERROR;
}

ECode CXmlPullParser::GetNamespacePrefix(
    /* [in] */ Int32 pos,
    /* [out] */ String* prefix)
{
    if (prefix == NULL) return E_INVALID_ARGUMENT;

    *prefix = (*mNspStack)[pos << 1];
    return NOERROR;
}

ECode CXmlPullParser::GetNamespaceUri(
    /* [in] */ Int32 pos,
    /* [out] */ String* uri)
{
    if (uri == NULL) return E_INVALID_ARGUMENT;

    *uri = (*mNspStack)[(pos << 1) + 1];
    return NOERROR;
}

ECode CXmlPullParser::GetNamespaceEx(
    /* [in] */ const String& prefix,
    /* [out] */ String* ns)
{
    if (ns == NULL) return E_INVALID_ARGUMENT;

    if (!prefix.Compare("xml")) {
        *ns = "http://www.w3.org/XML/1998/namespace";
        return NOERROR;
    }
    if (!prefix.Compare("xmlns")) {
        *ns = "http://www.w3.org/2000/xmlns/";
        return NOERROR;
    }

    Int32 i;
    GetNamespaceCount(mDepth, &i);
    i = (i << 1) - 2;
    for (; i >= 0; i -= 2) {
        if (prefix.IsNull()) {
            if ((*mNspStack)[i].IsNull()) {
                *ns = (*mNspStack)[i + 1];
                return NOERROR;
            }
        }
        else if (!prefix.Compare((*mNspStack)[i])) {
            *ns = (*mNspStack)[i + 1];
            return NOERROR;
        }
    }
    *ns = NULL;
    return NOERROR;
}

ECode CXmlPullParser::GetDepth(
    /* [out] */ Int32* depth)
{
    if (depth == NULL) return E_INVALID_ARGUMENT;

    *depth = mDepth;
    return NOERROR;
}

ECode CXmlPullParser::GetPositionDescription(
    /* [out] */ String* des)
{
    String text;

    if (des == NULL) return E_INVALID_ARGUMENT;

    StringBuffer buf(mType < TYPES_LENGTH ? (const char*)TYPES[mType] : "unknown");
    buf += ' ';

    if (mType == IXmlPullParser_START_TAG || mType == IXmlPullParser_END_TAG) {
        if (mDegenerated) {
            buf += "(empty) ";
        }
        buf += '<';
        if (mType == IXmlPullParser_END_TAG) {
            buf += '/';
        }

        if (!mPrefix.IsNull()) {
            buf += "{" + mNamespace + "}" + mPrefix + ":";
        }
        buf += mName;

        Int32 cnt = mAttributeCount << 2;
        for (Int32 i = 0; i < cnt; i += 4) {
            buf += ' ';
            if (!(*mAttributes)[i + 1].IsNull()) {
                buf += "{" + (*mAttributes)[i] + "}" + (*mAttributes)[i + 1] + ":";
            }
            buf += (*mAttributes)[i + 2];
            buf += "='" + (*mAttributes)[i + 3] + "'";
        }

        buf += '>';
    }
    else if (mType == IXmlPullParser_IGNORABLE_WHITESPACE);
    else if (mType != IXmlPullParser_TEXT) {
        GetText(&text);
        buf += text;
    }
    else if (mIsWhitespace)
        buf += "(whitespace)";
    else {
        GetText(&text);
        if (text.GetLength() > 16) {
            buf += (const char*)text.Substring(0, 16);
            buf += "...";
        }
        else {
            buf += text;
        }
    }

    buf += "@" + mLine;
    buf += ":" + mColumn;
    if(mLocation != NULL){
        buf += " in ";
//        buf += mLocation;
    }
    else if(mReader != NULL){
        buf += " in ";
//        buf.append(reader.toString());
    }
    *des = (const char*)buf;
    return NOERROR;
}

ECode CXmlPullParser::GetLineNumber(
    /* [out] */ Int32* number)
{
    if (number == NULL) return E_INVALID_ARGUMENT;

    *number = mLine;
    return NOERROR;
}

ECode CXmlPullParser::GetColumnNumber(
    /* [out] */ Int32* number)
{
    if (number == NULL) return E_INVALID_ARGUMENT;

    *number = mColumn;
    return NOERROR;
}

ECode CXmlPullParser::IsWhitespace(
    /* [out] */ Boolean* result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;

    if (mType != IXmlPullParser_TEXT && mType != IXmlPullParser_IGNORABLE_WHITESPACE && mType != IXmlPullParser_CDSECT) {
        return Exception(ILLEGAL_TYPE);
    }
    *result = mIsWhitespace;
    return NOERROR;
}

ECode CXmlPullParser::GetText(
    /* [out] */ String* text)
{
    if (text == NULL) return E_INVALID_ARGUMENT;

    if ((mType < IXmlPullParser_TEXT) || (mType == IXmlPullParser_ENTITY_REF && mUnresolved)) {
        *text = NULL;
        return NOERROR;
    }
    else {
        Get(0, text);
        return NOERROR;
    }
}

ECode CXmlPullParser::GetNamespace(
    /* [out] */ String* ns)
{
    if (ns == NULL) return E_INVALID_ARGUMENT;

    *ns = mNamespace;
    return NOERROR;
}

ECode CXmlPullParser::GetName(
    /* [out] */ String* name)
{
    if (name == NULL) return E_INVALID_ARGUMENT;

    *name = mName;
    return NOERROR;
}

ECode CXmlPullParser::GetPrefix(
    /* [out] */ String* prefix)
{
    if (prefix == NULL) return E_INVALID_ARGUMENT;

    *prefix = mPrefix;
    return NOERROR;
}

ECode CXmlPullParser::IsEmptyElementTag(
    /* [out] */ Boolean* result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;

    if (mType != IXmlPullParser_START_TAG) {
        return Exception(ILLEGAL_TYPE);
    }
    *result = mDegenerated;
    return NOERROR;
}

ECode CXmlPullParser::GetAttributeCount(
    /* [out] */ Int32* attrCount)
{
    if (attrCount == NULL) return E_INVALID_ARGUMENT;

    *attrCount = mAttributeCount;
    return NOERROR;
}

ECode CXmlPullParser::GetAttributeType(
    /* [in] */ Int32 index,
    /* [out] */ String* attrType)
{
    if (attrType == NULL) return E_INVALID_ARGUMENT;

    *attrType = "CDATA";
    return NOERROR;
}

ECode CXmlPullParser::IsAttributeDefault(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;

    *result = FALSE;
    return NOERROR;
}

ECode CXmlPullParser::GetAttributeNamespace(
    /* [in] */ Int32 index,
    /* [out] */ String* attrNamespace)
{
    if (attrNamespace == NULL || index >= mAttributeCount) {
        return E_INVALID_ARGUMENT;
    }

    *attrNamespace = (*mAttributes)[index << 2];
    return NOERROR;
}

ECode CXmlPullParser::GetAttributeName(
    /* [in] */ Int32 index,
    /* [out] */ String* attrName)
{
    if (attrName == NULL || index >= mAttributeCount) {
        return E_INVALID_ARGUMENT;
    }

    *attrName = (*mAttributes)[(index << 2) + 2];
    return NOERROR;
}

ECode CXmlPullParser::GetAttributePrefix(
    /* [in] */ Int32 index,
    /* [out] */ String* attrPrefix)
{
    if (attrPrefix == NULL || index >= mAttributeCount) {
        return E_INVALID_ARGUMENT;
    }

    *attrPrefix = (*mAttributes)[(index << 2) + 1];
    return NOERROR;
}

ECode CXmlPullParser::GetAttributeValue(
    /* [in] */ Int32 index,
    /* [out] */ String* attrValue)
{
    if (attrValue == NULL || index >= mAttributeCount) {
        return E_INVALID_ARGUMENT;
    }

    *attrValue = (*mAttributes)[(index << 2) + 3];
    return NOERROR;
}

ECode CXmlPullParser::GetAttributeValueEx(
    /* [in] */ CString ns,
    /* [in] */ CString name,
    /* [out] */ String* attrValue)
{
    if (attrValue == NULL) return E_INVALID_ARGUMENT;

    for (Int32 i = (mAttributeCount << 2) - 4; i >= 0; i -= 4) {
        if (!(*mAttributes)[i + 2].Compare(name)
            && (ns.IsNull() || !(*mAttributes)[i].Compare(ns))) {
            *attrValue = (*mAttributes)[i + 3];
            return NOERROR;
        }
    }

    *attrValue = NULL;
    return NOERROR;
}

ECode CXmlPullParser::GetEventType(
    /* [out] */ Int32* type)
{
    if (type == NULL) return E_INVALID_ARGUMENT;

    *type = mType;
    return NOERROR;
}

ECode CXmlPullParser::Next(
    /* [out] */ Int32* event)
{
    if (event == NULL) return E_INVALID_ARGUMENT;

    mTxtPos = 0;
    mIsWhitespace = TRUE;
    Int32 minType = 9999;
    mToken = FALSE;
    Int32 type;

    do {
        FAIL_RETURN(NextImpl());
        if (mType < minType) minType = mType;
        //    if (curr <= IXmlPullParser_TEXT) type = curr;

        FAIL_RETURN(PeekType(&type));
    }
    while (minType > IXmlPullParser_ENTITY_REF // ignorable
        || (minType >= IXmlPullParser_TEXT && type >= IXmlPullParser_TEXT));

    mType = minType;
    if (mType > IXmlPullParser_TEXT) mType = IXmlPullParser_TEXT;

    *event = mType;
    return NOERROR;
}

ECode CXmlPullParser::NextToken(
    /* [out] */ Int32* token)
{
    if (token == NULL) return E_INVALID_ARGUMENT;

    mIsWhitespace = TRUE;
    mTxtPos = 0;

    mToken = TRUE;
    FAIL_RETURN(NextImpl());
    *token = mType;
    return NOERROR;
}

ECode CXmlPullParser::NextTag(
    /* [out] */ Int32* tag)
{
    if (tag == NULL) return E_INVALID_ARGUMENT;

    Int32 event;
    FAIL_RETURN(Next(&event));
    if (mType == IXmlPullParser_TEXT && mIsWhitespace) {
        FAIL_RETURN(Next(&event));
    }

    if (mType != IXmlPullParser_END_TAG && mType != IXmlPullParser_START_TAG) {
        return Exception("unexpected type");
    }

    *tag = mType;
    return NOERROR;
}

ECode CXmlPullParser::Require(
    /* [in] */ Int32 type,
    /* [in] */ const String& ns,
    /* [in] */ const String& name)
{
    String _ns, _name;
    GetNamespace(&_ns);
    GetName(&_name);
    if (type != mType
            || (!ns.IsNull() && ns.Compare(_ns))
            || (!name.IsNull() && name.Compare(_name))) {
        StringBuffer sb("expected: ");
        sb += TYPES[type] + " {" + mNamespace + "}" + mName;
        return Exception(sb);
    }
    return NOERROR;
}

ECode CXmlPullParser::NextText(
    /* [out] */ String* text)
{
    if (text == NULL) return E_INVALID_ARGUMENT;

    if (mType != IXmlPullParser_START_TAG) {
        return Exception("precondition: IXmlPullParser_START_TAG");
    }

    Int32 event;
    FAIL_RETURN(Next(&event));

    if (mType == IXmlPullParser_TEXT) {
        GetText(text);
        FAIL_RETURN(Next(&event));
    }
    else *text = "";

    if (mType != IXmlPullParser_END_TAG) {
        return Exception("IXmlPullParser_END_TAG expected");
    }

    return NOERROR;
}

ECode CXmlPullParser::SetFeature(
    /* [in] */ const String& name,
    /* [in] */ Boolean value)
{
    if (!String(IXmlPullParser_FEATURE_PROCESS_NAMESPACES).Compare(name)) {
        mProcessNsp = value;
        return NOERROR;
    }
    else if (IsProp(name, FALSE, "relaxed")) {
        mRelaxed = value;
        return NOERROR;
    }
    else {
        String error("unsupported feature: ");
        error += name;
        return Exception(error);
    }

}

ECode CXmlPullParser::SetProperty(
    /* [in] */ const String& name,
    /* [in] */ IInterface* value)
{
    if(IsProp(name, TRUE, "location")) {
        mLocation = value;
        return NOERROR;
    } else {
        String error("unsupported property: ");
        error += name;
        return Exception(error);
    }
}
