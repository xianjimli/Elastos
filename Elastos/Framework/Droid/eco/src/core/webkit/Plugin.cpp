#include "webkit/Plugin.h"
#include "../../../res/gen/R.h"

PInterface Plugin::DefaultClickHandler::Probe(
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

UInt32 Plugin::DefaultClickHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Plugin::DefaultClickHandler::Release()
{
    return ElRefBase::Release();
}

ECode Plugin::DefaultClickHandler::GetInterfaceID(
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

CARAPI Plugin::DefaultClickHandler::HandleClickEvent(
            /* [in] */ IContext* context)
{
    // Show a simple popup dialog containing the description
    // string of the plugin.    
    if (mDialog == NULL) {
        AutoPtr<IAlertDialogBuilder> builder;        
        ECode ec = CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);
        if(FAILED(ec)) {
            return ec;
        }
        builder -> SetTitleEx(mName.Get());
        builder -> SetMessageEx(mDescription.Get());
        builder -> SetPositiveButtonEx(mRStringOk.Get(), this);
        builder -> SetCancelable(false); 

        AutoPtr<IAlertDialog> tDialog;    
        builder -> Show((IAlertDialog**)&tDialog);
        mDialog = tDialog;
    }
    return NOERROR;
}

ECode Plugin::DefaultClickHandler::OnClick(
    /* [in] */ IDialogInterface* dialog, 
    /* [in] */ Int32 which)
{    
    mDialog -> Dismiss();
    mDialog = NULL;
    return NOERROR;
}

Plugin::Plugin()
{
}

Plugin::Plugin(
    /* [in] */ const String& name,
    /* [in] */ const String& path,
    /* [in] */ const String& fileName,
    /* [in] */ const String& description)
{    
    Init(name, path, fileName, description);
}

String Plugin::ToString()
{
    return mName;
}

String Plugin::GetName()
{
    return mName;
}

String Plugin::GetPath()
{
    return mPath;
}

String Plugin::GetFileName()
{
    return mFileName;
}

String Plugin::GetDescription()
{
    return mDescription;
}

ECode Plugin::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode Plugin::SetPath(
    /* [in] */ const String& path)
{
    mPath = path;
    return NOERROR;
}

ECode Plugin::SetFileName(
    /* [in] */ const String& fileName)
{
    mFileName = fileName;
    return NOERROR;
}

ECode Plugin::SetDescription(
    /* [in] */ const String& description)
{
    mDescription = description;
    return NOERROR;
}

ECode Plugin::SetClickHandler(
    /* [in] */ IPluginPreferencesClickHandler * handler)
{
    mHandler = handler;
    return NOERROR;
}

ECode Plugin::DispatchClickEvent(
    /* [in] */ IContext * context)
{
    if(mHandler != NULL) {
        mHandler -> HandleClickEvent(context);
    }
    return NOERROR;
}

void Plugin::Init(
    /* [in] */ const String& name,
    /* [in] */ const String& path,
    /* [in] */ const String& fileName,
    /* [in] */ const String& description)
{
    mName = name;
    mPath = path;
    mFileName = fileName;
    mDescription = description;    

    AutoPtr<Plugin::DefaultClickHandler> pDch = new Plugin::DefaultClickHandler();
    mHandler = (IPluginPreferencesClickHandler*)(pDch.Get());    
    pDch -> mName = (ICharSequence *)&name;
    pDch -> mDescription = (ICharSequence *)&description;
    pDch -> mRStringOk = (ICharSequence *)(R::id::ok);     //RStringOK = JAVA: R.string.ok    
}