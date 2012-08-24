
#ifndef __CLIGHTSSERVICE_H__
#define __CLIGHTSSERVICE_H__

#include "_CLightsService.h"

using namespace Elastos;

CarClass(CLightsService)
{
public:
    static const Int32 LIGHT_ID_BACKLIGHT = 0;
    static const Int32 LIGHT_ID_KEYBOARD = 1;
    static const Int32 LIGHT_ID_BUTTONS = 2;
    static const Int32 LIGHT_ID_BATTERY = 3;
    static const Int32 LIGHT_ID_NOTIFICATIONS = 4;
    static const Int32 LIGHT_ID_ATTENTION = 5;
    static const Int32 LIGHT_ID_BLUETOOTH = 6;
    static const Int32 LIGHT_ID_WIFI = 7;
    static const Int32 LIGHT_ID_COUNT = 8;

    static const Int32 LIGHT_FLASH_NONE = 0;
    static const Int32 LIGHT_FLASH_TIMED = 1;
    static const Int32 LIGHT_FLASH_HARDWARE = 2;

    /**
     * Light brightness is managed by a user setting.
     */
    static const Int32 BRIGHTNESS_MODE_USER = 0;

    /**
     * Light brightness is managed by a light sensor.
     */
    static const Int32 BRIGHTNESS_MODE_SENSOR = 1;

public:
    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI GetLight(
        /*[in] */ Int32 id,
        /*[out] */ ILight** light);

private:
    // TODO: Add your private member variables here.
};

#endif // __CLIGHTSSERVICE_H__

