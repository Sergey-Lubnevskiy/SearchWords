#include"Header.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Search dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Search::DlgProc);
}

