
#ifndef __ELSTRINGBYTESINK_H__
#define __ELSTRINGBYTESINK_H__

#include <elastos.h>
#include <unicode/bytestream.h>

using namespace Elastos;

/**
 * Implementation of ByteSink that writes to a "string".
 * The StringClass is usually instantiated with a std::string.
 * @stable ICU 4.2
 */
class ElStringByteSink : public U_ICU_NAMESPACE::ByteSink
{
public:
    /**
     * Constructs a ByteSink that will append bytes to the dest string.
     * @param dest pointer to string object to append to
     * @stable ICU 4.2
     */
    ElStringByteSink(
        /* [in] */ String* dest)
        : dest_(dest)
    { }

    /**
     * Append "bytes[0,n-1]" to this.
     * @param data the pointer to the bytes
     * @param n the number of bytes; must be non-negative
     * @stable ICU 4.2
     */
    virtual void Append(
        /* [in] */ const char* data,
        /* [in] */ int32_t n)
    { dest_->Append(data, n); }

private:
    String* dest_;
    ElStringByteSink(); ///< default constructor not implemented
    ElStringByteSink(const ElStringByteSink &); ///< copy constructor not implemented
    ElStringByteSink &operator=(const ElStringByteSink &); ///< assignment operator not implemented
};

#endif //__ELSTRINGBYTESINK_H__
