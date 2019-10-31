#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"

//finds the file. if the file doesn't exist, prints the message otherwise gets
//the information of the file
int find_file(FILE *f,char *filename,directory_entry_t directory,int num_entries,
              int *start_block,int *num_blocks,int *file_size){
        int i;
        int found_file=0;

        fseek(f,num_entries,SEEK_SET);

        for(i=0;i<num_entries;i+=64){
            //read the directory info
            fread(&directory,sizeof(unsigned char),SIZE_DIR_ENTRY,f);

            //if the file status is normal file then process and print the file information
            if(directory.status==DIR_ENTRY_NORMALFILE){
              //printf("%d\n",directory.status);
                if(strcmp(directory.filename,filename)==0){
                    directory.file_size=htonl(directory.file_size);
                    found_file=1;
                    if(directory.file_size==0){
                        printf("%s is empty!!!\n",filename );
                        exit(1);
                    }
                    *start_block=htonl(directory.start_block);
                    *num_blocks=htonl(directory.num_blocks);
                    *file_size=directory.file_size;
                    break;
                }
                i+=64;
            }
            i+=64;
        }

        if(found_file==0){
        printf("%s doesn't EXIST!!!\n",filename );
        exit(1);
        }
        return 1;
}

int eof_value(FILE *f,int last_block){

    int endofFATblock = 0;
    char endoffile[4];
    fseek(f,last_block,SEEK_SET);

    memset(endoffile,0,sizeof(endoffile));
    fread(endoffile,sizeof(endoffile),1,f);
  	memcpy(&endofFATblock, endoffile, sizeof(endoffile));

  	return 	(endofFATblock = htonl(endofFATblock));
}

int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    directory_entry_t directory;
    int  i;
    char *imagename = NULL;
    char *filename  = NULL;
    FILE *f;
    FILE *newFile;
    int fat_address=0;



    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "--file") == 0 && i+1 < argc) {
            filename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL || filename == NULL) {
        fprintf(stderr, "usage: catuvfs --image <imagename> " \
            "--file <filename in image>");
        exit(1);
    }
    f = fopen(imagename, "r");
    fseek(f, 0, SEEK_SET);
    fread(&sb, sizeof(sb), 1, f);

    //find the directory start position
    sb.block_size = htons(sb.block_size);
    sb.num_blocks = htonl(sb.num_blocks);
    sb.fat_start = htonl(sb.fat_start);
    sb.fat_blocks = htonl(sb.fat_blocks);
    sb.dir_start = htonl(sb.dir_start);
    sb.dir_blocks = htonl(sb.dir_blocks);

    int numberofDirectories=sb.dir_start*sb.block_size;
    char file[sb.block_size];
    int charSize=sizeof(file);
    int start_block=0;
    int num_blocks=0;
    int file_size=0;
    int file_content=0;
    int remaining=0;
    char EOFblock[4];
    int endofFile=0;


    find_file(f,filename,directory,numberofDirectories,&start_block,&num_blocks,&file_size);

    newFile=fopen(filename,"wb");
    //printf("%d\n",file_size );
    while(file_size>file_content){

          remaining=file_size-file_content;
          if(sb.block_size>remaining){
              charSize=remaining;
          }
          fseek(f,start_block*sb.block_size,SEEK_SET);
          fread(file,charSize,sizeof(char),f);
          fwrite(file,charSize,sizeof(char),newFile);
          file_content+=charSize;

          fat_address=(sb.fat_start * sb.block_size) + (SIZE_FAT_ENTRY * start_block);
          start_block=eof_value(f,fat_address);

          fseek(f,fat_address,SEEK_SET);

          memset(EOFblock,0,sizeof(EOFblock));
          fread(EOFblock,sizeof(EOFblock),1,f);

          memcpy(&endofFile,file,sizeof(endofFile));
          endofFile=ntohl(endofFile);

 		     if(start_block == FAT_LASTBLOCK){
 			        break;
 		     }
    }

    fclose(newFile);
    fclose(f);
    return 0;
}
