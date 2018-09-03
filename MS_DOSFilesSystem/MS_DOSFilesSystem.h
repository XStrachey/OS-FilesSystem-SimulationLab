#pragma once
#include "systemStruction.h"
#include<windows.h>

void cmdParser()
{
	cout << "<<<";
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

void initTable()
{
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "��ӭʹ��MS-DOS�ļ�ϵͳ������" << endl;
	cout << "Input ";
	SetConsoleTextAttribute(h, 9);
	cout << "Help";
	SetConsoleTextAttribute(h, 7);
	cout << " to have a guide : " << endl;
	fat[0] = -1;
	fat[1] = -1;
	/*
	fat[3] = 6;//����������������Ϊ����׼����������Ƭ��֮����Կ��ǲ���һ���Զ����ɺ���
	fat[6] = 2;
	fat[2] = 9;
	fat[9] = -1;
	*/
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
		//diskInit << "1 f1 3 4 ";  //д������  
		diskInit.close();
		//return;
	}
	streamInit = fopen("disk.txt", "r");
	//�����û���Ϣ
	fscanf(streamInit, "%d", &totalFiles);
	int alreadyNum;
	int ret;
	alreadyNum = 0;
	//��Ҫ�ٴ��������\n�ָ�
	while (alreadyNum<totalFiles && (ret = fscanf(streamInit, "%s %d %d", &(filesDirectoryTableInput.fileName), &(filesDirectoryTableInput.rootBlock), &(filesDirectoryTableInput.fileLength))) != -1)
	{
		filesDirectoryTable.push_back(filesDirectoryTableInput);
		alreadyNum++;
	}

	alreadyNum = 0;
	while (alreadyNum<100 && (ret = fscanf(streamInit, "%d %d", &alreadyNum, &(newBlockLinkPointer))) != -1)
	{
		fat[alreadyNum] = newBlockLinkPointer;
		alreadyNum++;
	}

	fclose(streamInit);
}

void outToFile()
{
	FILE* fd;
	fd = fopen("disk.txt", "w");
	fprintf(fd, "%d%c", filesDirectoryTable.size(), ' ');//�м����ļ�
	for (size_t i = 0; i < filesDirectoryTable.size(); i++)
		fprintf(fd, "%s %d %d%c", filesDirectoryTable[i].fileName, filesDirectoryTable[i].rootBlock, filesDirectoryTable[i].fileLength, ' ');
	fprintf(fd, "\n");

	for (size_t i = 0; i < 100; i++)
	{
		//fprintf(fd, "%d %d %s%c",FileCluster[i].next_num, FileCluster[i].is_data, FileCluster[i].data, ' ');
		fprintf(fd, "%d %d %c", i, fat[i], ' ');
		fprintf(fd, "\n");
	}

	fclose(fd);
}
//==========================================================�ļ���������=======================================================================

//=========================================================��������=======================================================================

void findFreeBlock(int begin)//����Ѱ����һ�����ÿ��п�
{
	if (begin > 99 || begin < 0)//�趨�����ǰ�����ҿ�ţ�ǰ�����ҿ����һ����ָ���¿�Ŀ�
	{
		cout << "�������룡���������Ϊ0~99���߼���¼�ţ�" << endl;
		return;
	}
	for (int it = begin+1; it != begin; it++)//���ѭ����������ǿ���Ѱ����һ�����ÿ��Ǵ�ǰ�����ҿ��ʼѰ��
	{
		if (it == 100)//Ȼ�������ǰ�����ҿ��ʼ���ұ�����fat��ĩβ��û���ҵ����п飬��Ӧ�õ�ת��0�鿪ʼ���ң�ֱ�����ص�ǰ�����ҿ�λ�ã��Ӷ��������������п�ռ�
		{
			it = 0;
		}
		if (fat[it] == 0)//�ҵ����п�
		{
			newBlockLinkPointer = it;
			return;
		}
		if (it + 1 == 99)//ԭ��ͬ��
		{
			it = 0;
		}
	}
}

void findLink(int begin)//���������ļ��������ҵ��ļ�ĩ��ָ��
{
	if ((fat[begin] != -1) && (fat[begin] != 0))//�����fat��Ϊ��0��-1�ĳ�����˵�����ļ��ռ�������
	{
		blockLinkPointerLine.push_back(fat[begin]);
		findLink(fat[begin]);//����
	}
	if (fat[begin] == -1) endPointer = begin;//����ĩβ���ַ������β��
}

void deleteLink(int begin)//���������ļ��������ҵ��ļ�ĩ��ָ��,������ɾ��������ռ��״̬��0
{
	if ((fat[begin] != -1) && (fat[begin] != 0))
	{
		//blockLinkPointerLine.push_back(fat[begin]);
		deleteLink(fat[begin]);//����
		fat[begin] = 0;//����
	}
	if (fat[begin] == -1) fat[begin] = 0;//β��Ҳ����
}

void updateTotalFreeBlocksNum()//����������������ڳ�������ʱ���ݴ�����Ϣ����fat��
{
	for (int i = 0; i < 100; i++)
	{
		if (fat[i] == 0)//��¼���п���
		{
			totalFreeBlocks++;
		}
	}
}

void tempInsert()
{
	stringstream ss;
	stringstream ass;
	ss << cmd_in[2];//����
	ass << cmd_in[3];
	int temp;
	int accountTemp;
	ss >> temp;
	ass >> accountTemp;

	stringstream bss;
	int blockNumTemp;
	bss << cmd_in[3];
	bss >> blockNumTemp;
	if (totalFreeBlocks < blockNumTemp)
	{
		cout << "ȱ�ٿտ�����ṩ���ļ�����ʹ�ã�����ͨ��ɾ���ļ��ͷ��ļ�ռ�ÿ飡" << endl;
		return;
	}

	int rootBlockPointer = temp;
	fat[temp] = -1;

	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	int index = rootBlockPointer;

	for (int i = 0; i < accountTemp; i++)//accountTemp֮����Ҫ��1����Ϊ��ʹ�Ǹ���д��Ҳָ��Ϊ��ָ�������һ�鿪ʼд
	{
		int indexTemp = index;
		findFreeBlock(indexTemp);
		fat[index] = newBlockLinkPointer;
		index = newBlockLinkPointer;
		totalFreeBlocks--;
	}
	fat[index] = -1;
	totalFreeBlocks--;
	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	filesDirectoryTable.push_back(filesDirectoryTableInput);
	cout << "�ļ������ɹ���" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

//=========================================================��������=======================================================================

void showBlockLink()//�˺����Ĺ���������չ����ĳ����ʼ�鿪ʼ��չ�Ŀ�ռ��������Կ���findLink���������
{
	if ((cmd_in[0] == "") || (cmd_in[1] == ""))//���������Լ��
	{
		cout << "�������������" << endl;
		return;
	}

	stringstream ss;
	int temp;
	ss << cmd_in[1];
	ss >> temp;

	int rootBlockPointer = temp;
	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	vector<blockLinkPointer>::iterator it;
	for (it = blockLinkPointerLine.begin(); it != blockLinkPointerLine.end(); it++)//���չʾ
	{
		if ((it + 1) == blockLinkPointerLine.end()) cout << (*it) << endl;
		else cout << (*it) << " <- ";
	}
	//cout << "ĩָ��:" << endPointer << endl;

	for (int i = 0; i<4; ++i) cmd_in[i] = "";//���������У����ڰ�ȫ����
}

void do_Create()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == "") || (cmd_in[2] == ""))
	{
		cout << "�������������" << endl;
		return;
	}

	stringstream ss;
	//stringstream ass;
	ss << cmd_in[2];//����
	//ass << cmd_in[3];
	int temp;
	//int accountTemp;
	strcpy_s(filesDirectoryTableInput.fileName, cmd_in[1].c_str());
	ss >> temp;
	//ass >> accountTemp;

	vector<fdt>::iterator it;
	for (it = filesDirectoryTable.begin(); it != filesDirectoryTable.end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//
		{
			cout << "���ļ����Ѵ��ڣ�����ָ���ļ�����" << endl;//��ʵ������ǽ��ļ���-�������Ϊ������ʵ�ǿ����ļ���������������ģ�ⷽ�㿼�ǣ���ϵͳ����������
			return;
		}
	}

	/*
	if (fat[temp] != 0)
	{
		cout << "�ÿ��ѱ�ռ�ã�������Ϊ���飬����ѡ��Ϊ���飡" << endl;
		return;
	}
	*/

	for (int i = 0; i < 100; i++)
	{
		if (fat[i] == 0)
		{
			filesDirectoryTableInput.rootBlock = i;//ϵͳ�ҵ���һ���տ飬��Ϊ��ʼ��
			break;
		}
		else if (i == 99)
		{
			cout << "ϵͳ���޿ɷ���ռ䣡��ɾ���ļ��ͷſռ䣡" << endl;
			return;
		}
	}
	filesDirectoryTableInput.fileLength = temp;

	string s1, s2, s3, s4;
	stringstream css;
	stringstream iss;
	stringstream rss;
	int inputTemp;
	css << cmd_in[2];
	css >> inputTemp;
	inputTemp--;
	iss << inputTemp;
	iss >> s4;//�ļ���Ҫ�������
	s2 = cmd_in[1];//�ļ���
	rss << filesDirectoryTableInput.rootBlock;
	rss >> s3;//�����Ϊ��

	cmd_in[0] = "Insert";
	cmd_in[1] = s2;
	cmd_in[2] = s3;
	cmd_in[3] = s4;
	tempInsert();

	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Help()
{
	cout << "Create fileName fileLength    ����" << endl;
	cout << "Write fileName logicalRecordNumber recordBlocksAccount    д��" << endl;
	cout << "Insert	fileName logicalRecordNumber recordBlocksAccount    ����" << endl;
	cout << "Delete	fileName    ɾ��" << endl;
	cout << "showLink blockNumber   ��ʾ�ļ�����" << endl;
	cout << "dir   �г���ϵͳ�������ļ����ļ���Ϣ" << endl;
	cout << "sysc   ͬ�������� " << endl;
	cout << "clear   ����" << endl;
	cout << "exit   �˳�����" << endl;
}

void do_Write()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == "") || (cmd_in[2] == "") || (cmd_in[3] == ""))
	{
		cout << "�������������" << endl;
		return;
	}

	stringstream ss;
	stringstream bss;
	int temp;
	int blockNumTemp;
	ss << cmd_in[2];//����
	bss << cmd_in[3];
	ss >> temp;
	bss >> blockNumTemp;

	int rootBlockPointer;
	int fileLength;
	vector<fdt>::iterator it;
	for (it = filesDirectoryTable.begin(); it != filesDirectoryTable.end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//�ҵ����ļ�
		{
			rootBlockPointer = (*it).rootBlock;
			fileLength = (*it).fileLength;
			break;
		}
	}

	if (totalFreeBlocks < temp + blockNumTemp - fileLength)//ͨ���߼���ź͸���д�����Լ���ǰ�ļ��������Եõ�������������һ���ļ��ĵ�6λ��ʼ����д�����ļ�Ŀǰռ��10���飬����д����ʱ��Ҫռ��13���飬�����������Ϊ6 + 7 - 10 = 3
	{
		cout << "ȱ�ٿտ�����ṩ���ļ�����ʹ�ã�����ͨ��ɾ���ļ��ͷ��ļ�ռ�ÿ飡" << endl;//��ǰ��ռ���ÿ�����Ӧ�ô��ڵ����������
		return;
	}

	blockLinkPointerLine.clear();//���֮ǰ���ҵ�����������Ϊ��ʼλ�ò�ѯ���ɵĿ�����
	blockLinkPointerLine.push_back(rootBlockPointer);//����ʼ����ӽ���Ҫ�������ɵĿ�������Ϊ��һ��
	findLink(rootBlockPointer);//ִ�в������ɿ�������̣��ù��̽����ϵ�����ֱ����������β�ˣ���-1��

	
	stringstream ass;
	ass << cmd_in[3];
	int accountTemp;
	ass >> accountTemp;

	int index = 0;//��Ϊϵͳռ��0�飬���Բ��õ��ĳ�ͻ
	if (temp > (*it).fileLength)//�߼���Ų�Ӧ�����ļ�����
	{
		cout << "�ļ��޴˿飡" << endl;
		return;
	}
	else
	{
		index = blockLinkPointerLine[temp - 1];
	}

	for (int i = 0; i < accountTemp + 1; i++)//accountTemp֮����Ҫ��1����Ϊ�Ǹ���д��Ҳָ��Ϊ��ָ�������һ�鿪ʼд
	{
		int indexTemp = index;
		index = fat[index];
		if ((index == -1) && (i + 1 < accountTemp + 1))
		{
			findFreeBlock(indexTemp);//�ҿ��п�
			fat[indexTemp] = newBlockLinkPointer;
			if (i + 2 == accountTemp + 1)
			{
				fat[newBlockLinkPointer] = -1;
			}
			else
			{
				fat[newBlockLinkPointer] = -1;
				index = newBlockLinkPointer;
			}
			totalFreeBlocks--;
			(*it).fileLength++;
		}
	}
	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	cout << "�ļ�д��ɹ���" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Insert()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == "") || (cmd_in[2] == "") || (cmd_in[3] == ""))
	{
		cout << "�������������" << endl;
		return;
	}

	stringstream bss;
	int blockNumTemp;
	bss << cmd_in[3];
	bss >> blockNumTemp;
	if (totalFreeBlocks < blockNumTemp)//���ڲ���д���ԣ�������ٸ�������Ҫ���ٸ����п�
	{
		cout << "ȱ�ٿտ�����ṩ���ļ�����ʹ�ã�����ͨ��ɾ���ļ��ͷ��ļ�ռ�ÿ飡" << endl;
		return;
	}

	int rootBlockPointer;
	vector<fdt>::iterator it;
	for (it = filesDirectoryTable.begin(); it != filesDirectoryTable.end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//�ҵ���Ӧ����ʼ��
		{
			rootBlockPointer = (*it).rootBlock;
			break;
		}
	}

	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	stringstream ss;
	stringstream ass;
	ss << cmd_in[2];//����
	ass << cmd_in[3];
	int temp;
	int accountTemp;
	ss >> temp;
	ass >> accountTemp;

	int index = -1;//��Ϊϵͳռ��0�飬���Բ��õ��ĳ�ͻ
	if (temp > (*it).fileLength)
	{
		cout << "�ļ��޴˿飡" << endl;
		return;
	}
	else
	{
		index = blockLinkPointerLine[temp - 1];
	}

	int nextTemp = fat[index];//�߼���ԭ��ָ�����һ����λ�ã�����һ���ļ�������Ϊ3<-8<-9�����ڴ��߼����2���룬�����ļ��ĵڶ�����8������룬��ʱӦ�ü�ס�ļ�������Ŀ����ӵ�Ϊ9
	for (int i = 0; i < accountTemp; i++)
	{
		int indexTemp = index;
		findFreeBlock(indexTemp);
		fat[index] = newBlockLinkPointer;//����дָ���¿�
		index = newBlockLinkPointer;
		totalFreeBlocks--;//���п����
		(*it).fileLength++;//�ļ���������
	}
	fat[index] = nextTemp;//����д�����һ���ǽ���������һ��ָ���߼���ԭ��ָ��Ŀ飬���������������������9
	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	cout << "�ļ�����д��ɹ���" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Delete()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == ""))
	{
		cout << "�������������" << endl;
		return;
	}

	int rootBlockPointer;
	vector<fdt>::iterator it;
	for (it = filesDirectoryTable.begin(); it != filesDirectoryTable.end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//���û���ӵ���ļ��б�UFD��ȥ�����ļ�
		{
			rootBlockPointer = (*it).rootBlock;
			filesDirectoryTable.erase(it);
			break;
		}
	}

	deleteLink(rootBlockPointer);
	cout << "ɾ���ļ��ɹ����Ѿ��ͷſ�ռ䣡" << endl;
}

void do_dir()
{
	for (size_t i = 0; i < filesDirectoryTable.size(); i++)
	{
		cout << filesDirectoryTable[i].fileName << " rootBlock:" << filesDirectoryTable[i].rootBlock << " fileLength:" << filesDirectoryTable[i].fileLength << endl;
	}
}

void do_sysc()
{
	outToFile();
	cout << "д�뵽���̳ɹ�!" << endl;
}

void do_Clear()
{
	system("cls");
}

void do_exit()
{
	outToFile();
	cout << "�ɹ��˳�!" << endl;
	exit(0);
}