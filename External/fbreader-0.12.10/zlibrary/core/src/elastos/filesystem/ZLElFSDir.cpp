
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

#include "ZLElFSDir.h"
#include "ZLElFSManager.h"

void ZLElFSDir::collectSubDirs(std::vector<std::string> &names, bool includeSymlinks) {
	DIR *dir = opendir(path().c_str());
	if (dir != 0) {
		const std::string namePrefix = path() + delimiter();
		const dirent *file;
		struct stat fileInfo;
		std::string shortName;
		while ((file = readdir(dir)) != 0) {
			shortName = file->d_name;
			if ((shortName == ".") || (shortName == "..")) {
				continue;
			}
			const std::string path = namePrefix + shortName;
			if (includeSymlinks) {
				stat(path.c_str(), &fileInfo);
			} else {
				stat(path.c_str(), &fileInfo);
			}
			if (S_ISDIR(fileInfo.st_mode)) {
				names.push_back(shortName);
			}
		}
		closedir(dir);
	}
}

void ZLElFSDir::collectFiles(std::vector<std::string> &names, bool includeSymlinks) {
	DIR *dir = opendir(path().c_str());
	if (dir != 0) {
		const std::string namePrefix = path() + delimiter();
		const dirent *file;
		struct stat fileInfo;
		std::string shortName;
		while ((file = readdir(dir)) != 0) {
			shortName = file->d_name;
			if ((shortName == ".") || (shortName == "..")) {
				continue;
			}
			const std::string path = namePrefix + shortName;
			if (includeSymlinks) {
				stat(path.c_str(), &fileInfo);
			} else {
				stat(path.c_str(), &fileInfo);
			}
			if (S_ISREG(fileInfo.st_mode)) {
				names.push_back(shortName);
			}
		}
		closedir(dir);
	}
}
