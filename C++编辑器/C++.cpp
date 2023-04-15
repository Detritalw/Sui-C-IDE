#include<bits/stdc++.h>
#include<windows.h>
#define Button_Open_ID 1
#define Button_Save_ID 2
#define Button_Run_ID 3
#define Edit_Path_ID 6
#define Edit_Code_ID 7
using namespace std;
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
HINSTANCE hInstance;
HINSTANCE hPrevInstance;LPSTR lpCmdLine;
int nCmdShow,boxreturn,Tab,hang=1;
string frename,narm,ml,Temp[1005],newfrename;
class Widget{
	public:
		HWND hwnd = NULL;
		float x = 0;
		float y = 0;
		float width = 0;
		float height = 0;

	public:
		void setText(string text){
			SetWindowText(hwnd, text.data());
		}
		string getText()
		{
			int length = GetWindowTextLength(hwnd) + 1;
			char* text = new char[length];
			GetWindowText(hwnd, text, length);
			return string(text);
		}
		void setSize(int w, int h)
		{
			int nx = x * float(w);
			int ny = y * float(h);
			int nw = width * float(w);
			int nh = height * float(h);
			MoveWindow(hwnd, nx, ny, nw,nh, false);
		}
};

HWND mainWindow = NULL;
Widget Button_Open;
Widget Button_Save;
Widget Button_Run;
Widget Edit_Path;
Widget Edit_Code;

string filepath = "";

HWND initWindow(HINSTANCE instance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void onCreate(HWND hwnd, CREATESTRUCT* cs);
void onDestroy(HWND hwnd);
void onCommand(HWND hwnd, int id, int event, HWND widget);
void onWindowPosChanging(HWND hwnd);

string getFileText();
void saveToFile();
void runCode();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdLine, int cmdShow){
	mainWindow = initWindow(hInstance);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

HWND initWindow(HINSTANCE instance)
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = instance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "WindowClass";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wc);
	return CreateWindowEx(WS_EX_CLIENTEDGE, "WindowClass", "CppComplier", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, instance, NULL);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CREATE:
			onCreate(hwnd, (LPCREATESTRUCT)lp);
			break;
		case WM_DESTROY:
			onDestroy(hwnd);
			break;
		case WM_COMMAND:
			onCommand(hwnd, LOWORD(wp), HIWORD(wp), (HWND)lp);
			break;
		case WM_WINDOWPOSCHANGING:
			onWindowPosChanging(hwnd);
			break;
		case WM_WINDOWPOSCHANGED:
			onWindowPosChanging(hwnd);
			break;
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
			break;
	}
	return 0;
}

void onCreate(HWND hwnd, CREATESTRUCT* cs){
	CreateWindowEx(WS_EX_WINDOWEDGE, "button", "���ļ�", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER | BS_VCENTER,
	 0, 25, 75, 25, hwnd, (HMENU)Button_Open_ID, cs->hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE, "button", "�����ļ�", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER | BS_VCENTER,
	 75, 25, 75, 25, hwnd, (HMENU)Button_Save_ID, cs->hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE, "button", "����", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER | BS_VCENTER,
	 150, 25, 75, 25, hwnd, (HMENU)Button_Run_ID, cs->hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE, "button", "����", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER | BS_VCENTER,
	 225, 25, 75, 25, hwnd, (HMENU)Button_Run_ID, cs->hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE, "button", "����������", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER | BS_VCENTER,
	 300, 25, 75, 25, hwnd, (HMENU)Button_Run_ID, cs->hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE, "edit", "�����ļ���...(����׺��,����·��)", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_MULTILINE,
	 0, 0, 500, 25, hwnd, (HMENU)Edit_Path_ID, cs->hInstance, NULL);
	CreateWindowEx(WS_EX_WINDOWEDGE, "edit", "", WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_MULTILINE|ES_WANTRETURN,
	 0, 50, 620, 400, hwnd, (HMENU)Edit_Code_ID, cs->hInstance, NULL);
	 
}

void onDestroy(HWND hwnd){
	PostQuitMessage(0);
}

void onCommand(HWND hwnd, int id, int event, HWND widget){
	switch(id){
		case Button_Open_ID:{
			Edit_Code.setText(getFileText());
			break;
		}
		case Button_Save_ID:{
			saveToFile();
			break;
		}
		case 4:{
			runCode();
			break;
		}
		case 5:{
			runCode();
			break;
		}
	}
}

void onWindowPosChanging(HWND hwnd){
	RECT rect;
	GetWindowRect(hwnd, &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
}

string getFileText(){
	stringstream text;
	filepath = Edit_Path.getText();
	if (filepath.length() > 0){
		ifstream fin;
		fin.open(filepath);
		string temp;
		while (getline(fin, temp)){
			text << temp << "\r\n";
		}
		fin.close();
	}
	return text.str();
}

void saveToFile(){
	filepath=Edit_Path.getText();
	if(filepath.length() > 0){
		ofstream fout;
		fout.open(filepath);
		fout<<Edit_Code.getText();
		fout.close();
	}
}
void Gbianyi(string filename){
	saveToFile();
	string newpath = "\"E:\\�˴洢�ռ�\\Ӧ�ó��� (D)\\Program Files\\Dev-Cpp\\ConsolePauser.exe\" ";
	newpath = newpath+filepath;
	int pos = newpath.rfind(".");
	newpath = newpath.substr(0,pos + 1);
	newpath.append("exe");
	//cout << newpath << endl;

	stringstream command;
	command << "\"E:\\�˴洢�ռ�\\Ӧ�ó��� (D)\\Program Files\\Dev-Cpp\\TDM-GCC-64\\bin\\g++.exe\" " << filepath << " -o " << newpath;

	system(command.str().data());
	cout << "--------------------\n";
	system(newpath.data());
	return;
}
bool findcmd(string narm,int i){
	if(!(narm=="{"||narm=="}"||narm==";"||narm=="=")){
		for(int j=1;j<=Tab;j++) printf("    ");
	}else{
		if(narm=="}"){
			cout<<"\n";
			for(int j=1;j<=Tab-1;j++) printf("    ");
		}
	}
	if(narm=="������ȫ����"){
		hang++;
		printf("return 0;\n}\n");
		return 1;
	}
	if(narm=="��ȡ��"){
		printf("#include");
		return 0;
	}
	if(narm=="����ͷ�ļ�"){
		hang++;
		printf("<bits/stdc++.h>\n");
		return 0;
	}
	if(narm=="Windowsͷ�ļ�"){
		hang++;
		printf("<windows.h>\n");
		return 0;
	}
	if(narm=="="){
		hang++;
		printf("\n");
		return 0;
	}
	if(narm=="����"){
		printf("=");
		return 0;
	}
	if(narm=="�����ռ�"){
		hang++;
		printf("using namespace std;\n");
		return 0;
	}
	if(narm=="�������ֱ���"){
		printf("#include");
		return 0;
	}
	if(narm=="���"){
		printf("printf(\"");
		cin>>narm;cout<<narm<<"\")";
		return 0;
	}
	if(narm==";"){
		hang++;printf(";\n");
		return 0;
	}
	if(narm=="������"){
		printf("int main()");
		return 0;
	}
	if(narm=="{"){
		hang++;Tab++;printf("{\n");
		return 0;
	}
	if(narm=="}"){
		hang++;Tab--;printf("}\n");
		return 0;
	}
	if(narm=="ע��"){
		cin>>Temp[1];
		cout<<"/*"<<Temp[1]<<"*/";
		return 0;
	}
	if(narm=="����"){
		cout<<"int";
		return 0;
	}
	if(narm=="`"){
		cout<<" ";
		return 0;
	}
	if(narm=="�ַ���"){
		cout<<"string";
		return 0;
	}
	if(narm=="˫���ȸ�����"){
		cout<<"double";
		return 0;
	}
	if(narm=="������"){cout<<"long long";return 0;}
	if(narm=="ϵͳ����"){cin>>Temp[1];cout<<"system(\""<<Temp[1]<<"\")";return 0;}
	if(narm=="#"){cin>>Temp[1];cout<<Temp[1];return 0;}
	if(narm==","){printf(",");return 0;}
	if(narm=="�ı��ļ�����"){cin>>Temp[1]>>Temp[2];cout<<"#define "<<Temp[1]<<" "<<Temp[2]<<"\n";return 0;}
	if(narm=="��Ϣ��"){cin>>Temp[2]/*����*/>>Temp[1]/*����*/;printf("boxreturn=MessageBox(NULL,\"");cout<<Temp[1];printf("\",\"");cout<<Temp[2];printf("\",MB_SYSTEMMODAL|MB_YESNOCANCEL|MB_ICONQUESTION)");return 0;}
	if(narm=="ѭ��"){
		cin>>Temp[1]>>Temp[2]>>Temp[3];
		/*1:��ֵ 2:���� 3:ÿ��ִ��*/
		cout<<"for("<<Temp[1]<<";"<<Temp[2]<<";"<<Temp[3]<<")";
		return 0;
	}
	if(narm=="���"){
		cin>>Temp[1];
		cout<<"if("<<Temp[1]<<")";
		return 0;
	}
	narm="����\""+narm+"\"������";
	MessageBox(NULL,narm.data(),"����ʱ���ִ���",MB_SYSTEMMODAL|MB_ABORTRETRYIGNORE|MB_ICONERROR);
	if(boxreturn==IDABORT) exit(0);
	if(boxreturn==IDRETRY) findcmd(narm,i);
	if(boxreturn==IGNORE) return 0;
}
void bianyi(string frename){
	boxreturn=MessageBox(NULL,"��Ҫ���б�����\n\"��\"����׼����\n\"��\"��ȡ������","����",MB_SYSTEMMODAL|MB_YESNO|MB_ICONQUESTION);
	narm=frename+".spp";
	newfrename=frename+".cpp";
	freopen(narm.data(),"r",stdin);
	freopen(newfrename.data(),"w",stdout);
	for(int i=2;;i++){
		cin>>ml;
		if(findcmd(ml,i)){
//			Gbianyi(frename);
			return;
		}
	}
	fclose(stdin);
	fclose(stdout);
	freopen("CON","r",stdin);
	freopen("CON","w",stdout);
	MessageBox(NULL,"�������","�������",MB_SYSTEMMODAL|MB_OK|MB_ICONQUESTION);
	Gbianyi(frename);
}
void Compile(){
	filepath = Edit_Path.getText();
	if (filepath.length() > 0){
		if (Edit_Code.getText().length() > 0||Edit_Code.getText()=="�����ļ���...(����׺��,����·��)"){
			saveToFile();
			bianyi(filepath);
		}else{
			MessageBox(NULL,"��������:�ļ�Ϊ��","������",MB_SYSTEMMODAL|MB_OK|MB_ICONERROR);
		}
	}
}
void runCode(){
	Compile();
	
}
