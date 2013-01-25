#ifndef __CSETTINGS_H__
#define __CSETTINGS_H__

#include "_CSettings.h"
#include "ext/frameworkext.h"
//#include "provider/CSettingsSystem.h"
//#include "provider/CSettingsSecure.h"
#include <elastos/HashSet.h>
#include <elastos/HashMap.h>
#include "Elastos.Framework.Core.h"

using namespace Elastos;

/**
 * The Settings provider contains global system-level device preferences.
 */
CarClass(CSettings)
{
public:
    //friend class CSettingsSystem;
    //friend class CSettingsSecure;

    CARAPI GetGTalkDeviceId(
        /*[in]*/ Int64 androidId,
        /*[out]*/ String* mString);

private:
    static const String JID_RESOURCE_PREFIX;
    static const String TAG;
    static const Boolean LOCAL_LOGV;

    // Thread-safe.
    class NameValueCache
    {
    public:
        NameValueCache(
            /*[in]*/ const String& versionSystemProperty,
            /*[in]*/ IUri* uri,
            /*[in]*/ const String& callCommand);

        CARAPI GetString(
            /*[in]*/ IContentResolver* cr,
            /*[in]*/ const String& name,
            /*[out]*/ String* string);



    private:
        const String mVersionSystemProperty;
        const IUri* mUri;
        static const String SELECT_VALUE[];
        static const String NAME_EQ_PLACEHOLDER;
        const HashMap< String, String >* mValues;

        long mValuesVersion;

        IContentProvider* mContentProvider;

        // The method we'll call (or null, to not use) on the provider
        // for the fast path of retrieving settings.
        const String mCallCommand;

    };




};

#endif //__CSETTINGS_H__
