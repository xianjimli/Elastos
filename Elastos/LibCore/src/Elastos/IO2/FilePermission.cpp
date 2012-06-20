#include "FilePermission.h"
#include "File.h"


const String FilePermission::actionList[4] = { String("read"), 
    String("write"), String("execute"), String("delete") };


FilePermission::FilePermission()
    : m_includeAll(FALSE)
    , m_allDir(FALSE)
    , m_allSubdir(FALSE)
    , m_mask(1)
{
}

FilePermission::~FilePermission()
{
}

ECode FilePermission::Init(
    /* [in] */ const String path,
    /* [in] */ String pathActions)
{
    //super(path);
    if (pathActions.IsNull() || pathActions.IsEmpty()) {
        //throw new IllegalArgumentException("pathActions == NULL || pathActions.isEmpty()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ToCanonicalActionString(pathActions, &m_actions);
 
    if (path.IsNull()) {
        //throw new NullPointerException("path == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (path.Equals("<<ALL FILES>>")) {
        m_includeAll = TRUE;
    } else {
//        m_canonPath = AccessController
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
//            m_allDir = TRUE;
//        }
//        if (path.Equals(String("-")) || path.EndWith(File.separator + "-")) {
//            m_allSubdir = TRUE;
//        }
    }
    return NOERROR;
}

ECode FilePermission::GetActions(
    /* [out] */ String* actions)
{
    actions = &m_actions;
    return NOERROR;
}

ECode FilePermission::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* isEqual)
{
//    if (obj instanceof FilePermission) {
//        FilePermission fp = (FilePermission) obj;
//        if (!Objects.equal(fp.actions, actions)) {
//            * isEqual = FALSE;
//            return NOERROR;
//        }
// 
//        /* Matching actions and both are <<ALL FILES>> ? */
//        if (fp.m_includeAll || m_includeAll) {
//            return fp.m_includeAll == m_includeAll;
//        }
//        return fp.m_canonPath.Equals(m_canonPath);
//    }
    * isEqual = FALSE;
    return NOERROR;
}

ECode FilePermission::Implies(
//    /* [in] */ IPermission* p,
    /* [out] */ Boolean* isimplied)
{
//    Int32 match = ImpliesMask(p);
//    * isimplied = match != 0 && match == ((FilePermission) p).m_mask;
    return NOERROR;
}

ECode FilePermission::NewPermissionCollection()
//    /* [out] */ PermissionCollection* object)
{
//    return new FilePermissionCollection();
    return NOERROR;
}

ECode FilePermission::HashCode(
    /* [out] */ Int32* hasCode)
{
//    * hasCode = (m_canonPath.IsNull() ? getName().hashCode() : m_canonPath.hashCode())
//                + m_mask;
    return NOERROR;
}

ECode FilePermission::ToCanonicalActionString(
    /* [in] */ String action,
    /* [out] */ String* result)
{
    StringBuf strBuf(NULL, action.GetLength());
    StringBuf strBuf2(NULL, action.GetLength());
    strBuf.Copy(action);
    strBuf2.Copy(action.Trim(strBuf));

    //m_actions = action.Trim(strBuf).ToLowerCase();
    m_actions = action.ToLowerCase(strBuf2);

    // get the numerical representation of the action list
    GetMask(m_actions, &m_mask);

    // convert the mask to a canonical action list.
    //Int32 len = actionList.GetLength();
    Int32 len = 4;
    // the test mask - shift the 1 to the leftmost position of the
    // actionList
    Int32 highestBitMask = 1 << (len - 1);

    // if a bit of mask is set, append the corresponding action to result
    //StringBuilder result = new StringBuilder();
    StringBuf_<256> temp;
    Boolean addedItem = FALSE;
    for (Int32 i = 0; i < len; i++) {
        if ((highestBitMask & m_mask) != 0) {
            if (addedItem) {
                temp.Append(",");
            }
            temp.Append(actionList[i]);
            addedItem = TRUE;
        }
        highestBitMask = highestBitMask >> 1;
    }
    *result = temp;
    return NOERROR;
}

ECode FilePermission::GetMask(
    /* [in] */ String actionNames,
    /* [out] */ Int32* result)
{
    StringBuf_<25> strBuf;
    Int32 actionInt = 0, head = 0, tail = 0;
    do {
        tail = actionNames.IndexOf(',');
        String action = tail > 0 ? actionNames.Substring(head, tail, strBuf).Trim(strBuf)
                : actionNames.Substring(head).Trim(strBuf);
        if (action.Equals("read")) {
            actionInt |= 8;
        } else if (action.Equals("write")) {
            actionInt |= 4;
        } else if (action.Equals("execute")) {
            actionInt |= 2;
        } else if (action.Equals("delete")) {
            actionInt |= 1;
        } else {
            //throw new IllegalArgumentException("Invalid action: " + action);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        head = tail + 1;
    } while (tail > 0);
    *result = actionInt;
    return NOERROR;
}

ECode FilePermission::ImpliesMask(
//    /* [in] */ IPermission *p,
    /* [out] */ Int32* result)
{
//    if (!(p instanceof FilePermission)) {
//        return 0;
//    }
//    FilePermission fp = (FilePermission) p;
    FilePermission fp;
    Int32 matchedMask = m_mask & fp.m_mask;
    // Can't match any bits?
    if (matchedMask == 0) {
        return 0;
    }

    // Is this permission <<ALL FILES>>
    if (m_includeAll) {
        return matchedMask;
    }

    // We can't imply all files
    if (fp.m_includeAll) {
        return 0;
    }

    // Scan the length of p checking all match possibilities
    // - implies everything except 
    Int32 thisLength = m_canonPath.GetLength();
    if (m_allSubdir && thisLength == 2
            && !fp.m_canonPath.Equals(File::separator)) {
        return matchedMask;
    }
    // need /- to imply /-
    if (fp.m_allSubdir && !m_allSubdir) {
        return 0;
    }
    // need /- or /* to imply /*
    if (fp.m_allDir && !m_allSubdir && !m_allDir) {
        return 0;
    }

    Boolean includeDir = FALSE;
    Int32 pLength = fp.m_canonPath.GetLength();
    // do not compare the * or -
    if (m_allDir || m_allSubdir) {
        thisLength--;
    }
    if (fp.m_allDir || fp.m_allSubdir) {
        pLength--;
    }
    for (Int32 i = 0; i < pLength; i++) {
        char pChar = fp.m_canonPath.GetChar(i);
        // Is p longer than this permissions canonLength?
        if (i >= thisLength) {
            if (i == thisLength) {
                // Is this permission include all? (must have matched up
                // until this point).
                if (m_allSubdir) {
                    return matchedMask;
                }
                // Is this permission include a dir? Continue the check
                // afterwards.
                if (m_allDir) {
                    includeDir = TRUE;
                }
            }
            // If not includeDir then is has to be a mismatch.
            if (!includeDir) {
                return 0;
            }
            /**
             * If we have * for this and find a separator it is invalid. IE:
             * this is '/a/*' and p is '/a/b/c' we should fail on the
             * separator after the b. Except for root, canonical paths do
             * not end in a separator.
             */
            if (pChar == File::separatorChar) {
                return 0;
            }
        } else {
            // Are the characters matched?
            if (m_canonPath.GetChar(i) != pChar) {
                return 0;
            }
        }
    }
    // Must have matched up to this point or it's a valid file in an include
    // all directory
    if (pLength == thisLength) {
        if (m_allSubdir) {
            // /- implies /- or /*
            return fp.m_allSubdir || fp.m_allDir ? matchedMask : 0;
        }
        return m_allDir == fp.m_allDir ? matchedMask : 0;
    }
    return includeDir ? matchedMask : 0;
}

ECode FilePermission::WriteObject()
//    /* [in] */ IObjectOutputStream stream)
{
//    stream.defaultWriteObject();
    return NOERROR;
}

ECode FilePermission::ReadObject()
//    /* [in] */ IObjectOutputStream stream)
{
//    ClassNotFoundException {
//        stream.defaultReadObject();
//        Init(GetName(), actions);
//    }
    return NOERROR;
}