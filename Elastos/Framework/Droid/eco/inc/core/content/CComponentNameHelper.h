
#ifndef __CCOMPONENTNAMEHELPER_H_
#define __CCOMPONENTNAMEHELPER_H_

#include "_CComponentNameHelper.h"

CarClass(CComponentNameHelper)
{
public:
    /**
     * Recover a ComponentName from a String that was previously created with
     * {@link #flattenToString()}.  It splits the string at the first '/',
     * taking the part before as the package name and the part after as the
     * class name.  As a special convenience (to use, for example, when
     * parsing component names on the command line), if the '/' is immediately
     * followed by a '.' then the final class name will be the concatenation
     * of the package name with the string following the '/'.  Thus
     * "com.foo/.Blah" becomes package="com.foo" class="com.foo.Blah".
     *
     * @param str The String that was returned by flattenToString().
     * @return Returns a new ComponentName containing the package and class
     * names that were encoded in <var>str</var>
     *
     * @see #flattenToString()
     */
    CARAPI UnflattenFromString(
        /* [in] */ const String& str,
        /* [out] */ IComponentName** component);
};


#endif //__CCOMPONENTNAMEHELPER_H_
