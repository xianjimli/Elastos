
#ifndef __CMATCHER_H__
#define __CMATCHER_H__

#include "_CMatcher.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <unicode/regex.h>

CarClass(CMatcher)
{
public:
    CMatcher();

    /**
     * Appends a literal part of the input plus a replacement for the current
     * match to a given {@link StringBuffer}. The literal part is exactly the
     * part of the input between the previous match and the current match. The
     * method can be used in conjunction with {@link #find()} and
     * {@link #appendTail(StringBuffer)} to walk through the input and replace
     * all occurrences of the {@code Pattern} with something else.
     *
     * @param buffer
     *            the {@code StringBuffer} to append to.
     * @param replacement
     *            the replacement text.
     * @return the {@code Matcher} itself.
     * @throws IllegalStateException
     *             if no successful match has been made.
     */
    CARAPI AppendReplacement(
        /* [in] */ const String& replacement,
        /* [out] */ String* result);

    /**
     * Resets the {@code Matcher}. This results in the region being set to the
     * whole input. Results of a previous find get lost. The next attempt to
     * find an occurrence of the {@link Pattern} in the string will start at the
     * beginning of the input.
     *
     * @return the {@code Matcher} itself.
     */
    CARAPI Reset();

    /**
     * Provides a new input and resets the {@code Matcher}. This results in the
     * region being set to the whole input. Results of a previous find get lost.
     * The next attempt to find an occurrence of the {@link Pattern} in the
     * string will start at the beginning of the input.
     *
     * @param input
     *            the new input sequence.
     *
     * @return the {@code Matcher} itself.
     */
    CARAPI ResetEx(
        /* [in] */ ICharSequence* input);

    /**
     * Sets a new pattern for the {@code Matcher}. Results of a previous find
     * get lost. The next attempt to find an occurrence of the {@link Pattern}
     * in the string will start at the beginning of the input.
     *
     * @param pattern
     *            the new {@code Pattern}.
     *
     * @return the {@code Matcher} itself.
     */
    CARAPI UsePattern(
        /* [in] */ IPattern* pattern);

    /**
     * Resets this matcher and sets a region. Only characters inside the region
     * are considered for a match.
     *
     * @param start
     *            the first character of the region.
     * @param end
     *            the first character after the end of the region.
     * @return the {@code Matcher} itself.
     */
    CARAPI Region(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Appends the (unmatched) remainder of the input to the given
     * {@link StringBuffer}. The method can be used in conjunction with
     * {@link #find()} and {@link #appendReplacement(StringBuffer, String)} to
     * walk through the input and replace all matches of the {@code Pattern}
     * with something else.
     *
     * @param buffer
     *            the {@code StringBuffer} to append to.
     * @return the {@code StringBuffer}.
     * @throws IllegalStateException
     *             if no successful match has been made.
     */
    CARAPI AppendTail(
        /* [out] */ String* result);

    /**
     * Replaces the first occurrence of this matcher's pattern in the input with
     * a given string.
     *
     * @param replacement
     *            the replacement text.
     * @return the modified input string.
     */
    CARAPI ReplaceFirst(
        /* [in] */ const String& replacement,
        /* [out] */ String* result);

    /**
     * Replaces all occurrences of this matcher's pattern in the input with a
     * given string.
     *
     * @param replacement
     *            the replacement text.
     * @return the modified input string.
     */
    CARAPI ReplaceAll(
        /* [in] */ const String& replacement,
        /* [out] */ String* result);

    /**
     * Returns the {@link Pattern} instance used inside this matcher.
     *
     * @return the {@code Pattern} instance.
     */
    CARAPI Pattern(
        /* [out] */ IPattern** pattern);

    /**
     * Returns the text that matched the whole regular expression.
     *
     * @return the text.
     */
    CARAPI Group(
        /* [out] */ String* text);

    /**
     * Returns the text that matched a given group of the regular expression.
     *
     * @param group
     *            the group, ranging from 0 to groupCount() - 1, with 0
     *            representing the whole pattern.
     *
     * @return the text that matched the group.
     */
    CARAPI GroupEx(
        /* [in] */ Int32 group,
        /* [out] */ String* text);

    /**
     * Returns the next occurrence of the {@link Pattern} in the input. If a
     * previous match was successful, the method continues the search from the
     * first character following that match in the input. Otherwise it searches
     * either from the region start (if one has been set), or from position 0.
     *
     * @return true if (and only if) a match has been found.
     */
    CARAPI Find(
        /* [out] */ Boolean* found);

    /**
     * Returns the next occurrence of the {@link Pattern} in the input. The
     * method starts the search from the given character in the input.
     *
     * @param start
     *            The index in the input at which the find operation is to
     *            begin. If this is less than the start of the region, it is
     *            automatically adjusted to that value. If it is beyond the end
     *            of the region, the method will fail.
     * @return true if (and only if) a match has been found.
     */
    CARAPI FindEx(
        /* [in] */ Int32 start,
        /* [out] */ Boolean* found);

    /**
     * Tries to match the {@link Pattern}, starting from the beginning of the
     * region (or the beginning of the input, if no region has been set).
     * Doesn't require the {@code Pattern} to match against the whole region.
     *
     * @return true if (and only if) the {@code Pattern} matches.
     */
    CARAPI LookingAt(
        /* [out] */ Boolean* matched);

    /**
     * Tries to match the {@link Pattern} against the entire region (or the
     * entire input, if no region has been set).
     *
     * @return true if (and only if) the {@code Pattern} matches the entire
     *         region.
     */
    CARAPI Matches(
        /* [out] */ Boolean* matched);

    /**
     * Returns the index of the first character of the text that matched a given
     * group.
     *
     * @param group
     *            the group, ranging from 0 to groupCount() - 1, with 0
     *            representing the whole pattern.
     *
     * @return the character index.
     */
    CARAPI StartEx(
        /* [in] */ Int32 group,
        /* [out] */ Int32* index);

    /**
     * Returns the index of the first character following the text that matched
     * a given group.
     *
     * @param group
     *            the group, ranging from 0 to groupCount() - 1, with 0
     *            representing the whole pattern.
     * @return the character index.
     * @throws IllegalStateException
     *             if no successful match has been made.
     */
    CARAPI EndEx(
        /* [in] */ Int32 group,
        /* [out] */ Int32* index);

    /**
     * Returns the index of the first character following the text that matched
     * a given group.
     *
     * @param group
     *            the group, ranging from 0 to groupCount() - 1, with 0
     *            representing the whole pattern.
     *
     * @return the character index.
     */


    /**
     * Returns a replacement string for the given one that has all backslashes
     * and dollar signs escaped.
     *
     * @param s
     *            the input string.
     * @return the input string, with all backslashes and dollar signs having
     *         been escaped.
     */
    static CARAPI_(String) QuoteReplacement(
        /* [in] */ const String& s);

    /**
     * Returns the index of the first character of the text that matched
     * the whole regular expression.
     *
     * @return the character index.
     */
    CARAPI Start(
        /* [out] */ Int32* index);

    /**
     * Returns the number of groups in the result, which is always equal to
     * the number of groups in the original regular expression.
     *
     * @return the number of groups.
     */
    CARAPI GroupCount(
        /* [out] */ Int32* count);

    /**
     * Returns the index of the first character following the text that matched
     * the whole regular expression.
     *
     * @return the character index.
     */
    CARAPI End(
        /* [out] */ Int32* index);

    /**
     * Converts the current match into a separate {@link MatchResult} instance
     * that is independent from this matcher. The new object is unaffected when
     * the state of this matcher changes.
     *
     * @return the new {@code MatchResult}.
     * @throws IllegalStateException
     *             if no successful match has been made.
     */
    CARAPI ToMatchResult(
        /* [out] */ IMatchResult** result);

    /**
     * Determines whether this matcher has anchoring bounds enabled or not. When
     * anchoring bounds are enabled, the start and end of the input match the
     * '^' and '$' meta-characters, otherwise not. Anchoring bounds are enabled
     * by default.
     *
     * @param value
     *            the new value for anchoring bounds.
     * @return the {@code Matcher} itself.
     */
    CARAPI UseAnchoringBounds(
        /* [in] */ Boolean value);

    /**
     * Indicates whether this matcher has anchoring bounds enabled. When
     * anchoring bounds are enabled, the start and end of the input match the
     * '^' and '$' meta-characters, otherwise not. Anchoring bounds are enabled
     * by default.
     *
     * @return true if (and only if) the {@code Matcher} uses anchoring bounds.
     */
    CARAPI HasAnchoringBounds(
        /* [out] */ Boolean* result);

    /**
     * Determines whether this matcher has transparent bounds enabled or not.
     * When transparent bounds are enabled, the parts of the input outside the
     * region are subject to lookahead and lookbehind, otherwise they are not.
     * Transparent bounds are disabled by default.
     *
     * @param value
     *            the new value for transparent bounds.
     * @return the {@code Matcher} itself.
     */
    CARAPI UseTransparentBounds(
        /* [in] */ Boolean value);

    /**
     * Indicates whether this matcher has transparent bounds enabled. When
     * transparent bounds are enabled, the parts of the input outside the region
     * are subject to lookahead and lookbehind, otherwise they are not.
     * Transparent bounds are disabled by default.
     *
     * @return true if (and only if) the {@code Matcher} uses anchoring bounds.
     */
    CARAPI HasTransparentBounds(
        /* [out] */ Boolean* result);

    /**
     * Returns this matcher's region start, that is, the first character that is
     * considered for a match.
     *
     * @return the start of the region.
     */
    CARAPI RegionStart(
        /* [out] */ Int32* start);

    /**
     * Returns this matcher's region end, that is, the first character that is
     * not considered for a match.
     *
     * @return the end of the region.
     */
    CARAPI RegionEnd(
        /* [out] */ Int32* end);

    /**
     * Indicates whether more input might change a successful match into an
     * unsuccessful one.
     *
     * @return true if (and only if) more input might change a successful match
     *         into an unsuccessful one.
     */
    CARAPI RequireEnd(
        /* [out] */ Boolean* result);

    /**
     * Indicates whether the last match hit the end of the input.
     *
     * @return true if (and only if) the last match hit the end of the input.
     */
    CARAPI HitEnd(
        /* [out] */ Boolean* hit);

    constructor(
        /* [in] */ IPattern* pattern,
        /* [in] */ ICharSequence* input);

//protected:
//    CARAPI_(void) Finalize();

private:
    /**
     * Internal helper method to append a given string to a given string buffer.
     * If the string contains any references to groups, these are replaced by
     * the corresponding group's contents.
     *
     * @param buffer
     *            the string buffer.
     * @param s
     *            the string to append.
     */
    CARAPI AppendEvaluated(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    /**
     * Resets the Matcher. A new input sequence and a new region can be
     * specified. Results of a previous find get lost. The next attempt to find
     * an occurrence of the Pattern in the string will start at the beginning of
     * the region. This is the internal version of reset() to which the several
     * public versions delegate.
     *
     * @param input
     *            the input sequence.
     * @param start
     *            the start of the region.
     * @param end
     *            the end of the region.
     *
     * @return the matcher itself.
     */
    CARAPI ResetEx2(
        /* [in] */ ICharSequence* input,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) ResetForInput();

    /**
     * Makes sure that a successful match has been made. Is invoked internally
     * from various places in the class.
     *
     * @throws IllegalStateException
     *             if no successful match has been made.
     */
    CARAPI EnsureMatch();

    static CARAPI_(void) CloseImpl(
        /* [in] */ RegexMatcher* matcher);

    static CARAPI_(Boolean) FindImpl(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ String s,
        /* [in] */ Int32 startIndex,
        /* [in] */ ArrayOf<Int32>* offsets);

    static CARAPI_(Boolean) FindNextImpl(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ String s,
        /* [in] */ ArrayOf<Int32>* offsets);

    static CARAPI_(Int32) GroupCountImpl(
        /* [in] */ RegexMatcher* matcher);

    static CARAPI_(Boolean) HitEndImpl(
        /* [in] */ RegexMatcher* matcher);

    static CARAPI_(Boolean) LookingAtImpl(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ String s,
        /* [in] */ ArrayOf<Int32>* offsets);

    static CARAPI_(Boolean) MatchesImpl(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ String s,
        /* [in] */ ArrayOf<Int32>* offsets);

    static CARAPI_(RegexMatcher*) OpenImpl(
        /* [in] */ RegexPattern* pattern);

    static CARAPI_(Boolean) RequireEndImpl(
        /* [in] */ RegexMatcher* matcher);

    static CARAPI_(void) SetInputImpl(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ String s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(void) UseAnchoringBoundsImpl(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ Boolean value);

    static CARAPI_(void) UseTransparentBoundsImpl(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ Boolean value);

private:
    /**
     * Holds the pattern, that is, the compiled regular expression.
     */
    AutoPtr<IPattern> mPattern;

    /**
     * Holds the handle for the native version of the pattern.
     */
    RegexMatcher* mNativeMatcher;

    /**
     * Holds the input text.
     */
    String mInput;

    /**
     * Holds the start of the region, or 0 if the matching should start at the
     * beginning of the text.
     */
    Int32 mRegionStart;

    /**
     * Holds the end of the region, or input.length() if the matching should
     * go until the end of the input.
     */
    Int32 mRegionEnd;

    /**
     * Holds the position where the next find operation will take place.
     */
    Int32 mFindPos;

    /**
     * Holds the position where the next append operation will take place.
     */
    Int32 mAppendPos;

    /**
     * Reflects whether a match has been found during the most recent find
     * operation.
     */
    Boolean mMatchFound;

    /**
     * Holds the offsets for the most recent match.
     */
    ArrayOf<Int32>* mMatchOffsets;

    /**
     * Reflects whether the bounds of the region are anchoring.
     */
    Boolean mAnchoringBounds;// = true;

    /**
     * Reflects whether the bounds of the region are transparent.
     */
    Boolean mTransparentBounds;
};

#endif //__CMATCHER_H__
