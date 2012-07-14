
#ifndef __ZLELFILEINPUTSTREAM_H__
#define __ZLELFILEINPUTSTREAM_H__

#include <stdio.h>

#include <ZLInputStream.h>

class ZLElFileInputStream : public ZLInputStream {

public:
	ZLElFileInputStream(const std::string &name);
	~ZLElFileInputStream();
	bool open();
	size_t read(char *buffer, size_t maxSize);
	void close();

	void seek(int offset, bool absoluteOffset);
	size_t offset() const;
	size_t sizeOfOpened();

private:
	std::string myName;
	FILE *myFile;
	bool myNeedRepositionToStart;
};

#endif /* __ZLELFILEINPUTSTREAM_H__ */
