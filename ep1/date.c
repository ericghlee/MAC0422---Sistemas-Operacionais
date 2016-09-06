#include <time.h>
#include <stdio.h>

#include "date.h"

void print_date()
{
    time_t timer;
    char buffer[30];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, sizeof (buffer), "%a %b  %d %X %Z %Y", tm_info);
    printf("%s \n", buffer);
}