#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

//Global variables

#define NUMBER_OF_ARGUMENTS 7
#define LENGTH_OF_CHARACTERS 80
#define LENGTH_OF_PROMPT 10
#define NUMBER_OF_DIRECTORIES 10
#define LENGTH_OF_DIRECTORIES 100
#define FILENAME ".sh360rc"

char directories[NUMBER_OF_DIRECTORIES][LENGTH_OF_CHARACTERS];
char  *prompt[LENGTH_OF_PROMPT];
char new_path[LENGTH_OF_DIRECTORIES];
char input[LENGTH_OF_CHARACTERS];
char *token[NUMBER_OF_ARGUMENTS];



int num_dirs;
int length_dir;
int num_dir_path;
int num_tokens;

//appendix_e.c adapted
void tokenize_input(){

    num_tokens=0 ;
    char *t;

    num_tokens = 0;
    t = strtok(input, " ");

    while (t != NULL && num_tokens < NUMBER_OF_ARGUMENTS) {
        prompt[num_tokens] = t;
        num_tokens++;
        t = strtok(NULL, " ");
    }

  //printf("inside tokinize_input\n");
}

//write into an array from .sh360rc
//https://stackoverflow.com/questions/1800295/reading-text-file-into-an-array-of-lines-in-c?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa

void read_sh360rc(){

    FILE *fp=fopen(FILENAME,"r");
    int index;


    while (fgets(directories[index], LENGTH_OF_CHARACTERS, fp) != NULL) {
          if(directories[index][strlen(directories[index]) - 1] == '\n') {
              directories[index][strlen(directories[index]) - 1] = '\0';
          }
          index++;
    }

    //length_dir=index;
    //strcat (directories[0]," ");
    length_dir=index-1;
    //printf("inside read_sh360rc\n" );
    fclose(fp);

}



// check if the file exists
// appendix_f.c adapted

int check_for_file(char *filename){

    struct stat file_stat;
    if(stat(filename,&file_stat)!=0){
      return 0; //not exist
    }else{
      return 1; // exists
    }

    printf("file doesn't exist!!!\n");
    return 0;

}

int find_path(char *cmd) {
      int i;
	   for ( i = 0; i < length_dir; i++) {
		     strcpy(new_path, directories[i]);
		     strcat(new_path, "/");
		     strcat(new_path, cmd);

		     if(check_for_file(new_path)) {
			      return 1;
		     }
	   }

     fprintf(stderr, "%s unknown command\n", cmd);
	   return 0;
}



int main(int argc,char *argv[]){

    char *t;
    int directory_line=0;
    //int num_tokens=0; // global variable
    int num_dir_path;
    int fd;
    char *envp[] = {0};
    int pid;
    int status;
    int i;


    read_sh360rc();


    for(;;){
        fprintf(stdout, "> ");
        fflush(stdout);
        fgets(input,LENGTH_OF_CHARACTERS, stdin);

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }
        tokenize_input();


        if (strcmp(prompt[0], "exit") == 0) { //exit command
            printf("Terminating sh360.c \n");
            exit(EXIT_SUCCESS);
        }else if(strcmp(prompt[0],"PP")==0){
            //printf("%s \n",prompt[2]);
            int num_directory=1;
            for( i=0;i<num_tokens;i++){
                if(strcmp(prompt[i],"->")==0){
                    num_directory++;
                }
            }
            if(num_directory==1){
                fprintf(stderr, "no '->' in PP.\n");
                continue;
            }
             i=0;
            while(strcmp(prompt[i],"->")!=0){
              printf("PIPING IS NOT WORKING\n");
              break;
            }
        }
        else if(strcmp(prompt[0],"OR")==0){
            //printf("%s OR command\n",prompt[0]);
            //printf("%d\n",num_tokens );
           if(strcmp(prompt[num_tokens-2],"->")==0){
             //Consulting office
                if(find_path(prompt[1])){
                    int new_array;
                    for( i=0;i<num_tokens;i++){
                        token[i]=prompt[i];
                        new_array++;
                    }
                    token[0]=new_path;
                    token[new_array]=0;
          //appendix_c adapted
                    if((pid=fork())==0){
                        fd=open(prompt[num_tokens-1],O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
                        if(fd== -1){
                            printf("Cannot open the file!!!\n");
                            exit(1);
                        }
                        dup2(fd,1);
                        dup2(fd,2);
                        execve(token[0],token,envp);
                    }
                    waitpid(pid,&status,0);
                }

            }else{
                printf("OR command is not working(Else statement in OR)\n");
            }
        }else{
            //printf("else statement\n" );

            if(find_path(prompt[0])){
                int i;
                for(i=0;i<num_tokens;i++){
                    token[i]=prompt[i];

                }
              //appendix_c adapted
                token[0]=new_path;
                token[i]=0;
                if((pid==fork())==0){
                  execve(token[0],token,envp);
                }
                waitpid(pid,&status,0);
            }
        }

    }

}
