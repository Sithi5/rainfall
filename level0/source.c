#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int	main(int argc, char **argv)
{
	if (atoi(argv[1]) == 423)
	{
		char *args[2];
		args[0] = strdup("/bin/sh");
		args[1] = 0;
		printf("%s\n", args[0]);
		int gid = getegid();
		int uid = geteuid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);
		int ret = execv("/bin/sh", args);
		printf("%d\n", ret);
	}
	else
		fwrite("No !\n", 1, 5, stderr);
	return (0);
}