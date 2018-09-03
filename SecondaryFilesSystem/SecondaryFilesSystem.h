#pragma once
#include "systemStruction.h"

extern vector<mfd> userInfo;
extern vector<vector<ufd>> userFilesInfo;
extern vector<vector<uof>> userFilesState;
extern vector<cluster> clusterList;

extern mfd userInput;//����4����������������ʱ�����Ϣ�� ��������Ϣ��
extern ufd userFilesInput;
extern uof userFilesStateInput;
extern cluster clusterInput;

extern int num; //ѡ��ѡ��
extern int headNum;
extern int curID;

extern handler handlerList[];
void cmdParser()
{
	cout << "[" << userInfo[curID].userName << "]$ :";//�����б�־����չʾ��ǰ��¼���û���Ĭ���û�Ϊroot
	string s;
	getline(cin, s);

	stringstream stream(s);
	int i = 0;
	//��������� cmd_in ��
	while (stream >> cmd_in[i]) i++;
	//���������б�
	for (int i = 0; handlerList[i].pname != NULL; i++)//��������ָ�룬����û�ж�Ӧ����ĺ���
	{
		if (strcmp(handlerList[i].pname, cmd_in[0].c_str()) == 0)//0 == 0
		{
			handlerList[i].handler();
			break;
		}
		else if (handlerList[i].pname == NULL) cout << "δ�ҵ�����" << endl;
	}
}


void do_Help()
{
	cout << "Register usrName password   �û�ע��" << endl;
	cout << "Login	userName password    �û���½" << endl;
	cout << "Logout	�û��ǳ�" << endl;
	cout << "Passwd	oldPassword  newPassword    �޸��û�����" << endl;
	cout << "Create	 filename filemaxlength mode	�����ļ�" << endl;
	cout << "Delete	 fileName   ɾ���ļ�" << endl;
	cout << "Mv	srcFile desFile	   �ı��ļ���" << endl;
	cout << "Copy   srcFile desFile   �ļ�����" << endl;
	cout << "Chmod	fileName mode  	 �ı��ļ�Ȩ��" << endl;
	cout << "Chown	fileName new_owner    �ı��ļ�ӵ����" << endl;
	cout << "Open   fileName   ���ļ�" << endl;
	cout << "Close  fileName   �ر��ļ�" << endl;
	cout << "Write	fileName nbytes   д�ļ�" << endl;
	cout << "Read 	fileName nbytes   ���ļ�" << endl;
	cout << "Type	fileName   ��ʾ�ļ�����" << endl;
	cout << "netuser   �г�Ŀǰϵͳ������ע���û���" << endl;
	cout << "dir   �г����û��������ļ����ļ���Ϣ" << endl;
	cout << "diruof   �г����û�UOF�������ļ����ļ���Ϣ" << endl;
	cout << "exit   �˳�����" << endl;
	cout << "sysc   ͬ�������� " << endl;
	cout << "clear   ���� " << endl;
}

//==============================�ļ���ȡ��������=====================================================
void initFiletoRom()
{
	FILE *streamInit;
	ofstream diskInit;
	if (fopen("disk.txt", "r") == NULL)
	{
		ofstream diskInit; //����������  
		diskInit.open("./disk.txt"); //�����ļ�  
		diskInit << "1 root 123456 0 ";  //д������  
		diskInit.close();
		//return;
	}
	streamInit = fopen("disk.txt", "r");
	//�����û���Ϣ
	fscanf(streamInit, "%d", &headNum);
	int alreadyNum;
	int ret;
	alreadyNum = 0;
	//��Ҫ�ٴ��������\n�ָ�
	while (alreadyNum<headNum && (ret = fscanf(streamInit, "%s %s %d", &(userInput.userName), &(userInput.userPassword), &(userInput.userOwnUFDLink))) != -1)
	{
		userInfo.push_back(userInput);
		alreadyNum++;
	}
	//��ʼ���ļ���Ϣ
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

	//����
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

		//��Ҫ�ٴ��������\n�ָ�
		while (alreadyNum<headNum && (ret = fscanf(streamInit, "%s %d %d %d %d", &(userFilesInput.fileName), &(userFilesInput.mode), &(userFilesInput.fileLength), &(userFilesInput.fileMaxLength), &(userFilesInput.fileAddress))) != -1)
		{
			userFilesInfo[i].push_back(userFilesInput);
			alreadyNum++;
		}
	}
	/*char filename[14];
	int mode;
	int state; //0����,1��
	int read_poit; //��дָ��
	int write_poit;*/
	for (size_t i = 0; i < userInfo.size(); i++)
	{
		fscanf(streamInit, "%d", &headNum);
		alreadyNum = 0;
		//��Ҫ�ٴ��������\n�ָ�
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
	//��Ҫ�ٴ��������\n�ָ�
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
			//��ʼ��
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
	int state; //0����,1��
	int read_poit; //��дָ��
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
//==============================================�ļ���ȡ��������=====================================================================

//==============================================��������=====================================================================
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
			if (userFilesInfo[curID][i].mode == 2 || userFilesInfo[curID][i].mode == 3)//�ж�Ȩ��
			{
				break;
			}
			else
			{
				cout << "û��д��Ȩ��!" << endl;
				return;
			}
		}
	}

	int targetAddress;//��ʼ�����
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
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	strcpy(buffer, cmd_in[2].c_str());

	int writeBegin;
	writeBegin = userFilesState[curID][index].writePointer;

	//�ҵ�дָ�����ڵ����һ������
	while (clusterList[targetAddress].nextAddress != targetAddress)
	{
		targetAddress = clusterList[targetAddress].nextAddress;
	}

	vector <int> newspace_num;//���㽫Ҫռ�õ�����������
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

	//����дָ��
	userFilesState[curID][index].writePointer = writeBegin;
	cout << "����д��ɹ�!" << endl;
	return;
}

void makeNewCopy()//���������������
{
	string tempBuffer;

	int targetAddress;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)//���ѭ��������ȡ�����Ƶ��ļ��ĵ�ַ
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			targetAddress = userFilesInfo[curID][i].fileAddress;
			break;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)//�ڴ��ļ��б����ҵ��������ļ�
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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

	//Write	filename buffer nbytes д�ļ�   ����ռ�68
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
//==============================================��������=====================================================================

void do_Register()
{
	if (userInfo.size() < 16)//MFD������16���û�����Ϣ��һ���û�����ϢΪ1��MFD
	{
		size_t i = 0;
		for (; i < userInfo.size(); i++)//�����û��б�����Ƿ��Ѿ����ڸ����ֵ��û�
		{
			if (strcmp(cmd_in[1].c_str(), userInfo[i].userName) == 0)
			{
				cout << "�û����Ѿ����ڣ�������ע��" << endl;
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
				cout << "���������������������" << endl;
				for (int i = 0; i<4; ++i) cmd_in[i] = "";
				return;
			}
			userInput.userOwnUFDLink = userInfo.size();//linkΪ���û���UFD�����ڵ������ţ���Ϊһ���û�ӵ��һ��UFD�飬���Ըÿ��Ϊsize��С����Ϊ��һ����Ϊ0�����Բ��������Ϊ��ǰ�û���û�в��뵽�û��б�����µĸ��Ǵ���
			userInfo.push_back(userInput);

			//Ϊ�µ��û����ٿռ�
			vector<ufd> t;//UFD���û�ӵ���ļ��б�
			userFilesInfo.push_back(t);

			vector<uof> temp;//UOF���û������ļ��б�
			userFilesState.push_back(temp);

			cout << "�����û��������ɹ�!" << endl;
			//memset(cmd_in, 0, sizeof(cmd_in));
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
		}
	}
	else
		cout << "�������������޷�ע��" << endl;
}

void do_Login()
{
	if (cmd_in[2] == "")
	{
		cout << "���������룡" << endl;
		return;
	}
	for (size_t i = 0; i < userInfo.size(); i++)//vector <MFD> UsrInfo;//����һ���û��б���MFD
	{
		if (strcmp(cmd_in[1].c_str(), userInfo[i].userName) == 0 && strcmp(cmd_in[2].c_str(), userInfo[i].userPassword) == 0)
		{
			curID = i;   //ȫ�ּ�¼��½�ߵ�ID
			cout << "���Ѿ���½�ɹ���" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			break;
		}
		else if (i == userInfo.size() - 1)
		{
			cout << "�����û��������ڻ��������,�����µ�½!" << endl;
		}
	}
}

void do_sysc();
void do_Logout()
{
	cout << "�������Ƿ񱣴���Ϣ�����̣�Y/N" << endl;
	char c;
	c = getchar();
	if (c == 'Y')
	{
		do_sysc();
		curID = 0;
		cout << "�Ѿ��Զ�Ϊ��ʹ��sysc�������Ϣ������" << endl;
		cout << "ע���ɹ�����ѡ���û���½!" << endl;
	}
	else if (c == 'N')
	{
		curID = 0;//�������̸��ģ��仯�����˳�����ʱ��ʧ
		cout << "ע���ɹ�����ѡ���û���½!" << endl;
	}
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Password()
{
	if (cmd_in[1] == "" || cmd_in[2] == "")
	{
		cout << "�������������Passwd oldPasswd newPasswd ��" << endl;
		return;
	}
	if (strcmp(userInfo[curID].userPassword, cmd_in[1].c_str()) == 0)//���ڰ�ȫ���ǣ��ú���ֻ�����û��Ѿ���¼ʱ����Ч����ֻ�ɸ����Լ�������
	{
		strcpy(userInfo[curID].userPassword, cmd_in[2].c_str());
		cout << "�����޸ĳɹ���" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
	}
	else
	{
		cout << "��������" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
	}
}

void do_Create()
{
	if (userFilesInfo[curID].size() < 16)//һ���û�����ӵ�е��ļ���಻����16��
	{
		if (cmd_in[1] == "")
		{
			cout << "����������ļ��������" << endl;
			return;
		}

		for (size_t i = 0; i < userFilesInfo[curID].size(); i++)//�ȱ���һ���ļ��б�������û���Ѿ����ڵ�ͬ���ļ�
		{
			if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				cout << "���ܽ���ͬ���ļ���" << endl;
				return;
			}
		}

		strcpy(userFilesInput.fileName, cmd_in[1].c_str());//copy�ļ������������´����ļ����ļ���
		stringstream ss;
		stringstream lss;
		ss << cmd_in[3];//����mode
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
		//Ҫ��������������ݴ洢����
		for (size_t i = 0; i < clusterList.size(); i++)//����������б��ҵ�һ�����п飬Ϊ�����ļ��ҵ��ɴ�ŵ�ַ
		{
			if (clusterList[i].occupationState == 0)//����û������
			{
				userFilesInput.fileAddress = i;//�ÿ��п�Ϊ�ļ���
				clusterList[i].occupationState = true;//�޸������״̬Ϊ1
				break;
			}
		}
		userFilesInfo[curID].push_back(userFilesInput);
		cout << "�ļ������ɹ���" << endl;

		//״̬��
		strcpy(userFilesStateInput.fileName, cmd_in[1].c_str());
		userFilesStateInput.state = 0;
		userFilesStateInput.mode = userFilesInput.mode;
		userFilesStateInput.readPointer = 0;
		userFilesStateInput.writePointer = 0;
		userFilesStateInput.fileLength = 0;
		userFilesStateInput.fileAddress = userFilesInput.fileAddress;
		if (userFilesState[curID].size() < 4)//UOF�����ͬʱά��4���ļ�
		{
			userFilesState[curID].push_back(userFilesStateInput);
		}
		else
		{
			cout << "��δ�ܼ���UOF����ΪUOF������������Close����ر�һ���ļ���" << endl;
		}
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
	}
	else
		cout << "�����ļ����ޣ�" << endl;
}

void do_Delete()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	int flag = 0;
	vector<ufd>::iterator it;
	for (it = userFilesInfo[curID].begin(); it != userFilesInfo[curID].end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//���û���ӵ���ļ��б�UFD��ȥ�����ļ�
		{
			//FileInfo[curID].erase(it);
			flag = 1;
			break;
		}
	}

	if (flag)
	{
		//ע��Ӧ�����ռ�õ��ڴ�

		//Type filename
		int address;//��UFD��UOF���ҵ���Ӧ���ļ���
		for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
		{
			if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				address = userFilesInfo[curID][i].fileAddress;//�ҵ���Ҫɾ�����ļ��ĵ�һ��������ַ
				break;
			}
		}

		if (userFilesState[curID].size() == 0)//����ļ��Ƿ��Ѿ���UOF���У����UOF��Ϊ�գ���Ȼ�ǲ��ڵ�
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
		int index;
		for (size_t i = 0; i < userFilesState[curID].size(); i++)
		{
			if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����������¼
			{
				index = i;
				break;
			}
			else if (i == userFilesState[curID].size())
			{
				cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
				for (int i = 0; i<4; ++i) cmd_in[i] = "";
				return;
			}
		}

		vector<int> reset_addr;
		reset_addr.clear();

		while (1)
		{
			if (clusterList[address].nextAddress == address)//дָ�����ڵ����һ�����̼���ʼ���̣�˵���˾��Ǹ��ļ�����һ��������С
			{
				for (int i = 0; i < userFilesState[curID][index].writePointer; i++)
					clusterList[address].content[i] = 0;//��ԭ�ļ���ռ�õ�������״̬���±�עΪ0��������
				reset_addr.push_back(address);
				break;
			}
			else//�ļ�����һ�����������
			{
				reset_addr.push_back(address);
				for (int i = 0; i < 64; i++)
				{
					clusterList[address].content[i] = 0;//��ԭ�ļ���ռ�õ�������״̬���±�עΪ0��������
				}
				if (clusterList[address].nextAddress != address)//����һ�鵽��һ��
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

		//���϶��Ǵ����������

		//����ļ���Ϣ
		vector<ufd>::iterator it;//���û�ӵ���ļ��б���ȥ�����ļ���
		for (it = userFilesInfo[curID].begin(); it != userFilesInfo[curID].end(); it++)
		{
			if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)
			{
				userFilesInfo[curID].erase(it);
				flag = 1;
				break;
			}
		}
		//���״̬��
		vector<uof>::iterator itOfState;
		for (itOfState = userFilesState[curID].begin(); itOfState != userFilesState[curID].end(); itOfState++)
		{
			if (strcmp((*itOfState).fileName, cmd_in[1].c_str()) == 0)
			{
				userFilesState[curID].erase(itOfState);
				break;
			}
		}
		cout << "ɾ���ɹ�!" << endl;
	}
	else
		cout << "�ļ��Ѿ�����!" << endl;
}

void do_Mv()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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
				cout << "Ŀ��������Ѵ��ڣ�����ȡ���֣�" << endl;
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

		cout << "�ļ��������ɹ���" << endl;
	}
	
}

void do_Copy()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
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
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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
			cmd_in[3] = "3";//����ġ�3��Ϊģʽ��־��3Ϊ�ɶ�/��д
			do_Create();//ת�������������ȴ����ļ�
			cmd_in[0] = s1;
			cmd_in[1] = s2;
			cmd_in[2] = s3;
			makeNewCopy();//�ٿ�����ȥ
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
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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
			cout << "�޸�Ȩ�޳ɹ���" << endl;
		}
		else
		{
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			cout << "��������ȷ��ʽ��mode��1Ϊֻ����2Ϊֻд��3Ϊ�ɶ�/��д��" << endl;
		}
	}
	
}

void do_Chown()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int flag = 0;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����
		{
			flag = 1;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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
			cout << "�޴��û���" << endl;
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

		size_t i = 0;//��һ�ο��ܻ��б߽����
		for (; i < userFilesInfo[targetID].size(); )
		{
			if (strcmp(userFilesInfo[targetID][i].fileName, cmd_in[1].c_str()) == 0)
			{
				cout << "Ŀ���û��Ѿ�ӵ��ͬ���ļ���" << endl;
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
			cout << "�ļ�ӵ�����Ѿ����ĳɹ���" << endl;
		}
	}
	
}

void do_Open()//�����û���
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
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
			if (userFilesState[curID][i].state == 1)//����Ѿ���uof���ļ��б���
			{
				cout << "��Ŀ���ļ��Ѿ���" << endl;
			}
			else
			{
				userFilesState[curID][i].state = 1;
				cout << "���ļ��ɹ���" << endl;
			}
			break;
		}
		else if (i == userFilesState[curID].size() - 1)//������������uof��û�У���Ѹ��ļ���ӽ�uof�����棬�����Ϊ��
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
				cout << "���ļ��ɹ���" << endl;
			}
			else
			{
				cout << "UOF������������Close����ر�һ���ļ���" << endl;
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
		cout << "���ļ��ɹ���" << endl;
	}
}

void do_Close()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	vector<uof>::iterator itOfState;
	for (itOfState = userFilesState[curID].begin(); itOfState != userFilesState[curID].end(); itOfState++)
	{
		if (strcmp((*itOfState).fileName, cmd_in[1].c_str()) == 0)
		{
			userFilesState[curID].erase(itOfState);
			cout << "�ر��ļ��ɹ���" << endl;
			break;
		}
	}
}

void do_Write()//���������ûд��
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	char buffer[1024];
	int temp;
	stringstream ss;
	ss << cmd_in[2];//����nbytes����Ҫд���ٸ��ֽ�
	ss >> temp;

	int fileIndex;
	for (size_t i = 0; i < userFilesInfo[curID].size(); i++)
	{
		if (strcmp(userFilesInfo[curID][i].fileName, cmd_in[1].c_str()) == 0)
		{
			fileIndex = i;
			if (userFilesInfo[curID][i].mode == 2 || userFilesInfo[curID][i].mode == 3)//����Ƿ�ӵ��дȨ��
			{
				if (temp < 0) temp = userFilesInfo[curID][i].fileMaxLength;
				break;
			}
			else
			{
				cout << "û��дȨ�ޣ�" << endl;
				return;
			}
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����������¼
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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

	cout << "������buff�����ݣ�" << endl;
	gets_s(buffer);
	fflush(stdin);

	//strcpy(buf,  cmd_in.cmd_num[2].c_str());

	int writeBegin;//�ҵ�дָ��
	writeBegin = userFilesState[curID][index].writePointer;

	//�ҵ�дָ�����ڵ����һ������
	while (clusterList[targetAddress].nextAddress != targetAddress)
		targetAddress = clusterList[targetAddress].nextAddress;

	vector <int> newspaceNum;//���㽫Ҫռ�õ�����������
	newspaceNum.clear();

	//int num = (64-wbegin+temp) / 64-1;
	if (temp <= 64 - writeBegin)//numΪд����Ҫ�������Ŀ���
		num = 0;
	else
	{
		num = (int)ceil((temp - (64 - writeBegin))*1.0 / 64);//cei()����Ϊȡ�϶�ֵ����
	}

	newspaceNum.push_back(targetAddress);//��ѹ��дָ�����ڵ������

										 //cout << newspace_num.size() << endl;//

	for (size_t i = 0; i < clusterList.size(); i++)
	{
		if (newspaceNum.size() == num + 1)//�Ѿ�����num����鼴ֹͣ�����ܳ����ڿ�϶
			break;
		if (clusterList[i].occupationState == 0)//0Ϊ���п�
		{
			newspaceNum.push_back(i);//��¼��Ҫʹ�õ������
			clusterList[i].occupationState = 1;//���Ѷ�Ӧ�������Ϊռ��״̬
		}
	}

	for (size_t k = 0; k < newspaceNum.size() - 1; k++)//�����ļ��������ṹ��������ָ���ϵ
	{
		clusterList[newspaceNum[k]].nextAddress = newspaceNum[k + 1];
	}

	for (int i = 0; (i < temp) && (i < fileMaxLength); i++)//д��ִ��Ӧ��Ϊ��д���ַ��������㲻�����ļ��趨��󳤶�
	{
		if (writeBegin == 64)//���д����1������飬��Ӧ����ת����һ�������
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
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����������¼
		{
			userFilesState[curID][i].fileLength = userFilesInfo[curID][fileIndex].fileLength;
			userFilesState[curID][i].state = 1;
			break;
		}
	}

	//����дָ��
	userFilesState[curID][index].writePointer = writeBegin;
	cout << "����д��ɹ�!" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
	return;
}

void do_Read()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	char buffer[1024];
	int temp;
	stringstream ss;
	ss << cmd_in[2];//����nbytes����Ҫд���ٸ��ֽ�
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
				cout << "û�ж�Ȩ�ޣ�" << endl;
				return;
			}
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����������¼
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
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
	readBegin = userFilesState[curID][index].readPointer;//ָ���ļ��Ķ�ȡλ�ã���ָ��ָʾ���Ƕ��ļ����ж�ȡ����Ӧλ��
	if (readBegin >= fileLength)
	{
		cout << "�ļ��Ѿ���ȡ��ϣ���ָ���Ѿ�ָ���ļ�ĩβ�������ͨ��Close����رո��ļ����ٴ�Open�������ö�ָ�룡" << endl;
		userFilesState[curID][index].readPointer = fileLength;
		return;
	}
	int curAddress = readBegin / 64;//����飬1���СΪ64�ֽڣ�addcurΪ��ǰ��ָ�����ڵ������������rbeginΪ61��61/64=1����˵����ǰ��ָ��λ�ڵ�2�飬�����2����һ�������������е�2����������ļ��洢��ǰ���������������Ϊ1��7����ǰ��ָ��λ�����Ϊ7�������

	for (int i = 0; i < curAddress; i++)//������������������һ��ע��������ģ���ͨ��next_num�ҵ���Ӧ��������
	{
		targetAddress = clusterList[targetAddress].nextAddress;
	}

	if (temp > fileLength) temp = fileLength;
	for (int i = 0; i < temp; i++)//Ҫ�������ַ�
	{
		if (readBegin % 64 == 63)//����1������飬�õ���һ����
		{
			targetAddress = clusterList[targetAddress].nextAddress;
		}
		buffer[i] = clusterList[targetAddress].content[readBegin % 64];//addressΪ���ַ��data���������Ϊƫ����
		readBegin++;
	}

	userFilesState[curID][index].readPointer = readBegin;//��ָ�����
	userFilesState[curID][index].state = 1;

	cout << "�����������ǣ�" << endl;//����Ϊ���
	for (int i = 0; i < temp; i++) cout << buffer[i];
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
	cout << endl;
	cout << "����ؼǵ���Close����ر��ļ����Ա��ⲻ�ɿش���" << endl;
	cout << endl;
}

void do_Type()
{
	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	if (userFilesInfo[curID].size() == 0)
	{
		cout << "���û���û�д��ļ���" << endl;
		return;
	}
	for (size_t it = 0; it < userFilesInfo[curID].size(); it++)
	{
		if (strcmp(userFilesInfo[curID][it].fileName, cmd_in[1].c_str()) == 0)//�ȼ���Ƿ�ӵ�и��ļ�
		{
			break;
		}
		else if (it == userFilesInfo[curID].size() - 1)
		{
			cout << "���û���û�д��ļ���" << endl;
			return;
		}
	}

	if (userFilesState[curID].size() == 0)
	{
		cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
		for (int i = 0; i<4; ++i) cmd_in[i] = "";
		return;
	}
	int index;
	for (size_t i = 0; i < userFilesState[curID].size(); i++)
	{
		if (strcmp(userFilesState[curID][i].fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ��б��ж�Ӧ���ļ�����������¼
		{
			index = i;
			break;
		}
		else if (i == userFilesState[curID].size() - 1)
		{
			cout << "���ļ�����UOF�У�������Open����򿪣�" << endl;
			for (int i = 0; i<4; ++i) cmd_in[i] = "";
			return;
		}
	}

	int targetAddress;//��¼�ļ����ڵ�ַ
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
		else//ָ����һ����
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
	cout << "д�뵽���̳ɹ�!" << endl;
}

void do_exit()
{
	do_sysc();
	cout << "�ɹ��˳�!" << endl;
	exit(0);
}

void do_Clear()
{
	system("cls");
}
