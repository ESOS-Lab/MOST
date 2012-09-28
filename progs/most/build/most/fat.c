/*
 * Mobile Storage Analyzer
 *
 * Copyright (C) 2012 Kisung Lee <kisunglee@hanyang.ac.kr>
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <asm/types.h>

#include "DoublyLinkedList.h" //File Linked list
#include "LCRSTree.h" //Directory Tree
#include "RedBlackTree.h" //Cluster Tree
#include "fat.h"

#define NEXUS_S

extern unsigned char partition_path[100];

#ifdef NEXUS_S
#define PARTITION_NAME "/dev/block/mmcblk0p3"

#define PARTITION_BLOCK_START 393472
#define PARTITION_BLOCK_SIZE 3494651
#define PARTITION_BLOCK_END (PARTITION_BLOCK_START+PARTITION_BLOCK_SIZE-1)
#define GROUP_NUM 1
#define START_DATA_BLOCK 6816

static unsigned int group_data_range[GROUP_NUM][2]={{START_DATA_BLOCK, 3494651}};


#define SECTOR_SIZE 512
#define CLUSTER_SIZE 512*8

#define FAT1_POS 0x4000
#define ROOT_DIR_CLUSTER 2
#define FAT_ENTRY_SIZE 4
#define FAT_DIR_ENTRY_NUMBER 128
#define END_OF_FILE 0x0FFFFFFF

#define CLUSTER_POS 0x1A9e400 //Virtual
#define CLUSTER2_POS 0x1AA0400

#else
#error
#endif


RBTNode* Nil=NULL;
LCRSNode * root;

unsigned char buf[SECTOR_SIZE];
unsigned char cluster[CLUSTER_SIZE];
unsigned char file_name[512];

static int fd; //Main file descriptor

char full[200];
char data_name[100];

void print_longName(LongDirEntry * pLongDir, unsigned int entryNum)
{
	//wchar_t filename[512];
	unsigned short filename[512];
	unsigned char final[512];
	unsigned int nameOffset=0;
	int j;

	do
	{
		memcpy(&filename[nameOffset], pLongDir[entryNum].name1, 10);
		nameOffset +=5;
		memcpy(&filename[nameOffset], pLongDir[entryNum].name2, 12);
		nameOffset +=6;
		memcpy(&filename[nameOffset], pLongDir[entryNum].name3, 4);
		nameOffset +=2;

		
	}while((pLongDir[entryNum--].order & 0x40) ==0);

	filename[nameOffset] = 0x0000;

	wcstombs(final, filename, 512);

	printf("long name : %s\n", final);
				
}

void print_longName_Ex(LongDirEntry * pLongDir, unsigned int entryNum, unsigned char *filename)
{
	int i=0;
	int j=0;
	
	//memset(file_name, 0, sizeof(file_name));

	do
	{
	
 		for(j=0; j<10; j=j+2)
 		{
 			filename[i]=pLongDir[entryNum].name1[j];
			i++;
 		}
			
 		for(j=0; j<12; j=j+2)
		{
			filename[i]=pLongDir[entryNum].name2[j];
			i++;
		}

		for(j=0; j<4; j=j+2)
		{
			filename[i]=pLongDir[entryNum].name3[j];
			i++;
		}
	}while(entryNum!=0 && (pLongDir[entryNum--].order & 0x40) ==0);

	//Need to check
	//while(entryNum!=0) => entryNum이 0으로 들어오는 경우 처리해 줌. 
	
	//printf("%s\n", filename);
}

void read_cluster(unsigned int cluster_num, unsigned char * cluster)
{
	unsigned long long offset;

	offset = CLUSTER_POS +(unsigned long long)cluster_num*CLUSTER_SIZE;

	//printf("read_cluster = %ld\n", offset);
	
	if(lseek(fd, offset, SEEK_SET) == -1)
		printf("read_cluster : Lseek error!!!\n");

	memset(cluster, 0, CLUSTER_SIZE);
	
	if(read(fd, cluster, CLUSTER_SIZE) == -1)
		printf("read_cluster : read error!!!\n");

}


void make_cluster_chain(unsigned int cluster_num, DirInfo *dir)
{
	int cluster_offset=0;

	dir->cluster[cluster_offset] = cluster_num;

	do
	{
		dir->cluster_count++;
		
		if(lseek(fd, FAT1_POS, SEEK_SET)==-1)
			printf("make_cluster_chain : lseek error!!!\n");

		if(lseek(fd, dir->cluster[cluster_offset]*FAT_ENTRY_SIZE, SEEK_CUR) == -1)
			printf("make_cluster_chain : lseek error!!!\n");

		cluster_offset++;

		if(cluster_offset > 40)
		{
			printf("make_cluster_chain(). Big directory!\n");
			while(1);
			break;
		}
	
		if(read(fd, &dir->cluster[cluster_offset], FAT_ENTRY_SIZE) == -1)
			printf("make_cluster_chain : read error!!!\n");

		//printf("cluster[%ld]=%d\n", dir->cluster_count, dir->cluster[
	}while(dir->cluster[cluster_offset]!=END_OF_FILE);	
	
}

void make_file_cluster_chain(unsigned int cluster_num, FileInfo *file)
{
	int cluster_offset=0;
	unsigned int cluster;

	//printf("make_file_cluster_chain. cluster_num[%lld]\n", cluster_num);

	file->cluster[cluster_offset] = cluster_num;

	do
	{
		file->cluster_count++;
		
		if(lseek(fd, FAT1_POS, SEEK_SET)==-1)
			printf("make_file_cluster_chain : lseek error!!!\n");

		if(lseek(fd, file->cluster[cluster_offset]*FAT_ENTRY_SIZE, SEEK_CUR) == -1)
			printf("make_file_cluster_chain : lseek error!!!\n");

		cluster_offset++;

		if(cluster_offset > 40)
		{
			printf("make_file_cluster_chain(). Big files!\n");
			while(1);
			break;
		}
		
		if(read(fd, &file->cluster[cluster_offset], FAT_ENTRY_SIZE) == -1)
			printf("make_file_cluster_chain : read error!!!\n");

		
		//printf("make_file_cluster_chain. insert [%lld]\n", file->cluster[cluster_offset]);
	}while(file->cluster[cluster_offset]!=END_OF_FILE);	
}

void make_file_cluster_tree_chain(unsigned int cluster_num, FileInfo *file)
{
	unsigned int pre_cluster=0;
	unsigned int cluster=0;

	//printf("make_file_cluster_tree_chain. cluster_num[%lld]\n", cluster_num);

	pre_cluster=cluster_num;

	file->nil 		  = RBT_CreateNode(0);
	file->nil->Color  = BLACK;	 

	Nil = file->nil;
	
	do
	{
		file->cluster_count++;

		//printf("make_file_cluster_tree_chain. insert [%lld]\n", pre_cluster);
		RBT_InsertNode( &file->tree, RBT_CreateNode(pre_cluster) );	
		
		
		if(lseek(fd, FAT1_POS, SEEK_SET)==-1)
			printf("make_file_cluster_tree_chain : lseek error!!!\n");

		if(lseek(fd, pre_cluster*FAT_ENTRY_SIZE, SEEK_CUR) == -1)
			printf("make_file_cluster_tree_chain : lseek error!!!\n");

		
		if(read(fd, &cluster, FAT_ENTRY_SIZE) == -1)
			printf("make_file_cluster_tree_chain : read error!!!\n");
	
		pre_cluster=cluster;
	}while(pre_cluster!=END_OF_FILE);	

	//RBT_PrintTree( file->tree, 0, 0 );
}

void print_node( LCRSNode* Node, int depth )
{
    int i, j;
	int count=0;
	DirInfo *dir;
	FileInfo *file;
    LinkNode* current = NULL;

	dir = (DirInfo *)Node->Data;

#if 0
    for ( i=0; i<depth; i++ )
        printf("   ");
	printf("##################################\n", depth);
    for ( i=0; i<depth; i++ )
        printf("   ");
    printf("cluster[0]=%ld\n", dir->cluster[0]);
    for ( i=0; i<depth; i++ )
        printf("   ");
    printf("cluster[1]=%ld\n", dir->cluster[1]);
    for ( i=0; i<depth; i++ )
        printf("   ");
    printf("cluster count=%d\n", dir->cluster_count);
#endif
    for ( i=0; i<depth; i++ )
        printf("   ");
	printf("\\%s\n", dir->name);

    count = DLL_GetNodeCount(dir->head);
	
    for ( i = 0; i<count; i++ )
    {
        current = DLL_GetNodeAt( dir->head, i );
		file = (FileInfo *)current->Data;

		for ( j=0; j<depth; j++ )
			printf("   ");

        printf( " + %s", file->name );
        printf( "    %lld\n", file->cluster[0] );
        printf( "    %d\n", file->cluster_count );
    }

	printf("\n");

    if ( Node->LeftChild != NULL )
        print_node(Node->LeftChild, depth+1);

    if ( Node->RightSibling != NULL )
        print_node(Node->RightSibling, depth);
}


void lookup_cluster( LCRSNode* Node, int depth, unsigned int cluster )
{
    int i, j;
	int count=0;
	DirInfo *dir;
	FileInfo *file;
    LinkNode* current = NULL;
    RBTNode* rbNode = NULL;

	dir = (DirInfo *)Node->Data;

    count = DLL_GetNodeCount(dir->head);
	
    for ( i = 0; i<count; i++ )
    {
        current = DLL_GetNodeAt( dir->head, i );
		file = (FileInfo *)current->Data;

		//small files
		#if 0
		for(j=0; j<40; j++)
		{
			if(file->cluster[j] == cluster)
			{
				printf( "lookup_cluster(). Cluster[%lld]:Filename[%s]\n", cluster, file->name );
				return;
			}
	    }
		#endif

		//big files
		if(file->tree != NULL)
		{
			Nil=file->nil;
			
			//printf( "lookup_cluster(). Filename[%s], tree[%x], nil[%x]\n", file->name, file->tree, file->nil );
			
	        rbNode = RBT_SearchNode(file->tree, cluster);
			if(rbNode != NULL)
			{
				//fprintf(ofp, "%s", file->name);
				sprintf(data_name,"%s\t%s", data_name, file->name);
				//fprintf(ofp, "D %s/%.*s", iw->parent,(dirent->name_len & 0xFF), dirent->name);
		        //printf("lookup_cluster(). Cluster[%lld]:Filename[%s]\n", rbNode->Data, file->name);  
				return;
			}
			
			//printf( "lookup_cluster(). Filename[%s] end!\n", file->name );
		}
    }

    if ( Node->LeftChild != NULL )
        lookup_cluster(Node->LeftChild, depth+1, cluster);

    if ( Node->RightSibling != NULL )
        lookup_cluster(Node->RightSibling, depth, cluster);
}

void make_dir_tree(DirInfo *p_dir, LCRSNode *p_node)
{
	DirInfo *dir;
	DirEntry * dirEntry;
	FileInfo *file;
	unsigned int start_cluster=0;
	int i, j, k;
	LCRSNode* child;
    LinkNode* file_node = NULL;
	
	unsigned char *cluster_dump;
	unsigned int longEntryEn=0;
	unsigned int bDir=0;
	unsigned int bFile=0;

	//printf("make_dir_tree : start cluster [%lld], cluster count [%d]\n", p_dir->cluster[0], p_dir->cluster_count);
	
	make_cluster_chain(p_dir->cluster[0], p_dir);
    cluster_dump = malloc(CLUSTER_SIZE);
	memset(cluster_dump, 0, CLUSTER_SIZE);

	for(i=0; i<p_dir->cluster_count; i++)
	{
		read_cluster(p_dir->cluster[i], cluster_dump);

		dirEntry = (DirEntry *)cluster_dump;
		
		for(j=0; j<FAT_DIR_ENTRY_NUMBER; j++)
		{
			//NEED TO CHECK!!! J가 0인데도, longEntryEn이 1인 경우가 있다. 
			if(j==0)
			{
				longEntryEn=0;
				bDir=0;
			}
			
			switch((unsigned char) dirEntry[j].name[0])
			{
				case 0x00: return;
				case 0xE5: continue;
			}

			start_cluster = (dirEntry[j].fisrtClusLow | dirEntry[j].fastClusHi << 16);

			//Skip parent directory
			if(strncmp(dirEntry[j].name, "..", 2)==0)
			{
				//printf("iterate_dir : This is parent directory.\n");
				continue;
			}

			//Skip same directory 
			if((start_cluster == p_dir->cluster[0]) || (start_cluster ==0))
			{
				//printf("iterate_dir : Same directory.\n");
				continue;
			}

			//Long Entry
			if(dirEntry[j].attr == 0x0f)
			{
				longEntryEn = 1;
				continue;
			}

			//Directory
			if(dirEntry[j].attr == 0x10)
			{
				bDir=1;
			}

			//printf("make_dir_tree : Entry[%d], longEntryEn[%d], bDir[%d]\n",j, longEntryEn, bDir);

			//directory processing
			if(bDir == 1)
			{	
				bDir=0;
				
				dir = malloc(sizeof(DirInfo));
				memset(dir, 0, sizeof(DirInfo));

				dir->cluster[0] = start_cluster;

				if(longEntryEn == 1)
				{
					//print_longName((LongDirEntry *)dirEntry, j-1);
					print_longName_Ex((LongDirEntry *)dirEntry, j-1, dir->name);

					#if 0
					for(k=0; k<11; k++)
						dir->name[k]=file_name[k];
					#endif
				}
				else
				{
					for(k=0; k<10; k++)
						dir->name[k]=dirEntry[j].name[k];
				}					
				
				child = LCRS_CreateNode(dir);
				
				LCRS_AddChildNode(p_node, child);

				make_dir_tree(dir, child);
			}
			else //file processing
			{
				file = malloc(sizeof(FileInfo));
				memset(file, 0, sizeof(FileInfo));

				file->cluster[0] = start_cluster;
			

				if(longEntryEn == 1)
				{
					print_longName_Ex((LongDirEntry *)dirEntry, j-1, file->name);

					#if 0
					for(k=0; k<11; k++)
						file->name[k]=file_name[k];
					#endif

				}
				else
				{
					for(k=0; k<11; k++)
					{
						file->name[k]=dirEntry[j].name[k];
					}

					#if 0
					for(k=0; k<10; k++)
					{
						printf("%c", file->name[k]);
					}
					
					printf("\n");
					#endif
				}

				//printf("fileSize=%d\n", dirEntry[j].fileSize);

				#if 0
				if(dirEntry[j].fileSize < 16*1024) // UNDER 16KB - Table
				{
					make_file_cluster_chain(start_cluster, file);
				}
				else //Over 16KB - RB Tree
				#endif
				{
					make_file_cluster_tree_chain(start_cluster, file);
				}

		        file_node = DLL_CreateNode(file);
		        DLL_AppendNode( &p_dir->head, file_node );
			}

			if(longEntryEn == 1)
				longEntryEn=0;
		
		}
		
	}

	free(cluster_dump);

}

static int is_fat_data_block(unsigned long long block)
{
	int i =0;
	int result = 0;
	
	for(i=0; i<GROUP_NUM; i++)
	{
		if(block >= group_data_range[i][0] && block < group_data_range[i][1])
		{
			result = 1;
			break;
		}
	}

	return result;
}

void fat_lookup_cluster(unsigned long long block)
{
	unsigned int cluster;

	if(is_fat_data_block(block))
	{
		cluster = (unsigned int)(block - START_DATA_BLOCK + 2);

		//printf("Cluster[%d] ",cluster);
		//printf("fat_lookup_cluster: cluster [%d]\n", cluster);

		//fprintf(ofp, "D ");
		sprintf(data_name,"%s", "D");

		lookup_cluster(root, 0, cluster);
	}
	else
	{
		//fprintf(ofp, "M ");
		sprintf(data_name,"%s", "M");
	}
	
	//printf("\n");
}

void start_make_dir_tree()
{
	DirInfo *dir;
	
    dir = malloc(sizeof(DirInfo));

	memset(dir, 0, sizeof(DirInfo));

	dir->cluster[0]=ROOT_DIR_CLUSTER;
	
    root = LCRS_CreateNode(dir);
	
	make_dir_tree(dir, root);
}

void fat_open_filesystem()
{
	ssize_t ret;
	FAT32_BPB *bpb;

	fd = open(partition_path, O_RDONLY | O_TRUNC | O_SYNC);

	if(fd < 0)
		printf("File open error!!!\n");

	ret = read(fd, buf, SECTOR_SIZE);	

	if(ret < 0)
		printf("File read error!!!\n");

	bpb = (FAT32_BPB *)buf;

	printf("fat_open_filesystem\n");
	printf("OemName= %s\n", bpb->oemName);
	printf("Bytes per Sector = %d\n", bpb->bytesPerSec);
	printf("Sector per Cluster = %d\n", bpb->secPerClus);
	printf("Total sectors = %d\n", bpb->totalSec32);
	printf("Fat size 32 = %d\n", bpb->fatSize32);
	printf("Root Dir Cluster = %d\n", bpb->rootDirClus);
	printf("Signature = 0x%x\n", bpb->signature);

	start_make_dir_tree();

}

void fat_close_filesystem()
{
	LCRS_DestroyTree(root);
	close(fd);
}

int fat_main(int argc, char *argv[])

{
	FILE *fd;
	int ch;
	char str[20], outfile[20];
	int i=0, num;
	int count;
	__u64 pre_block=0, block=0;
	int bjblock=0;
	char *ofp_buffer = NULL;
	FILE *ofp = NULL;
	int bpart=0;
	
	fd = fopen(argv[3], "r");
	ofp = fopen(argv[4], "w");

	if (!ofp) {
		perror("fopen");
		return 1;
	}

	ofp_buffer = malloc(4096);
	
	if (setvbuf(ofp, ofp_buffer, _IOFBF, 4096)) 
	{
		perror("setvbuf");
		return 1;
	}

	fat_open_filesystem();

	ch=fgetc(fd);
	//fprintf(ofp,"%c", (char)ch);	

	do
	{
		sprintf(full,"%s%c",full,(char)ch);
		
		if((char)ch=='W' || (char)ch=='R')
		{
			count=10;
			i=0;
			
			do
			{
				ch=fgetc(fd);
				//fprintf(ofp,"%c", (char)ch);	
				sprintf(full,"%s%c",full,(char)ch);
				
				str[i]=(char)ch;
				i++;
			}while(count--);

			block = (__u64)atoi(str)/8;

			if(pre_block != block || bjblock!=1)
			{

				if(block >= PARTITION_BLOCK_START && block < PARTITION_BLOCK_END)
				{
					pre_block=block;
					
					block = block - PARTITION_BLOCK_START;
			
					fat_lookup_cluster(block);
					//fprintf(ofp, "\n");		
					bpart=1;
				}
			}

			bjblock=0;
			
			do
			{
				ch=fgetc(fd);

				if(ch!='\n' && ch!=EOF)
					sprintf(full,"%s%c",full,(char)ch);

				if(ch==EOF)
				{
					sprintf(full,"%s\t%s\n",full,data_name);
					//printf("%s", full);
					goto EXIT;
				}

			}while(ch!='\n');

			//sprintf(full,"%s\t%s\n",full,data_name);
			//printf("%s", full);

			if(bpart == 1)
			{
				if(data_name[0] == 'J')
					bjblock=1;
				
				fprintf(ofp, "%s\t%s\n", full, data_name);		
				bpart=0;
			}
			
			memset(full, 0, sizeof(full));
			memset(data_name, 0, sizeof(data_name));

		}
		
	}while((ch=fgetc(fd))!=EOF);
	
EXIT:	
	
	//printf("EXIT\n");
	
	fclose(fd);

	if (ofp_buffer) 
	{
		fflush(ofp);
		free(ofp_buffer);
	}

	fclose(ofp);
	
	fat_close_filesystem();

}

