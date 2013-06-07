#ifndef __NATIVEBIDI_H__
#define __NATIVEBIDI_H__

#include <elastos.h>
#include "Elastos.Text_server.h"
#include <elastos/AutoPtr.h>

/**
 * Dalvik Bidi wrapper. Derived from an old version of Harmony; today they call
 * straight through to ICU4J.
 */
class NativeBidi
{
public:
    // Allocate a UBiDi structure.
    static CARAPI_(Int64) Ubidi_open();

    // ubidi_close() must be called to free the memory associated with a
    // UBiDi object.
    static CARAPI_(void) Ubidi_close(
        /* [in] */ Int64 pBiDi);

    // Perform the Unicode BiDi algorithm.
    static CARAPI Ubidi_setPara(
        /* [in] */ Int64 pBiDi,
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 length,
        /* [in] */ Int32 paraLevel,
        /* [in] */ ArrayOf<Byte>* embeddingLevels);

    // ubidi_setLine() sets a UBiDi to contain the reordering information,
    // especially the resolved levels, for all the characters in a line of
    // text.
    static CARAPI Ubidi_setLine(
        /* [in] */ Int64 pParaBiDi,
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [out] */ Int64* line);

    // Get the directionality of the text.
    static CARAPI_(Int32) Ubidi_getDirection(
        /* [in] */ Int64 pBiDi);

    // Get the length of the text.
    static CARAPI_(Int32) Ubidi_getLength(
        /* [in] */ Int64 pBiDi);

    // Get the paragraph level of the text.
    static CARAPI_(Byte) Ubidi_getParaLevel(
        /* [in] */ Int64 pBiDi);

    // Get an array of levels for each character.
    static CARAPI Ubidi_getLevels(
        /* [in] */ Int64 pBiDi,
        /* [out] */ ArrayOf<Byte>** levels);

    // Get the number of runs.
    static CARAPI Ubidi_countRuns(
        /* [in] */ Int64 pBiDi,
        /* [out] */ Int32* count);

    // Get the BidiRuns
    static CARAPI Ubidi_getRuns(
        /* [in] */ Int64 pBidi,
        /* [out] */ ArrayOf<AutoPtr<IBidiRun> >** runs);

    // This is a convenience function that does not use a UBiDi object
    static CARAPI_(ArrayOf<Int32>*) Ubidi_reorderVisual(
        /* [in] */ ArrayOf<Byte>* levelBytes,
        /* [in] */ Int32 length);

public:
    const static Int32 UBIDI_DEFAULT_LTR = 0xfe;

    const static Int32 UBIDI_DEFAULT_RTL = 0xff;

    const static Int32 UBIDI_MAX_EXPLICIT_LEVEL = 61;

    const static Int32 UBIDI_LEVEL_OVERRIDE = 0x80;

    const static Int32 UBIDI_KEEP_BASE_COMBINING = 1;

    const static Int32 UBIDI_DO_MIRRORING = 2;

    const static Int32 UBIDI_INSERT_LRM_FOR_NUMERIC = 4;

    const static Int32 UBIDI_REMOVE_BIDI_CONTROLS = 8;

    const static Int32 UBIDI_OUTPUT_REVERSE = 16;

    const static Int32 UBiDiDirection_UBIDI_LTR = 0;

    const static Int32 UBiDiDirection_UBIDI_RTL = 1;

    const static Int32 UBiDiDirection_UBIDI_MIXED = 2;
};
#endif //__NATIVEBIDI_H__