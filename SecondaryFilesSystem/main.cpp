#include "systemStruction.h"
#include "SecondaryFilesSystem.h"
#include<windows.h>

using namespace std;

vector<mfd> userInfo;
vector<vector<ufd>> userFilesInfo;
vector<vector<uof>> userFilesState;
vector<cluster> clusterList;

mfd userInput;//以下4个变量都是用来临时存放信息的 是输入信息等
ufd userFilesInput;
uof userFilesStateInput;
cluster clusterInput;

int num; //选项选择
int headNum;
int curID;

handler handlerList[] =//命令与函数指针的结构，改成pair好了
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
	cout << "欢迎使用二级文件目录系统！" << endl;
	cout << "Input ";
	SetConsoleTextAttribute(h, 9);
	cout << "Help";
	SetConsoleTextAttribute(h, 7);
	cout <<	" to have a guide : " << endl;
	initFiletoRom();
	while (1)
	{
		//默认root登陆
		//cout << "[" << userInfo[curID].userName << "]$ :";
		cmdParser();
	}
	return 0;
}