
#include "content/XmlBlock.h"
#include <content/CAssetManager.h>
#include <utils/CTypedValue.h>
#include <utils/XmlUtils.h>
#include <elastos/Math.h>
#include <Logger.h>
#include <StringBuffer.h>
#include <assert.h>
#include <elastos/AutoFree.h>

#define CASSETMANAGER(x) ((CAssetManager*)x.Get())

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const char* XmlBlock::TAG = "XmlBlock";
const Boolean XmlBlock::DEBUG;

XmlBlock::XmlBlock(
    /* [in] */ const ArrayOf<Byte>& data)
    : mOpen(TRUE)
    , mOpenCount(1)
{
    mNative = NativeCreate(data, 0, data.GetLength());
    mStrings = new StringBlock(NativeGetStringBlock(mNative), FALSE);
    assert(mStrings);
}

XmlBlock::XmlBlock(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size)
    : mNative(0)
    , mStrings(NULL)
    , mOpen(TRUE)
    , mOpenCount(1)
{
    mNative = NativeCreate(data, offset, size);
    mStrings = new StringBlock(NativeGetStringBlock(mNative), FALSE);
    assert(mStrings);
}

XmlBlock::~XmlBlock()
{
    delete mStrings;
}

PInterface XmlBlock::Probe(
    /* [in]  */ REIID riid)
{
    return NULL;
}

UInt32 XmlBlock::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 XmlBlock::Release()
{
    return ElRefBase::Release();
}

ECode XmlBlock::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void XmlBlock::Close()
{
    Mutex::Autolock lock(mSyncLock);

    if (mOpen) {
        mOpen = FALSE;
        DecOpenCountLocked();
    }
}

void XmlBlock::DecOpenCountLocked()
{
    mOpenCount--;
    if (mOpenCount == 0) {
        NativeDestroy(mNative);
        if (mAssets != NULL) {
//	            CASSETMANAGER(mAssets)->XmlBlockGone(HashCode());
        }
    }
}

ECode XmlBlock::NewParser(
    /* [out] */ IXmlResourceParser** parser)
{
    VALIDATE_NOT_NULL(parser);
    *parser = NULL;

    Mutex::Autolock lock(mSyncLock);

    if (mNative != 0) {
        android::ResXMLParser* andParser;
        ASSERT_SUCCEEDED(NativeCreateParseState(mNative, &andParser));
        Parser* p = new Parser(andParser, this);
        if (!p) return E_OUT_OF_MEMORY;
        *parser = (IXmlResourceParser*)p->Probe(EIID_IXmlResourceParser);
        (*parser)->AddRef();
        return NOERROR;
    }
    return E_FAIL;
}

XmlBlock::Parser::Parser(
    /* [in]  */ android::ResXMLParser* parseState,
    /* [in]  */ XmlBlock* block)
    : mParseState(parseState)
    , mBlock(block)
    , mStarted(FALSE)
    , mDecNextDepth(FALSE)
    , mDepth(0)
    , mEventType(IXmlPullParser_START_DOCUMENT)
{
    assert(block);
    block->mOpenCount++;
}

XmlBlock::Parser::~Parser()
{
}

UInt32 XmlBlock::Parser::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 XmlBlock::Parser::Release()
{
    return ElRefBase::Release();
}

PInterface XmlBlock::Parser::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IXmlResourceParser*)this;
    }
    else if (riid == EIID_IXmlResourceParser) {
        return (IXmlResourceParser*)this;
    }
    else if (riid == EIID_IAttributeSet) {
        return (IAttributeSet*)this;
    }
    return NULL;
}

ECode XmlBlock::Parser::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IXmlResourceParser*)this) {
        *pIID = EIID_IXmlResourceParser;
    }
    else if (pObject == (IInterface*)(IAttributeSet*)this) {
        *pIID = EIID_IAttributeSet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode XmlBlock::Parser::SetFeature(
    /* [in] */ const String& name,
    /* [in] */ Boolean state)
{
    VALIDATE_STRING_NOT_NULL(name);

    if (name.Compare(IXmlPullParser_FEATURE_PROCESS_NAMESPACES) == 0 && state) {
        return NOERROR;
    }
    if (name.Compare(IXmlPullParser_FEATURE_REPORT_NAMESPACE_ATTRIBUTES) == 0 && state) {
        return NOERROR;
    }
    Logger::E(TAG, StringBuffer("Unsupported feature: ") + name);
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlBlock::Parser::GetFeature(
    /* [in] */ const String& name,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(name);

    if (name.Compare(IXmlPullParser_FEATURE_PROCESS_NAMESPACES) == 0) {
        *value = TRUE;
        return NOERROR;
    }
    if (name.Compare(IXmlPullParser_FEATURE_REPORT_NAMESPACE_ATTRIBUTES) == 0) {
        *value = TRUE;
        return NOERROR;
    }
    *value = FALSE;
    return NOERROR;
}

ECode XmlBlock::Parser::SetProperty(
    /* [in] */ const String& name,
    /* [in] */ IInterface * value)
{
    Logger::E(TAG, "setProperty() not supported");
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlBlock::Parser::GetProperty(
    /* [in] */ const String& name,
    /* [out] */ IInterface ** value)
{
    VALIDATE_STRING_NOT_NULL(name);
    VALIDATE_NOT_NULL(value);

    *value = NULL;
    return NOERROR;
}

ECode XmlBlock::Parser::SetInput(
    /* [in]*/ IReader* reader)
{
    return E_NOT_IMPLEMENTED;
}

ECode XmlBlock::Parser::SetInputEx(
    /* [in] */ IInputStream * inputStream,
    /* [in] */ const String& inputEncoding)
{
    Logger::E(TAG, "setInput() not supported");
    return NOERROR;
}

ECode XmlBlock::Parser::DefineEntityReplacementText(
    /* [in] */ const String& entityName,
    /* [in] */ const String& replacementText)
{
    Logger::E(TAG, "defineEntityReplacementText() not supported");
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlBlock::Parser::GetNamespacePrefix(
    /* [in] */ Int32 pos,
    /* [out] */ String * prefix)
{
    Logger::E(TAG, "getNamespacePrefix() not supported");
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlBlock::Parser::GetInputEncoding(
    /* [out] */ String * inputEncoding)
{
    VALIDATE_NOT_NULL(inputEncoding);

    *inputEncoding = NULL;
    return NOERROR;
}

ECode XmlBlock::Parser::GetNamespaceEx(
    /* [in] */ const String& prefix,
    /* [out] */ String * ns)
{
    Logger::E(TAG, "getNamespace() not supported");
    return E_RUNTIME_EXCEPTION;
}

ECode XmlBlock::Parser::GetNamespaceCount(
    /* [in] */ Int32 depth,
    /* [out] */ Int32 * count)
{
    Logger::E(TAG, "getNamespaceCount() not supported");
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlBlock::Parser::GetPositionDescription(
    /* [out] */ String * des)
{
    VALIDATE_NOT_NULL(des);

    StringBuffer buf("Binary XML file line #");
    Int32 line = 0;
    GetLineNumber(&line);
    buf += line;
    *des = (const char*)buf;
    return NOERROR;
}

ECode XmlBlock::Parser::GetNamespaceUri(
    /* [in] */ Int32 pos,
    /* [out] */ String * uri)
{
    Logger::E(TAG, "getNamespaceUri() not supported");
    return E_XML_PULL_PARSER_EXCEPTION;
}

ECode XmlBlock::Parser::GetColumnNumber(
    /* [out] */ Int32 * number)
{
    VALIDATE_NOT_NULL(number);

    *number = -1;
    return NOERROR;
}

ECode XmlBlock::Parser::GetDepth(
    /* [out] */ Int32 * depth)
{
    VALIDATE_NOT_NULL(depth);

    *depth = mDepth;
    return NOERROR;
}

ECode XmlBlock::Parser::GetText(
    /* [out] */ String * text)
{
    VALIDATE_NOT_NULL(text);

    Int32 id = mBlock->NativeGetText(mParseState);
    if (id < 0) {
        *text = NULL;
        return NOERROR;
    }
    AutoPtr<ICharSequence> csq;
    FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
    return csq->ToString(text);
}

ECode XmlBlock::Parser::GetLineNumber(
    /* [out] */ Int32 * number)
{
    VALIDATE_NOT_NULL(number);

    *number = mBlock->NativeGetLineNumber(mParseState);
    return NOERROR;
}

ECode XmlBlock::Parser::GetEventType(
    /* [out] */ Int32 * type)
{
    VALIDATE_NOT_NULL(type);

    *type = mEventType;
    return NOERROR;
}

ECode XmlBlock::Parser::IsWhitespace(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);

    // whitespace was stripped by aapt.
    *result = FALSE;
    return NOERROR;
}

ECode XmlBlock::Parser::GetPrefix(
    /* [out] */ String * prefix)
{
    Logger::E(TAG, "getPrefix not supported");
    return E_RUNTIME_EXCEPTION;
}

ECode XmlBlock::Parser::GetNamespace(
    /* [out] */ String * ns)
{
    VALIDATE_NOT_NULL(ns);

    Int32 id = mBlock->NativeGetNamespace(mParseState);
    if (id < 0) {
        *ns = NULL;
        return NOERROR;
    }
    AutoPtr<ICharSequence> csq;
    FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
    return csq->ToString(ns);
}

ECode XmlBlock::Parser::GetName(
    /* [out] */ String * name)
{
    VALIDATE_NOT_NULL(name);

    Int32 id = mBlock->NativeGetName(mParseState);
    if (id < 0) {
        *name = NULL;
        return NOERROR;
    }
    AutoPtr<ICharSequence> csq;
    FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
    return csq->ToString(name);
}

ECode XmlBlock::Parser::GetAttributeNamespace(
    /* [in] */ Int32 index,
    /* [out] */ String * attrNamespace)
{
    VALIDATE_NOT_NULL(attrNamespace);

    Int32 id = mBlock->NativeGetAttributeNamespace(mParseState, index);
//	    if (DEBUG) System.out.println("getAttributeNamespace of " + index + " = " + id);
    if (id >= 0) {
        AutoPtr<ICharSequence> csq;
        FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
        return csq->ToString(attrNamespace);
    }
    else if (id == -1) {
        *attrNamespace = "";
        return NOERROR;
    }
    Logger::E(TAG, StringBuffer("") + index);
    return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode XmlBlock::Parser::GetAttributeName(
    /* [in] */ Int32 index,
    /* [out] */ String * attrName)
{
    VALIDATE_NOT_NULL(attrName);

    Int32 id = mBlock->NativeGetAttributeName(mParseState, index);
//	    if (DEBUG) System.out.println("getAttributeName of " + index + " = " + id);
    if (id >= 0) {
        AutoPtr<ICharSequence> csq;
        FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
        return csq->ToString(attrName);
    }
    Logger::E(TAG, StringBuffer("") + index);
    return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode XmlBlock::Parser::GetAttributePrefix(
    /* [in] */ Int32 index,
    /* [out] */ String * attrPrefix)
{
    Logger::E(TAG, "getAttributePrefix not supported");
    return E_RUNTIME_EXCEPTION;
}

ECode XmlBlock::Parser::IsEmptyElementTag(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);

    // XXX Need to detect this.
    *result = FALSE;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeCount(
    /* [out] */ Int32 * attrCount)
{
    VALIDATE_NOT_NULL(attrCount);

    *attrCount = mEventType == IXmlPullParser_START_TAG
        ? mBlock->NativeGetAttributeCount(mParseState) : -1;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeValue(
    /* [in] */ Int32 index,
    /* [out] */ String * attrValue)
{
    VALIDATE_NOT_NULL(attrValue);

    Int32 id = mBlock->NativeGetAttributeStringValue(mParseState, index);
//	    if (DEBUG) System.out.println("getAttributeValue of " + index + " = " + id);
    if (id >= 0) {
        AutoPtr<ICharSequence> csq;
        FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
        return csq->ToString(attrValue);
    }

    // May be some other type...  check and try to convert if so.
    Int32 t = mBlock->NativeGetAttributeDataType(mParseState, index);
    if (t == TypedValue_TYPE_NULL) {
        Logger::E(TAG, StringBuffer("") + index);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 v = mBlock->NativeGetAttributeData(mParseState, index);
    return CTypedValue::CoerceToStringEx(t, v, attrValue);
}

ECode XmlBlock::Parser::GetAttributeType(
    /* [in] */ Int32 index,
    /* [out] */ String * attrType)
{
    VALIDATE_NOT_NULL(attrType);

    *attrType = "CDATA";
    return NOERROR;
}

ECode XmlBlock::Parser::IsAttributeDefault(
    /* [in] */ Int32 index,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}

ECode XmlBlock::Parser::NextToken(
    /* [out] */ Int32 * token)
{
    VALIDATE_NOT_NULL(token);

    return Next(token);
}

ECode XmlBlock::Parser::GetAttributeValueEx(
    /* [in] */ CString ns,
    /* [in] */ CString name,
    /* [out] */ String * attrValue)
{
    VALIDATE_NOT_NULL(attrValue);

    Int32 idx = mBlock->NativeGetAttributeIndex(mParseState, ns, name);
    if (idx >= 0) {
//	        if (DEBUG) System.out.println("getAttributeName of "
//	                + namespace + ":" + name + " index = " + idx);
//	        if (DEBUG) System.out.println(
//	                "Namespace=" + getAttributeNamespace(idx)
//	                + "Name=" + getAttributeName(idx)
//	                + ", Value=" + getAttributeValue(idx));
        return GetAttributeValue(idx, attrValue);
    }
    *attrValue = NULL;
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode XmlBlock::Parser::Next(
    /* [out] */ Int32 * event)
{
    VALIDATE_NOT_NULL(event);

    if (!mStarted) {
        mStarted = true;
        *event = IXmlPullParser_START_DOCUMENT;
        return NOERROR;
    }
    if (mParseState == 0) {
        *event = IXmlPullParser_END_DOCUMENT;
        return NOERROR;
    }
    Int32 ev = mBlock->NativeNext(mParseState);
    if (mDecNextDepth) {
        mDepth--;
        mDecNextDepth = FALSE;
    }
    switch (ev) {
    case IXmlPullParser_START_TAG:
        mDepth++;
        break;
    case IXmlPullParser_END_TAG:
        mDecNextDepth = TRUE;
        break;
    }
    mEventType = ev;
    if (ev == IXmlPullParser_END_DOCUMENT) {
        // Automatically close the parse when we reach the end of
        // a document, since the standard XmlPullParser interface
        // doesn't have such an API so most clients will leave us
        // dangling.
        Close();
    }
    *event = ev;
    return NOERROR;
}

ECode XmlBlock::Parser::Require(
    /* [in] */ Int32 type,
    /* [in] */ const String& ns,
    /* [in] */ const String& name)
{
    VALIDATE_STRING_NOT_NULL(ns);
    VALIDATE_STRING_NOT_NULL(name);

    Int32 t = 0;
    String n;
    String na;
    GetEventType(&t);
    GetNamespace(&n);
    GetName(&na);
    if ( type != t
        || (!ns.IsNull() && ns.Compare(n) != 0)
        || (!name.IsNull() && name.Compare(na) != 0) ) {
        String des;
        GetPositionDescription(&des);
        Logger::E(TAG, StringBuffer("expected ") + /*IXmlPullParser_TYPES[type] + */ des);
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    return NOERROR;
}

ECode XmlBlock::Parser::NextText(
    /* [out] */ String * text)
{
    VALIDATE_NOT_NULL(text);

    Int32 t = 0;
    GetEventType(&t);
    if(t != IXmlPullParser_START_TAG) {
        String des;
        GetPositionDescription(&des);
        Logger::E(TAG, StringBuffer(des) + ": parser must be on START_TAG to read next text");
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    Int32 eventType = 0;
    Next(&eventType);
    if(eventType == IXmlPullParser_TEXT) {
        String result;
        GetText(&result);
        Next(&eventType);
        if(eventType != IXmlPullParser_END_TAG) {
            String des;
            GetPositionDescription(&des);
            Logger::E(TAG, StringBuffer(des) + ": event TEXT it must be immediately followed by END_TAG");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        *text = result;
        return NOERROR;
    }
    else if(eventType == IXmlPullParser_END_TAG) {
       *text = "";
       return NOERROR;
    }
    else {
        String des;
        GetPositionDescription(&des);
        Logger::E(TAG, StringBuffer(des) + ": parser must be on START_TAG or TEXT to read text");
        return E_XML_PULL_PARSER_EXCEPTION;
    }
}

ECode XmlBlock::Parser::NextTag(
    /* [out] */ Int32 * tag)
{
    VALIDATE_NOT_NULL(tag);

    Int32 eventType = 0;
    Next(&eventType);
    Boolean isWhitespace;
    IsWhitespace(&isWhitespace);
    if(eventType == IXmlPullParser_TEXT && isWhitespace) {   // skip whitespace
        Next(&eventType);
    }
    if (eventType != IXmlPullParser_START_TAG && eventType != IXmlPullParser_END_TAG) {
        String des;
        GetPositionDescription(&des);
        Logger::E(TAG, StringBuffer(des) + ": expected start or end tag");
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    *tag = eventType;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeNameResource(
    /* [in] */ Int32 index,
    /* [out] */ Int32 * resId)
{
    VALIDATE_NOT_NULL(resId);

    *resId = mBlock->NativeGetAttributeResource(mParseState, index);
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeListValue(
    /* [in] */ const String& ns,
    /* [in] */ const String& attribute,
    /* [in] */ const ArrayOf<String> & options,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 idx = mBlock->NativeGetAttributeIndex(mParseState, ns, attribute);
    if (idx >= 0) {
        return GetAttributeListValueEx(idx, options, defaultValue, value);
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeBooleanValue(
    /* [in] */ const String& ns,
    /* [in] */ const String& attribute,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 idx = mBlock->NativeGetAttributeIndex(mParseState, ns, attribute);
    if (idx >= 0) {
        return GetAttributeBooleanValueEx(idx, defaultValue, value);
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeResourceValue(
    /* [in] */ const String& ns,
    /* [in] */ const String& attribute,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 idx = mBlock->NativeGetAttributeIndex(mParseState, ns, attribute);
    if (idx >= 0) {
        return GetAttributeResourceValueEx(idx, defaultValue, value);
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeIntValue(
    /* [in] */ const String& ns,
    /* [in] */ const String& attribute,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 idx = mBlock->NativeGetAttributeIndex(mParseState, ns, attribute);
    if (idx >= 0) {
        return GetAttributeIntValueEx(idx, defaultValue, value);
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeUnsignedIntValue(
    /* [in] */ const String& ns,
    /* [in] */ const String& attribute,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 idx = mBlock->NativeGetAttributeIndex(mParseState, ns, attribute);
    if (idx >= 0) {
        return GetAttributeUnsignedIntValueEx(idx, defaultValue, value);
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeFloatValue(
    /* [in] */ const String& ns,
    /* [in] */ const String& attribute,
    /* [in] */ Float defaultValue,
    /* [out] */ Float * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 idx = mBlock->NativeGetAttributeIndex(mParseState, ns, attribute);
    if (idx >= 0) {
        return GetAttributeFloatValueEx(idx, defaultValue, value);
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeListValueEx(
    /* [in] */ Int32 idx,
    /* [in] */ const ArrayOf<String> & options,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 t = mBlock->NativeGetAttributeDataType(mParseState, idx);
    Int32 v = mBlock->NativeGetAttributeData(mParseState, idx);
    if (t == TypedValue_TYPE_STRING) {
        AutoPtr<ICharSequence> csq;
        mBlock->mStrings->Get(v, (ICharSequence**)&csq);
        *value = XmlUtils::ConvertValueToList(csq, options, defaultValue);
        return NOERROR;
    }
    *value = v;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeBooleanValueEx(
    /* [in] */ Int32 idx,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 t = mBlock->NativeGetAttributeDataType(mParseState, idx);
    // Note: don't attempt to convert any other types, because
    // we want to count on appt doing the conversion for us.
    if (t >= TypedValue_TYPE_FIRST_INT &&
        t <= TypedValue_TYPE_LAST_INT) {
        *value = mBlock->NativeGetAttributeData(mParseState, idx) != 0;
        return NOERROR;
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeResourceValueEx(
    /* [in] */ Int32 idx,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 t = mBlock->NativeGetAttributeDataType(mParseState, idx);
    // Note: don't attempt to convert any other types, because
    // we want to count on appt doing the conversion for us.
    if (t == TypedValue_TYPE_REFERENCE) {
        *value = mBlock->NativeGetAttributeData(mParseState, idx);
        return NOERROR;
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeIntValueEx(
    /* [in] */ Int32 idx,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 t = mBlock->NativeGetAttributeDataType(mParseState, idx);
    // Note: don't attempt to convert any other types, because
    // we want to count on appt doing the conversion for us.
    if (t >= TypedValue_TYPE_FIRST_INT &&
        t <= TypedValue_TYPE_LAST_INT) {
        *value = mBlock->NativeGetAttributeData(mParseState, idx);
        return NOERROR;
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeUnsignedIntValueEx(
    /* [in] */ Int32 idx,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 t = mBlock->NativeGetAttributeDataType(mParseState, idx);
    // Note: don't attempt to convert any other types, because
    // we want to count on appt doing the conversion for us.
    if (t >= TypedValue_TYPE_FIRST_INT &&
        t <= TypedValue_TYPE_LAST_INT) {
        *value = mBlock->NativeGetAttributeData(mParseState, idx);
        return NOERROR;
    }
    *value = defaultValue;
    return NOERROR;
}

ECode XmlBlock::Parser::GetAttributeFloatValueEx(
    /* [in] */ Int32 idx,
    /* [in] */ Float defaultValue,
    /* [out] */ Float * value)
{
    VALIDATE_NOT_NULL(value);

    Int32 t = mBlock->NativeGetAttributeDataType(mParseState, idx);
    // Note: don't attempt to convert any other types, because
    // we want to count on appt doing the conversion for us.
    if (t == TypedValue_TYPE_FLOAT) {
        return Math::Int32BitsToFloat(
            mBlock->NativeGetAttributeData(mParseState, idx));
    }
    Logger::E(TAG, "not a float!");
    return E_RUNTIME_EXCEPTION;
}

ECode XmlBlock::Parser::GetIdAttribute(
    /* [out] */ String * attr)
{
    VALIDATE_NOT_NULL(attr);

    Int32 id = mBlock->NativeGetIdAttribute(mParseState);
    if (id < 0) {
        *attr = NULL;
        return NOERROR;
    }
    AutoPtr<ICharSequence> csq;
    FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
    return csq->ToString(attr);
}

ECode XmlBlock::Parser::GetClassAttribute(
    /* [out] */ String * attr)
{
    VALIDATE_NOT_NULL(attr);

    Int32 id = mBlock->NativeGetClassAttribute(mParseState);
    if (id < 0) {
        *attr = NULL;
        return NOERROR;
    }
    AutoPtr<ICharSequence> csq;
    FAIL_RETURN(mBlock->mStrings->Get(id, (ICharSequence**)&csq));
    return csq->ToString(attr);
}

ECode XmlBlock::Parser::GetIdAttributeResourceValue(
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    //todo: create and use native method
    return GetAttributeResourceValue(String(NULL), String("id"), defaultValue, value);
}

ECode XmlBlock::Parser::GetStyleAttribute(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);

    *value = mBlock->NativeGetStyleAttribute(mParseState);
    return NOERROR;
}

ECode XmlBlock::Parser::Close()
{
    Mutex::Autolock lock(mBlock->mSyncLock);
    if (mParseState != 0) {
        mBlock->NativeDestroyParseState(mParseState);
        mParseState = 0;
        mBlock->DecOpenCountLocked();
    }
    return NOERROR;
}

ECode XmlBlock::Parser::GetPooledString(
    /* [in] */ Int32 id,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    return mBlock->mStrings->Get(id, csq);
}

XmlBlock::XmlBlock(
    /* [in] */ IAssetManager* assets,
    /* [in] */ android::ResXMLTree* xmlBlock)
    : mNative(0)
    , mStrings(NULL)
    , mOpen(TRUE)
    , mOpenCount(1)
{
    mAssets = assets;
    mNative = xmlBlock;
    mStrings = new StringBlock(NativeGetStringBlock(xmlBlock), FALSE);
    assert(mStrings);
}

android::ResXMLTree* XmlBlock::NativeCreate(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size)
{
    Int32 bLen = data.GetLength();
    if (offset < 0 || offset >= bLen || size < 0 || size > bLen || (offset + size) > bLen) {
//	        doThrow(env, "java/lang/IndexOutOfBoundsException");
        return NULL;
    }

    Byte* b = data.GetPayload();
    android::ResXMLTree* osb = new android::ResXMLTree(b + offset, size, TRUE);
    if (osb == NULL || osb->getError() != android::NO_ERROR) {
//	        doThrow(env, "java/lang/IllegalArgumentException");
        return NULL;
    }

    return osb;
}

const android::ResStringPool* XmlBlock::NativeGetStringBlock(
    /* [in] */ android::ResXMLTree* xmlTree)
{
    assert(xmlTree);

    return &xmlTree->getStrings();
}

ECode XmlBlock::NativeCreateParseState(
    /* [in] */ android::ResXMLTree* xmlTree,
    /* [out] */ android::ResXMLParser** parser)
{
    VALIDATE_NOT_NULL(xmlTree);
    VALIDATE_NOT_NULL(parser);

    *parser = new android::ResXMLParser(*xmlTree);
    if (*parser == NULL) {
//	        doThrow(env, "java/lang/OutOfMemoryError");
        return E_OUT_OF_MEMORY;
    }

    (*parser)->restart();

    return NOERROR;
}

Int32 XmlBlock::NativeNext(
    /* [in] */ android::ResXMLParser* parser)
{
    if (parser == NULL) {
        return android::ResXMLParser::END_DOCUMENT;
    }

    do {
        Int32 code = (Int32)parser->next();
        switch (code) {
            case android::ResXMLParser::START_TAG:
                return 2;
            case android::ResXMLParser::END_TAG:
                return 3;
            case android::ResXMLParser::TEXT:
                return 4;
            case android::ResXMLParser::START_DOCUMENT:
                return 0;
            case android::ResXMLParser::END_DOCUMENT:
                return 1;
            case android::ResXMLParser::BAD_DOCUMENT:
                goto bad;
        }
    } while (TRUE);

bad:
//	    doThrow(env, "org/xmlpull/v1/XmlPullParserException",
//	            "Corrupt XML binary file");
    return android::ResXMLParser::BAD_DOCUMENT;
}

Int32 XmlBlock::NativeGetNamespace(
    /* [in] */ android::ResXMLParser* parser)
{
    if (!parser) {
        return -1;
    }

    return (Int32)parser->getElementNamespaceID();
}

Int32 XmlBlock::NativeGetName(
    /* [in] */ android::ResXMLParser* parser)
{
    if (!parser) {
        return -1;
    }

    return (Int32)parser->getElementNameID();
}

Int32 XmlBlock::NativeGetText(
    /* [in] */ android::ResXMLParser* parser)
{
    if (!parser) {
        return -1;
    }

    return (Int32)parser->getTextID();
}

Int32 XmlBlock::NativeGetLineNumber(
    /* [in] */ android::ResXMLParser* parser)
{
    assert(parser);

    return (Int32)parser->getLineNumber();
}

Int32 XmlBlock::NativeGetAttributeCount(
    /* [in] */ android::ResXMLParser* parser)
{
    assert(parser);

    return (Int32)parser->getAttributeCount();
}

Int32 XmlBlock::NativeGetAttributeNamespace(
    /* [in] */ android::ResXMLParser* parser,
    /* [in] */ Int32 idx)
{
    assert(parser);

    return (Int32)parser->getAttributeNamespaceID(idx);
}

Int32 XmlBlock::NativeGetAttributeName(
    /* [in] */ android::ResXMLParser* parser,
    /* [in] */ Int32 idx)
{
    assert(parser);

    return (Int32)parser->getAttributeNameID(idx);
}

Int32 XmlBlock::NativeGetAttributeResource(
    /* [in] */ android::ResXMLParser* parser,
    /* [in] */ Int32 idx)
{
    assert(parser);

    return (Int32)parser->getAttributeNameResID(idx);
}

Int32 XmlBlock::NativeGetAttributeDataType(
    /* [in] */ android::ResXMLParser* parser,
    /* [in] */ Int32 idx)
{
    assert(parser);

    return (Int32)parser->getAttributeDataType(idx);
}

Int32 XmlBlock::NativeGetAttributeData(
    /* [in] */ android::ResXMLParser* parser,
    /* [in] */ Int32 idx)
{
    assert(parser);

    return (Int32)parser->getAttributeData(idx);
}

Int32 XmlBlock::NativeGetAttributeStringValue(
    /* [in] */ android::ResXMLParser* parser,
    /* [in] */ Int32 idx)
{
    assert(parser);

    return (Int32)parser->getAttributeValueStringID(idx);
}

Int32 XmlBlock::NativeGetIdAttribute(
    /* [in] */ android::ResXMLParser* parser)
{
    assert(parser);

    ssize_t idx = parser->indexOfID();
    return idx >= 0 ? (Int32)parser->getAttributeValueStringID(idx) : -1;
}

Int32 XmlBlock::NativeGetClassAttribute(
    /* [in] */ android::ResXMLParser* parser)
{
    assert(parser);

    ssize_t idx = parser->indexOfClass();
    return idx >= 0 ? (Int32)parser->getAttributeValueStringID(idx) : -1;
}

Int32 XmlBlock::NativeGetStyleAttribute(
    /* [in] */ android::ResXMLParser* parser)
{
    assert(parser);

    ssize_t idx = parser->indexOfStyle();
    if (idx < 0) {
        return 0;
    }

    android::Res_value value;
    if (parser->getAttributeValue(idx, &value) < 0) {
        return 0;
    }

    return value.dataType == value.TYPE_REFERENCE
        || value.dataType == value.TYPE_ATTRIBUTE
        ? value.data : 0;
}

Int32 XmlBlock::NativeGetAttributeIndex(
    /* [in] */ android::ResXMLParser* parser,
    /* [in] */ CString ns,
    /* [in] */ CString name)
{
    assert(parser);
    assert(!name.IsNull());

    return (Int32)parser->indexOfAttribute((const char*)ns, (const char*)name);
}

void XmlBlock::NativeDestroyParseState(
    /* [in] */ android::ResXMLParser* parser)
{
    assert(parser);

    delete parser;
}

void XmlBlock::NativeDestroy(
    /* [in] */ android::ResXMLTree* xmlTree)
{
    assert(xmlTree);

    delete xmlTree;
}

ECode XmlBlock::GetHashCode(
    /* [out] */ Int32* code)
{
    assert(code);

    *code = (Int32)this;
    return NOERROR;
}

