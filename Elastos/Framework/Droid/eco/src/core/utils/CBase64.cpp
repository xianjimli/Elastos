
#include "ext/frameworkext.h"
#include "utils/CBase64.h"

//  --------------------------------------------------------
//  shared code
//  --------------------------------------------------------

/* package */ class Coder
{
public:

    Byte* output;
    Int32 op;

    Coder()
    {
        output = NULL;
    }

    virtual ~Coder()
    {
        if (!output) {
            free((Void*)output);
        }
    }

    /**
     * Encode/decode another block of input data.  this->output is
     * provided by the caller, and must be big enough to hold all
     * the coded data.  On exit, this->opwill be set to the length
     * of the coded data.
     *
     * @param finish true if this is the final call to process for
     *        this object.  Will finalize the coder state and
     *        include any final bytes in the output.
     *
     * @return true if the input so far is good; false if some
     *         error has been detected in the input stream..
     */
    virtual Boolean Process(
        Byte* input, Int32 offset, Int32 len, Boolean finish) = 0;

    /**
     * @return the maximum number of bytes a call to process()
     * could produce for the given number of input bytes.  This may
     * be an overestimate.
     */
    virtual Int32 MaxOutputSize(Int32 len) = 0;
};

/**
 * Lookup table for turning bytes into their position in the
 * Base64 alphabet.
 */
const Int32 DECODE[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

/**
 * Decode lookup table for the "web safe" variant (RFC 3548
 * sec. 4) where - and _ replace + and /.
 */
const Int32 DECODE_WEBSAFE[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

/** Non-data values in the DECODE arrays. */
const Int32 SKIP = -1;
const Int32 EQUALS = -2;

/* package */ class Decoder : public Coder
{
private:

    /**
     * States 0-3 are reading through the next input tuple.
     * State 4 is having read one '=' and expecting exactly
     * one more.
     * State 5 is expecting no more data or padding characters
     * in the input.
     * State 6 is the error state; an error has been detected
     * in the input and no future input can "fix" it.
     */
    Int32 state;   // state number (0 to 6)
    Int32 value;

    const Int32* alphabet;

public:

    Decoder(Int32 flags, Byte* output)
    {
        this->output = output;

        alphabet = ((flags & Base64_URL_SAFE) == 0) ? DECODE : DECODE_WEBSAFE;
        state = 0;
        value = 0;
    }

    /**
     * @return an overestimate for the number of bytes {@code
     * len} bytes could decode to.
     */
    virtual Int32 MaxOutputSize(Int32 len)
    {
        return len * 3 / 4 + 10;
    }

    /**
     * Decode another block of input data.
     *
     * @return true if the state machine is still healthy.  false if
     *         bad base-64 data has been detected in the input stream.
     */
    virtual Boolean Process(
        Byte* input, Int32 offset, Int32 len, Boolean finish)
    {
        if (this->state == 6) return FALSE;

        Int32 p = offset;
        len += offset;

        // Using local variables makes the decoder about 12%
        // faster than if we manipulate the member variables in
        // the loop.  (Even alphabet makes a measurable
        // difference, which is somewhat surprising to me since
        // the member variable is final.)
        Int32 state = this->state;
        Int32 value = this->value;
        Int32 op = 0;
        Byte* output = this->output;
        const Int32* alphabet = this->alphabet;

        while (p < len) {
            // Try the fast path:  we're starting a new tuple and the
            // next four bytes of the input stream are all data
            // bytes.  This corresponds to going through states
            // 0-1-2-3-0.  We expect to use this method for most of
            // the data.
            //
            // If any of the next four bytes of input are non-data
            // (whitespace, etc.), value will end up negative.  (All
            // the non-data values in decode are small negative
            // numbers, so shifting any of them up and or'ing them
            // together will result in a value with its top bit set.)
            //
            // You can remove this whole block and the output should
            // be the same, just slower.
            if (state == 0) {
                while (p + 4 <= len &&
                       (value = ((alphabet[input[p] & 0xff] << 18) |
                                 (alphabet[input[p + 1] & 0xff] << 12) |
                                 (alphabet[input[p + 2] & 0xff] << 6) |
                                 (alphabet[input[p + 3] & 0xff]))) >= 0) {
                    output[op + 2] = (Byte)value;
                    output[op + 1] = (Byte)(value >> 8);
                    output[op] = (Byte)(value >> 16);
                    op += 3;
                    p += 4;
                }
                if (p >= len) break;
            }

            // The fast path isn't available -- either we've read a
            // partial tuple, or the next four input bytes aren't all
            // data, or whatever.  Fall back to the slower state
            // machine implementation.

            Int32 d = alphabet[input[p++] & 0xff];

            switch (state) {
            case 0:
                if (d >= 0) {
                    value = d;
                    ++state;
                } else if (d != SKIP) {
                    this->state = 6;
                    return FALSE;
                }
                break;

            case 1:
                if (d >= 0) {
                    value = (value << 6) | d;
                    ++state;
                } else if (d != SKIP) {
                    this->state = 6;
                    return FALSE;
                }
                break;

            case 2:
                if (d >= 0) {
                    value = (value << 6) | d;
                    ++state;
                } else if (d == EQUALS) {
                    // Emit the last (partial) output tuple;
                    // expect exactly one more padding character.
                    output[op++] = (Byte)(value >> 4);
                    state = 4;
                } else if (d != SKIP) {
                    this->state = 6;
                    return FALSE;
                }
                break;

            case 3:
                if (d >= 0) {
                    // Emit the output triple and return to state 0.
                    value = (value << 6) | d;
                    output[op + 2] = (Byte)value;
                    output[op + 1] = (Byte)(value >> 8);
                    output[op] = (Byte)(value >> 16);
                    op += 3;
                    state = 0;
                } else if (d == EQUALS) {
                    // Emit the last (partial) output tuple;
                    // expect no further data or padding characters.
                    output[op + 1] = (Byte)(value >> 2);
                    output[op] = (Byte)(value >> 10);
                    op += 2;
                    state = 5;
                } else if (d != SKIP) {
                    this->state = 6;
                    return FALSE;
                }
                break;

            case 4:
                if (d == EQUALS) {
                    ++state;
                } else if (d != SKIP) {
                    this->state = 6;
                    return FALSE;
                }
                break;

            case 5:
                if (d != SKIP) {
                    this->state = 6;
                    return FALSE;
                }
                break;
            }
        }

        if (!finish) {
            // We're out of input, but a future call could provide
            // more.
            this->state = state;
            this->value = value;
            this->op = op;
            return TRUE;
        }

        // Done reading input.  Now figure out where we are left in
        // the state machine and finish up.

        switch (state) {
        case 0:
            // Output length is a multiple of three.  Fine.
            break;
        case 1:
            // Read one extra input byte, which isn't enough to
            // make another output byte.  Illegal.
            this->state = 6;
            return FALSE;
        case 2:
            // Read two extra input bytes, enough to emit 1 more
            // output byte.  Fine.
            output[op++] = (Byte)(value >> 4);
            break;
        case 3:
            // Read three extra input bytes, enough to emit 2 more
            // output bytes.  Fine.
            output[op++] = (Byte)(value >> 10);
            output[op++] = (Byte)(value >> 2);
            break;
        case 4:
            // Read one padding '=' when we expected 2.  Illegal.
            this->state = 6;
            return FALSE;
        case 5:
            // Read all the padding '='s we expected and no more.
            // Fine.
            break;
        }

        this->state = state;
        this->op = op;

        return TRUE;
    }
};

/**
 * Emit a new line every this many output tuples.  Corresponds to
 * a 76-character line length (the maximum allowable according to
 * <a href="http://www.ietf.org/rfc/rfc2045.txt">RFC 2045</a>).
 */
const Int32 LINE_GROUPS = 19;

/**
 * Lookup table for turning Base64 alphabet positions (6 bits)
 * into output bytes.
 */
const Byte ENCODE[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

/**
 * Lookup table for turning Base64 alphabet positions (6 bits)
 * into output bytes.
 */
const Byte ENCODE_WEBSAFE[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_',
};

/* package */ class Encoder : public Coder
{
private:

    Byte tail[2];
    /* package */ Int32 tailLen;
    Int32 count;
    const Byte* alphabet;

public:

    Boolean do_padding;
    Boolean do_newline;
    Boolean do_cr;

    Encoder(Int32 flags, Byte* output)
    {
        this->output = output;

        do_padding = (flags & Base64_NO_PADDING) == 0;
        do_newline = (flags & Base64_NO_WRAP) == 0;
        do_cr = (flags & Base64_CRLF) != 0;
        alphabet = ((flags & Base64_URL_SAFE) == 0) ? ENCODE : ENCODE_WEBSAFE;

        // tail = new byte[2];
        tailLen = 0;

        count = do_newline ? LINE_GROUPS : -1;
    }

    /**
     * @return an overestimate for the number of bytes {@code
     * len} bytes could encode to.
     */
    virtual Int32 MaxOutputSize(Int32 len)
    {
        return len * 8 / 5 + 10;
    }

    virtual Boolean Process(
        Byte* input, Int32 offset, Int32 len, Boolean finish)
    {
        // Using local variables makes the encoder about 9% faster.
        const Byte* alphabet = this->alphabet;
        Byte* output = this->output;
        Int32 op = 0;
        Int32 count = this->count;

        Int32 p = offset;
        len += offset;
        Int32 v = -1;

        // First we need to concatenate the tail of the previous call
        // with any input bytes available now and see if we can empty
        // the tail.

        switch (tailLen) {
            case 0:
                // There was no tail.
                break;

            case 1:
                if (p + 2 <= len) {
                    // A 1-byte tail with at least 2 bytes of
                    // input available now.
                    v = ((tail[0] & 0xff) << 16) |
                        ((input[p++] & 0xff) << 8) |
                        (input[p++] & 0xff);
                    tailLen = 0;
                };
                break;

            case 2:
                if (p + 1 <= len) {
                    // A 2-byte tail with at least 1 byte of input.
                    v = ((tail[0] & 0xff) << 16) |
                        ((tail[1] & 0xff) << 8) |
                        (input[p++] & 0xff);
                    tailLen = 0;
                }
                break;
        }

        if (v != -1) {
            output[op++] = alphabet[(v >> 18) & 0x3f];
            output[op++] = alphabet[(v >> 12) & 0x3f];
            output[op++] = alphabet[(v >> 6) & 0x3f];
            output[op++] = alphabet[v & 0x3f];
            if (--count == 0) {
                if (do_cr) output[op++] = '\r';
                output[op++] = '\n';
                count = LINE_GROUPS;
            }
        }

        // At this point either there is no tail, or there are fewer
        // than 3 bytes of input available.

        // The main loop, turning 3 input bytes into 4 output bytes on
        // each iteration.
        while (p + 3 <= len) {
            v = ((input[p] & 0xff) << 16) |
                ((input[p + 1] & 0xff) << 8) |
                (input[p + 2] & 0xff);
            output[op] = alphabet[(v >> 18) & 0x3f];
            output[op + 1] = alphabet[(v >> 12) & 0x3f];
            output[op + 2] = alphabet[(v >> 6) & 0x3f];
            output[op + 3] = alphabet[v & 0x3f];
            p += 3;
            op += 4;
            if (--count == 0) {
                if (do_cr) output[op++] = '\r';
                output[op++] = '\n';
                count = LINE_GROUPS;
            }
        }

        if (finish) {
            // Finish up the tail of the input.  Note that we need to
            // consume any bytes in tail before any bytes
            // remaining in input; there should be at most two bytes
            // total.

            if (p - tailLen == len - 1) {
                Int32 t = 0;
                v = ((tailLen > 0 ? tail[t++] : input[p++]) & 0xff) << 4;
                tailLen -= t;
                output[op++] = alphabet[(v >> 6) & 0x3f];
                output[op++] = alphabet[v & 0x3f];
                if (do_padding) {
                    output[op++] = '=';
                    output[op++] = '=';
                }
                if (do_newline) {
                    if (do_cr) output[op++] = '\r';
                    output[op++] = '\n';
                }
            } else if (p - tailLen == len - 2) {
                Int32 t = 0;
                v = (((tailLen > 1 ? tail[t++] : input[p++]) & 0xff) << 10) |
                    (((tailLen > 0 ? tail[t++] : input[p++]) & 0xff) << 2);
                tailLen -= t;
                output[op++] = alphabet[(v >> 12) & 0x3f];
                output[op++] = alphabet[(v >> 6) & 0x3f];
                output[op++] = alphabet[v & 0x3f];
                if (do_padding) {
                    output[op++] = '=';
                }
                if (do_newline) {
                    if (do_cr) output[op++] = '\r';
                    output[op++] = '\n';
                }
            } else if (do_newline && op > 0 && count != LINE_GROUPS) {
                if (do_cr) output[op++] = '\r';
                output[op++] = '\n';
            }

            assert(tailLen == 0);
            assert(p == len);
        } else {
            // Save the leftovers in tail to be consumed on the next
            // call to encodeInternal.

            if (p == len - 1) {
                tail[tailLen++] = input[p];
            } else if (p == len - 2) {
                tail[tailLen++] = input[p];
                tail[tailLen++] = input[p+1];
            }
        }

        this->op = op;
        this->count = count;

        return TRUE;
    }
};

ECode CBase64::Decode(
    /* [in] */ const String& str,
    /* [in] */ Int32 flags,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    Int32 length = str.GetLength();
    ArrayOf<Byte> input((Byte*)str.string(), length);

    return Decode2(input, flags, result);
}

ECode CBase64::Decode2(
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 flags,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return Decode3(input, 0, input.GetLength(), flags, result);
}

ECode CBase64::Decode3(
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [in] */ Int32 flags,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    // Allocate space for the most data the input could represent.
    // (It could contain less if it contains whitespace, etc.)
    Int32 outputLength = sizeof(Byte) * len * 3 / 4;
    Byte* buffer = (Byte*)malloc(outputLength);
    if (!buffer) {
        return E_OUT_OF_MEMORY;
    }

    Decoder* decoder = new Decoder(flags, buffer);
    if (!decoder) {
        free((Void*)buffer);
        return E_OUT_OF_MEMORY;
    }

    if (!decoder->Process(input.GetPayload(), offset, len, TRUE)) {
        delete decoder;
        // throw new IllegalArgumentException("bad base-64");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Maybe we got lucky and allocated exactly enough output space.
    // if (decoder->op == outputLength) {
    //     return decoder->output;
    // }

    // Need to shorten the array, so allocate a new one of the
    // right size and copy.
    ArrayOf<Byte>* temp = ArrayOf<Byte>::Alloc(decoder->op);
    if (!temp) {
        delete decoder;
        return E_OUT_OF_MEMORY;
    }

    memcpy(temp->GetPayload(), decoder->output, sizeof(Byte) * decoder->op);

    delete decoder;
    *result = temp;

    return NOERROR;
}

ECode CBase64::EncodeToString(
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 flags,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    // try {
    ArrayOf<Byte>* data;
    FAIL_RETURN(Encode(input, flags, &data));
    FAIL_RETURN(result->SetTo(
        (const char*)data->GetPayload(), (UInt32)data->GetLength()));

    return NOERROR;
    // } catch (UnsupportedEncodingException e) {
    //     // US-ASCII is guaranteed to be available.
    //     throw new AssertionError(e);
    // }
}

ECode CBase64::EncodeToString2(
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [in] */ Int32 flags,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    // try {
    ArrayOf<Byte>* data;
    FAIL_RETURN(Encode2(input, offset, len, flags, &data));
    FAIL_RETURN(result->SetTo(
        (const char*)data->GetPayload(), (UInt32)data->GetLength()));

    return NOERROR;
    // } catch (UnsupportedEncodingException e) {
    //     // US-ASCII is guaranteed to be available.
    //     throw new AssertionError(e);
    // }
}

ECode CBase64::Encode(
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 flags,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return Encode2(input, 0, input.GetLength(), flags, result);
}

ECode CBase64::Encode2(
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [in] */ Int32 flags,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    Encoder* encoder = new Encoder(flags, NULL);
    if (!encoder) {
        return E_OUT_OF_MEMORY;
    }

    // Compute the exact length of the array we will produce.
    Int32 output_len = len / 3 * 4;

    // Account for the tail of the data and the padding bytes, if any.
    if (encoder->do_padding) {
        if (len % 3 > 0) {
            output_len += 4;
        }
    } else {
        switch (len % 3) {
            case 0: break;
            case 1: output_len += 2; break;
            case 2: output_len += 3; break;
        }
    }

    // Account for the newlines, if any.
    if (encoder->do_newline && len > 0) {
        output_len += (((len - 1) / (3 * LINE_GROUPS)) + 1) *
            (encoder->do_cr ? 2 : 1);
    }

    encoder->output = (Byte*)malloc(output_len);
    if (!encoder->output) {
        delete encoder;
        return E_OUT_OF_MEMORY;
    }

    encoder->Process(input.GetPayload(), offset, len, TRUE);
    assert(encoder->op == output_len);

    ArrayOf<Byte>* temp = ArrayOf<Byte>::Alloc(output_len);
    if (!temp) {
        delete encoder;
        return E_OUT_OF_MEMORY;
    }

    memcpy(temp->GetPayload(), encoder->output, sizeof(Byte) * output_len);

    delete encoder;
    *result = temp;

    return NOERROR;
}

