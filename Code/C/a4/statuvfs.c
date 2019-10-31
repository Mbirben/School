#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"


int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    FILE  *f;
    int   fat_data;
    int free_blocks=0;
    int reserved_blocks=0;
    int allocated_blocks=0;


    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL)
    {
        fprintf(stderr, "usage: statuvfs --image <imagename>\n");
        exit(1);
    }
        //open the file
        f=fopen(imagename,"r");

        //set the file position and read data
        fseek(f,0,SEEK_SET);
        fread(&sb,sizeof(sb),1,f);

        //print the superblock info
        printf("========SuperBlock information========\n\n");
        printf("%s (%s)\n\n",sb.magic,imagename);
        printf("Block size (Bsz): %d\n",htons(sb.block_size));
        printf("Block count (Bcnt): %d\n",htonl(sb.num_blocks));
        printf("FAT starts (FATst): %d\n",htonl(sb.fat_start));
        printf("FAT blocks (FATcnt): %d\n",htonl(sb.fat_blocks));
        printf("Root directory starts (DIRst): %d\n",htonl(sb.dir_start));
        printf("Root directory blocks (DIRcnt): %d\n",htonl(sb.dir_blocks));

        //find the starting position
        fseek(f, htonl(sb.fat_start) * htons(sb.block_size), SEEK_SET);



        //printf("%d\n",htonl(sb.num_blocks));
        for( i=0; i<htonl(sb.num_blocks); i++){
          //read 4-bytes of data and convert the value to network byte order
          fread(&fat_data,SIZE_FAT_ENTRY,1,f);
          fat_data=htonl(fat_data);
          //printf("%d\n",fat_data);

          //increase the counter based on the return value
          //0x00000000=Free
          //0x00000001=Reserved
          //0xffffffff=Allocated

            if(fat_data==FAT_AVAILABLE){
                free_blocks++;
            }else if(fat_data==FAT_RESERVED){
                reserved_blocks++;
            }else{
                allocated_blocks++;
            }
        }
        //print the FAT information
        printf("\n=====FAT information=====\n");
        printf("Free blocks:  %d\n",free_blocks);
        printf("Reserved blocks: %d\n",reserved_blocks);
        printf("Allocated blocks: %d\n",allocated_blocks);

   return 0;
}
