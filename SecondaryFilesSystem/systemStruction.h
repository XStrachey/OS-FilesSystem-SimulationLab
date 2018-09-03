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
//using mfd = tuple<userName, userPassword, userOwnUFDLink>;
typedef struct mfd//一个mfd32字节，一个块512字节，可以存16个mfd
{
	char userName[14];
	char userPassword[14];
	int userOwnUFDLink; //该用户的UFD所在的盘号，4字节

}mfd;

//using ufd = tuple<fileName, mode, fileMaxLength, fileAddress>;
typedef struct ufd//当前一个ufd，30字节，还可以考虑添加一个信息，即最大长度，实现可以通过搜寻时返回首次适配空间的大小，不过要考虑其他文件建立时导致的可用空间的变化
{
	char fileName[14];//文件名14字节
	int mode; // 文件权限1 - readonly;  2 - writeonly;  3 - read / write，4字节
	int fileLength;//文件长度，初始为0
	int fileMaxLength;//文件最大长度
	int fileAddress;//文件地址，4字节，该地址为首
}ufd;

//using uof = tuple<fileName, state, mode, readPointer, writePointer>;
typedef struct uof//当前设计下，用户最多可以同时操作4个文件，当前设计的uof为30字节，缺少文件打开后的操作方式，即“文件操作方式”
{
	char fileName[14];//文件名14字节
	int mode;// 文件权限1 - readonly;  2 - writeonly;  3 - read / write，4字节
	int readPointer; //读写指针
	int writePointer;
	int fileLength;
	int fileAddress;
	int state; //0初始创建/打开，1为被读写且未关闭（一种简化的占用表示），状态应该考虑多加几个，空目录/被占用/自己打开/新建
}uof;
/*
using nextAddress = int;
using occupationState = bool;
using content = char[64];
using cluster = tuple<nextAddress, occupationState, content>;
*/

typedef struct cluster//簇，一个簇设置为64字节的大小，还要加额外簇信息8字节
{
	int nextAddress;//指向下一个簇
	bool occupationState;//该块占用情况
	char content[64];//块内容

}cluster;

using command = string[4];
command cmd_in;

typedef void(*func)(void);

typedef struct hand//一个用以存储命令字符串和函数指针的数据结构，可以考虑用pair替代
{
	const char *pname;
	func handler;
}handler;
extern handler handlerList[];