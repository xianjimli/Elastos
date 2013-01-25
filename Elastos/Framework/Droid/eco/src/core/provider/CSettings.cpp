#include "provider/CSettings.h"
#include "utils/Config.h"
#include "net/Uri.h"
#include "content/CContentValues.h"
#include <Slogger.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const String Settings_JID_RESOURCE_PREFIX = String("android");
const String Settings_TAG = String("CSettings");
const Boolean CSettings::LOCAL_LOGV = Config::LOGV || FALSE;


//const String CSettings::NameValueCache::SELECT_VALUE[] = { CSettingsNameValueTable_VALUE };
//const String CSettings::NameValueCache::NAME_EQ_PLACEHOLDER = String("name=?");


CSettings::NameValueCache::NameValueCache(
    /*[in]*/ const String& versionSystemProperty,
    /*[in]*/ IUri* uri,
    /*[in]*/ const String& callCommand):
    mVersionSystemProperty(versionSystemProperty),
    mUri(uri),
    mCallCommand(callCommand)
{
    mValuesVersion = 0;
    mContentProvider = NULL;

    // Must synchronize on 'this' to access mValues and mValuesVersion.
    const HashMap<String, String>* mValues = new HashMap<String, String>();
}

ECode CSettings::NameValueCache::GetString(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ const String& name,
    /*[out]*/ String* string)
{
/*            long newValuesVersion = SystemProperties.getLong(mVersionSystemProperty, 0);

            synchronized (this) {
                if (mValuesVersion != newValuesVersion) {
                    if (LOCAL_LOGV) {
                        Log.v(TAG, "invalidate [" + mUri.getLastPathSegment() + "]: current " +
                                newValuesVersion + " != cached " + mValuesVersion);
                    }

                    mValues.clear();
                    mValuesVersion = newValuesVersion;
                }

                if (mValues.containsKey(name)) {
                    return mValues.get(name);  // Could be null, that's OK -- negative caching
                }
            }

            IContentProvider cp = null;
            synchronized (this) {
                cp = mContentProvider;
                if (cp == null) {
                    cp = mContentProvider = cr.acquireProvider(mUri.getAuthority());
                }
            }

            // Try the fast path first, not using query().  If this
            // fails (alternate Settings provider that doesn't support
            // this interface?) then we fall back to the query/table
            // interface.
            if (mCallCommand != null) {
                try {
                    Bundle b = cp.call(mCallCommand, name, null);
                    if (b != null) {
                        String value = b.getPairValue();
                        synchronized (this) {
                            mValues.put(name, value);
                        }
                        return value;
                    }
                    // If the response Bundle is null, we fall through
                    // to the query interface below.
                } catch (RemoteException e) {
                    // Not supported by the remote side?  Fall through
                    // to query().
                }
            }

            Cursor c = null;
            try {
                c = cp.query(mUri, SELECT_VALUE, NAME_EQ_PLACEHOLDER,
                             new String[]{name}, null);
                if (c == null) {
                    Log.w(TAG, "Can't get key " + name + " from " + mUri);
                    return null;
                }

                String value = c.moveToNext() ? c.getString(0) : null;
                synchronized (this) {
                    mValues.put(name, value);
                }
                if (LOCAL_LOGV) {
                    Log.v(TAG, "cache miss [" + mUri.getLastPathSegment() + "]: " +
                            name + " = " + (value == null ? "(null)" : value));
                }
                return value;
            } catch (RemoteException e) {
                Log.w(TAG, "Can't get key " + name + " from " + mUri, e);
                return null;  // Return null, but don't cache it.
            } finally {
                if (c != null) c.close();
            }*/
    return E_NOT_IMPLEMENTED;
}
