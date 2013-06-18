#ifndef __RULEBASEDBREAKITERATOR_H__
#define __RULEBASEDBREAKITERATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include "BreakIterator.h"

class RuleBasedBreakIterator
    : public ElRefBase
    , public IBreakIterator
    , public BreakIterator
{
public:
    /*
     * Wrapping constructor.
     */
    RuleBasedBreakIterator(
            /* [in] */ NativeBreakIterator* iterator);

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    ~RuleBasedBreakIterator();

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

    CARAPI SetText(
        /* [in] */ const String& newText);

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

private:
    /*
     * check the offset, throw exception if it is invalid
     */
    CARAPI ValidateOffset(
        /* [in] */ Int32 offset);
};
#endif //__RULEBASEDBREAKITERATOR_H__