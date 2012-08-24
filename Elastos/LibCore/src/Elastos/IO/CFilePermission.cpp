#include "CFilePermission.h"
#include <StringBuffer.h>
#include "cmdef.h"

using namespace Elastos::Core;

const Int64 CFilePermission::sSerialVersionUID = 7930732926638008763LL;

const String CFilePermission::sActionList[] = { String("read"), String("write"),
        String("execute"), String("delete") };

CFilePermission::CFilePermission()
    : mMask(-1)
    , mIncludeAll(FALSE)
    , mAllDir(FALSE)
    , mAllSubdir(FALSE)
{
}

CFilePermission::~CFilePermission()
{
}

ECode CFilePermission::constructor(
    /* [in] */ const String& path,
    /* [in] */ const String& actions)
{
    //super(path);
    return Init(path, actions);
}

ECode CFilePermission::Init(
    /* [in] */ const String& path,
    /* [in] */ const String& pathActions)
{
    if (pathActions.IsNullOrEmpty()) {
        //throw new IllegalArgumentException("pathActions == NULL || pathActions.isEmpty()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mActions = ToCanonicalActionString(pathActions);

    if (path.IsNull()) {
        //throw new NullPointerException("path == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (path.Equals("<<ALL FILES>>")) {
        mIncludeAll = TRUE;
    }
    else {
//        mCanonPath = AccessController
//                .doPrivileged(new PrivilegedAction<String>() {
//                    public String run() {
//                        try {
//                            return new File(path).getCanonicalPath();
//                        } catch (IOException e) {
//                            return path;
//                        }
//                    }
//                });
//        if (path.Equals(String("*")) || path.EndWith(File.separator + "*")) {
//            mAllDir = TRUE;
//        }
//        if (path.Equals(String("-")) || path.EndWith(File.separator + "-")) {
//            mAllSubdir = TRUE;
//        }
    }
    return NOERROR;
}

String CFilePermission::ToCanonicalActionString(
    /* [in] */ const String& action)
{
    mActions = action.Trim();
    mActions.ToLowerCase();

    // get the numerical representation of the action list
    mMask = GetMask(mActions);

    // convert the mask to a canonical action list.
    Int32 len = 4 /*sActionList.GetLength()*/;
    // the test mask - shift the 1 to the leftmost position of the
    // actionList
    Int32 highestBitMask = 1 << (len - 1);

    // if a bit of mask is set, append the corresponding action to result
    StringBuffer result;
    Boolean addedItem = FALSE;
    for (Int32 i = 0; i < len; i++) {
        if ((highestBitMask & mMask) != 0) {
            if (addedItem) {
                result += ",";
            }
            result += sActionList[i];
            addedItem = TRUE;
        }
        highestBitMask = highestBitMask >> 1;
    }
    return String(result);
}

Int32 CFilePermission::GetMask(
    /* [in] */ const String& actionNames)
{
    Int32 actionInt = 0, head = 0, tail = 0;
//    do {
//        tail = actionNames.IndexOf(',', head);
//        String action = tail > 0 ? actionNames.Substring(head, tail).Trim()
//                : actionNames.Substring(head).Trim();
//        if (action.Equals("read")) {
//            actionInt |= 8;
//        }
//        else if (action.Equals("write")) {
//            actionInt |= 4;
//        }
//        else if (action.Equals("execute")) {
//            actionInt |= 2;
//        }
//        else if (action.Equals("delete")) {
//            actionInt |= 1;
//        }
//        else {
//            //throw new IllegalArgumentException("Invalid action: " + action);
//            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        }
//        head = tail + 1;
//    } while (tail > 0);
    return actionInt;
}

ECode CFilePermission::GetActions(
    /* [out] */ String* actions)
{
    VALIDATE_NOT_NULL(actions);
    *actions = mActions;
    return NOERROR;
}

ECode CFilePermission::Equals(
    /* [in] */ IFilePermission* fp,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
//    if (!Objects.equal(fp.actions, actions)) {
//        *isEqual = FALSE;
//        return NOERROR;
//    }
//
//    /* Matching actions and both are <<ALL FILES>> ? */
//    if (fp.m_includeAll || m_includeAll) {
//        return fp.m_includeAll == m_includeAll;
//    }
//    return fp.m_canonPath.Equals(m_canonPath);
    return E_NOT_IMPLEMENTED;
}

ECode CFilePermission::Implies(
    /* [in] */ IPermission* p,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 match = ImpliesMask(p);
//    *result = match != 0 && match == ((FilePermission) p).m_mask;
    return E_NOT_IMPLEMENTED;
}

Int32 CFilePermission::ImpliesMask(
    /* [in] */ IPermission* p)
{
    if (IFilePermission::Probe(p) == NULL) {
        return 0;
    }
//    FilePermission fp = (FilePermission) p;
//    Int32 matchedMask = m_mask & fp.m_mask;
//    // Can't match any bits?
//    if (matchedMask == 0) {
//        return 0;
//    }
//
//    // Is this permission <<ALL FILES>>
//    if (m_includeAll) {
//        return matchedMask;
//    }
//
//    // We can't imply all files
//    if (fp.m_includeAll) {
//        return 0;
//    }
//
//    // Scan the length of p checking all match possibilities
//    // - implies everything except
//    Int32 thisLength = m_canonPath.GetLength();
//    if (m_allSubdir && thisLength == 2
//            && !fp.m_canonPath.Equals(File::separator)) {
//        return matchedMask;
//    }
//    // need /- to imply /-
//    if (fp.m_allSubdir && !m_allSubdir) {
//        return 0;
//    }
//    // need /- or /* to imply /*
//    if (fp.m_allDir && !m_allSubdir && !m_allDir) {
//        return 0;
//    }
//
//    Boolean includeDir = FALSE;
//    Int32 pLength = fp.m_canonPath.GetLength();
//    // do not compare the * or -
//    if (m_allDir || m_allSubdir) {
//        thisLength--;
//    }
//    if (fp.m_allDir || fp.m_allSubdir) {
//        pLength--;
//    }
//    for (Int32 i = 0; i < pLength; i++) {
//        char pChar = fp.m_canonPath.GetChar(i);
//        // Is p longer than this permissions canonLength?
//        if (i >= thisLength) {
//            if (i == thisLength) {
//                // Is this permission include all? (must have matched up
//                // until this point).
//                if (m_allSubdir) {
//                    return matchedMask;
//                }
//                // Is this permission include a dir? Continue the check
//                // afterwards.
//                if (m_allDir) {
//                    includeDir = TRUE;
//                }
//            }
//            // If not includeDir then is has to be a mismatch.
//            if (!includeDir) {
//                return 0;
//            }
//            /**
//             * If we have * for this and find a separator it is invalid. IE:
//             * this is '/a/*' and p is '/a/b/c' we should fail on the
//             * separator after the b. Except for root, canonical paths do
//             * not end in a separator.
//             */
//            if (pChar == File::separatorChar) {
//                return 0;
//            }
//        }
//        else {
//            // Are the characters matched?
//            if (m_canonPath.GetChar(i) != pChar) {
//                return 0;
//            }
//        }
//    }
//    // Must have matched up to this point or it's a valid file in an include
//    // all directory
//    if (pLength == thisLength) {
//        if (m_allSubdir) {
//            // /- implies /- or /*
//            return fp.m_allSubdir || fp.m_allDir ? matchedMask : 0;
//        }
//        return m_allDir == fp.m_allDir ? matchedMask : 0;
//    }
//    return includeDir ? matchedMask : 0;
    return -1; //E_NOT_IMPLEMENTED;
}

ECode CFilePermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** object)
{
    VALIDATE_NOT_NULL(object);
//    return new FilePermissionCollection();
    return NOERROR;
}

ECode CFilePermission::GetHashCode(
    /* [out] */ Int32* hasCode)
{
    VALIDATE_NOT_NULL(hasCode);
//    *hasCode = (mCanonPath.IsNull() ? GetName().GetHashCode() : mCanonPath.GetHashCode())
//                + mMask;
    return NOERROR;
}

//private void writeObject(ObjectOutputStream stream) throws IOException {
//    stream.defaultWriteObject();
//}
//
//private void readObject(ObjectInputStream stream) throws IOException,
//        ClassNotFoundException {
//    stream.defaultReadObject();
//    init(getName(), actions);
//}
