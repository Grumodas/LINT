#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LINT.h"

int main() {
	char *result = addLINT("99999999999999999999999999999999999999999999", "1");
	printf("addLINT: %s\n", result);
	free(result);

	result = subLINT("1000000000000000000000000000000000000000000000000", "1");
	printf("subLINT: %s\n", result);
	free(result);

	result = mulLINT("100000000000000000000", "99999999999999999999999999999");
	printf("mulLINT: %s\n", result);
	free(result);

	result = divLINT("2222222222222222222222222", "1111111111111111111111111");
	printf("divLINT: %s\n", result);
	free(result);

	result = modLINT("1111111111111111111111111", "2222222222222222222222222");
	printf("modLINT: %s\n", result);
	free(result);

	result = modLINT("1111111111111111111111111", "2222222222222222222222222");
	printf("modLINT: %s\n", result);
	free(result);

	int bool = isLINTlarger("999999999999999999999", "111111111111111111111");
	printf("isLINTlarger: %d\n", bool);

	bool = isLINTsmaller("999999999999999999999", "111111111111111111111");
	printf("isLINTsmaller: %d\n", bool);

	bool = isLINTequal("999999999999999999999", "111111111111111111111");
	printf("isLINTequal: %d\n", bool);

  return 0;
}
