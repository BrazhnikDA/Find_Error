#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>
#include <omp.h>
#include "Log.h"

#define SIZE_MASSIVE 100

#define PathFolderFind     L"Z:\\Bad\\"
#define PathForFolderCopy  L"Z:\\Archive\\"

//#define PathFolderFind      L"D:\\1\\ТЕСТ_2\\"
//#define PathForFolderCopy   L"D:\\1\\ТЕСТ_2_КОПИ\\"

#define KEY_FIND_ERROR_80   L"Неизвестный тег записи: 80"
#define KEY_FIND_ERROR_86   L"Неизвестный тег записи: 86"

std::wstring* FindFolder(std::wstring NameFolder[]);
std::wstring* FindFilesInFolder(std::wstring NamesFiles[], std::wstring NameFolder);
std::wstring  CheckFirstLine(std::wstring AllPath);
int CountFolder();


// Записать весь вывод в 1 переменную и вывести в файл
int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE); 
    std::locale system("");
    std::locale::global(system);

    Log log;
    std::wstring output;
    int countFolder = CountFolder();

    output = L"[" + log.GetDate() + L", " + log.GetTime() + L"]" + L" Start work, ";

    std::wstring* NameFolder = new std::wstring[CountFolder() + 2];
    std::wstring  NameFiles[SIZE_MASSIVE];

    std::wstring PathTo;      // Откуда
    std::wstring PathFrom;    // Куда

    FindFolder(NameFolder);

    output += L"Selected folder \"" + NameFolder[countFolder - 1] + L"\", ";
    
    FindFilesInFolder(NameFiles, NameFolder[countFolder - 1]);

    if (NameFiles[0] == L"") { output += L"Folder is empty, "; }

    bool FileIsFound = false;
    int bad_files = 0;
    for (int j = 0; j < NameFiles[j].size(); j++)
    {
        std::wstring FLine = CheckFirstLine(PathFolderFind + NameFolder[countFolder - 1] + L"\\" + NameFiles[j]);
        if (FLine == KEY_FIND_ERROR_80 || FLine == KEY_FIND_ERROR_86)
        {
            std::wstring tmpPath = PathFolderFind + NameFolder[countFolder - 1] + L"\\" + NameFiles[j];
            std::wstring tmpPathCopy = PathForFolderCopy + NameFiles[j];
            if(!(MoveFile(tmpPath.c_str(), tmpPathCopy.c_str())))   // Откуда -> Куда
            {
                bad_files++;
            }
            FileIsFound = true;
        }
        NameFiles[j] = L"";
    }
    
    if (!FileIsFound) { output += L"Correct files not found!"; }
    else
    {
        if (bad_files == 0)
        {
            output += L"Files move";
        }
        else { output += L"Files with this name have already been copied (" + std::to_wstring(bad_files) + L"x)"; }
    }

    log.OutputFile(output + L"\n",false);

    return 0;
}

int CountFolder()
{
    int res = 0;

    WIN32_FIND_DATAW wfd;
    HANDLE const hFind = FindFirstFileW(PathFolderFind"*", &wfd);
    int i = 0;
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            std::wstring tmpNameFile = wfd.cFileName;

            if (!(tmpNameFile.find('.') != std::wstring::npos))
            {
                res++;
            }
        } while (NULL != FindNextFileW(hFind, &wfd));

        FindClose(hFind);
    }

    return res;
}

std::wstring* FindFolder(std::wstring NameFolder[])
{
    // Для просмотра файлов в директории
    WIN32_FIND_DATAW wfd;
    // Где искать и что  (* - все расширения, *.png - все png)
    HANDLE const hFind = FindFirstFileW(PathFolderFind"*", &wfd);

    // Поиск файлов и папок и вывод
    int i = 0;
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            std::wstring tmpNameFile = wfd.cFileName;

            if (!(tmpNameFile.find('.') != std::wstring::npos))
            {
                NameFolder[i] = wfd.cFileName;             
                i++;
            }
        } while (NULL != FindNextFileW(hFind, &wfd));

        FindClose(hFind);
    }
    NameFolder[i + 1] = L"\0";

    return NameFolder;
}

std::wstring* FindFilesInFolder(std::wstring NamesFiles[], std::wstring NameFolder)
{
    // Для просмотра файлов в директории
    WIN32_FIND_DATAW wfd;
    // Где искать и что  (* - все расширения, *.png - все png)
    std::wstring allPath = PathFolderFind + NameFolder + L"\\*.err";
    HANDLE const hFind = FindFirstFileW(allPath.c_str(), &wfd);         //(L"D:\\1\\ТЕСТ_2\\2021-03-05\\*.err", &wfd);

    // Поиск файлов и папок и вывод
    int i = 0;
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            NamesFiles[i] = wfd.cFileName;
            i++;
        } while (NULL != FindNextFileW(hFind, &wfd));

        FindClose(hFind);
    }
    NamesFiles[i + 1] = L"\0";

    return NamesFiles;
}

std::wstring CheckFirstLine(std::wstring AllPath)
{
    std::wifstream fs(AllPath);
    if (fs)
    {
        std::wstring FLine;
        getline(fs, FLine);
        fs.close();
        return FLine;
    }
    fs.close();
    return L"";
}