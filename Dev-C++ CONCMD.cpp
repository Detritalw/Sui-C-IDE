#define thisfreline 63
#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
int aline,len;
string a,his[10005],lj;
bool last=1;
void fcw(){
	fclose(stdout);
	freopen("CON","w",stdout);
}
void fcr(){
	fclose(stdin);
	freopen("CON","r",stdin);
}
void couthis(int line){
	system("cls");
	cout<<"  "<<lj<<"|总行数:"<<aline<<"|目前行:"<<len<<"\n";
	for(int i=1;i<=line;i++){
		if(last){
			cout<<len<<"|"<<his[i];
			last=0;
		}
		else cout<<" "<<his[i];
		cout<<" ";
	}
//	cout<<len<<"|";
}
void loadfre(string len){
	freopen(len.data(),"r",stdin);
	cin>>a;cin>>a;cin>>aline;
	for(int i=1;i<=aline;i++){
		cin>>his[i];
	}
	fcr();
	couthis(aline);
	return;
}
void savefre(int line){
	freopen("a.cpp","w",stdout);
	cout<<"#define thisfreline "<<line<<"\n";
	for(int i=1;i<=line;i++){
		cout<<his[i];
		cout<<" ";
	}
	exit(0);
}
int main(){
	cout<<"键入文件名或路径:";
	cin>>lj;
	loadfre(lj);
	for(int i=aline+1;;i++){
		couthis(i);
		getline(cin,his[i]);
		if(his[i]==":存储文件"||
		his[i]==":savefre"||
		his[i]==":保存文件"||
		his[i]==":save"){
			his[i]="";
			savefre(i);
		}
		aline++;cout<<" ";
	}
	return 0;
}

/*
解读MessageBox()函数 (消息弹窗box函数)
 MessageBox(NULL→可能是必要的,消息正文内容,标题,这里是按钮包含的内容,用|分割,MB_OK是确认按钮,MB_ICONERROR是错误图标,MB_ICONINFORMATION是i信息图标);
 由ChatGPT提供的解释
在 Windows.h 中，MessageBox() 函数的最后一个参数 flags 是一个整型值，它指定了显示消息框时的选项和按钮，可以是以下值之一或它们的组合：
MB_OK：消息框包含一个“确定”按钮。
MB_OKCANCEL：消息框包含“确定”和“取消”两个按钮。
MB_ABORTRETRYIGNORE：消息框包含“终止”、“重试”和“忽略”三个按钮。
MB_YESNOCANCEL：消息框包含“是”、“否”和“取消”三个按钮。
MB_YESNO：消息框包含“是”和“否”两个按钮。
MB_RETRYCANCEL：消息框包含“重试”和“取消”两个按钮。
MB_ICONHAND：消息框包含一个手形图标。
MB_ICONQUESTION：消息框包含一个问号图标。
MB_ICONEXCLAMATION：消息框包含一个感叹号图标。
MB_ICONASTERISK：消息框包含一个星号图标。
MB_USERICON：消息框包含一个自定义图标，应使用 SetWindowLong 函数来设置 hIcon 成员。
MB_TOPMOST：消息框显示在所有其他窗口的顶部。
MB_SETFOREGROUND：指定消息框后，将其移到最前面。
MB_DEFAULT_DESKTOP_ONLY：将消息框放置在桌面窗口的顶部。如果使用了 MB_TOPMOST 或 MB_SERVICE_NOTIFICATION，则忽略此标志。
MB_SERVICE_NOTIFICATION：当设置了 MB_SERVICE_NOTIFICATION 标志时，表示应用程序是一个服务程序，应在用户没有登录的情况下显示消息框。

当你使用MessageBox函数并将参数设置为MB_OKCANCEL时，MessageBox函数将返回一个整数值来指示用户点击了哪个按钮。如果用户点击"确定"按钮，函数返回值为IDOK(即1)，如果用户点击"取消"按钮，则函数返回值为IDCANCEL(即2)。
你可以使用这个返回值来确定用户单击的是哪个按钮。
*/

