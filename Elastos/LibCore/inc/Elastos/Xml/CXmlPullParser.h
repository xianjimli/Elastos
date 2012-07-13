
#ifndef __CXMLPULLPARSER_H__
#define __CXMLPULLPARSER_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CXmlPullParser.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>


CarClass(CXmlPullParser)
{
private:
    static const String UNEXPECTED_EOF;
    static const String ILLEGAL_TYPE;
    static const Int32 LEGACY = 999;
    static const Int32 XML_DECL = 998;

public:
    CXmlPullParser();

    ~CXmlPullParser();

    CARAPI_(void) KeepNamespaceAttributes();

    CARAPI SetFeature(
        /* [in] */ const String& name,
        /* [in] */ Boolean state);

    CARAPI GetFeature(
        /* [in] */ const String& name,
        /* [out] */ Boolean* value);

    CARAPI SetProperty(
        /* [in] */ const String& name,
        /* [in] */ IInterface* value);

    CARAPI GetProperty(
        /* [in] */ const String& name,
        /* [out] */ IInterface** value);

    CARAPI SetInput(
        /* [in]*/ IReader* reader);

    CARAPI SetInputEx(
        /* [in] */ IInputStream* inputStream,
        /* [in] */ const String& inputEncoding);

    CARAPI GetInputEncoding(
        /* [out] */ String* inputEncoding);

    CARAPI DefineEntityReplacementText(
        /* [in] */ const String& entityName,
        /* [in] */ const String& replacementText);

    CARAPI GetNamespaceCount(
        /* [in] */ Int32 depth,
        /* [out] */ Int32* count);

    CARAPI GetNamespacePrefix(
        /* [in] */ Int32 pos,
        /* [out] */ String* prefix);

    CARAPI GetNamespaceUri(
        /* [in] */ Int32 pos,
        /* [out] */ String* uri);

    CARAPI GetNamespaceEx(
        /* [in] */ const String& prefix,
        /* [out] */ String* ns);

    CARAPI GetDepth(
        /* [out] */ Int32* depth);

    CARAPI GetPositionDescription(
        /* [out] */ String* des);

    CARAPI GetLineNumber(
        /* [out] */ Int32* number);

    CARAPI GetColumnNumber(
        /* [out] */ Int32* number);

    CARAPI IsWhitespace(
        /* [out] */ Boolean* result);

    CARAPI GetText(
        /* [out] */ String* text);

    CARAPI GetNamespace(
        /* [out] */ String* ns);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetPrefix(
        /* [out] */ String* prefix);

    CARAPI IsEmptyElementTag(
        /* [out] */ Boolean* result);

    CARAPI GetAttributeCount(
        /* [out] */ Int32* attrCount);

    CARAPI GetAttributeNamespace(
        /* [in] */ Int32 index,
        /* [out] */ String* attrNamespace);

    CARAPI GetAttributeName(
        /* [in] */ Int32 index,
        /* [out] */ String* attrName);

    CARAPI GetAttributePrefix(
        /* [in] */ Int32 index,
        /* [out] */ String* attrPrefix);

    CARAPI GetAttributeType(
        /* [in] */ Int32 index,
        /* [out] */ String* attrType);

    CARAPI IsAttributeDefault(
        /* [in] */ Int32 index,
        /* [out] */ Boolean* result);

    CARAPI GetAttributeValue(
        /* [in] */ Int32 index,
        /* [out] */ String* attrValue);

    CARAPI GetAttributeValueEx(
        /* [in] */ CString ns,
        /* [in] */ CString name,
        /* [out] */ String* attrValue);

    CARAPI GetEventType(
        /* [out] */ Int32* type);

    CARAPI Next(
        /* [out] */ Int32* event);

    CARAPI NextToken(
        /* [out] */ Int32* token);

    CARAPI Require(
        /* [in] */ Int32 type,
        /* [in] */ const String& ns,
        /* [in] */ const String& name);

    CARAPI NextText(
        /* [out] */ String* text);

    CARAPI NextTag(
        /* [out] */ Int32* tag);

public:
    /**
     * This array can be used to convert the event type integer constants
     * such as START_TAG or TEXT to
     * to a string. For example, the value of TYPES[START_TAG] is
     * the string "START_TAG".
     *
     * This array is intended for diagnostic output only. Relying
     * on the contents of the array may be dangerous since malicious
     * applications may alter the array, although it is final, due
     * to limitations of the Java language.
     */
    static const Int32 TYPES_LENGTH = 11;
    static const String TYPES[TYPES_LENGTH];

private:
    CARAPI_(Boolean) IsProp(
        /* [in] */ const String& n1,
        /* [in] */ Boolean prop,
        /* [in] */ const char* n2);

    CARAPI AdjustNsp(
        /* [in] */ Boolean* result);

    CARAPI_(void) EnsureCapacity(
        /* [in] */ ArrayOf<String>* oldArr,
        /* [in] */ Int32 required,
        /* [out] */ ArrayOf<String>** newArr);

    CARAPI Exception(
        /* [in] */ const char* desc);

    CARAPI NextImpl();

    CARAPI ParseLegacy(
        /* [in] */ Boolean push,
        /* [out] */ Int32* value);

    CARAPI ParseDoctype(
        /* [in] */ Boolean push);

    CARAPI ParseEndTag();

    CARAPI PeekType(
        /* [out] */ Int32* type);

    CARAPI_(void) Get(
        /* [in] */ Int32 pos,
        /* [out] */ String* str);

    CARAPI_(void) Push(
        /* [in] */ Int32 c);

    CARAPI ParseStartTag(
        /* [in] */ Boolean xmldecl);

    CARAPI PushEntity();

    CARAPI PushText(
        /* [in] */ Int32 delimiter,
        /* [in] */ Boolean resolveEntities,
        /* [in] */ Boolean inAttributeValue);

    CARAPI Read(
        /* [in] */ Char8 c);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI Peek(
        /* [in] */ Int32 pos,
        /* [out] */ Int32* value);

    CARAPI ReadName(
        /* [out] */ String* name);

    CARAPI Skip();

private:
    AutoPtr<IInterface> mLocation;

    // general

    String mVersion;
    Boolean mStandalone;

    Boolean mProcessNsp;
    Boolean mRelaxed;
    Boolean mKeepNamespaceAttributes; // android-added
    HashMap<String, String>* mEntityMap;
    Int32 mDepth;
    ArrayOf<String>* mElementStack;
    ArrayOf<String>* mNspStack;
    ArrayOf<Int32>* mNspCounts;

    // source

    AutoPtr<IReader> mReader;
    String mEncoding;
    ArrayOf<Char8>* mSrcBuf;

    Int32 mSrcPos;
    Int32 mSrcCount;

    Int32 mLine;
    Int32 mColumn;

    // txtbuffer

    /** Target buffer for storing incoming text (including aggregated resolved entities) */
    ArrayOf<Char8>* mTxtBuf;
    /** Write position  */
    Int32 mTxtPos;

    // Event-related

    Int32 mType;
    Boolean mIsWhitespace;
    String mNamespace;
    String mPrefix;
    String mName;

    Boolean mDegenerated;
    Int32 mAttributeCount;

    /**
     * The current element's attributes arranged in groups of 4:
     * i + 0 = attribute namespace URI
     * i + 1 = attribute namespace prefix
     * i + 2 = attribute qualified name (may contain ":", as in "html:h1")
     * i + 3 = attribute value
     */
    ArrayOf<String>* mAttributes;
//    private int stackMismatch = 0;
    String mError;

    /**
     * A separate peek buffer seems simpler than managing
     * wrap around in the first level read buffer */

    Int32 mPeek[2];
    Int32 mPeekCount;
    Boolean mWasCR;

    Boolean mUnresolved;
    Boolean mToken;
};

#endif // __CXMLPULLPARSER_H__
