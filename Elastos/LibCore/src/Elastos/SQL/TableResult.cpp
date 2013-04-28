#include "TableResult.h"
#include <StringBuffer.h>

TableResult::TableResult()
    :mMaxrows(0)
{
}

TableResult::TableResult(
    /** [in] **/Int32 maxrows)
    :mMaxrows(0)
{
}

ECode TableResult::Init()
{
    Clear();
    return NOERROR;
}

ECode TableResult::Init(
    /** [in] **/Int32 maxrows)
{
    mMaxrows = maxrows;
    Clear();
    return NOERROR;
}

ECode TableResult::Clear()
{
    mColumn = ArrayOf<String>::Alloc(0);
    mTypes = NULL;
  //  rows = new Vector();
  //  mRows = new Elastos::Vector<ArrayOf<String>* >();
    mNcolumns = mNrows = 0;
    mAtmaxrows = FALSE;
    return NOERROR;
}

ECode TableResult::Columns(
    /** [in] **/ArrayOf<String>* coldata)
{
    mColumn = coldata;
    mNcolumns = mColumn->GetLength();
    return NOERROR;
}

ECode TableResult::Types(
    /** [in] **/ArrayOf<String>* types)
{
    mTypes = types;
    return NOERROR;
}

ECode TableResult::Newrow(
    /** [in] **/ArrayOf<String>* rowdata,
    /** [out] **/Boolean* row)
{
    assert(row != NULL);
    if (rowdata != NULL) {
        if (mMaxrows > 0 && mNrows >= mMaxrows) {
            mAtmaxrows = TRUE;
            *row = TRUE;
            return NOERROR;
        }
    //  rows.addElement(rowdata);
    //  mRows.push_back(rowdata);
        mNrows++;
    }
    *row = FALSE;
    return NOERROR;
}

ECode TableResult::ToString(
    /** [out] **/String* str)
{
    StringBuffer sb;
    Int32 i;
    for (i = 0; i < mNcolumns; i++) {
        sb += ((*mColumn)[i] == NULL ? "NULL" : (*mColumn)[i]);
        sb += ('|');
    }
    sb += ('\n');
    for (i = 0; i < mNrows; i++) {
        //Int32 k;
       // String row[] = (String[]) rows.elementAt(i);    ?????????????????????????????
       // for (k = 0; k < ncolumns; k++) {
       //     sb += (row[k] == null ? "NULL" : row[k]);
       //     sb += ('|');
       // }
        sb += ('\n');
    }
    *str = String(sb);

    return NOERROR;
}