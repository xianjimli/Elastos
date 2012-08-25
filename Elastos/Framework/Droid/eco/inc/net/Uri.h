
#ifndef __URI_H__
#define __URI_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
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
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded);

        CARAPI GetDecoded(
            /* [out] */ String* decoded);
    };

    class Part : public AbstractPart
    {
    public:
        Part(
            /* [in] */ const String& encoded,
            /* [in] */ const String& decoded);

        /**
         * Creates a part from the encoded string.
         *
         * @param encoded part string
         */
        static CARAPI_(Part*) FromEncoded(
            /* [in] */ const String& encoded);

    private:
        /**
         * Creates a part from the encoded and decoded strings.
         *
         * @param encoded part string
         * @param decoded part string
         */
        static CARAPI_(Part*) From(
            /* [in] */ const String& encoded,
            /* [in] */ const String& decoded);
    };

public:
    static CARAPI Parse(
        /* [in] */ const String& uriString,
        /* [out] */ IUri **ppUri);

    static CARAPI Decode(
        /* [in] */ const String& str,
        /* [out] */ String* decoded);

    /**
     * Creates a Uri from a file. The URI has the form
     * "file://<absolute path>". Encodes path characters with the exception of
     * '/'.
     *
     * <p>Example: "file:///tmp/android.txt"
     *
     * @throws NullPointerException if file is null
     * @return a Uri for the given file
     */
    static CARAPI_(AutoPtr<IUri>) FromFile(
        /* [in] */ IFile* file);

    /**
     * Creates an opaque Uri from the given components. Encodes the ssp
     * which means this method cannot be used to create hierarchical URIs.
     *
     * @param scheme of the URI
     * @param ssp scheme-specific-part, everything between the
     *  scheme separator (':') and the fragment separator ('#'), which will
     *  get encoded
     * @param fragment fragment, everything after the '#', null if undefined,
     *  will get encoded
     *
     * @throws NullPointerException if scheme or ssp is null
     * @return Uri composed of the given scheme, ssp, and fragment
     *
     * @see Builder if you don't want the ssp and fragment to be encoded
     */
    static CARAPI FromParts(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& fragment,
        /* [out] */ IUri** uri);

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
