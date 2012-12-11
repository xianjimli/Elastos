
#ifndef __NATIVECURSORWINDOW_H__
#define __NATIVECURSORWINDOW_H__

#include <binder/IMemory.h>
#include <utils/RefBase.h>


#if LOG_NDEBUG

#define IF_LOG_WINDOW() if (false)
#define LOG_WINDOW(...)

#else

#define IF_LOG_WINDOW() IF_LOG(LOG_DEBUG, "CursorWindow")
#define LOG_WINDOW(...) LOG(LOG_DEBUG, "CursorWindow", __VA_ARGS__)

#endif

typedef struct
{
    uint32_t numRows;
    uint32_t numColumns;
} window_header_t;

typedef struct
{
    uint32_t offset;
} row_slot_t;

typedef struct
{
    uint8_t type;
    union {
        double d;
        int64_t l;
        struct {
            uint32_t offset;
            uint32_t size;
        } buffer;
    } data;
} __attribute__((packed)) field_slot_t;

#define FIELD_TYPE_INTEGER 1
#define FIELD_TYPE_FLOAT 2
#define FIELD_TYPE_STRING 3
#define FIELD_TYPE_BLOB 4
#define FIELD_TYPE_NULL 5

/**
 * This class stores a set of rows from a database in a buffer. The begining of the
 * window has first chunk of row_slot_ts, which are offsets to the row directory, followed by
 * an offset to the next chunk in a linked-list of additional chunk of row_slot_ts in case
 * the pre-allocated chunk isn't big enough to refer to all rows. Each row directory has a
 * field_slot_t per column, which has the size, offset, and type of the data for that field.
 * Note that the data types come from sqlite3.h.
 */
class NativeCursorWindow
{
public:
    NativeCursorWindow(size_t maxSize);
    NativeCursorWindow() {}
    bool setMemory(const android::sp<android::IMemory>&);
    ~NativeCursorWindow();

    bool initBuffer(bool localOnly);
    android::sp<android::IMemory> getMemory() {return mMemory;}

    size_t size() {return mSize;}
    uint8_t* data() {return mData;}
    uint32_t getNumRows() {return mHeader->numRows;}
    uint32_t getNumColumns() {return mHeader->numColumns;}
    void freeLastRow()
    {
        if (mHeader->numRows > 0) {
            mHeader->numRows--;
        }
    }
    bool setNumColumns(uint32_t numColumns)
    {
        uint32_t cur = mHeader->numColumns;
        if (cur > 0 && cur != numColumns) {
            LOGE("Trying to go from %d columns to %d", cur, numColumns);
            return false;
        }
        mHeader->numColumns = numColumns;
        return true;
    }

    int32_t freeSpace();

    void clear();

    /**
     * Allocate a row slot and its directory. The returned
     * pointer points to the begining of the row's directory
     * or NULL if there wasn't room. The directory is
     * initialied with NULL entries for each field.
     */
    field_slot_t* allocRow();

    /**
     * Allocate a portion of the window. Returns the offset
     * of the allocation, or 0 if there isn't enough space.
     * If aligned is true, the allocation gets 4 byte alignment.
     */
    uint32_t alloc(size_t size, bool aligned = false);

    uint32_t read_field_slot(int row, int column, field_slot_t * slot);

    /**
     * Copy data into the window at the given offset.
     */
    void copyIn(uint32_t offset, uint8_t const * data, size_t size);
    void copyIn(uint32_t offset, int64_t data);
    void copyIn(uint32_t offset, double data);

    void copyOut(uint32_t offset, uint8_t * data, size_t size);
    int64_t copyOutLong(uint32_t offset);
    double copyOutDouble(uint32_t offset);

    bool putLong(unsigned int row, unsigned int col, int64_t value);
    bool putDouble(unsigned int row, unsigned int col, double value);
    bool putNull(unsigned int row, unsigned int col);

    bool getLong(unsigned int row, unsigned int col, int64_t * valueOut);
    bool getDouble(unsigned int row, unsigned int col, double * valueOut);
    bool getNull(unsigned int row, unsigned int col, bool * valueOut);

    uint8_t* offsetToPtr(uint32_t offset) {return mData + offset;}

    row_slot_t* allocRowSlot();

    row_slot_t* getRowSlot(int row);

    /**
     * return NULL if Failed to find rowSlot or
     * Invalid rowSlot
     */
    field_slot_t* getFieldSlotWithCheck(int row, int column);
    field_slot_t* getFieldSlot(int row, int column)
    {
        int fieldDirOffset = getRowSlot(row)->offset;
        return ((field_slot_t *)offsetToPtr(fieldDirOffset)) + column;
    }

private:
    uint8_t * mData;
    size_t mSize;
    size_t mMaxSize;
    window_header_t * mHeader;
    android::sp<android::IMemory> mMemory;

    /**
     * Offset of the lowest unused data byte in the array.
     */
    uint32_t mFreeOffset;
};

#endif //__NATIVECURSORWINDOW_H__
