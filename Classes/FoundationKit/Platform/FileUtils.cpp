/****************************************************************************
 Copyright (c) 2015 libo All rights reserved.
 
 losemymind.libo@gmail.com
 
 ****************************************************************************/
#include <sys/stat.h>
#include <algorithm>
#include <dirent.h> // for DIR
#include "FileUtils.h"
#include "FoundationKit/Foundation/Logger.h"
#include "FoundationKit/Foundation/StringUtils.h"
#include "unzip.h"

//#include <codecvt> //ndk is not support

#if (TARGET_PLATFORM==PLATFORM_WIN32)
#include <Windows.h>
#endif

#if (TARGET_PLATFORM==PLATFORM_IOS)
#define MINIZIP_FROM_SYSTEM
#endif

NS_FK_BEGIN

static Data getData(const std::string& filename, bool forString)
{
    if (filename.empty())
    {
        return Data::Null;
    }

    Data ret;
    unsigned char* buffer = nullptr;
    size_t size = 0;
    size_t readsize = 0;
    const char* mode = nullptr;

    if (forString)
        mode = "rt";
    else
        mode = "rb";

    auto fileutils = FileUtils::getInstance();
    do
    {
        // Read the file from hardware
        std::string fullPath = fileutils->fullPathForFilename(filename);
        FILE *fp = fopen(fullPath.c_str(), mode);
        BREAK_IF(!fp);
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (forString)
        {
            buffer = new unsigned char[sizeof(unsigned char) * (size + 1)];
            buffer[size] = '\0';
        }
        else
        {
            buffer = new unsigned char[sizeof(unsigned char) * (size)];
        }

        readsize = fread(buffer, sizeof(unsigned char), size, fp);
        fclose(fp);

        if (forString && readsize <= size)
        {
            buffer[readsize] = '\0';
        }
    } while (0);

    if (nullptr == buffer || 0 == readsize)
    {
        LOG_ERROR("Get data from file %s failed", filename.c_str());
    }
    else
    {
        ret.fastSet(buffer, readsize, true);
    }

    return ret;
}

FileUtils::FileUtils()
{
    initRootPath();
}

FileUtils::~FileUtils()
{

}

void FileUtils::setDefaultResourceRootPath(const std::string& path)
{
    LOG_ASSERT(!path.empty(), "setDefaultResourceRootPath:Invalid path.");
    _resourceRootPath = path;
    if (_resourceRootPath.back() != '/' || _resourceRootPath.back() != '\\')
    {
        _resourceRootPath.push_back('/');
    }

    _resourceRootPath = convertPathFormatToUnixStyle(_resourceRootPath);
}

std::string FileUtils::getResourceRootPath()
{
    return _resourceRootPath;
}


std::string FileUtils::fullPathForFilename(const std::string &filename) const
{
    if (filename.empty())
    {
        return "";
    }

    if (isAbsolutePath(filename))
    {
        return filename;
    }

    for (const auto& searchIt : _searchPaths)
    {
        std::string fullPath = _resourceRootPath + searchIt + filename;
        if (isFileExist(fullPath))
        {
            return fullPath;
        }
    }

    // The file wasn't found, return empty string.
    return "";
}

void FileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    _searchPaths = searchPaths;
}

void FileUtils::addSearchPath(const std::string & searchpath, const bool front /*= false*/)
{
    std::string prefix;
    if (!isAbsolutePath(searchpath))
        prefix = _resourceRootPath;

    std::string path = prefix + searchpath;
    if (path.length() > 0 && path[path.length() - 1] != '/')
    {
        path += "/";
    }
    if (front) {
        _searchPaths.insert(_searchPaths.begin(), path);
    }
    else {
        _searchPaths.push_back(path);
    }
}

const std::vector<std::string>& FileUtils::getSearchPaths() const
{
    return _searchPaths;
}

void FileUtils::setWritablePath(const std::string& writablePath)
{
    _writablePath = writablePath;
}

bool FileUtils::isDirectoryExist(const std::string& dirPath) const
{
#if (TARGET_PLATFORM==PLATFORM_WIN32)
    std::wstring utf16Str = StringUtils::string2UTF8wstring(dirPath);
    unsigned long fAttrib = GetFileAttributes(reinterpret_cast<LPCWSTR>(utf16Str.c_str()));
    if (fAttrib != INVALID_FILE_ATTRIBUTES && (fAttrib & FILE_ATTRIBUTE_DIRECTORY))
    {
        return true;
    }
    return false;
#else
    struct stat st;
    if (stat(dirPath.c_str(), &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return false;
#endif
}

bool FileUtils::isDirectory(const std::string& path)const
{
    return path[path.size() - 1] == '/' || path[path.size() - 1] == '\\';
}

bool FileUtils::isAbsolutePath(const std::string& path) const
{
#if (TARGET_PLATFORM==PLATFORM_WIN32)
    if ((path.length() > 2 && 
        ((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z'))&& 
        path[1] == ':') || (path[0] == '/' && path[1] == '/'))
    {
        return true;
    }
    return false;
#else
    return (path[0] == '/');
#endif
}

#if (TARGET_PLATFORM==PLATFORM_WIN32)
#else
#endif


std::string FileUtils::readStringFromFile(const std::string& filename)
{
    Data data = getData(filename, true);
    if (data.isNull())
        return "";

    std::string ret((const char*)data.getBytes());
    return ret;
}

Data FileUtils::readDataFromFile(const std::string& filename)
{
    return getData(filename, false);
}

Data FileUtils::readDataFromZip(const std::string& zipFilePath, const std::string& filename, size_t *size)
{
    Data retData;
    unzFile file = nullptr;
    *size = 0;

    do
    {
        BREAK_IF(zipFilePath.empty());

        file = unzOpen(zipFilePath.c_str());
        BREAK_IF(!file);

        // FIXME: Other platforms should use upstream minizip like mingw-w64
#ifdef MINIZIP_FROM_SYSTEM
        int ret = unzLocateFile(file, filename.c_str(), NULL);
#else
        int ret = unzLocateFile(file, filename.c_str(), 1);
#endif
        BREAK_IF(UNZ_OK != ret);

        char filePathA[260];
        unz_file_info fileInfo;
        ret = unzGetCurrentFileInfo(file, &fileInfo, filePathA, sizeof(filePathA), nullptr, 0, nullptr, 0);
        BREAK_IF(UNZ_OK != ret);

        ret = unzOpenCurrentFile(file);
        BREAK_IF(UNZ_OK != ret);
        unsigned char * buffer = (unsigned char*)malloc(fileInfo.uncompressed_size);
        int readedSize = unzReadCurrentFile(file, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
        LOG_ASSERT(readedSize == 0 || readedSize == (int)fileInfo.uncompressed_size, "the file size is wrong");
        UNUSED_ARG(readedSize);
        *size = fileInfo.uncompressed_size;
        unzCloseCurrentFile(file);

        retData.fastSet(buffer, *size, true);

    } while (0);

    if (file)
    {
        unzClose(file);
    }

    return retData;
}

bool FileUtils::writeStringToFile(const std::string& dataStr, const std::string& fullPath)
{
    Data retData;
    retData.copy((unsigned char*)dataStr.c_str(), dataStr.size());
    return writeDataToFile(retData, fullPath);
}

bool FileUtils::writeDataToFile(Data retData, const std::string& fullPath)
{
    LOG_ASSERT(!fullPath.empty() && retData.getSize() != 0, "Invalid parameters.");
    do
    {
        size_t size = 0;
        const char* mode = "wb";
        // Read the file from hardware
        FILE *fp = fopen(fullPath.c_str(), mode);
        BREAK_IF(!fp);
        size = retData.getSize();
        fwrite(retData.getBytes(), size, 1, fp);
        fclose(fp);
        return true;
    } while (0);

    return false;
}

std::string FileUtils::getFileExtension(const std::string& filePath) const
{
    std::string fileExtension;
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos)
    {
        fileExtension = filePath.substr(pos, filePath.length());

        std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
    }

    return fileExtension;
}

std::string FileUtils::getFileName(const std::string& filePath)const
{
    std::string fileName = "";
    if (!isDirectory(filePath))
    {
        size_t pos = filePath.find_last_of("/");
        if (pos != std::string::npos)
        {
            fileName = filePath.substr(pos + 1);
        }
    }
    return fileName;
}

std::string FileUtils::getFileNameWithoutExtension(const std::string& filePath)const
{
    std::string fileName = "";
    if (!isDirectory(filePath))
    {
        size_t posStart = filePath.find_last_of("/");
        size_t posEnd = filePath.find_last_of(".");
        if (posEnd == std::string::npos)
            posEnd = filePath.size() - 1;
        if (posStart != std::string::npos)
        {
            fileName = filePath.substr(posStart + 1, posEnd - posStart-1);
        }
    }
    return fileName;
}

std::string FileUtils::getFilePathWithoutFileName(const std::string& filePath)const
{
    std::string fileDir = "";
    if (!isDirectory(filePath))
    {
        size_t pos = filePath.find_last_of("/");
        if (pos != std::string::npos)
        {
            fileDir = filePath.substr(0, pos + 1);
        }
    }
    return fileDir;
}

long FileUtils::getFileSize(const std::string &filepath)const
{
    LOG_ASSERT(!filepath.empty(), "Invalid path");

    std::string fullpath = filepath;
    if (!isAbsolutePath(filepath))
    {
        fullpath = fullPathForFilename(filepath);
        if (fullpath.empty())
            return 0;
    }

    struct stat info;
    // Get data associated with "crt_stat.c":
    int result = stat(fullpath.c_str(), &info);

    // Check if statistics are valid:
    if (result != 0)
    {
        // Failed
        return -1;
    }
    else
    {
        return (long)(info.st_size);
    }
}
bool FileUtils::copyFile(const std::string &oldfullpath, const std::string &newfullpath)const
{
    bool ret = false;
    const size_t BUFF_SIZE = 1024;
    do 
    {
        BREAK_IF(oldfullpath == newfullpath);
        const long srcFileSize = getFileSize(oldfullpath);
        FILE *fpSrc = fopen(oldfullpath.c_str(), "rb");
        BREAK_IF(!fpSrc);
        FILE *fpDes = fopen(newfullpath.c_str(), "wb");
        BREAK_IF(!fpDes);
        char read_buff[BUFF_SIZE];
        long readedSize = 0;
        long unreadSize = srcFileSize;
        while (readedSize < srcFileSize)
        {
            if (unreadSize > BUFF_SIZE)
            {
                readedSize += fread(read_buff, sizeof(char), BUFF_SIZE, fpSrc);
                fwrite(read_buff, 1, BUFF_SIZE, fpDes);
            }
            else
            {
                readedSize += fread(read_buff, sizeof(char), unreadSize, fpSrc);
                fwrite(read_buff, 1, unreadSize, fpDes);
            }
            unreadSize = srcFileSize - readedSize;
        }
        fclose(fpSrc);
        fclose(fpDes);
        ret = true;
    } while (false);
    return ret;
}

void FileUtils::getFilesFromDir(const std::string& dirPath, std::vector<std::string>& files, bool includeChild)const
{
    std::string finallyPath = dirPath;
    if (*(finallyPath.end() - 1) != '/' && *(finallyPath.end() - 1) != '\\')
    {
        finallyPath.append("/");
    }
    DIR* dir =opendir(finallyPath.c_str());
    if (!dir) return;
    dirent* entry = readdir(dir);
    while (entry)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            entry = readdir(dir);
            continue;
        }
            
        if (entry->d_type == DT_REG)
        {
            files.push_back(finallyPath + entry->d_name);
        }
        if (entry->d_type == DT_DIR && includeChild)
        {
            std::string strChildDir = finallyPath + entry->d_name;
            getFilesFromDir(strChildDir, files, includeChild);
        }

        entry = readdir(dir);
    }
    closedir(dir);
}

void FileUtils::getDirs(const std::string& dirPath, std::vector<std::string>& dirs, bool includeChild)const
{
    std::string finallyPath = dirPath;
    if (*(finallyPath.end() - 1) != '/' && *(finallyPath.end() - 1) != '\\')
    {
        finallyPath.append("/");
    }
    DIR* dir = opendir(finallyPath.c_str());
    if (!dir) return;
    dirent* entry = readdir(dir);
    while (entry)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            entry = readdir(dir);
            continue;
        }
        if (entry->d_type == DT_DIR)
        {
            dirs.push_back(finallyPath+entry->d_name);
            if (includeChild)
            {
                getDirs(finallyPath + entry->d_name, dirs, includeChild);
            }
        }

        entry = readdir(dir);
    }
    closedir(dir);
}

std::string FileUtils::getDirName(const std::string& dirPath)const
{
    std::string tempPath = convertPathFormatToUnixStyle(dirPath);
    std::string dirName;
    size_t pos = tempPath.find_last_of('/');
    if (pos != std::string::npos)
    {
        dirName = tempPath.substr(pos+1, tempPath.length());
    }

    return dirName;
}


#if (TARGET_PLATFORM == PLATFORM_WIN32)
// windows os implement should override in platform specific FileUtiles class
//bool moveFile(const std::string &oldfullpath, const std::string &newfullpath)const;
//bool createDirectory(const std::string& dirPath);
//bool removeDirectory(const std::string& dirPath);
//bool removeFile(const std::string &filepath);
//bool renameFile(const std::string &path, const std::string &oldname, const std::string &name);
//bool renameFile(const std::string &oldfullpath, const std::string &newfullpath);
#else
// default implements for unix like os
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

bool FileUtils::moveFile(const std::string &oldfullpath, const std::string &newfullpath)const
{
    return rename(oldfullpath.c_str(), oldfullpath.c_str()) != -1;
}


bool FileUtils::createDirectory(const std::string& path)
{
    LOG_ASSERT(!path.empty(), "Invalid path");

    if (isDirectoryExist(path))
        return true;

    // Split the path
    size_t start = 0;
    size_t found = path.find_first_of("/\\", start);
    std::string subpath;
    std::vector<std::string> dirs;

    if (found != std::string::npos)
    {
        while (true)
        {
            subpath = path.substr(start, found - start + 1);
            if (!subpath.empty())
                dirs.push_back(subpath);
            start = found + 1;
            found = path.find_first_of("/\\", start);
            if (found == std::string::npos)
            {
                if (start < path.length())
                {
                    dirs.push_back(path.substr(start));
                }
                break;
            }
        }
    }

    // Create path recursively
    subpath = "";
    for (int i = 0; i < dirs.size(); ++i)
    {
        subpath += dirs[i];
        DIR *dir = opendir(subpath.c_str());

        if (!dir)
        {
            // directory doesn't exist, should create a new one

            int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
            if (ret != 0 && (errno != EEXIST))
            {
                // current directory can not be created, sub directories can not be created too
                // should return
                return false;
            }
        }
        else
        {
            // directory exists, should close opened dir
            closedir(dir);
        }
    }
    return true;
}

#if (TARGET_PLATFORM == PLATFORM_IOS) || (TARGET_PLATFORM == PLATFORM_MAC)
// IOS MAC implement should override in platform specific FileUtiles class
// bool FileUtils::removeDirectory(const std::string& path)
#else
bool FileUtils::removeDirectory(const std::string& path)
{
    if (path.size() > 0 && path[path.size() - 1] != '/')
    {
        LOG_ERROR("Fail to remove directory, path must termniate with '/': %s", path.c_str());
        return false;
    }

    std::string command = "rm -r ";
    // Path may include space.
    command += "\"" + path + "\"";
    if (system(command.c_str()) >= 0)
        return true;
    else
        return false;
}
#endif //(TARGET_PLATFORM == PLATFORM_IOS) || (TARGET_PLATFORM == PLATFORM_MAC)

bool FileUtils::removeFile(const std::string &path)
{
    if (remove(path.c_str())) {
        return false;
    }
    else {
        return true;
    }
}

bool FileUtils::renameFile(const std::string &oldfullpath, const std::string &newfullpath)
{
    LOG_ASSERT(!oldfullpath.empty(), "Invalid path");
    LOG_ASSERT(!newfullpath.empty(), "Invalid path");

    int errorCode = rename(oldfullpath.c_str(), newfullpath.c_str());

    if (0 != errorCode)
    {
        LOG_ERROR("Fail to rename file %s to %s !Error code is %d", oldfullpath.c_str(), newfullpath.c_str(), errorCode);
        return false;
    }
    return true;
}

bool FileUtils::renameFile(const std::string &path, const std::string &oldname, const std::string &name)
{
    LOG_ASSERT(!path.empty(), "Invalid path");
    std::string oldPath = path + oldname;
    std::string newPath = path + name;

    return this->renameFile(oldPath, newPath);
}



#endif //(TARGET_PLATFORM == PLATFORM_WIN32) || (TARGET_PLATFORM == PLATFORM_WINRT)


NS_FK_END
