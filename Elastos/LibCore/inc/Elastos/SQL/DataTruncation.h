#ifdef _DATATRUNCATION_H_
#define _DATATRUNCATION_H_

#include <elastos.h>

using namespace Elastos;

class DataTruncation {
public:
    /**
     * Creates the {@code DataTruncation} object. The reason is set to {@code
     * "Data truncation"}, the {@code ErrorCode} is set to the {@code
     * SQLException} default value, and the other fields are set to the values
     * supplied as arguments.
     *
     * @param index
     *            the Index value of the column value or parameter that was
     *            truncated.
     * @param parameter
     *            {@code true} if it was a parameter value that was truncated,
     *            {@code false} otherwise.
     * @param read
     *            {@code true} if the truncation occurred on a read operation,
     *            {@code false} otherwise.
     * @param dataSize
     *            the original size of the truncated data.
     * @param transferSize
     *            the size of the data after truncation.
     */
    DataTruncation();

    ~DataTruncation();

    CARAPI Init(
        /**[in]**/Int32 index, 
        /**[in]**/Boolean parameter, 
        /**[in]**/Boolean read,
        /**[in]**/Int32 dataSize, 
        /**[in]**/Int32 transferSize);

    /**
     * Creates a DataTruncation. The Reason is set to "Data truncation", the
     * ErrorCode is set to the SQLException default value and other fields are
     * set to the values supplied on this method.
     *
     * @param index
     *            the Index value of the column value or parameter that was
     *            truncated
     * @param parameter
     *            true if it was a Parameter value that was truncated, false
     *            otherwise
     * @param read
     *            true if the truncation occurred on a read operation, false
     *            otherwise
     * @param dataSize
     *            the original size of the truncated data
     * @param transferSize
     *            the size of the data after truncation
     * @param cause
     *            the root reason for this DataTruncation
     *
     * @since 1.6
     */
//    DataTruncation(
//        /**[in]**/Int32 index, 
//        /**[in]**/Boolean parameter, 
//        /**[in]**/Boolean read,
//        /**[in]**/Int32 dataSize, 
//        /**[in]**/Int32 transferSize, 
//        /**[in]**/IThrowable* cause);

    /**
     * Gets the number of bytes of data that should have been read/written.
     *
     * @return the number of bytes that should have been read or written. The
     *         value is set to {@code -1} if the size is unknown.
     */
    CARAPI GetDataSize(
        /**[out]**/Int32* size);

    /**
     * Gets the index of the column or of the parameter that was truncated.
     *
     * @return the index number of the column or of the parameter.
     */
    CARAPI GetIndex(
        /**[out]**/Int32* index);

    /**
     * Gets whether the value truncated was a parameter value or a column value.
     *
     * @return {@code true} if the value truncated was a parameter value,
     *         {@code false} if it was a column value.
     */
    CARAPI GetParameter(
        /**[out]**/Boolean* result);

    /**
     * Gets whether the value was truncated on a read operation or a write
     * operation
     *
     * @return {@code true} if the value was truncated on a read operation,
     *         {@code false} otherwise.
     */
    CARAPI GetRead(
        /**[out]**/Boolean* result);

    /**
     * Gets the number of bytes of data that was actually read or written.
     *
     * @return the number of bytes actually read/written. The value may be set
     *         to {@code -1} if the size is unknown.
     */
    CARAPI GetTransferSize(
        /**[out]**/Int32* size);
private:
    static const Int64 serialVersionUID;// = 6464298989504059473L;

    Int32 mIndex = 0;

    Boolean mParameter = false;

    Boolean mRead = false;

    Int32 mDataSize = 0;

    Int32 mTransferSize = 0;

    static const String THE_REASON;// = "Data truncation";

    static const String THE_SQLSTATE_READ;// = "01004";

    static const String THE_SQLSTATE_WRITE;// = "22001";

    static const Int32 THE_ERROR_CODE;// = 0;
};
#endif//_DATATRUNCATION_H_