#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int tmp;

int putstr(char *str)
{
    int i = 0;
    while(str[i])
        write(2, &str[i++], 1);
    return 1;
}

int ft_cd(char **av, int i)
{
    if (i != 2)
        return (putstr("error: cd: bad arguments\n"));
    if (chdir(av[1]))
        return (putstr("error: cd: cannot change directory to ") && putstr(av[1]) && putstr("\n"));
    return (1);
}

int ft_execute(char **av, char **env, int i)
{
    int pip = 0;
    int pid;
    int fd[2];

    if (av[i] && !strcmp(av[i], "|"))
        pip = 1;
    if (pip && pipe(fd) == -1)
        return (putstr("error: fatal\n"));
    pid = fork();
    if (!pid)
    {
        av[i] = 0;
        if (dup2(tmp, 0) == -1 || close(tmp) == -1)
            return (putstr("error: fatal\n"));
        if (pip && (dup2(fd[1], 1) == -1 || close(fd[1]) == -1 || close(fd[0]) == -1))
            return (putstr("error: fatal\n"));
        execve(av[0], av, env);
            return (putstr("error: cannot execute ") && putstr(av[0]) && putstr("\n"));
    }
    if (pip && (dup2(fd[0], tmp) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
        return (putstr("error: fatal\n"));
    if (!pip && (dup2(0, tmp) == -1 || waitpid(pid, NULL, 0) == -1))
        return (putstr("error: fatal\n"));
    return 0;
}

int main(int ac, char **av, char **env)
{
    (void) ac;

    tmp = dup(0);
    int i = 0;
    while(av[i] && av[++i])
    {
        av += i;
        i = 0;
        while(av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
            i++;
        if (!strcmp(av[0], "cd"))
            ft_cd(av, i);
        else if (i)
            ft_execute(av, env, i);
    }
    return 0;
}
