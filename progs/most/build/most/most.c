/*
* Mobile Storage Analyzer
*
* Copyright (C) 2012 Kisung Lee <kisunglee@hanyang.ac.kr>
*
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

unsigned char partition_path[100];

int main(int argc, char **argv)
{
	int		exit_status = 0;
	const char	*usage = "Usage: %s [-b blocksize] [-s superblock] [-f cmd_file] [-R request] [-V] [[-w] [-c] device]";

	if(argc!=5)
	{
		printf("argc error\n");
		goto print_usage;

	}

	memset(partition_path, 0x0, sizeof(partition_path));

	strcpy(partition_path, argv[2]);

	printf("partition %s\n",partition_path );	
		
	if(!strncmp("EXT4", argv[1], 4))
		ext_main(argc, argv);
	else if(!strncmp("FAT32", argv[1], 5))
		fat_main(argc, argv);
	else
	{
		printf("Not identified\n");	
		goto print_usage;
	}

	return exit_status;

print_usage:
	fprintf(stderr, "Usage: %s [filesystem (EXT4/FAT32)] [partition path] [input file] [output file]\n", argv[0]);
	fprintf(stderr, "Example: %s EXT4 /dev/block/mmcblk0p1 trace_in.log trace_out.log \n", argv[0]);

	return 1;

}

