
#ifndef __URI_H__
#define __URI_H__

#include "Elastos.Framework_server.h"
#include <elastos/etl_hash_fun.h>

using namespace Elastos;

class Uri
{
public:
    class AbstractPart
    {
    public:
        String mEncoded;
        String mDecoded;

    public:
        AbstractPart(
        /* [in] */ String encoded,
        /* [in] */ String decoded);

        CARAPI GetDecoded(
            /* [out] */ String* decoded);
    };

    class Part : public AbstractPart
    {
    public:
        Part(
            /* [in] */ String encoded,
            /* [in] */ String decoded);

        /**
         * Creates a part from the encoded string.
         *
         * @param encoded part string
         */
        static CARAPI_(Part*) FromEncoded(
            /* [in] */ String encoded);

    private:
        /**
         * Creates a part from the encoded and decoded strings.
         *
         * @param encoded part string
         * @param decoded part string
         */
        static CARAPI_(Part*) From(
            /* [in] */ String encoded,
            /* [in] */ String decoded);
    };

public:
    static CARAPI Parse(
        /* [in] */ String uriString,
        /* [out] */ IUri **ppUri);

    static CARAPI Decode(
        /* [in] */ String str,
        /* [out] */ String* decoded);

private:
    /**
     * NOTE: EMPTY accesses this field during its own initialization, so this
     * field *must* be initialized first, or else EMPTY will see a null value!
     *
     * Placeholder for strings which haven't been cached. This enables us
     * to cache null. We intentionally create a new String instance so we can
     * compare its identity and there is no chance we will confuse it with
     * user data.
     */
    static const String NOT_CACHED;

    /** Placeholder value for an index which hasn't been calculated yet. */
    static const Int32 NOT_CALCULATED = -2;

    friend class CStringUri;
};

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<IUri*>
{
    size_t operator()(IUri* uri) const
    {
        Int32 hashCode;
        assert(uri != NULL);
        uri->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

_ELASTOS_NAMESPACE_END

#endif //__URI_H__
