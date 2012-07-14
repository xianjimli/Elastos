
#ifndef __ZLELFSDIR_H__
#define __ZLELFSDIR_H__

#include "../../filesystem/ZLFSDir.h"

class ZLElFSDir : public ZLFSDir {

public:
	ZLElFSDir(const std::string &name) : ZLFSDir(name) {}

	void collectSubDirs(std::vector<std::string> &names, bool includeSymlinks);
	void collectFiles(std::vector<std::string> &names, bool includeSymlinks);
};

#endif /* __ZLELFSDIR_H__ */
