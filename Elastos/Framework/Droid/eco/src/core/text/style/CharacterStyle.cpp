
#include "text/style/CharacterStyle.h"


/**
 * A Passthrough CharacterStyle is one that
 * passes {@link #updateDrawState} calls through to the
 * specified CharacterStyle while still being a distinct object,
 * and is therefore able to be attached to the same Spannable
 * to which the specified CharacterStyle is already attached.
 */
class Passthrough
    : public ElRefBase
    , public CharacterStyle
    , public ICharacterStyle
{
public:
    /**
     * Creates a new Passthrough of the specfied CharacterStyle.
     */
    Passthrough(
        /* [in] */ ICharacterStyle* cs)
        : mStyle(cs)
    {}

    CARAPI_(UInt32) AddRef()
    {
        return ElRefBase::AddRef();
    }

    CARAPI_(UInt32) Release()
    {
        return ElRefBase::Release();
    }

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid)
    {
        if (riid == EIID_IInterface) {
            return (PInterface)(ICharacterStyle*)this;
        }
        else if (riid == EIID_ICharacterStyle) {
            return (ICharacterStyle*)this;
        }

        return NULL;
    }

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID)
    {
        if (pIID == NULL) {
            return E_INVALID_ARGUMENT;
        }

        if (pObject == (IInterface*)(ICharacterStyle*)this) {
            *pIID = EIID_ICharacterStyle;
        }
        else {
            return E_INVALID_ARGUMENT;
        }

        return NOERROR;
    }

    /**
     * Passes updateDrawState through to the underlying CharacterStyle.
     */
    //@Override
    CARAPI UpdateDrawState(
        /* [in]*/ ITextPaint* tp)
    {
        return mStyle->UpdateDrawState(tp);
    }

    /**
     * Returns the CharacterStyle underlying this one, or the one
     * underlying it if it too is a Passthrough.
     */
    //@Override
    CARAPI GetUnderlying(
        /* [out] */ ICharacterStyle** underlying)
    {
        return mStyle->GetUnderlying(underlying);
    }

private:
    AutoPtr<ICharacterStyle> mStyle;
};


AutoPtr<ICharacterStyle> CharacterStyle::Wrap(
    /* [in] */ ICharacterStyle* cs)
{
    if (IMetricAffectingSpan::Probe(cs) != NULL) {
        // return new MetricAffectingSpan.Passthrough((MetricAffectingSpan) cs);
        assert(0);
        return NULL;
    }
    else {
        return new Passthrough(cs);
    }
}

ECode CharacterStyle::GetUnderlying(
    /* [out] */ ICharacterStyle** underlying)
{
    *underlying = (ICharacterStyle*)this->Probe(EIID_ICharacterStyle);
    (*underlying)->AddRef();
    return NOERROR;
}
