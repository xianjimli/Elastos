
#include "content/CUriMatcher.h"
#include "ext/frameworkext.h"

CUriMatcher::CUriMatcher()
    : mCode(UriMatcher_NO_MATCH)
    , mWhich(-1)
    , mText(NULL) 
{}

CUriMatcher::~CUriMatcher()
{
    mChildren.Clear();
}

ECode CUriMatcher::AddURI(
    /* [in] */ const String& authority,
    /* [in] */ const String& path,
    /* [in] */ Int32 code)
{   
    if (0 >= code) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ArrayOf<String>* tokens;

    if (!path.IsNull()) {
        ICharSequence* pCS = NULL;
        CStringWrapper::New(path, &pCS);
        //TODO
        //FAIL_RETURN(PATH_SPLIT_PATTERN->Split(pCS, &tokens));
    }

    Int32 numTokens = tokens != NULL ? tokens->GetLength() : 0;
    AutoPtr<CUriMatcher> node = this;

    for (Int32 i = -1; i < numTokens; i++) {
        String token;

        if (i < 0) {
            token.SetTo(authority);
        } else {
            token.SetTo((*tokens)[i]);
        }

        List<AutoPtr<CUriMatcher> > children = node->mChildren;
        Int32 numChildren = children.GetSize();
        AutoPtr<CUriMatcher> child;
        Int32 j = 0;
        List<AutoPtr<CUriMatcher> >::Iterator it;

        for (it = mChildren.Begin(); it != mChildren.End(); it++, j++) {
            child = *it;
            if (!token.Compare(child->mText)) {
                node = child;
                break;
            }
        }

        if (j == numChildren) {
            // Child not found, create it
            FAIL_RETURN(CUriMatcher::NewByFriend(UriMatcher_NO_MATCH, (CUriMatcher**)&child));

            if (!token.Compare("#")) {
                child->mWhich = NUMBER;
            } else if (!token.Compare("*")) {
                child->mWhich = TEXT;
            } else {
                child->mWhich = EXACT;
            }

            child->mText.SetTo(token);
            node->mChildren.PushBack(child);
            node = child;
        }
    }

    node->mCode = code;
    return E_NOT_IMPLEMENTED;
}

ECode CUriMatcher::Match(
    /* [in] */ IUri* uri,
    /* [out] */ Int32* matchCode)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(matchCode);
    ArrayOf<String>* pathSegments;
    FAIL_RETURN(uri->GetPathSegments(&pathSegments));
    Int32 length = pathSegments->GetLength();
    AutoPtr<CUriMatcher> node = this;
    String authority;
    uri->GetAuthority(&authority);

    if (0 == length && authority.IsNull()) {
        *matchCode = node->mCode;
        return NOERROR;
    }

    for (Int32 i = -1; i < length; i++) {
        String u;

        if (0 > i) {
            u.SetTo(authority);
        } else {
            u.SetTo((*pathSegments)[i]);
        }

        List<AutoPtr<CUriMatcher> > list = node->mChildren;
        if (list.IsEmpty()) break;
        node = NULL;
        List<AutoPtr<CUriMatcher> >::Iterator it;

        for (it = list.Begin(); it != list.End(); it++) {
            CUriMatcher* n = *it;
            Int32 lk = 0;
            //which_switch:
            switch (n->mWhich) {
            case EXACT:
                if (!n->mText.Compare(u)) {
                    node = n;
                }
                break;
            case NUMBER:
                lk = u.GetLength();
                for (Int32 k = 0; k < lk; k++) {
                    Char8 c = u[k];
                    if (c < '0' || c > '9') {
                        //break which_switch;
                        goto which_switch;
                    }
                }
                node = n;
                break;
            case TEXT:
                node = n;
                break;
            }

            which_switch:
            if (NULL != node) {
                break;
            }
        }

        if (NULL == node) {
            *matchCode = UriMatcher_NO_MATCH;
            return NOERROR;
        }
    }

    *matchCode = node->mCode;
    return NOERROR;
}

ECode CUriMatcher::constructor(
    /* [in] */ Int32 code)
{
    mCode = code;
    return NOERROR;
}

