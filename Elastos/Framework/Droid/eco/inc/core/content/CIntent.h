
#ifndef __CINTENT_H__
#define __CINTENT_H__

#include "ext/frameworkdef.h"
#include "_CIntent.h"
#include "content/CComponentName.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>

CarClass(CIntent)
{
public:
    /**
     * Wrapper class holding an Intent and implementing comparisons on it for
     * the purpose of filtering.  The class implements its
     * {@link #equals equals()} and {@link #hashCode hashCode()} methods as
     * simple calls to {@link Intent#filterEquals(Intent)}  filterEquals()} and
     * {@link android.content.Intent#filterHashCode()}  filterHashCode()}
     * on the wrapped Intent.
     */
    class FilterComparison
    {
    public:
        FilterComparison(
            /* [in] */ IIntent* intent);

        /**
         * Return the Intent that this FilterComparison represents.
         * @return Returns the Intent held by the FilterComparison.  Do
         * not modify!
         */
        CARAPI_(IIntent*) GetIntent();

        CARAPI_(Int32) GetHashCode();

        CARAPI_(Boolean) Equals(
            /* [in] */ FilterComparison* obj);

        CARAPI GetDescription(
            /* [out] */ String* description);

    private:
        AutoPtr<IIntent> mIntent;
        Int32 mHashCode;
    };

public:
    CIntent();

    ~CIntent();

    /**
     * Make a clone of only the parts of the Intent that are relevant for
     * filter matching: the action, data, type, component, and categories.
     */
    CARAPI CloneFilter(
        /* [out] */ IIntent** result);

    /**
     * Retrieve the general action to be performed, such as
     * {@link #ACTION_VIEW}.  The action describes the general way the rest of
     * the information in the intent should be interpreted -- most importantly,
     * what to do with the data returned by {@link #getData}.
     *
     * @return The action of this intent or null if none is specified.
     *
     * @see #setAction
     */
    CARAPI GetAction(
        /* [out] */ String* action);

    /**
     * Retrieve data this intent is operating on.  This URI specifies the name
     * of the data; often it uses the content: scheme, specifying data in a
     * content provider.  Other schemes may be handled by specific activities,
     * such as http: by the web browser.
     *
     * @return The URI of the data this intent is targeting or null.
     *
     * @see #getScheme
     * @see #setData
     */
    CARAPI GetData(
        /* [out] */ IUri** data);

    /**
     * The same as {@link #getData()}, but returns the URI as an encoded
     * String.
     */
    CARAPI GetDataString(
        /* [out] */ String* dataString);

    /**
     * Return the scheme portion of the intent's data.  If the data is null or
     * does not include a scheme, null is returned.  Otherwise, the scheme
     * prefix without the final ':' is returned, i.e. "http".
     *
     * <p>This is the same as calling getData().getScheme() (and checking for
     * null data).
     *
     * @return The scheme of this intent.
     *
     * @see #getData
     */
    CARAPI GetScheme(
        /* [out] */ String* scheme);

    /**
     * Retrieve any explicit MIME type included in the intent.  This is usually
     * null, as the type is determined by the intent data.
     *
     * @return If a type was manually set, it is returned; else null is
     *         returned.
     *
     * @see #resolveType(ContentResolver)
     * @see #setType
     */
    CARAPI GetType(
        /* [out] */ String* type);

    /**
     * Return the MIME data type of this intent.  If the type field is
     * explicitly set, that is simply returned.  Otherwise, if the data is set,
     * the type of that data is returned.  If neither fields are set, a null is
     * returned.
     *
     * @return The MIME type of this intent.
     *
     * @see #getType
     * @see #resolveType(ContentResolver)
     */
    CARAPI ResolveType(
        /* [in] */ IContext* context,
        /* [out] */ String* type);

    /**
     * Return the MIME data type of this intent.  If the type field is
     * explicitly set, that is simply returned.  Otherwise, if the data is set,
     * the type of that data is returned.  If neither fields are set, a null is
     * returned.
     *
     * @param resolver A ContentResolver that can be used to determine the MIME
     *                 type of the intent's data.
     *
     * @return The MIME type of this intent.
     *
     * @see #getType
     * @see #resolveType(Context)
     */
    CARAPI ResolveTypeEx(
        /* [in] */ IContentResolver* resolver,
        /* [out] */ String* type);

    /**
     * Return the MIME data type of this intent, only if it will be needed for
     * intent resolution.  This is not generally useful for application code;
     * it is used by the frameworks for communicating with back-end system
     * services.
     *
     * @param resolver A ContentResolver that can be used to determine the MIME
     *                 type of the intent's data.
     *
     * @return The MIME type of this intent, or null if it is unknown or not
     *         needed.
     */
    CARAPI ResolveTypeIfNeeded(
        /* [in] */ IContentResolver* resolver,
        /* [out] */ String* type);

    /**
     * Check if an category exists in the intent.
     *
     * @param category The category to check.
     *
     * @return boolean True if the intent contains the category, else false.
     *
     * @see #getCategories
     * @see #addCategory
     */
    CARAPI HasCategory(
        /* [in] */ const String& category,
        /* [out] */ Boolean* result);

    /**
     * Return the set of all categories in the intent.  If there are no categories,
     * returns NULL.
     *
     * @return Set The set of categories you can examine.  Do not modify!
     *
     * @see #hasCategory
     * @see #addCategory
     */
    CARAPI GetCategories(
        /* [out, callee] */ ArrayOf<String>** categories);

    /**
     * Sets the ClassLoader that will be used when unmarshalling
     * any Parcelable values from the extras of this Intent.
     *
     * @param loader a ClassLoader, or null to use the default loader
     * at the time of unmarshalling.
     */
    CARAPI SetExtrasClassLoader(
        /* [in] */ IClassLoader* loader);

    /**
     * Returns true if an extra value is associated with the given name.
     * @param name the extra's name
     * @return true if the given extra is present.
     */
    CARAPI HasExtra(
        /* [in] */ const String& name,
        /* [out] */ Boolean* result);

    /**
     * Returns true if the Intent's extras contain a parcelled file descriptor.
     * @return true if the Intent contains a parcelled file descriptor.
     */
    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* result);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if none was found.
     *
     * @deprecated
     * @hide
     */
    // @Deprecated
    // public Object getExtra(String name);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra()
     * or the default value if none was found.
     *
     * @see #putExtra(String, boolean)
     */
    CARAPI GetBooleanExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra()
     * or the default value if none was found.
     *
     * @see #putExtra(String, byte)
     */
    CARAPI GetByteExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Byte defaultValue,
        /* [out] */ Byte* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra()
     * or the default value if none was found.
     *
     * @see #putExtra(String, short)
     */
    CARAPI GetInt16ExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Int16 defaultValue,
        /* [out] */ Int16* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra()
     * or the default value if none was found.
     *
     * @see #putExtra(String, char)
     */
    CARAPI GetCharExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Char32 defaultValue,
        /* [out] */ Char32* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra()
     * or the default value if none was found.
     *
     * @see #putExtra(String, int)
     */
    CARAPI GetInt32ExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra()
     * or the default value if none was found.
     *
     * @see #putExtra(String, long)
     */
    CARAPI GetInt64ExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra(),
     * or the default value if no such item is present
     *
     * @see #putExtra(String, float)
     */
    CARAPI GetFloatExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Float defaultValue,
        /* [out] */ Float* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue the value to be returned if no value of the desired
     * type is stored with the given name.
     *
     * @return the value of an item that previously added with putExtra()
     * or the default value if none was found.
     *
     * @see #putExtra(String, double)
     */
    CARAPI GetDoubleExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Double defaultValue,
        /* [out] */ Double* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no String value was found.
     *
     * @see #putExtra(String, String)
     */
    CARAPI GetStringExtra(
        /* [in] */ const String& name,
        /* [out] */ String* value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no CharSequence value was found.
     *
     * @see #putExtra(String, CharSequence)
     */
    CARAPI GetCharSequenceExtra(
        /* [in] */ const String& name,
        /* [out] */ ICharSequence** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no Parcelable value was found.
     *
     * @see #putExtra(String, Parcelable)
     */
    CARAPI GetParcelableExtra(
        /* [in] */ const String& name,
        /* [out] */ IParcelable** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no Parcelable[] value was found.
     *
     * @see #putExtra(String, Parcelable[])
     */
    CARAPI GetParcelableArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<IParcelable*>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no ArrayList<Parcelable> value was found.
     *
     * @see #putParcelableArrayListExtra(String, ArrayList)
     */
    CARAPI GetParcelableArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no Serializable value was found.
     *
     * @see #putExtra(String, Serializable)
     */
    // GetSerializableExtra(
    //     /* [in] */ const String& name,
    //     /* [out] */ ISerializable** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no ArrayList<Integer> value was found.
     *
     * @see #putIntegerArrayListExtra(String, ArrayList)
     */
    CARAPI GetIntegerArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *a
     * @return the value of an item that previously added with putExtra()
     * or null if no ArrayList<String> value was found.
     *
     * @see #putStringArrayListExtra(String, ArrayList)
     */
    CARAPI GetStringArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no ArrayList<CharSequence> value was found.
     *
     * @see #putCharSequenceArrayListExtra(String, ArrayList)
     */
    CARAPI GetCharSequenceArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no boolean array value was found.
     *
     * @see #putExtra(String, boolean[])
     */
    CARAPI GetBooleanArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Boolean>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no byte array value was found.
     *
     * @see #putExtra(String, byte[])
     */
    CARAPI GetByteArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Byte>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no short array value was found.
     *
     * @see #putExtra(String, short[])
     */
    CARAPI GetInt16ArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Int16>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no char array value was found.
     *
     * @see #putExtra(String, char[])
     */
    CARAPI GetCharArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Char32>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no int array value was found.
     *
     * @see #putExtra(String, int[])
     */
    CARAPI GetInt32ArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Int32>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no long array value was found.
     *
     * @see #putExtra(String, long[])
     */
    CARAPI GetInt64ArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Int64>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no float array value was found.
     *
     * @see #putExtra(String, float[])
     */
    CARAPI GetFloatArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Float>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no double array value was found.
     *
     * @see #putExtra(String, double[])
     */
    CARAPI GetDoubleArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Double>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no String array value was found.
     *
     * @see #putExtra(String, String[])
     */
    CARAPI GetStringArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<String>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no CharSequence array value was found.
     *
     * @see #putExtra(String, CharSequence[])
     */
    CARAPI GetCharSequenceArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<ICharSequence*>** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no Bundle value was found.
     *
     * @see #putExtra(String, Bundle)
     */
    CARAPI GetBundleExtra(
        /* [in] */ const String& name,
        /* [out] */ IBundle** value);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     *
     * @return the value of an item that previously added with putExtra()
     * or null if no IBinder value was found.
     *
     * @see #putExtra(String, IBinder)
     *
     * @deprecated
     * @hide
     */
    // @Deprecated
    // public IBinder getIBinderExtra(String name);

    /**
     * Retrieve extended data from the intent.
     *
     * @param name The name of the desired item.
     * @param defaultValue The default value to return in case no item is
     * associated with the key 'name'
     *
     * @return the value of an item that previously added with putExtra()
     * or defaultValue if none was found.
     *
     * @see #putExtra
     *
     * @deprecated
     * @hide
     */
    // @Deprecated
    // public Object getExtra(String name, Object defaultValue);

    /**
     * Retrieves a map of extended data from the intent.
     *
     * @return the map of all extras previously added with putExtra(),
     * or null if none have been added.
     */
    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    /**
     * Retrieve any special flags associated with this intent.  You will
     * normally just set them with {@link #setFlags} and let the system
     * take the appropriate action with them.
     *
     * @return int The currently set flags.
     *
     * @see #setFlags
     */
    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    /**
     * Retrieve the application package name this Intent is limited to.  When
     * resolving an Intent, if non-null this limits the resolution to only
     * components in the given application package.
     *
     * @return The name of the application package for the Intent.
     *
     * @see #resolveActivity
     * @see #setPackage
     */
    CARAPI GetCapsule(
        /* [out] */ String* capsuleName);

    /**
     * Retrieve the concrete component associated with the intent.  When receiving
     * an intent, this is the component that was found to best handle it (that is,
     * yourself) and will always be non-null; in all other cases it will be
     * null unless explicitly set.
     *
     * @return The name of the application component to handle the intent.
     *
     * @see #resolveActivity
     * @see #setComponent
     */
    CARAPI GetComponent(
        /* [out] */ IComponentName** result);

    /**
     * Get the bounds of the sender of this intent, in screen coordinates.  This can be
     * used as a hint to the receiver for animations and the like.  Null means that there
     * is no source bounds.
     */
    CARAPI GetSourceBounds(
        /* [out] */ IRect** result);

    /**
     * Return the Activity component that should be used to handle this intent.
     * The appropriate component is determined based on the information in the
     * intent, evaluated as follows:
     *
     * <p>If {@link #getComponent} returns an explicit class, that is returned
     * without any further consideration.
     *
     * <p>The activity must handle the {@link Intent#CATEGORY_DEFAULT} Intent
     * category to be considered.
     *
     * <p>If {@link #getAction} is non-NULL, the activity must handle this
     * action.
     *
     * <p>If {@link #resolveType} returns non-NULL, the activity must handle
     * this type.
     *
     * <p>If {@link #addCategory} has added any categories, the activity must
     * handle ALL of the categories specified.
     *
     * <p>If {@link #getPackage} is non-NULL, only activity components in
     * that application package will be considered.
     *
     * <p>If there are no activities that satisfy all of these conditions, a
     * null string is returned.
     *
     * <p>If multiple activities are found to satisfy the intent, the one with
     * the highest priority will be used.  If there are multiple activities
     * with the same priority, the system will either pick the best activity
     * based on user preference, or resolve to a system class that will allow
     * the user to pick an activity and forward from there.
     *
     * <p>This method is implemented simply by calling
     * {@link PackageManager#resolveActivity} with the "defaultOnly" parameter
     * true.</p>
     * <p> This API is called for you as part of starting an activity from an
     * intent.  You do not normally need to call it yourself.</p>
     *
     * @param pm The package manager with which to resolve the Intent.
     *
     * @return Name of the component implementing an activity that can
     *         display the intent.
     *
     * @see #setComponent
     * @see #getComponent
     * @see #resolveActivityInfo
     */
    CARAPI ResolveActivity(
        /* [in] */ ICapsuleManager* cm,
        /* [out] */ IComponentName** result);

    /**
     * Resolve the Intent into an {@link ActivityInfo}
     * describing the activity that should execute the intent.  Resolution
     * follows the same rules as described for {@link #resolveActivity}, but
     * you get back the completely information about the resolved activity
     * instead of just its class name.
     *
     * @param pm The package manager with which to resolve the Intent.
     * @param flags Addition information to retrieve as per
     * {@link PackageManager#getActivityInfo(ComponentName, int)
     * PackageManager.getActivityInfo()}.
     *
     * @return PackageManager.ActivityInfo
     *
     * @see #resolveActivity
     */
    CARAPI ResolveActivityInfo(
        /* [in] */ ICapsuleManager* cm,
        /* [in] */ Int32 flags,
        /* [out] */ IActivityInfo** result);

    /**
     * Set the general action to be performed.
     *
     * @param action An action name, such as ACTION_VIEW.  Application-specific
     *               actions should be prefixed with the vendor's package name.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #getAction
     */
    CARAPI SetActionEx(
        /* [in] */ const String& action,
        /* [out] */ IIntent** result);

    /**
     * Set the data this intent is operating on.  This method automatically
     * clears any type that was previously set by {@link #setType}.
     *
     * <p><em>Note: scheme and host name matching in the Android framework is
     * case-sensitive, unlike the formal RFC.  As a result,
     * you should always ensure that you write your Uri with these elements
     * using lower case letters, and normalize any Uris you receive from
     * outside of Android to ensure the scheme and host is lower case.</em></p>
     *
     * @param data The URI of the data this intent is now targeting.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #getData
     * @see #setType
     * @see #setDataAndType
     */
    CARAPI SetDataEx(
        /* [in] */ IUri* data,
        /* [out] */ IIntent** result);

    /**
     * Set an explicit MIME data type.  This is used to create intents that
     * only specify a type and not data, for example to indicate the type of
     * data to return.  This method automatically clears any data that was
     * previously set by {@link #setData}.
     *
     * <p><em>Note: MIME type matching in the Android framework is
     * case-sensitive, unlike formal RFC MIME types.  As a result,
     * you should always write your MIME types with lower case letters,
     * and any MIME types you receive from outside of Android should be
     * converted to lower case before supplying them here.</em></p>
     *
     * @param type The MIME type of the data being handled by this intent.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #getType
     * @see #setData
     * @see #setDataAndType
     */
    CARAPI SetTypeEx(
      /* [in] */ const String& type,
      /* [out] */ IIntent** result);

    /**
     * (Usually optional) Set the data for the intent along with an explicit
     * MIME data type.  This method should very rarely be used -- it allows you
     * to override the MIME type that would ordinarily be inferred from the
     * data with your own type given here.
     *
     * <p><em>Note: MIME type, Uri scheme, and host name matching in the
     * Android framework is case-sensitive, unlike the formal RFC definitions.
     * As a result, you should always write these elements with lower case letters,
     * and normalize any MIME types or Uris you receive from
     * outside of Android to ensure these elements are lower case before
     * supplying them here.</em></p>
     *
     * @param data The URI of the data this intent is now targeting.
     * @param type The MIME type of the data being handled by this intent.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #setData
     * @see #setType
     */
    CARAPI SetDataAndTypeEx(
	    /* [in] */ IUri* data,
	    /* [in] */ const String& type,
	    /* [out] */ IIntent** result);

    /**
     * Add a new category to the intent.  Categories provide additional detail
     * about the action the intent is perform.  When resolving an intent, only
     * activities that provide <em>all</em> of the requested categories will be
     * used.
     *
     * @param category The desired category.  This can be either one of the
     *               predefined Intent categories, or a custom category in your own
     *               namespace.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #hasCategory
     * @see #removeCategory
     */
    CARAPI AddCategoryEx(
        /* [in] */ const String& category,
        /* [out] */ IIntent** result);

    /**
     * Remove an category from an intent.
     *
     * @param category The category to remove.
     *
     * @see #addCategory
     */
    CARAPI RemoveCategory(
        /* [in] */ const String& category);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The boolean data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getBooleanExtra(String, boolean)
     */
    CARAPI PutBooleanExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Boolean value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The byte data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getByteExtra(String, byte)
     */
    CARAPI PutByteExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Byte value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The char data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getCharExtra(String, char)
     */
    CARAPI PutCharExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Char32 value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The short data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getShortExtra(String, short)
     */
    CARAPI PutInt16ExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Int16 value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The integer data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getIntExtra(String, int)
     */
    CARAPI PutInt32ExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Int32 value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The long data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getLongExtra(String, long)
     */
    CARAPI PutInt64ExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Int64 value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The float data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getFloatExtra(String, float)
     */
    CARAPI PutFloatExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Float value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The double data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getDoubleExtra(String, double)
     */
    CARAPI PutDoubleExtraEx(
        /* [in] */ const String& name,
        /* [in] */ Double value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The String data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getStringExtra(String)
     */
    CARAPI PutStringExtraEx(
        /* [in] */ const String& name,
        /* [in] */ const String& value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The CharSequence data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getCharSequenceExtra(String)
     */
    CARAPI PutCharSequenceExtra(
        /* [in] */ const String& name,
        /* [in] */ ICharSequence* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The Parcelable data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getParcelableExtra(String)
     */
    CARAPI PutParcelableExtraEx(
        /* [in] */ const String& name,
        /* [in] */ IParcelable* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The Parcelable[] data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getParcelableArrayExtra(String)
     */
    CARAPI PutParcelableArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<IParcelable*>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The ArrayList<Parcelable> data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getParcelableArrayListExtra(String)
     */
    CARAPI PutParcelableArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The ArrayList<Integer> data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getIntegerArrayListExtra(String)
     */
    CARAPI PutIntegerArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The ArrayList<String> data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getStringArrayListExtra(String)
     */
    CARAPI PutStringArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The ArrayList<CharSequence> data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getCharSequenceArrayListExtra(String)
     */
    CARAPI PutCharSequenceArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The Serializable data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getSerializableExtra(String)
     */
    // PutSerializableExtra(
    //     /* [in] */ const String& name,
    //     /* [in] */ ISerializable* value,
    //     /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The boolean array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getBooleanArrayExtra(String)
     */
    CARAPI PutBooleanArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Boolean>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The byte array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getByteArrayExtra(String)
     */
    CARAPI PutByteArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The short array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getShortArrayExtra(String)
     */
    CARAPI PutInt16ArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Int16>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The char array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getCharArrayExtra(String)
     */
    CARAPI PutCharArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Char32>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The int array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getIntArrayExtra(String)
     */
    CARAPI PutInt32ArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Int32>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The byte array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getLongArrayExtra(String)
     */
    CARAPI PutInt64ArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Int64>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The float array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getFloatArrayExtra(String)
     */
    CARAPI PutFloatArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Float>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The double array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getDoubleArrayExtra(String)
     */
    CARAPI PutDoubleArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Double>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The String array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getStringArrayExtra(String)
     */
    CARAPI PutStringArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The CharSequence array data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getCharSequenceArrayExtra(String)
     */
    CARAPI PutCharSequenceArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<ICharSequence*>* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The Bundle data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getBundleExtra(String)
     */
    CARAPI PutBundleExtra(
        /* [in] */ const String& name,
        /* [in] */ IBundle* value,
        /* [out] */ IIntent** result);

    /**
     * Add extended data to the intent.  The name must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param name The name of the extra data, with package prefix.
     * @param value The IBinder data value.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #putExtras
     * @see #removeExtra
     * @see #getIBinderExtra(String)
     *
     * @deprecated
     * @hide
     */
    // @Deprecated
    // public Intent putExtra(String name, IBinder value);

    /**
     * Copy all extras in 'src' in to this intent.
     *
     * @param src Contains the extras to copy.
     *
     * @see #putExtra
     */
    CARAPI PutExtras2(
        /* [in] */ IIntent* src,
        /* [out] */ IIntent** result);

    /**
     * Add a set of extended data to the intent.  The keys must include a package
     * prefix, for example the app com.android.contacts would use names
     * like "com.android.contacts.ShowAll".
     *
     * @param extras The Bundle of extras to add to this intent.
     *
     * @see #putExtra
     * @see #removeExtra
     */
    CARAPI PutExtrasEx(
        /* [in] */ IBundle* extras,
        /* [out] */ IIntent** result);

    /**
     * Completely replace the extras in the Intent with the extras in the
     * given Intent.
     *
     * @param src The exact extras contained in this Intent are copied
     * into the target intent, replacing any that were previously there.
     */
    CARAPI ReplaceExtrasEx(
        /* [in] */ IIntent* src,
        /* [out] */ IIntent** result);

    /**
     * Completely replace the extras in the Intent with the given Bundle of
     * extras.
     *
     * @param extras The new set of extras in the Intent, or null to erase
     * all extras.
     */
    CARAPI ReplaceExtras2(
        /* [in] */ IBundle* extras,
        /* [out] */ IIntent** result);

    /**
     * Remove extended data from the intent.
     *
     * @see #putExtra
     */
    CARAPI RemoveExtra(
        /* [in] */ const String& name);

    /**
     * Set special flags controlling how this intent is handled.  Most values
     * here depend on the type of component being executed by the Intent,
     * specifically the FLAG_ACTIVITY_* flags are all for use with
     * {@link Context#startActivity Context.startActivity()} and the
     * FLAG_RECEIVER_* flags are all for use with
     * {@link Context#sendBroadcast(Intent) Context.sendBroadcast()}.
     *
     * <p>See the <a href="{@docRoot}guide/topics/fundamentals.html#acttask">Application Fundamentals:
     * Activities and Tasks</a> documentation for important information on how some of these options impact
     * the behavior of your application.
     *
     * @param flags The desired flags.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #getFlags
     * @see #addFlags
     *
     * @see #FLAG_GRANT_READ_URI_PERMISSION
     * @see #FLAG_GRANT_WRITE_URI_PERMISSION
     * @see #FLAG_DEBUG_LOG_RESOLUTION
     * @see #FLAG_FROM_BACKGROUND
     * @see #FLAG_ACTIVITY_BROUGHT_TO_FRONT
     * @see #FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET
     * @see #FLAG_ACTIVITY_CLEAR_TOP
     * @see #FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS
     * @see #FLAG_ACTIVITY_FORWARD_RESULT
     * @see #FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY
     * @see #FLAG_ACTIVITY_MULTIPLE_TASK
     * @see #FLAG_ACTIVITY_NEW_TASK
     * @see #FLAG_ACTIVITY_NO_HISTORY
     * @see #FLAG_ACTIVITY_NO_USER_ACTION
     * @see #FLAG_ACTIVITY_PREVIOUS_IS_TOP
     * @see #FLAG_ACTIVITY_RESET_TASK_IF_NEEDED
     * @see #FLAG_ACTIVITY_SINGLE_TOP
     * @see #FLAG_RECEIVER_REGISTERED_ONLY
     */
    CARAPI SetFlagsEx(
        /* [in] */ Int32 flags,
        /* [out] */ IIntent** result);

    /**
     * Add additional flags to the intent (or with existing flags
     * value).
     *
     * @param flags The new flags to set.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #setFlags
     */
    CARAPI AddFlagsEx(
        /* [in] */ Int32 flags,
        /* [out] */ IIntent** result);

    /**
     * (Usually optional) Set an explicit application package name that limits
     * the components this Intent will resolve to.  If left to the default
     * value of null, all components in all applications will considered.
     * If non-null, the Intent can only match the components in the given
     * application package.
     *
     * @param packageName The name of the application package to handle the
     * intent, or null to allow any application package.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #getPackage
     * @see #resolveActivity
     */
    CARAPI SetCapsule(
        /* [in] */ const String& capsuleName);

    /**
     * (Usually optional) Explicitly set the component to handle the intent.
     * If left with the default value of null, the system will determine the
     * appropriate class to use based on the other fields (action, data,
     * type, categories) in the Intent.  If this class is defined, the
     * specified class will always be used regardless of the other fields.  You
     * should only set this value when you know you absolutely want a specific
     * class to be used; otherwise it is better to let the system find the
     * appropriate class so that you will respect the installed applications
     * and user preferences.
     *
     * @param component The name of the application component to handle the
     * intent, or null to let the system find one for you.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #setClass
     * @see #setClassName(Context, String)
     * @see #setClassName(String, String)
     * @see #getComponent
     * @see #resolveActivity
     */
    CARAPI SetComponentEx(
        /* [in] */ IComponentName* component,
        /* [out] */ IIntent** result);

    /**
     * Convenience for calling {@link #setComponent} with an
     * explicit class name.
     *
     * @param packageContext A Context of the application package implementing
     * this class.
     * @param className The name of a class inside of the application package
     * that will be used as the component for this Intent.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #setComponent
     * @see #setClass
     */
    CARAPI SetClassName2(
        /* [in] */ IContext* packageContext,
        /* [in] */ const String& className,
        /* [out] */ IIntent** result);

    /**
     * Convenience for calling {@link #setComponent} with an
     * explicit application package name and class name.
     *
     * @param packageName The name of the package implementing the desired
     * component.
     * @param className The name of a class inside of the application package
     * that will be used as the component for this Intent.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #setComponent
     * @see #setClass
     */
    CARAPI SetClassNameEx(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className,
        /* [out] */ IIntent** result);

    /**
     * Convenience for calling {@link #setComponent(ComponentName)} with the
     * name returned by a {@link Class} object.
     *
     * @param packageContext A Context of the application package implementing
     * this class.
     * @param cls The class name to set, equivalent to
     *            <code>setClassName(context, cls.getName())</code>.
     *
     * @return Returns the same Intent object, for chaining multiple calls
     * into a single statement.
     *
     * @see #setComponent
     */
    // TODO: ALEX
    // public Intent setClass(Context packageContext, Class<?> cls);

    /**
     * Set the bounds of the sender of this intent, in screen coordinates.  This can be
     * used as a hint to the receiver for animations and the like.  Null means that there
     * is no source bounds.
     */
    CARAPI SetSourceBounds(
        /* [in] */ IRect* r);

    /**
     * Copy the contents of <var>other</var> in to this object, but only
     * where fields are not defined by this object.  For purposes of a field
     * being defined, the following pieces of data in the Intent are
     * considered to be separate fields:
     *
     * <ul>
     * <li> action, as set by {@link #setAction}.
     * <li> data URI and MIME type, as set by {@link #setData(Uri)},
     * {@link #setType(String)}, or {@link #setDataAndType(Uri, String)}.
     * <li> categories, as set by {@link #addCategory}.
     * <li> package, as set by {@link #setPackage}.
     * <li> component, as set by {@link #setComponent(ComponentName)} or
     * related methods.
     * <li> source bounds, as set by {@link #setSourceBounds}
     * <li> each top-level name in the associated extras.
     * </ul>
     *
     * <p>In addition, you can use the {@link #FILL_IN_ACTION},
     * {@link #FILL_IN_DATA}, {@link #FILL_IN_CATEGORIES}, {@link #FILL_IN_PACKAGE},
     * and {@link #FILL_IN_COMPONENT} to override the restriction where the
     * corresponding field will not be replaced if it is already set.
     *
     * <p>Note: The component field will only be copied if {@link #FILL_IN_COMPONENT} is explicitly
     * specified.
     *
     * <p>For example, consider Intent A with {data="foo", categories="bar"}
     * and Intent B with {action="gotit", data-type="some/thing",
     * categories="one","two"}.
     *
     * <p>Calling A.fillIn(B, Intent.FILL_IN_DATA) will result in A now
     * containing: {action="gotit", data-type="some/thing",
     * categories="bar"}.
     *
     * @param other Another Intent whose values are to be used to fill in
     * the current one.
     * @param flags Options to control which fields can be filled in.
     *
     * @return Returns a bit mask of {@link #FILL_IN_ACTION},
     * {@link #FILL_IN_DATA}, {@link #FILL_IN_CATEGORIES}, {@link #FILL_IN_PACKAGE},
     * and {@link #FILL_IN_COMPONENT} indicating which fields were changed.
     */
    CARAPI FillIn(
        /* [in] */ IIntent* other,
        /* [in] */ Int32 flags,
        /* [out] */ Int32* result);

    /**
     * Determine if two intents are the same for the purposes of intent
     * resolution (filtering). That is, if their action, data, type,
     * class, and categories are the same.  This does <em>not</em> compare
     * any extra data included in the intents.
     *
     * @param other The other Intent to compare against.
     *
     * @return Returns true if action, data, type, class, and categories
     *         are the same.
     */
    CARAPI FilterEquals(
        /* [in] */ IIntent* other,
        /* [out] */ Boolean* isEqual);

    /**
     * Generate hash code that matches semantics of filterEquals().
     *
     * @return Returns the hash value of the action, data, type, class, and
     *         categories.
     *
     * @see #filterEquals
     */
    CARAPI FilterHashCode(
        /* [out] */ Int32* result);

    // @Override
    // public String toString();

    /** @hide */
    CARAPI ToShortString(
        /* [in] */ Boolean comp,
        /* [in] */ Boolean extras,
        /* [out] */ String* result);

    /** @hide */
    CARAPI ToShortString2(
        /* [in] */ const String& b,
        /* [in] */ Boolean comp,
        /* [in] */ Boolean extras);

    /**
     * Call {@link #toUri} with 0 flags.
     * @deprecated Use {@link #toUri} instead.
     */
    // @Deprecated
    // public String toURI();

    /**
     * Convert this Intent into a String holding a URI representation of it.
     * The returned URI string has been properly URI encoded, so it can be
     * used with {@link Uri#parse Uri.parse(String)}.  The URI contains the
     * Intent's data as the base URI, with an additional fragment describing
     * the action, categories, type, flags, package, component, and extras.
     *
     * <p>You can convert the returned string back to an Intent with
     * {@link #getIntent}.
     *
     * @param flags Additional operating flags.  Either 0 or
     * {@link #URI_INTENT_SCHEME}.
     *
     * @return Returns a URI encoding URI string describing the entire contents
     * of the Intent.
     */
    CARAPI ToUri(
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    CARAPI SetComponent(
        /* [in] */ IComponentName* component);

    CARAPI SetAction(
        /* [in] */ const String& action);

    CARAPI SetData(
        /* [in] */ IUri *pData);

    CARAPI SetType(
        /*[in]*/ const String& type);

    CARAPI SetDataAndType(
        /*[in]*/ IUri* data,
        /*[in]*/ const String& type);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI AddFlags(
        /* [in] */ Int32 flags);

    CARAPI SetClassName(
        /* [in] */ const String& capsuleName,
        /* [in] */ const String& className);

    CARAPI AddCategory(
        /* [in] */ const String& category);

    CARAPI PutBooleanExtra(
        /* [in] */ const String& name,
        /* [out] */ Boolean value);

    CARAPI GetBooleanExtra(
        /* [in] */ const String& name,
        /* [out] */ Boolean *pValue);

    CARAPI PutByteExtra(
        /* [in] */ const String& name,
        /* [in] */ Byte value);

    CARAPI GetByteExtra(
        /* [in] */ const String& name,
        /* [out] */ Byte * pValue);

    CARAPI PutCharExtra(
        /* [in] */ const String& name,
        /* [in] */ Char32 value);

    CARAPI GetCharExtra(
        /* [in] */ const String& name,
        /* [out] */ Char32 * pValue);

    CARAPI PutInt16Extra(
        /* [in] */ const String& name,
        /* [in] */ Int16 value);

    CARAPI GetInt16Extra(
        /* [in] */ const String& name,
        /* [out] */ Int16 * pValue);

    CARAPI PutInt32Extra(
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    CARAPI GetInt32Extra(
        /* [in] */ const String& name,
        /* [out] */ Int32 *pValue);

    CARAPI PutInt64Extra(
        /* [in] */ const String& name,
        /* [in] */ Int64 value);

    CARAPI GetInt64Extra(
        /* [in] */ const String& name,
        /* [out] */ Int64 * pValue);

    CARAPI PutFloatExtra(
        /* [in] */ const String& name,
        /* [in] */ Float value);

    CARAPI GetFloatExtra(
        /* [in] */ const String& name,
        /* [out] */ Float * pValue);

    CARAPI PutDoubleExtra(
        /* [in] */ const String& name,
        /* [in] */ Double value);

    CARAPI GetDoubleExtra(
        /* [in] */ const String& name,
        /* [out] */ Double * pValue);

    CARAPI PutStringExtra(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    CARAPI PutExtras(
        /* [in] */ IBundle * pExtras);

    CARAPI PutParcelableExtra(
        /* [in] */ const String& name,
        /* [in] */ IParcelable* value);

    /**
     * Completely replace the extras in the Intent with the extras in the
     * given Intent.
     *
     * @param src The exact extras contained in this Intent are copied
     * into the target intent, replacing any that were previously there.
     */
    CARAPI ReplaceExtras(
        /* [in] */ IIntent* src);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IIntent* intent);

    CARAPI constructor(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean all);

    CARAPI constructor(
        /* [in] */ const String& action);

    CARAPI constructor(
        /* [in] */ const String& action,
        /* [in] */ IUri* uri);

    CARAPI constructor(
        /* [in] */ Handle32 intent);

private:
/*
    android intent:

    private String mAction;
    private Uri mData;
    private String mType;
    private String mPackage;
    private ComponentName mComponent;
    private Int32 mFlags;
    private HashSet<String> mCategories;
    private Bundle mExtras;
*/

    String mAction;
    AutoPtr<IUri> mData;
    String mType;
    String mCapsule;
    AutoPtr<CComponentName> mComponent;
    Int32 mFlags;
    Set<String>* mCategories;
    AutoPtr<IBundle> mExtras;
    AutoPtr<IRect> mSourceBounds;
};

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<CIntent::FilterComparison*>
{
    size_t operator()(CIntent::FilterComparison* filter) const
    {
        assert(filter != NULL);
        return (size_t)filter->GetHashCode();
    }
};

template<> struct EqualTo<CIntent::FilterComparison*>
{
    Boolean operator()(CIntent::FilterComparison* x,
                       CIntent::FilterComparison* y) const
    {
        assert(x != NULL);
        return x->Equals(y);
    }
};

_ELASTOS_NAMESPACE_END

#endif // __CINTENT_H__
