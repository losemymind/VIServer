/****************************************************************************
Copyright (c) 2015 libo.

losemymind.libo@gmail.com

****************************************************************************/
#include "FoundationKit/GenericPlatformMacros.h"
#if (TARGET_PLATFORM == PLATFORM_WIN32)

#include <codecvt>
#include <regex>
#include <Shlobj.h>
#include <cstdlib>
#include <Windows.h>
#include "FoundationKit/Foundation/StringUtils.h"
#include "FoundationKit/Foundation/Logger.h"
#include "FoundationKit/Platform/FileUtils.h"

NS_FK_BEGIN

void FileUtils::initRootPath()
{
    if (0 == _resourceRootPath.length())
    {
        WCHAR *pUtf16ExePath = nullptr;

        // If app character set is Unicode, the program entry use wmain, 
        // else use main, change _get_wpgmptr to _get_pgmptr
        _get_wpgmptr(&pUtf16ExePath);
        // We need only directory part without exe
        WCHAR *pUtf16DirEnd = wcsrchr(pUtf16ExePath, L'\\');
        char utf8ExeDir[MAX_PATH] = { 0 };
        WideCharToMultiByte(CP_ACP, 0, pUtf16ExePath, pUtf16DirEnd - pUtf16ExePath + 1, utf8ExeDir, sizeof(utf8ExeDir), nullptr, nullptr);
        _resourceRootPath = convertPathFormatToUnixStyle(utf8ExeDir);
    }
}

bool FileUtils::isFileExist(const std::string& filename) const
{
    if (filename.empty())
        return false;

    std::string fullPath = filename;
    if (!isAbsolutePath(fullPath))
    {
        fullPath.insert(0, _resourceRootPath);
    }

    DWORD attr = GetFileAttributesW(StringUtils::string2UTF8wstring(fullPath).c_str());
    if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
        return false;
    return true;
}

bool FileUtils::moveFile(const std::string &oldfullpath, const std::string &newfullpath)const
{
    return MoveFileA(oldfullpath.c_str(), newfullpath.c_str()) != 0;
}

bool FileUtils::createDirectory(const std::string& dirPath)
{
    if (isDirectoryExist(dirPath))
        return true;

    std::wstring path = StringUtils::string2UTF8wstring(dirPath);

    // Split the path
    size_t start = 0;
    size_t found = path.find_first_of(L"/\\", start);
    std::wstring subpath;
    std::vector<std::wstring> dirs;

    if (found != std::wstring::npos)
    {
        while (true)
        {
            subpath = path.substr(start, found - start + 1);
            if (!subpath.empty())
                dirs.push_back(subpath);
            start = found + 1;
            found = path.find_first_of(L"/\\", start);
            if (found == std::wstring::npos)
            {
                if (start < path.length())
                {
                    dirs.push_back(path.substr(start));
                }
                break;
            }
        }
    }

    if ((GetFileAttributes(path.c_str())) == INVALID_FILE_ATTRIBUTES)
    {
        subpath = L"";
        for (unsigned int i = 0; i < dirs.size(); ++i)
        {
            subpath += dirs[i];

            std::string utf8Path = StringUtils::wstring2UTF8string(subpath);
            if (!isDirectoryExist(utf8Path))
            {
                BOOL ret = CreateDirectory(subpath.c_str(), NULL);
                if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
                {
                    LOG_ERROR("Fail create directory %s !Error code is 0x%x", utf8Path.c_str(), GetLastError());
                    return false;
                }
            }
        }
    }
    return true;
}

bool FileUtils::removeDirectory(const std::string& dirPath)
{
    std::wstring wpath = StringUtils::string2UTF8wstring(dirPath);
    std::wstring files = wpath + L"*.*";
    WIN32_FIND_DATA wfd;
    HANDLE  search = FindFirstFileEx(files.c_str(), FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
    bool ret = true;
    if (search != INVALID_HANDLE_VALUE)
    {
        BOOL find = true;
        while (find)
        {
            //. ..
            if (wfd.cFileName[0] != '.')
            {
                std::wstring temp = wpath + wfd.cFileName;
                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    temp += '/';
                    ret = ret && this->removeDirectory(StringUtils::wstring2UTF8string(temp));
                }
                else
                {
                    SetFileAttributes(temp.c_str(), FILE_ATTRIBUTE_NORMAL);
                    ret = ret && DeleteFile(temp.c_str());
                }
            }
            find = FindNextFile(search, &wfd);
        }
        FindClose(search);
    }
    if (ret && RemoveDirectory(wpath.c_str()))
    {
        return true;
    }
    return false;
}

bool FileUtils::removeFile(const std::string &filepath)
{
    std::regex pat("\\/");
    std::string win32path = std::regex_replace(filepath, pat, "\\");
    if (DeleteFile(StringUtils::string2UTF8wstring(win32path).c_str()))
    {
        return true;
    }
    else
    {
        LOG_ERROR("Fail remove file %s !Error code is 0x%x", filepath.c_str(), GetLastError());
        return false;
    }
}

bool FileUtils::renameFile(const std::string &path, const std::string &oldname, const std::string &name)
{
    std::string oldPath = path + oldname;
    std::string newPath = path + name;

    std::regex pat("\\/");
    std::string _old = std::regex_replace(oldPath, pat, "\\");
    std::string _new = std::regex_replace(newPath, pat, "\\");

    return renameFile(_old, _new);
}

bool FileUtils::renameFile(const std::string &oldfullpath, const std::string &newfullpath)
{
    if (FileUtils::getInstance()->isFileExist(newfullpath))
    {
        if (!DeleteFileA(newfullpath.c_str()))
        {
            LOG_ERROR("Fail to delete file %s !Error code is 0x%x", newfullpath.c_str(), GetLastError());
        }
    }

    if (MoveFileA(oldfullpath.c_str(), newfullpath.c_str()))
    {
        return true;
    }
    else
    {
        LOG_ERROR("Fail to rename file %s to %s !Error code is 0x%x", oldfullpath.c_str(), newfullpath.c_str(), GetLastError());
        return false;
    }
}


std::string FileUtils::getWritablePath() const
{
    if (_writablePath.length())
    {
        return _writablePath;
    }

    // Get full path of executable, e.g. c:\Program Files (x86)\My Game Folder\MyGame.exe
    WCHAR full_path[MAX_PATH + 1] = { 0 };
    ::GetModuleFileName(nullptr, full_path, MAX_PATH + 1);

    // Debug app uses executable directory; Non-debug app uses local app data directory
    //#ifndef _DEBUG
    // Get filename of executable only, e.g. MyGame.exe
    WCHAR *base_name = wcsrchr(full_path, '\\');
    std::wstring retPath;
    if (base_name)
    {
        WCHAR app_data_path[MAX_PATH + 1];

        // Get local app data directory, e.g. C:\Documents and Settings\username\Local Settings\Application Data
        if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, app_data_path)))
        {
            std::wstring ret(app_data_path);

            // Adding executable filename, e.g. C:\Documents and Settings\username\Local Settings\Application Data\MyGame.exe
            ret += base_name;

            // Remove ".exe" extension, e.g. C:\Documents and Settings\username\Local Settings\Application Data\MyGame
            ret = ret.substr(0, ret.rfind(L"."));

            ret += L"\\";

            // Create directory
            if (SUCCEEDED(SHCreateDirectoryEx(nullptr, ret.c_str(), nullptr)))
            {
                retPath = ret;
            }
        }
    }
    if (retPath.empty())
        //#endif // not defined _DEBUG
    {
        // If fetching of local app data directory fails, use the executable one
        retPath = full_path;

        // remove xxx.exe
        retPath = retPath.substr(0, retPath.rfind(L"\\") + 1);
    }

    return convertPathFormatToUnixStyle(StringUtils::wstring2UTF8string(retPath));
}


NS_FK_END

#endif


