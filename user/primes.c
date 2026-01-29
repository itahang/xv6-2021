#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include "user/custom.h"

void worker(int fd) 
{

  int tmp;
  int num;
  int p2c[2];
  if (read(fd, &num, sizeof(int)) == 0)
  {
    return;
  }
  printf("prime %d\n", num);

  pipe(p2c);
  int pid = fork();

  if (pid > 0)
  {
    close(p2c[READ_PIPE]);
    while (read(fd, &tmp, sizeof(int)) != 0)
    {
      if (tmp % num != 0)
      {
        write(p2c[WRITE_PIPE], &tmp, sizeof(int));
      }
    }
    close(fd); 
    close(p2c[WRITE_PIPE]);
    wait(0);
  }
  else
  {
    close(p2c[WRITE_PIPE]);
    worker(p2c[READ_PIPE]);
    exit(0);
  }
}

int main(int argc, char *argv[])
{
  int p2c[2];
  pipe(p2c);

  int pid = fork();
  if (pid > 0)
  {
    

    close(p2c[READ_PIPE]);
    for (int i = 2; i <= 35; i++)
    {
      write(p2c[WRITE_PIPE], &i, sizeof(int));
    }
    close(p2c[WRITE_PIPE]);
    wait(0);
  }
  else
  {
    close(p2c[WRITE_PIPE]);
    worker(p2c[READ_PIPE]);
  }

  exit(0);
}
