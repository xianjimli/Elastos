
#ifndef __ZLELFILEOUTPUTSTREAM_H__
#define __ZLELFILEOUTPUTSTREAM_H__

#include <stdio.h>

#include <ZLOutputStream.h>

class ZLElFileOutputStream : public ZLOutputStream {

public:
	ZLElFileOutputStream(const std::string &name);
	~ZLElFileOutputStream();
	bool open();
	void write(const char *data, size_t len);
	void write(const std::string &str);
	void close();

private:
	std::string myName;
	std::string myTemporaryName;
	bool myHasErrors;
	FILE *myFile;
};

#endif /* __ZLELFILEOUTPUTSTREAM_H__ */
