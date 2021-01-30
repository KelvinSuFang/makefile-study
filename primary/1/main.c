
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "shell.h"

void main(void)
{
    printf_welcome();
    shell(1);
}
