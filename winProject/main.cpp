
#include <windows.h>
#include "resource.h"

#define ID_EDITCHILD 100
const wchar_t g_szClassName[] = L"myWindowClass";
int width=640,height=480;
HFONT hFont=CreateFont(18,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,EASTEUROPE_CHARSET,OUT_OUTLINE_PRECIS,
             CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY, VARIABLE_PITCH,TEXT("Courier New"));
WNDCLASSEX wc;
HWND hwnd;
MSG Msg;
HWND hwndEdit;
LPWSTR currentFile=L"";

void LoadFile(LPCWSTR file)
{
	currentFile=(LPWSTR)GlobalAlloc(GPTR, lstrlen(file));
	lstrcpy(currentFile,file);
	SetWindowText(hwnd, currentFile);
	HANDLE hFile;
	DWORD dwSize;
	DWORD dw;


	hFile = CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	dwSize = GetFileSize(hFile, NULL);
	LPWSTR pszText;
	pszText = (LPWSTR) GlobalAlloc(GPTR, dwSize);
	ReadFile(hFile,pszText,dwSize,&dw,NULL);
	CloseHandle(hFile);
	pszText[dwSize]='\0';
	
	SetWindowText(hwndEdit, pszText);
	GlobalFree(pszText);
}

void SaveFile(LPCWSTR file)
{
	currentFile=(LPWSTR)GlobalAlloc(GPTR, lstrlen(file));
	lstrcpy(currentFile,file);
	HANDLE hFile;
	SetWindowText(hwnd, currentFile);
	hFile = CreateFile(file, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwTextLength = GetWindowTextLength(hwndEdit);
	LPWSTR pszText;
	DWORD dwBufferSize = dwTextLength + 1;
	pszText = (LPWSTR) GlobalAlloc(GPTR, dwBufferSize);
	GetWindowText(hwndEdit, pszText, dwBufferSize);
	DWORD dwWritten;

	WriteFile(hFile, pszText, sizeof(WCHAR)*wcslen(pszText), &dwWritten, NULL);
	GlobalFree(pszText);
	CloseHandle(hFile);
}

void OpenDialog(HWND hwnd)
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile =sizeof(szFile);
	ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
		LoadFile(ofn.lpstrFile);
}

void SaveDialog(HWND hwnd)
{
	TCHAR szFile[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile =sizeof(szFile);
	ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn))
		SaveFile(ofn.lpstrFile);
}

void NewFile()
{
	currentFile=L"";
	SetWindowText(hwndEdit, L"");
}
void SetFont()
{
	CHOOSEFONT chfnt;
	LOGFONT lf;
	chfnt.lStructSize=sizeof(CHOOSEFONT);
	chfnt.hwndOwner=hwnd;
	chfnt.lpLogFont=&lf;
	chfnt.Flags=CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT;
	if(ChooseFont(&chfnt))
	{
		hFont=CreateFontIndirect(chfnt.lpLogFont);
		SendMessage(hwndEdit,WM_SETFONT,(WPARAM) hFont, MAKELPARAM(TRUE,0));

	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
		case WM_CREATE:
            hwndEdit = CreateWindowEx(
                                0, TEXT("EDIT"), 
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
                                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
                                0, 0, LOWORD(lParam), HIWORD(lParam), 
                                hwnd,
                                (HMENU) ID_EDITCHILD, 
                                (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), 
                                NULL); 

			 SendMessage(hwndEdit,
                    WM_SETFONT,
                    (WPARAM) hFont,
                    MAKELPARAM(TRUE, 0)
                    ); 
 
            return 0; 

			
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case ID_ACCELERATOR40014:
				SendMessage(hwndEdit, EM_SETSEL, 0, -1);
				return 0;
			case ID_HELP_ABOUT: 
				MessageBox(hwnd, TEXT("Notepad 16 12 2015"), TEXT("Notepad"), MB_OK | MB_ICONINFORMATION);
				return 0;
			case ID_FILE_EXIT:
				PostQuitMessage(0);
				return 0;
			case ID_FILE_OPEN40006:
				OpenDialog(hwnd);
				return 0;
			case ID_FILE_SAVE40003:
				if(lstrcmp(currentFile,L"")) SaveFile(currentFile);
				else SaveDialog(hwnd);
				return 0;
			case ID_FILE_SAVEAS:
				SaveDialog(hwnd);
				return 0;
			case ID_FILE_NEW40001:
				NewFile();
				return 0;
			case ID_FORMAT_FONT40007:
				SetFont();
				return 0;
			case ID_EDIT_COPY40008:
				SendMessage(hwndEdit, WM_COPY, 0, 0);
				return 0;
			case ID_EDIT_PASTE40009:
				SendMessage(hwndEdit, WM_PASTE, 0, 0);
				return 0;
			case ID_EDIT_CUT40011:
				SendMessage(hwndEdit, WM_CUT, 0, 0);
				return 0;
			case ID_EDIT_DELETE40012:
				SendMessage(hwndEdit, WM_CLEAR, 0, 0);
				return 0;
			case ID_EDIT_SELECTALL:
				SendMessage(hwndEdit, EM_SETSEL, 0, -1);
				return 0;
			}
			return 0;
		case WM_SETFOCUS:
			SetFocus(hwndEdit);
			return 0;
		case WM_SIZE:
			MoveWindow(hwndEdit,0,0,LOWORD(lParam), HIWORD(lParam), TRUE);
			return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	HACCEL hAccel;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm		 = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
    wc.lpszClassName = g_szClassName;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"),
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        TEXT("Notepad"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"),
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
	hAccel=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
		if (! TranslateAccelerator(hwnd, hAccel, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
    }
    return Msg.wParam;
}