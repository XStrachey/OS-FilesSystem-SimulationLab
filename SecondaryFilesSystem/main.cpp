#include "systemStruction.h"
#include "SecondaryFilesSystem.h"
#include<windows.h>

using namespace std;

vector<mfd> userInfo;
vector<vector<ufd>> userFilesInfo;
vector<vector<uof>> userFilesState;
vector<cluster> clusterList;

mfd userInput;//����4����������������ʱ�����Ϣ�� ��������Ϣ��
ufd userFilesInput;
uof userFilesStateInput;
cluster clusterInput;

int num; //ѡ��ѡ��
int headNum;
int curID;

handler handlerList[] =//�����뺯��ָ��Ľṹ���ĳ�pair����
{
	{ "Help", do_Help },
    { "Register", do_Register },
    { "Login", do_Login },
    { "Logout", do_Logout },
    { "Passwd", do_Password },
    { "Create", do_Create },
    { "Delete", do_Delete },
    { "Mv", do_Mv },
    { "Copy", do_Copy },
    { "Chmod", do_Chmod },
    { "Chown", do_Chown },
    { "Open", do_Open },
    { "Close", do_Close },
    { "Write", do_Write },
    { "Read", do_Read },
    { "Type", do_Type },
    { "netuser", do_netUser },
    { "dir", do_dir },
    { "diruof", do_dirUOF },
    { "sysc",do_sysc },
    { "exit", do_exit },
    { "clear", do_Clear },
    { NULL, NULL }
};

int main()
{
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "��ӭʹ�ö����ļ�Ŀ¼ϵͳ��" << endl;
	cout << "Input ";
	SetConsoleTextAttribute(h, 9);
	cout << "Help";
	SetConsoleTextAttribute(h, 7);
	cout <<	" to have a guide : " << endl;
	initFiletoRom();
	while (1)
	{
		//Ĭ��root��½
		//cout << "[" << userInfo[curID].userName << "]$ :";
		cmdParser();
	}
	return 0;
}