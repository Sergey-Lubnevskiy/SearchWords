#define _CRT_SECURE_NO_WARNINGS
#include "Header.h"
Search* Search::ptr = NULL;
static HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

CRITICAL_SECTION cs;
wstring path_to_copy;
vector<int> words_count;
Search::Search()
{
	ptr = this;
}

BOOL CALLBACK Search::DlgProc(HWND hwnd, UINT mes, WPARAM wParam, LPARAM lParam)
{
	switch (mes)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Close);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->InitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Command);
	case WM_CTLCOLORLISTBOX:
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(120, 0, 0));
		return (INT_PTR)hBrush;

	}
	return FALSE;
}

BOOL Search::InitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hSearch = GetDlgItem(hwnd, IDC_EDIT1);
	hBSearch = GetDlgItem(hwnd, IDC_Search);
	hReport = GetDlgItem(hwnd, IDC_LIST3);
	hWords = GetDlgItem(hwnd, IDC_LIST1);
	hFile = GetDlgItem(hwnd, IDC_LIST2);
	hDialog = hwnd;
	
	
	TCHAR GUID[] = TEXT("text");
	hMutex = CreateMutex(NULL, FALSE, GUID);
	DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
	if (dwAnswer == WAIT_TIMEOUT)
	{
		MessageBox(hwnd, TEXT("Нельзя запускать более одной копии приложения!!!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
		EndDialog(hwnd, 0);
	}
	return TRUE;
}


void Search::OnBrowse()
{
	BROWSEINFO binfo = { 0 };
	binfo.hwndOwner = hDialog;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_BROWSEINCLUDEFILES;
	binfo.lpszTitle = L"Выбор файла или папки...";
	LPITEMIDLIST ptr = SHBrowseForFolder(&binfo);
	if (ptr)
	{
		TCHAR path[MAX_PATH];
		SHGetPathFromIDList(ptr, path);
		if (_tcslen(path))
			SetWindowText(hSearch, path);
	}
}

void CreateCopyFile(string result, wstring file_name)
{
	int tmp = file_name.find(L".TXT");
	wstring name = file_name.substr(0, tmp) + L"Copy" + L".TXT";
	ofstream f_write;
	f_write.open(path_to_copy + L"\\" + name);
	if (f_write.is_open())
	{
		f_write << result;
	}
	f_write.close();
}

void Search::Command(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	wstring words;
	list<wstring> tmp;
	switch (id)
	{
		
	case IDC_Search:
	{
		ptr->OnBrowse();

		
		TCHAR buffs[100];
		GetWindowText(hSearch, buffs, 100);
		SendMessage(hFile, LB_RESETCONTENT, 0, 0);
		WIN32_FIND_DATAW wfd;
		TCHAR mask[10] = TEXT("\\*.txt*");
		_tcscat(buffs, mask);
		HANDLE const hFind = FindFirstFileW(buffs, &wfd);
		setlocale(LC_ALL, "");
		if (INVALID_HANDLE_VALUE != hFind)
		{
			do
			{
				SendMessage(hReport, LB_ADDSTRING, 0, LPARAM(&wfd.cFileName[0]));
			} while (NULL != FindNextFileW(hFind, &wfd));

			FindClose(hFind);
		}


		
		TCHAR buff[20];
		LPCWSTR a = L"tmp"; 
		CreateDirectory(a, NULL);
		int count = 0;
		int num = 0;
		wstring str;
		wstring result;
		wstring tmp;
		int A;
		SendMessage(hWords, LB_RESETCONTENT, 0, 0);
		wifstream in(TEXT("C:\\Users\\fanda\\source\\repos\\Exam 2.0\\Text\\Text.txt"));
		if (in.is_open())
		{
			while (!in.eof())
			{
				in >> str;
			}
			for (int i = 0; i < str.size(); i++)
			{
				if ((num = tmp.find(words[i])) != string::npos)
				{
					count++;
					result += tmp.substr(0, num);
					
					words_count[i]++;
					continue;
				}
			}
			in.close();

		}
		break;
	}
	case IDOK:
	{
		TCHAR buff[20];
		SendMessage(hWords, LB_RESETCONTENT, 0, 0);
		wifstream in(TEXT("W.txt"));
		if (in.is_open())
		{
			while (!in.eof())
			{
			in >> buff;
			in >> words;
			tmp.push_back(words);
			SendMessage(hWords, LB_ADDSTRING, 0, LPARAM(buff));
			}
			in.close();

		}
	}
		
	}
}

void Search::Close(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

Search::~Search(void)
{
	ReleaseMutex(hMutex);
	DeleteCriticalSection(&cs);
}

void MessageAboutError(DWORD dwError)
{
	LPVOID lpMsgBuf = NULL;
	TCHAR szBuf[300];

	BOOL fOK = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	if (lpMsgBuf != NULL)
	{
		wsprintf(szBuf, TEXT("Ошибка %d: %s"), dwError, lpMsgBuf);
		MessageBox(0, szBuf, TEXT("Сообщение об ошибке"), MB_OK | MB_ICONSTOP);
		LocalFree(lpMsgBuf);
	}
}
