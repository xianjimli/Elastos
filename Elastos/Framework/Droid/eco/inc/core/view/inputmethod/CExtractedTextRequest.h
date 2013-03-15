
#ifndef __CEXTRACTEDTEXTREQUEST_H__
#define __CEXTRACTEDTEXTREQUEST_H__

#include "_CExtractedTextRequest.h"

CarClass(CExtractedTextRequest)
{
public:
    CARAPI GetToken(
        /* [out] */ Int32* token);

    CARAPI SetToken(
        /* [in] */ Int32 token);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI GetHintMaxLines(
        /* [out] */ Int32* hintMaxLines);

    CARAPI SetHintMaxLines(
        /* [in] */ Int32 hintMaxLines);

    CARAPI GetHintMaxChars(
        /* [out] */ Int32* hintMaxChars);

    CARAPI SetHintMaxChars(
        /* [in] */ Int32 hintMaxChars);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

public:
    /**
     * Arbitrary integer that can be supplied in the request, which will be
     * delivered back when reporting updates.
     */
    Int32 mToken;

    /**
     * Additional request flags, having the same possible values as the
     * flags parameter of {@link InputConnection#getTextBeforeCursor
     * InputConnection.getTextBeforeCursor()}.
     */
    Int32 mFlags;

    /**
     * Hint for the maximum number of lines to return.
     */
    Int32 mHintMaxLines;

    /**
     * Hint for the maximum number of characters to return.
     */
    Int32 mHintMaxChars;
};

#endif //__CEXTRACTEDTEXTREQUEST_H__