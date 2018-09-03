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

typedef struct fdt//一个mfd32字节，一个块512字节，可以存16个mfd
{
	char fileName[14];//文件名
	int rootBlock;//起始块号
	int fileLength; //该文件长度，单位为块

}fdt;
vector<fdt> filesDirectoryTable;
fdt filesDirectoryTableInput;

using blockLinkPointer = int;
blockLinkPointer newBlockLinkPointer;
blockLinkPointer fat[100];//假设磁盘有100个块

int totalFreeBlocks;//假设磁盘有100个块，其中前两个块被系统占用

int totalFiles = filesDirectoryTable.size();

vector<blockLinkPointer> blockLinkPointerLine;//文件块链接向量存储

blockLinkPointer endPointer;//末指针

using command = string[4];
command cmd_in;

typedef void(*func)(void);

typedef struct hand//一个用以存储命令字符串和函数指针的数据结构，可以考虑用pair替代
{
	const char *pname;
	func handler;
}handler;
extern handler handlerList[];