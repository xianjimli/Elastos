
#ifndef __MATCHRESULTIMPL_H__
#define __MATCHRESULTIMPL_H__

#include "Elastos.Utility.Regex_server.h"
#include <elastos/ElRefBase.h>

/**
 * Holds the results of a successful match of a regular expression against a
 * given string. Only used internally, thus sparsely documented (though the
 * defining public interface has full documentation).
 *
 * @see java.util.regex.MatchResult
 */
class MatchResultImpl : public ElRefBase, public IMatchResult
{
public:
    MatchResultImpl(
        /* [in] */ const String& text,
        /* [in] */ ArrayOf<Int32>* offsets);

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI End(
        /* [out] */ Int32* index);

    CARAPI EndEx(
        /* [in] */ Int32 group,
        /* [out] */ Int32* index);

    CARAPI Group(
        /* [out] */ String* text);

    CARAPI GroupEx(
        /* [in] */ Int32 group,
        /* [out] */ String* text);

    CARAPI GroupCount(
        /* [out] */ Int32* count);

    CARAPI StartEx(
        /* [in] */ Int32 group,
        /* [out] */ Int32* index);

    CARAPI Start(
        /* [out] */ Int32* index);

private:
    /**
     * Holds the original input text.
     */
    String mText;

    /**
     * Holds the offsets of the groups in the input text. The first two
     * elements specifiy start and end of the zero group, the next two specify
     * group 1, and so on.
     */
    ArrayOf<Int32>* mOffsets;
};

#endif //__MATCHRESULTIMPL_H__
