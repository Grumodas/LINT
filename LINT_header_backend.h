#ifndef LINT_header
#define LINT_header

typedef struct digit{
  short value;
  struct digit *prev;
  struct digit *next;
} digit;

digit* createLINT(char *string); 
void freeLINT (digit *head);
char* addLINTsecret(char *string1, char *string2, int sign);
char* LINTToString(digit **list_head, int sign);
char* subLINTSecret(char *string1, char *string2, int sign);
int isLargerAbsoluteValue(char *string1, char *string2);
int isListLargerOrEqual(digit* LINT_list1, digit* LINT_list2);
void subLongList(digit* LINT_list1, digit* LINT_list2);
int getSign(char *string);
digit* incLinkList(digit* LINT_list);
digit *addLINTsecretList(digit *LINT1, digit *LINT2);
void addZeroes(digit *LINT_head);
digit *copyLinkedList(digit *linked_list_head);

#endif
