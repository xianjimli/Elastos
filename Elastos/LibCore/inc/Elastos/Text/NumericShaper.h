#ifndef __NUMERICSHAPER_H__
#define __NUMERICSHAPER_H__

#include <elastos.h>
#include "Elastos.Text_server.h"

using namespace Elastos;

class NumericShaper
{
public:
    NumericShaper();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID id);

    virtual ~NumericShaper();

    /**
     * Gets the NumericShaper for the specified unicode ranges
     * and default unicode range. The defaultContext parameter
     * is used as the starting context (which indicates the
     * language/script being used). The OR logical operation
     * should be used for multiple ranges:
     * NumericShaper.DEVANAGARI | NumericShaper.BENGALI.
     * The NumericShaper returned by this method is contextual
     * in that it supports multiple character ranges, depending
     * on the context.
     *
     * @param ranges the unicode ranges.
     * @param defaultContext the default, starting context.
     *
     * @return the NumericShaper for the specified ranges.
     */
    static CARAPI_(INumericShaper*) GetContextualShaper(
        /* [in] */ Int32 ranges,
        /* [in] */ Int32 defaultContext);

    /**
     * Gets the NumericShaper for the specified unicode ranges.
     * The OR logical operation should be used for multiple ranges:
     * NumericShaper.DEVANAGARI | NumericShaper.BENGALI.
     * The NumericShaper returned by this method is contextual
     * in that it supports multiple character ranges, depending
     * on the context.
     *
     * @param ranges the unicode ranges.
     *
     * @return the NumericShaper for the specified ranges.
     */
    static CARAPI_(INumericShaper*) GetContextualShaper(
        /* [in] */ Int32 ranges);

    /**
     * Gets the masks for all of the ranges supported by this NumericShaper,
     * packed into an int value using the logical OR logical operation
     * for multiple ranges:
     * NumericShaper.DEVANAGARI | NumericShaper.BENGALI.
     *
     * @return all ranges of this NumericShaper.
     */
    CARAPI GetRanges(
        /* [out] */ Int32 * ranges);

    /**
     * Gets a NumericShaper for the specified unicode range.
     * The NumericShaper supports only a single range and
     * hence is not contextual.
     *
     * @param singleRange the specified unicode single range.
     *
     * @return the NumericShaper for the specified unicode range.
     */
    static CARAPI_(INumericShaper*) GetShaper(
        /* [in] */ Int32 singleRange);

    /**
     * Checks if this NumericShaper is contextual (supporting
     * multiple script ranges) or not.
     *
     * @return true, if this NumericShaper is contextual, false otherwise.
     */
    CARAPI IsContextual(
        /* [out] */ Boolean * isContextual);

    /**
     * Transforms the encoding of the text, starting from the character
     * at index start and transforming count characters,
     * using the specified context.
     *
     * @param text the text to be shaped.
     * @param start the start offset of the text.
     * @param count the number of characters to be shaped.
     * @param context the context to be used for shaping.
     */
    CARAPI Shape(
        /* [in] */ ArrayOf<Char32> * text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 context);

    /**
     * Transforms the encoding of the text, starting from the character
     * at index start and transforming count characters.
     *
     * @param text the text to be shaped.
     * @param start the start offset of the text.
     * @param count the number of characters to be shaped.
     */
    CARAPI ShapeEx(
        /* [in] */ ArrayOf<Char32> * Text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    /**
     * Converts count of digits of the given array of characters from the start
     * index using specified context. This method is applied for the contextual
     * shaping, if the shaper instance is not contextual use nonContextualShape
     * method.
     *
     * @param text an array of chars
     * @param start index of the first character to convert
     * @param count a number of characters to convert
     * @param contextIndex index of the script index to use in shaper
     */
    CARAPI ContextualShape(
        /* [in] */ ArrayOf<Char32> * text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 contextIndex);

    /**
     * Converts count of digits of the given array of characters from the start
     * index. Method is applied for non-contextual shaper.
     *
     * @param text an array of chars
     * @param start index of the first character to convert
     * @param count a number of characters to convert
     */
    CARAPI NonContextualShape(
        /* [in] */ ArrayOf<Char32> * text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    /**
     * Returns the index of the script of the specified char.
     *
     * @param ch specified unicode character
     *
     * @return script index corresponding to the given char
     */
    CARAPI GetCharIndex(
        /* [in] */ Char32 ch,
        /* [out] */ Int32 * count);

    /**
     * Returns true if the bidirectional category of the character
     * is strong.
     *
     * @param chr the chr
     *
     * @return true, if the character is strong, false otherwise
     */
    CARAPI IsCharStrong(
        /* [in] */ Int32 chr,
        /* [out] */ Boolean * isCharStrong);

    /**
     * Updates all private serialized fields for object to be correctly serialized
     * according to the serialized form of this class mentioned in the
     * documentation.
     */
    CARAPI UpdateRangesFields();

    /**
     * Updates private fields for object after deserialization
     * according to the serialized form of this class mentioned in the
     * documentation.
     */
    CARAPI UpdateKeyMaskFields();

    /**
     * Write object.
     *
     * @param out the out
     *
     * @throws IOException Signals that an I/O exception has occurred.
     */
/*    private void writeObject(java.io.ObjectOutputStream out)
                                throws IOException{
        updateKeyMaskFields();
        out.defaultWriteObject();
    }*/

    /**
     * Read object.
     *
     * @param in the in
     *
     * @throws IOException Signals that an I/O exception has occurred.
     * @throws ClassNotFoundException the class not found exception
     */
/*    private void readObject(java.io.ObjectInputStream in)
                                throws IOException, ClassNotFoundException{
        in.defaultReadObject();
        updateRangesFields();
    }*/

private:
    /**
     * Creates NumericShaper with specified parameters.
     *
     * @param ranges specified ranges to be shaped
     * @param defaultContext default context range
     * @param isContextual specifies if the instance is contextual
     */
    NumericShaper(
        /* [in] */ Int32 ranges,
        /* [in] */ Int32 defaultContext,
        /* [in] */ Boolean isContextual);

    /**
     * Returns script index of the specified context range.
     *
     * @param range specified range
     *
     * @return one of the script indices according to the specified range.
     */
    CARAPI GetIndexFromRange(
        /* [in] */ Int32 range,
        /* [out] */ Int32 * index);

    /**
     * Returns range corresponding to the specified script index.
     *
     * @param index specified script index
     *
     * @return one of the range constants according to the specified script index.
     */
    CARAPI GetRangeFromIndex(
        /* [in] */ Int32 index,
        /* [out] */ Int32 * range);

    static CARAPI RangeException(
        /* [in] */ Int32 value);

public:
    /**
     * The Constant EUROPEAN indicates the latin and extended range,
     * and latin decimal base.
     */
    const static Int32 EUROPEAN = 1;

    /** The Constant ARABIC indicates the ARABIC range and decimal base. */
    const static Int32 ARABIC = 2;

    /**
     * The Constant EASTERN_ARABIC indicates the ARABIC range and
     * ARABIC_EXTENDED decimal base.
     */
    const static Int32 EASTERN_ARABIC = 4;

    /**
     * The Constant DEVANAGARI indicates the DEVANAGARI range and
     * decimal base.
     */
    const static Int32 DEVANAGARI = 8;

    /** The Constant BENGALI indicates the BENGALI range and decimal base. */
    const static Int32 BENGALI = 16;

    /** The Constant GURMUKHI indicates the GURMUKHI range and decimal base. */
    const static Int32 GURMUKHI = 32;

    /** The Constant GUJARATI indicates the GUJARATI range and decimal base. */
    const static Int32 GUJARATI = 64;

    /** The Constant ORIYA indicates the ORIYA range and decimal base. */
    const static Int32 ORIYA = 128;

    /** The Constant TAMIL indicates the TAMIL range and decimal base. */
    const static Int32 TAMIL = 256;

    /** The Constant TELUGU indicates the TELUGU range and decimal base. */
    const static Int32 TELUGU = 512;

    /** The Constant KANNADA indicates the KANNADA range and decimal base. */
    const static Int32 KANNADA = 1024;

    /**
     * The Constant MALAYALAM indicates the MALAYALAM range and decimal base.
     */
    const static Int32 MALAYALAM = 2048;

    /** The Constant THAI indicates the THAI range and decimal base. */
    const static Int32 THAI = 4096;

    /** The Constant LAO indicates the LAO range and decimal base. */
    const static Int32 LAO = 8192;

    /** The Constant TIBETAN indicates the TIBETAN range and decimal base. */
    const static Int32 TIBETAN = 16384;

    /** The Constant MYANMAR indicates the MYANMAR range and decimal base. */
    const static Int32 MYANMAR = 32768;

    /**
     * The Constant ETHIOPIC indicates the ETHIOPIC range and decimal base.
     */
    const static Int32 ETHIOPIC = 65536;

    /** The Constant KHMER indicates the KHMER range and decimal base. */
    const static Int32 KHMER = 131072;

    /**
     * The Constant MONGOLIAN indicates the MONGOLIAN range and
     * decimal base.
     */
    const static Int32 MONGOLIAN = 262144;

    /** The Constant ALL_RANGES indicates all ranges. */
    const static Int32 ALL_RANGES = 524287;

private:
    /* Further one can find the set of script indices.
     * Index is the power you need the 2 to raise to to get corresponding
     * range constant value. Also script ranges, context names and digits low
     * ranges are indexed with these indices.
     */

    // Index of the EUROPEAN range
    /** The Constant INDEX_EUROPEAN. */
    const static Int32 INDEX_EUROPEAN = 0;

    // Index of the ARABIC range
    /** The Constant INDEX_ARABIC. */
    const static Int32 INDEX_ARABIC = 1;

    // Index of the EASTERN_ARABIC range
    /** The Constant INDEX_EASTERN_ARABIC. */
    const static Int32 INDEX_EASTERN_ARABIC = 2;

    // Index of the DEVANAGARI range
    /** The Constant INDEX_DEVANAGARI. */
    const static Int32 INDEX_DEVANAGARI = 3;

    // Index of the BENGALI range
    /** The Constant INDEX_BENGALI. */
    const static Int32 INDEX_BENGALI = 4;

    // Index of the GURMUKHI range
    /** The Constant INDEX_GURMUKHI. */
    const static Int32 INDEX_GURMUKHI = 5;

    // Index of the GUJARTI range
    /** The Constant INDEX_GUJARATI. */
    const static Int32 INDEX_GUJARATI = 6;

    // Index of the ORIYA range
    /** The Constant INDEX_ORIYA. */
    const static Int32 INDEX_ORIYA = 7;

    // Index of the TAMIL range
    /** The Constant INDEX_TAMIL. */
    const static Int32 INDEX_TAMIL = 8;

    // Index of the TELUGU range
    /** The Constant INDEX_TELUGU. */
    const static Int32 INDEX_TELUGU = 9;

    // Index of the KANNADA range
    /** The Constant INDEX_KANNADA. */
    const static Int32 INDEX_KANNADA = 10;

    // Index of the MALAYALAM range
    /** The Constant INDEX_MALAYALAM. */
    const static Int32 INDEX_MALAYALAM = 11;

    // Index of the THAI range
    /** The Constant INDEX_THAI. */
    const static Int32 INDEX_THAI = 12;

    // Index of the LAO range
    /** The Constant INDEX_LAO. */
    const static Int32 INDEX_LAO = 13;

    // Index of the TIBETAN range
    /** The Constant INDEX_TIBETAN. */
    const static Int32 INDEX_TIBETAN = 14;

    // Index of the MYANMAR range
    /** The Constant INDEX_MYANMAR. */
    const static Int32 INDEX_MYANMAR = 15;

    // Index of the ETHIOPIC range
    /** The Constant INDEX_ETHIOPIC. */
    const static Int32 INDEX_ETHIOPIC = 16;

    // Index of the KHMER range
    /** The Constant INDEX_KHMER. */
    const static Int32 INDEX_KHMER = 17;

    // Index of the MONGOLIAN range
    /** The Constant INDEX_MONGOLIAN. */
    const static Int32 INDEX_MONGOLIAN = 18;

    // Maximum index that range can't exceed
    /** The Constant MAX_INDEX. */
    const static Int32 MAX_INDEX = 19;

    /*
     * Scripts ranges array. Array represents ranges as pairs of
     * lowest and highest range bounds.
     * Data is taken from the UnicodeData.txt file from
     * http://www.unicode.org/Public/UNIDATA/
     */
    /** The scripts ranges. */
    ArrayOf<Int32> * mScriptsRanges;/* = {
            0x0000, 0x024F,     // EUROPEAN (basic latin + latin-1 + extended)
            0x0600, 0x06FF,     // ARABIC
            0x0600, 0x06FF,     // EASTERN_ARABIC (XXX: diff with ARABIC ? )
            0x0900, 0x097F,     // DEVANAGARI
            0x0980, 0x09FF,     // BENGALI
            0x0A00, 0x0A7F,     // GURMUKHI
            0x0A80, 0x0AFF,     // GUJARATI
            0x0B00, 0x0B7F,     // ORIYA
            0x0B80, 0x0BFF,     // TAMIL
            0x0C00, 0x0C7F,     // TELUGU
            0x0C80, 0x0CFF,     // KANNADA
            0x0D00, 0x0D7F,     // MALAYALAM
            0x0E00, 0x0E7F,     // THAI
            0x0E80, 0x0EFF,     // LAO
            0x0F00, 0x0FFF,     // TIBETAN
            0x1000, 0x109F,     // MYANMAR
            0x1200, 0x137F,     // ETHIOPIC
            0x1780, 0x17FF,     // KHMER
            0x1800, 0x18AF      // MONGOLIAN
    };*/

    /*
     * Digit low ranges values decreased by 0x0030. Each low range
     * value decreased by 0x0030 for easy obtaing unicode value of the
     * context dependent digit. European digits starts from 0x0030 hence
     * context dependent unicode digit value equals to
     *      digitsLowRanges[script index] + european digit char unicode value.
     * !! the only exception is ETHIOPIC script where there is no '0' digit
     * Data is taken from the UnicodeData.txt file from
     * http://www.unicode.org/Public/UNIDATA/
     */
    /** The digits low ranges. */
    ArrayOf<Int32> * mDigitsLowRanges;/* = {
            0x0000,             // EUROPEAN
            0x0630,             // ARABIC
            0x0630,             // EASTERN_ARABIC
            0x0936,             // DEVANAGARI
            0x09B6,             // BENGALI
            0x0A36,             // GURMUKHI
            0x0AB6,             // GUJARATI
            0x0B36,             // ORIYA
            0x0BB6,             // TAMIL
            0x0C36,             // TELUGU
            0x0CB6,             // KANNADA
            0x0D36,             // MALAYALAM
            0x0E20,             // THAI
            0x0EA0,             // LAO
            0x0EF0,             // TIBETAN
            0x1010,             // MYANMAR
            0x1338,             // ETHIOPIC - (low range-1) no ETHIOPIC '0' DIGIT!
            0x17B0,             // KHMER
            0x17E0              // MONGOLIAN
    };*/

    // Set of context names used in toString method
    /** The contexts. */
    ArrayOf<String> * mContexts;/* = {
            "EUROPEAN",
            "ARABIC",
            "EASTERN_ARABIC",
            "DEVANAGARI",
            "BENGALI",
            "GURMUKHI",
            "GUJARATI",
            "ORIYA",
            "TAMIL",
            "TELUGU",
            "KANNADA",
            "MALAYALAM",
            "THAI",
            "LAO",
            "TIBETAN",
            "MYANMAR",
            "ETHIOPIC",
            "KHMER",
            "MONGOLIAN"
    };*/

    /*
     * Strong characters flags array is to determine if the
     * unicode bidirectional category of the character is strong,
     * according to Unicode specification. If the bit with index equals to
     * character's unicode value is 1 - the character is strong.
     * This array was generated using UnicodeData.txt file from
     * http://www.unicode.org/Public/UNIDATA/
     */

    /** The Constant STRONG_TEXT_FLAGS. */
    static ArrayOf<Int32> * STRONG_TEXT_FLAGS;/* = { 0, 0, 134217726, 134217726,
            0, 69207040, -8388609, -8388609, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -65533, -1, -1, -100663297, 196611, 16415, 0, 0, 0,
            67108864, -10432, -5, -32769, -4194305, -1, -1, -1, -1, -1017, -1,
            -32769, 67108863, 65535, -131072, -25165825, -2, 767, 1073741824,
            -65463, 2033663, -939513841, 134217726, 2047, -73728, -1, -1,
            541065215, -67059616, -180225, 65535, -8192, 16383, -1, 131135, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, -8, -469762049, -16703999, 537001971,
            -417812, -473563649, -1333765759, 133431235, -423960, -1016201729,
            1577058305, 1900480, -278552, -470942209, 72193, 65475, -417812,
            1676541439, -1333782143, 262083, -700594200, -1006647528, 8396230,
            524224, -139282, 66059775, 30, 65475, -139284, -470811137,
            1080036831, 65475, -139284, -1006633473, 8396225, 65475, -58720276,
            805044223, -16547713, 1835008, -2, 917503, 268402815, 0, -17816170,
            537783470, 872349791, 0, -50331649, -1050673153, -257, -2147481601,
            3872, -1073741824, 237503, 0, -1, 16914171, 16777215, 0, 0, -1,
            -65473, 536870911, -1, -1, -2080374785, -1, -1, -249, -1, 67108863,
            -1, -1, 1031749119, -1, -49665, 2134769663, -8388803, -1,
            -12713985, -1, 134217727, 536870911, 65535, -1, -1, 2097151, -2,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 8388607, 134217726, -1, -1, 131071, 253951, 6553599, 262143,
            122879, -1, -1065353217, 401605055, 1023, 67043328, -1, -1,
            16777215, -1, 511, 0, 0, 536870911, 33226872, -64, 2047999, -1,
            -64513, 67044351, 0, -830472193, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0,
            -1, -1, -1, -1, 268435455, -1, -1, 67108863, 1061158911, -1,
            -1426112705, 1073741823, -1, 1608515583, 265232348, 534519807,
            49152, 27648, 0, -2147352576, 2031616, 0, 0, 0, 1043332228,
            -201605808, 992, -1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            -4194304, -1, 134217727, 2097152, 0, 0, 0, 0, 0, 0, 0, -268435456,
            -1, -1, 1023, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1,
            -32769, 2147483647, 0, -1, -1, -1, 31, -1, -65473, -1, 32831,
            8388607, 2139062143, 2139062143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 224, 524157950, -2, -1, -528482305, -2, -1,
            -134217729, -32, -122881, -1, -1, -32769, 16777215, 0, -65536,
            536870911, -1, 15, -1879048193, -1, 131071, -61441, 2147483647, -1,
            -1, -1, -125829121, -1, -1, 1073741823, 2147483647, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 2097152, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            134217728, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, 8191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2117, 159, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 8, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2147483648, 1, 0, 0, -2147483648,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, -2147483648, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2147483648, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -49153, -1, -63489, -1, -1, 67108863, 0,
            -1594359681, 1602223615, -37, -1, -1, 262143, -524288, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, 1073741823, -65536, -1, -196609, -1,
            255, 536805376, 0, 0, 0, -2162688, -1, -1, -1, 536870911, 0,
            134217726, 134217726, -64, -1, 2147483647, 486341884, 0

    };*/

    // index of context range (Serialization support)
    /** The key. */
    Int32 mKey;

    // flag, true if shaping contextual (Serialization support)
    /** The mask. */
    Int32 mMask;

    // ranges to be shaped
    /** The ranges. */
    Int32 mFRanges;

    // index of the default context
    /** The default context index. */
    Int32 mFDefaultContextIndex;

    // flag if NumericShaper shapes contextually
    /** The contextual. */
    Boolean mFContextual;

    // uses for non-context dependent case only
    /** The single range index. */
    Int32 mFSingleRangeIndex;
};
#endif //__NUMERICSHAPER_H__