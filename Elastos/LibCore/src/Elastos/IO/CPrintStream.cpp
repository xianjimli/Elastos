#include "cmdef.h"
#include "CPrintStream.h"
#include "CFile.h"
#include "CCharsetHelper.h"
#include "CFileOutputStream.h"

const String CPrintStream::TOKEN_NULL = String(NULL);
const String CPrintStream::lineSeparator = String("\n");

ECode CPrintStream::ClearError()
{
    mIoError = FALSE;
    return NOERROR;
}

ECode CPrintStream::SetError()
{
    mIoError = TRUE;
    return NOERROR;
}

ECode CPrintStream::constructor(
    /** [in] **/ IOutputStream* outs)
{
    FAIL_RETURN(FilterOutputStream::Init(outs));
    if (outs == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode CPrintStream::constructor(
    /** [in] **/ IOutputStream* outs, 
    /** [in] **/ Boolean autoflush)
{
    FAIL_RETURN(FilterOutputStream::Init(outs));
    if (outs == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mAutoflush = autoflush;
    return NOERROR;
}

ECode CPrintStream::constructor(
    /** [in] **/ IOutputStream* outs, 
    /** [in] **/ Boolean autoflush, 
    /** [in] **/ String enc)
{
    FAIL_RETURN(FilterOutputStream::Init(outs));
    if (outs == NULL || enc.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mAutoflush = autoflush;
    Boolean tmp;
    ECode ec;

    AutoPtr<ICharsetHelper> helper;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);
    ec = helper->IsSupported(enc, &tmp);

    if(!tmp || NOERROR != ec)
    {
        return E_UNSUPPORTED_ENCODING_EXCEPTION;
    }
    mEncoding = enc;
    return NOERROR;
}

ECode CPrintStream::constructor(
    /** [in] **/ IFile* file)
{
    //super(new FileOutputStream(file));

    //AutoPtr<IFileOutputStream> fos;
    //CFileOutputStream::New(file, (IFileOutputStream**)&fos);
    //FAIL_RETURN(FilterOutputStream((IOutputStream*)&fos));

    return NOERROR;
}

ECode CPrintStream::constructor(
    /** [in] **/ IFile* file, 
    /** [in] **/ String csn)
{
    //super(new FileOutputStream(file));
    if (csn.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Boolean tmp;
    ECode ec;

    AutoPtr<ICharsetHelper> helper;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);
    ec = helper->IsSupported(csn, &tmp);

    if (!tmp) {
        return E_UNSUPPORTED_ENCODING_EXCEPTION;
    }
    mEncoding = csn;
    return NOERROR;
}

ECode CPrintStream::constructor(
    /** [in] **/ String fileName)
{
    AutoPtr<IFile> file;
    CFile::New(fileName, (IFile**)&file);
    constructor(file);
    return NOERROR;
}

ECode CPrintStream::constructor(
    /** [in] **/ String fileName, 
    /** [in] **/ String csn)
{
    AutoPtr<IFile> file;
    CFile::New(fileName, (IFile**)&file);
    constructor(file, csn);
    return NOERROR;
}

ECode CPrintStream::CheckError(
    /** [out] **/ Boolean* hasError)
{
    assert(hasError != NULL);
    Boolean tmp;
    AutoPtr<IOutputStream> delegate;
    delegate = mOut;
    if (delegate == NULL) {
        *hasError = mIoError;
        return NOERROR;
    }

    Flush();
    delegate->CheckError(&tmp);
    *hasError = mIoError || tmp;
    return NOERROR;
}

ECode CPrintStream::Close()
{
    Flush();
    if (mOut != NULL) {
        if(NOERROR != mOut->Close())
        {
            SetError();
        }
        mOut = NULL;
    }
    return NOERROR;
}

ECode CPrintStream::Flush()
{
    if (mOut != NULL) {
        if(NOERROR != mOut->Flush())
        {
            SetError();
        }
    }
    return NOERROR;
}

ECode CPrintStream::Format(
    /** [in] **/ const String& format, 
//    /** [in] **/ Object... args,
    /** [out] **/ IPrintStream** pw)
{
    //return format(Locale.getDefault(), format, args);
    return NOERROR;
}

ECode CPrintStream::FormatEx(
    /** [in] **/ ILocale* l, 
    /** [in] **/ const String& format, 
 //   /** [in] **/ Object... args,
    /** [out] **/ IPrintStream** pw)
{
    if (format.IsNull()) {
        return E_EOF_EXCEPTION;
    }
  // new Formatter(this, l).format(format, args); /////////////////
    *pw = this;
    return NOERROR;
}

ECode CPrintStream::Printf(
    /** [in] **/ const String& format, 
//    /** [in] **/ Object... args,
    /** [out] **/ IPrintStream** pw)
{
    //return format(format, args);
    return NOERROR;
}

ECode CPrintStream::PrintfEX(
        /** [in] **/ ILocale* l, 
        /** [in] **/ const String& format, 
    //  [in] Object... args,
        /** [out] **/ IPrintStream** ps)
{
    //return format(l, format, args);
    return NOERROR;
}

ECode CPrintStream::PrintChar8(
    /** [in] **/ Char8 ch)
{
    return PrintString(String::FromInt32((Int32)ch));
}

ECode CPrintStream::PrintChar8Ex(
    /** [in] **/ const ArrayOf<Char8>& charArray)
{
    return PrintString(String(charArray.GetPayload()));
}

ECode CPrintStream::PrintDouble(
    /** [in] **/ Double dnum)
{
    return PrintString(String::FromDouble(dnum));
}

ECode CPrintStream::PrintFloat(
    /** [in] **/ Float fnum)
{
    //PrintString(String.valueOf(fnum));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintStream::PrintInt32(
    /** [in] **/ Int32 inum)
{
    return PrintString(String::FromInt32(inum));
}

ECode CPrintStream::PrintInt64(
    /** [in] **/ Int64 lnum)
{
    return PrintString(String::FromInt64(lnum));
}

ECode CPrintStream::PrintObj(
    /** [in] **/ IInterface* obj)
{
    //PrintString(String.valueOf(obj));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintStream::PrintString(
    /** [in] **/ const String& str)
{
    if (mOut == NULL) {
        SetError();
        return NOERROR;
    }
    if (str.IsNull()) {
        PrintString(String("NULL"));
        return NOERROR;
    }

    ArrayOf<Byte>* tmp = ArrayOf<Byte>::Alloc(str.GetLength());
    UInt32 i;
    for(i=0;i<str.GetLength();i++)
    {
        (*tmp)[i] = str.GetChar(i);
    }
    if(mEncoding == NULL && NOERROR != WriteBuffer(*tmp))
    {
        SetError();
    }
/*    if(mEncoding != NULL && NOERROR != WriteBuffer(str.getBytes(mEncoding)))
    {
        SetError();
    }*/
    ArrayOf<Byte>::Free(tmp);
    return NOERROR;
}

ECode CPrintStream::PrintBoolean(
    /** [in] **/ Boolean result)
{
    return PrintString(String::FromBoolean(result));
}

ECode CPrintStream::Println()
{
    return Newline();
}

ECode CPrintStream::PrintlnChar8(
    /** [in] **/ Char8 ch)
{
    return PrintlnString(String::FromInt32((Int32)ch));
}

ECode CPrintStream::PrintlnChar8Ex(
    /** [in] **/ const ArrayOf<Char8>& charArray)
{
    return PrintlnString(String(charArray.GetPayload()));
}

ECode CPrintStream::PrintlnDouble(
    /** [in] **/ Double dnum)
{
    return PrintlnString(String::FromDouble(dnum));
}

ECode CPrintStream::PrintlnFloat(
    /** [in] **/ Float fnum)
{
    //PrintlnString(String.valueOf(fnum));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintStream::PrintlnInt32(
    /** [in] **/ Int32 inum)
{
    return PrintlnString(String::FromInt32(inum));
}

ECode CPrintStream::PrintlnInt64(
    /** [in] **/ Int64 lnum)
{
    return PrintlnString(String::FromInt64(lnum));
}

ECode CPrintStream::PrintlnObj(
    /** [in] **/ IInterface* obj)
{
    //PrintlnString(String.valueOf(obj));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintStream::PrintlnString(
    /** [in] **/ const String& str)
{
    PrintString(str);
    Newline();
    return NOERROR;
}

ECode CPrintStream::PrintlnBoolean(
    /** [in] **/ Boolean result)
{
    return PrintlnString(String::FromBoolean(result));
}

ECode CPrintStream::Write(
    /** [in] **/ Int32 oneChar32)
{
    if (mOut == NULL) {
        SetError();
        return NOERROR;
    }

    if(NOERROR != mOut->Write(oneChar32))
    {
        SetError();
    }
    Int32 b = oneChar32 & 0xFF;
    // 0x0A is ASCII newline, 0x15 is EBCDIC newline.
    Boolean isNewline = (b == 0x0A || b == 0x15);
    if (mAutoflush && isNewline) {
        Flush();
    }
    return NOERROR;
}

ECode CPrintStream::WriteBuffer(
    /** [in] **/ const ArrayOf<Byte>& buffer)
{
    return FilterOutputStream::WriteBuffer(buffer);
}

ECode CPrintStream::WriteBufferEx(
    /** [in] **/ Int32 offset,
    /** [in] **/ Int32 count,
    /** [in] **/ const ArrayOf<Byte>& buffer)
{
    // Force buffer null check first!
    if (offset > buffer.GetLength() || offset < 0) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (count < 0 || count > buffer.GetLength() - offset) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
//    synchronized (this) {
        if (mOut == NULL) {
            SetError();
            return NOERROR;
        }
        if(NOERROR != mOut->WriteBufferEx(offset, count, buffer))
        {
            SetError();
        }
        if (mAutoflush) {
            Flush();
        }

//    }
    return NOERROR;
}

ECode CPrintStream::Append(
    /** [in] **/ Char32 c)
{
    return PrintInt32(c);
}

ECode CPrintStream::AppendEx(
    /** [in] **/ ICharSequence* csq)
{
    if (NULL == csq) {
        PrintString(TOKEN_NULL);
    } else {
        String str;
        csq->ToString(&str);
        PrintString(str);
    }
    return NOERROR;
}

ECode CPrintStream::AppendEx2(
    /** [in] **/ ICharSequence* csq,
    /** [in] **/ Int32 start,
    /** [in] **/ Int32 end)
{
    if (NULL == csq) {
        PrintString(TOKEN_NULL.Substring(start, end));
    } else {
        AutoPtr<ICharSequence> subStr;
        csq->SubSequence(start, end, (ICharSequence**)&subStr);
        String subString;          
        subStr->ToString(&subString);
        PrintString(subString);
    }
    return NOERROR;
}

ECode CPrintStream::Newline()
{
    return PrintString(lineSeparator);
}

Mutex* CPrintStream::GetSelfLock()
{
	return &_m_syncLock;
}