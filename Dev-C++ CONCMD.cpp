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
	cout<<"  "<<lj<<"|������:"<<aline<<"|Ŀǰ��:"<<len<<"\n";
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
	cout<<"�����ļ�����·��:";
	cin>>lj;
	loadfre(lj);
	for(int i=aline+1;;i++){
		couthis(i);
		getline(cin,his[i]);
		if(his[i]==":�洢�ļ�"||
		his[i]==":savefre"||
		his[i]==":�����ļ�"||
		his[i]==":save"){
			his[i]="";
			savefre(i);
		}
		aline++;cout<<" ";
	}
	return 0;
}

/*
���MessageBox()���� (��Ϣ����box����)
 MessageBox(NULL�������Ǳ�Ҫ��,��Ϣ��������,����,�����ǰ�ť����������,��|�ָ�,MB_OK��ȷ�ϰ�ť,MB_ICONERROR�Ǵ���ͼ��,MB_ICONINFORMATION��i��Ϣͼ��);
 ��ChatGPT�ṩ�Ľ���
�� Windows.h �У�MessageBox() ���������һ������ flags ��һ������ֵ����ָ������ʾ��Ϣ��ʱ��ѡ��Ͱ�ť������������ֵ֮һ�����ǵ���ϣ�
MB_OK����Ϣ�����һ����ȷ������ť��
MB_OKCANCEL����Ϣ�������ȷ�����͡�ȡ����������ť��
MB_ABORTRETRYIGNORE����Ϣ���������ֹ���������ԡ��͡����ԡ�������ť��
MB_YESNOCANCEL����Ϣ��������ǡ������񡱺͡�ȡ����������ť��
MB_YESNO����Ϣ��������ǡ��͡���������ť��
MB_RETRYCANCEL����Ϣ����������ԡ��͡�ȡ����������ť��
MB_ICONHAND����Ϣ�����һ������ͼ�ꡣ
MB_ICONQUESTION����Ϣ�����һ���ʺ�ͼ�ꡣ
MB_ICONEXCLAMATION����Ϣ�����һ����̾��ͼ�ꡣ
MB_ICONASTERISK����Ϣ�����һ���Ǻ�ͼ�ꡣ
MB_USERICON����Ϣ�����һ���Զ���ͼ�꣬Ӧʹ�� SetWindowLong ���������� hIcon ��Ա��
MB_TOPMOST����Ϣ����ʾ�������������ڵĶ�����
MB_SETFOREGROUND��ָ����Ϣ��󣬽����Ƶ���ǰ�档
MB_DEFAULT_DESKTOP_ONLY������Ϣ����������洰�ڵĶ��������ʹ���� MB_TOPMOST �� MB_SERVICE_NOTIFICATION������Դ˱�־��
MB_SERVICE_NOTIFICATION���������� MB_SERVICE_NOTIFICATION ��־ʱ����ʾӦ�ó�����һ���������Ӧ���û�û�е�¼���������ʾ��Ϣ��

����ʹ��MessageBox����������������ΪMB_OKCANCELʱ��MessageBox����������һ������ֵ��ָʾ�û�������ĸ���ť������û����"ȷ��"��ť����������ֵΪIDOK(��1)������û����"ȡ��"��ť����������ֵΪIDCANCEL(��2)��
�����ʹ���������ֵ��ȷ���û����������ĸ���ť��
*/

