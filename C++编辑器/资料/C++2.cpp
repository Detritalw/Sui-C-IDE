//! C/C++语言Windows程序设计 -> 简易文本编辑器 -> 演示
#include <windows.h>
#include <winldap.h>
#include <stdio.h>
#include <stdlib.h>

//! 各控件所使用的ID
#define ID_EDITBOX    1      //< 文本编辑框控件
#define ID_TXTPATH    2      //< 路径编辑框控件
#define ID_SAVEBTN    3      //< 保存文件按钮
#define ID_CLSBTN    4       //< 清空编辑区按钮
#define ID_GROUP    5        //< 组合框

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CreateChildWindow(HWND, HWND*, LPARAM);          //< 创建将使用到的子窗口控件

int SavaInputContent(TCHAR*, TCHAR*);                //< 保存输入的文字到文件

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
	static HWND    hwndChild[5];  //<主窗口的子窗口
	HDC            hdc;
	PAINTSTRUCT    ps;
	RECT           rect ;

	static TCHAR   *szBuffer = nullptr;    //< 写入文件的缓冲区
	static TCHAR   szPath[256] = {0};      //< 文本路径
	static TCHAR   szLineNum[32] = {0};    //< 行数与字数个数输出缓冲区
	static TCHAR   tmp[128] = {0};         //< 保存成功字样的缓冲区
	static int     iLength = 0;
	static int     iFlag = 0;
	static TCHAR   Prompt[32] = {0};
	int            iLineCount = 0;         //< 文本行数
	int            iCharCount = 0;         //< 文本字符个数

	switch(message)
	{
	case WM_CREATE:
		CreateChildWindow(hwnd, hwndChild, lParam);
		return 0;

	case WM_SIZE:
		GetClientRect(hwnd, &rect);

		//! 对父窗口中的子窗口予以布置
		MoveWindow(hwndChild[ID_EDITBOX], 0, 0,
			rect.right, rect.bottom-50, TRUE);  //< 文本编辑区

		MoveWindow(hwndChild[ID_TXTPATH], 60,  rect.bottom-31,
			200, 20, TRUE);                     //< 文本路径输入框

		MoveWindow(hwndChild[ID_SAVEBTN], 280, rect.bottom-35,
			50,  25, TRUE);                     //<保存按钮

		MoveWindow(hwndChild[ID_CLSBTN ], 400, rect.bottom-35,
			50,  25, TRUE);                     //<清空按钮

		MoveWindow(hwndChild[ID_GROUP  ], 10,  rect.bottom-50,
			330, 48, TRUE);                     //<组合框
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

	//! 当用户从菜单中选择命令项、控件向其父窗口发送通知消息或翻译快捷键击键时发送。
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			//! 文本编辑框
		case ID_EDITBOX:
			switch(HIWORD(wParam))
			{
				//! 当编辑控件即将重绘自身时发送。此通知代码在控件格式化文本之后、显示文本之前发送.
				//!    此键存在原因：在输入之后就要显示字数和行数,
				//! 因此要通过此键来保证字数行数的变换跟上输入的变换
			case EN_UPDATE:
				iLineCount = SendMessage(hwndChild[ID_EDITBOX], EM_GETLINECOUNT, 0, 0);
				iCharCount = GetWindowTextLength(hwndChild[ID_EDITBOX]);
				wsprintf(szLineNum, L"LineNum:%d  WordsNum:%d", iLineCount, iCharCount);

				//! 添加一个矩形到指定窗口的更新区域，表示必须重绘该矩形部分
				//! 参数1：该矩形窗口的句柄
				//!   2：指向包含要添加到更新区域的矩形的客户端坐标的 RECT结构的指针。
				//       如果此参数为NULL，则将整个客户区添加到更新区域。
				//!   3：是否要擦除更新区域背景(bool值)
				//! 在此处的作用：保持字数和行数变化后会及时显示
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);

				break;
			default:
				break;
			}
			return 0;

//! 保存文件的按钮
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
			SavaInputContent(szPath, szBuffer);//<保存文件

			free(szBuffer);
			szBuffer = nullptr;

			//!显示保存成功的提示
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

//! 创建子窗口
int CreateChildWindow(HWND hwnd, HWND *hwndChild, LPARAM lParam)
{
	HINSTANCE hInst = ((LPCREATESTRUCT) lParam) -> hInstance;

	//! 创建编辑区
	hwndChild[ID_EDITBOX] = CreateWindow(TEXT("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		0, 0, 0, 0,
		hwnd, (HMENU)ID_EDITBOX, hInst, NULL);

	//! 路径输入框
	hwndChild[ID_TXTPATH] = CreateWindow(TEXT("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL,
		0, 0, 0, 0,
		hwnd, (HMENU)ID_TXTPATH, hInst, NULL);

	//! 保存按钮
	hwndChild[ID_SAVEBTN] = CreateWindow(TEXT("button"), TEXT("Save"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
		hwnd, (HMENU)ID_SAVEBTN, hInst, NULL);

	//! 清空按钮
	hwndChild[ID_CLSBTN] = CreateWindow(TEXT("button"), TEXT("Clear"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
		hwnd, (HMENU)ID_CLSBTN, hInst, NULL);

	//! 组合框
	hwndChild[ID_GROUP] = CreateWindow(TEXT("button"), NULL,
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, 0, 0, 0,
		hwnd, (HMENU)ID_GROUP, hInst, NULL);

	return 0;
}

//! 将unicode转换为UTF-8
char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1,
		NULL, 0, NULL, NULL) +1;

	char* Ptr = (char*)malloc(len);    //< 转换之后存放在Ptr缓冲区中
	if (Ptr == nullptr)
	{
		MessageBox(NULL, TEXT("Allocation Failure"),
			TEXT("Prompt"), MB_OK | MB_ICONINFORMATION);
		return nullptr;
	}
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, Ptr, len, NULL, NULL);

	return Ptr;
}

//!保存输入内容
#if 1
int SavaInputContent(TCHAR* path, TCHAR* content)
{
	FILE *fSave = NULL;
	char* contentPath = nullptr;
	errno_t err = _wfopen_s(&fSave, path, L"a");//<a:追加写 w:只写
	if (0 != err || NULL == fSave)
	{
		MessageBox(NULL, TEXT("File create failed!"),
			TEXT("Prompt"), MB_OK | MB_ICONINFORMATION);
		return err;
	}

	contentPath = UnicodeToUtf8(content);
	fwrite(contentPath, 1 ,strlen(contentPath), fSave);
	fclose(fSave);

	//!写完之后释放资源
	free(contentPath);
	contentPath = nullptr;

	return 0;
}

#else
int SavaInputContent(TCHAR* path, TCHAR* content)
{
	FILE *fSave = NULL;
	errno_t err = _wfopen_s(&fSave, path, L"a");//<a:追加写 w:只写
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
