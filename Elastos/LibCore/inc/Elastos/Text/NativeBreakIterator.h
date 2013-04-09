#ifndef __NATIVEBREAKITERATOR_H__
#define __NATIVEBREAKITERATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"

#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>


using namespace Elastos;

class NativeBreakIterator {
public:
/*
    @Override
    public Object clone() {
        int cloneAddr = cloneImpl(this.addr);
        NativeBreakIterator clone = new NativeBreakIterator(cloneAddr, this.type);
        // The RI doesn't clone the CharacterIterator.
        clone.charIter = this.charIter;
        return clone;
    }

    @Override
    public boolean equals(Object object) {
        if (object == this) {
            return true;
        }
        if (!(object instanceof NativeBreakIterator)) {
            return false;
        }
        // TODO: is this sufficient? shouldn't we be checking the underlying rules?
        NativeBreakIterator rhs = (NativeBreakIterator) object;
        return type == rhs.type && charIter.equals(rhs.charIter);
    }

    @Override
    public int hashCode() {
        return 42; // No-one uses BreakIterator as a hash key.
    }
*/

    CARAPI_(Int32) Current();

    CARAPI_(Int32) First();

    CARAPI_(Int32) Following(
        /* [in] */ Int32 offset);

    CARAPI_(AutoPtr<ICharacterIterator>) GetText();

    CARAPI_(Int32) Last();

    CARAPI_(Int32) Next(
        /* [in] */ Int32 n);

    CARAPI_(Int32) Next();

    CARAPI_(Int32) Previous();

    CARAPI_(void) SetText(
        /* [in] */ ICharacterIterator* newText);

    CARAPI_(void) SetText(
        /* [in] */ String newText);

    CARAPI_(Boolean) IsBoundary(
        /* [in] */ Int32 offset);

    CARAPI_(Int32) Preceding(
        /* [in] */ Int32 offset);

    static CARAPI_(NativeBreakIterator*) GetCharacterInstance(
        /* [in] */ ILocale* where);

    static CARAPI_(NativeBreakIterator*) GetLineInstance(
        /* [in] */ ILocale* where);

    static CARAPI_(NativeBreakIterator*) GetSentenceInstance(
        /* [in] */ ILocale* where);

    static CARAPI_(NativeBreakIterator*) GetWordInstance(
        /* [in] */ ILocale* where);

protected:
    ~NativeBreakIterator();

private:
    NativeBreakIterator(
        /* [in] */ Int32 iterAddr,
        /* [in] */ Int32 type);

    static CARAPI_(Int32) GetCharacterInstanceImpl(
        /* [in] */ const String& locale);

    static CARAPI_(Int32) GetWordInstanceImpl(
        /* [in] */ String locale);

    static CARAPI_(Int32) GetLineInstanceImpl(
        /* [in] */ String locale);

    static CARAPI_(Int32) GetSentenceInstanceImpl(
        /* [in] */ String locale);

    static CARAPI_(void) CloseBreakIteratorImpl(
        /* [in] */ Int32 addr);

    static CARAPI_(void) SetTextImpl(
        /* [in] */ Int32 addr, 
        /* [in] */ String text);

    static CARAPI_(Int32) CloneImpl(
        /* [in] */ Int32 addr);

    static CARAPI_(Int32) PrecedingImpl(
        /* [in] */ Int32 addr, 
        /* [in] */ Int32 offset);

    static CARAPI_(Boolean) IsBoundaryImpl(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 offset);

    static CARAPI_(Int32) NextImpl(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 n);

    static CARAPI_(Int32) PreviousImpl(
        /* [in] */ Int32 addr);

    static CARAPI_(Int32) CurrentImpl(
        /* [in] */ Int32 addr);

    static CARAPI_(Int32) FirstImpl(
        /* [in] */ Int32 addr);

    static CARAPI_(Int32) FollowingImpl(
        /* [in] */ Int32 addr,
        /* [in] */ Int32 offset);

    static CARAPI_(Int32) LastImpl(
        /* [in] */ Int32 addr);

private:
    // Acceptable values for the 'type' field.
    static const Int32 BI_CHAR_INSTANCE = 1;
    static const Int32 BI_WORD_INSTANCE = 2;
    static const Int32 BI_LINE_INSTANCE = 3;
    static const Int32 BI_SENT_INSTANCE = 4;

    Int32 addr;
    Int32 type;
    AutoPtr<ICharacterIterator> charIter;
};
#endif //__NATIVEBREAKITERATOR_H__