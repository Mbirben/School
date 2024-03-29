#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"


char *month_to_string(short m) {
    switch(m) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "?!?";
    }
}


void unpack_datetime(unsigned char *time, short *year, short *month,
    short *day, short *hour, short *minute, short *second)
{
    assert(time != NULL);

    memcpy(year, time, 2);
    *year = htons(*year);

    *month = (unsigned short)(time[2]);
    *day = (unsigned short)(time[3]);
    *hour = (unsigned short)(time[4]);
    *minute = (unsigned short)(time[5]);
    *second = (unsigned short)(time[6]);
}


int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    directory_entry_t directory;
    int  i;
    char *imagename = NULL;
    FILE *f;
    short year, month, day, hour, minute, second;


    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL)
    {
        fprintf(stderr, "usage: lsuvfs --image <imagename>\n");
        exit(1);
    }
    //open the file and set the position
    f = fopen(imagename, "r");
    fseek(f, 0, SEEK_SET);
    fread(&sb, sizeof(sb), 1, f);

    //find the directory start position

    int numberofDirectories=htonl(sb.dir_start)*htons(sb.block_size);
    //printf("%d\n",numberofDirectories );
    fseek(f,numberofDirectories,SEEK_SET);



    //each directory entry is 64-bytes in size
    for(i=0;i<numberofDirectories;i+=64){
        //read the directory info
        fread(&directory,sizeof(unsigned char),64,f);

        //if the file status is normal file then process and print the file information
        if(directory.status==DIR_ENTRY_NORMALFILE){
          //printf("%d\n",directory.status);
            unpack_datetime(directory.create_time,&year, &month, &day, &hour, &minute, &second);
            printf("%8d %d/%s/%d %d:%d:%d %s\n",htonl(directory.file_size),
                 year,month_to_string(month),day,hour,minute,second,directory.filename);

        }

        i+=64;
    }
    return 0;
}
