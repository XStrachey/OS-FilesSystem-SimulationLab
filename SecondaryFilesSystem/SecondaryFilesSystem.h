#pragma once
#include "systemStruction.h"

extern vector<mfd> userInfo;
extern vector<vector<ufd>> userFilesInfo;
extern vector<vector<uof>> userFilesState;
extern vector<cluster> clusterList;

extern mfd userInput;//以下4个变量都是用来临时存放信息的 是输入信息等
extern ufd userFilesInput;
extern uof userFilesStateInput;
extern cluster clusterInput;

extern int num; //选项选择
extern int headNum;
extern int curID;

extern handler handlerList[];
void cmdParser()
{
	cout << "[" << userInfo[curID].userName << "]$ :";//命令行标志，并展示当前登录的用户，默认用户为root
	string s;
	getline(cin, s);

	stringstream stream(s);
	int i = 0;
	//命令解析至 cmd_in 中
	while (stream >> cmd_in[i]) i++;
	//遍历命令列表
	for (int i = 0; handlerList[i].pname != NULL; i++)//遍历函数指针，看有没有对应命令的函数
	{
		if (strcmp(handlerList[i].pname, cmd_in[0].c_str()) == 0)//0 == 0
		{
			handlerList[i].handler();
			break;
		}
		else if (handlerList[i].pname == NULL) cout << "未找到命令" << endl;
	}
}


void do_Help()
{
	cout << "Register usrName password   用户注册" << endl;
	cout << "Login	userName password    用户登陆" << endl;
	cout << "Logout	用户登出" << endl;
	cout << "Passwd	oldPassword  newPassword    修改用户口令" << endl;
	cout << "Create	 filename filemaxlength mode	建立文件" << endl;
	cout << "Delete	 fileName   删除文件" << endl;
	cout << "Mv	srcFile desFile	   改变文件名" << endl;
	cout << "Copy   srcFile desFile   文件拷贝" << endl;
	cout << "Chmod	fileName mode  	 改变文件权限" << endl;
	cout << "Chown	fileName new_owner    改变文件拥有者" << endl;
	cout << "Open   fileName   打开文件" << endl;
	cout << "Close  fileName   关闭文件" << endl;
	cout << "Write	fileName nbytes   写文件" << endl;
	cout << "Read 	fileName nbytes   读文件" << endl;
	cout << "Type	fileName   显示文件内容" << endl;
	cout << "netuser   列出目前系统中所有注册用户名" << endl;
	cout << "dir   列出该用户下所有文件及文件信息" << endl;
	cout << "diruof   列出该用户UOF中所有文件及文件信息" << endl;
	cout << "exit   退出程序" << endl;
	cout << "sysc   同步到磁盘 " << endl;
	cout << "clear   清屏 " << endl;
}

//==============================文件读取辅助函数=====================================================
void initFiletoRom()
{
	FILE *streamInit;
	ofstream diskInit;
	if (fopen("disk.txt", "r") == NULL)
	{
		ofstream diskInit; //创建个对象  
		diskInit.open("./disk.txt"); //创建文件  
		diskInit << "1 root 123456 0 ";  //写入内容  
		diskInit.close();
		//return;
	}
	streamInit = fopen("disk.txt", "r");
	//读入用户信息
	fscanf(streamInit, "%d", &headNum);
	int alreadyNum;
	int ret;
	alreadyNum = 0;
	//需要再处理，最好以\n分割
	while (alreadyNum<headNum && (ret = fscanf(streamInit, "%s %s %d", &(userInput.userName), &(userInput.userPassword), &(userInput.userOwnUFDLink))) != -1)
	{
		userInfo.push_back(userInput);
		alreadyNum++;
	}
	//初始化文件信息
	vector<ufd> temp;
	for (size_t i = 0; i < userInfo.size(); i++)
	{
		userFilesInfo.push_back(temp);
	}
	vector<uof> tempOfState;
	for (size_t i = 0; i < userInfo.size(); i++)
	{
		userFilesState.push_back(tempOfState);
	}

	//读入
	/*
	char filename[14];
	int mode;
	int length;
	int addr;
	*/
	for (size_t i = 0; i < userInfo.size(); i++)
	{
		fscanf(streamInit, "%d", &headNum);
		alreadyNum = 0;

		//需要再处理，最好以\n分割
		while (alreadyNum<headNum && (ret = fscanf(streamInit, "%s %d %d %d %d", &(userFilesInput.fileName), &(userFilesInput.mode), &(userFilesInput.fileLength), &(userFilesInput.fileMaxLength), &(userFilesInput.fileAddress))) != -1)
		{
			userFilesInfo[i].push_back(userFilesInput);
			alreadyNum++;
		}
	}
	/*char filename[14];
	int mode;
	int state; //0建立,1打开
	int read_poit; //读写指针
	int write_poit;*/
	for (size_t i = 0; i < userInfo.size(); i++)
	{
		fscanf(streamInit, "%d", &headNum);
		alreadyNum = 0;
		//需要再处理，最好以\n分割
		while (alreadyNum<headNum && (ret = fscanf(streamInit, "%s %d %d %d %d %d %d", &(userFilesStateInput.fileName), &(userFilesStateInput.mode), &(userFilesStateInput.readPointer), &(userFilesStateInput.writePointer), &(userFilesStateInput.fileLength), &(userFilesStateInput.fileAddress), &(userFilesStateInput.state))) != -1)
		{
			userFilesState[i].push_back(userFilesStateInput);
			alreadyNum++;
		}
	}

	/*
	int next_num;
	int is_data;
	char data[64];*/
	//fscanf(streamInit, "%d", &headNum);
	alreadyNum = 0;
	char Tempbuf[64];
	//需要再处理，最好以\n分割
	int total = 0;
	char c;
	while (total < 68)
	{
		total++;
		memset(Tempbuf, 0, sizeof(Tempbuf));
		if ((ret = fscanf(streamInit, "%d %d", &clusterInput.nextAddress, &clusterInput.occupationState)) != -1)
		{
			if (clusterInput.occupationState == 1)
			{
				fscanf(streamInit, "%c", &c);
				fgets(Tempbuf, 64, streamInit);
			}
			strcpy(clusterInput.content, Tempbuf);
			clusterList.push_back(clusterInput);
		}
		else
		{
			//初始化
			//FileCluster[alreadynum].next_num = alreadynum;
			//FileCluster[alreadynum].is_data = 0;
			clusterInput.nextAddress = alreadyNum;
			clusterInput.occupationState = 0;
			strcpy(clusterInput.content, Tempbuf);

			clusterList.push_back(clusterInput);
		}
		alreadyNum++;
	}

	fclose(streamInit);
}

void outToFile()
{
	FILE* fd;
	fd = fopen("disk.txt", "w");
	fprintf(fd, "%d%c", userInfo.size(), ' ');
	for (size_t i = 0; i < userInfo.size(); i++)
		fprintf(fd, "%s %s %d%c", userInfo[i].userName, userInfo[i].userPassword, userInfo[i].userOwnUFDLink, ' ');
	fprintf(fd, "\n");

	/*
	char filename[14];
	int mode;
	int length;
	int addr;
	*/
	for (size_t i = 0; i < userFilesInfo.size(); i++)
	{
		fprintf(fd, "%d%c", userFilesInfo[i].size(), ' ');
		for (size_t j = 0; j < userFilesInfo[i].size(); j++)
		{
			fprintf(fd, "%s %d %d %d %d%c", userFilesInfo[i][j].fileName, userFilesInfo[i][j].mode, userFilesInfo[i][j].fileLength, userFilesInfo[i][j].fileMaxLength, userFilesInfo[i][j].fileAddress, ' ');
		}
		fprintf(fd, "\n");
	}

	/*char filename[14];
	int mode;
	int state; //0建立,1打开
	int read_poit; //读写指针
	int write_poit;*/
	for (size_t i = 0; i < userFilesState.size(); i++)
	{
		fprintf(fd, "%d%c", userFilesState[i].size(), ' ');
		for (size_t j = 0; j < userFilesState[i].size(); j++)
		{
			fprintf(fd, "%s %d %d %d %d %d %d%c", userFilesState[i][j].fileName, userFilesState[i][j].mode, userFilesState[i][j].readPointer, userFilesState[i][j].writePointer, userFilesState[i][j].fileLength, userFilesState[i][j].fileAddress, userFilesState[i][j].state, ' ');
		}
		fprintf(fd, "\n");
	}

	/*int num;
	int next_num;
	int is_data;
	char data[64];*/

	//fprintf(fd, "%d%c", FileCluster.size(), ' ');
	for (size_t i = 0; i < clusterList.size(); i++)
	{
		//fprintf(fd, "%d %d %s%c",FileCluster[i].next_num, FileCluster[i].is_data, FileCluster[i].data, ' ');
		fprintf(fd, "%d %d%c", clusterList[i].nextAddress, clusterList[i].occupationState, ' ');
		fputs(clusterList[i].content, fd);
		fprintf(fd, "\n");
	}

	fclose(fd);
}
//==============================================文件读取辅助函数=====================================================================

//==============================================辅助函数=====================================================================
void doTempWrite()
{
	char buffer[1024];
	int temp;
	stringstream ss;
	ss << cmd_in[3];
	ss >> temp;

	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			if (userFilesInfo[curID][i].mode == 2 || userFilesInfo[curID][i].mode == 3)//判断权限
			{
				break;
			}
			else
			{
				cout << "没有写的权限!" << endl;
				return;
			}
		}
	}

	int targetAddress;//起始物理块
	int fileMaxLength;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			targetAddress = userFilesInfo[curID][i].fileAddress;
			fileMaxLength = userFilesInfo[curID][i].fileMaxLength;
			break;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	strcpy(buffer, cmd_in[2].c_str());

	int writeBegin;
	writeBegin = userFilesState[curID][index].writePointer;

	//找到写指针所在的最后一个磁盘
	while (clusterList[targetAddress].nextAddress != targetAddress)
	{
		targetAddress = clusterList[targetAddress].nextAddress;
	}

	vector <int> newspace_num;//计算将要占用的物理块的数量
	newspace_num.clear();

	if (temp <= 64 - writeBegin) num = 0;
	else
	{
		num = (int)ceil((temp - (64 - writeBegin))*1.0 / 64);
	}

	newspace_num.push_back(targetAddress);

	cout << newspace_num.size() << endl;//

	for (size_t i = 0; i < clusterList.size(); i++)
	{
		if (newspace_num.size() == num + 1)
			break;
		if (clusterList[i].occupationState == 0)
		{
			newspace_num.push_back(i);
			clusterList[i].occupationState = 1;
		}
	}

	for (size_t k = 0; k < newspace_num.size() - 1; k++)
	{
		clusterList[newspace_num[k]].nextAddress = newspace_num[k + 1];
	}
	for (int i = 0; (i < temp) && (i < fileMaxLength); i++)
	{
		if (writeBegin == 64)
		{
			writeBegin = 0;
			targetAddress = clusterList[targetAddress].nextAddress;
		}
		clusterList[targetAddress].content[writeBegin] = buffer[i];
		writeBegin++;
	}

	//更新写指针
	userFilesState[curID][index].writePointer = writeBegin;
	cout << "磁盘写入成功!" << endl;
	return;
}

void makeNewCopy()//这个函数还有问题
{
	string tempBuffer;

	int targetAddress;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)//这个循环用来获取待复制的文件的地址
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			targetAddress = userFilesInfo[curID][i].fileAddress;
			break;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)//在打开文件列表中找到待拷贝文件
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	while (1)
	{
		if (clusterList[targetAddress].nextAddress == targetAddress)
		{
			for (int i = 0; i < userFilesState[curID][index].writePointer; i++)
			{
				//cout << FileCluster[addre].data[i];
				tempBuffer += clusterList[targetAddress].content[i];
			}
			break;
		}
		else
		{
			for (int i = 0; i < 64; i++)
			{
				//cout << FileCluster[addre].data[i];
				tempBuffer += clusterList[targetAddress].content[i];
			}
			if (clusterList[targetAddress].nextAddress != targetAddress)
			{
				targetAddress = clusterList[targetAddress].nextAddress;
			}
			else
				break;
		}
	}

	//Write	filename buffer nbytes 写文件   物理空间68
	cmd_in[0] = "Write";
	cmd_in[1] = cmd_in[2];
	cmd_in[2] = tempBuffer;
	stringstream ss;
	ss << tempBuffer.length();
	string curtp;
	ss >> curtp;
	cmd_in[3] = curtp;
	//cout <<  cmd_in.cmd_num[0] << " " <<  cmd_in.cmd_num[1] << "  " <<  cmd_in.cmd_num[2] << " " <<  cmd_in.cmd_num[3] << endl;
	//do_Write();
	doTempWrite();
}
//==============================================辅助函数=====================================================================

void do_Register()
{
	if (userInfo.size() < 16)//MFD块最多存16个用户的信息，一个用户的信息为1个MFD
	{
		size_t i = 0;
		for (; i < userInfo.size(); i++)//遍历用户列表，检查是否已经存在该名字的用户
		{
			if (strcmp(cmd_in[1].c_str(), userInfo[i].userName) == 0)
			{
				cout << "用户名已经存在，请重新注册" << endl;
				for (int i = 0; i<4; ++i) cmd_in[i] = "";
				break;
			}
		}

		if (i == userInfo.size())
		{
			strcpy(userInput.userName, cmd_in[1].c_str());
			if(cmd_in[2] != "") strcpy(userInput.userPassword, cmd_in[2].c_str());
			else
			{
				cout << "请输入包含密码的完整命令！" << endl;
				for (int i = 0; i<4; ++i) cmd_in[i] = "";
				return;
			}
			userInput.userOwnUFDLink = userInfo.size();//link为该用户的UFD块所在的物理块号，因为一个用户拥有一个UFD块，所以该块号为size大小（因为第一块块号为0，所以不会出现因为当前用户还没有插入到用户列表而导致的覆盖错误）
			userInfo.push_back(userInput);

			//为新的用户开辟空间
			vector<ufd> t;//UFD，用户拥有文件列表
			userFilesInfo.push_back(t);

			vector<uof> temp;//UOF，用户操作文件列表
			userFilesState.push_back(temp);

			cout << "您的用户名创建成功!" << endl;
			//memset(cmd_in, 0, sizeof(cmd_in));
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
		}
	}
	else
		cout << "总人数已满，无法注册" << endl;
}

void do_Login()
{
	if (cmd_in[2] == "")
	{
		cout << "请输入密码！" << endl;
		return;
	}
	for (size_t i = 0; i < userInfo.size(); i++)//vector <MFD> UsrInfo;//这是一个用户列表，即MFD
	{
		if (strcmp(cmd_in[1].c_str(), userInfo[i].userName) == 0 && strcmp(cmd_in[2].c_str(), userInfo[i].userPassword) == 0)
		{
			curID = i;   //全局记录登陆者的ID
			cout << "您已经登陆成功！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			break;
		}
		else if (i == userInfo.size() - 1)
		{
			cout << "您的用户名不存在或密码错误,请重新登陆!" << endl;
		}
	}
}

void do_sysc();
void do_Logout()
{
	cout << "请问您是否保存信息至磁盘？Y/N" << endl;
	char c;
	c = getchar();
	if (c == 'Y')
	{
		do_sysc();
		curID = 0;
		cout << "已经自动为您使用sysc命令保存信息至磁盘" << endl;
		cout << "注销成功，请选择用户登陆!" << endl;
	}
	else if (c == 'N')
	{
		curID = 0;//不做磁盘更改，变化将在退出程序时丢失
		cout << "注销成功，请选择用户登陆!" << endl;
	}
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Password()
{
	if (cmd_in[1] == "" || cmd_in[2] == "")
	{
		cout << "请输入完整命令：Passwd oldPasswd newPasswd ！" << endl;
		return;
	}
	if (strcmp(userInfo[curID].userPassword, cmd_in[1].c_str()) == 0)//出于安全考虑，该函数只有在用户已经登录时才有效，即只可更改自己的密码
	{
		strcpy(userInfo[curID].userPassword, cmd_in[2].c_str());
		cout << "密码修改成功！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
	}
	else
	{
		cout << "密码有误！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
	}
}

void do_Create()
{
	if (userFilesInfo[curID].size() < 16)//一个用户可以拥有的文件最多不超过16个
	{
		if (cmd_in[1] == "")
		{
			cout << "请输入包含文件名的命令！" << endl;
			return;
		}

		for (size_t i = 0; i < userFilesInfo[curID].size(); i++)//先遍历一下文件列表，看看有没有已经存在的同名文件
		{
			if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				cout << "不能建立同名文件！" << endl;
				return;
			}
		}

		strcpy(userFilesInput.fileName, cmd_in[1].c_str());//copy文件名，即赋予新创建文件以文件名
		stringstream ss;
		stringstream lss;
		ss << cmd_in[3];//传入mode
		int temp;
		ss >> temp;
		int lengthTemp;
		lss << cmd_in[2];
		lss >> lengthTemp;
		if((temp == 1)||(temp == 2)||(temp == 3)) userFilesInput.mode = temp;
		else userFilesInput.mode = 1;
		userFilesInput.fileMaxLength = lengthTemp;
		if (cmd_in[2] == "") userFilesInput.fileMaxLength = 1024;
		userFilesInput.fileLength = 0;
		//要处理接下来的内容存储……
		for (size_t i = 0; i < clusterList.size(); i++)//遍历物理块列表，找到一个空闲块，为创建文件找到可存放地址
		{
			if (clusterList[i].occupationState == 0)//盘中没有数据
			{
				userFilesInput.fileAddress = i;//该空闲块为文件首
				clusterList[i].occupationState = true;//修改物理块状态为1
				break;
			}
		}
		userFilesInfo[curID].push_back(userFilesInput);
		cout << "文件创建成功！" << endl;

		//状态栏
		strcpy(userFilesStateInput.fileName, cmd_in[1].c_str());
		userFilesStateInput.state = 0;
		userFilesStateInput.mode = userFilesInput.mode;
		userFilesStateInput.readPointer = 0;
		userFilesStateInput.writePointer = 0;
		userFilesStateInput.fileLength = 0;
		userFilesStateInput.fileAddress = userFilesInput.fileAddress;
		if (userFilesState[curID].size() < 4)//UOF表最多同时维护4个文件
		{
			userFilesState[curID].push_back(userFilesStateInput);
		}
		else
		{
			cout << "但未能加入UOF表，因为UOF已满，请先用Close命令关闭一个文件！" << endl;
		}
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
	}
	else
		cout << "超过文件上限！" << endl;
}

void do_Delete()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	int flag = 0;
	vector<ufd>::iterator it;
	for (it = userFilesInfo[curID].begin(); it != userFilesInfo[curID].end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//从用户的拥有文件列表UFD中去除该文件
		{
			//FileInfo[curID].erase(it);
			flag = 1;
			break;
		}
	}

	if (flag)
	{
		//注意应该清除占用的内存

		//Type filename
		int address;//从UFD和UOF中找到对应的文件项
		for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
		{
			if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				address = userFilesInfo[curID][i].fileAddress;//找到需要删除的文件的第一个物理块地址
				break;
			}
		}

		if (userFilesState[curID].size() == 0)//检查文件是否已经在UOF表中，如果UOF表为空，当然是不在的
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
		int index;
		for (size_t i = 0; i < userFilesState[curID].size(); i++)
		{
			if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引，并记录
			{
				index = i;
				break;
			}
			else if (i == userFilesState[curID].size())
			{
				cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
				for (int i = 0; i<4; ++i) cmd_in[i] = "";
				return;
			}
		}

		vector<int> reset_addr;
		reset_addr.clear();

		while (1)
		{
			if (clusterList[address].nextAddress == address)//写指针所在的最后一个磁盘即初始磁盘，说白了就是该文件不足一个物理块大小
			{
				for (int i = 0; i < userFilesState[curID][index].writePointer; i++)
					clusterList[address].content[i] = 0;//将原文件所占用的物理块的状态重新标注为0，即空闲
				reset_addr.push_back(address);
				break;
			}
			else//文件大于一个物理块的情况
			{
				reset_addr.push_back(address);
				for (int i = 0; i < 64; i++)
				{
					clusterList[address].content[i] = 0;//将原文件所占用的物理块的状态重新标注为0，即空闲
				}
				if (clusterList[address].nextAddress != address)//读完一块到下一块
				{
					address = clusterList[address].nextAddress;
				}
				else
					break;
			}
		}

		for (size_t w = 0; w < reset_addr.size(); w++)
		{
			clusterList[reset_addr[w]].occupationState = 0;
			clusterList[reset_addr[w]].nextAddress = reset_addr[w];
		}

		//以上都是磁盘清除操作

		//清除文件信息
		vector<ufd>::iterator it;//从用户拥有文件列表中去除该文件名
		for (it = userFilesInfo[curID].begin(); it != userFilesInfo[curID].end(); it++)
		{
			if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)
			{
				userFilesInfo[curID].erase(it);
				flag = 1;
				break;
			}
		}
		//清除状态栏
		vector<uof>::iterator itOfState;
		for (itOfState = userFilesState[curID].begin(); itOfState != userFilesState[curID].end(); itOfState++)
		{
			if (strcmp((*itOfState).fileName, cmd_in[1].c_str()) == 0)
			{
				userFilesState[curID].erase(itOfState);
				break;
			}
		}
		cout << "删除成功!" << endl;
	}
	else
		cout << "文件已经撤销!" << endl;
}

void do_Mv()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	if (flag)
	{
		vector<ufd>::iterator it;
		for (it = userFilesInfo[curID].begin(); it != userFilesInfo[curID].end(); it++)
		{
			if (strcmp((*it).fileName, cmd_in[2].c_str()) == 0)
			{
				cout << "目标更改名已存在，请另取名字！" << endl;
				return;
			}
		}

		for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
		{
			if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				strcpy(userFilesInfo[curID][i].fileName, cmd_in[2].c_str());
				break;
			}
		}

		for (size_t i = 0; i < userFilesState[curID].size(); i++)
		{
			if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				strcpy(userFilesState[curID][i].fileName, cmd_in[2].c_str());
				break;
			}
		}

		cout << "文件重命名成功！" << endl;
	}
	
}

void do_Copy()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	int targetAddress;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			targetAddress = userFilesInfo[curID][i].fileAddress;
			break;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	if (flag)
	{
		size_t i = 0;
		for (; i < userFilesInfo[curID].size(); i++)
		{
			if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[2].c_str()) == 0)
			{
				makeNewCopy();
				return;
			}
		}

		string s1, s2, s3;
		s1 = cmd_in[0];
		s2 = cmd_in[1];
		s3 = cmd_in[2];

		if (i == userFilesInfo[curID].size())
		{
			cmd_in[0] = "Create";
			cmd_in[1] = cmd_in[2];
			cmd_in[2] = "1024";
			cmd_in[3] = "3";//传入的“3”为模式标志，3为可读/可写
			do_Create();//转到创建函数，先创建文件
			cmd_in[0] = s1;
			cmd_in[1] = s2;
			cmd_in[2] = s3;
			makeNewCopy();//再拷贝过去
		}
	}
	
}

void do_Chmod()
{
	int temp;
	stringstream ss;
	ss << cmd_in[2].c_str();
	ss >> temp;

	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}
	if (flag)
	{
		if ((temp == 1) || (temp == 2) || (temp == 3))
		{
			for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
			{
				if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
				{
					userFilesInfo[curID][i].mode = temp;
					break;
				}
			}

			for (size_t i = 0; i < userFilesState[curID].size(); i++)
			{
				if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)
				{
					userFilesState[curID][i].mode = temp;
					break;
				}
			}

			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			cout << "修改权限成功！" << endl;
		}
		else
		{
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			cout << "请输入正确格式的mode，1为只读，2为只写，3为可读/可写！" << endl;
		}
	}
	
}

void do_Chown()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}
	vector<mfd>::iterator targetUser;
	for (targetUser = userInfo.begin(); targetUser != userInfo.end(); targetUser++)
	{
		if (strcmp((*targetUser).userName, cmd_in[2].c_str()) == 0)
		{
			flag = 1;
			break;
		}
		else if (targetUser == userInfo.end() - 1)
		{
			flag = 0;
			cout << "无此用户！" << endl;
			break;
		}
	}
	if (flag)
	{
		vector<ufd>::iterator it;
		for (it = userFilesInfo[curID].begin(); it != userFilesInfo[curID].end(); it++)
		{
			if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)
			{
				strcpy(userFilesInput.fileName, (*it).fileName);
				userFilesInput.mode = (*it).mode;
				userFilesInput.fileMaxLength = (*it).fileMaxLength;
				userFilesInput.fileAddress = (*it).fileAddress;
				userFilesInput.fileLength = (*it).fileLength;

				break;
			}
		}

		vector<uof>::iterator itOfState;
		for (itOfState = userFilesState[curID].begin(); itOfState != userFilesState[curID].end(); itOfState++)
		{
			if (strcmp((*itOfState).fileName, cmd_in[1].c_str()) == 0)
			{
				strcpy(userFilesStateInput.fileName, (*itOfState).fileName);
				userFilesStateInput.state = (*itOfState).state;
				userFilesStateInput.mode = (*itOfState).mode;
				userFilesStateInput.readPointer = (*itOfState).readPointer;
				userFilesStateInput.writePointer = (*itOfState).writePointer;
				userFilesStateInput.fileLength = (*itOfState).fileLength;
				userFilesStateInput.fileAddress = (*itOfState).fileAddress;

				break;
			}
		}

		int targetID;
		for (size_t i = 0; i < userInfo.size(); i++)
		{
			if (strcmp(userInfo[i].userName, cmd_in[2].c_str()) == 0)
			{
				targetID = i;
				break;
			}
		}

		size_t i = 0;//这一段可能会有边界错误
		for (; i < userFilesInfo[targetID].size(); )
		{
			if (strcmp(userFilesInfo[targetID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				cout << "目标用户已经拥有同名文件！" << endl;
				break;
			}
			i++;
		}

		if (i == userFilesInfo[targetID].size())
		{
			userFilesInfo[targetID].push_back(userFilesInput);
			userFilesState[targetID].push_back(userFilesStateInput);
			userFilesInfo[curID].erase(it);
			userFilesState[curID].erase(itOfState);
			cout << "文件拥有者已经更改成功！" << endl;
		}
	}
	
}

void do_Open()//这个还没完成
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	int temp;
	int lengthTemp;
	int addressTemp;
	vector<ufd>::iterator it;
	for (it = userFilesInfo[curID].begin(); it != userFilesInfo[curID].end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)
		{
			//FileInfo[curID].erase(it);
			temp = (*it).mode;
			lengthTemp = (*it).fileLength;
			addressTemp = (*it).fileAddress;
			break;
		}
	}
	
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			if (userFilesState[curID][i].state == 1)//如果已经在uof打开文件列表中
			{
				cout << "该目标文件已经打开" << endl;
			}
			else
			{
				userFilesState[curID][i].state = 1;
				cout << "打开文件成功！" << endl;
			}
			break;
		}
		else if (i == userFilesState[curID].size() - 1)//如果检查完整个uof表还没有，则把该文件添加进uof表里面，并标记为打开
		{
			strcpy(userFilesStateInput.fileName, cmd_in[1].c_str());
			userFilesStateInput.mode = temp;
			userFilesStateInput.state = 0;
			userFilesStateInput.readPointer = 0;
			userFilesStateInput.writePointer = lengthTemp;
			userFilesStateInput.fileLength = lengthTemp;
			userFilesStateInput.fileAddress = addressTemp;
			if (userFilesState[curID].size() < 4)
			{
				userFilesState[curID].push_back(userFilesStateInput);
				cout << "打开文件成功！" << endl;
			}
			else
			{
				cout << "UOF已满，请先用Close命令关闭一个文件！" << endl;
			}
			break;
		}
	}
	if (userFilesState[curID].size() == 0)
	{
		strcpy(userFilesStateInput.fileName, cmd_in[1].c_str());
		userFilesStateInput.mode = temp;
		userFilesStateInput.state = 1;
		userFilesStateInput.readPointer = 0;
		userFilesStateInput.writePointer = lengthTemp;
		userFilesStateInput.fileLength = lengthTemp;
		userFilesStateInput.fileAddress = addressTemp;
		userFilesState[curID].push_back(userFilesStateInput);
		cout << "打开文件成功！" << endl;
	}
}

void do_Close()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	vector<uof>::iterator itOfState;
	for (itOfState = userFilesState[curID].begin(); itOfState != userFilesState[curID].end(); itOfState++)
	{
		if (strcmp((*itOfState).fileName, cmd_in[1].c_str()) == 0)
		{
			userFilesState[curID].erase(itOfState);
			cout << "关闭文件成功！" << endl;
			break;
		}
	}
}

void do_Write()//这个函数还没写完
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	char buffer[1024];
	int temp;
	stringstream ss;
	ss << cmd_in[2];//读入nbytes，即要写多少个字节
	ss >> temp;

	int fileIndex;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			fileIndex = i;
			if (userFilesInfo[curID][i].mode == 2 || userFilesInfo[curID][i].mode == 3)//检查是否拥有写权限
			{
				if (temp < 0) temp = userFilesInfo[curID][i].fileMaxLength;
				break;
			}
			else
			{
				cout << "没有写权限！" << endl;
				return;
			}
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引，并记录
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	int targetAddress;
	int fileMaxLength;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			targetAddress = userFilesInfo[curID][i].fileAddress;
			fileMaxLength = userFilesInfo[curID][i].fileMaxLength;
			break;
		}
	}

	cout << "请输入buff的内容：" << endl;
	gets_s(buffer);
	fflush(stdin);

	//strcpy(buf,  cmd_in.cmd_num[2].c_str());

	int writeBegin;//找到写指针
	writeBegin = userFilesState[curID][index].writePointer;

	//找到写指针所在的最后一个磁盘
	while (clusterList[targetAddress].nextAddress != targetAddress)
		targetAddress = clusterList[targetAddress].nextAddress;

	vector <int> newspaceNum;//计算将要占用的物理块的数量
	newspaceNum.clear();

	//int num = (64-wbegin+temp) / 64-1;
	if (temp <= 64 - writeBegin)//num为写所需要的物理块的块数
		num = 0;
	else
	{
		num = (int)ceil((temp - (64 - writeBegin))*1.0 / 64);//cei()函数为取上顶值函数
	}

	newspaceNum.push_back(targetAddress);//先压入写指针所在的物理块

										 //cout << newspace_num.size() << endl;//

	for (size_t i = 0; i < clusterList.size(); i++)
	{
		if (newspaceNum.size() == num + 1)//已经用完num物理块即停止，可能出现内空隙
			break;
		if (clusterList[i].occupationState == 0)//0为空闲块
		{
			newspaceNum.push_back(i);//记录需要使用的物理块
			clusterList[i].occupationState = 1;//并把对应物理块标记为占用状态
		}
	}

	for (size_t k = 0; k < newspaceNum.size() - 1; k++)//完善文件块的物理结构，即完善指向关系
	{
		clusterList[newspaceNum[k]].nextAddress = newspaceNum[k + 1];
	}

	for (int i = 0; (i < temp) && (i < fileMaxLength); i++)//写入执行应该为待写入字符数或满足不超过文件设定最大长度
	{
		if (writeBegin == 64)//如果写完了1个物理块，则应该跳转到下一个物理块
		{
			writeBegin = 0;
			targetAddress = clusterList[targetAddress].nextAddress;
		}
		clusterList[targetAddress].content[writeBegin] = buffer[i];
		userFilesInfo[curID][fileIndex].fileLength++;
		writeBegin++;
	}

	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引，并记录
		{
			userFilesState[curID][i].fileLength = userFilesInfo[curID][fileIndex].fileLength;
			userFilesState[curID][i].state = 1;
			break;
		}
	}

	//更新写指针
	userFilesState[curID][index].writePointer = writeBegin;
	cout << "磁盘写入成功!" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
	return;
}

void do_Read()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	char buffer[1024];
	int temp;
	stringstream ss;
	ss << cmd_in[2];//读入nbytes，即要写多少个字节
	ss >> temp;

	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			if (userFilesInfo[curID][i].mode == 1 || userFilesInfo[curID][i].mode == 3)
			{
				break;
			}
			else
			{
				cout << "没有读权限！" << endl;
				return;
			}
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引，并记录
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	int targetAddress;
	int fileLength;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			targetAddress = userFilesInfo[curID][i].fileAddress;
			fileLength = userFilesInfo[curID][i].fileLength;
			break;
		}
	}

	int readBegin;
	readBegin = userFilesState[curID][index].readPointer;//指定文件的读取位置，读指针指示的是对文件进行读取的相应位置
	if (readBegin >= fileLength)
	{
		cout << "文件已经读取完毕，读指针已经指向文件末尾！你可以通过Close命令关闭该文件！再次Open将可重置读指针！" << endl;
		userFilesState[curID][index].readPointer = fileLength;
		return;
	}
	int curAddress = readBegin / 64;//物理块，1块大小为64字节，addcur为当前读指针所在的物理块数，如rbegin为61，61/64=1，这说明当前读指针位于第2块，而这个2并不一定是物理块序号中的2，比如这个文件存储的前两个物理块物理块号为1、7，则当前读指针位于序号为7的物理块

	for (int i = 0; i < curAddress; i++)//这个函数是用来解决上一条注释中问题的，即通过next_num找到对应的物理块号
	{
		targetAddress = clusterList[targetAddress].nextAddress;
	}

	if (temp > fileLength) temp = fileLength;
	for (int i = 0; i < temp; i++)//要读几个字符
	{
		if (readBegin % 64 == 63)//读完1个物理块，该到下一块了
		{
			targetAddress = clusterList[targetAddress].nextAddress;
		}
		buffer[i] = clusterList[targetAddress].content[readBegin % 64];//address为块地址，data里面的索引为偏移量
		readBegin++;
	}

	userFilesState[curID][index].readPointer = readBegin;//读指针更新
	userFilesState[curID][index].state = 1;

	cout << "读出的数据是：" << endl;//以下为输出
	for (int i = 0; i < temp; i++) cout << buffer[i];
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
	cout << endl;
	cout << "请务必记得用Close命令关闭文件！以避免不可控错误！" << endl;
	cout << endl;
}

void do_Type()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	if (userFilesInfo[curID].size() == 0)
	{
		cout << "该用户并没有此文件！" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//先检查是否拥有该文件
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "该用户并没有此文件！" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//找到打开文件列表中对应的文件索引，并记录
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "该文件不在UOF中，请先用Open命令打开！" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	int targetAddress;//记录文件所在地址
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			targetAddress = userFilesInfo[curID][i].fileAddress;
			break;
		}
	}

	while (1)
	{
		if (clusterList[targetAddress].nextAddress == targetAddress)
		{
			for (int i = 0; i < userFilesState[curID][index].writePointer; i++)
			{
				cout << clusterList[targetAddress].content[i];
			}
			break;
		}
		else//指向下一个簇
		{
			for (int i = 0; i < 64; i++)
			{
				cout << clusterList[targetAddress].content[i];
			}
			targetAddress = clusterList[targetAddress].nextAddress;
		}
	}
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
	cout << endl;
}

void do_netUser()
{
	for (size_t i = 0; i < userInfo.size(); i++)
	{
		cout << userInfo[i].userName << " userOwnUFDLink:" << userInfo[i].userOwnUFDLink << endl;
	}
}

void do_dir()
{
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		cout << userFilesInfo[curID][i].fileName << " fd:"<< i << " fileLength:" << userFilesInfo[curID][i].fileLength << " fileMaxLength:" << userFilesInfo[curID][i].fileMaxLength << " fileAddress:" << userFilesInfo[curID][i].fileAddress << " mode:" << userFilesInfo[curID][i].mode << endl;
	}
}

void do_dirUOF()
{
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		cout << userFilesState[curID][i].fileName << " mode:" << userFilesState[curID][i].mode << " readPointer:" << userFilesState[curID][i].readPointer << " writePointer:" << userFilesState[curID][i].writePointer << " fileLength:" << userFilesState[curID][i].fileLength << " fileAddress:" << userFilesState[curID][i].fileAddress << " state:" << userFilesState[curID][i].state << endl;
	}
}

void do_sysc()
{
	outToFile();
	cout << "写入到磁盘成功!" << endl;
}

void do_exit()
{
	do_sysc();
	cout << "成功退出!" << endl;
	exit(0);
}

void do_Clear()
{
	system("cls");
}
