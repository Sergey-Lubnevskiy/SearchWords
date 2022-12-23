#pragma once

#include<iostream>
#include<Windows.h>
#include<tchar.h>
#include <windowsX.h>
#include "resource.h"
#include <time.h>
#include<fstream>
#include<string>
#include<shlobj.h>
#include<string>
#include<vector>
#include<list>

using namespace std;

class Search
{
public:
	const int size = 10;
	int arr[10];
	Search();
	HWND hSearch, hReport, hBSearch, hWords, hDialog, hFile;
	HANDLE hMutex;
public:
	void CreateCopyFile(string result, wstring file_name);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Search* ptr;
	BOOL InitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void OnBrowse();
	void Command(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Close(HWND hwnd);
	~Search(void);
};