#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naryTree.h"

void initTree(Node *myNode){
   int i = 0;

   for(i = 0; i < NUM_SYMB; i++){
      myNode->code[i] = i;
      myNode->next[i] = NULL;
   }
}

void recycle(Node* myNode){
   int i = 0;
   for(i = 0; i < NUM_SYMB; i++)
   {
      if(myNode->next[i] != NULL){
         recycle(myNode->next[i]);
         free(myNode->next[i]);
      }
   }  
}

void writeBit(FILE* output, unsigned short* buffer, unsigned short prevCode,
   unsigned char c, int* buffMoves, int endF, unsigned short cW, 
   unsigned short validAdd, unsigned short myCode){

   int i = 0;

   c = (char) (prevCode >> cW);
   *buffer = (*buffer) << 8;
   *buffer = (*buffer) | c;
   fprintf(output, "%c", (*buffer >> *buffMoves));

   for(i = cW; i > 0; i--){
      if(*buffMoves == 8){
         fprintf(output, "%c", (*buffer));
         *buffMoves = 0;
      }
      *buffer = (*buffer) << 1;
      *buffer += ((prevCode >> (i-1)) & 0x0001);
      (*buffMoves)++;
   }
   if(*buffMoves == 8){
      fprintf(output, "%c", (*buffer));
      *buffMoves = 0;
   }

}

void writeCode(unsigned short prevCode, FILE* output, unsigned short* buffer
   , int* buffMoves, unsigned short myCode, int endF){
   
   unsigned char c = 0;
   unsigned short cW = 0;
   unsigned short validAdd = 0;

   if(myCode <= 512)
      cW = 1;
   else if(myCode <= 1024)
      cW = 2;
   else if(myCode <= 2048)
      cW = 3;
   else if(myCode <= 4096)
      cW = 4;
   else if(myCode <= 8192)
      cW = 5;
   else if(myCode <= 16384)
      cW = 6;
   else if(myCode <= 32768)
      cW = 7;

   writeBit(output,buffer,prevCode,c,buffMoves,endF,cW,validAdd,myCode);
}

void checkRecycle(unsigned short* myCode, unsigned short limit, 
   Node* myNode, unsigned short prevCode, unsigned short* buffer
   , int* buffMoves, FILE* output, int endF){
   
   writeCode(prevCode, output, buffer, buffMoves, *myCode, endF);

   if(*myCode == limit){
      recycle(myNode);
      initTree(myNode);
      *myCode = 257;
   }
   else
      (*myCode)++;
}

void parseString(FILE* input, FILE* output, Node* dict, unsigned short limit){

   int c = 0;
   Node* currNode = dict;
   unsigned short prevCode = 0;
   unsigned short mycode = 257;
   unsigned short buffer = 0;
   int buffMoves = 0;
   int endF = 0;

   while((c = fgetc(input)) != EOF){
      if(currNode->code[c] == 65535){
         currNode->code[c] = mycode;
         checkRecycle(&mycode, limit, dict, prevCode, &buffer, &buffMoves
            , output, endF);
         currNode = dict;
         
         fseek(input, -1, SEEK_CUR);
      }
      else{
         if(currNode->next[c] == NULL){
            currNode->next[c] = calloc(1 ,sizeof(Node));
            memset(currNode->next[c]->code, 255, 512);
         }
         prevCode = currNode->code[c];
         currNode = currNode->next[c];
      }
   }

   writeCode(prevCode, output, &buffer, &buffMoves, mycode, endF); 
   writeCode(256, output, &buffer, &buffMoves, mycode, endF);
   if(buffMoves != 0)
      fprintf(output, "%c", (buffer));

}
