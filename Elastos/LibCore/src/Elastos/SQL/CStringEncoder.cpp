#include "CStringEncoder.h"
#include <StringBuffer.h>

static const Char8 mXdigits[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

ECode CStringEncoder::Encode(
    /** [in] **/const ArrayOf<Byte>& a,
    /** [out] **/String* str)
{
    // check input
    if (a.GetPayload() == NULL || a.GetLength() == 0) {
        // bogus shift, no data
        *str = String("x");
        return NOERROR;
    }
    // determine count
    ArrayOf<Int32> *cnt = ArrayOf<Int32>::Alloc(256);
    for (Int32 i = 0 ; i < a.GetLength(); i++) {
        (*cnt)[a[i] & 0xff]++;
    }
    // determine shift for minimum number of escapes
    Int32 shift = 1;
    Int32 nEscapes = a.GetLength();
    for (Int32 i = 1; i < 256; i++) {
        if (i == '\'') {
            continue;
        }
        Int32 sum = (*cnt)[i] + (*cnt)[(i + 1) & 0xff] + (*cnt)[(i + '\'') & 0xff];
        if (sum < nEscapes) {
        nEscapes = sum;
        shift = i;
        if (nEscapes == 0) {
            // cannot become smaller
            break;
        }
        }
    }
    // construct encoded output
    Int32 outLen = a.GetLength() + nEscapes + 1;
    StringBuffer out(outLen);

    out += ((Char8)shift);
    for (Int32 i = 0; i < a.GetLength(); i++) {
        // apply shift
        Char8 c = (Char8)((a[i] - shift)&0xff);
        // insert escapes
        if (c == 0) { // forbidden
        out += ((Char8)1);
        out += ((Char8)1);
        } else if (c == 1) { // escape character
        out += ((Char8)1);
        out += ((Char8)2);
        } else if (c == '\'') { // forbidden
        out += ((Char8)1);
        out += ((Char8)3);
        } else {
        out += (c);
        }
    }
    ArrayOf<Int32>::Free(cnt);
    *str = String(out);

    return NOERROR;
}

ECode CStringEncoder::Decode(
    /** [in] **/const String& s,
    /** [out,callee] **/ArrayOf<Byte>** code)
{
    //char[] a = s.toCharArray();
    ArrayOf<Char8> *a = ArrayOf<Char8>::Alloc(s.GetLength());
    for(UInt32 i=0;i<s.GetLength();i++){
        (*a)[i] = s.GetChar(i);
    }

    if (a->GetLength() > 2 && (*a)[0] == 'X' &&
        (*a)[1] == '\'' && (*a)[a->GetLength()-1] == '\'') {
        // SQLite3 BLOB syntax
        //byte[] result = new byte[(a.length-3)/2];
        ArrayOf<Byte> *result = ArrayOf<Byte>::Alloc((a->GetLength()-3)/2);
        for (Int32 i = 2, k = 0; i < a->GetLength() - 1; i += 2, k++) {
        Byte tmp;
        switch ((*a)[i]) {
        case '0': tmp = 0; break;
        case '1': tmp = 1; break;
        case '2': tmp = 2; break;
        case '3': tmp = 3; break;
        case '4': tmp = 4; break;
        case '5': tmp = 5; break;
        case '6': tmp = 6; break;
        case '7': tmp = 7; break;
        case '8': tmp = 8; break;
        case '9': tmp = 9; break;
        case 'A':
        case 'a': tmp = 10; break;
        case 'B':
        case 'b': tmp = 11; break;
        case 'C':
        case 'c': tmp = 12; break;
        case 'D':
        case 'd': tmp = 13; break;
        case 'E':
        case 'e': tmp = 14; break;
        case 'F':
        case 'f': tmp = 15; break;
        default:  tmp = 0; break;
        }
        (*result)[k] = (Byte) (tmp << 4);
        switch ((*a)[i+1]) {
        case '0': tmp = 0; break;
        case '1': tmp = 1; break;
        case '2': tmp = 2; break;
        case '3': tmp = 3; break;
        case '4': tmp = 4; break;
        case '5': tmp = 5; break;
        case '6': tmp = 6; break;
        case '7': tmp = 7; break;
        case '8': tmp = 8; break;
        case '9': tmp = 9; break;
        case 'A':
        case 'a': tmp = 10; break;
        case 'B':
        case 'b': tmp = 11; break;
        case 'C':
        case 'c': tmp = 12; break;
        case 'D':
        case 'd': tmp = 13; break;
        case 'E':
        case 'e': tmp = 14; break;
        case 'F':
        case 'f': tmp = 15; break;
        default:  tmp = 0; break;
        }
        (*result)[k] |= tmp;
        }
        *code = result;
        return NOERROR;
    }
    // first element is the shift
    //byte[] result = new byte[a.length-1];
    ArrayOf<Byte> *result = ArrayOf<Byte>::Alloc(a->GetLength()-1);
    UInt32 i = 0;
    Int32 shift = s.GetChar(i++);
    Int32 j = 0;
    while (i < s.GetLength()) {
        Int32 c;
        if ((c = s.GetChar(i++)) == 1) { // escape character found
        if ((c = s.GetChar(i++)) == 1) {
            c = 0;
        } else if (c == 2) {
            c = 1;
        } else if (c == 3) {
            c = '\'';
        } else {
            //throw new IllegalArgumentException(
            //"invalid string passed to decoder: " + j);
            return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        }
        // do shift
        (*result)[j++] = (Byte)((c + shift) & 0xff);
    }
    Int32 outLen = j;
    // provide array of correct length
    if (result->GetLength() != outLen) {
        ByteCopy(*result, 0, outLen, ArrayOf<Byte>::Alloc(outLen), result);
    }
    *code = result;
    return NOERROR;
}


ECode CStringEncoder::EncodeX(
    /** [in] **/const ArrayOf<Byte>& a,
    /** [out] **/String* str)
{
    // check input
    if (a.GetPayload() == NULL || a.GetLength() == 0) {
        *str = String("X''");
        return NOERROR;
    }
    Int32 outLen = a.GetLength() * 2 + 3;
    //StringBuffer out = new StringBuffer(outLen);
    StringBuffer out(outLen);
    out += ('X');
    out += ('\'');
    for (Int32 i = 0; i < a.GetLength(); i++) {
        out += (mXdigits[(a[i] >> 4) & 0x0F]);
        out += (mXdigits[a[i] & 0x0F]);
    }
    out += ('\'');
    *str = String(out);

    return NOERROR;
}

ECode CStringEncoder::ByteCopy(
    /** [in] **/const ArrayOf<Byte>& source, 
    /** [in] **/Int32 offset,
    /** [in] **/Int32 count, 
    /** [in] **/ArrayOf<Byte>* target,
    /** [out] **/ArrayOf<Byte>* code)
{
    for(Int32 i = offset, j = 0; i < offset + count; i++, j++) {
        (*target)[j] = source[i];
    }
    ArrayOf<Byte>::Free(code);
    code = target;
    return NOERROR;
}