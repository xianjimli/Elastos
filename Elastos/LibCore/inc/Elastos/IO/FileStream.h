//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__

#include <_pubcrt.h>

#include <stdio.h>
#include <sys/stat.h>

using namespace Elastos;
/*!
  @brief 
  @since
  @sa
*/
class FileStream
{
public:
/*!
	@brief Open a file named @a fileName and read it into the iternal string buffer @a sbFileName.
	@param fileName <b>[in]</b> the source file.
	@return
    @since
    @sa
*/
    ECode OpenRead(String fileName) {
        fp = fopen(fileName, "rb");
        if(!fp) {
            return E_FS_NO_SUCH_FILE;
        }
        sbFileName.Copy(fileName);
        return NOERROR;
    }
/*!
	@brief Open a file named @a fileName and write it on the iternal string buffer @a sbFileName.
	@param fileName <b>[in]</b> the source file. 
	@param append a boolean value.if @a append is true,the data is written starting from the end
	mark;if it is false,the original data in the @a filename will be clear up first,then write the 
	new data on the file @a filename.
	@return
    @since
    @sa
*/
    ECode OpenWrite(String fileName, Boolean append) {
        if (append) {
            fp = fopen(fileName, "ab");
        }
        else {
            fp = fopen(fileName, "wb");
        }

        if (!fp) return E_ACCESS_DENIED;
        sbFileName.Copy(fileName);
        return NOERROR;
    }
/*!
	@brief Get the bytes size of this file stream object.
	@param pSize <b>[out]</b> the bytes size of this FileStream object.
	@return
    @since
    @sa
*/
    ECode GetSize(Int32 * pSize) {
        if (!pSize) {
            return E_INVALID_ARGUMENT;
        }

        struct stat buf;
        if (-1 == stat(sbFileName.GetPayload(), &buf)) {
            return E_CLOSED_STREAM;
        }
        *pSize = (Int32)buf.st_size;

        return NOERROR;
    }
/*!
	@brief read @a toRead bytes from @a pBuf into the iternal file @a fp and 
	store the data starting at offset @a offset.
	@param offset <b>[in]</b> the offset in the iternal file.
	@param toRead <b>[in]</b> the maximum number of bytes to be read.
	@param pBuf <b>[in]</b> the source data.
	@return
    @since
    @sa
*/
    ECode Read(Int32 offset, Int32 toRead, MemoryBuf * pBuf) {
        fseek(fp, offset, SEEK_SET);
        Int32 ReadNum = fread(pBuf->GetPayload(), sizeof(Byte), toRead, fp);
        if (ReadNum <= 0) {
            return E_OUT_OF_STREAM;
        }

        pBuf->SetUsed(ReadNum);
        return NOERROR;
    }
/*!
	@brief write bytes from @a buf on the iternal file @a fp and 
	store the data starting at offset @a offset.
	@param offset <b>[in]</b> the offset in the iternal file.
	@param buf <b>[in]</b> the source data.
	@param pWrite <b>[out]</b> the pointer pointed to the number has been written.
	@return
    @since
    @sa
*/
    ECode Write(Int32 offset, const MemoryBuf & buf, Int32 * pWrite) {
        *pWrite = fwrite(buf.GetPayload(), sizeof(Byte), buf.GetUsed(), fp);
        if (*pWrite == -1) {
            return E_FS_NO_SPACE;
        }
        fflush(fp);
        return NOERROR;
    }
/*!
	@brief flush the iternal file.
	@return
    @since
    @sa
*/
    ECode Flush() {
        fflush(fp);
        return NOERROR;
    }
/*!
	@brief close all the system resource associated with the iternal file @a fp.
	@return
    @since
    @sa
*/
    UInt32 Release() {
        if (fp) fclose(fp);
        delete this;
        return 0;
    }
/*!
	@brief create a FileStream object with a null iternal file.
*/
    FileStream() : fp(NULL) {}
/*!
	@brief the destructor of the FileStream.
*/
    ~FileStream() {}

private:
/*!
	@brief the underlying file.
*/
    FILE *fp;
/*!
	@brief the iternal buffer.
*/
    StringBuf_<_MAX_FNAME> sbFileName;
};

#endif //__FILESTREAM_H__
