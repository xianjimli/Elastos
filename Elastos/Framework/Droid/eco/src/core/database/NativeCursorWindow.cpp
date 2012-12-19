
#include "database/NativeCursorWindow.h"
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>


NativeCursorWindow::NativeCursorWindow(size_t maxSize) :
    mMaxSize(maxSize)
{
}

Boolean NativeCursorWindow::setMemory(const android::sp<android::IMemory>& memory)
{
    mMemory = memory;
    mData = (uint8_t *) memory->pointer();
    if (mData == NULL) {
        return FALSE;
    }
    mHeader = (window_header_t *) mData;

    // Make the window read-only
    ssize_t size = memory->size();
    mSize = size;
    mMaxSize = size;
    mFreeOffset = size;
    // LOG_WINDOW("Created NativeCursorWindow from existing IMemory: mFreeOffset = %d, numRows = %d, numColumns = %d, mSize = %d, mMaxSize = %d, mData = %p", mFreeOffset, mHeader->numRows, mHeader->numColumns, mSize, mMaxSize, mData);
    return TRUE;
}

Boolean NativeCursorWindow::initBuffer(Boolean localOnly)
{
    //TODO Use a non-memory dealer mmap region for localOnly

    android::sp<android::MemoryHeapBase> heap;
    heap = new android::MemoryHeapBase(mMaxSize, 0, "NativeCursorWindow");
    if (heap != NULL) {
        mMemory = new android::MemoryBase(heap, 0, mMaxSize);
        if (mMemory != NULL) {
            mData = (uint8_t *) mMemory->pointer();
            if (mData) {
                mHeader = (window_header_t *) mData;
                mSize = mMaxSize;

                // Put the window into a clean state
                clear();
            // LOG_WINDOW("Created NativeCursorWindow with new MemoryDealer: mFreeOffset = %d, mSize = %d, mMaxSize = %d, mData = %p", mFreeOffset, mSize, mMaxSize, mData);
                return TRUE;
            }
        }
        // LOGE("NativeCursorWindow heap allocation failed");
        return FALSE;
    } else {
        // LOGE("failed to create the NativeCursorWindow heap");
        return FALSE;
    }
}

NativeCursorWindow::~NativeCursorWindow()
{
    // Everything that matters is a smart pointer
}

void NativeCursorWindow::clear()
{
    mHeader->numRows = 0;
    mHeader->numColumns = 0;
    mFreeOffset = sizeof(window_header_t) + ROW_SLOT_CHUNK_SIZE;
    // Mark the first chunk's next 'pointer' as null
    *((uint32_t *)(mData + mFreeOffset - sizeof(uint32_t))) = 0;
}

int32_t NativeCursorWindow::freeSpace()
{
    int32_t freeSpace = mSize - mFreeOffset;
    if (freeSpace < 0) {
        freeSpace = 0;
    }
    return freeSpace;
}

field_slot_t * NativeCursorWindow::allocRow()
{
    // Fill in the row slot
    row_slot_t * rowSlot = allocRowSlot();
    if (rowSlot == NULL) {
        return NULL;
    }

    // Allocate the slots for the field directory
    size_t fieldDirSize = mHeader->numColumns * sizeof(field_slot_t);
    uint32_t fieldDirOffset = alloc(fieldDirSize);
    if (!fieldDirOffset) {
        mHeader->numRows--;
        LOGE("The row failed, so back out the new row accounting from allocRowSlot %d", mHeader->numRows);
        return NULL;
    }
    field_slot_t * fieldDir = (field_slot_t *)offsetToPtr(fieldDirOffset);
    memset(fieldDir, 0x0, fieldDirSize);

LOG_WINDOW("Allocated row %u, rowSlot is at offset %u, fieldDir is %d bytes at offset %u\n", (mHeader->numRows - 1), ((uint8_t *)rowSlot) - mData, fieldDirSize, fieldDirOffset);
    rowSlot->offset = fieldDirOffset;

    return fieldDir;
}

uint32_t NativeCursorWindow::alloc(size_t requestedSize, Boolean aligned)
{
    int32_t size;
    uint32_t padding;
    if (aligned) {
        // 4 byte alignment
        padding = 4 - (mFreeOffset & 0x3);
    } else {
        padding = 0;
    }

    size = requestedSize + padding;

    if (size > freeSpace()) {
        LOGE("need to grow: mSize = %d, size = %d, freeSpace() = %d, numRows = %d", mSize, size, freeSpace(), mHeader->numRows);
        // Only grow the window if the first row doesn't fit
        if (mHeader->numRows > 1) {
LOGE("not growing since there are already %d row(s), max size %d", mHeader->numRows, mMaxSize);
            return 0;
        }

        // Find a new size that will fit the allocation
        int allocated = mSize - freeSpace();
        int newSize = mSize + WINDOW_ALLOCATION_SIZE;
        while (size > (newSize - allocated)) {
            newSize += WINDOW_ALLOCATION_SIZE;
            if (newSize > mMaxSize) {
                LOGE("Attempting to grow window beyond max size (%d)", mMaxSize);
                return 0;
            }
        }
LOG_WINDOW("found size %d", newSize);
        mSize = newSize;
    }

    uint32_t offset = mFreeOffset + padding;
    mFreeOffset += size;
    return offset;
}

row_slot_t * NativeCursorWindow::getRowSlot(int row)
{
    LOG_WINDOW("enter getRowSlot current row num %d, this row %d", mHeader->numRows, row);
    int chunkNum = row / ROW_SLOT_CHUNK_NUM_ROWS;
    int chunkPos = row % ROW_SLOT_CHUNK_NUM_ROWS;
    int chunkPtrOffset = sizeof(window_header_t) + ROW_SLOT_CHUNK_SIZE - sizeof(uint32_t);
    uint8_t * rowChunk = mData + sizeof(window_header_t);
    for (int i = 0; i < chunkNum; i++) {
        rowChunk = offsetToPtr(*((uint32_t *)(mData + chunkPtrOffset)));
        chunkPtrOffset = rowChunk - mData + (ROW_SLOT_CHUNK_NUM_ROWS * sizeof(row_slot_t));
    }
    return (row_slot_t *)(rowChunk + (chunkPos * sizeof(row_slot_t)));
    LOG_WINDOW("exit getRowSlot current row num %d, this row %d", mHeader->numRows, row);
}

row_slot_t * NativeCursorWindow::allocRowSlot()
{
    int chunkNum = mHeader->numRows / ROW_SLOT_CHUNK_NUM_ROWS;
    int chunkPos = mHeader->numRows % ROW_SLOT_CHUNK_NUM_ROWS;
    int chunkPtrOffset = sizeof(window_header_t) + ROW_SLOT_CHUNK_SIZE - sizeof(uint32_t);
    uint8_t * rowChunk = mData + sizeof(window_header_t);
LOG_WINDOW("Allocating row slot, mHeader->numRows is %d, chunkNum is %d, chunkPos is %d", mHeader->numRows, chunkNum, chunkPos);
    for (int i = 0; i < chunkNum; i++) {
        uint32_t nextChunkOffset = *((uint32_t *)(mData + chunkPtrOffset));
LOG_WINDOW("nextChunkOffset is %d", nextChunkOffset);
        if (nextChunkOffset == 0) {
            // Allocate a new row chunk
            nextChunkOffset = alloc(ROW_SLOT_CHUNK_SIZE, TRUE);
            if (nextChunkOffset == 0) {
                return NULL;
            }
            rowChunk = offsetToPtr(nextChunkOffset);
LOG_WINDOW("allocated new chunk at %d, rowChunk = %p", nextChunkOffset, rowChunk);
            *((uint32_t *)(mData + chunkPtrOffset)) = rowChunk - mData;
            // Mark the new chunk's next 'pointer' as null
            *((uint32_t *)(rowChunk + ROW_SLOT_CHUNK_SIZE - sizeof(uint32_t))) = 0;
        } else {
LOG_WINDOW("follwing 'pointer' to next chunk, offset of next pointer is %d", chunkPtrOffset);
            rowChunk = offsetToPtr(nextChunkOffset);
            chunkPtrOffset = rowChunk - mData + (ROW_SLOT_CHUNK_NUM_ROWS * sizeof(row_slot_t));
        }
    }
    mHeader->numRows++;

    return (row_slot_t *)(rowChunk + (chunkPos * sizeof(row_slot_t)));
}

field_slot_t * NativeCursorWindow::getFieldSlotWithCheck(int row, int column)
{
  if (row < 0 || row >= mHeader->numRows || column < 0 || column >= mHeader->numColumns) {
      LOGE("Bad request for field slot %d,%d. numRows = %d, numColumns = %d", row, column, mHeader->numRows, mHeader->numColumns);
      return NULL;
  }
  row_slot_t * rowSlot = getRowSlot(row);
  if (!rowSlot) {
      LOGE("Failed to find rowSlot for row %d", row);
      return NULL;
  }
  if (rowSlot->offset == 0 || rowSlot->offset >= mSize) {
      LOGE("Invalid rowSlot, offset = %d", rowSlot->offset);
      return NULL;
  }
  int fieldDirOffset = rowSlot->offset;
  return ((field_slot_t *)offsetToPtr(fieldDirOffset)) + column;
}

uint32_t NativeCursorWindow::read_field_slot(int row, int column, field_slot_t * slotOut)
{
    if (row < 0 || row >= mHeader->numRows || column < 0 || column >= mHeader->numColumns) {
        LOGE("Bad request for field slot %d,%d. numRows = %d, numColumns = %d", row, column, mHeader->numRows, mHeader->numColumns);
        return -1;
    }
    row_slot_t * rowSlot = getRowSlot(row);
    if (!rowSlot) {
        LOGE("Failed to find rowSlot for row %d", row);
        return -1;
    }
    if (rowSlot->offset == 0 || rowSlot->offset >= mSize) {
        LOGE("Invalid rowSlot, offset = %d", rowSlot->offset);
        return -1;
    }
LOG_WINDOW("Found field directory for %d,%d at rowSlot %d, offset %d", row, column, (uint8_t *)rowSlot - mData, rowSlot->offset);
    field_slot_t * fieldDir = (field_slot_t *)offsetToPtr(rowSlot->offset);
LOG_WINDOW("Read field_slot_t %d,%d: offset = %d, size = %d, type = %d", row, column, fieldDir[column].data.buffer.offset, fieldDir[column].data.buffer.size, fieldDir[column].type);

    // Copy the data to the out param
    slotOut->data.buffer.offset = fieldDir[column].data.buffer.offset;
    slotOut->data.buffer.size = fieldDir[column].data.buffer.size;
    slotOut->type = fieldDir[column].type;
    return 0;
}

void NativeCursorWindow::copyIn(uint32_t offset, uint8_t const * data, size_t size)
{
    assert(offset + size <= mSize);
    memcpy(mData + offset, data, size);
}

void NativeCursorWindow::copyIn(uint32_t offset, int64_t data)
{
    assert(offset + sizeof(int64_t) <= mSize);
    memcpy(mData + offset, (uint8_t *)&data, sizeof(int64_t));
}

void NativeCursorWindow::copyIn(uint32_t offset, double data)
{
    assert(offset + sizeof(double) <= mSize);
    memcpy(mData + offset, (uint8_t *)&data, sizeof(double));
}

void NativeCursorWindow::copyOut(uint32_t offset, uint8_t * data, size_t size)
{
    assert(offset + size <= mSize);
    memcpy(data, mData + offset, size);
}

int64_t NativeCursorWindow::copyOutInt64(uint32_t offset)
{
    int64_t value;
    assert(offset + sizeof(int64_t) <= mSize);
    memcpy(&value, mData + offset, sizeof(int64_t));
    return value;
}

double NativeCursorWindow::copyOutDouble(uint32_t offset)
{
    double value;
    assert(offset + sizeof(double) <= mSize);
    memcpy(&value, mData + offset, sizeof(double));
    return value;
}

Boolean NativeCursorWindow::putInt64(unsigned int row, unsigned int col, int64_t value)
{
    field_slot_t * fieldSlot = getFieldSlotWithCheck(row, col);
    if (!fieldSlot) {
        return FALSE;
    }

#if WINDOW_STORAGE_INLINE_NUMERICS
    fieldSlot->data.l = value;
#else
    int offset = alloc(sizeof(int64_t));
    if (!offset) {
        return FALSE;
    }

    copyIn(offset, value);

    fieldSlot->data.buffer.offset = offset;
    fieldSlot->data.buffer.size = sizeof(int64_t);
#endif
    fieldSlot->type = FIELD_TYPE_INTEGER;
    return TRUE;
}

Boolean NativeCursorWindow::putDouble(unsigned int row, unsigned int col, double value)
{
    field_slot_t * fieldSlot = getFieldSlotWithCheck(row, col);
    if (!fieldSlot) {
        return FALSE;
    }

#if WINDOW_STORAGE_INLINE_NUMERICS
    fieldSlot->data.d = value;
#else
    int offset = alloc(sizeof(int64_t));
    if (!offset) {
        return FALSE;
    }

    copyIn(offset, value);

    fieldSlot->data.buffer.offset = offset;
    fieldSlot->data.buffer.size = sizeof(double);
#endif
    fieldSlot->type = FIELD_TYPE_FLOAT;
    return TRUE;
}

Boolean NativeCursorWindow::putNull(unsigned int row, unsigned int col)
{
    field_slot_t * fieldSlot = getFieldSlotWithCheck(row, col);
    if (!fieldSlot) {
        return FALSE;
    }

    fieldSlot->type = FIELD_TYPE_NULL;
    fieldSlot->data.buffer.offset = 0;
    fieldSlot->data.buffer.size = 0;
    return TRUE;
}

Boolean NativeCursorWindow::getInt64(unsigned int row, unsigned int col, int64_t * valueOut)
{
    field_slot_t * fieldSlot = getFieldSlotWithCheck(row, col);
    if (!fieldSlot || fieldSlot->type != FIELD_TYPE_INTEGER) {
        return FALSE;
    }

#if WINDOW_STORAGE_INLINE_NUMERICS
    *valueOut = fieldSlot->data.l;
#else
    *valueOut = copyOutInt64(fieldSlot->data.buffer.offset);
#endif
    return TRUE;
}

Boolean NativeCursorWindow::getDouble(unsigned int row, unsigned int col, Double * valueOut)
{
    field_slot_t * fieldSlot = getFieldSlotWithCheck(row, col);
    if (!fieldSlot || fieldSlot->type != FIELD_TYPE_FLOAT) {
        return FALSE;
    }

#if WINDOW_STORAGE_INLINE_NUMERICS
    *valueOut = fieldSlot->data.d;
#else
    *valueOut = copyOutDouble(fieldSlot->data.buffer.offset);
#endif
    return TRUE;
}

Boolean NativeCursorWindow::getNull(unsigned int row, unsigned int col, Boolean* valueOut)
{
    field_slot_t * fieldSlot = getFieldSlotWithCheck(row, col);
    if (!fieldSlot) {
        return FALSE;
    }

    if (fieldSlot->type != FIELD_TYPE_NULL) {
        *valueOut = FALSE;
    } else {
        *valueOut = TRUE;
    }
    return TRUE;
}
