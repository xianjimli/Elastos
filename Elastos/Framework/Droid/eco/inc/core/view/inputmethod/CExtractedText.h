
#ifndef __CExtractedText_h__
#define __CExtractedText_h__

#include "_CExtractedText.h"
#include <elastos/AutoPtr.h>


CarClass(CExtractedText)
{
public:
	CExtractedText();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

public:
    /**
     * The text that has been extracted.
     */
    AutoPtr<ICharSequence> mText;

    /**
     * The offset in the overall text at which the extracted text starts.
     */
    Int32 mStartOffset;

    /**
     * If the content is a report of a partial text change, this is the
     * offset where the change starts and it runs until
     * {@link #partialEndOffset}.  If the content is the full text, this
     * field is -1.
     */
    Int32 mPartialStartOffset;

    /**
     * If the content is a report of a partial text change, this is the offset
     * where the change ends.  Note that the actual text may be larger or
     * smaller than the difference between this and {@link #partialEndOffset},
     * meaning a reduction or increase, respectively, in the total text.
     */
    Int32 mPartialEndOffset;

    /**
     * The offset where the selection currently starts within the extracted
     * text.  The real selection start position is at
     * <var>startOffset</var>+<var>selectionStart</var>.
     */
    Int32 mSelectionStart;

    /**
     * The offset where the selection currently ends within the extracted
     * text.  The real selection end position is at
     * <var>startOffset</var>+<var>selectionEnd</var>.
     */
    Int32 mSelectionEnd;

    /**
     * Bit for {@link #flags}: set if the text being edited can only be on
     * a single line.
     */
    static const Int32 FLAG_SINGLE_LINE = 0x0001;

    /**
     * Bit for {@link #flags}: set if the editor is currently in selection mode.
     */
    static const Int32 FLAG_SELECTING = 0x0002;

    /**
     * Additional bit flags of information about the edited text.
     */
    Int32 mFlags;
};

#endif //__CExtractedText_h__