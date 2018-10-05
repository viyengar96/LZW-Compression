#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "naryTree.h"
#include "send.h"

void exitFail(){
   fprintf(stderr,"Usage: lzwCompress [-rN] file\n");
   fprintf(stderr,"Where: N is a number from 9 to 15 specifying the");
   fprintf(stderr," recycle code as a power of 2\n");
   exit(EXIT_FAILURE);
}

char checkFlag(char* arg){
   long i = 0;
   char* num;
   if(arg[1] != 'r')
      exitFail();
   if(arg[2] == '9')
      return 9;
   num = &(arg[2]);
   i = strtol(num, &num, 10);
   if(i < 10 || i > 15)
      exitFail();
   return (char)i;
}

char checkArgs(int argc, char** argv, int* fileLoc){
   int i = 0;
   char flag = 12;
   int dubflag = 0;
   if(argc > 3 || argc < 2)
      exitFail();

   for(i = 1; i < argc; i++){
      if(argv[i][0] == '-' && !dubflag){
         flag = checkFlag(argv[i]);
         dubflag++;
      }
   }
   if(argc == 2)
      *fileLoc = 1;
   else if(argv[1][0] == '-')
      *fileLoc = 2;
   else
      *fileLoc = 1;
   if((dubflag == 0 && argc == 3) || dubflag == 2 || *fileLoc == 0)
      exitFail();

   return flag;
}

void checkValidInput(FILE* input){
   int c = 0;
   if((c = fgetc(input)) == EOF)
      exit(EXIT_SUCCESS);
   fseek(input, -1, SEEK_CUR);
}

FILE* fileOpen(const char *fname, char* argument){
   FILE *tempF;
   tempF = fopen(fname, argument);
   if(tempF == NULL){
      fprintf(stderr, "lzwCompress: %s: ", fname);
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   return tempF;
}

int main(int argc, char** argv){
   FILE* input;
   FILE* output;
   char recycleCode = 12;
   int fileLoc = 0;
   unsigned short limit = 0;
   Node* myNode;
   
   recycleCode = checkArgs(argc, argv, &fileLoc);
   input = fileOpen(argv[fileLoc], "r");
   output = fileOpen(strcat(argv[fileLoc], ".lzw"), "w");

   checkValidInput(input);

   fprintf(output, "%c", recycleCode);
   
   limit = pow(2.0, (double)recycleCode);
 
   myNode = (Node*)calloc(1, sizeof(Node));
   initTree(myNode);

   parseString(input, output, myNode, limit);
   
   recycle(myNode);
   free(myNode);
   fclose(input);
   fclose(output);

   return 0;
}
