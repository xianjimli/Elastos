
#ifndef __CIDNHELPER_H__
#define __CIDNHELPER_H__

#include "_CIDNHelper.h"

CarClass(CIDNHelper)
{
public:
    /**
     * Transform a Unicode String to ASCII Compatible Encoding String according
     * to the algorithm defined in RFC 3490.
     *
     * <p>If the transformation fails (because the input is not a valid IDN), an
     * exception will be thrown.
     *
     * <p>This method can handle either an individual label or an entire domain name.
     * In the latter case, the separators are: U+002E (full stop), U+3002 (ideographic full stop),
     * U+FF0E (fullwidth full stop), and U+FF61 (halfwidth ideographic full stop).
     * All of these will become U+002E (full stop) in the result.
     *
     * @param input the Unicode name
     * @param flags 0, {@code ALLOW_UNASSIGNED}, {@code USE_STD3_ASCII_RULES},
     *         or {@code ALLOW_UNASSIGNED | USE_STD3_ASCII_RULES}
     * @return the ACE name
     * @throws IllegalArgumentException if {@code input} does not conform to RFC 3490
     */
    CARAPI ToASCIIEx(
        /* [in] */ const String& input,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    /**
     * Equivalent to {@code toASCII(input, 0)}.
     *
     * @param input the Unicode name
     * @return the ACE name
     * @throws IllegalArgumentException if {@code input} does not conform to RFC 3490
     */
    CARAPI ToASCII(
        /* [in] */ const String& input,
        /* [out] */ String* result);

     /**
     * Translates a string from ASCII Compatible Encoding (ACE) to Unicode
     * according to the algorithm defined in RFC 3490.
     *
     * <p>Unlike {@code toASCII}, this transformation cannot fail.
     *
     * <p>This method can handle either an individual label or an entire domain name.
     * In the latter case, the separators are: U+002E (full stop), U+3002 (ideographic full stop),
     * U+FF0E (fullwidth full stop), and U+FF61 (halfwidth ideographic full stop).
     *
     * @param input the ACE name
     * @return the Unicode name
     * @param flags 0, {@code ALLOW_UNASSIGNED}, {@code USE_STD3_ASCII_RULES},
     *         or {@code ALLOW_UNASSIGNED | USE_STD3_ASCII_RULES}
     */
    CARAPI ToUnicodeEx(
        /* [in] */ const String& input,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    /**
     * Equivalent to {@code toUnicode(input, 0)}.
     *
     * @param input the ACE name
     * @return the Unicode name
     */
    CARAPI ToUnicode(
        /* [in] */ const String& input,
        /* [out] */ String* result);
};

#endif //__CIDNHELPER_H__
