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
    mDialog -> Release();
    mDialog = NULL;
    return NOERROR;
}

Plugin::Plugin(
	/* [in] */ String name,
	/* [in] */ String path,
	/* [in] */ String fileName,
	/* [in] */ String description)
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

void Plugin::SetName(
    /* [in] */ String name)
{
    mName = name;
}

void Plugin::SetPath(
    /* [in] */ String path)
{
    mPath = path;
}

void Plugin::SetFileName(
    /* [in] */ String fileName)
{
    mFileName = fileName;
}

void Plugin::SetDescription(
    /* [in] */ String description)
{
    mDescription = description;
}

void Plugin::SetClickHandler(
    /* [in] */ IPluginPreferencesClickHandler * handler)
{
    mHandler = handler;
}

void Plugin::DispatchClickEvent(
    /* [in] */ IContext * context)
{
    if(mHandler != NULL) {
        mHandler -> HandleClickEvent(context);
    }
}
