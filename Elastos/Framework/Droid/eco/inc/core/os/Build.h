
#ifndef __HH_BUILD_H
#define __HH_BUILD_H

#include <elastos.h>
using namespace Elastos;

/**
 * Information about the current build, extracted from system properties.
 */
class Build
{
public:
    /** Various version strings. */
    class VERSION
    {
    public:
        /**
         * The internal value used by the underlying source control to
         * represent this build.  E.g., a perforce changelist number
         * or a git hash.
         */
//	        static const String INCREMENTAL = getString("ro.build.version.incremental");

        /**
         * The user-visible version string.  E.g., "1.0" or "3.4b5".
         */
//	        static const String RELEASE = getString("ro.build.version.release");

        /**
         * The user-visible SDK version of the framework in its raw String
         * representation; use {@link #SDK_INT} instead.
         *
         * @deprecated Use {@link #SDK_INT} to easily get this as an integer.
         */
//	        static const String SDK = getString("ro.build.version.sdk");

        /**
         * The user-visible SDK version of the framework; its possible
         * values are defined in {@link Build.VERSION_CODES}.
         */
        static const Int32 SDK_INT;

        /**
         * The current development codename, or the string "REL" if this is
         * a release build.
         */
        static const String CODENAME;
    };

    /**
     * Enumeration of the currently known SDK version codes.  These are the
     * values that can be found in {@link VERSION#SDK}.  Version numbers
     * increment monotonically with each official platform release.
     */
    class VERSION_CODES
    {
    public:
        /**
         * Magic version number for a current development build, which has
         * not yet turned into an official release.
         */
        static const Int32 CUR_DEVELOPMENT = 10000;

        /**
         * October 2008: The original, first, version of Android.  Yay!
         */
        static const Int32 BASE = 1;

        /**
         * February 2009: First Android update, officially called 1.1.
         */
        static const Int32 BASE_1_1 = 2;

        /**
         * May 2009: Android 1.5.
         */
        static const Int32 CUPCAKE = 3;

        /**
         * September 2009: Android 1.6.
         *
         * <p>Applications targeting this or a later release will get these
         * new changes in behavior:</p>
         * <ul>
         * <li> They must explicitly request the
         * {@link android.Manifest.permission#WRITE_EXTERNAL_STORAGE} permission to be
         * able to modify the contents of the SD card.  (Apps targeting
         * earlier versions will always request the permission.)
         * <li> They must explicitly request the
         * {@link android.Manifest.permission#READ_PHONE_STATE} permission to be
         * able to be able to retrieve phone state info.  (Apps targeting
         * earlier versions will always request the permission.)
         * <li> They are assumed to support different screen densities and
         * sizes.  (Apps targeting earlier versions are assumed to only support
         * medium density normal size screens unless otherwise indicated).
         * They can still explicitly specify screen support either way with the
         * supports-screens manifest tag.
         * </ul>
         */
        static const Int32 DONUT = 4;

        /**
         * November 2009: Android 2.0
         *
         * <p>Applications targeting this or a later release will get these
         * new changes in behavior:</p>
         * <ul>
         * <li> The {@link android.app.Service#onStartCommand
         * Service.onStartCommand} function will return the new
         * {@link android.app.Service#START_STICKY} behavior instead of the
         * old compatibility {@link android.app.Service#START_STICKY_COMPATIBILITY}.
         * <li> The {@link android.app.Activity} class will now execute back
         * key presses on the key up instead of key down, to be able to detect
         * canceled presses from virtual keys.
         * <li> The {@link android.widget.TabWidget} class will use a new color scheme
         * for tabs. In the new scheme, the foreground tab has a medium gray background
         * the background tabs have a dark gray background.
         * </ul>
         */
        static const Int32 ECLAIR = 5;

        /**
         * December 2009: Android 2.0.1
         */
        static const Int32 ECLAIR_0_1 = 6;

        /**
         * January 2010: Android 2.1
         */
        static const Int32 ECLAIR_MR1 = 7;

        /**
         * June 2010: Android 2.2
         */
        static const Int32 FROYO = 8;

        /**
         * November 2010: Android 2.3
         */
        static const Int32 GINGERBREAD = 9;

        /**
         * Newest version of Android, version 2.3.3.
         */
        static const Int32 GINGERBREAD_MR1 = 10;
    };

private:
    // return value is buffered.
    static CARAPI_(String) GetString(
        /* [in] */ const char* property);

    static CARAPI_(Int64) GetLong(
        /* [in] */ const char* property);

public:
    /** Value used for when a build property is unknown. */
    static const char* UNKNOWN;

    /** Either a changelist number, or a label like "M4-rc20". */
    static const String ID;

    /** A build ID string meant for displaying to the user */
    static const String DISPLAY;

    /** The name of the overall product. */
    static const String PRODUCT;

    /** The name of the industrial design. */
    static const String DEVICE;

    /** The name of the underlying board, like "goldfish". */
    static const String BOARD;

    /** The name of the instruction set (CPU type + ABI convention) of native code. */
    static const String CPU_ABI;

    /** The name of the second instruction set (CPU type + ABI convention) of native code. */
    static const String CPU_ABI2;

    /** The manufacturer of the product/hardware. */
    static const String MANUFACTURER;

    /** The brand (e.g., carrier) the software is customized for, if any. */
    static const String BRAND;

    /** The end-user-visible name for the end product. */
    static const String MODEL;

    /** The system bootloader version number. */
    static const String BOOTLOADER;

    /** The radio firmware version number. */
    static const String RADIO;

    /** The name of the hardware (from the kernel command line or /proc). */
    static const String HARDWARE;

    /** A hardware serial number, if available.  Alphanumeric only, case-insensitive. */
    static const String SERIAL;

public:
    /** The type of build, like "user" or "eng". */
    static const String TYPE;

    /** Comma-separated tags describing the build, like "unsigned,debug". */
    static const String TAGS;

    /** A string that uniquely identifies this build.  Do not attempt to parse this value. */
    static const String FINGERPRINT;

    // The following properties only make sense for internal engineering builds.
    static const Int64 TIME;
    static const String USER;
    static const String HOST;
};

#endif // __HH_BUILD_H
