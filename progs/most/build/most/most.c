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
#include <fcntl.h>

unsigned char partition_path[100];
unsigned int block_start = 0;

unsigned int get_block_start(char* partition_path)
{
	char seps[]   = "/"; 
	char* token;
	char block_dev[100] = {0, };
	int fd;
	char buf[100];
	char blockname[32];
	unsigned int block_start = 0;

	strcpy(buf, partition_path);
	
	token = strtok( buf, seps );

	while(token != NULL)
	{
		strcpy(blockname, token);
		token = strtok(NULL, seps);
	}

	sprintf(block_dev, "/sys/class/block/%s/start", blockname);

	fd = open(block_dev, O_RDONLY);

	read(fd, buf, sizeof(buf));

	sscanf(buf, "%u", &block_start);
	
	close(fd);

	printf("read %s : %d\n", block_dev, block_start);

	return block_start;
	
}


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
		
	block_start = get_block_start(partition_path);

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

