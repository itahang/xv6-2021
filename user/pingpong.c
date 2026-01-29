#include "kernel/types.h"
#include "user/user.h"
#include "user/custom.h"

int main(int argc, char *argv[])
{

    int p2c[2], c2p[2];

    if (pipe(p2c) < 0 || pipe(c2p) < 0)
    {
        fprintf(2, "failed to create pipe\n");
        exit(1);
    }

    int pid = fork();

    if (pid == 0)
    {

        // Child processX
        close(p2c[WRITE_PIPE]);
        close(c2p[READ_PIPE]);
        char buff[1];

        read(p2c[READ_PIPE], buff, 1);
        close(p2c[READ_PIPE]);

        printf("%d: received ping\n", getpid());


        write(c2p[WRITE_PIPE], buff, 1);
        close(c2p[WRITE_PIPE]);

        exit(0);
    }
    else if (pid > 0)
    {
        // Parent proces
        close(p2c[READ_PIPE]);
        close(c2p[WRITE_PIPE]);

        char buff[1] = "a";

        write(p2c[WRITE_PIPE], buff, 1);
        close(p2c[WRITE_PIPE]);

        read(c2p[READ_PIPE], buff, 1);
        close(c2p[READ_PIPE]);
        
        printf("%d: received pong\n", getpid());

        wait(0);
        exit(0);
    }
}
