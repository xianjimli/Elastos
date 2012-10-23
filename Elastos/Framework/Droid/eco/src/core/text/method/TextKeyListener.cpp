
#include "text/method/TextKeyListener.h"
#include "text/method/CTextKeyListener.h"
#include "text/method/QwertyKeyListener.h"
#include "text/SpannableStringInternal.h"
#include "text/TextUtils.h"
#include "text/Selection.h"
#include "view/ElKeyCharacterMap.h"

AutoPtr<TextKeyListener::NullKeyListener> TextKeyListener::NullKeyListener::sInstance;

PInterface TextKeyListener::NullKeyListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IKeyListener*)this;
    }
    else if (riid == EIID_IKeyListener) {
        return (IKeyListener*)this;
    }
    return NULL;
}

UInt32 TextKeyListener::NullKeyListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextKeyListener::NullKeyListener::Release()
{
    return ElRefBase::Release();
}

ECode TextKeyListener::NullKeyListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IKeyListener*)this) {
        *pIID = EIID_IKeyListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::GetInputType(
    /* [out] */ Int32* inputType)
{
    VALIDATE_NOT_NULL(inputType);
    *inputType = InputType_TYPE_NULL;

    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode TextKeyListener::NullKeyListener::ClearMetaKeyState(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    return NOERROR;
}

AutoPtr<TextKeyListener::NullKeyListener> TextKeyListener::NullKeyListener::GetInstance()
{
    if (sInstance != NULL)
        return sInstance;

    sInstance = new NullKeyListener();
    return sInstance;
}

TextKeyListener::SettingsObserver::SettingsObserver(
    /* [in] */ TextKeyListener* host)
    : mHost(host)
{
}

PInterface TextKeyListener::SettingsObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IContentObserver*)this;
    }
    else if (riid == EIID_IContentObserver) {
        return (IContentObserver*)this;
    }
    return NULL;
}

UInt32 TextKeyListener::SettingsObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextKeyListener::SettingsObserver::Release()
{
    return ElRefBase::Release();
}

ECode TextKeyListener::SettingsObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IContentObserver*)this) {
        *pIID = EIID_IContentObserver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

//@Override
ECode TextKeyListener::SettingsObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    if (mHost->mResolver == NULL) {
        mHost->mPrefsInited = FALSE;
    }
    else {
        mHost->UpdatePrefs(mHost->mResolver);
    }

    return NOERROR;
}

const AutoPtr<IInterface> TextKeyListener::ACTIVE = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> TextKeyListener::CAPPED = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> TextKeyListener::INHIBIT_REPLACEMENT = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> TextKeyListener::LAST_TYPED = MetaKeyKeyListener::NewNoCopySpan();

const Int32 TextKeyListener::AUTO_CAP;
const Int32 TextKeyListener::AUTO_TEXT;
const Int32 TextKeyListener::AUTO_PERIOD;
const Int32 TextKeyListener::SHOW_PASSWORD;

const Int32 TextKeyListener::CAPITALIZELENGTH;
AutoPtr<ITextKeyListener> TextKeyListener::sInstance[TextKeyListener::CAPITALIZELENGTH * 2];

TextKeyListener::TextKeyListener()
{}

/**
 * Returns a new or existing instance with the specified capitalization
 * and correction properties.
 *
 * @param cap when, if ever, to automatically capitalize.
 * @param autotext whether to automatically do spelling corrections.
 */
AutoPtr<ITextKeyListener> TextKeyListener::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap)
{
    Int32 off = cap * 2 + (autotext ? 1 : 0);

    if (sInstance[off] == NULL) {
        AutoPtr<ITextKeyListener> listener;
        CTextKeyListener::New(cap, autotext, (ITextKeyListener**)&listener);
        sInstance[off] = listener;
    }

    return sInstance[off];
}

/**
 * Returns a new or existing instance with no automatic capitalization
 * or correction.
 */
AutoPtr<ITextKeyListener> TextKeyListener::GetInstance()
{
    return GetInstance(FALSE, Capitalize_NONE);
}

/**
 * Returns whether it makes sense to automatically capitalize at the
 * specified position in the specified text, with the specified rules.
 *
 * @param cap the capitalization rules to consider.
 * @param cs the text in which an insertion is being made.
 * @param off the offset into that text where the insertion is being made.
 *
 * @return whether the character being inserted should be capitalized.
 */
Boolean TextKeyListener::ShouldCap(
    /* [in] */ Capitalize cap,
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 off)
{
    if (cap == Capitalize_NONE) {
        return FALSE;
    }
    if (cap == Capitalize_CHARACTERS) {
        return TRUE;
    }

    return TextUtils::GetCapsMode(cs, off, cap == Capitalize_WORDS
        ? TextUtils::CAP_MODE_WORDS : TextUtils::CAP_MODE_SENTENCES)
            != 0;
}

Int32 TextKeyListener::GetInputType()
{
    return MakeTextContentType(mAutoCap, mAutoText);
}

//@Override
Boolean TextKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyListener> im = GetKeyListener(event);

    Boolean res;
    im->OnKeyDown(view, content, keyCode, event, &res);
    return res;
}

//@Override
Boolean TextKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyListener> im = GetKeyListener(event);

    Boolean res;
    im->OnKeyUp(view, content, keyCode, event, &res);
    return res;
}

//@Override
Boolean TextKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyListener> im = GetKeyListener(event);

    Boolean res;
    im->OnKeyOther(view, content, event, &res);
    return res;
}

/**
 * Clear all the input state (autotext, autocap, multitap, undo)
 * from the specified Editable, going beyond Editable.clear(), which
 * just clears the text but not the input state.
 *
 * @param e the buffer whose text and state are to be cleared.
 */
void TextKeyListener::Clear(
    /* [in] */ IEditable* e)
{
    e->Clear();
    e->RemoveSpan(ACTIVE);
    e->RemoveSpan(CAPPED);
    e->RemoveSpan(INHIBIT_REPLACEMENT);
    e->RemoveSpan(LAST_TYPED);

    Int32 len;
    e->GetLength(&len);
    ArrayOf<IInterface*>* repl = NULL;
    e->GetSpans(0, len, EIID_Replaced, &repl);
    Int32 count = repl->GetLength();
    for (Int32 i = 0; i < count; i++) {
        e->RemoveSpan((*repl)[i]);
    }
    FreeArray(repl);
}

ECode TextKeyListener::OnSpanAdded(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return NOERROR;
}

ECode TextKeyListener::OnSpanRemoved(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return NOERROR;
}

ECode TextKeyListener::OnSpanChanged(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 st,
    /* [in] */ Int32 en)
{
    if (what == Selection::SELECTION_END) {
        s->RemoveSpan(ACTIVE);
    }
    return NOERROR;
}

AutoPtr<IKeyListener> TextKeyListener::GetKeyListener(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyboard;
    event->GetKeyboardDevice(&keyboard);
    ElKeyCharacterMap* kmap = ElKeyCharacterMap::Load(keyboard);
    Int32 kind = kmap->GetKeyboardType();

    if (kind == ElKeyCharacterMap::ALPHA) {
        return QwertyKeyListener::GetInstance(mAutoText, mAutoCap);
    }
    else if (kind == ElKeyCharacterMap::NUMERIC) {
        //return MultiTapKeyListener.getInstance(mAutoText, mAutoCap);
    }

    return NullKeyListener::GetInstance();
}

ECode TextKeyListener::ReleaseListener()
{
    if (mResolver != NULL) {
        if (mResolver != NULL) {
            //mResolver->UnregisterContentObserver(mObserver);
        }
        mObserver = NULL;
        mResolver = NULL;
        mPrefsInited = FALSE;
    }

    return NOERROR;
}

void TextKeyListener::InitPrefs(
    /* [in] */ IContext* context)
{
    AutoPtr<IContentResolver> contentResolver;
    context->GetContentResolver((IContentResolver**)&contentResolver);
    mResolver = contentResolver;
    if (mObserver == NULL) {
        mObserver = new SettingsObserver(this);
        //contentResolver->RegisterContentObserver(Settings.System.CONTENT_URI, TRUE, mObserver);
    }

    UpdatePrefs(contentResolver);
    mPrefsInited = TRUE;
}

void TextKeyListener::UpdatePrefs(
    /* [in] */ IContentResolver* resolver)
{
    Boolean cap = FALSE;//System.getInt(resolver, System.TEXT_AUTO_CAPS, 1) > 0;
    Boolean text = FALSE;//System.getInt(resolver, System.TEXT_AUTO_REPLACE, 1) > 0;
    Boolean period = FALSE;//System.getInt(resolver, System.TEXT_AUTO_PUNCTUATE, 1) > 0;
    Boolean pw = FALSE;//System.getInt(resolver, System.TEXT_SHOW_PASSWORD, 1) > 0;

    mPrefs = (cap ? AUTO_CAP : 0) |
             (text ? AUTO_TEXT : 0) |
             (period ? AUTO_PERIOD : 0) |
             (pw ? SHOW_PASSWORD : 0);
}

Int32 TextKeyListener::GetPrefs(
    /* [in] */ IContext* context)
{
    Mutex::Autolock lock(mPrefsLock);

    if (!mPrefsInited || mResolver == NULL) {
        InitPrefs(context);
    }

    return mPrefs;
}

/**
 * Creates a new TextKeyListener with the specified capitalization
 * and correction properties.
 *
 * @param cap when, if ever, to automatically capitalize.
 * @param autotext whether to automatically do spelling corrections.
 */
ECode TextKeyListener::Init(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    mAutoCap = cap;
    mAutoText = autotext;

    return NOERROR;
}
