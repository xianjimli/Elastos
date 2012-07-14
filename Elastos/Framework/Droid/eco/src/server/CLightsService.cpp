
#include "server/CLightsService.h"

const Int32 CLightsService::LIGHT_ID_BACKLIGHT;
const Int32 CLightsService::LIGHT_ID_KEYBOARD;
const Int32 CLightsService::LIGHT_ID_BUTTONS;
const Int32 CLightsService::LIGHT_ID_BATTERY;
const Int32 CLightsService::LIGHT_ID_NOTIFICATIONS;
const Int32 CLightsService::LIGHT_ID_ATTENTION;
const Int32 CLightsService::LIGHT_ID_BLUETOOTH;
const Int32 CLightsService::LIGHT_ID_WIFI;
const Int32 CLightsService::LIGHT_ID_COUNT;
const Int32 CLightsService::LIGHT_FLASH_NONE;
const Int32 CLightsService::LIGHT_FLASH_TIMED;
const Int32 CLightsService::LIGHT_FLASH_HARDWARE;
const Int32 CLightsService::BRIGHTNESS_MODE_USER;
const Int32 CLightsService::BRIGHTNESS_MODE_SENSOR;

ECode CLightsService::constructor(
    /* [in] */ IContext* ctx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLightsService::GetLight(
    /*[in] */ Int32 id,
    /*[out] */ ILight** light)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
