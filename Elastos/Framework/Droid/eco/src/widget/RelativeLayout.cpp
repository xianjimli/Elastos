
#include "widget/RelativeLayout.h"
#include "view/CGravity.h"
#include "view/ViewGroup.h"
#include "utils/Pools.h"
#include <elastos/Math.h>
#include <Logger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;

PInterface RelativeLayout::TopToBottomLeftToRightComparator::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IComparator) {
        return (IComparator*)this;
    }

    return NULL;
}

UInt32 RelativeLayout::TopToBottomLeftToRightComparator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RelativeLayout::TopToBottomLeftToRightComparator::Release()
{
    return ElRefBase::Release();
}

ECode RelativeLayout::TopToBottomLeftToRightComparator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IComparator*)this) {
        *pIID = EIID_IComparator;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode RelativeLayout::TopToBottomLeftToRightComparator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    IView* first = IView::Probe(object1);
    IView* second = IView::Probe(object2);

    VALIDATE_NOT_NULL(first);
    VALIDATE_NOT_NULL(second);

    Int32 iFirst, iSecond;

    // top - bottom
    first->GetTop(&iFirst);
    second->GetTop(&iSecond);
    Int32 topDifference = iFirst - iSecond;
    if (topDifference != 0) {
        *result = topDifference;
        return NOERROR;
    }
    // left - right
    first->GetLeft(&iFirst);
    second->GetLeft(&iSecond);
    Int32 leftDifference = iFirst - iSecond;
    if (leftDifference != 0) {
        *result = leftDifference;
        return NOERROR;
    }
    // break tie by height
    first->GetHeight(&iFirst);
    second->GetHeight(&iSecond);
    Int32 heightDiference = iFirst - iSecond;
    if (heightDiference != 0) {
        *result = heightDiference;
        return NOERROR;
    }
    // break tie by width
    first->GetWidth(&iFirst);
    second->GetWidth(&iSecond);
    Int32 widthDiference = iFirst - iSecond;
    if (widthDiference != 0) {
        *result = widthDiference;
        return NOERROR;
    }
    *result = 0;
    return NOERROR;
}

ECode RelativeLayout::TopToBottomLeftToRightComparator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* isEqual)
{
    return NOERROR;
}

PInterface RelativeLayout::DependencyGraph::Node::NodePoolableManager::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPoolableManager) {
        return (IPoolableManager*)this;
    }

    return NULL;
}

UInt32 RelativeLayout::DependencyGraph::Node::NodePoolableManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RelativeLayout::DependencyGraph::Node::NodePoolableManager::Release()
{
    return ElRefBase::Release();
}

ECode RelativeLayout::DependencyGraph::Node::NodePoolableManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPoolableManager*)this) {
        *pIID = EIID_IPoolableManager;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode RelativeLayout::DependencyGraph::Node::NodePoolableManager::NewInstance(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = new Node();
    if (*element == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    (*element)->AddRef();

    return NOERROR;
}

ECode RelativeLayout::DependencyGraph::Node::NodePoolableManager::OnAcquired(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

ECode RelativeLayout::DependencyGraph::Node::NodePoolableManager::OnReleased(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

const Int32 RelativeLayout::DependencyGraph::Node::POOL_LIMIT;
const AutoPtr<IPool> RelativeLayout::DependencyGraph::Node::sPool = \
    Pools::AcquireSynchronizedPool(Pools::AcquireFinitePool(new NodePoolableManager(), POOL_LIMIT));

RelativeLayout::DependencyGraph::Node::Node()
{
}

PInterface RelativeLayout::DependencyGraph::Node::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPoolable) {
        return (IPoolable*)this;
    }

    return NULL;
}

UInt32 RelativeLayout::DependencyGraph::Node::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RelativeLayout::DependencyGraph::Node::Release()
{
    return ElRefBase::Release();
}

ECode RelativeLayout::DependencyGraph::Node::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPoolable*)this) {
        *pIID = EIID_IPoolable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode RelativeLayout::DependencyGraph::Node::SetNextPoolable(
    /* [in] */ IPoolable* element)
{
    mNext = element;
    return NOERROR;
}

ECode RelativeLayout::DependencyGraph::Node::GetNextPoolable(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = mNext;
    if (*element) {
        (*element)->AddRef();
    }

    return NOERROR;
}

AutoPtr<RelativeLayout::DependencyGraph::Node>
RelativeLayout::DependencyGraph::Node::Acquire(
    /* [in] */ IView* view)
{
    AutoPtr<IPoolable> tmp;
    sPool->Acquire((IPoolable**)&tmp);
    AutoPtr<Node> node = (Node*)tmp.Get();
    node->mView = view;
    return node;
}

void RelativeLayout::DependencyGraph::Node::ReleaseNode()
{
    mView = NULL;
    mDependents.Clear();
    mDependencies.Clear();

    sPool->ReleaseElement((IPoolable*)this);
}

RelativeLayout::DependencyGraph::DependencyGraph()
{
}

/**
 * Clears the graph.
 */
void RelativeLayout::DependencyGraph::Clear()
{
    List< AutoPtr<Node> >::Iterator it;
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        (*it)->ReleaseNode();
    }
    mNodes.Clear();

    mKeyNodes.Clear();
    mRoots.Clear();
}

/**
 * Adds a view to the graph.
 *
 * @param view The view to be added as a node to the graph.
 */
void RelativeLayout::DependencyGraph::Add(
    /* [in] */ IView* view)
{
    assert(view != NULL);

    Int32 id;
    view->GetId(&id);
    AutoPtr<Node> node = Node::Acquire(view);

    if (id != View_NO_ID) {
        mKeyNodes[id] =  node;
    }

    mNodes.PushBack(node);
}

/**
 * Builds a sorted list of views. The sorting order depends on the dependencies
 * between the view. For instance, if view C needs view A to be processed first
 * and view A needs view B to be processed first, the dependency graph
 * is: B -> A -> C. The sorted array will contain view s B, A and C in this order.
 *
 * @param sorted The sorted list of views. The length of this array must
 *        be equal to getChildCount().
 * @param rules The list of rules to take into account.
 */
ECode RelativeLayout::DependencyGraph::GetSortedViews(
    /* [in] */ ArrayOf< AutoPtr<IView> >* sorted,
    /* [in] */ ArrayOf<Int32>* rules)
{
    assert(sorted != NULL);

    List< AutoPtr<Node> >* roots = FindRoots(rules);
    Int32 index = 0;

    while (roots->Begin() != roots->End()) {
        AutoPtr<Node> node = *(roots->Begin());
        roots->PopFront();

        IView* view = node->mView;
        Int32 key;
        view->GetId(&key);

        (*sorted)[index++] = view;

        Set< AutoPtr<Node> >* dependents = &node->mDependents;
        Set< AutoPtr<Node> >::Iterator it;
        for (it = dependents->Begin(); it != dependents->End(); ++it) {
            Node* dependent = *it;
            HashMap<Int32, AutoPtr<Node> >* dependencies = &dependent->mDependencies;
            dependencies->Erase(key);
            if (dependencies->Begin() == dependencies->End()) {
                roots->PushBack(dependent);
            }
        }
    }

    if (index < sorted->GetLength()) {
//        throw new IllegalStateException("Circular dependencies cannot exist"
//                + " in RelativeLayout");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Finds the roots of the graph. A root is a node with no dependency and
 * with [0..n] dependents.
 *
 * @param rulesFilter The list of rules to consider when building the
 *        dependencies
 *
 * @return A list of node, each being a root of the graph
 */
List< AutoPtr<RelativeLayout::DependencyGraph::Node> >*
RelativeLayout::DependencyGraph::FindRoots(
    /* [in] */ ArrayOf<Int32>* rulesFilter)
{
    assert(rulesFilter != NULL);
    List< AutoPtr<Node> >::Iterator it;

    // Find roots can be invoked several times, so make sure to clear
    // all dependents and dependencies before running the algorithm
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        Node* node = *it;
        node->mDependents.Clear();
        node->mDependencies.Clear();
    }

    // Builds up the dependents and dependencies for each node of the graph
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        Node* node = *it;

        AutoPtr<IRelativeLayoutLayoutParams> layoutParams;
        node->mView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        ArrayOf<Int32>* rules = ((CRelativeLayoutLayoutParams*)layoutParams.Get())->mRules;
        Int32 rulesCount = rulesFilter->GetLength();

        // Look only the the rules passed in parameter, this way we build only the
        // dependencies for a specific set of rules
        for (Int32 j = 0; j < rulesCount; j++) {
            Int32 rule = (*rules)[(*rulesFilter)[j]];
            if (rule > 0) {
                // The node this node depends on
                HashMap<Int32, AutoPtr<Node> >::Iterator it = mKeyNodes.Find(rule);
                Node* dependency = it == mKeyNodes.End() ? NULL : it->mSecond;
                // Skip unknowns and self dependencies
                if (dependency == NULL || dependency == node) {
                    continue;
                }
                // Add the current node as a dependent
                dependency->mDependents.Insert(node);
                // Add a dependency to the current node
                node->mDependencies[rule] = dependency;
            }
        }
    }

    mRoots.Clear();

    // Finds all the roots in the graph: all nodes with no dependencies
    for (it = mNodes.Begin(); it != mNodes.End(); ++it) {
        Node* node = *it;
        if (node->mDependencies.Begin() == node->mDependencies.End()) {
            mRoots.PushBack(node);
        }
    }

    return &mRoots;
}

/**
 * Prints the dependency graph for the specified rules.
 *
 * @param resources The context's resources to print the ids.
 * @param rules The list of rules to take into account.
 */
void RelativeLayout::DependencyGraph::Log(
    /* [in] */ IResources* resources,
    /* [in] */ ArrayOf<Int32>* rules)
{
//    final LinkedList<Node> roots = findRoots(rules);
//    for (Node node : roots) {
//        printNode(resources, node);
//    }
}

void RelativeLayout::DependencyGraph::PrintViewId(
    /* [in] */ IResources* resources,
    /* [in] */ IView* view)
{
//    if (view.getId() != View.NO_ID) {
//        d(LOG_TAG, resources.getResourceEntryName(view.getId()));
//    } else {
//        d(LOG_TAG, "NO_ID");
//    }
}

void RelativeLayout::DependencyGraph::AppendViewId(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node,
    /* [in] */ StringBuffer& buffer)
{
    Int32 id;
    node->mView->GetId(&id);
    if (id != View_NO_ID) {
        String name;
        resources->GetResourceEntryName(id, &name);
        buffer += name;
    }
    else {
        buffer += "NO_ID";
    }
}

void RelativeLayout::DependencyGraph::PrintNode(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node)
{
//    if (node.dependents.size() == 0) {
//        printViewId(resources, node.view);
//    } else {
//        for (Node dependent : node.dependents) {
//            StringBuilder buffer = new StringBuilder();
//            appendViewId(resources, node, buffer);
//            printdependents(resources, dependent, buffer);
//        }
//    }
}

void RelativeLayout::DependencyGraph::Printdependents(
    /* [in] */ IResources* resources,
    /* [in] */ Node* node,
    /* [in] */ StringBuffer& buffer)
{
//    buffer.append(" -> ");
//    appendViewId(resources, node, buffer);
//
//    if (node.dependents.size() == 0) {
//        d(LOG_TAG, buffer.toString());
//    } else {
//        for (Node dependent : node.dependents) {
//            StringBuilder subBuffer = new StringBuilder(buffer);
//            printdependents(resources, dependent, subBuffer);
//        }
//    }
}

const Int32 RelativeLayout::VERB_COUNT;

RelativeLayout::RelativeLayout()
    : mHasBaselineAlignedChild(FALSE)
    , mGravity(Gravity_LEFT | Gravity_TOP)
    , mIgnoreGravity(0)
    , mTopToBottomLeftToRightSet(NULL)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mSortedVerticalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mGraph = new DependencyGraph();
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
    , mHasBaselineAlignedChild(FALSE)
    , mGravity(Gravity_LEFT | Gravity_TOP)
    , mIgnoreGravity(0)
    , mTopToBottomLeftToRightSet(NULL)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mSortedVerticalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mGraph = new DependencyGraph();
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
    , mHasBaselineAlignedChild(FALSE)
    , mGravity(Gravity_LEFT | Gravity_TOP)
    , mIgnoreGravity(0)
    , mTopToBottomLeftToRightSet(NULL)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mSortedVerticalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mGraph = new DependencyGraph();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
}

RelativeLayout::RelativeLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
    , mHasBaselineAlignedChild(FALSE)
    , mGravity(Gravity_LEFT | Gravity_TOP)
    , mIgnoreGravity(0)
    , mTopToBottomLeftToRightSet(NULL)
    , mDirtyHierarchy(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentBounds));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSelfBounds));
    mSortedHorizontalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mSortedVerticalChildren = ArrayOf< AutoPtr<IView> >::Alloc(0);
    mGraph = new DependencyGraph();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
}

RelativeLayout::~RelativeLayout()
{
    if (mTopToBottomLeftToRightSet != NULL) {
        delete mTopToBottomLeftToRightSet;
    }

    ArrayOf< AutoPtr<IView> >::Free(mSortedHorizontalChildren);
    ArrayOf< AutoPtr<IView> >::Free(mSortedVerticalChildren);
    delete mGraph;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context));
    return NOERROR;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)

{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
    return NOERROR;
}

ECode RelativeLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs, defStyle));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs));
    return NOERROR;
}

static Int32 R_Styleable_RelativeLayout[] = {
    0x010100af, 0x010101ff
};

ECode RelativeLayout::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    assert(context != NULL);

    AutoPtr<ITypedArray> a;

    FAIL_RETURN(context->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_RelativeLayout,
            sizeof(R_Styleable_RelativeLayout) / sizeof(Int32)),/*com.android.internal.R.styleable.RelativeLayout*/
            (ITypedArray**)&a));

    a->GetResourceId(1/*R.styleable.RelativeLayout_ignoreGravity*/, View_NO_ID, &mIgnoreGravity);
    a->GetInt32(0/*R.styleable.RelativeLayout_gravity*/, mGravity, &mGravity);
    a->Recycle();

    return NOERROR;
}

/**
 * Defines which View is ignored when the gravity is applied. This setting has no
 * effect if the gravity is <code>Gravity.LEFT | Gravity.TOP</code>.
 *
 * @param viewId The id of the View to be ignored by gravity, or 0 if no View
 *        should be ignored.
 *
 * @see #setGravity(int)
 *
 * @attr ref android.R.styleable#RelativeLayout_ignoreGravity
 */
ECode RelativeLayout::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    mIgnoreGravity = viewId;

    return NOERROR;
}

ECode RelativeLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mGravity != gravity) {
        if ((gravity & Gravity_HORIZONTAL_GRAVITY_MASK) == 0) {
            gravity |= Gravity_LEFT;
        }

        if ((gravity & Gravity_VERTICAL_GRAVITY_MASK) == 0) {
            gravity |= Gravity_TOP;
        }

        mGravity = gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    Int32 gravity = horizontalGravity & Gravity_HORIZONTAL_GRAVITY_MASK;
    if ((mGravity & Gravity_HORIZONTAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~Gravity_HORIZONTAL_GRAVITY_MASK) | gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    Int32 gravity = verticalGravity & Gravity_VERTICAL_GRAVITY_MASK;
    if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != gravity) {
        mGravity = (mGravity & ~Gravity_VERTICAL_GRAVITY_MASK) | gravity;
        return RequestLayout();
    }

    return NOERROR;
}

ECode RelativeLayout::GetBaseline(
    /* [out] */ Int32* baseline)
{
    assert(baseline != NULL);

    return mBaselineView != NULL ?
            mBaselineView->GetBaseline(baseline) : ViewGroup::GetBaseline(baseline);
}

ECode RelativeLayout::RequestLayout()
{
    FAIL_RETURN(ViewGroup::RequestLayout());
    mDirtyHierarchy = TRUE;

    return NOERROR;
}

void RelativeLayout::SortChildren()
{
    Int32 count = GetChildCount();

    if (mSortedVerticalChildren->GetLength() != count) {
        mSortedVerticalChildren = ArrayOf< AutoPtr<IView> >::Alloc(count);
    }
    if (mSortedHorizontalChildren->GetLength() != count) {
        mSortedHorizontalChildren = ArrayOf< AutoPtr<IView> >::Alloc(count);
    }

    mGraph->Clear();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        mGraph->Add(child);
    }
//    if (DEBUG_GRAPH) {
//        d(LOG_TAG, "=== Sorted vertical children");
//        graph.log(getResources(), ABOVE, BELOW, ALIGN_BASELINE, ALIGN_TOP, ALIGN_BOTTOM);
//        d(LOG_TAG, "=== Sorted horizontal children");
//        graph.log(getResources(), LEFT_OF, RIGHT_OF, ALIGN_LEFT, ALIGN_RIGHT);
//    }

    Int32 rulesInt[] = {RelativeLayout_ABOVE, RelativeLayout_BELOW, RelativeLayout_ALIGN_BASELINE,
            RelativeLayout_ALIGN_TOP, RelativeLayout_ALIGN_BOTTOM};
    ArrayOf<Int32> rules(rulesInt, 5);
    mGraph->GetSortedViews(mSortedVerticalChildren, &rules);
    Int32 rulesInt2[] = {RelativeLayout_LEFT_OF, RelativeLayout_RIGHT_OF, RelativeLayout_ALIGN_LEFT,
            RelativeLayout_ALIGN_RIGHT};
    ArrayOf<Int32> rules2(rulesInt2, 4);
    mGraph->GetSortedViews(mSortedHorizontalChildren, &rules2);

//    if (DEBUG_GRAPH) {
//        d(LOG_TAG, "=== Ordered list of vertical children");
//        for (View view : mSortedVerticalChildren) {
//            DependencyGraph.printViewId(getResources(), view);
//        }
//        d(LOG_TAG, "=== Ordered list of horizontal children");
//        for (View view : mSortedHorizontalChildren) {
//            DependencyGraph.printViewId(getResources(), view);
//        }
//    }
}

//TODO: we need to find another way to implement RelativeLayout
// This implementation cannot handle every case
void RelativeLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mDirtyHierarchy) {
        mDirtyHierarchy = FALSE;
        SortChildren();
    }

    Int32 myWidth = -1;
    Int32 myHeight = -1;

    Int32 width = 0;
    Int32 height = 0;

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    // Record our dimensions if they are known;
    if (widthMode != MeasureSpec::UNSPECIFIED) {
        myWidth = widthSize;
    }

    if (heightMode != MeasureSpec::UNSPECIFIED) {
        myHeight = heightSize;
    }

    if (widthMode == MeasureSpec::EXACTLY) {
        width = myWidth;
    }

    if (heightMode == MeasureSpec::EXACTLY) {
        height = myHeight;
    }

    mHasBaselineAlignedChild = FALSE;

    AutoPtr<IView> ignore;
    Int32 gravity = mGravity & Gravity_HORIZONTAL_GRAVITY_MASK;
    Boolean horizontalGravity = gravity != Gravity_LEFT && gravity != 0;
    gravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;
    Boolean verticalGravity = gravity != Gravity_TOP && gravity != 0;

    Int32 left = Math::INT32_MAX_VALUE;
    Int32 top = Math::INT32_MAX_VALUE;
    Int32 right = Math::INT32_MIN_VALUE;
    Int32 bottom = Math::INT32_MIN_VALUE;

    Boolean offsetHorizontalAxis = FALSE;
    Boolean offsetVerticalAxis = FALSE;

    if ((horizontalGravity || verticalGravity) && mIgnoreGravity != View_NO_ID) {
        ignore = FindViewById(mIgnoreGravity);
    }

    Boolean isWrapContentWidth = widthMode != MeasureSpec::EXACTLY;
    Boolean isWrapContentHeight = heightMode != MeasureSpec::EXACTLY;

    ArrayOf< AutoPtr<IView> >* views = mSortedHorizontalChildren;
    Int32 count = views->GetLength();
    for (Int32 i = 0; i < count; i++) {
        IView* child = (*views)[i];
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != View_GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
            ApplyHorizontalSizeRules(lp, myWidth);
            MeasureChildHorizontal(child, lp, myWidth, myHeight);
            if (PositionChildHorizontal(child, lp, myWidth, isWrapContentWidth)) {
                offsetHorizontalAxis = TRUE;
            }
        }
    }

    views = mSortedVerticalChildren;
    count = views->GetLength();

    for (Int32 i = 0; i < count; i++) {
        IView* child = (*views)[i];
        assert(child);

        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != View_GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
            assert(lp);
            ApplyVerticalSizeRules(lp, myHeight);
            MeasureChild(child, lp, myWidth, myHeight);
            if (PositionChildVertical(child, lp, myHeight, isWrapContentHeight)) {
                offsetVerticalAxis = TRUE;
            }

            if (isWrapContentWidth) {
                width = Math::Max(width, lp->mRight);
            }

            if (isWrapContentHeight) {
                height = Math::Max(height, lp->mBottom);
            }

            if (child != ignore || verticalGravity) {
                left = Math::Min(left, lp->mLeft - lp->mLeftMargin);
                top = Math::Min(top, lp->mTop - lp->mTopMargin);
            }

            if (child != ignore || horizontalGravity) {
                right = Math::Max(right, lp->mRight + lp->mRightMargin);
                bottom = Math::Max(bottom, lp->mBottom + lp->mBottomMargin);
            }
        }
    }

    if (mHasBaselineAlignedChild) {
        for (Int32 i = 0; i < count; i++) {
            IView* child = (*views)[i];
            Int32 visibility;
            child->GetVisibility(&visibility);
            if (visibility != View_GONE) {
                AutoPtr<IRelativeLayoutLayoutParams> params;
                child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                AlignBaseline(child, lp);
                if (child != ignore || verticalGravity) {
                    left = Math::Min(left, lp->mLeft - lp->mLeftMargin);
                    top = Math::Min(top, lp->mTop - lp->mTopMargin);
                }

                if (child != ignore || horizontalGravity) {
                    right = Math::Max(right, lp->mRight + lp->mRightMargin);
                    bottom = Math::Max(bottom, lp->mBottom + lp->mBottomMargin);
                }
            }
        }
    }
    if (isWrapContentWidth) {
        // Width already has left padding in it since it was calculated by looking at
        // the right of each child view
        width += mPaddingRight;

        Int32 lpWidth;
        mLayoutParams->GetWidth(&lpWidth);
        if (lpWidth >= 0) {
            width = Math::Max(width, lpWidth);
        }

        width = Math::Max(width, GetSuggestedMinimumWidth());
        width = ResolveSize(width, widthMeasureSpec);

        if (offsetHorizontalAxis) {
            for (Int32 i = 0; i < count; ++i) {
                IView* child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != View_GONE) {
                    AutoPtr<IRelativeLayoutLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                    CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                    ArrayOf<Int32>* rules = lp->GetRules();
                    if ((*rules)[RelativeLayout_CENTER_IN_PARENT] != 0 ||
                            (*rules)[RelativeLayout_CENTER_HORIZONTAL] != 0) {
                        CenterHorizontal(child, lp, width);
                    }
                    else if ((*rules)[RelativeLayout_ALIGN_PARENT_RIGHT] != 0) {
                        Int32 childWidth;
                        child->GetMeasuredWidth(&childWidth);
                        lp->mLeft = width - mPaddingRight - childWidth;
                        lp->mRight = lp->mLeft + childWidth;
                    }
                }
            }
        }
    }
    if (isWrapContentHeight) {
        // Height already has top padding in it since it was calculated by looking at
        // the bottom of each child view
        height += mPaddingBottom;

        Int32 lpHeight;
        mLayoutParams->GetHeight(&lpHeight);
        if (lpHeight >= 0) {
            height = Math::Max(height, lpHeight);
        }

        height = Math::Max(height, GetSuggestedMinimumHeight());
        height = ResolveSize(height, heightMeasureSpec);

        if (offsetVerticalAxis) {
            for (Int32 i = 0; i < count; i++) {
                IView* child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != View_GONE) {
                    AutoPtr<IRelativeLayoutLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);

                    CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                    ArrayOf<Int32>* rules = lp->GetRules();
                    if ((*rules)[RelativeLayout_CENTER_IN_PARENT] != 0 ||
                            (*rules)[RelativeLayout_CENTER_VERTICAL] != 0) {
                        CenterVertical(child, lp, height);
                    }
                    else if ((*rules)[RelativeLayout_ALIGN_PARENT_BOTTOM] != 0) {
                        Int32 childHeight;
                        child->GetMeasuredHeight(&childHeight);
                        lp->mTop = height - mPaddingBottom - childHeight;
                        lp->mBottom = lp->mTop + childHeight;
                    }
                }
            }
        }
    }
    if (horizontalGravity || verticalGravity) {
        mSelfBounds->Set(mPaddingLeft, mPaddingTop, width - mPaddingRight,
                height - mPaddingBottom);

        AutoPtr<IGravity> gravity;
        CGravity::AcquireSingleton((IGravity**)&gravity);
        gravity->Apply(mGravity, right - left, bottom - top,
                mSelfBounds, mContentBounds);

        Int32 horizontalOffset = ((CRect*)mContentBounds.Get())->mLeft - left;
        Int32 verticalOffset = ((CRect*)mContentBounds.Get())->mTop - top;
        if (horizontalOffset != 0 || verticalOffset != 0) {
            for (Int32 i = 0; i < count; ++i) {
                IView* child = GetChildAt(i);

                Int32 visibility;
                child->GetVisibility(&visibility);
                if (visibility != View_GONE && child != ignore) {
                    AutoPtr<IRelativeLayoutLayoutParams> params;
                    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
                    CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)params.Get();
                    if (horizontalGravity) {
                        lp->mLeft += horizontalOffset;
                        lp->mRight += horizontalOffset;
                    }
                    if (verticalGravity) {
                        lp->mTop += verticalOffset;
                        lp->mBottom += verticalOffset;
                    }
                }
            }
        }
    }
    SetMeasuredDimension(width, height);

}

void RelativeLayout::AlignBaseline(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params)
{
    assert(child != NULL && params != NULL);

    ArrayOf<Int32>* rules = params->GetRules();
    Int32 anchorBaseline = GetRelatedViewBaseline(rules, RelativeLayout_ALIGN_BASELINE);

    if (anchorBaseline != -1) {
        AutoPtr<CRelativeLayoutLayoutParams> anchorParams =
                GetRelatedViewParams(rules, RelativeLayout_ALIGN_BASELINE);
        if (anchorParams != NULL) {
            Int32 offset = anchorParams->mTop + anchorBaseline;
            Int32 baseline;
            child->GetBaseline(&baseline);
            if (baseline != -1) {
                offset -= baseline;
            }
            Int32 height = params->mBottom - params->mTop;
            params->mTop = offset;
            params->mBottom = params->mTop + height;
        }
    }

    if (mBaselineView == NULL) {
        mBaselineView = child;
    }
    else {
        AutoPtr<IRelativeLayoutLayoutParams> layoutParams;
        mBaselineView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        CRelativeLayoutLayoutParams* lp = (CRelativeLayoutLayoutParams*)layoutParams.Get();
        if (params->mTop < lp->mTop || (params->mTop == lp->mTop && params->mLeft < lp->mLeft)) {
            mBaselineView = child;
        }
    }
}

/**
 * Measure a child. The child should have left, top, right and bottom information
 * stored in its LayoutParams. If any of these values is -1 it means that the view
 * can extend up to the corresponding edge.
 *
 * @param child Child to measure
 * @param params LayoutParams associated with child
 * @param myWidth Width of the the RelativeLayout
 * @param myHeight Height of the RelativeLayout
 */
void RelativeLayout::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(params->mLeft,
            params->mRight, params->mWidth,
            params->mLeftMargin, params->mRightMargin,
            mPaddingLeft, mPaddingRight, myWidth);
    Int32 childHeightMeasureSpec = GetChildMeasureSpec(params->mTop,
            params->mBottom, params->mHeight,
            params->mTopMargin, params->mBottomMargin,
            mPaddingTop, mPaddingBottom, myHeight);
    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

void RelativeLayout::MeasureChildHorizontal(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(params->mLeft,
            params->mRight, params->mWidth,
            params->mLeftMargin, params->mRightMargin,
            mPaddingLeft, mPaddingRight, myWidth);
    Int32 childHeightMeasureSpec;
    if (params->mWidth == ViewGroupLayoutParams_MATCH_PARENT) {
        childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::EXACTLY);
    }
    else {
        childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(myHeight, MeasureSpec::AT_MOST);
    }
    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

/**
 * Get a measure spec that accounts for all of the constraints on this view.
 * This includes size contstraints imposed by the RelativeLayout as well as
 * the View's desired dimension.
 *
 * @param childStart The left or top field of the child's layout params
 * @param childEnd The right or bottom field of the child's layout params
 * @param childSize The child's desired size (the width or height field of
 *        the child's layout params)
 * @param startMargin The left or top margin
 * @param endMargin The right or bottom margin
 * @param startPadding mPaddingLeft or mPaddingTop
 * @param endPadding mPaddingRight or mPaddingBottom
 * @param mySize The width or height of this view (the RelativeLayout)
 * @return MeasureSpec for the child
 */
Int32 RelativeLayout::GetChildMeasureSpec(
    /* [in] */ Int32 childStart,
    /* [in] */ Int32 childEnd,
    /* [in] */ Int32 childSize,
    /* [in] */ Int32 startMargin,
    /* [in] */ Int32 endMargin,
    /* [in] */ Int32 startPadding,
    /* [in] */ Int32 endPadding,
    /* [in] */ Int32 mySize)
{
    Int32 childSpecMode = 0;
    Int32 childSpecSize = 0;

    // Figure out start and end bounds.
    Int32 tempStart = childStart;
    Int32 tempEnd = childEnd;

    // If the view did not express a layout constraint for an edge, use
    // view's margins and our padding
    if (tempStart < 0) {
        tempStart = startPadding + startMargin;
    }
    if (tempEnd < 0) {
        tempEnd = mySize - endPadding - endMargin;
    }

    // Figure out maximum size available to this view
    Int32 maxAvailable = tempEnd - tempStart;

    if (childStart >= 0 && childEnd >= 0) {
        // Constraints fixed both edges, so child must be an exact size
        childSpecMode = MeasureSpec::EXACTLY;
        childSpecSize = maxAvailable;
    }
    else {
        if (childSize >= 0) {
            // Child wanted an exact size. Give as much as possible
            childSpecMode = MeasureSpec::EXACTLY;

            if (maxAvailable >= 0) {
                // We have a maxmum size in this dimension.
                childSpecSize = Math::Min(maxAvailable, childSize);
            }
            else {
                // We can grow in this dimension.
                childSpecSize = childSize;
            }
        }
        else if (childSize == ViewGroupLayoutParams_MATCH_PARENT) {
            // Child wanted to be as big as possible. Give all availble
            // space
            childSpecMode = MeasureSpec::EXACTLY;
            childSpecSize = maxAvailable;
        }
        else if (childSize == ViewGroupLayoutParams_WRAP_CONTENT) {
            // Child wants to wrap content. Use AT_MOST
            // to communicate available space if we know
            // our max size
            if (maxAvailable >= 0) {
                // We have a maxmum size in this dimension.
                childSpecMode = MeasureSpec::AT_MOST;
                childSpecSize = maxAvailable;
            }
            else {
                // We can grow in this dimension. Child can be as big as it
                // wants
                childSpecMode = MeasureSpec::UNSPECIFIED;
                childSpecSize = 0;
            }
        }
    }

    return MeasureSpec::MakeMeasureSpec(childSpecSize, childSpecMode);
}

Boolean RelativeLayout::PositionChildHorizontal(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth,
    /* [in] */ Boolean wrapContent)
{
    assert(child != NULL && params != NULL);

    ArrayOf<Int32>* rules = params->GetRules();

    if (params->mLeft < 0 && params->mRight >= 0) {
        // Right is fixed, but left varies
        Int32 width;
        child->GetMeasuredWidth(&width);
        params->mLeft = params->mRight - width;
    }
    else if (params->mLeft >= 0 && params->mRight < 0) {
        // Left is fixed, but right varies
        Int32 width;
        child->GetMeasuredWidth(&width);
        params->mRight = params->mLeft + width;
    }
    else if (params->mLeft < 0 && params->mRight < 0) {
        // Both left and right vary
        if ((*rules)[RelativeLayout_CENTER_IN_PARENT] != 0 || (*rules)[RelativeLayout_CENTER_HORIZONTAL] != 0) {
            if (!wrapContent) {
                CenterHorizontal(child, params, myWidth);
            }
            else {
                Int32 width;
                child->GetMeasuredWidth(&width);
                params->mLeft = mPaddingLeft + params->mLeftMargin;
                params->mRight = params->mLeft + width;
            }
            return TRUE;
        }
        else {
            Int32 width;
            child->GetMeasuredWidth(&width);
            params->mLeft = mPaddingLeft + params->mLeftMargin;
            params->mRight = params->mLeft + width;
        }
    }
    return (*rules)[RelativeLayout_ALIGN_PARENT_RIGHT] != 0;
}

Boolean RelativeLayout::PositionChildVertical(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myHeight,
    /* [in] */ Boolean wrapContent)
{
    assert(child != NULL && params != NULL);

    ArrayOf<Int32>* rules = params->GetRules();

    if (params->mTop < 0 && params->mBottom >= 0) {
        // Bottom is fixed, but top varies
        Int32 height;
        child->GetMeasuredHeight(&height);
        params->mTop = params->mBottom - height;
    }
    else if (params->mTop >= 0 && params->mBottom < 0) {
        // Top is fixed, but bottom varies
        Int32 height;
        child->GetMeasuredHeight(&height);
        params->mBottom = params->mTop + height;
    }
    else if (params->mTop < 0 && params->mBottom < 0) {
        // Both top and bottom vary
        if ((*rules)[RelativeLayout_CENTER_IN_PARENT] != 0 || (*rules)[RelativeLayout_CENTER_VERTICAL] != 0) {
            if (!wrapContent) {
                CenterVertical(child, params, myHeight);
            }
            else {
                Int32 height;
                child->GetMeasuredHeight(&height);
                params->mTop = mPaddingTop + params->mTopMargin;
                params->mBottom = params->mTop + height;
            }
            return TRUE;
        }
        else {
            Int32 height;
            child->GetMeasuredHeight(&height);
            params->mTop = mPaddingTop + params->mTopMargin;
            params->mBottom = params->mTop + height;
        }
    }
    return (*rules)[RelativeLayout_ALIGN_PARENT_BOTTOM] != 0;
}

void RelativeLayout::ApplyHorizontalSizeRules(
    /* [in] */ CRelativeLayoutLayoutParams* childParams,
    /* [in] */ Int32 myWidth)
{
    assert(childParams != NULL);

    ArrayOf<Int32>* rules = childParams->GetRules();
    AutoPtr<CRelativeLayoutLayoutParams> anchorParams;

    // -1 indicated a "soft requirement" in that direction. For example:
    // left=10, right=-1 means the view must start at 10, but can go as far as it wants to the right
    // left =-1, right=10 means the view must end at 10, but can go as far as it wants to the left
    // left=10, right=20 means the left and right ends are both fixed
    childParams->mLeft = -1;
    childParams->mRight = -1;

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_LEFT_OF);
    if (anchorParams != NULL) {
        childParams->mRight =
                anchorParams->mLeft - (anchorParams->mLeftMargin + childParams->mRightMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_LEFT_OF] != 0) {
        if (myWidth >= 0) {
            childParams->mRight =
                    myWidth - mPaddingRight - childParams->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_RIGHT_OF);
    if (anchorParams != NULL) {
        childParams->mLeft =
                anchorParams->mRight + (anchorParams->mRightMargin + childParams->mLeftMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_RIGHT_OF] != 0) {
        childParams->mLeft = mPaddingLeft + childParams->mLeftMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_LEFT);
    if (anchorParams != NULL) {
        childParams->mLeft = anchorParams->mLeft + childParams->mLeftMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_ALIGN_LEFT] != 0) {
        childParams->mLeft = mPaddingLeft + childParams->mLeftMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_RIGHT);
    if (anchorParams != NULL) {
        childParams->mRight = anchorParams->mRight - childParams->mRightMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_ALIGN_RIGHT] != 0) {
        if (myWidth >= 0) {
            childParams->mRight = myWidth - mPaddingRight - childParams->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if (0 != (*rules)[RelativeLayout_ALIGN_PARENT_LEFT]) {
        childParams->mLeft = mPaddingLeft + childParams->mLeftMargin;
    }

    if (0 != (*rules)[RelativeLayout_ALIGN_PARENT_RIGHT]) {
        if (myWidth >= 0) {
            childParams->mRight = myWidth - mPaddingRight - childParams->mRightMargin;
        }
        else {
            // FIXME uh oh...
        }
    }
}

void RelativeLayout::ApplyVerticalSizeRules(
    /* [in] */ CRelativeLayoutLayoutParams* childParams,
    /* [in] */ Int32 myHeight)
{
    assert(childParams != NULL);

    ArrayOf<Int32>* rules = childParams->GetRules();
    AutoPtr<CRelativeLayoutLayoutParams> anchorParams;

    childParams->mTop = -1;
    childParams->mBottom = -1;

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ABOVE);
    if (anchorParams != NULL) {
        childParams->mBottom =
                anchorParams->mTop - (anchorParams->mTopMargin + childParams->mBottomMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_ABOVE] != 0) {
        if (myHeight >= 0) {
            childParams->mBottom = myHeight - mPaddingBottom - childParams->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_BELOW);
    if (anchorParams != NULL) {
        childParams->mTop =
                anchorParams->mBottom + (anchorParams->mBottomMargin + childParams->mTopMargin);
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_BELOW] != 0) {
        childParams->mTop = mPaddingTop + childParams->mTopMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_TOP);
    if (anchorParams != NULL) {
        childParams->mTop = anchorParams->mTop + childParams->mTopMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_ALIGN_TOP] != 0) {
        childParams->mTop = mPaddingTop + childParams->mTopMargin;
    }

    anchorParams = GetRelatedViewParams(rules, RelativeLayout_ALIGN_BOTTOM);
    if (anchorParams != NULL) {
        childParams->mBottom = anchorParams->mBottom - childParams->mBottomMargin;
    }
    else if (childParams->mAlignWithParent && (*rules)[RelativeLayout_ALIGN_BOTTOM] != 0) {
        if (myHeight >= 0) {
            childParams->mBottom = myHeight - mPaddingBottom - childParams->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if (0 != (*rules)[RelativeLayout_ALIGN_PARENT_TOP]) {
        childParams->mTop = mPaddingTop + childParams->mTopMargin;
    }

    if (0 != (*rules)[RelativeLayout_ALIGN_PARENT_BOTTOM]) {
        if (myHeight >= 0) {
            childParams->mBottom = myHeight - mPaddingBottom - childParams->mBottomMargin;
        }
        else {
            // FIXME uh oh...
        }
    }

    if ((*rules)[RelativeLayout_ALIGN_BASELINE] != 0) {
        mHasBaselineAlignedChild = TRUE;
    }
}

AutoPtr<IView> RelativeLayout::GetRelatedView(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    assert(rules != NULL);

    Int32 id = (*rules)[relation];
    if (id != 0) {
        HashMap<Int32, AutoPtr<DependencyGraph::Node> >::Iterator it = mGraph->mKeyNodes.Find(id);
        DependencyGraph::Node* node = it == mGraph->mKeyNodes.End() ? NULL : it->mSecond;
        if (node == NULL) return NULL;
        IView* v = node->mView;

        // Find the first non-GONE view up the chain
        Int32 visibility;
        v->GetVisibility(&visibility);
        while (visibility == View_GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            v->GetLayoutParams((IViewGroupLayoutParams**)&params);
            rules = ((CRelativeLayoutLayoutParams*)params.Get())->GetRules();
            it = mGraph->mKeyNodes.Find((*rules)[relation]);
            node = it == mGraph->mKeyNodes.End() ? NULL : it->mSecond;
            if (node == NULL) return NULL;
            v = node->mView;
            v->GetVisibility(&visibility);
        }

        return v;
    }

    return NULL;
}

AutoPtr<CRelativeLayoutLayoutParams> RelativeLayout::GetRelatedViewParams(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    AutoPtr<IView> v = GetRelatedView(rules, relation);
    if (v != NULL) {
        AutoPtr<IViewGroupLayoutParams> params;
        v->GetLayoutParams((IViewGroupLayoutParams**)&params);
        if (params != NULL && params->Probe(EIID_IRelativeLayoutLayoutParams) != NULL) {
            AutoPtr<CRelativeLayoutLayoutParams> lp =
                    (CRelativeLayoutLayoutParams*)IRelativeLayoutLayoutParams::Probe(params);
            return lp;
        }
    }

    return NULL;
}

Int32 RelativeLayout::GetRelatedViewBaseline(
    /* [in] */ ArrayOf<Int32>* rules,
    /* [in] */ Int32 relation)
{
    assert(rules != NULL);

    AutoPtr<IView> v = GetRelatedView(rules, relation);
    if (v != NULL) {
        Int32 baseline;
        v->GetBaseline(&baseline);
        return baseline;
    }
    return -1;
}

void RelativeLayout::CenterHorizontal(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myWidth)
{
    assert(child != NULL);

    Int32 childWidth;
    child->GetMeasuredWidth(&childWidth);
    Int32 left = (myWidth - childWidth) / 2;

    params->mLeft = left;
    params->mRight = left + childWidth;
}

void RelativeLayout::CenterVertical(
    /* [in] */ IView* child,
    /* [in] */ CRelativeLayoutLayoutParams* params,
    /* [in] */ Int32 myHeight)
{
    assert(child != NULL);

    Int32 childHeight;
    child->GetMeasuredHeight(&childHeight);
    Int32 top = (myHeight - childHeight) / 2;

    params->mTop = top;
    params->mBottom = top + childHeight;
}

void RelativeLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    //  The layout has actually already been performed and the positions
    //  cached.  Apply the cached values to the children.
    Int32 count = GetChildCount();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != View_GONE) {
            AutoPtr<IRelativeLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);
            CRelativeLayoutLayoutParams* st = (CRelativeLayoutLayoutParams*)params.Get();
            child->Layout(st->mLeft, st->mTop, st->mRight, st->mBottom);
        }
    }
}

AutoPtr<IViewGroupLayoutParams> RelativeLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    assert(attrs != NULL);
    AutoPtr<IViewGroupLayoutParams> lp;
    assert(SUCCEEDED(CRelativeLayoutLayoutParams::New(GetContext(), attrs,
        (IRelativeLayoutLayoutParams**)&lp)));

    return lp;
}

ECode RelativeLayout::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** lp)
{
    assert(lp != NULL);

    return CRelativeLayoutLayoutParams::New(
                ViewGroupLayoutParams_WRAP_CONTENT,
                ViewGroupLayoutParams_WRAP_CONTENT,
                (IRelativeLayoutLayoutParams**)lp);
}

// Override to allow type-checking of LayoutParams.
Boolean RelativeLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p != NULL && IRelativeLayoutLayoutParams::Probe(p) != NULL;
}

AutoPtr<IViewGroupLayoutParams> RelativeLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    assert(p != NULL);

    AutoPtr<IViewGroupLayoutParams> lp;
    assert(SUCCEEDED(CRelativeLayoutLayoutParams::New(p,
            (IRelativeLayoutLayoutParams**)&lp)));

    return lp;
}

Boolean RelativeLayout::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* p)
{
    if (mTopToBottomLeftToRightSet == NULL) {
        //todo:
//        mTopToBottomLeftToRightSet = new TreeSet<View>(new TopToBottomLeftToRightComparator());
        mTopToBottomLeftToRightSet = new Set<AutoPtr<IView> >;
    }

    // sort children top-to-bottom and left-to-right
    for (Int32 i = 0, count = GetChildCount(); i < count; i++) {
        mTopToBottomLeftToRightSet->Insert(GetChildAt(i));
    }

    Set<AutoPtr<IView> >::Iterator it;
    for (it = mTopToBottomLeftToRightSet->Begin(); it != mTopToBottomLeftToRightSet->End(); ++it) {
        IView* v = *it;
        Boolean result;
        v->DispatchPopulateAccessibilityEvent(p, &result);
        if (result) {
            mTopToBottomLeftToRightSet->Clear();
            return TRUE;
        }
    }

    mTopToBottomLeftToRightSet->Clear();
    return FALSE;
}

