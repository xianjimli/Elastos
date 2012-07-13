
#ifndef __CXMLSERIALIZER_H__
#define __CXMLSERIALIZER_H__

#include "_CXmlSerializer.h"
#include <Elastos.IO.h>
#include <elastos/AutoPtr.h>

CarClass(CXmlSerializer)
{
public:
    CXmlSerializer();

    ~CXmlSerializer();

    CARAPI WriteDocDecl(
        /* [in] */ CString dd);

    CARAPI EndDocument();

    CARAPI WriteEntityRef(
        /* [in] */ CString text);

    CARAPI GetFeature(
        /* [in] */ CString name,
        /* [out] */ Boolean* value);

    CARAPI GetPrefix(
        /* [in] */ CString ns,
        /* [in] */ Boolean generatePrefix,
        /* [out] */ String* prefix);

    CARAPI GetProperty(
        /* [in] */ CString name,
        /* [out] */ IInterface** value);

    CARAPI WriteIgnorableWhitespace(
        /* [in] */ CString text);

    CARAPI SetFeature(
        /* [in] */ CString name,
        /* [in] */ Boolean state);

    CARAPI SetProperty(
        /* [in] */ CString name,
        /* [in] */ IInterface* value);

    CARAPI SetPrefix(
        /* [in] */ CString prefix,
        /* [in] */ CString ns);

    CARAPI SetOutputEx(
        /* [in] */ IWriter* writer);

    CARAPI SetOutput(
        /* [in] */ IOutputStream* os,
        /* [in] */ const String& encoding);

    CARAPI StartDocument(
        /* [in] */ const String& encoding,
        /* [in] */ Boolean standalone);

    CARAPI WriteStartTag(
        /* [in] */ CString ns,
        /* [in] */ CString name);

    CARAPI WriteAttribute(
        /* [in] */ CString ns,
        /* [in] */ CString name,
        /* [in] */ CString value);

    CARAPI Flush();

    CARAPI WriteEndTag(
        /* [in] */ CString ns,
        /* [in] */ CString name);

    CARAPI GetNamespace(
        /* [out] */ String* ns);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetDepth(
        /* [out] */ Int32* depth);

    CARAPI WriteText(
        /* [in] */ CString text);

    CARAPI WriteTextEx(
        /* [in] */ const ArrayOf<Char8>& buf,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len);

    CARAPI WriteCdSect(
        /* [in] */ CString text);

    CARAPI WriteComment(
        /* [in] */ CString text);

    CARAPI WriteProcessingInstruction(
        /* [in] */ CString text);

private:
    CARAPI Check(
        /* [in] */ Boolean close);

    CARAPI WriteEscaped(
        /* [in] */ CString s,
        /* [in] */ Int32 quot);

    // BEGIN android-added
    static CARAPI ReportInvalidCharacter(
        /* [in] */ Char32 ch);
    // END android-added

    CARAPI GetPrefix(
        /* [in] */ CString ns,
        /* [in] */ Boolean includeDefault,
        /* [in] */ Boolean create,
        /* [out] */ String* prefix);

private:
    //    static final String UNDEFINED = ":";

    // BEGIN android-added
    /** size (in characters) for the write buffer */
    static const Int32 WRITE_BUFFER_SIZE = 500;
    // END android-added

    // BEGIN android-changed
    // (Guarantee that the writer is always buffered.)
    AutoPtr<IBufferedWriter> mWriter;
    // END android-changed

    Boolean mPending;
    Int32 mAuto;
    Int32 mDepth;

    ArrayOf<String>* mElementStack;
    //nsp/prefix/name
    ArrayOf<Int32>* mNspCounts;
    ArrayOf<String>* mNspStack;
    //prefix/nsp; both empty are ""
    ArrayOf<Boolean>* mIndent;
    Boolean mUnicode;
    String mEncoding;
};

#endif //__CXMLSERIALIZER_H__
