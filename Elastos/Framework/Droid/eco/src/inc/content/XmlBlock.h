
#ifndef __HH_XMLBLOCK_H
#define __HH_XMLBLOCK_H

#include <utils/ResourceTypes.h>
#include "ext/frameworkext.h"
#include "utils/ElRefBase.h"
#include "content/StringBlock.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos.h>

using namespace Elastos;
using namespace Elastos::System::Threading;

class XmlBlock
{
public:
    static const String TAG;

    XmlBlock(
        /* [in] */ const ArrayOf<Byte>& data);

    XmlBlock(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 size);

    /**
     * Create from an existing xml block native object.  This is
     * -extremely- dangerous -- only use it if you absolutely know what you
     *  are doing!  The given native object must exist for the entire lifetime
     *  of this newly creating XmlBlock.
     */
    /*package*/ XmlBlock(
        /* [in] */ IAssetManager* assets,
        /* [in] */ android::ResXMLTree* xmlBlock);

    ~XmlBlock();

    CARAPI_(void) Close();

    CARAPI NewParser(
        /* [out] */ IXmlResourceParser** parser);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

public: /*package*/
    class Parser :
        public ElRefBase,
        public IXmlResourceParser,
        public IAttributeSet
    {
    public:
        Parser(
            /* [in]  */ android::ResXMLParser* parse,
            /* [in]  */ XmlBlock* block);

        virtual ~Parser();

    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    public:
        CARAPI SetFeature(
            /* [in] */ String name,
            /* [in] */ Boolean state);

        CARAPI GetFeature(
            /* [in] */ String name,
            /* [out] */ Boolean * value);

        CARAPI SetProperty(
            /* [in] */ String name,
            /* [in] */ IInterface * value);

        CARAPI GetProperty(
            /* [in] */ String name,
            /* [out] */ IInterface ** value);

        CARAPI SetInput(
            /* [in] */ IInputStream * pInputStream,
            /* [in] */ String inputEncoding);

        CARAPI GetInputEncoding(
            /* [out] */ String * inputEncoding);

        CARAPI DefineEntityReplacementText(
            /* [in] */ String entityName,
            /* [in] */ String replacementText);

        CARAPI GetNamespaceCount(
            /* [in] */ Int32 depth,
            /* [out] */ Int32 * count);

        CARAPI GetNamespacePrefix(
            /* [in] */ Int32 pos,
            /* [out] */ String * prefix);

        CARAPI GetNamespaceUri(
            /* [in] */ Int32 pos,
            /* [out] */ String * uri);

        CARAPI GetNamespaceEx(
            /* [in] */ String prefix,
            /* [out] */ String * ns);

        CARAPI GetDepth(
            /* [out] */ Int32 * depth);

        CARAPI GetPositionDescription(
            /* [out] */ String * des);

        CARAPI GetLineNumber(
            /* [out] */ Int32 * number);

        CARAPI GetColumnNumber(
            /* [out] */ Int32 * number);

        CARAPI IsWhitespace(
            /* [out] */ Boolean * result);

        CARAPI GetText(
            /* [out] */ String * text);

        CARAPI GetNamespace(
            /* [out] */ String * ns);

        CARAPI GetName(
            /* [out] */ String * name);

        CARAPI GetPrefix(
            /* [out] */ String * prefix);

        CARAPI IsEmptyElementTag(
            /* [out] */ Boolean * result);

        CARAPI GetAttributeCount(
            /* [out] */ Int32 * attrCount);

        CARAPI GetAttributeNamespace(
            /* [in] */ Int32 index,
            /* [out] */ String * attrNamespace);

        CARAPI GetAttributeName(
            /* [in] */ Int32 index,
            /* [out] */ String * attrName);

        CARAPI GetAttributePrefix(
            /* [in] */ Int32 index,
            /* [out] */ String * attrPrefix);

        CARAPI GetAttributeType(
            /* [in] */ Int32 index,
            /* [out] */ String * attrType);

        CARAPI IsAttributeDefault(
            /* [in] */ Int32 index,
            /* [out] */ Boolean * result);

        CARAPI GetAttributeValue(
            /* [in] */ Int32 index,
            /* [out] */ String * attrValue);

        CARAPI GetAttributeValueEx(
            /* [in] */ String ns,
            /* [in] */ String name,
            /* [out] */ String * attrValue);

        CARAPI GetEventType(
            /* [out] */ Int32 * type);

        CARAPI Next(
            /* [out] */ Int32 * event);

        CARAPI NextToken(
            /* [out] */ Int32 * token);

        CARAPI Require(
            /* [in] */ Int32 type,
            /* [in] */ String ns,
            /* [in] */ String name);

        CARAPI NextText(
            /* [out] */ String * text);

        CARAPI NextTag(
            /* [out] */ Int32 * tag);

        CARAPI Close();

    public:
        CARAPI GetAttributeNameResource(
            /* [in] */ Int32 index,
            /* [out] */ Int32 * resId);

        CARAPI GetAttributeListValue(
            /* [in] */ String ns,
            /* [in] */ String attribute,
            /* [in] */ const ArrayOf<String> & options,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeBooleanValue(
            /* [in] */ String ns,
            /* [in] */ String attribute,
            /* [in] */ Boolean defaultValue,
            /* [out] */ Boolean * value);

        CARAPI GetAttributeResourceValue(
            /* [in] */ String ns,
            /* [in] */ String attribute,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeIntValue(
            /* [in] */ String ns,
            /* [in] */ String attribute,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeUnsignedIntValue(
            /* [in] */ String ns,
            /* [in] */ String attribute,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeFloatValue(
            /* [in] */ String ns,
            /* [in] */ String attribute,
            /* [in] */ Float defaultValue,
            /* [out] */ Float * value);

        CARAPI GetAttributeListValueEx(
            /* [in] */ Int32 idx,
            /* [in] */ const ArrayOf<String> & options,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeBooleanValueEx(
            /* [in] */ Int32 idx,
            /* [in] */ Boolean defaultValue,
            /* [out] */ Boolean * value);

        CARAPI GetAttributeResourceValueEx(
            /* [in] */ Int32 idx,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeIntValueEx(
            /* [in] */ Int32 idx,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeUnsignedIntValueEx(
            /* [in] */ Int32 idx,
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetAttributeFloatValueEx(
            /* [in] */ Int32 idx,
            /* [in] */ Float defaultValue,
            /* [out] */ Float * value);

        CARAPI GetIdAttribute(
            /* [out] */ String * attr);

        CARAPI GetClassAttribute(
            /* [out] */ String * attr);

        CARAPI GetIdAttributeResourceValue(
            /* [in] */ Int32 defaultValue,
            /* [out] */ Int32 * value);

        CARAPI GetStyleAttribute(
            /* [out] */ Int32 * value);

    public:
        CARAPI GetPooledString(
            /* [in] */ Int32 id,
            /* [out] */ ICharSequence** csq);

    public:
        /*package*/ android::ResXMLParser* mParseState;

    private:
        XmlBlock*   mBlock;
        Boolean     mStarted;
        Boolean     mDecNextDepth;
        Int32       mDepth;
        Int32       mEventType;
    };

    private:
        CARAPI_(void) DecOpenCountLocked();

        CARAPI_(android::ResXMLTree*) NativeCreate(
            /* [in] */ const ArrayOf<Byte>& data,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 size);

        CARAPI_(const android::ResStringPool*) NativeGetStringBlock(
            /* [in] */ android::ResXMLTree* xmlTree);

        CARAPI NativeCreateParseState(
            /* [in] */ android::ResXMLTree* xmlTree,
            /* [out] */ android::ResXMLParser** parser);

        CARAPI_(Int32) NativeNext(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetNamespace(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetName(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetText(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetLineNumber(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetAttributeCount(
            /* [in] */ android::ResXMLParser* parsere);

        CARAPI_(Int32) NativeGetAttributeNamespace(
            /* [in] */ android::ResXMLParser* parser,
            /* [in] */ Int32 idx);

        CARAPI_(Int32) NativeGetAttributeName(
            /* [in] */ android::ResXMLParser* parser,
            /* [in] */ Int32 idx);

        CARAPI_(Int32) NativeGetAttributeResource(
            /* [in] */ android::ResXMLParser* parser,
            /* [in] */ Int32 idx);

        CARAPI_(Int32) NativeGetAttributeDataType(
            /* [in] */ android::ResXMLParser* parser,
            /* [in] */ Int32 idx);

        CARAPI_(Int32) NativeGetAttributeData(
            /* [in] */ android::ResXMLParser* parser,
            /* [in] */ Int32 idx);

        CARAPI_(Int32) NativeGetAttributeStringValue(
            /* [in] */ android::ResXMLParser* parser,
            /* [in] */ Int32 idx);

        CARAPI_(Int32) NativeGetIdAttribute(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetClassAttribute(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetStyleAttribute(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(Int32) NativeGetAttributeIndex(
            /* [in] */ android::ResXMLParser* parser,
            /* [in] */ String ns,
            /* [in] */ String name);

        CARAPI_(void) NativeDestroyParseState(
            /* [in] */ android::ResXMLParser* parser);

        CARAPI_(void) NativeDestroy(
            /* [in] */ android::ResXMLTree* xmlTree);

    private:
        static const Boolean DEBUG = FALSE;

        AutoPtr<IAssetManager>  mAssets;
        android::ResXMLTree*    mNative;
        StringBlock*            mStrings;
        Boolean                 mOpen;
        Int32                   mOpenCount;

        Mutex mSyncLock;
};

#endif // __HH_XMLBLOCK_H
