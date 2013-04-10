#ifndef __RULEBASEDBREAKITERATOR_H__
#define __RULEBASEDBREAKITERATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include "BreakIterator.h"
using namespace Elastos;

class RuleBasedBreakIterator : public BreakIterator {

public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    RuleBasedBreakIterator();

    /*
     * Wrapping constructor.
     */
    RuleBasedBreakIterator(
            /* [in] */ NativeBreakIterator* iterator);

    virtual ~RuleBasedBreakIterator();

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#current()
     */
    //@Override
    CARAPI Current(
        /* [out] */ Int32* currentValue);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#first()
     */
    //@Override
    CARAPI First(
        /* [out] */ Int32* firstValue);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#following(int)
     */
    //@Override
    CARAPI Following(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* followingValue);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#getText()
     */
    //@Override
    CARAPI GetText(
        /* [out] */ ICharacterIterator** text);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#last()
     */
    //@Override
    CARAPI Last(
        /* [out] */ Int32* lastValue);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#next()
     */
    //@Override
    CARAPI Next(
        /* [out] */ Int32* nextValue);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#next(int)
     */
    //@Override
    CARAPI NextEx(
        /* [in] */ Int32 n,
        /* [out] */ Int32* nextValue);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#previous()
     */
    //@Override
    CARAPI Previous(
        /* [out] */ Int32* previousValue);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#setText(java.text.CharacterIterator)
     */
    //@Override
    CARAPI SetTextEx(
        /* [in] */ ICharacterIterator* newText);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#isBoundary(int)
     */
    //@Override
    CARAPI IsBoundary(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* isBoundary);

    /*
     * (non-Javadoc)
     *
     * @see java.text.BreakIterator#preceding(int)
     */
    //@Override
    CARAPI Preceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* precedingValue);

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#equals(java.lang.Object)
     */
/*    @Override
    public boolean equals(Object o) {
        if (!(o instanceof RuleBasedBreakIterator)) {
            return false;
        }
        return wrapped.equals(((RuleBasedBreakIterator) o).wrapped);
    }*/

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#toString()
     */
/*    @Override
    public String toString() {
        return wrapped.toString();
    }*/

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#hashCode()
     */
/*    @Override
    public int hashCode() {
        return wrapped.hashCode();
    }*/

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#clone()
     */
/*    @Override
    public Object clone() {
        RuleBasedBreakIterator cloned = (RuleBasedBreakIterator) super.clone();
        cloned.wrapped = (NativeBreakIterator) wrapped.clone();
        return cloned;
    }*/

private:
    /*
     * check the offset, throw exception if it is invalid
     */
    CARAPI ValidateOffset(
        /* [in] */ Int32 offset);
};
#endif //__RULEBASEDBREAKITERATOR_H__