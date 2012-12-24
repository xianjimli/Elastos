#ifdef _SQLXML_H_
#define _SQLXML_H_

#include <elastos.h>

using namespace Elastos;

class SQLXML {
protected:
	/**
     * Frees any resources held by this object. After {@code free} is called, calling
     * method other than {@code free} will throw {@code SQLException} (calling {@code free}
     * repeatedly will do nothing).
     * @throws SQLException
     */
    Free()=0;

    /**
     * Returns a stream that can be used to read binary data from this SQL {@code XML} object.
     * @throws SQLException if an error occurs accessing the data
     */
    GetBinaryStream(
        /** [out] **/IInputStream** stream)=0;

    /**
     * Returns a stream that can be used to write binary data to this SQL {@code XML} object.
     * @throws SQLException if an error occurs accessing the data
     */
    SetBinaryStream(
        /** [out] **/IOutputStream** stream)=0;

    /**
     * Returns a reader that can be used to read character data from this SQL {@code XML} object.
     * @throws SQLException if an error occurs accessing the data
     */
    GetCharacterStream(
        /** [out] **/IReader** stream)=0;

    /**
     * Returns a writer that can be used to write character data to this SQL {@code XML} object.
     * @throws SQLException if an error occurs accessing the data
     */
    SetCharacterStream(
        /** [out] **/IWriter** stream)=0;

    /**
     * Returns this object's data as an XML string.
     * @throws SQLException if an error occurs accessing the data
     */
    GetString(
        /** [out] **/String* str)=0;

    /**
     * Sets this object's data to the given XML string.
     * @throws SQLException if an error occurs accessing the data
     */
    SetString(
        /** [out] **/String* str)=0;

    /**
     * Returns a {@link Source} for reading this object's data.
     * @throws SQLException if an error occurs accessing the data
     */
//    <T extends Source> T GetSource(Class<T> sourceClass)=0;

    /**
     * Returns a {@link Result} for writing this object's data.
     * @throws SQLException if an error occurs accessing the data
     */
//    <T extends Result> T SetResult(Class<T> resultClass)=0;
};
#endif//_SQLXML_H_