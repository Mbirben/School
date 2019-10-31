#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "disk.h"


/*
 * Based on http://bit.ly/2vniWNb
 */
void pack_current_datetime(unsigned char *entry) {
    assert(entry);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    unsigned short year   = tm.tm_year + 1900;
    unsigned char  month  = (unsigned char)(tm.tm_mon + 1);
    unsigned char  day    = (unsigned char)(tm.tm_mday);
    unsigned char  hour   = (unsigned char)(tm.tm_hour);
    unsigned char  minute = (unsigned char)(tm.tm_min);
    unsigned char  second = (unsigned char)(tm.tm_sec);

    year = htons(year);

    memcpy(entry, &year, 2);
    entry[2] = month;
    entry[3] = day;
    entry[4] = hour;
    entry[5] = minute;
    entry[6] = second;
}


int next_free_block(int *FAT, int max_blocks) {
    assert(FAT != NULL);

    int i;

    for (i = 0; i < max_blocks; i++) {
        if (FAT[i] == FAT_AVAILABLE) {
            return i;
        }
    }

    return -1;
}
void file_exist(FILE *f,directory_entry_t directory,int num_entries,char *sourcename){

      int i;
      int file_not_found=0;

      fseek(f,num_entries,SEEK_SET);

      //each directory entry is 64-bytes in size
      for(i=0;i<num_entries;i+=64){

            fread(&directory,sizeof(unsigned char),SIZE_DIR_ENTRY,f);

            //if the file status is normal file then process and print the file information
            if(directory.status==DIR_ENTRY_NORMALFILE){
              //printf("%d\n",directory.status);
                if(strcmp(directory.filename,sourcename)==0){
                        file_not_found=1;
                        printf("%s already exists!!!\n",sourcename);
                        exit(1);
                }

            }
            i+=64;
      }
}

int free_root_block(FILE *f,directory_entry_t directory,superblock_entry_t sb){

    int i;

    fseek(f,sb.fat_start*sb.block_size,SEEK_SET);
    for(i=0;i<sb.num_blocks;i+=64){
          fread(&directory,sizeof(unsigned char),SIZE_DIR_ENTRY,f);
          if(directory.status==DIR_ENTRY_AVAILABLE){
              return i;
          }else{
            i+=64;
          }
    }
    printf("NO file\n" );
    return -1;
}

int free_fat_block(FILE *f,int fat_entries, int fat_blocks){

    int i;
    int file=0;

    fseek(f,fat_blocks,SEEK_SET);
    for(i=0;i<fat_entries;i++){
        fseek(f,fat_blocks,SEEK_SET);
        fread(&file,sizeof(unsigned char),SIZE_FAT_ENTRY,f);
        file=htonl(file);
        if(file==FAT_AVAILABLE){
          return i;
        }
        fat_blocks+=4;
    }
    printf("NO free FAT blocks\n");
    return -1;

}

int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    directory_entry_t directory;
    directory_entry_t new_directory;
    int  i;
    char *imagename  = NULL;
    char *filename   = NULL;
    char *sourcename = NULL;
    FILE *f;

    FILE *newFile;
    int new_filesize=0;
    int blocks_needed=0;
    int blocks_used=0;
    int first_root_block=0;
    int first_fat_block=0;
    int free_fat=0;
    int free_fat_space=0;
    int file_status=3;
    int file_create_time;
    int file_modified_time;
    char EOFblock[4];



    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "--file") == 0 && i+1 < argc) {
            filename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "--source") == 0 && i+1 < argc) {
            sourcename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL || filename == NULL || sourcename == NULL) {
        fprintf(stderr, "usage: storuvfs --image <imagename> " \
            "--file <filename in image> " \
            "--source <filename on host>\n");
        exit(1);
    }

    f = fopen(imagename, "rb+");
    fseek(f, 0, SEEK_SET);
    fread(&sb, sizeof(sb), 1, f);


    sb.block_size = htons(sb.block_size);
    sb.num_blocks = htonl(sb.num_blocks);
    sb.fat_start = htonl(sb.fat_start);
    sb.fat_blocks = htonl(sb.fat_blocks);
    sb.dir_start = htonl(sb.dir_start);
    sb.dir_blocks = htonl(sb.dir_blocks);

    int numberofDirectories=sb.dir_start*sb.block_size;
    char file[sb.block_size];
    int charSize=sizeof(file);
    //printf("%d\n",numberofDirectories );
    file_exist(f,directory,numberofDirectories,sourcename);

    newFile=fopen(sourcename,"r");
    if(!newFile){
      printf("(%s) source file cannot be found!!!\n",sourcename);
      exit(1);
    }
    fseek(newFile,0,SEEK_END);
    new_filesize=ftell(newFile);

    blocks_needed=new_filesize/sb.block_size;

    if(new_filesize%sb.block_size!=0){
        blocks_needed++;
    }
    fseek(f,sb.fat_start*sb.block_size,SEEK_SET);
    for(i=0;i<sb.num_blocks;i++){
        fread(&free_fat,SIZE_FAT_ENTRY,1,f);
        free_fat=htonl(free_fat);
        if(free_fat==FAT_AVAILABLE){
            free_fat_space++;
        }
    }
      //printf("%d blocks needed\n",blocks_needed );
    //printf("%d\n",free_fat_space*sb.block_size);

      if((free_fat_space*sb.block_size)<new_filesize){
        printf("NOT ENOUGH SPACE FOR %s\n",sourcename);
        exit(1);
      }


      int fat_blocks;
      first_root_block=free_root_block(f,directory,sb);
      first_fat_block=next_free_block(&sb.fat_start,sb.fat_blocks);




      fseek(f,first_root_block,SEEK_SET);

      fwrite(&file_status,1,1,f);

      fseek(f,first_root_block+1,SEEK_SET);
      fwrite(&first_fat_block,1,4,f);

      fseek(f,first_root_block+5,SEEK_SET);
      fwrite(&blocks_needed,1,4,f);

      fseek(f,first_root_block+9,SEEK_SET);
      fwrite(&new_filesize,1,4,f);

      pack_current_datetime(directory.create_time);
      fseek(f,first_root_block+13,SEEK_SET);
      fwrite(&directory.create_time,1,7,f);

      pack_current_datetime(directory.modify_time);
      fseek(f,first_root_block+20,SEEK_SET);
      fwrite(&directory.modify_time,1,7,f);

      fseek(f,first_fat_block+27,SEEK_SET);
      fwrite(&filename,1,31,f);

      fseek(f,first_fat_block+58,SEEK_SET);
      fwrite(&directory._padding,1,6,f);

      int file_fat;
      int file_fat_total;
      int file_content=0;

      file_fat_total=(sb.fat_start*sb.block_size)+(first_fat_block*SIZE_FAT_ENTRY);
      file_fat=first_fat_block*sb.block_size;

      while(new_filesize>sb.block_size){
          new_filesize-=sb.block_size;
          //printf("%d\n",new_filesize );
          fseek(f,file_fat_total,SEEK_SET);
          fwrite(&first_fat_block,1,4,f);

          char *data=malloc(sizeof(char)*sb.block_size);

          fseek(newFile,file_fat,SEEK_SET);
          fread(data,1,sb.block_size,newFile);
          //printf("%s\n",data );
          fseek(f,file_fat,SEEK_SET);
          fwrite(data,1,sb.block_size,f);
          free(data);

          first_fat_block=next_free_block(&sb.fat_start,sb.fat_blocks);

          fseek(f,file_fat_total,SEEK_SET);
          fwrite(&first_fat_block,1,4,f);

          file_fat_total=file_fat+(first_fat_block*SIZE_FAT_ENTRY);
          file_fat=first_fat_block*sb.block_size;
          file_content+=sb.block_size;
      }

      fseek(f,file_fat_total,SEEK_SET);
      fwrite(EOFblock,1,1,f);

      char *lastblock=malloc(sizeof(char)*new_filesize);

      fseek(newFile,file_content,SEEK_SET);
      fwrite(lastblock,1,sb.block_size,newFile);
      fseek(f,file_fat_total,SEEK_SET);
      fwrite(lastblock,1,sb.block_size,f);
      free(lastblock);









    return 0;
}
