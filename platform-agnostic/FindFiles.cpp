


#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <memory>

#ifndef WIN32
#include <fts.h>
#include <dirent.h>
#endif

#include "WindowsDef.hpp"


LPVOID STDMETHODCALLTYPE MyAllocate(SIZE_T cb)  { return std::malloc(cb); }


#ifdef WIN32
std::vector<std::string> GetAllFilesInDirectory(const std::string& directory)
{
    static std::wstring dot(L".");
    static std::wstring dotdot(L"..");

    std::vector<std::string> files;
    std::queue<std::wstring> directories;
    directories.push(utf8_to_utf16(directory));

    do
    {
        std::wstring root = directories.front();
        std::wstring directory = root + L"\\*";
        directories.pop();

        WIN32_FIND_DATA findFileData = {};
        std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(&::FindClose)> find(
            FindFirstFile(reinterpret_cast<LPCWSTR>(directory.c_str()), &findFileData),
            &FindClose);
        
        if (INVALID_HANDLE_VALUE != find.get())
        {
            do
            {
                std::wstring utf16Name = std::wstring(findFileData.cFileName);
                std::wstring child = root + L"\\" + utf16Name;
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (dot != utf16Name && dotdot != utf16Name)
                    {
                        directories.push(child);
                    }
                }
                else
                {
                    files.push_back(utf16_to_utf8(child));
                }
            } while(FindNextFile(find.get(), &findFileData));
        }

    } while (!directories.empty());
    return files;
}
#else
std::vector<std::string> GetAllFilesInDirectory(const std::string& directory)
{
    std::vector<std::string> files;
    std::queue<std::string> directories;
    directories.push(directory);

    static std::string dot(".");
    static std::string dotdot("..");
    do
    {
        auto root = directories.front();
        directories.pop();
        std::unique_ptr<DIR, decltype(&closedir)> dir(opendir(root.c_str()), closedir);
        if (dir.get() != nullptr)
        {
            struct dirent* dp;
            while((dp = readdir(dir.get())) != nullptr)
            {
                std::string fileName = std::string(dp->d_name);
                std::string child = root + "/" + fileName;
                if (dp->d_type == DT_DIR)
                {
                    if ((fileName != dot) && (fileName != dotdot))
                    {
                        directories.push(child);
                    }
                }
                else
                {
                    files.push_back(child);
                }
            }
        }
    } while (!directories.empty());
    return files;
}
#endif


int main(int argc, char* argv[])
{
	std::cout << "Hello World" << std::endl;
	std::vector<std::string> file_list = GetAllFilesInDirectory(".");
	for(std::string fname : file_list)
	{
		std::cout << fname << std::endl;
	}
}