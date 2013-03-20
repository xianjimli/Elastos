
#include "webkit/CPlugin.h"
#include "../../../res/gen/R.h"

PInterface CPlugin::DefaultClickHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IPluginPreferencesClickHandler*)this;
    }
    else if (riid == EIID_IPluginPreferencesClickHandler) {
        return (IPluginPreferencesClickHandler*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (IDialogInterfaceOnClickListener*)this;
    }
    return NULL;
}

UInt32 CPlugin::DefaultClickHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPlugin::DefaultClickHandler::Release()
{
    return ElRefBase::Release();
}

ECode CPlugin::DefaultClickHandler::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IPluginPreferencesClickHandler*)this) {
        *iID = EIID_IPluginPreferencesClickHandler;
    }
    if (Object == (IInterface*)(IDialogInterfaceOnClickListener*)this) {
        *iID = EIID_IDialogInterfaceOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CPlugin::DefaultClickHandler::HandleClickEvent(
    /* [in] */ IContext* context)
{
    // Show a simple popup dialog containing the description
    // string of the plugin.
    if (mDialog == NULL) {
        AutoPtr<IAlertDialogBuilder> builder;        
        ECode ec = CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);
        if(FAILED(ec)){
            return ec;
        }
        builder -> SetTitleEx(mName.Get());
        builder -> SetMessageEx(mDescription.Get());
        builder -> SetPositiveButtonEx(mRStringOk.Get(), this);
        builder -> SetCancelable(false); 

        AutoPtr<IAlertDialog> tDialog;
        builder -> Show( (IAlertDialog**)&tDialog );
        mDialog = tDialog;
    }
    return NOERROR;
}

ECode CPlugin::DefaultClickHandler::OnClick(
    /* [in] */ IDialogInterface* dialog, 
    /* [in] */ Int32 which)
{
    mDialog -> Dismiss();
    mDialog -> Release();
    mDialog = NULL;
    return NOERROR;
}

ECode CPlugin::ToString(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CPlugin::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CPlugin::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath;
    return NOERROR;
}

ECode CPlugin::GetFileName(
    /* [out] */ String* fileName)
{
    VALIDATE_NOT_NULL(fileName);
    *fileName = mFileName;
    return NOERROR;
}

ECode CPlugin::GetDescription(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    *description = mDescription;
    return NOERROR;
}

ECode CPlugin::SetName(
    /* [in] */ String name)
{
    mName = name;
    return NOERROR;
}

ECode CPlugin::SetPath(
    /* [in] */ String path)
{
    mPath = path;
    return NOERROR;
}

ECode CPlugin::SetFileName(
    /* [in] */ String fileName)
{
    mFileName = fileName;
    return NOERROR;
}

ECode CPlugin::SetDescription(
    /* [in] */ String description)
{
    mDescription = description;
    return NOERROR;
}

ECode CPlugin::SetClickHandler(
    /* [in] */ IPluginPreferencesClickHandler* handler)
{
    mHandler = handler;
    return NOERROR;
}

ECode CPlugin::DispatchClickEvent(
    /* [in] */ IContext* context)
{
    if(mHandler != NULL){
        mHandler -> HandleClickEvent(context);
    }
    return NOERROR;
}

ECode CPlugin::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& path,
    /* [in] */ const String& fileName,
    /* [in] */ const String& description)
{
    mName = name;
    mPath = path;
    mFileName = fileName;
    mDescription = description;    

    CPlugin::DefaultClickHandler * pDch = new CPlugin::DefaultClickHandler();
    mHandler = (IPluginPreferencesClickHandler*)pDch;    
    pDch -> mName = (ICharSequence *)&name;
    pDch -> mDescription = (ICharSequence *)&description;
    pDch -> mRStringOk = (ICharSequence *)(R::id::ok);     //RStringOK = JAVA: R.string.ok

    return NOERROR;
}

