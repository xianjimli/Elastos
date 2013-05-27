
#include "webkit/CPlugin.h"
#include "../../../res/gen/R.h"

ECode CPlugin::ToString(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name =  Plugin::ToString();
    return NOERROR;
}

ECode CPlugin::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = Plugin::GetName();
    return NOERROR;
}

ECode CPlugin::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = Plugin::GetPath();
    return NOERROR;
}

ECode CPlugin::GetFileName(
    /* [out] */ String* fileName)
{
    VALIDATE_NOT_NULL(fileName);
    *fileName = Plugin::GetFileName();
    return NOERROR;
}

ECode CPlugin::GetDescription(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    *description = Plugin::GetDescription();
    return NOERROR;
}

ECode CPlugin::SetName(
    /* [in] */ const String& name)
{
    Plugin::SetName(name);
    return NOERROR;
}

ECode CPlugin::SetPath(
    /* [in] */ const String& path)
{
    Plugin::SetPath(path);
    return NOERROR;
}

ECode CPlugin::SetFileName(
    /* [in] */ const String& fileName)
{
    Plugin::SetFileName(fileName);
    return NOERROR;
}

ECode CPlugin::SetDescription(
    /* [in] */ const String& description)
{
    Plugin::SetDescription(description);
    return NOERROR;
}

ECode CPlugin::SetClickHandler(
    /* [in] */ IPluginPreferencesClickHandler* handler)
{
    Plugin::SetClickHandler(handler);
    return NOERROR;
}

ECode CPlugin::DispatchClickEvent(
    /* [in] */ IContext* context)
{
    Plugin::DispatchClickEvent(context);
    return NOERROR;
}

ECode CPlugin::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& path,
    /* [in] */ const String& fileName,
    /* [in] */ const String& description)
{
    Plugin::Init(name, path, fileName, description);
    return NOERROR;
}

