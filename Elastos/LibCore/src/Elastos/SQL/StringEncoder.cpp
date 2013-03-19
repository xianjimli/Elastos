#include "StringEncoder.h"

ECode StringEncoder::Encode(
    /** [in] **/const ArrayOf<Byte>& a,
    /** [out] **/String* str)
{/*
    // check input
    if (a == null || a.length == 0) {
        // bogus shift, no data
        return "x";
    }
    // determine count
    int[] cnt = new int[256];
    for (int i = 0 ; i < a.length; i++) {
        cnt[a[i] & 0xff]++;
    }
    // determine shift for minimum number of escapes
    int shift = 1;
    int nEscapes = a.length;
    for (int i = 1; i < 256; i++) {
        if (i == '\'') {
        continue;
        }
        int sum = cnt[i] + cnt[(i + 1) & 0xff] + cnt[(i + '\'') & 0xff];
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
    int outLen = a.length + nEscapes + 1;
    StringBuffer out = new StringBuffer(outLen);
    out.append((char)shift);
    for (int i = 0; i < a.length; i++) {
        // apply shift
        char c = (char)((a[i] - shift)&0xff);
        // insert escapes
        if (c == 0) { // forbidden
        out.append((char)1);
        out.append((char)1);
        } else if (c == 1) { // escape character
        out.append((char)1);
        out.append((char)2);
        } else if (c == '\'') { // forbidden
        out.append((char)1);
        out.append((char)3);
        } else {
        out.append(c);
        }
    }
    return out.toString();*/
    return NOERROR;
}

ECode StringEncoder::Decode(
    /** [in] **/String s,
    /** [out] **/ArrayOf<Byte>* code)
{/*
    char[] a = s.toCharArray();
    if (a.length > 2 && a[0] == 'X' &&
        a[1] == '\'' && a[a.length-1] == '\'') {
        // SQLite3 BLOB syntax
        byte[] result = new byte[(a.length-3)/2];
        for (int i = 2, k = 0; i < a.length - 1; i += 2, k++) {
        byte tmp;
        switch (a[i]) {
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
        result[k] = (byte) (tmp << 4);
        switch (a[i+1]) {
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
        result[k] |= tmp;
        }
        return result;
    }
    // first element is the shift
    byte[] result = new byte[a.length-1];
    int i = 0;
    int shift = s.charAt(i++);
    int j = 0;
    while (i < s.length()) {
        int c;
        if ((c = s.charAt(i++)) == 1) { // escape character found
        if ((c = s.charAt(i++)) == 1) {
            c = 0;
        } else if (c == 2) {
            c = 1;
        } else if (c == 3) {
            c = '\'';
        } else {
            throw new IllegalArgumentException(
            "invalid string passed to decoder: " + j);
        }
        }
        // do shift
        result[j++] = (byte)((c + shift) & 0xff);
    }
    int outLen = j;
    // provide array of correct length
    if (result.length != outLen) {
        result = byteCopy(result, 0, outLen, new byte[outLen]);
    }
    return result;*/
    return NOERROR;
}


ECode StringEncoder::EncodeX(
    /** [in] **/const ArrayOf<Byte>& a,
    /** [out] **/String* str)
{/*
    // check input
    if (a == null || a.length == 0) {
        return "X''";
    }
    int outLen = a.length * 2 + 3;
    StringBuffer out = new StringBuffer(outLen);
    out.append('X');
    out.append('\'');
    for (int i = 0; i < a.length; i++) {
        out.append(xdigits[(a[i] >> 4) & 0x0F]);
        out.append(xdigits[a[i] & 0x0F]);
    }
    out.append('\'');
    return out.toString();*/

    return NOERROR;
}

ECode StringEncoder::ByteCopy(
    /** [in] **/const ArrayOf<Byte>& source, 
    /** [in] **/Int32 offset,
    /** [in] **/Int32 count, 
    /** [in] **/const ArrayOf<Byte>& target,
    /** [out] **/ArrayOf<Byte>* code)
{/*
    for (int i = offset, j = 0; i < offset + count; i++, j++) {
        target[j] = source[i];
    }
    return target;*/
    return NOERROR;
}