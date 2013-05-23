#ifndef __HEADER_H__
#define __HEADER_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <elastos/List.h>
#include <elastos/Map.h>

using namespace Elastos;

class Header
{
    /**
     * A generic header structure. Used mostly for request / response header.
     * The key/value pair of the header may be inserted for later use. The key
     * is stored in an array for indexed slot access.
     */
public:
    Header();
    /**
     * The alternative constructor which sets the input map as its initial
     * keyTable.
     *
     * @param map
     *            the initial keyTable as a map
     */
    Header(Map<String, List<String> > map);

    //public Object clone() {

    /**
     * Add a field with the specified value.
     *
     * @param key
     * @param value
     */
    void Add(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    void RemoveAll(
        /* [in] */ const String& key);

    void AddAll(
        /* [in] */ const String& key,
        /* [in] */ List<String> headers);

    void AddIfAbsent(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /**
     * Set a field with the specified value. If the field is not found, it is
     * added. If the field is found, the existing value(s) are overwritten.
     *
     * @param key
     * @param value
     */
    void Set(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /**
     * Provides an unmodifiable map with all String header names mapped to their
     * String values. The map keys are Strings and the values are unmodifiable
     * Lists of Strings.
     *
     * @return an unmodifiable map of the headers
     *
     * @since 1.4
     */
    Map<String, List<String> > GetFieldMap();

    /**
     * Returns the element at <code>pos</code>, null if no such element
     * exist.
     *
     * @return java.lang.String the value of the key
     * @param pos
     *            int the position to look for
     */
    String Get(
        /* [in] */ Int32 pos);

    /**
     * Returns the key of this header at <code>pos</code>, null if there are
     * fewer keys in the header
     *
     *
     * @return the key the desired position
     * @param pos
     *            the position to look for
     */
    String GetKey(
        /* [in] */ Int32 pos);

    /**
     * Returns the value corresponding to the specified key, or null.
     */
    String Get(
        /* [in] */ const String& key);

    /**
     * Returns the number of keys stored in this header
     *
     * @return the number of keys.
     */
    Int32 Length();

    /**
     * Sets the status line in the header request example: GET / HTTP/1.1
     * response example: HTTP/1.1 200 OK
     *
     * @param statusLine
     */
    void SetStatusLine(
        /* [in] */ const String& statusLine);

    /**
     * Gets the status line in the header request example: GET / HTTP/1.1
     * response example: HTTP/1.1 200 OK
     *
     * @return the status line
     */
    String GetStatusLine();
};
#endif //__HEADER_H__