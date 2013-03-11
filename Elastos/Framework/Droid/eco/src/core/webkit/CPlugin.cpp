
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
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IPluginPreferencesClickHandler*)this) {
        *pIID = EIID_IPluginPreferencesClickHandler;
    }
    if (pObject == (IInterface*)(IDialogInterfaceOnClickListener*)this) {
        *pIID = EIID_IDialogInterfaceOnClickListener;
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
    if (mDialog == NULL) 
    {
        IAlertDialogBuilder * builder;        
        ECode ec = CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);
        if(FAILED(ec))
        {
            return ec;
        }
        builder -> SetTitleEx(pName.Get());
        builder -> SetMessageEx(pDescription.Get());
        builder -> SetPositiveButtonEx(pRStringOk.Get(), this);
        builder -> SetCancelable(false); 

        IAlertDialog* tDialog = NULL;    
        builder -> Show(&tDialog);
        mDialog = tDialog;
        tDialog -> Release();

        builder -> Release();
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
    /* [out] */ String * pName)
{
    *pName = mName;
    return NOERROR;
}

ECode CPlugin::GetName(
    /* [out] */ String * pName)
{
    *pName = mName;
    return NOERROR;
}

ECode CPlugin::GetPath(
    /* [out] */ String * pPath)
{
    *pPath = mPath;
    return NOERROR;
}

ECode CPlugin::GetFileName(
    /* [out] */ String * pFileName)
{
    *pFileName = mFileName;
    return NOERROR;
}

ECode CPlugin::GetDescription(
    /* [out] */ String * pDescription)
{
    *pDescription = mDescription;
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
    /* [in] */ IPluginPreferencesClickHandler * pHandler)
{
    mHandler = pHandler;
    return NOERROR;
}

ECode CPlugin::DispatchClickEvent(
    /* [in] */ IContext * pContext)
{
    if(mHandler != NULL)
    {
        mHandler -> HandleClickEvent(pContext);
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
    pDch -> pName = (ICharSequence *)&name;
    pDch -> pDescription = (ICharSequence *)&description;
    pDch -> pRStringOk = (ICharSequence *)(R::id::ok);     //RStringOK = JAVA: R.string.ok

    return NOERROR;
}

