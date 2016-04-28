#if defined( _MSC_VER )
	#if !defined( _CRT_SECURE_NO_WARNINGS )
		#define _CRT_SECURE_NO_WARNINGS		// This test file is not intended to be secure.
	#endif
#endif

#if defined( _MSC_VER )
	#include <direct.h>		// _mkdir
	#include <crtdbg.h>
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#elif defined(MINGW32) || defined(__MINGW32__)
    #include <io.h>  // mkdir
#else
	#include <sys/stat.h>	// mkdir
#endif

#include "tinyxml2.h"
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace tinyxml2;
using namespace std;

vector<string> GetXmlFileNames(string folder)
{
    vector<string> names;
    char search_path[200];
    sprintf(search_path, "%s/*.xml", folder.c_str());
    WIN32_FIND_DATA fd; 
    HANDLE hFind = FindFirstFile(search_path, &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
        do { 
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
    } 
    return names;
}

int main(int argc, char* argv[])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	vector<string> fileList = GetXmlFileNames(argv[1]);
	vector<string>::iterator itr;

	for (itr = fileList.begin(); itr != fileList.end(); itr++) {
		const char* test = (*itr).c_str();
		cout << test << endl;
		XMLDocument* doc = new XMLDocument();
		doc->LoadFile(test);
		doc->Parse(test);
		XMLElement* probeIdElement = doc->FirstChildElement("Probe");
		const char* probeId = probeIdElement->GetText();
		cout << probeId << endl;
	}
	Sleep(1000000);
	return 0;
}