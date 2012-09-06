
#include "graphics/drawable/CLevelListDrawable.h"


IDRAWABLE_METHODS_IMPL(
    CLevelListDrawable, LevelListDrawable, LevelListDrawable);

IDrawableCallback_METHODS_IMPL(
    CLevelListDrawable, LevelListDrawable, LevelListDrawable);


ECode CLevelListDrawable::AddLevel(
    /* [in] */ Int32 low,
    /* [in] */ Int32 high,
    /* [in] */ IDrawable* drawable)
{
    return LevelListDrawable::AddLevel(low, high, drawable);
}

ECode CLevelListDrawable::constructor()
{
    return LevelListDrawable::Init();
}

ECode CLevelListDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return LevelListDrawable::Init((LevelListState*)state, res);
}

PInterface CLevelListDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CLevelListDrawable::Probe(riid);
}
