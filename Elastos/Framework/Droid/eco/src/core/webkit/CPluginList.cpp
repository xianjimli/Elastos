
#include "webkit/CPluginList.h"
ECode CPluginList::GetList(
    /* [out] */ IObjectContainer ** list)
{
    VALIDATE_NOT_NULL(list);
    *list = (IObjectContainer *)mPlugins;
    return NOERROR;
}

ECode CPluginList::AddPlugin(
    /* [in] */ IPlugin * plugin)
{
    mPlugins -> PushBack(plugin);
    return NOERROR;
}

ECode CPluginList::RemovePlugin(
    /* [in] */ IPlugin * plugin)
{
    mPlugins -> Remove(plugin);
    return NOERROR;
}

ECode CPluginList::Clear()
{
    mPlugins -> Clear();
    return NOERROR;
}

ECode CPluginList::PluginClicked(
    /* [in] */ IContext * context,
    /* [in] */ Int32 position)
{
    List<AutoPtr<IPlugin> >::Iterator iterT;
    List<AutoPtr<IPlugin> >::Iterator iterE;
    iterT = mPlugins -> Begin();
    iterE = mPlugins -> End();
    for(int n = 0; n < position ; n ++ ) {
        if(iterE == iterT ) {
            return E_INFLATE_EXCEPTION;
        }            
        iterT ++;
    }
    ECode ec = (*iterT) -> DispatchClickEvent(context);
    if(FAILED(ec)) {
        //JAVA: catch(IIndexOutOfBoundsException e)
        // This can happen if the list of plugins
        // gets changed while the pref menu is up.  
    }
    return NOERROR;
}

ECode CPluginList::constructor()
{
    mPlugins = new List<AutoPtr<IPlugin> >; 
    return NOERROR;
}

CPluginList::~CPluginList()
{
    mPlugins -> Clear();
    delete mPlugins;
}

