/*
 * Copyright (C) 2009-2010 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <algorithm>
#include <set>

#include <ZLStringUtil.h>
#include <ZLFile.h>

#include <sys/stat.h>
#include <unistd.h>
#include <ZLLanguageList.h>

#include "Book.h"
#include "Author.h"
#include "Tag.h"

#include "../formats/FormatPlugin.h"
#include "../migration/BookInfo.h"

const std::string Book::AutoEncoding = "auto";

Book::Book(const std::string filePath, int id) : myBookId(id), myFilePath(filePath), myIndexInSeries(0) {
}

Book::~Book() {
	for (std::vector<char*>::const_iterator it = myChapterLists.begin(); it != myChapterLists.end(); ++it) {
		delete[] *it;
	}
}

shared_ptr<Book> Book::createBook(
	const std::string &filePath,
	int id,
	const std::string &encoding,
	const std::string &language,
	const std::string &title
) {
	Book *book = new Book(filePath, id);
	book->setEncoding(encoding);
	book->setLanguage(language);
	book->setTitle(title);
	return book;
}

shared_ptr<Book> Book::loadFromFile(const std::string &filePath) {
	ZLFile bookFile(filePath);
	if (!bookFile.exists() || bookFile.isDirectory())
	{
		return 0;
	}

	shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(bookFile, false);
	if (plugin.isNull()) {
		return 0;
	}

	shared_ptr<Book> book = new Book(filePath, 0);
	if (!plugin->readMetaInfo(*book)) {
		return 0;
	}

	if (book->title().empty()) {
		book->setTitle(ZLFile::fileNameToUtf8(bookFile.name(true)));
	}

	if (book->encoding().empty()) {
		book->setEncoding(AutoEncoding);
	}

	if (book->language().empty()) {
		book->setLanguage(PluginCollection::Instance().DefaultLanguageOption.value());
	}

	return book;
}

bool Book::addTag(shared_ptr<Tag> tag) {
	if (tag.isNull()) {
		return false;
	}
	TagList::const_iterator it = std::find(myTags.begin(), myTags.end(), tag);
	if (it == myTags.end()) {
		myTags.push_back(tag);
		return true;
	}
	return false;
}

bool Book::addTag(const std::string &fullName) {
	return addTag(Tag::getTagByFullName(fullName));
}

bool Book::removeTag(shared_ptr<Tag> tag, bool includeSubTags) {
	bool changed = false;
	for (TagList::iterator it = myTags.begin(); it != myTags.end();) {
		if (tag == *it || (includeSubTags && tag->isAncestorOf(*it))) {
			it = myTags.erase(it);
			changed = true;
		} else {
			++it;
		}
	}
	return changed;
}

bool Book::renameTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags) {
	if (includeSubTags) {
		std::set<shared_ptr<Tag> > tagSet;
		bool changed = false;
		for (TagList::const_iterator it = myTags.begin(); it != myTags.end(); ++it) {
			if (*it == from) {
				tagSet.insert(to);
				changed = true;
			} else {
				shared_ptr<Tag> newtag = Tag::cloneSubTag(*it, from, to);
				if (newtag.isNull()) {
					tagSet.insert(*it);
				} else {
					tagSet.insert(newtag);
					changed = true;
				}
			}
		}
		if (changed) {
			myTags.clear();
			myTags.insert(myTags.end(), tagSet.begin(), tagSet.end());
			return true;
		}
	} else {
		TagList::iterator it = std::find(myTags.begin(), myTags.end(), from);
		if (it != myTags.end()) {
			TagList::const_iterator jt = std::find(myTags.begin(), myTags.end(), to);
			if (jt == myTags.end()) {
				*it = to;
			} else {
				myTags.erase(it);
			}
			return true;
		}
	}
	return false;
}

bool Book::cloneTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags) {
	if (includeSubTags) {
		std::set<shared_ptr<Tag> > tagSet;
		for (TagList::const_iterator it = myTags.begin(); it != myTags.end(); ++it) {
			if (*it == from) {
				tagSet.insert(to);
			} else {
				shared_ptr<Tag> newtag = Tag::cloneSubTag(*it, from, to);
				if (!newtag.isNull()) {
					tagSet.insert(newtag);
				}
			}
		}
		if (!tagSet.empty()) {
			tagSet.insert(myTags.begin(), myTags.end());
			myTags.clear();
			myTags.insert(myTags.end(), tagSet.begin(), tagSet.end());
			return true;
		}
	} else {
		TagList::const_iterator it = std::find(myTags.begin(), myTags.end(), from);
		if (it != myTags.end()) {
			TagList::const_iterator jt = std::find(myTags.begin(), myTags.end(), to);
			if (jt == myTags.end()) {
				myTags.push_back(to);
				return true;
			}
		}
	}
	return false;
}

shared_ptr<Book> Book::loadFromBookInfo(const std::string &filePath) {
	BookInfo info(filePath);

	shared_ptr<Book> book = createBook(
		filePath, 0,
		info.EncodingOption.value(),
		info.LanguageOption.value(),
		info.TitleOption.value()
	);

	book->setSeries(
		info.SeriesTitleOption.value(),
		info.IndexInSeriesOption.value()
	);

	if (book->language().empty()) {
		book->setLanguage(PluginCollection::Instance().DefaultLanguageOption.value());
	}

	const std::string &tagList = info.TagsOption.value();
	if (!tagList.empty()) {
		size_t index = 0;
		do {
			size_t newIndex = tagList.find(',', index);
			book->addTag(Tag::getTagByFullName(tagList.substr(index, newIndex - index)));
			index = newIndex + 1;
		} while (index != 0);
	}

	const std::string &authorList = info.AuthorDisplayNameOption.value();
	if (!authorList.empty()) {
		size_t index = 0;
		do {
			size_t newIndex = authorList.find(',', index);
			book->addAuthor(authorList.substr(index, newIndex - index));
			index = newIndex + 1;
		} while (index != 0);
	}

	return book;
}

bool Book::replaceAuthor(shared_ptr<Author> from, shared_ptr<Author> to) {
	AuthorList::iterator it = std::find(myAuthors.begin(), myAuthors.end(), from);
	if (it == myAuthors.end()) {
		return false;
	}
	if (to.isNull()) {
		myAuthors.erase(it);
	} else {
		*it = to;
	}
	return true;
}

void Book::setTitle(const std::string &title) {
	myTitle = title;
}

void Book::setLanguage(const std::string &language) {
	if (!myLanguage.empty()) {
		const std::vector<std::string> &codes = ZLLanguageList::languageCodes();
		std::vector<std::string>::const_iterator it =
			std::find(codes.begin(), codes.end(), myLanguage);
		std::vector<std::string>::const_iterator jt =
			std::find(codes.begin(), codes.end(), language);
		if (it != codes.end() && jt == codes.end()) {
			return;
		}
	}
	myLanguage = language;
}

void Book::setEncoding(const std::string &encoding) {
	myEncoding = encoding;
}

void Book::setSeries(const std::string &title, int index) {
	mySeriesTitle = title;
	myIndexInSeries = index;
}

void Book::removeAllTags() {
	myTags.clear();
}

void Book::addAuthor(const std::string &displayName, const std::string &sortKey) {
	addAuthor(Author::getAuthor(displayName, sortKey));
}

void Book::addAuthor(shared_ptr<Author> author) {
	if (!author.isNull()) {
		myAuthors.push_back(author);
	}
}

void Book::removeAllAuthors() {
	myAuthors.clear();
}
void Book::addContent(shared_ptr<Content> content) {
	if (!content.isNull()) {
		myContents.push_back(content);
	}
}

void Book::addContent(std::string text, int order, int level, std::string href) {
	shared_ptr<Content> content = new Content(text, order, level, href);
	addContent(content);
}

void Book::removeAllContent() {
	myContents.clear();
}

int Book::getChapterCount() {
    return myContents.size();
}

bool Book::makeImagePathDirs(std::string path, std::string fileName)
{
    std::string::size_type pos = fileName.find_first_of('/');

    if (pos != std::string::npos) {
        std::string dirPath = path + fileName.substr(0, pos+1);
        if (access(dirPath.c_str(), 2) != 0) {
            int res = mkdir(dirPath.c_str(), 0777);
            if (res != 0)
                return false;
        }
        return makeImagePathDirs(dirPath, fileName.substr(pos+1, fileName.length()));
    }

    return true;
}

//bool Book::saveImageFile(std::string path, std::string fileName)
//{
//    int readSize, fileSize;
//    const std::string imageFilePath = path + fileName;
//    const std::string ZLfilePath = myFilePrefix + fileName;
//    shared_ptr<ZLInputStream> stream = ZLFile(ZLfilePath).inputStream();
//    if (!(stream->open())) return false;
//
//    FILE* fp = fopen(imageFilePath.c_str(), "wb");
//    if (fp == NULL) {
//        stream->close();
//        return false;
//    }
//
//    fileSize = stream->sizeOfOpened();
//    char *buff = (char*)malloc(fileSize);
//    readSize = stream->read(buff, fileSize);
//    if (readSize != fileSize) {
//        free(buff);
//        stream->close();
//        fclose(fp);
//        return false;
//    }
//
//    fwrite(buff, sizeof(char), fileSize, fp);
//    free(buff);
//    stream->close();
//    fclose(fp);
//
//    return true;
//}
//
//bool Book::getImages(char *dirPath)
//{
//    std::vector<std::string>::iterator it;
//    std::string dir = std::string(dirPath) + "/";
//    for (it = myImageFiles.begin(); it != myImageFiles.end(); ++it) {
//        if (!makeImagePathDirs(dir, *it)) {
//            return false;
//        }
//        if (!saveImageFile(dir, *it)) {
//            return false;
//        }
//    }
//
//    return true;
//}
//
//bool Book::getFile(char *dirPath, char *fileName)
//{
//    std::string dir = std::string(dirPath) + "/";
//    if (!makeImagePathDirs(dir, fileName)) {
//        return false;
//    }
//    if (!saveImageFile(dir, fileName)) {
//        return false;
//    }
//
//    return true;
//}

void Book::addChapter(const std::vector<std::string> & chapter)
{
    //printf("<%s, %d>\n", __FILE__, __LINE__);
	if (chapter.size() == 0) {
		return;
	}

	size_t len = 0;
	for (std::vector<std::string>::const_iterator it = chapter.begin(); it != chapter.end(); ++it) {
		len += it->length();
	}

	char* chapterContent = new char[len + 1];
	memset(chapterContent, len + 1, 0);
	if(chapterContent == NULL) {
        return;
	}

	size_t offset = 0;
	for (std::vector<std::string>::const_iterator it = chapter.begin(); it != chapter.end(); ++it) {
		memcpy(chapterContent + offset, it->data(), it->length());
		offset += it->length();
	}

    chapterContent[len] = '\0';
    //printf("<%s, %d>, chapterContent=[%s]\n", __FILE__, __LINE__, chapterContent);

    myChapterLists.push_back(chapterContent);
}


//paragraph array.
void Book::addParagraph(const std::vector<std::string> & paragraph)
{
	if (paragraph.size() == 0) {
		return;
	}

	size_t len = 0;
	for (std::vector<std::string>::const_iterator it = paragraph.begin(); it != paragraph.end(); ++it) {
		len += it->length();
	}

	char* paragraphrContent = new char[len + 1];
	memset(paragraphrContent, len + 1, 0);
	if(paragraphrContent == NULL) {
        return;
	}

	size_t offset = 0;
	for (std::vector<std::string>::const_iterator it = paragraph.begin(); it != paragraph.end(); ++it) {
		memcpy(paragraphrContent + offset, it->data(), it->length());
		offset += it->length();
	}

    paragraphrContent[len] = '\0';
    //printf("<%s, %d>, paragraphrContent=[%s]\n", __FILE__, __LINE__, paragraphrContent);

    myParagraphs.push_back(paragraphrContent);
}

//const std::map<int,int> & Book::getChapterMap() const
//{
//    return myChapterMap;
//}

void Book::addChapterValue(int chapNum, int startParaNum)
{
    myChapterMap[chapNum] = startParaNum;
}


