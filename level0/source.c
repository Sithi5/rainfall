#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int ac, char **av)
{
    char *str[2];
    gid_t gid;
    uid_t uid;

    if (423 == atoi(av[1]))
    {
        str[0] = strdup("/bin/sh");
        str[1] = 0;
        gid = getegid();
        uid = geteuid();
        setresgid(gid, gid, gid);
        setresuid(uid, uid, uid);
        execv("/bin/sh", str)
    }
    else
    {
        fwrite("No !\n", 1, 5, stderr);
    }
    return 0;
}