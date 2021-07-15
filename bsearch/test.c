#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mi {
    int nr;
    char *name;
} months[] = {
    { 1, "jan" }, { 2, "feb" }, { 3, "mar" }, { 4, "apr" },
    { 5, "may" }, { 6, "jun" }, { 7, "jul" }, { 8, "aug" },
    { 9, "sep" }, {10, "oct" }, {11, "nov" }, {12, "dec" }
};

#define nr_of_months (sizeof(months)/sizeof(months[0]))

static int
compmi(const void *m1, const void *m2)
{
    struct mi *mi1 = (struct mi *) m1;
    struct mi *mi2 = (struct mi *) m2;
    return strcmp(mi1->name, mi2->name);
}

int
main(int argc, char **argv)
{
    int i;

    qsort(months, nr_of_months, sizeof(struct mi), compmi);
    for (i = 1; i < argc; i++) {
        struct mi key, *res;
        key.name = argv[i];
        res = bsearch(&key, months, nr_of_months,
                      sizeof(struct mi), compmi);
        if (res == NULL)
            printf("'%s': unknown month\n", argv[i]);
        else
            printf("%s: month #%d\n", res->name, res->nr);
    }
    exit(EXIT_SUCCESS);
}
