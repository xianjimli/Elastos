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

CARAPI Plugin::DefaultClickHandler::HandleClickEvent(
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

    Plugin::DefaultClickHandler * pDch = new Plugin::DefaultClickHandler();
    mHandler = (IPluginPreferencesClickHandler*)pDch;    
    pDch -> pName = (ICharSequence *)&name;
    pDch -> pDescription = (ICharSequence *)&description;
    pDch -> pRStringOk = (ICharSequence *)(R::id::ok);     //RStringOK = JAVA: R.string.ok
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
    /* [in] */ IPluginPreferencesClickHandler * pHandler)
{
    mHandler = pHandler;
}

void Plugin::DispatchClickEvent(
    /* [in] */ IContext * pContext)
{
    if(mHandler != NULL)
    {
        mHandler -> HandleClickEvent(pContext);
    }
}
