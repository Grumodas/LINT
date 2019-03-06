#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LINT.h"
#include "LINT_header_backend.h"

digit *createLINT(char *string) {
    char single_char = *string;
    if (single_char != '-' && (single_char < '0' || single_char > '9'))
    {
      printf("Please enter an integer number next time. Exiting...\n");
      exit(-2);
    }

    for (int i = 1; *(string + i) != '\0'; i++)
    {
      single_char = *(string + i);
      if (single_char < '0' || single_char > '9') {
        printf("Please enter a integer number next time. Exiting...\n");
        exit(-2);
      }
    }

  int num;
  digit* current;
  digit* last;
  digit* head = (digit*) malloc (sizeof(digit));
  if (head == NULL) {
    printf("---- Incorrect memory allocation -----\n");
    exit(-1);
  }

  single_char = *string;
  // num = atoi(&single_char);
  if(single_char == '-'){   
    num = 0;                
  }
  else {
    num = single_char - '0';  
  }                         
  current = head;
  current->value = num;
  current->prev = NULL;
  current->next = NULL;

  for (int i = 1; i < strlen(string); i++) {
    single_char = *(string + i);
    // num = atoi(&single_char);
    num = single_char - '0';
    last = current;
    current->next = (digit*) malloc (sizeof(digit));
    if (current->next == NULL) {
      printf("---- Incorrect memory allocation in createLINT-----\n");
      exit(-1);
    }

    current = current->next;
    current->value = num;
    last->next = current;
    current->prev = last;
  }

  current->next = NULL;

  while (head->value == 0 && head->next != NULL) {
    head = head->next;
    free(head->prev);
    head->prev = NULL;
  }

  return head;
}


void freeLINT (digit *head) {
  digit *current = head;
  head = head->next;

  while (head != NULL) {
    free(current);
    current = head;
    head = head->next;
  }
  free(current->next);
  free(current);

  // digit *current = head;

  // while (head != NULL) {
  //   head = head->next;
  //   free(current);
  //   current = head;
  // }

}


char* addLINTsecret(char *string1, char *string2, int sign) {
  digit *LINT1_head = createLINT(string1);
  digit *LINT2_head = createLINT(string2);
  digit *LINT1 = LINT1_head;
  digit *LINT2 = LINT2_head;
  int LINT1_len = 1;
  int LINT2_len = 1;
  digit *longer;
  digit *longer_head;
  digit *shorter;
  digit *shorter_head;
  char *ptr_LINT_sum;
  int carry_flag = 0;

  //string'u ilgiai
  while (LINT1->next != NULL){
    LINT1 = LINT1->next;
    LINT1_len++;
  }
  while (LINT2->next != NULL){
    LINT2 = LINT2->next;
    LINT2_len++;
  }

  // kuris ilgesnis?
  if (LINT1_len > LINT2_len) {
    longer = LINT1;
    shorter = LINT2;
    longer_head = LINT1_head;
    shorter_head = LINT2_head;
  }
  else {
    longer = LINT2;
    shorter = LINT1;
    longer_head = LINT2_head;
    shorter_head = LINT1_head;
  }

  // prg sudeties ciklas. Dedam nuo desines i kaire
  while (shorter != NULL || carry_flag == 1) {
    if (carry_flag) {
      if (longer == NULL) {
        longer = (digit*) malloc (sizeof(digit));
        if (longer == NULL) {
          printf("---- Incorrect memory allocation -----\n");
          exit(-1);
        }
        longer->next = longer_head;
        longer_head->prev = longer;
        longer_head = longer;
        longer->value = 0; //nes pradzioj buna undefined
      }

      longer->value += 1;
    }

    if (shorter != NULL) {
      longer->value += shorter->value;
      shorter = shorter->prev;
    }

    if (longer->value > 9) {
      longer->value -= 10;
      carry_flag = 1;
    }
    else {
      carry_flag = 0;
    }

    longer = longer->prev;

  }

  ptr_LINT_sum = LINTToString(&longer_head, sign);

  freeLINT(longer_head);
  freeLINT(shorter_head);

  return ptr_LINT_sum;
}

char* LINTToString(digit **list_head, int sign){
  char *ptr_char_array;
  digit *list_current;
  int char_array_len = 0;
  int i = 0;

  // Istrinu nereikalingus nulius, bet jei visas sk == 0, tai palieku viena nuli.
  while((*list_head)->value == 0 && (*list_head)->next != NULL){

    (*list_head) = (*list_head)->next;
    free((*list_head)->prev);
    (*list_head)->prev = NULL;
  }
  list_current = *list_head;

  while (list_current != NULL){
    list_current = list_current->next;
    char_array_len++;
  }
  list_current = *list_head;

  if ((*list_head)->value == 0) {
    sign = 0;
  }

  // minusui irgi reiks atminties
  if(sign){
     char_array_len++;
  }
  char_array_len++;

  ptr_char_array = (char*) malloc(char_array_len * sizeof(char));
  if (ptr_char_array == NULL) {
    printf("Error allocating memory in LINTToString. Goodbye\n");
    exit(-10);
  }

  if (sign){
    *ptr_char_array = '-';
    i++;
  }

  for(; i < char_array_len - 1; i++){
    *(ptr_char_array + i) = list_current->value + '0';
    list_current = list_current->next;
  }
  *(ptr_char_array + i) = '\0';
  return ptr_char_array;
}

char* addLINT(char *string1, char *string2){
  int sign; //abieju string'u ir rezultato sign'ai. 0 - teigiamas, 1 - neigiamas
  char *ptr_long_sum;

  //patikrinu ar pirmas char yra minusas
  int sign1 = getSign(string1);
  int sign2 = getSign(string2);

  // nusprendziu kaip elgtis su vadinama sudetim ir issiaiskinu ar reiks minusa prirasign2t gale ir ar realiai reik sudet ar atimt
  if(sign1 == 0 && sign2 == 0){
    sign = 0;
    addLINTsecret(string1, string2, sign); // paduodu s, kad zinociau ar jau paskaiciavus reikia irasign2ti '-' i rezultata. Be to tikraAddFunkcija, tai ta, kur mes pasirasem jau
  }
   else if(sign1 != sign2){
     if (isLINTlarger(string1, string2)){
       sign = 0;
     }
     else{
       sign = 1;
     }
     subLINTSecret(string1, string2, sign);
   }
  else{ // cia bus jei sign1 == 1 && sign2 == 1
    sign = 1;
    addLINTsecret(string1, string2, sign);
  }

}


char* subLINT(char *string1, char *string2){
  int sign; //abieju string'u ir rezultato sign'ai. 0 - teigiamas, 1 - neigiamas
  char *ptr_long_sum;

  int sign1 = getSign(string1);
  int sign2 = !getSign(string2);

  // nusprendziu kaip elgtis su vadinama sudetim ir issiaiskinu ar reiks minusa prirasign2t gale ir ar realiai reik sudet ar atimt
  if(sign1 == 0 && sign2 == 0){ //int1 teig, int2 neig
    sign = 0;
    ptr_long_sum = addLINTsecret(string1, string2, sign); // paduodu s, kad zinociau ar jau paskaiciavus reikia irasign2ti '-' i rezultata. Be to tikraAddFunkcija, tai ta, kur mes pasirasem jau
  } else if(sign1 != sign2){ //ženklai sutampa
    // if (isLINTlarger(string2, string1)) {
    //     if (isLINTequal(string1, string2)) {
    //     sign = 0;
    //   } else {
    //     sign = 1;
    //   }
    // } else if (isLINTlarger(string1, string2)) {
    //   sign = 0;
    // } else {
    //   sign = 0;
    // }
      if (isLINTlarger(string2, string1)) {
        sign = 1;
      } else {
        sign = 0;
      }
      subLINTSecret(string1, string2, sign);
    }
  else { // int1 neig, int2 teig
      sign = 1;
      ptr_long_sum = addLINTsecret(string1, string2, sign);
  }

}


char* subLINTSecret(char *string1, char *string2, int sign) {
  digit *LINT1_head = createLINT(string1);
  digit *LINT2_head = createLINT(string2);
  digit *LINT1 = LINT1_head;
  digit *LINT2 = LINT2_head;
  int LINT1_len = 1;
  int LINT2_len = 1;
  digit *longer;
  digit *longer_head;
  digit *shorter;
  digit *shorter_head;
  int carry_flag = 0;

  LINT1 = LINT1_head;
  LINT2 = LINT2_head;

  while (LINT1->next != NULL) {
    LINT1 = LINT1->next;
    LINT1_len++;
  }

  while (LINT2->next != NULL) {
    LINT2 = LINT2->next;
    LINT2_len++;
  }

  //sube reikia sudėtingesnio ilgio tikrinimo, nes čia ne tik svarbu kas ilgesnis, bet ir kuris didesnis, nes norim visada iš didesnio atimti mažesnį.
  //gaunasi trys atvejai: kai vienas ilgesnis, kitas ilgesnis, ir kai abiejų ilgiai lygūs
  if (LINT1_len > LINT2_len) {
    longer = LINT1;
    shorter = LINT2;
    longer_head = LINT1_head;
    shorter_head = LINT2_head;
  }
  else if (LINT1_len < LINT2_len) {
    longer = LINT2;
    shorter = LINT1;
    longer_head = LINT2_head;
    shorter_head = LINT1_head;
  }
  else {  //abiejų ilgiai lygūs
    digit *temp1 = LINT1_head; //kad nereiktų atliktus skaičiavimus eiti vėl iki linked listo galo, susikuriame temp kintamuosius.
    digit *temp2 = LINT2_head;

    while (temp1 != NULL) { //iteruojam per linked listą nuo pradžioj link galo, tol, kol randame kad kažkuri iš reikšmių didesnė, ją padarom longer ir breakinam
      //not pretty but it works
      if (temp1->value > temp2->value) {
        longer = LINT1;
        shorter = LINT2;
        longer_head = LINT1_head;
        shorter_head = LINT2_head;
        break;
      } else if (temp1->value < temp2->value) {
        longer = LINT2;
        shorter = LINT1;
        longer_head = LINT2_head;
        shorter_head = LINT1_head;
        break;
      }

      temp1 = temp1->next;
      temp2 = temp2->next;
    }

    if (temp1 == NULL) { //gavosi kad praėjom visą linked listą ir abu skaičiai visiškai identiški. tada renkamės bet kurį
      longer = LINT1;
      shorter = LINT2;
      longer_head = LINT1_head;
      shorter_head = LINT2_head;
    }
  }

  //pati atimtis
  while (shorter != NULL || carry_flag == 1) {
    //jeigu iš dabartinės reikšmės buvo pasiskolinta, patikrinam ar reikšmė 0. Jei taip, padarom ją 9 ir paliekam flagą, jei ne, tiesiog atimam tą paskolintą vienetą.
    if (carry_flag == 1) {
      if (longer->value == 0) {
        longer->value += 10;
        carry_flag = 1;
      } else {
        carry_flag = 0;
      }

      longer->value -= 1;
    }

  //jei dar nepasiekėm shorto galo, atimam jo reikšmę ir atitinkamai setinam flagą jeigu reikėjo skolintis
  if (shorter != NULL) {
    if (longer->value < shorter->value) {
      longer->value += 10;
      carry_flag = 1;
    }

    longer->value -= shorter->value;
    shorter = shorter->prev;
  }

  longer = longer->prev;
  }

  char * LINTString = LINTToString(&longer_head, sign);

  freeLINT(longer_head);
  freeLINT(shorter_head);

  return LINTString;
}

int isLargerAbsoluteValue(char *string1, char *string2) {
  digit *LINT1_head = createLINT(string1);
  digit *LINT2_head = createLINT(string2);
  digit *LINT1 = LINT1_head;
  digit *LINT2 = LINT2_head;

  int LINT1_len = 0;
  int LINT2_len = 0;
  digit *longer_head;

  while (LINT1->next != NULL) {
    LINT1 = LINT1->next;
    LINT1_len++;
  }

  while (LINT2->next != NULL) {
    LINT2 = LINT2->next;
    LINT2_len++;
  }

  if (LINT1_len > LINT2_len) {
    freeLINT(LINT1_head);
    freeLINT(LINT2_head);
    return 1;
  } else if (LINT1_len < LINT2_len) {
    freeLINT(LINT1_head);
    freeLINT(LINT2_head);
    return 0;
      } else {                                //abiejų ilgiai lygūs
    digit *temp1 = LINT1_head; //kad nereiktų atliktus skaičiavimus eiti vėl iki linked listo galo, susikuriame temp kintamuosius.
    digit *temp2 = LINT2_head;

    while (temp1 != NULL) { //iteruojam per linked listą nuo pradžioj link galo, tol, kol randame kad kažkuri iš reikšmių didesnė, ją padarom longer ir breakinam
      //not pretty but it works
      if (temp1->value > temp2->value) {
        freeLINT(LINT1_head);
        freeLINT(LINT2_head);
        return 1;
      } else if (temp1->value < temp2->value) {
        freeLINT(LINT1_head);
        freeLINT(LINT2_head);
        return 0;
      }

      temp1 = temp1->next;
      temp2 = temp2->next;
    }

    if (temp1 == NULL) { //gavosi kad praėjom visą linked listą ir abu skaičiai visiškai identiški
      free(LINT1_head);
      free(LINT2_head);
      return 0;
    }
  }
}

int isLINTlarger (char *string1, char *string2) {
  int sign1 = getSign(string1);
  int sign2 = getSign(string2);

  if (!sign1 && !sign2) {
    return isLargerAbsoluteValue(string1, string2);\
  }
  else if (sign1 && sign2) {
    return !isLargerAbsoluteValue(string1, string2);\
  }
  else if (sign1 && !sign2) {
    return 0;
  } else {
    return 1;
  }
}


int getSign(char *string){
    if (*string == '-')
    {
      return 1;
    }
    return 0;
}

int isLINTequal(char *string1, char *string2){
    int sign1 = getSign(string1);
    int sign2 = getSign(string2);

    if (sign1 != sign2) {
      return 0;
    }

    digit *LINT1_head = createLINT(string1);
    digit *LINT2_head = createLINT(string2);
    digit *LINT1 = LINT1_head;
    digit *LINT2 = LINT2_head;

    int LINT1_len = 0;
    int LINT2_len = 0;
    digit *longer_head;

    while (LINT1->next != NULL)
    {
      LINT1 = LINT1->next;
      LINT1_len++;
    }

    while (LINT2->next != NULL)
    {
      LINT2 = LINT2->next;
      LINT2_len++;
    }

    if (LINT1_len != LINT2_len) {
      freeLINT(LINT1_head);
      freeLINT(LINT2_head);
      return 0;
    } else {                                //abiejų ilgiai lygūs
    LINT1 = LINT1_head; //kad nereiktų atliktus skaičiavimus eiti vėl iki linked listo galo, susikuriame temp kintamuosius.
    LINT2 = LINT2_head;

    while (LINT1 != NULL) { //iteruojam per linked listą nuo pradžioj link galo, tol, kol randame kad kažkuri iš reikšmių didesnė, ją padarom longer ir breakinam
      //not pretty but it works
      if (LINT1->value != LINT2->value) {
        freeLINT(LINT1_head);
        freeLINT(LINT2_head);
        return 0;
      }

      LINT1 = LINT1->next;
      LINT2 = LINT2->next;
    }

  freeLINT(LINT1_head);
  freeLINT(LINT2_head);
  return 1;
  }
}

 // mod

// div'as nuo cia
int isListLargerOrEqual(digit* LINT_list1, digit* LINT_list2){
  int list_len1 = 0;
  int list_len2 = 0;
  digit *LINT_list_head1 = LINT_list1;
  digit *LINT_list_head2 = LINT_list2;
  int zf = 1;

  // ilgiai
  while (LINT_list1 != NULL){
    if(LINT_list1->value != 0){
      zf = 0;
    }
    if(!zf){
      list_len1++;
    }
    LINT_list1 = LINT_list1->next;
  }
  zf = 1;
  while (LINT_list2 != NULL) {
    if(LINT_list2->value != 0){
      zf = 0;
    }
    if(!zf){
      list_len2++;
    }
    LINT_list2 = LINT_list2->next;
  }

  //palyginimas pagal realu ilgi(atmetus nulius)
  if(list_len1 > list_len2){
    return 1;
  }
  if(list_len1 < list_len2){
    return 0;
  }

  LINT_list1 = LINT_list_head1;
  LINT_list2 = LINT_list_head2;

  //nustumimas i realia head(nes atimant gali susidaryti nereiksminiu nuliu)
  while (LINT_list1->next != NULL){
    if(LINT_list1->value != 0){
      break;
    }
    LINT_list1 = LINT_list1->next;
  }
  while (LINT_list2->next != NULL) {
    if(LINT_list2->value != 0){
      break;
    }
    LINT_list2 = LINT_list2->next;
  }

  // tikrinimas pagal reiksme
  while(LINT_list1 != NULL){
    if(LINT_list1->value < LINT_list2->value){
      return 0;
    }
    else if(LINT_list1->value > LINT_list2->value){
      return 1;
    }
    LINT_list1 = LINT_list1->next;
    LINT_list2 = LINT_list2->next;
  }

  return 1;
}

void subLongList(digit* LINT_list1, digit* LINT_list2){
  int cf = 0;

  // nueinu i gala
  while(LINT_list1->next != NULL){
    LINT_list1 = LINT_list1->next;
  }
  while(LINT_list2->next != NULL){
    LINT_list2 = LINT_list2->next;
  }

  // atimtis. cf - carry flag, zymi pasiskolinima
  while(LINT_list1 != NULL && LINT_list2 != NULL || cf){
    if(cf){
      LINT_list1->value -= 1;
      cf = 0;
    }
    if(LINT_list2 != NULL){
        LINT_list1->value -= LINT_list2->value;
    }

    if(LINT_list1->value < 0){
      LINT_list1->value += 10;
      cf = 1;
    }

    if(LINT_list1 != NULL){
      LINT_list1 = LINT_list1->prev;
    }
    if(LINT_list2 != NULL){
      LINT_list2 = LINT_list2->prev;
    }
  }
}

digit* incLinkList(digit* LINT_list){

  digit* LINT_list_head = LINT_list;
  int cf = 0;

  while (LINT_list->next != NULL){
    LINT_list = LINT_list->next;
  }

  LINT_list->value++;

  if (LINT_list->value == 10){
    LINT_list->value = 0;
    cf = 1;
  }

  while (cf) {
    if (LINT_list->prev == NULL) {
      LINT_list->prev = (digit*) malloc (sizeof(digit));
      if (LINT_list->prev == NULL) {
        printf("---- Incorrect memory allocation -----\n");
        exit(-1);
      }

      LINT_list_head = LINT_list->prev;
      LINT_list_head->next = LINT_list;
      LINT_list_head->value = 0;
      LINT_list_head->prev = NULL;
    }

    LINT_list = LINT_list->prev;
    LINT_list->value++;
    cf = 0;

    if (LINT_list->value == 10){
      LINT_list->value = 0;
      cf = 1;
    }
  }

  return LINT_list_head;
}

char *divLINT(char *string1, char *string2){
  if(isLINTequal(string2, "0")){
    printf("Division by zero. Exiting.\n");
    exit(-3);
  }

  int sign1 = getSign(string1);
  int sign2 = getSign(string2);
  int sign;

  if (sign1 != sign2){
    sign = 1;
  }
  else{
    sign = 0;
  }

  char *result = "0";
  digit *int_list1_head = createLINT(string1);
  digit *int_list2_head = createLINT(string2);
  digit *result_list_head = createLINT(result);

  digit *int_list1 = int_list1_head;
  digit *int_list2 = int_list2_head;
  digit *result_list = result_list_head;

  while (isListLargerOrEqual(int_list1, int_list2)){
    subLongList(int_list1, int_list2);
    result_list_head = incLinkList(result_list_head);
  }

  result = LINTToString(&result_list_head, sign);

  freeLINT(int_list1_head);
  freeLINT(int_list2_head);
  freeLINT(result_list_head);

  return result;
}

char *modLINT(char *string1, char *string2){
  if(isLINTequal(string2, "0")){
    printf("Modulus by zero. Exiting.\n");
    exit(-3);
  }

  int sign1 = getSign(string1);
  int sign2 = getSign(string2);
  int sign;

  if (sign1 != sign2){
    sign = 1;
  }
  else{
    sign = 0;
  }

  char *result = "0";
  // char *lchar = "0";
  digit *int_list1 = createLINT(string1);
  digit *int_list2 = createLINT(string2);
  // digit *result_list = createLINT(result);

  while (isListLargerOrEqual(int_list1, int_list2)){
    subLongList(int_list1, int_list2);
    // result_list = incLinkList(result_list);
  }

  result = LINTToString(&int_list1, sign);

  freeLINT(int_list1);
  freeLINT(int_list2);
  //freeLINT(result_list);

  return result;
}


char* mulLINT(char *string1, char *string2) {
  int sign1 = getSign(string1);
  int sign2 = getSign(string2);
  
  digit *LINT1_head;
  digit *LINT2_head;
  if (isLINTlarger(string2, string1)) {
		LINT1_head = createLINT(string2);
		LINT2_head = createLINT(string1);
  } else {
  	LINT1_head = createLINT(string1);
  	LINT2_head = createLINT(string2);
  }
  
  digit *LINT1 = LINT1_head;
  digit *LINT2 = LINT2_head;
  int sign;
  digit *sum1 = createLINT("0");
  digit *sum2 = createLINT("0");
  digit *temp;
  
  
  if (sign1 != sign2) {
    sign = 1;
  } 
  else {
    sign = 0;
  }

  /*
  nueinam į LINT2 galą ir paimam value
  į sum1 su for ciklu sudedam LINT1 tiek kartų kol value = 0
  laikom counterį i = 0 kiek nulių pridėt
  paeinam LINT2 į kairę ir vėl paimam value ir sudedam viską į sum2
  pridedam gale 0, sudedam su sum1 ir išsaugom sum1 += sum2
  kartojam kol LINT2->prev != NULL
  */

  //einam LINT2 į galą
  while (LINT2->next != NULL) {
    LINT2 = LINT2->next;
  }

  //kiek nulių pridėt
  int d = 0;

  while (LINT2 != NULL) {

    for (int i = 1; i <= LINT2->value; i++) {
      temp = sum1;
      sum1 = addLINTsecretList(sum1, LINT1);
      freeLINT(temp);
    }

    if (d) {

      for (int i = 0; i < d; i++) {
        addZeroes(sum1);
      }
      d++;
    } else {
      d++;
    }

    temp = sum2;
    sum2 = addLINTsecretList(sum1, sum2);
    freeLINT(sum1);
    freeLINT(temp);
    sum1 = createLINT("0");
    LINT2 = LINT2->prev;
  }

  digit *sum2_head = sum2;

  while (sum2->next != NULL) {
    sum2 = sum2->next;
  }

  char *finalResults = LINTToString(&sum2_head, sign);

  freeLINT(LINT1_head);
  freeLINT(LINT2_head);
  freeLINT(sum1);
  freeLINT(sum2_head);
  return finalResults;
}


digit *addLINTsecretList(digit *LINT1, digit *LINT2) {
  digit *LINT1_head = LINT1;
  digit *LINT2_head = LINT2;
  int LINT1_len = 1;
  int LINT2_len = 1;
  int carry_flag = 0;
  digit *longer;
  digit *longer_head;
  digit *shorter;
  digit *shorter_head;
  //string'u ilgiai
  while (LINT1->next != NULL) {
    LINT1 = LINT1->next;
    LINT1_len++;
  }

  while (LINT2->next != NULL) {
    LINT2 = LINT2->next;
    LINT2_len++;
  }

  // kuris ilgesnis?
  if (LINT1_len > LINT2_len) {
    digit *result_head = copyLinkedList(LINT1_head);
    digit *result = result_head;
    longer = result;
    shorter = LINT2;
    longer_head = result_head;
    shorter_head = LINT2_head;
  }
  else {
    digit *result_head = copyLinkedList(LINT2_head);
    digit *result = result_head;
    longer = result;
    shorter = LINT1;
    longer_head = result_head;
    shorter_head = LINT1_head;
  }

  //važiuojam su longer į galą nes reiks nuo galo sudėt
  while (longer->next != NULL) {
    longer = longer->next;
  }

  // prg sudeties ciklas. Dedam nuo desines i kaire
  while (shorter != NULL || carry_flag == 1) {
    if (carry_flag) {
      if (longer == NULL) {
        longer = (digit *)malloc(sizeof(digit));
        if (longer == NULL) {
          printf("---- Incorrect memory allocation in addLINTsecretList-----\n");
          exit(-1);
        }

        longer->next = longer_head;
        longer_head->prev = longer;
        longer_head = longer;
        longer->value = 0; //nes pradzioj buna undefined
      }

      longer->value += 1;
    }

    if (shorter != NULL)
    {
      longer->value += shorter->value;
      shorter = shorter->prev;
    }

    if (longer->value > 9)
    {
      longer->value -= 10;
      carry_flag = 1;
    }
    else
    {
      carry_flag = 0;
    }

    longer = longer->prev;
  }



  return longer_head;
}

void addZeroes(digit *LINT_head) {
  digit *LINT_current = LINT_head;

  while (LINT_current->next != NULL) {
    LINT_current = LINT_current->next;
  } 

  LINT_current->next = (digit*) malloc(sizeof(digit));
  if (LINT_current->next == NULL) {
    printf("Memory error in function addZeroes\n");
    exit(-7);
  }

  (LINT_current->next)->prev = LINT_current;
  LINT_current = LINT_current->next;
  LINT_current->value = 0;
  LINT_current->next = NULL;
}

//copied linked list. If source linked list is empty, return it's head;
digit *copyLinkedList (digit *linked_list_head) {
  if (linked_list_head == NULL) {
    return linked_list_head;
  }

  digit *copy_head = (digit*) malloc(sizeof(digit));
  if (copy_head == NULL) {
    printf("error allocating memory in function copyLinkedList\n");
    exit(-6);
  }
  copy_head->value = linked_list_head->value;
  copy_head->prev = NULL;
  copy_head->next = NULL;

  digit *copy_current = copy_head;
  digit *linked_list_current = linked_list_head;

  while (linked_list_current->next != NULL) {
    copy_current->next = (digit*) malloc (sizeof(digit));
    if (copy_current == NULL) {
      printf("error allocating memory in function copyLinkedList\n");
      exit(-6);   
    }

    (copy_current->next)->prev = copy_current;
    copy_current = copy_current->next;
    linked_list_current = linked_list_current->next;
    copy_current->value = linked_list_current->value;
  }

  copy_current->next = NULL;
  return copy_head;
}

int isLINTsmaller(char *string1, char *string2) {
  return (!isLINTlarger(string1, string2) && !isLINTequal(string1, string2));
}


void test(char *a, char *b) {
  digit *int1_head = createLINT(a);
  digit *int2_head = createLINT(b);
  digit *result_list_head = createLINT("0");
  digit *int1 = int1_head;
  digit *int2 = int1_head;
  digit *result_list = result_list_head;

  isListLargerOrEqual(int1, int2);
  subLongList(int1, int2);
  result_list = incLinkList(int1);

  freeLINT(int1_head);
  freeLINT(int2_head);
  freeLINT(result_list_head);
}