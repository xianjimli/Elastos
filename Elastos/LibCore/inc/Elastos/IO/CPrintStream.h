#ifndef __CPRINTSTREAM_H__
#define __CPRINTSTREAM_H__

#include "_CPrintStream.h"
#include "FilterOutputStream.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CPrintStream), public FilterOutputStream
{
private:

    static const String TOKEN_NULL;// = "NULL";

    /**
     * indicates whether or not this PrintStream has incurred an error.
     */
    Boolean mIoError;

    /**
     * indicates whether or not this PrintStream should flush its contents after
     * printing a new line.
     */
    Boolean mAutoflush;

    String mEncoding;

    static const  String lineSeparator;//AccessController.doPrivileged(new PriviAction<String>("line.separator"));

protected:
    CARAPI ClearError();

    CARAPI SetError();

public:
    CARAPI constructor(
        /** [in] **/ IOutputStream* outs);

    CARAPI constructor(
        /** [in] **/ IOutputStream* outs, 
        /** [in] **/ Boolean autoflush);

    CARAPI constructor(
        /** [in] **/ IOutputStream* outs, 
        /** [in] **/ Boolean autoflush, 
        /** [in] **/ String enc);

    CARAPI constructor(
        /** [in] **/ IFile* file);

    CARAPI constructor(
        /** [in] **/ IFile* file, 
        /** [in] **/ String csn);

    CARAPI constructor(
        /** [in] **/ String fileName);

    CARAPI constructor(
        /** [in] **/ String fileName, 
        /** [in] **/ String csn);

    CARAPI CheckError(
        /** [out] **/ Boolean* hasError);

    CARAPI Close();

    CARAPI Flush();

    CARAPI Format(
        /** [in] **/ const String& format, 
    //    /** [in] **/ Object... args,
        /** [out] **/ IPrintStream** pw);

    CARAPI FormatEx(
        /** [in] **/ ILocale* l, 
        /** [in] **/ const String& format, 
     //   /** [in] **/ Object... args,
        /** [out] **/ IPrintStream** pw);

    CARAPI Printf(
        /** [in] **/ const String& format, 
    //    /** [in] **/ Object... args,
        /** [out] **/ IPrintStream** pw);

    CARAPI PrintfEX(
        /** [in] **/ ILocale* l, 
        /** [in] **/ const String& format, 
    //  [in] Object... args,
        /** [out] **/ IPrintStream** ps);

    CARAPI PrintChar8(
        /** [in] **/ Char8 ch);

    CARAPI PrintChar8Ex(
        /** [in] **/ const ArrayOf<Char8>& charArray);

    CARAPI PrintDouble(
        /** [in] **/ Double dnum);

    CARAPI PrintFloat(
        /** [in] **/ Float fnum);

    CARAPI PrintInt32(
        /** [in] **/ Int32 inum);

    CARAPI PrintInt64(
        /** [in] **/ Int64 lnum);

    CARAPI PrintObj(
        /** [in] **/ IInterface* obj);

    CARAPI PrintString(
        /** [in] **/ const String& str);

    CARAPI PrintBoolean(
        /** [in] **/ Boolean result);

    CARAPI Println();

    CARAPI PrintlnChar8(
        /** [in] **/ Char8 ch);

    CARAPI PrintlnChar8Ex(
        /** [in] **/ const ArrayOf<Char8>& charArray);

    CARAPI PrintlnDouble(
        /** [in] **/ Double dnum);

    CARAPI PrintlnFloat(
        /** [in] **/ Float fnum);

    CARAPI PrintlnInt32(
        /** [in] **/ Int32 inum);

    CARAPI PrintlnInt64(
        /** [in] **/ Int64 lnum);

    CARAPI PrintlnObj(
        /** [in] **/ IInterface* obj);

    CARAPI PrintlnString(
        /** [in] **/ const String& str);

    CARAPI PrintlnBoolean(
        /** [in] **/ Boolean result);

    CARAPI Write(
        /** [in] **/ Int32 oneChar32);

    CARAPI WriteBuffer(
        /** [in] **/ const ArrayOf<Byte>& buffer);

    CARAPI WriteBufferEx(
        /** [in] **/ Int32 offset,
        /** [in] **/ Int32 count,
        /** [in] **/ const ArrayOf<Byte>& buffer);

    CARAPI Append(
        /** [in] **/ Char32 c);

    CARAPI AppendEx(
        /** [in] **/ ICharSequence* csq);

    CARAPI AppendEx2(
        /** [in] **/ ICharSequence* csq,
        /** [in] **/ Int32 start,
        /** [in] **/ Int32 end);
private:
    CARAPI Newline();

    CARAPI_(Mutex*) GetSelfLock();
};
#endif //__CPRINTSTREAM_H__