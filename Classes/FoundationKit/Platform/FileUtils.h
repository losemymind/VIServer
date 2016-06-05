/****************************************************************************
 Copyright (c) 2015 libo All rights reserved.
 
 losemymind.libo@gmail.com
 
 ****************************************************************************/
#ifndef LOSEMYMIND_FILEUTILS_H
#define LOSEMYMIND_FILEUTILS_H
#pragma once

#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/Data.h"
#include "FoundationKit/Foundation/Singleton.h"
#include <string>
#include <vector>

NS_FK_BEGIN

class FileUtils : public Singleton<FileUtils>
{
public:
    ~FileUtils();

   /**
    * Set default resource root path.
    */
    void setDefaultResourceRootPath(const std::string& path);

    std::string getResourceRootPath();

    std::string fullPathForFilename(const std::string &filename) const;

    void setSearchPaths(const std::vector<std::string>& searchPaths);
    void addSearchPath(const std::string & searchpath, const bool front = false);
    const std::vector<std::string>& getSearchPaths() const;
    void setWritablePath(const std::string& writablePath);
    std::string getWritablePath() const;

   /**
    *  Checks whether the path is a directory.
    *
    *  @param dirPath The path of the directory, it could be a relative or an absolute path.
    *  @return True if the directory exists, false if not.
    */
    bool isDirectoryExist(const std::string& dirPath) const;


   /**
    *  Checks whether the 'path' is a directory.
    *
    *  @param path The path of the directory, it could be a relative or an absolute path.
    *  @return True if the 'path' is a directory, false if not.
    */
    bool isDirectory(const std::string& path)const;

   /**
    *  Checks whether the path is an absolute path.
    *
    *  @note On Android, if the parameter passed in is relative to "assets/", this method will treat it as an absolute path.
    *        Also on Blackberry, path starts with "app/native/Resources/" is treated as an absolute path.
    *
    *  @param path The path that needs to be checked.
    *  @return True if it's an absolute path, false if not.
    */
    bool isAbsolutePath(const std::string& path) const;

   /**
    *  Gets string from a file.
    */
    std::string readStringFromFile(const std::string& filename);

   /**
    *  Creates binary data from a file.
    *  @return A data object.
    */
    Data readDataFromFile(const std::string& filename);

   /**
    *  Gets resource file data from a zip file.
    *
    *  @param[in]  filename The resource file name which contains the relative path of the zip file.
    *  @param[out] size If the file read operation succeeds, it will be the data size, otherwise 0.
    *  @return Upon success, a pointer to the data is returned, otherwise nullptr.
    *  @warning Recall: you are responsible for calling free() on any Non-nullptr pointer returned.
    */
    Data readDataFromZip(const std::string& zipFilePath, const std::string& filename, size_t *size);

   /**
    *  write a string into a file
    *
    * @param dataStr the string want to save
    * @param fullPath The full path to the file you want to save a string
    * @return bool True if write success
    */
    bool writeStringToFile(const std::string& dataStr, const std::string& fullPath);

   /**
    * write Data into a file
    *
    *@param retData the data want to save
    *@param fullPath The full path to the file you want to save a string
    *@return bool
    */
    bool writeDataToFile(Data retData, const std::string& fullPath);

   /**
    *  Gets filename extension is a suffix (separated from the base filename by a dot) in lower case.
    *  Examples of filename extensions are .png, .jpeg, .exe, .dmg and .txt.
    *  @param filePath The path of the file, it could be a relative or absolute path.
    *  @return suffix for filename in lower case or empty if a dot not found.
    */
    std::string getFileExtension(const std::string& filePath) const;

    std::string getFileName(const std::string& filePath)const;
    std::string getFileNameWithoutExtension(const std::string& filePath)const;
    std::string getFilePathWithoutFileName(const std::string& filePath)const;

   /**
    *  Retrieve the file size.
    *
    *  @note If a relative path was passed in, it will be inserted a default root path at the beginning.
    *  @param filepath The path of the file, it could be a relative or absolute path.
    *  @return The file size.
    */
    long getFileSize(const std::string &filepath)const;

   /**
    *  copy a file
    *
    *  @param oldfullpath  The current fullpath of the file. Includes path and name.
    *  @param newfullpath  The new fullpath of the file. Includes path and name.
    *  @return True if the file have been copy successfully, false if not.
    */
    bool copyFile(const std::string &oldfullpath, const std::string &newfullpath)const;

   /**
    *  move a file
    *
    *  @param oldfullpath  The current fullpath of the file. Includes path and name.
    *  @param newfullpath  The new fullpath of the file. Includes path and name.
    *  @return True if the file have been move successfully, false if not.
    */
    bool moveFile(const std::string &oldfullpath, const std::string &newfullpath)const;


   /**
    *  get all files from a directory
    *
    *  @param dirPath The path of the directory.
    */
    void getFilesFromDir(const std::string& dirPath, std::vector<std::string>& files, bool includeChild = false)const;

    /**
     *  get all files from a directory
     *
     *  @param dirPath The path of the directory.
     */
    void getDirs(const std::string& dirPath, std::vector<std::string>& dirs, bool includeChild = false)const;

    std::string getDirName(const std::string& dirPath)const ;

    //=====================================================================
    //                         Implementation in platform 
    //=====================================================================

   /**
    *  Checks whether a file exists.
    *
    *  @note If a relative path was passed in, it will be inserted a default root path at the beginning.
    *  @param filename The path of the file, it could be a relative or absolute path.
    *  @return True if the file exists, false if not.
    */
    bool isFileExist(const std::string& filename) const;

   /**
    *  Creates a directory.
    *
    *  @param dirPath The path of the directory, it must be an absolute path.
    *  @return True if the directory have been created successfully, false if not.
    */
    bool createDirectory(const std::string& dirPath);

   /**
    *  Removes a directory.
    *
    *  @param dirPath  The full path of the directory, it must be an absolute path.
    *  @return True if the directory have been removed successfully, false if not.
    */
    bool removeDirectory(const std::string& dirPath);

   /**
    *  Removes a file.
    *
    *  @param filepath The full path of the file, it must be an absolute path.
    *  @return True if the file have been removed successfully, false if not.
    */
    bool removeFile(const std::string &filepath);

   /**
    *  Renames a file under the given directory.
    *
    *  @param path     The parent directory path of the file, it must be an absolute path.
    *  @param oldname  The current name of the file.
    *  @param name     The new name of the file.
    *  @return True if the file have been renamed successfully, false if not.
    */
    bool renameFile(const std::string &path, const std::string &oldname, const std::string &name);

   /**
    *  Renames a file under the given directory.
    *
    *  @param oldfullpath  The current fullpath of the file. Includes path and name.
    *  @param newfullpath  The new fullpath of the file. Includes path and name.
    *  @return True if the file have been renamed successfully, false if not.
    */
    bool renameFile(const std::string &oldfullpath, const std::string &newfullpath);

protected:
    friend Singleton < FileUtils > ;
    FileUtils();
    void initRootPath();
    
    std::string                 _resourceRootPath;
    std::string                 _writablePath;
    std::vector<std::string>    _searchPaths;


};

// D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
static inline std::string convertPathFormatToUnixStyle(const std::string& path)
{
    std::string ret = path;
    size_t len = ret.length();
	for (size_t i = 0; i < len; ++i)
    {
        if (ret[i] == '\\')
        {
            ret[i] = '/';
        }
    }
    return ret;
}

NS_FK_END
#endif // LOSEMYMIND_FILEUTILS_H



