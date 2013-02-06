#ifndef __IPLUGINSTUB_H__
#define __IPLUGINSTUB_H__

/**
 * This interface is used to implement plugins in a WebView. A plugin
 * package may extend this class and implement the abstract functions to create
 * embedded or fullscreeen views displayed in a WebView. The PluginStub
 * implementation will be provided the same NPP instance that is created
 * through the native interface.
 */
class IPluginStub {
public:
    /**
     * Return a custom embedded view to draw the plugin.
     * @param NPP The native NPP instance.
     * @param context The current application's Context.
     * @return A custom View that will be managed by WebView.
     */
    virtual CARAPI_(View) GetEmbeddedView(
        /* [in] */ Int32 NPP, 
        /* [in] */ IContext* context) = 0;

    /**
     * Return a custom full-screen view to be displayed when the user requests
     * a plugin display as full-screen. Note that the application may choose not
     * to display this View as completely full-screen.
     * @param NPP The native NPP instance.
     * @param context The current application's Context.
     * @return A custom View that will be managed by the application.
     */
    virtual CARAPI_(IView*) getFullScreenView(
        /* [in] */ Int32 NPP, 
        /* [in] */ IContext* context) = 0;
};

#endif //__IPLUGINSTUB_H__