//! C/C++����Windows������� -> �����ı��༭�� -> ��ʾ
#include <windows.h>
#include <winldap.h>
#include <stdio.h>
#include <stdlib.h>

//! ���ؼ���ʹ�õ�ID
#define ID_EDITBOX    1      //< �ı��༭��ؼ�
#define ID_TXTPATH    2      //< ·���༭��ؼ�
#define ID_SAVEBTN    3      //< �����ļ���ť
#define ID_CLSBTN    4       //< ��ձ༭����ť
#define ID_GROUP    5        //< ��Ͽ�

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CreateChildWindow(HWND, HWND*, LPARAM);          //< ������ʹ�õ����Ӵ��ڿؼ�

int SavaInputContent(TCHAR*, TCHAR*);                //< ������������ֵ��ļ�

char* UnicodeToUtf8(const wchar_t* unicode);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("demo");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	wndclass.style            = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc      = WndProc;
	wndclass.hInstance        = hInstance;
	wndclass.cbClsExtra       = 0;
	wndclass.cbWndExtra       = 0;
	wndclass.hbrBackground    = CreateSolidBrush(RGB(236, 233, 216));
	wndclass.hCursor          = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpszClassName    = szAppName;
	wndclass.lpszMenuName     = NULL;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Unable to register the window class!"),
			TEXT("error"), MB_OK | MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,
		TEXT("Simple text editor -> demo"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND    hwndChild[5];  //<�����ڵ��Ӵ���
	HDC            hdc;
	PAINTSTRUCT    ps;
	RECT           rect ;

	static TCHAR   *szBuffer = nullptr;    //< д���ļ��Ļ�����
	static TCHAR   szPath[256] = {0};      //< �ı�·��
	static TCHAR   szLineNum[32] = {0};    //< �����������������������
	static TCHAR   tmp[128] = {0};         //< ����ɹ������Ļ�����
	static int     iLength = 0;
	static int     iFlag = 0;
	static TCHAR   Prompt[32] = {0};
	int            iLineCount = 0;         //< �ı�����
	int            iCharCount = 0;         //< �ı��ַ�����

	switch(message)
	{
	case WM_CREATE:
		CreateChildWindow(hwnd, hwndChild, lParam);
		return 0;

	case WM_SIZE:
		GetClientRect(hwnd, &rect);

		//! �Ը������е��Ӵ������Բ���
		MoveWindow(hwndChild[ID_EDITBOX], 0, 0,
			rect.right, rect.bottom-50, TRUE);  //< �ı��༭��

		MoveWindow(hwndChild[ID_TXTPATH], 60,  rect.bottom-31,
			200, 20, TRUE);                     //< �ı�·�������

		MoveWindow(hwndChild[ID_SAVEBTN], 280, rect.bottom-35,
			50,  25, TRUE);                     //<���水ť

		MoveWindow(hwndChild[ID_CLSBTN ], 400, rect.bottom-35,
			50,  25, TRUE);                     //<��հ�ť

		MoveWindow(hwndChild[ID_GROUP  ], 10,  rect.bottom-50,
			330, 48, TRUE);                     //<��Ͽ�
		return 0;

	case WM_PAINT:
		GetClientRect(hwnd, &rect);

		hdc = BeginPaint(hwnd, &ps);
		if (1 == iFlag)
		{
			iFlag = 0;
			swprintf_s(tmp, 128, "Saved!");
		}
		else
		{
			swprintf_s(tmp, 128, "UnSaved!");
		}

		TextOut(hdc, rect.right - 110,rect.bottom - 30, tmp,lstrlen(tmp));
		TextOut(hdc, 20, rect.bottom - 30, TEXT("Path:"), lstrlen(TEXT("Path:")));
		TextOut(hdc, 500, rect.bottom - 30, szLineNum, lstrlen(szLineNum));
		EndPaint(hwnd, &ps);

		return 0;

	//! ���û��Ӳ˵���ѡ��������ؼ����丸���ڷ���֪ͨ��Ϣ�����ݼ�����ʱ���͡�
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			//! �ı��༭��
		case ID_EDITBOX:
			switch(HIWORD(wParam))
			{
				//! ���༭�ؼ������ػ�����ʱ���͡���֪ͨ�����ڿؼ���ʽ���ı�֮����ʾ�ı�֮ǰ����.
				//!    �˼�����ԭ��������֮���Ҫ��ʾ����������,
				//! ���Ҫͨ���˼�����֤���������ı任��������ı任
			case EN_UPDATE:
				iLineCount = SendMessage(hwndChild[ID_EDITBOX], EM_GETLINECOUNT, 0, 0);
				iCharCount = GetWindowTextLength(hwndChild[ID_EDITBOX]);
				wsprintf(szLineNum, L"LineNum:%d  WordsNum:%d", iLineCount, iCharCount);

				//! ���һ�����ε�ָ�����ڵĸ������򣬱�ʾ�����ػ�þ��β���
				//! ����1���þ��δ��ڵľ��
				//!   2��ָ�����Ҫ��ӵ���������ľ��εĿͻ�������� RECT�ṹ��ָ�롣
				//       ����˲���ΪNULL���������ͻ�����ӵ���������
				//!   3���Ƿ�Ҫ�����������򱳾�(boolֵ)
				//! �ڴ˴������ã����������������仯��ἰʱ��ʾ
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);

				break;
			default:
				break;
			}
			return 0;

//! �����ļ��İ�ť
		case ID_SAVEBTN:
			iLength = GetWindowTextLength(hwndChild[ID_EDITBOX]);
			if( iLength != 0)
			{
				szBuffer =  (TCHAR*)malloc((iLength+1)*sizeof(TCHAR));
				if (szBuffer == nullptr)
				{
					MessageBox(NULL, TEXT("Allocation Failure"),
						TEXT("Prompt"), MB_OK | MB_ICONINFORMATION);
					return -1;
				}
			}
			else
			{
				return -1;
			}

			GetWindowText(hwndChild[ID_EDITBOX], szBuffer, (iLength+1)*sizeof(TCHAR));
			if(GetWindowText(hwndChild[ID_TXTPATH], szPath, 256) < 1)
			{
				MessageBox(NULL, TEXT("The path cannot be null"),
					TEXT("Prompt"), MB_OK | MB_ICONINFORMATION);
				return -1;
			}
			SavaInputContent(szPath, szBuffer);//<�����ļ�

			free(szBuffer);
			szBuffer = nullptr;

			//!��ʾ����ɹ�����ʾ
			iFlag = 1;
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);

			return 0;

		case ID_CLSBTN:
			SetWindowText(hwndChild[ID_EDITBOX], TEXT(""));
			wsprintf(szLineNum, L"LineNum:%d  WordsNum:%d", 1, 0);
			InvalidateRect(hwnd, NULL, FALSE);

			return 0;

		default:
			break;
		}

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

//! �����Ӵ���
int CreateChildWindow(HWND hwnd, HWND *hwndChild, LPARAM lParam)
{
	HINSTANCE hInst = ((LPCREATESTRUCT) lParam) -> hInstance;

	//! �����༭��
	hwndChild[ID_EDITBOX] = CreateWindow(TEXT("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		0, 0, 0, 0,
		hwnd, (HMENU)ID_EDITBOX, hInst, NULL);

	//! ·�������
	hwndChild[ID_TXTPATH] = CreateWindow(TEXT("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL,
		0, 0, 0, 0,
		hwnd, (HMENU)ID_TXTPATH, hInst, NULL);

	//! ���水ť
	hwndChild[ID_SAVEBTN] = CreateWindow(TEXT("button"), TEXT("Save"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
		hwnd, (HMENU)ID_SAVEBTN, hInst, NULL);

	//! ��հ�ť
	hwndChild[ID_CLSBTN] = CreateWindow(TEXT("button"), TEXT("Clear"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
		hwnd, (HMENU)ID_CLSBTN, hInst, NULL);

	//! ��Ͽ�
	hwndChild[ID_GROUP] = CreateWindow(TEXT("button"), NULL,
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, 0, 0, 0,
		hwnd, (HMENU)ID_GROUP, hInst, NULL);

	return 0;
}

//! ��unicodeת��ΪUTF-8
char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1,
		NULL, 0, NULL, NULL) +1;

	char* Ptr = (char*)malloc(len);    //< ת��֮������Ptr��������
	if (Ptr == nullptr)
	{
		MessageBox(NULL, TEXT("Allocation Failure"),
			TEXT("Prompt"), MB_OK | MB_ICONINFORMATION);
		return nullptr;
	}
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, Ptr, len, NULL, NULL);

	return Ptr;
}

//!������������
#if 1
int SavaInputContent(TCHAR* path, TCHAR* content)
{
	FILE *fSave = NULL;
	char* contentPath = nullptr;
	errno_t err = _wfopen_s(&fSave, path, L"a");//<a:׷��д w:ֻд
	if (0 != err || NULL == fSave)
	{
		MessageBox(NULL, TEXT("File create failed!"),
			TEXT("Prompt"), MB_OK | MB_ICONINFORMATION);
		return err;
	}

	contentPath = UnicodeToUtf8(content);
	fwrite(contentPath, 1 ,strlen(contentPath), fSave);
	fclose(fSave);

	//!д��֮���ͷ���Դ
	free(contentPath);
	contentPath = nullptr;

	return 0;
}

#else
int SavaInputContent(TCHAR* path, TCHAR* content)
{
	FILE *fSave = NULL;
	errno_t err = _wfopen_s(&fSave, path, L"a");//<a:׷��д w:ֻд
	if (0 != err || NULL == fSave)
	{
		MessageBox(NULL, TEXT("File create failed!"),
			TEXT("Prompt"), MB_OK | MB_ICONINFORMATION);
		return err;
	}

	const unsigned char uszHead[] = {0xFF, 0xFE};
	fwrite(uszHead, 1, sizeof(uszHead), fSave);
	fwrite(content, 2, wcslen(content), fSave);
	fclose(fSave);
	fSave = NULL;
	return 0;
}
#endif
