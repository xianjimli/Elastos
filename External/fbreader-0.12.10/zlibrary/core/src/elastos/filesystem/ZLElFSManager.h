
#ifndef __ZLELFSMANAGER_H__
#define __ZLELFSMANAGER_H__

#include "../../filesystem/ZLFSManager.h"

class ZLElFSManager : public ZLFSManager {
public:
    static void createInstance() { ourInstance = new ZLElFSManager(); }

protected:
    void normalizeRealPath(std::string &path) const;

private:
    std::string resolveSymlink(const std::string &path) const;
    ZLFSDir *createNewDirectory(const std::string &path) const;
    ZLFSDir *createPlainDirectory(const std::string &path) const;
    ZLInputStream *createPlainInputStream(const std::string &path) const;
    ZLOutputStream *createOutputStream(const std::string &path) const;
    bool removeFile(const std::string &path) const;

    ZLFileInfo fileInfo(const std::string &path) const;
    std::string convertFilenameToUtf8(const std::string &name) const;

    int findArchiveFileNameDelimiter(const std::string &path) const;
    shared_ptr<ZLDir> rootDirectory() const;
    const std::string &rootDirectoryPath() const;
    std::string parentPath(const std::string &path) const;

    bool canRemoveFile(const std::string &path) const;
};

#endif /* __ZLELFSMANAGER_H__ */
