#include "cmdef.h"
#include "CPrintWriter.h"
#include "COutputStreamWriter.h"
#include "CFileOutputStream.h"
#include "CBufferedOutputStream.h"

ECode CPrintWriter::ClearError()
{
//	synchronized (lock) {
    mIoError = FALSE;
//    }
    return NOERROR;
}

ECode CPrintWriter::SetError()
{
//	synchronized (lock) {
    mIoError = TRUE;
//    }
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ IOutputStream* outs)
{
    AutoPtr<IOutputStreamWriter> opsw;
    COutputStreamWriter::New(outs, (IOutputStreamWriter**)&opsw);
    constructor(opsw, FALSE);
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ IOutputStream* outs, 
    /** [in] **/ Boolean autoflush)
{
    AutoPtr<IOutputStreamWriter> opsw;
    COutputStreamWriter::New(outs, (IOutputStreamWriter**)&opsw);
    constructor(opsw, autoflush);
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ IWriter* wr)
{
    constructor(wr, FALSE);
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ IWriter* wr, 
    /** [in] **/ Boolean autoflush)
{
    //super(wr);  ////////////////////////////
    mAutoflush = autoflush;
    mOut = wr;
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ IFile* file)
{
    AutoPtr<IFileOutputStream> fops;
    CFileOutputStream::New(file, (IFileOutputStream**)&fops);
    AutoPtr<IBufferedOutputStream> bops;
    CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops);
    AutoPtr<IOutputStreamWriter> opsr;
    COutputStreamWriter::New(bops, (IOutputStreamWriter**)&opsr);
    constructor(opsr, FALSE);
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ IFile* file, 
    /** [in] **/ String csn)
{
    AutoPtr<IFileOutputStream> fops;
    CFileOutputStream::New(file, (IFileOutputStream**)&fops);
    AutoPtr<IBufferedOutputStream> bops;
    CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops);
    AutoPtr<IOutputStreamWriter> opsr;
    COutputStreamWriter::New(bops, csn, (IOutputStreamWriter**)&opsr);
    constructor(opsr, FALSE);
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ String fileName)
{
    AutoPtr<IFileOutputStream> fops;
    CFileOutputStream::New(fileName, (IFileOutputStream**)&fops);
    AutoPtr<IBufferedOutputStream> bops;
    CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops);
    AutoPtr<IOutputStreamWriter> opsr;
    COutputStreamWriter::New(bops, (IOutputStreamWriter**)&opsr);
    constructor(opsr, FALSE);
    return NOERROR;
}

ECode CPrintWriter::constructor(
    /** [in] **/ String fileName, 
    /** [in] **/ String csn)
{
    AutoPtr<IFileOutputStream> fops;
    CFileOutputStream::New(fileName, (IFileOutputStream**)&fops);
    AutoPtr<IBufferedOutputStream> bops;
    CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops);
    AutoPtr<IOutputStreamWriter> opsr;
    COutputStreamWriter::New(bops, csn, (IOutputStreamWriter**)&opsr);
    constructor(opsr, FALSE);
    return NOERROR;
}

ECode CPrintWriter::CheckError(
    /** [out] **/ Boolean* hasError)
{
    assert(hasError != NULL);
    Boolean tmp;
    AutoPtr<IWriter> delegate;
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

ECode CPrintWriter::Close()
{
//	synchronized (lock) {
    if (mOut != NULL) {
        if(NOERROR != mOut->Close())
        {
            SetError();
        }
        mOut = NULL;
    }
//    }
    return NOERROR;
}

ECode CPrintWriter::Flush()
{
//	synchronized (lock) {
    if (mOut != NULL) {
        if(NOERROR != mOut->Flush())
        {
            SetError();
        }
    } else {
        SetError();
    }
//    }
    return NOERROR;
}

ECode CPrintWriter::Format(
    /** [in] **/ const String& format, 
    /** [out] **/ IPrintWriter** pw//,
//    /** [in] **/ ...
    )
{
	//return format(Locale.getDefault(), format, args);
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::FormatEx(
    /** [in] **/ ILocale* l, 
    /** [in] **/ const String& format, 
    /** [out] **/ IPrintWriter** pw//,
 //   /** [in] **/ ...
    )
{
	if (format.IsNull()) {
        //throw new NullPointerException("format == null");
        return E_EOF_EXCEPTION;
    }
    //new Formatter(this, l).format(format, args); /////////////////////////////
    if (mAutoflush) {
        Flush();
    }
    *pw = this;
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::Printf(
    /** [in] **/ const String& format, 
    /** [out] **/ IPrintWriter** pw//,
 //   /** [in] **/ ...
    )
{
	//return Format(format, args, pw);
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::PrintfEx(
    /** [in] **/ ILocale* l, 
    /** [in] **/ const String& format, 
    /** [out] **/ IPrintWriter** pw//,
//    /** [in] **/ ...
    )
{
	//return FormatEx(l, format, args, pw);
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::PrintChar8(
    /** [in] **/ Char8 ch)
{
	return PrintString(String::FromInt32((Int32)ch));
}

ECode CPrintWriter::PrintChar8Ex(
    /** [in] **/ const ArrayOf<Char8>& charArray)
{
	return PrintString(String(charArray.GetPayload()));
}

ECode CPrintWriter::PrintDouble(
    /** [in] **/ Double dnum)
{
    return PrintString(String::FromDouble(dnum));
}

ECode CPrintWriter::PrintFloat(
    /** [in] **/ Float fnum)
{
	//PrintString(String.valueOf(fnum));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::PrintInt32(
    /** [in] **/ Int32 inum)
{
	return PrintString(String::FromInt32(inum));
}

ECode CPrintWriter::PrintInt64(
    /** [in] **/ Int64 lnum)
{
	return PrintString(String::FromInt64(lnum));
}

ECode CPrintWriter::PrintObj(
    /** [in] **/ IInterface* obj)
{
	//PrintString(String.valueOf(obj));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::PrintString(
    /** [in] **/ const String& str)
{
    return WriteString(!str.IsNull() ? str : String("NULL"));
}

ECode CPrintWriter::PrintBoolean(
    /** [in] **/ Boolean result)
{
    return PrintString(String::FromBoolean(result));
}

ECode CPrintWriter::Println()
{
//	synchronized (lock) {
    PrintString(lineSeparator);
    if (mAutoflush) {
        Flush();
    }
//    }
    return NOERROR;
}

ECode CPrintWriter::PrintlnChar8(
    /** [in] **/ Char8 ch)
{
	return PrintlnString(String::FromInt32((Int32)ch));
}

ECode CPrintWriter::PrintlnChar8Ex(
    /** [in] **/const ArrayOf<Char8>& charArray)
{
    return PrintlnString(String(charArray.GetPayload()));
}

ECode CPrintWriter::PrintlnDouble(
    /** [in] **/ Double dnum)
{
	return PrintlnString(String::FromDouble(dnum));
}

ECode CPrintWriter::PrintlnFloat(
    /** [in] **/ Float fnum)
{
	//PrintlnString(String.valueOf(fnum));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::PrintlnInt32(
    /** [in] **/ Int32 inum)
{
	return PrintlnString(String::FromInt32(inum));
}

ECode CPrintWriter::PrintlnInt64(
    /** [in] **/ Int64 lnum)
{
	return PrintlnString(String::FromInt64(lnum));
}

ECode CPrintWriter::PrintlnObj(
    /** [in] **/ IInterface* obj)
{
	//PrintlnString(String.valueOf(obj));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintWriter::PrintlnString(
    /** [in] **/ const String& str)
{
	//synchronized (lock) {
    PrintString(str);
    Println();
  //  }
    return NOERROR;
}

ECode CPrintWriter::PrintlnBoolean(
    /** [in] **/ Boolean result)
{
    return PrintlnString(String::FromBoolean(result));
}

ECode CPrintWriter::Write(
    /** [in] **/ Int32 oneChar32)
{
    ArrayOf<Char8>* tmp = ArrayOf<Char8>::Alloc(1);
    (*tmp)[0] = (Char8) oneChar32;
    DoWrite(0, 1, *tmp);
    ArrayOf<Char8>::Free(tmp);
    return NOERROR;
}

ECode CPrintWriter::WriteBuffer(
    /** [in] **/ const ArrayOf<Char8>& buffer)
{
	return WriteBufferEx(0, buffer.GetLength(), buffer);
}

ECode CPrintWriter::WriteBufferEx(
    /** [in] **/ Int32 offset,
    /** [in] **/ Int32 count,
    /** [in] **/ const ArrayOf<Char8>& buffer)
{
	return DoWrite(offset, count, buffer);
}

ECode CPrintWriter::WriteString(
    /** [in] **/ CString str)
{
    Int32 i;
    ArrayOf<Char8> *tmp = ArrayOf<Char8>::Alloc(str.GetLength());
    for(i=0;i<str.GetLength();i++)
    {
        (*tmp)[i] = str.GetChar(i);
    }
    WriteBuffer(*tmp);
    ArrayOf<Char8>::Free(tmp);
    return NOERROR;
}

ECode CPrintWriter::WriteStringEx(
    /** [in] **/ Int32 offset,
    /** [in] **/ Int32 count,
    /** [in] **/ CString str)
{
	//write(str.substring(offset, offset + count).toCharArray());
    return NOERROR;
}

ECode CPrintWriter::Append(
    /** [in] **/ Char32 c)
{
	return Write((Int32)c);
}

ECode CPrintWriter::AppendEx(
    /** [in] **/ ICharSequence* csq)
{
	if (NULL == csq) {
        AppendEx2(NULL, 0, 0);
    } else {
        Int32 length;
        csq->GetLength(&length);
        AppendEx2(csq, 0, length);
    }
    return NOERROR;
}

ECode CPrintWriter::AppendEx2(
    /** [in] **/ ICharSequence* csq,
    /** [in] **/ Int32 start,
    /** [in] **/ Int32 end)
{
	if (NULL == csq) {
       // csq = TOKEN_NULL;/////////////////////////////////
    }

    AutoPtr<ICharSequence> subStr;
    csq->SubSequence(start, end, (ICharSequence**)&subStr);
    String output;          
    subStr->ToString(&output);
    WriteStringEx(0, output.GetLength(), output);
    return NOERROR;
}

ECode CPrintWriter::DoWrite(
    /** [in] **/ Int32 offset, 
    /** [in] **/ Int32 count,
    /** [in] **/ const ArrayOf<Char8>& buf)
{
//	synchronized (lock) {
    if (mOut != NULL) {
        if(NOERROR != mOut->WriteBufferEx(offset, count, buf))
        {
            SetError();
        }
    } else {
        SetError();
    }
//    }
    return NOERROR;
}

Mutex* CPrintWriter::GetSelfLock()
{
    return &_m_syncLock;
}