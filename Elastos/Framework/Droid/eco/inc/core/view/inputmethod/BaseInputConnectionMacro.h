#ifndef  __BASEINPUTCONNECTIONMACRO_H__
#define  __BASEINPUTCONNECTIONMACRO_H__

#define BASEINPUTCONNECTION_METHODS_DECL()                                          \
    CARAPI RemoveComposingSpans(                                                    \
        /* [in] */ ISpannable* text);                                               \
                                                                                    \
    CARAPI SetComposingSpans(                                                       \
        /* [in] */ ISpannable* text);                                               \
                                                                                    \
    CARAPI SetComposingSpans(                                                       \
        /* [in] */ ISpannable* text,                                                \
        /* [in] */ Int32 start,                                                     \
        /* [in] */ Int32 end);                                                      \
                                                                                    \
    CARAPI GetComposingSpanStart(                                                   \
        /* [in] */ ISpannable* text,                                                \
        /* [out] */ Int32* start);                                                  \
                                                                                    \
    CARAPI GetComposingSpanEnd(                                                     \
        /* [in] */ ISpannable* text,                                                \
        /* [out] */ Int32* end);                                                    \
                                                                                    \
    CARAPI GetEditable(                                                             \
        /* [out] */ IEditable** editable);                                          \
                                                                                    \
    CARAPI BeginBatchEdit(                                                          \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI EndBatchEdit(                                                            \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI ClearMetaKeyStates(                                                      \
        /* [in] */ Int32 states,                                                    \
        /* [out] */ Boolean* clear);                                                \
                                                                                    \
    CARAPI CommitCompletion(                                                        \
        /* [in] */ ICompletionInfo* text,                                           \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI CommitText(                                                              \
        /* [in] */ ICharSequence* text,                                             \
        /* [in] */ Int32 newCursorPosition,                                         \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI DeleteSurroundingText(                                                   \
        /* [in] */ Int32 leftLength,                                                \
        /* [in] */ Int32 rightLength,                                               \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI FinishComposingText(                                                     \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI GetCursorCapsMode(                                                       \
        /* [in] */ Int32 reqModes,                                                  \
        /* [out] */ Int32* capsMode);                                               \
                                                                                    \
    CARAPI GetExtractedText(                                                        \
        /* [in] */ IExtractedTextRequest* request,                                  \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ IExtractedText** text);                                         \
                                                                                    \
    CARAPI GetTextBeforeCursor(                                                     \
        /* [in] */ Int32 length,                                                    \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ ICharSequence** text);                                          \
                                                                                    \
    CARAPI GetSelectedText(                                                         \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ ICharSequence** text);                                          \
                                                                                    \
    CARAPI GetTextAfterCursor(                                                      \
        /* [in] */ Int32 length,                                                    \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ ICharSequence** text);                                          \
                                                                                    \
    CARAPI PerformEditorAction(                                                     \
        /* [in] */ Int32 actionCode,                                                \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI PerformContextMenuAction(                                                \
        /* [in] */ Int32 id,                                                        \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI PerformPrivateCommand(                                                   \
        /* [in] */ String action,                                                   \
        /* [in] */ IBundle* data,                                                   \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI SetComposingText(                                                        \
        /* [in] */ ICharSequence* text,                                             \
        /* [in] */ Int32 newCursorPosition,                                         \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI SetComposingRegion(                                                      \
        /* [in] */ Int32 start,                                                     \
        /* [in] */ Int32 end,                                                       \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI SetSelection(                                                            \
        /* [in] */ Int32 start,                                                     \
        /* [in] */ Int32 end,                                                       \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI SendKeyEvent(                                                            \
        /* [in] */ IKeyEvent* event,                                                \
        /* [out] */ Boolean* state);                                                \
                                                                                    \
    CARAPI ReportFullscreenMode(                                                    \
        /* [in] */ Boolean enabled,                                                 \
        /* [out] */ Boolean* state);                                                \



#define BASEINPUTCONNECTION_METHODS_IMPL(className, superClass, overRideClass)      \
    ECode className::RemoveComposingSpans(                                          \
        /* [in] */ ISpannable* text)                                                \
    {                                                                               \
        return superClass::RemoveComposingSpans(text);                              \
    }                                                                               \
                                                                                    \
    ECode className::SetComposingSpans(                                             \
        /* [in] */ ISpannable* text)                                                \
    {                                                                               \
        return superClass::SetComposingSpans(text);                                 \
    }                                                                               \
                                                                                    \
    ECode className::SetComposingSpans(                                             \
        /* [in] */ ISpannable* text,                                                \
        /* [in] */ Int32 start,                                                     \
        /* [in] */ Int32 end)                                                       \
    {                                                                               \
        return superClass::SetComposingSpans(text, start, end);                     \
    }                                                                               \
                                                                                    \
    ECode className::GetComposingSpanStart(                                         \
        /* [in] */ ISpannable* text,                                                \
        /* [out] */ Int32* start)                                                   \
    {                                                                               \
        return superClass::GetComposingSpanStart(text, start);                      \
    }                                                                               \
                                                                                    \
    ECode className::GetComposingSpanEnd(                                           \
        /* [in] */ ISpannable* text,                                                \
        /* [out] */ Int32* end)                                                     \
    {                                                                               \
        return superClass::GetComposingSpanEnd(text, end);                          \
    }                                                                               \
                                                                                    \
    ECode className::GetEditable(                                                   \
        /* [out] */ IEditable** editable)                                           \
    {                                                                               \
        return superClass::GetEditable(editable);                                   \
    }                                                                               \
                                                                                    \
    ECode className::BeginBatchEdit(                                                \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::BeginBatchEdit(state);                                   \
    }                                                                               \
                                                                                    \
    ECode className::EndBatchEdit(                                                  \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::EndBatchEdit(state);                                     \
    }                                                                               \
                                                                                    \
    ECode className::ClearMetaKeyStates(                                            \
        /* [in] */ Int32 states,                                                    \
        /* [out] */ Boolean* clear)                                                 \
    {                                                                               \
        return superClass::ClearMetaKeyStates(states, clear);                       \
    }                                                                               \
                                                                                    \
    ECode className::CommitCompletion(                                              \
        /* [in] */ ICompletionInfo* text,                                           \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::CommitCompletion(text, state);                           \
    }                                                                               \
                                                                                    \
    ECode className::CommitText(                                                    \
        /* [in] */ ICharSequence* text,                                             \
        /* [in] */ Int32 newCursorPosition,                                         \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::CommitText(text, newCursorPosition, state);              \
    }                                                                               \
                                                                                    \
    ECode className::DeleteSurroundingText(                                         \
        /* [in] */ Int32 leftLength,                                                \
        /* [in] */ Int32 rightLength,                                               \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::DeleteSurroundingText(leftLength, rightLength, state);   \
    }                                                                               \
                                                                                    \
    ECode className::FinishComposingText(                                           \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::FinishComposingText(state);                              \
    }                                                                               \
                                                                                    \
    ECode className::GetCursorCapsMode(                                             \
        /* [in] */ Int32 reqModes,                                                  \
        /* [out] */ Int32* capsMode)                                                \
    {                                                                               \
        return superClass::GetCursorCapsMode(reqModes, capsMode);                   \
    }                                                                               \
                                                                                    \
    ECode className::GetExtractedText(                                              \
        /* [in] */ IExtractedTextRequest* request,                                  \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ IExtractedText** text)                                          \
    {                                                                               \
        return superClass::GetExtractedText(request, flags, text);                  \
    }                                                                               \
                                                                                    \
    ECode className::GetTextBeforeCursor(                                           \
        /* [in] */ Int32 length,                                                    \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ ICharSequence** text)                                           \
    {                                                                               \
        return superClass::GetTextBeforeCursor(length, flags, text);                \
    }                                                                               \
                                                                                    \
    ECode className::GetSelectedText(                                               \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ ICharSequence** text)                                           \
    {                                                                               \
        return superClass::GetSelectedText(flags, text);                            \
    }                                                                               \
                                                                                    \
    ECode className::GetTextAfterCursor(                                            \
        /* [in] */ Int32 length,                                                    \
        /* [in] */ Int32 flags,                                                     \
        /* [out] */ ICharSequence** text)                                           \
    {                                                                               \
        return superClass::GetTextAfterCursor(length, flags, text);                 \
    }                                                                               \
                                                                                    \
    ECode className::PerformEditorAction(                                           \
        /* [in] */ Int32 actionCode,                                                \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::PerformEditorAction(actionCode, state);                  \
    }                                                                               \
                                                                                    \
    ECode className::PerformContextMenuAction(                                      \
        /* [in] */ Int32 id,                                                        \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::PerformContextMenuAction(id, state);                     \
    }                                                                               \
                                                                                    \
    ECode className::PerformPrivateCommand(                                         \
        /* [in] */ String action,                                                   \
        /* [in] */ IBundle* data,                                                   \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::PerformPrivateCommand(action, data, state);              \
    }                                                                               \
                                                                                    \
    ECode className::SetComposingText(                                              \
        /* [in] */ ICharSequence* text,                                             \
        /* [in] */ Int32 newCursorPosition,                                         \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::SetComposingText(text, newCursorPosition, state);        \
    }                                                                               \
                                                                                    \
    ECode className::SetComposingRegion(                                            \
        /* [in] */ Int32 start,                                                     \
        /* [in] */ Int32 end,                                                       \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::SetComposingRegion(start, end, state);                   \
    }                                                                               \
                                                                                    \
    ECode className::SetSelection(                                                  \
        /* [in] */ Int32 start,                                                     \
        /* [in] */ Int32 end,                                                       \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::SetSelection(start, end, state);                         \
    }                                                                               \
                                                                                    \
    ECode className::SendKeyEvent(                                                  \
        /* [in] */ IKeyEvent* event,                                                \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::SendKeyEvent(event, state);                              \
    }                                                                               \
                                                                                    \
    ECode className::ReportFullscreenMode(                                          \
        /* [in] */ Boolean enabled,                                                 \
        /* [out] */ Boolean* state)                                                 \
    {                                                                               \
        return superClass::ReportFullscreenMode(enabled, state);                    \
    }                                                                               \


#endif  //__BASEINPUTCONNECTIONMACRO_H__
