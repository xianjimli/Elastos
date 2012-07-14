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

#ifndef __BOOK_H__
#define __BOOK_H__

#include <string>
#include <map>

#include <shared_ptr.h>

#include "Lists.h"

#include "Content.h"


class Author;
class Tag;

class Book {

public:
	static const std::string AutoEncoding;

public:
	static shared_ptr<Book> createBook(
		const std::string &filePath,
		int id,
		const std::string &encoding,
		const std::string &language,
		const std::string &title
	);

	static shared_ptr<Book> loadFromFile(const std::string &filePath);

	// this method is used in Migration only
	static shared_ptr<Book> loadFromBookInfo(const std::string &filePath);

private:
	Book(const std::string filePath, int id);

public:
	~Book();

public: // unmodifiable book methods
	const std::string &title() const;
	const std::string &filePath() const;
	const std::string &language() const;
	const std::string &encoding() const;
	const std::string &seriesTitle() const;
    const std::string &getFilePrefix() const;
	int indexInSeries() const;

	const TagList &tags() const;
	const AuthorList &authors() const;
	const ContentList &contents() const;
    const ContentList::const_iterator contentsBegin() const;
    const ContentList::const_iterator contentsEnd() const;

public: // modifiable book methods
	void setTitle(const std::string &title);
	void setLanguage(const std::string &language);
	void setEncoding(const std::string &encoding);
	void setSeries(const std::string &title, int index);
	void setFilePrefix(const std::string &title);

public:
	bool addTag(shared_ptr<Tag> tag);
	bool addTag(const std::string &fullName);
	bool removeTag(shared_ptr<Tag> tag, bool includeSubTags);
	bool renameTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags);
	bool cloneTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags);
	void removeAllTags();

	void addAuthor(shared_ptr<Author> author);
	void addAuthor(const std::string &displayName, const std::string &sortKey = std::string());
	bool replaceAuthor(shared_ptr<Author> from, shared_ptr<Author> to);
	void removeAllAuthors();

    void addImageFile(std::string filName);
	void addContent(shared_ptr<Content> content);
	void addContent(std::string text, int order, int level, std::string href);
	void removeAllContent();
    int  getChapterCount();
//    bool getImages(char *dirPath);
    void addFirstChapter(const std::string &strHref);
    std::string getFirstChapter();

    void addHRef(std::string hRef);
    std::vector<std::string> getHRefs();
//    bool getFile(char *dirPath, char *fileName);
    void createParagraph();
    void addParagraph(const std::vector<std::string> & paragraph);
	void addChapter(const std::vector<std::string> & chapter);
	void addChapterValue(int chapNum, int startParaNum);
    const std::vector<char* > & getChapters() const;
	const std::vector<char* > & getParagraphs() const;
	const std::map<int,int> &getChapterMap() const;

public:
	int bookId() const;
	void setBookId(int bookId);

private:
	int myBookId;

	const std::string myFilePath;
    std::string myFilePrefix;
	std::string myTitle;
	std::string myLanguage;
	std::string myEncoding;
	std::string mySeriesTitle;
    std::string myFirstChapter;
	int myIndexInSeries;
	TagList myTags;
	AuthorList myAuthors;
	ContentList myContents;
    std::vector<std::string> myImageFiles;

	std::vector<std::string> myHRefs;
	std::vector<char*> myChapterLists;
	std::vector<char*> myParagraphs;
	std::map<int, int> myChapterMap;

private: // disable copying
	Book(const Book &);
	const Book &operator = (const Book &);
    bool makeImagePathDirs(std::string path, std::string fileName);
//    bool saveImageFile(std::string path, std::string fileName);
};

inline const std::string &Book::title() const { return myTitle; }
inline const std::string &Book::filePath() const { return myFilePath; }
inline const std::string &Book::language() const { return myLanguage; }
inline const std::string &Book::encoding() const { return myEncoding; }
inline const std::string &Book::seriesTitle() const { return mySeriesTitle; }
inline const std::string &Book::getFilePrefix() const { return myFilePrefix; }
inline int Book::indexInSeries() const { return myIndexInSeries; }

inline const TagList &Book::tags() const { return myTags; }
inline const AuthorList &Book::authors() const { return myAuthors; }
inline const ContentList &Book::contents() const { return myContents; }
inline const ContentList::const_iterator Book::contentsBegin() const { return myContents.begin(); }
inline const ContentList::const_iterator Book::contentsEnd() const { return myContents.end(); }

inline int Book::bookId() const { return myBookId; }
inline void Book::setBookId(int bookId) { myBookId = bookId; }
inline void Book::setFilePrefix(const std::string &prefix) { myFilePrefix = prefix; }
inline void Book::addImageFile(std::string filName) { myImageFiles.push_back(filName); }
inline void Book::addFirstChapter(const std::string &strHref) { myFirstChapter = strHref; }
inline std::string Book::getFirstChapter() { return myFirstChapter; }

inline void Book::addHRef(std::string hRef) { myHRefs.push_back(hRef); }
inline std::vector<std::string> Book::getHRefs() { return myHRefs; }
inline const std::vector<char*> & Book::getChapters() const { return myChapterLists;}
inline const std::vector<char*> & Book::getParagraphs() const { return myParagraphs;}

//multiple definition of `Book::getChapterMap() const'
inline const std::map<int,int> & Book::getChapterMap() const {return myChapterMap;}

#endif /* __BOOK_H__ */
