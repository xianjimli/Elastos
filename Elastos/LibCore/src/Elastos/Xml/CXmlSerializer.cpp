
#include "cmdef.h"
#include "CXmlSerializer.h"
#include <StringBuffer.h>
#include <elastos/Character.h>

using namespace Elastos::Core;

const Int32 CXmlSerializer::WRITE_BUFFER_SIZE;

CXmlSerializer::CXmlSerializer()
{
    mElementStack = ArrayOf<String>::Alloc(12);
    mNspCounts = ArrayOf<Int32>::Alloc(4);
    mNspStack = ArrayOf<String>::Alloc(8);
    mIndent = ArrayOf<Boolean>::Alloc(4);
}

CXmlSerializer::~CXmlSerializer()
{
    for (Int32 i = 0; i < mElementStack->GetLength(); i++) {
        (*mElementStack)[i].~String();
    }
    ArrayOf<String>::Free(mElementStack);
    ArrayOf<Int32>::Free(mNspCounts);
    for (Int32 i = 0; i < mNspStack->GetLength(); i++) {
        (*mNspStack)[i].~String();
    }
    ArrayOf<String>::Free(mNspStack);
    ArrayOf<Boolean>::Free(mIndent);
}

ECode CXmlSerializer::Check(
    /* [in] */ Boolean close)
{
    if (!mPending) {
        return NOERROR;
    }

    mDepth++;
    mPending = FALSE;

    if (mIndent->GetLength() <= mDepth) {
        ArrayOf<Boolean>* hlp = ArrayOf<Boolean>::Alloc(mDepth + 4);
        memcpy(hlp->GetPayload(), mIndent->GetPayload(), mDepth * sizeof(Boolean));
        ArrayOf<Boolean>::Free(mIndent);
        mIndent = hlp;
    }
    (*mIndent)[mDepth] = (*mIndent)[mDepth - 1];

    for (Int32 i = (*mNspCounts)[mDepth - 1]; i < (*mNspCounts)[mDepth]; i++) {
        String ns;
        FAIL_RETURN(mWriter->Write(' '));
        FAIL_RETURN(mWriter->WriteString("xmlns"));
        if (!(*mNspStack)[i * 2].IsEmpty()) {
            FAIL_RETURN(mWriter->Write(':'));
            FAIL_RETURN(mWriter->WriteString((*mNspStack)[i * 2]));
        }
        else if (GetNamespace(&ns), ns.IsEmpty() && !(*mNspStack)[i * 2 + 1].IsEmpty()) {
//            throw new IllegalStateException("Cannot set default namespace for elements in no namespace");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        FAIL_RETURN(mWriter->WriteString("=\""));
        FAIL_RETURN(WriteEscaped((*mNspStack)[i * 2 + 1], '"'));
        FAIL_RETURN(mWriter->Write('"'));
    }

    if (mNspCounts->GetLength() <= mDepth + 1) {
        ArrayOf<Int32>* hlp = ArrayOf<Int32>::Alloc(mDepth + 8);
        memcpy(hlp->GetPayload(), mNspCounts->GetPayload(), (mDepth + 1) * sizeof(Int32));
        ArrayOf<Int32>::Free(mNspCounts);
        mNspCounts = hlp;
    }

    (*mNspCounts)[mDepth + 1] = (*mNspCounts)[mDepth];
    //   nspCounts[depth + 2] = nspCounts[depth];

    return mWriter->WriteString(close ? " />" : ">");
}

ECode CXmlSerializer::WriteEscaped(
    /* [in] */ CString s,
    /* [in] */ Int32 quot)
{
    Int32 N = s.GetCharCount();
    for (Int32 i = 0; i < N; i++) {
        Char32 c = s.GetChar(i);
        switch (c) {
            case '\n':
            case '\r':
            case '\t':
                if(quot == -1) {
                    FAIL_RETURN(mWriter->Write(c));
                }
                else {
                    FAIL_RETURN(mWriter->WriteString(StringBuffer("&#") + c + ";"));
                }
                break;
            case '&' :
                FAIL_RETURN(mWriter->WriteString("&amp;"));
                break;
            case '>' :
                FAIL_RETURN(mWriter->WriteString("&gt;"));
                break;
            case '<' :
                FAIL_RETURN(mWriter->WriteString("&lt;"));
                break;
            case '"' :
            case '\'' :
                if (c == quot) {
                    FAIL_RETURN(mWriter->WriteString(
                        c == '"' ? "&quot;" : "&apos;"));
                    break;
                }
            default :
                // BEGIN android-changed: refuse to output invalid characters
                // See http://www.w3.org/TR/REC-xml/#charsets for definition.
                // No other Java XML mWriter we know of does this, but no Java
                // XML reader we know of is able to parse the bad output we'd
                // otherwise generate.
                // Note: tab, newline, and carriage return have already been
                // handled above.
                Boolean valid = (c >= 0x20 && c <= 0xd7ff) || (c >= 0xe000 && c <= 0xfffd);
                if (!valid) {
                    return ReportInvalidCharacter(c);
                }
                if (mUnicode || c < 127) {
                    FAIL_RETURN(mWriter->Write(c));
                }
                else {
                    FAIL_RETURN(mWriter->WriteString(StringBuffer("&#") + c + ";"));
                }
                // END android-changed
        }
    }
    return NOERROR;
}

// BEGIN android-added
ECode CXmlSerializer::ReportInvalidCharacter(
    /* [in] */ Char32 ch)
{
//    throw new IllegalArgumentException("Illegal character (" + Integer.toHexString((int) ch) + ")");
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}
// END android-added

ECode CXmlSerializer::WriteDocDecl(
    /* [in] */ CString dd)
{
    FAIL_RETURN(mWriter->WriteString("<!DOCTYPE"));
    FAIL_RETURN(mWriter->WriteString(dd));
    return mWriter->WriteString(">");
}

ECode CXmlSerializer::EndDocument()
{
    while (mDepth > 0) {
        FAIL_RETURN(WriteEndTag(
            (*mElementStack)[mDepth * 3 - 3],
            (*mElementStack)[mDepth * 3 - 1]));
    }
    return Flush();
}

ECode CXmlSerializer::WriteEntityRef(
    /* [in] */ CString text)
{
    FAIL_RETURN(Check(FALSE));
    FAIL_RETURN(mWriter->Write('&'));
    FAIL_RETURN(mWriter->WriteString(text));
    return mWriter->Write(';');
}

ECode CXmlSerializer::GetFeature(
    /* [in] */ CString name,
    /* [out] */ Boolean* value)
{
    //return false;
    return
        CString("http://xmlpull.org/v1/doc/features.html#indent-output").Equals(name)
        ? (*mIndent)[mDepth]
        : FALSE;
}

ECode CXmlSerializer::GetPrefix(
    /* [in] */ CString ns,
    /* [in] */ Boolean generatePrefix,
    /* [out] */ String* prefix)
{
    VALIDATE_NOT_NULL(prefix);

//    try {
    if (FAILED(GetPrefix(ns, FALSE, generatePrefix, prefix))) {
//        throw new RuntimeException(e.toString());
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
//    }
//    catch (IOException e) {
//        throw new RuntimeException(e.toString());
//    }
}

ECode CXmlSerializer::GetPrefix(
    /* [in] */ CString ns,
    /* [in] */ Boolean includeDefault,
    /* [in] */ Boolean create,
    /* [out] */ String* prefix)
{
    assert(prefix != NULL);

    for (Int32 i = (*mNspCounts)[mDepth + 1] * 2 - 2; i >= 0; i -= 2) {
        if ((*mNspStack)[i + 1].Equals(ns)
            && (includeDefault || !(*mNspStack)[i].IsEmpty())) {
            String cand = (*mNspStack)[i];
            for (Int32 j = i + 2; j < (*mNspCounts)[mDepth + 1] * 2; j++) {
                if ((*mNspStack)[j].Equals(cand)) {
                    cand = NULL;
                    break;
                }
            }
            if (!cand.IsNull()) {
                *prefix = cand;
                return NOERROR;
            }
        }
    }

    if (!create) {
        *prefix = NULL;
        return NOERROR;
    }

    String _prefix;

    if (ns.IsEmpty()) {
        _prefix = "";
    }
    else {
        do {
            _prefix = "n" + (mAuto++);
            for (Int32 i = (*mNspCounts)[mDepth + 1] * 2 - 2; i >= 0; i -= 2) {
                if (_prefix.Equals((*mNspStack)[i])) {
                    _prefix = NULL;
                    break;
                }
            }
        }
        while (_prefix.IsNull());
    }

    Boolean p = mPending;
    mPending = FALSE;
    FAIL_RETURN(SetPrefix(_prefix, ns));
    mPending = p;
    *prefix = _prefix;
    return NOERROR;
}

ECode CXmlSerializer::GetProperty(
    /* [in] */ CString name,
    /* [out] */ IInterface** value)
{
//    throw new RuntimeException("Unsupported property");
    return E_RUNTIME_EXCEPTION;
}

ECode CXmlSerializer::WriteIgnorableWhitespace(
    /* [in] */ CString text)
{
    return WriteText(text);
}

ECode CXmlSerializer::SetFeature(
    /* [in] */ CString name,
    /* [in] */ Boolean state)
{
    if (CString("http://xmlpull.org/v1/doc/features.html#indent-output").Equals(name)) {
        (*mIndent)[mDepth] = state;
        return NOERROR;
    }
    else {
//        throw new RuntimeException("Unsupported Feature");
        return E_RUNTIME_EXCEPTION;
    }
}

ECode CXmlSerializer::SetProperty(
    /* [in] */ CString name,
    /* [in] */ IInterface* value)
{
//    throw new RuntimeException(
//            "Unsupported Property:" + value);
    return E_RUNTIME_EXCEPTION;
}

ECode CXmlSerializer::SetPrefix(
    /* [in] */ CString prefix,
    /* [in] */ CString ns)
{
    FAIL_RETURN(Check(FALSE));
    if (prefix.IsNull()) {
        prefix = "";
    }
    if (ns.IsNull()) {
        ns = "";
    }

    String def;
    FAIL_RETURN(GetPrefix(ns, TRUE, FALSE, &def));

    // boil out if already defined

    if (prefix.Equals(def)) {
        return NOERROR;
    }

    Int32 pos = ((*mNspCounts)[mDepth + 1]++) << 1;

    if (mNspStack->GetLength() < pos + 1) {
        ArrayOf<String>* hlp = ArrayOf<String>::Alloc(mNspStack->GetLength() + 16);
        memcpy(hlp->GetPayload(), mNspStack->GetPayload(), pos * sizeof(String));
        ArrayOf<String>::Free(mNspStack);
        mNspStack = hlp;
    }

    (*mNspStack)[pos++] = prefix;
    (*mNspStack)[pos] = ns;
    return NOERROR;
}

ECode CXmlSerializer::SetOutputEx(
    /* [in] */ IWriter* writer)
{
    // BEGIN android-changed
    // Guarantee that the mWriter is always buffered.
    if (IBufferedWriter::Probe(writer) != NULL) {
        mWriter = IBufferedWriter::Probe(writer);
    }
    else {
        CBufferedWriter::New(writer, WRITE_BUFFER_SIZE, (IBufferedWriter**)&mWriter);
    }
    // END android-changed

    // elementStack = new String[12]; //nsp/prefix/name
    //nspCounts = new int[4];
    //nspStack = new String[8]; //prefix/nsp
    //indent = new boolean[4];

    (*mNspCounts)[0] = 2;
    (*mNspCounts)[1] = 2;
    (*mNspStack)[0] = "";
    (*mNspStack)[1] = "";
    (*mNspStack)[2] = "xml";
    (*mNspStack)[3] = "http://www.w3.org/XML/1998/namespace";
    mPending = FALSE;
    mAuto = 0;
    mDepth = 0;

    mUnicode = FALSE;
    return NOERROR;
}

ECode CXmlSerializer::SetOutput(
    /* [in] */ IOutputStream* os,
    /* [in] */ const String& encoding)
{
    if (os == NULL) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IWriter> writer;
//    if (encoding.IsNull()) {
//        COutputStreamWriter::New(os, (IWriter**)&writer);
//    }
//    else {
//        COutputStreamWriter::New(os, encoding, (IWriter**)&writer);
//    }
    SetOutputEx(writer);
    mEncoding = encoding;
    if (!encoding.IsNull()
        && encoding.StartWith("utf", StringCase_Insensitive)) {
        mUnicode = TRUE;
    }
    return NOERROR;
}

ECode CXmlSerializer::StartDocument(
    /* [in] */ const String& encoding,
    /* [in] */ Boolean standalone)
{
    FAIL_RETURN(mWriter->WriteString("<?xml version='1.0' "));

    if (!encoding.IsNull()) {
        mEncoding = encoding;
        if (encoding.StartWith("utf", StringCase_Insensitive)) {
            mUnicode = TRUE;
        }
    }

    if (!mEncoding.IsNull()) {
        FAIL_RETURN(mWriter->WriteString("encoding='"));
        FAIL_RETURN(mWriter->WriteString(mEncoding));
        FAIL_RETURN(mWriter->WriteString("' "));
    }

    //todo:
//    if (standalone != null) {
        FAIL_RETURN(mWriter->WriteString("standalone='"));
        FAIL_RETURN(mWriter->WriteString(
            standalone ? "yes" : "no"));
        FAIL_RETURN(mWriter->WriteString("' "));
//    }
    return mWriter->WriteString("?>");
}

ECode CXmlSerializer::WriteStartTag(
    /* [in] */ CString ns,
    /* [in] */ CString name)
{
    FAIL_RETURN(Check(FALSE));

    //        if (namespace == null)
    //            namespace = "";

    if ((*mIndent)[mDepth]) {
        FAIL_RETURN(mWriter->WriteString("\r\n"));
        for (Int32 i = 0; i < mDepth; i++) {
            FAIL_RETURN(mWriter->WriteString("  "));
        }
    }

    Int32 esp = mDepth * 3;

    if (mElementStack->GetLength() < esp + 3) {
        ArrayOf<String>* hlp = ArrayOf<String>::Alloc(mElementStack->GetLength() + 12);
        memcpy(hlp->GetPayload(), mNspStack->GetPayload(), esp * sizeof(String));
        ArrayOf<String>::Free(mElementStack);
        mElementStack = hlp;
    }

    String prefix("");
    if (!ns.IsNull()) {
        FAIL_RETURN(GetPrefix(ns, TRUE, TRUE, &prefix));
    }

    if (!ns.IsNull() && ns.IsEmpty()) {
        for (Int32 i = (*mNspCounts)[mDepth]; i < (*mNspCounts)[mDepth + 1]; i++) {
            if ((*mNspStack)[i * 2].IsEmpty() && !(*mNspStack)[i * 2 + 1].IsEmpty()) {
//                throw new IllegalStateException("Cannot set default namespace for elements in no namespace");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
    }

    (*mElementStack)[esp++] = ns;
    (*mElementStack)[esp++] = prefix;
    (*mElementStack)[esp] = name;

    FAIL_RETURN(mWriter->Write('<'));
    if (!prefix.IsEmpty()) {
        FAIL_RETURN(mWriter->WriteString(prefix));
        FAIL_RETURN(mWriter->Write(':'));
    }

    FAIL_RETURN(mWriter->WriteString(name));

    mPending = TRUE;

    return NOERROR;
}

ECode CXmlSerializer::WriteAttribute(
    /* [in] */ CString ns,
    /* [in] */ CString name,
    /* [in] */ CString value)
{
    if (!mPending) {
//        throw new IllegalStateException("illegal position for attribute");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    //        int cnt = nspCounts[depth];

    if (ns.IsNull()) {
        ns = "";
    }

    //        depth--;
    //        pending = false;

    String prefix("");
    if (!ns.IsEmpty()) {
        FAIL_RETURN(GetPrefix(ns, FALSE, TRUE, &prefix));
    }

    //        pending = true;
    //        depth++;

    /*        if (cnt != nspCounts[depth]) {
                mWriter.write(' ');
                mWriter.write("xmlns");
                if (nspStack[cnt * 2] != null) {
                    mWriter.write(':');
                    mWriter.write(nspStack[cnt * 2]);
                }
                mWriter.write("=\"");
                writeEscaped(nspStack[cnt * 2 + 1], '"');
                mWriter.write('"');
            }
            */

    FAIL_RETURN(mWriter->Write(' '));
    if (!prefix.IsEmpty()) {
        FAIL_RETURN(mWriter->WriteString(prefix));
        FAIL_RETURN(mWriter->Write(':'));
    }
    FAIL_RETURN(mWriter->WriteString(name));
    FAIL_RETURN(mWriter->Write('='));
    Char32 q = value.IndexOf('"') == -1 ? '"' : '\'';
    FAIL_RETURN(mWriter->Write(q));
    FAIL_RETURN(WriteEscaped(value, q));
    return mWriter->Write(q);
}

ECode CXmlSerializer::Flush()
{
    FAIL_RETURN(Check(FALSE));
    return mWriter->Flush();
}

ECode CXmlSerializer::WriteEndTag(
    /* [in] */ CString ns,
    /* [in] */ CString name)
{
    if (!mPending) {
        mDepth--;
    }
    //        if (namespace == null)
    //          namespace = "";

    if ((ns.IsNull()
        && !(*mElementStack)[mDepth * 3].IsNull())
        || (!ns.IsNull()
            && !ns.Equals((*mElementStack)[mDepth * 3]))
        || !(*mElementStack)[mDepth * 3 + 2].Equals(name)) {
//        throw new IllegalArgumentException("</{"+namespace+"}"+name+"> does not match start");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mPending) {
        FAIL_RETURN(Check(TRUE));
        mDepth--;
    }
    else {
        if ((*mIndent)[mDepth + 1]) {
            FAIL_RETURN(mWriter->WriteString("\r\n"));
            for (int i = 0; i < mDepth; i++) {
                FAIL_RETURN(mWriter->WriteString("  "));
            }
        }

        FAIL_RETURN(mWriter->WriteString("</"));
        CString prefix = (*mElementStack)[mDepth * 3 + 1];
        if (!prefix.IsEmpty()) {
            FAIL_RETURN(mWriter->WriteString(prefix));
            FAIL_RETURN(mWriter->Write(':'));
        }
        FAIL_RETURN(mWriter->WriteString(name));
        FAIL_RETURN(mWriter->Write('>'));
    }

    (*mNspCounts)[mDepth + 1] = (*mNspCounts)[mDepth];
    return NOERROR;
}

ECode CXmlSerializer::GetNamespace(
    /* [out] */ String* ns)
{
    VALIDATE_NOT_NULL(ns);
    Int32 depth;
    GetDepth(&depth);
    *ns = depth == 0 ? String(NULL) : (*mElementStack)[depth * 3 - 3];
    return NOERROR;
}

ECode CXmlSerializer::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    Int32 depth;
    GetDepth(&depth);
    *name = depth == 0 ? String(NULL) : (*mElementStack)[depth * 3 - 1];
    return NOERROR;
}

ECode CXmlSerializer::GetDepth(
    /* [out] */ Int32* depth)
{
    VALIDATE_NOT_NULL(depth);
    *depth = mPending ? mDepth + 1 : mDepth;
    return NOERROR;
}

ECode CXmlSerializer::WriteText(
    /* [in] */ CString text)
{
    FAIL_RETURN(Check(FALSE));
    (*mIndent)[mDepth] = FALSE;
    return WriteEscaped(text, -1);
}

ECode CXmlSerializer::WriteTextEx(
    /* [in] */ const ArrayOf<Char8>& buf,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len)
{
    Int32 offset;

    Character::GetOffsetByChars(buf, 0, buf.GetLength(), 0, start, &offset);
    String str(buf.GetPayload() + offset, len);
    return WriteText(str);
}

ECode CXmlSerializer::WriteCdSect(
    /* [in] */ CString text)
{
    FAIL_RETURN(Check(FALSE));
    // BEGIN android-changed: ]]> is not allowed within a CDATA,
    // so break and start a new one when necessary.
    String data(text);
    data = data.Replace("]]>", "]]]]><![CDATA[>");
    // We also aren't allowed any invalid characters.
    Int32 N = data.GetCharCount();
    for (Int32 i = 0; i < N; i++) {
        Char32 ch = data.GetChar(i);
        Boolean valid = (ch >= 0x20 && ch <= 0xd7ff) ||
                (ch == '\t' || ch == '\n' || ch == '\r') ||
                (ch >= 0xe000 && ch <= 0xfffd);
        if (!valid) {
            return ReportInvalidCharacter(ch);
        }
    }
    FAIL_RETURN(mWriter->WriteString("<![CDATA["));
    FAIL_RETURN(mWriter->WriteString(data));
    return mWriter->WriteString("]]>");
    // END android-changed
}

ECode CXmlSerializer::WriteComment(
    /* [in] */ CString text)
{
    FAIL_RETURN(Check(FALSE));
    FAIL_RETURN(mWriter->WriteString("<!--"));
    FAIL_RETURN(mWriter->WriteString(text));
    return mWriter->WriteString("-->");
}

ECode CXmlSerializer::WriteProcessingInstruction(
    /* [in] */ CString text)
{
    FAIL_RETURN(Check(FALSE));
    FAIL_RETURN(mWriter->WriteString("<?"));
    FAIL_RETURN(mWriter->WriteString(text));
    return mWriter->WriteString("?>");
}
