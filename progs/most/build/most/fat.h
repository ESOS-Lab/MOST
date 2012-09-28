#ifndef FAT_H
#define FAT_H

#include "DoublyLinkedList.h"
#include "LCRSTree.h"
#include "RedBlackTree.h"

#pragma pack(1)
typedef struct _FAT32_BPB_struct
{
	unsigned char jmpBoot[3];
	unsigned char oemName[8];
	unsigned short bytesPerSec;
	unsigned char secPerClus;
	unsigned short rsvdSecCnt;
	unsigned char numFats;
	unsigned short rootEntCnt;
	unsigned short totalSec16;
	unsigned char media;
	unsigned short fatSize16;
	unsigned short secPerTrk;
	unsigned short numHeads;
	unsigned int hiddSec;
	unsigned int totalSec32;
	unsigned int fatSize32;
	unsigned short extFlags;
	unsigned short fileSysVer;
	unsigned int rootDirClus;
	unsigned short fileSysInfo;
	unsigned short backBootSec;
	unsigned char reserved[12];

	unsigned char driveNumber;
	unsigned char reserved1;
	unsigned char bootSignal;
	unsigned int volumeID;
	unsigned char volumeLabel[11];
	unsigned char fileSysType[8];

	unsigned char bootCodeArea[420];
	unsigned short signature;
}FAT32_BPB;
#pragma pack()

#pragma pack(1)
typedef struct _DIR_struct
{
	unsigned char name[11];
	unsigned char attr;
	unsigned char ntRes;
	unsigned char creatTimeTenth;
	unsigned short creatTime;
	unsigned short creatDate;
	unsigned short lastAccDate;
	unsigned short fastClusHi;
	unsigned short writeTime;
	unsigned short writeDate;
	unsigned short fisrtClusLow;
	unsigned int fileSize;
}DirEntry;
#pragma pack()

#pragma pack(1)
typedef struct _LONG_DIR_struct
{
	unsigned char order;
	unsigned char name1[10];
	unsigned char attr;
	unsigned char type;
	unsigned char chksem;
	unsigned char name2[12];
	unsigned short fisrtClusLow;
	unsigned char name3[4];
}LongDirEntry;
#pragma pack()

typedef struct dir_info_struct
{
	unsigned int cluster[41];
	unsigned int cluster_count;
    LinkNode* head; //link list of files
	unsigned char name[512];
}DirInfo;

typedef struct file_info_struct
{
	unsigned int cluster[41]; //Start cluster
	unsigned int cluster_count;
    RBTNode* tree;
    RBTNode* nil;
	unsigned char name[512];
}FileInfo;
#endif FAT_H
