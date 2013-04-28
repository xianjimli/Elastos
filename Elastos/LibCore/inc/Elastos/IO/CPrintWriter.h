
#ifndef __CPRINTWRITR_H__
#define __CPRINTWRITR_H__

#include "_CPrintWriter.h"
#include <elastos.h>
#include "Writer.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CPrintWriter), public Writer
{
protected:
    /**
     * The writer to print data to.
     */
    AutoPtr<IWriter> mOut;
private:
    /**
     * Indicates whether this PrintWriter is in an error state.
     */
    Boolean mIoError;

    /**
     * Indicates whether or not this PrintWriter should flush its contents after
     * printing a new line.
     */
    Boolean mAutoflush;

    const String lineSeparator;// = AccessController.doPrivileged(new PriviAction<String>("line.separator"));

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
        /** [in] **/ IWriter* wr);

    CARAPI constructor(
        /** [in] **/ IWriter* wr, 
        /** [in] **/ Boolean autoflush);

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
        /** [out] **/ IPrintWriter** pw//,
    //    /** [in] **/ ...
        );

    CARAPI FormatEx(
        /** [in] **/ ILocale* l, 
        /** [in] **/ const String& format, 
        /** [out] **/ IPrintWriter** pw//,
    //    /** [in] **/ ...
        );

    CARAPI Printf(
        /** [in] **/ const String& format, 
        /** [out] **/ IPrintWriter** pw//,
    //    /** [in] **/ ...
        );

    CARAPI PrintfEx(
        /** [in] **/ ILocale* l, 
        /** [in] **/ const String& format, 
        /** [out] **/ IPrintWriter** pw//,
    //    /** [in] **/ ...
        );

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
        /** [in] **/ const ArrayOf<Char8>& buffer);

    CARAPI WriteBufferEx(
        /** [in] **/ Int32 offset,
        /** [in] **/ Int32 count,
        /** [in] **/ const ArrayOf<Char8>& buffer);

    CARAPI WriteString(
        /** [in] **/ CString str);

    CARAPI WriteStringEx(
        /** [in] **/ Int32 offset,
        /** [in] **/ Int32 count,
        /** [in] **/ CString str);

    CARAPI Append(
        /** [in] **/ Char32 c);

    CARAPI AppendEx(
        /** [in] **/ ICharSequence* csq);

    CARAPI AppendEx2(
        /** [in] **/ ICharSequence* csq,
        /** [in] **/ Int32 start,
        /** [in] **/ Int32 end);
private:
    CARAPI DoWrite(
    /** [in] **/ Int32 offset, 
    /** [in] **/ Int32 count,
    /** [in] **/ const ArrayOf<Char8>& buf);

    CARAPI_(Mutex*) GetSelfLock();
};
#endif //__CPRINTWRITR_H__