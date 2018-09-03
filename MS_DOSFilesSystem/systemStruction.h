#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

typedef struct fdt//һ��mfd32�ֽڣ�һ����512�ֽڣ����Դ�16��mfd
{
	char fileName[14];//�ļ���
	int rootBlock;//��ʼ���
	int fileLength; //���ļ����ȣ���λΪ��

}fdt;
vector<fdt> filesDirectoryTable;
fdt filesDirectoryTableInput;

using blockLinkPointer = int;
blockLinkPointer newBlockLinkPointer;
blockLinkPointer fat[100];//���������100����

int totalFreeBlocks;//���������100���飬����ǰ�����鱻ϵͳռ��

int totalFiles = filesDirectoryTable.size();

vector<blockLinkPointer> blockLinkPointerLine;//�ļ������������洢

blockLinkPointer endPointer;//ĩָ��

using command = string[4];
command cmd_in;

typedef void(*func)(void);

typedef struct hand//һ�����Դ洢�����ַ����ͺ���ָ������ݽṹ�����Կ�����pair���
{
	const char *pname;
	func handler;
}handler;
extern handler handlerList[];