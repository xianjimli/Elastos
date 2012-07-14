
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ZLElFileOutputStream.h"
#include "ZLElFSManager.h"

ZLElFileOutputStream::ZLElFileOutputStream(const std::string &name) : myName(name), myHasErrors(false), myFile(0) {
}

ZLElFileOutputStream::~ZLElFileOutputStream() {
	close();
}

bool ZLElFileOutputStream::open() {
	close();

	myTemporaryName = myName + ".XXXXXX" + '\0';
	mode_t currentMask = umask(S_IRWXO | S_IRWXG);
	int temporaryFileDescriptor = ::mkstemp(const_cast<char*>(myTemporaryName.data()));
	umask(currentMask);
	if (temporaryFileDescriptor == -1) {
		return false;
	}

	myFile = fdopen(temporaryFileDescriptor, "w+");
	return myFile != 0;
}

void ZLElFileOutputStream::write(const char *data, size_t len) {
	if (::fwrite(data, 1, len, myFile) != len) {
		myHasErrors = true;
	}
}

void ZLElFileOutputStream::write(const std::string &str) {
	if (::fwrite(str.data(), 1, str.length(), myFile) != (size_t)str.length()) {
		myHasErrors = true;
	}
}

void ZLElFileOutputStream::close() {
	if (myFile != 0) {
		::fclose(myFile);
		myFile = 0;
		if (!myHasErrors) {
			rename(myTemporaryName.c_str(), myName.c_str());
		}
	}
}
