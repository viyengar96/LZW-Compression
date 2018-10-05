#ifndef SEND_H
   #define SEND_H

   void initTree(Node* myNode);
   void parseString(FILE* input, FILE* output, Node* myNode, 
      unsigned short limit);
   void printTrie(Node* myNode);
#endif
