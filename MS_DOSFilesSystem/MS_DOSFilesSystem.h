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

void initTable()
{
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "欢迎使用MS-DOS文件系统！！！" << endl;
	cout << "Input ";
	SetConsoleTextAttribute(h, 9);
	cout << "Help";
	SetConsoleTextAttribute(h, 7);
	cout << " to have a guide : " << endl;
	fat[0] = -1;
	fat[1] = -1;
	/*
	fat[3] = 6;//从这行起下四行是为测试准备的人造碎片，之后可以考虑补充一个自动生成函数
	fat[6] = 2;
	fat[2] = 9;
	fat[9] = -1;
	*/
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
		//diskInit << "1 f1 3 4 ";  //写入内容  
		diskInit.close();
		//return;
	}
	streamInit = fopen("disk.txt", "r");
	//读入用户信息
	fscanf(streamInit, "%d", &totalFiles);
	int alreadyNum;
	int ret;
	alreadyNum = 0;
	//需要再处理，最好以\n分割
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
	fprintf(fd, "%d%c", filesDirectoryTable.size(), ' ');//有几个文件
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
//==========================================================文件辅助函数=======================================================================

//=========================================================辅助函数=======================================================================

void findFreeBlock(int begin)//用来寻找下一个可用空闲块
{
	if (begin > 99 || begin < 0)//设定传入的前驱查找块号，前驱查找块号是一个将指向新块的块
	{
		cout << "错误输入！请输入序号为0~99的逻辑记录号！" << endl;
		return;
	}
	for (int it = begin+1; it != begin; it++)//这个循环块的设置是考虑寻找下一个可用块是从前驱查找块后开始寻找
	{
		if (it == 100)//然而如果从前驱查找块后开始查找遍历到fat表末尾还没有找到空闲块，则应该调转从0块开始查找，直到返回到前驱查找块位置，从而真正遍历完所有块空间
		{
			it = 0;
		}
		if (fat[it] == 0)//找到空闲块
		{
			newBlockLinkPointer = it;
			return;
		}
		if (it + 1 == 99)//原因同上
		{
			it = 0;
		}
	}
}

void findLink(int begin)//用来生成文件块链和找到文件末块指针
{
	if ((fat[begin] != -1) && (fat[begin] != 0))//如果该fat项为非0非-1的常数则说明在文件空间链表中
	{
		blockLinkPointerLine.push_back(fat[begin]);
		findLink(fat[begin]);//迭代
	}
	if (fat[begin] == -1) endPointer = begin;//更新末尾块地址，触及尾端
}

void deleteLink(int begin)//用来生成文件块链和找到文件末块指针,并沿链删除，更改占用状态回0
{
	if ((fat[begin] != -1) && (fat[begin] != 0))
	{
		//blockLinkPointerLine.push_back(fat[begin]);
		deleteLink(fat[begin]);//迭代
		fat[begin] = 0;//重置
	}
	if (fat[begin] == -1) fat[begin] = 0;//尾块也重置
}

void updateTotalFreeBlocksNum()//这个函数的作用是在程序启动时根据磁盘信息更新fat表
{
	for (int i = 0; i < 100; i++)
	{
		if (fat[i] == 0)//记录空闲块数
		{
			totalFreeBlocks++;
		}
	}
}

void tempInsert()
{
	stringstream ss;
	stringstream ass;
	ss << cmd_in[2];//传入
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
		cout << "缺少空块可以提供给文件创建使用！请先通过删除文件释放文件占用块！" << endl;
		return;
	}

	int rootBlockPointer = temp;
	fat[temp] = -1;

	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	int index = rootBlockPointer;

	for (int i = 0; i < accountTemp; i++)//accountTemp之所以要加1是因为即使是覆盖写，也指定为从指定块的下一块开始写
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
	cout << "文件创建成功！" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

//=========================================================辅助函数=======================================================================

void showBlockLink()//此函数的功能是用来展现由某个起始块开始延展的块空间链表，可以看做findLink函数加输出
{
	if ((cmd_in[0] == "") || (cmd_in[1] == ""))//命令完整性检测
	{
		cout << "请输入完整命令！" << endl;
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
	for (it = blockLinkPointerLine.begin(); it != blockLinkPointerLine.end(); it++)//输出展示
	{
		if ((it + 1) == blockLinkPointerLine.end()) cout << (*it) << endl;
		else cout << (*it) << " <- ";
	}
	//cout << "末指针:" << endPointer << endl;

	for (int i = 0; i<4; ++i) cmd_in[i] = "";//重置命令行，出于安全考虑
}

void do_Create()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == "") || (cmd_in[2] == ""))
	{
		cout << "请输入完整命令！" << endl;
		return;
	}

	stringstream ss;
	//stringstream ass;
	ss << cmd_in[2];//传入
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
			cout << "该文件名已存在，请另指定文件名！" << endl;//其实如果考虑将文件名-根块号作为索引其实是可以文件重名，不过出于模拟方便考虑，此系统不允许重名
			return;
		}
	}

	/*
	if (fat[temp] != 0)
	{
		cout << "该块已被占用，不可作为根块，请另选块为根块！" << endl;
		return;
	}
	*/

	for (int i = 0; i < 100; i++)
	{
		if (fat[i] == 0)
		{
			filesDirectoryTableInput.rootBlock = i;//系统找到第一个空块，作为起始块
			break;
		}
		else if (i == 99)
		{
			cout << "系统已无可分配空间！请删除文件释放空间！" << endl;
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
	iss >> s4;//文件需要块的数量
	s2 = cmd_in[1];//文件名
	rss << filesDirectoryTableInput.rootBlock;
	rss >> s3;//插入点为根

	cmd_in[0] = "Insert";
	cmd_in[1] = s2;
	cmd_in[2] = s3;
	cmd_in[3] = s4;
	tempInsert();

	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Help()
{
	cout << "Create fileName fileLength    创建" << endl;
	cout << "Write fileName logicalRecordNumber recordBlocksAccount    写入" << endl;
	cout << "Insert	fileName logicalRecordNumber recordBlocksAccount    插入" << endl;
	cout << "Delete	fileName    删除" << endl;
	cout << "showLink blockNumber   显示文件块链" << endl;
	cout << "dir   列出该系统中所有文件及文件信息" << endl;
	cout << "sysc   同步到磁盘 " << endl;
	cout << "clear   清屏" << endl;
	cout << "exit   退出程序" << endl;
}

void do_Write()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == "") || (cmd_in[2] == "") || (cmd_in[3] == ""))
	{
		cout << "请输入完整命令！" << endl;
		return;
	}

	stringstream ss;
	stringstream bss;
	int temp;
	int blockNumTemp;
	ss << cmd_in[2];//传入
	bss << cmd_in[3];
	ss >> temp;
	bss >> blockNumTemp;

	int rootBlockPointer;
	int fileLength;
	vector<fdt>::iterator it;
	for (it = filesDirectoryTable.begin(); it != filesDirectoryTable.end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//找到该文件
		{
			rootBlockPointer = (*it).rootBlock;
			fileLength = (*it).fileLength;
			break;
		}
	}

	if (totalFreeBlocks < temp + blockNumTemp - fileLength)//通过逻辑块号和覆盖写块数以及当前文件块数可以得到尚需块数，如从一个文件的第6位后开始覆盖写，该文件目前占有10个块，覆盖写结束时需要占有13个块，所以尚需块数为6 + 7 - 10 = 3
	{
		cout << "缺少空块可以提供给文件创建使用！请先通过删除文件释放文件占用块！" << endl;//当前块空间可用块数量应该大于等于尚需块数
		return;
	}

	blockLinkPointerLine.clear();//清空之前查找的由其他块作为起始位置查询生成的块链表
	blockLinkPointerLine.push_back(rootBlockPointer);//将起始块添加进新要查找生成的块链表，作为第一项
	findLink(rootBlockPointer);//执行查找生成块链表过程，该过程将不断迭代，直至触及链表尾端，即-1项

	
	stringstream ass;
	ass << cmd_in[3];
	int accountTemp;
	ass >> accountTemp;

	int index = 0;//因为系统占有0块，所以不用担心冲突
	if (temp > (*it).fileLength)//逻辑块号不应大于文件长度
	{
		cout << "文件无此块！" << endl;
		return;
	}
	else
	{
		index = blockLinkPointerLine[temp - 1];
	}

	for (int i = 0; i < accountTemp + 1; i++)//accountTemp之所以要加1是因为是覆盖写，也指定为从指定块的下一块开始写
	{
		int indexTemp = index;
		index = fat[index];
		if ((index == -1) && (i + 1 < accountTemp + 1))
		{
			findFreeBlock(indexTemp);//找空闲块
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

	cout << "文件写入成功！" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Insert()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == "") || (cmd_in[2] == "") || (cmd_in[3] == ""))
	{
		cout << "请输入完整命令！" << endl;
		return;
	}

	stringstream bss;
	int blockNumTemp;
	bss << cmd_in[3];
	bss >> blockNumTemp;
	if (totalFreeBlocks < blockNumTemp)//对于插入写而言，插入多少个块则需要多少个空闲块
	{
		cout << "缺少空块可以提供给文件创建使用！请先通过删除文件释放文件占用块！" << endl;
		return;
	}

	int rootBlockPointer;
	vector<fdt>::iterator it;
	for (it = filesDirectoryTable.begin(); it != filesDirectoryTable.end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//找到对应的起始块
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
	ss << cmd_in[2];//传入
	ass << cmd_in[3];
	int temp;
	int accountTemp;
	ss >> temp;
	ass >> accountTemp;

	int index = -1;//因为系统占有0块，所以不用担心冲突
	if (temp > (*it).fileLength)
	{
		cout << "文件无此块！" << endl;
		return;
	}
	else
	{
		index = blockLinkPointerLine[temp - 1];
	}

	int nextTemp = fat[index];//逻辑块原先指向的下一个块位置，比如一个文件，块链为3<-8<-9，现在从逻辑块号2插入，即从文件的第二个块8后面插入，这时应该记住文件插入点后的块链接点为9
	for (int i = 0; i < accountTemp; i++)
	{
		int indexTemp = index;
		findFreeBlock(indexTemp);
		fat[index] = newBlockLinkPointer;//插入写指向新块
		index = newBlockLinkPointer;
		totalFreeBlocks--;//空闲块减少
		(*it).fileLength++;//文件长度增加
	}
	fat[index] = nextTemp;//插入写的最后一步是将插入的最后一块指向逻辑块原先指向的块，在上面的例子中来看，是9
	blockLinkPointerLine.clear();
	blockLinkPointerLine.push_back(rootBlockPointer);
	findLink(rootBlockPointer);

	cout << "文件插入写入成功！" << endl;
	for (int i = 0; i<4; ++i) cmd_in[i] = "";
}

void do_Delete()
{
	if ((cmd_in[0] == "") || (cmd_in[1] == ""))
	{
		cout << "请输入完整命令！" << endl;
		return;
	}

	int rootBlockPointer;
	vector<fdt>::iterator it;
	for (it = filesDirectoryTable.begin(); it != filesDirectoryTable.end(); it++)
	{
		if (strcmp((*it).fileName, cmd_in[1].c_str()) == 0)//从用户的拥有文件列表UFD中去除该文件
		{
			rootBlockPointer = (*it).rootBlock;
			filesDirectoryTable.erase(it);
			break;
		}
	}

	deleteLink(rootBlockPointer);
	cout << "删除文件成功！已经释放块空间！" << endl;
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
	cout << "写入到磁盘成功!" << endl;
}

void do_Clear()
{
	system("cls");
}

void do_exit()
{
	outToFile();
	cout << "成功退出!" << endl;
	exit(0);
}