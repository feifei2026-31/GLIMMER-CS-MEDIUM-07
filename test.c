#include <stdio.h>
#include <sys/types.h>

void parse_permissions(mode_t mode, char *perms) {
    char rwx[] = {'r', 'w', 'x'};
    mode &= 0777;
    for (int i = 0; i < 9; i++)
        perms[i] = (mode & (0400 >> i)) ? rwx[i % 3] : '-';
    perms[9] = '\0';
}

int main(void) {
    mode_t tests[] = {0755, 0644, 0754, 0700, 0000, 0777, 0100644};
    char buf[11];
    for (int i = 0; i < 7; i++) {
        parse_permissions(tests[i], buf);
        printf("%07o  ->  %s\n", (unsigned)tests[i], buf);
    }
    return 0;
}