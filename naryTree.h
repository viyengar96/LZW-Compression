#ifndef NARYTREE_H
   #define NUM_SYMB 256
   typedef unsigned short Code;
   
   typedef struct Node
   {
      Code code[NUM_SYMB];
      struct Node* next[NUM_SYMB];
   } Node;

   FILE* getFile(char* fileName);
   void recycle(Node* myNode);
#endif
