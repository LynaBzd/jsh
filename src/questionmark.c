#include "questionmark.h"
#include "utils.h"
#include "colors.h"


int cmd_questionmark(int prevalr) {
    char valrs[MAX_INT_STRLEN];
    itos(valrs, prevalr);
    write(STDOUT_FILENO, valrs, strlen(valrs));
    write(STDOUT_FILENO, "\n", 1);

    return 0;
}
