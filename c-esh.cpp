#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iterator>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <csignal>
#include <termios.h>

#include "c-esh.hpp"
#include "utils.hpp"

std::vector<DataStructures::Job> global_job_list;
pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;

int main()
{
    // https://www.tutorialspoint.com/unix/unix-signals-traps.htm
    // https://www.gnu.org/software/libc/manual/html_node/Job-Control-Signals.html
    // For the shell itself, ignore  SIGINT, SIGTSTP
    // Ignore if a process in background accesses terminal?
    signal (SIGINT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    // signal (SIGTTIN, SIG_IGN);
    // signal (SIGTTOU, SIG_IGN);

    shell_pgid = getpid();
    if (setpgid(shell_pgid, shell_pgid) < 0){
        perror("Failed to put shell in own process group");
        exit(1);
    }
    shell_terminal = STDOUT_FILENO;
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);

    std::string line;

    while (1)
    {
        std::cout << "cppShell > ";
        std::getline(std::cin, line);
        if (line == "exit")
        {
            break;
        }
        Main::process_command(line);
    }
}

namespace Main
{
// TODO: work with job_list
// Generate a job, of PGID -1. If that job's PGID is set in child,
// then set the child's pgrp to that job. otherwise, make
// that child the pgrp leader.
void process_command(std::string line)
{
    Utils::trim(line);
    DataStructures::Job job = Utils::get_job_from_line(line);
    // Gets the first process in the pipeline (multipiping soooon)
    char **executed_command = job.processes[0].argv;
    //cd is not built in
    if (!strcmp(executed_command[0], "cd"))
    {
        if (chdir(executed_command[1]) != 0) {
            perror("cd failed");
        }
        return;
    }



    int pid = fork();
    if (pid < 0)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {
        //launch_process(&job.processes[0], job, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO, 1);
        launch_job(job);
    }
    else
    {
        // If job PGID is there, put the child in that group
        // Else, make the process group
        if (job.pgid == 0) {
            job.pgid = pid;
            setpgid(pid, job.pgid);
        }

        int status;
        int waitCode;
        if ((waitCode = waitpid(-1, &status, WUNTRACED)) < 0) {
            std::cout << errno << std::endl;
        }
        // Give terminal back to the shell (ignore SIGTTOU in case)
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(shell_terminal, getpid());
        signal(SIGTTOU, SIG_DFL);
    }
    free(executed_command);
}

void
launch_process (DataStructures::Process *p, DataStructures::Job &job,
                int infile, int outfile, int errfile,
                int foreground)
{
    pid_t pid;

    pid = getpid ();
    if (job.pgid == 0) {
        job.pgid = pid;
        setpgid (pid, job.pgid);
    }
    // give terminal foreground
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp (shell_terminal, job.pgid);

    // Restore signal handling to work properly in child process.
    signal (SIGINT, SIG_DFL);
    signal (SIGTSTP, SIG_DFL);
    signal (SIGTTIN, SIG_DFL);
    signal (SIGTTOU, SIG_DFL);
    

    /* Set the standard input/output channels of the new process.  */
    if (infile != STDIN_FILENO)
    {
        dup2 (infile, STDIN_FILENO);
        close (infile);
    }
    if (outfile != STDOUT_FILENO)
    {
        dup2 (outfile, STDOUT_FILENO);
        close (outfile);
    }
    if (errfile != STDERR_FILENO)
    {
        dup2 (errfile, STDERR_FILENO);
        close (errfile);
    }

    /* Exec the new process.  Make sure we exit.  */
    execvp (p->argv[0], p->argv);
    perror ("execvp");
    exit (1);
}

void launch_job(DataStructures::Job &job) {
    DataStructures::Process* proc;
    int std_in, fd[2];
    int job_list_length = job.processes.size();

    for (int i = 0; i < job.processes.size(); i++) {
        if (i < job_list_length - 1) {
            // If not at end of process chain, set up pipes
            pipe(fd);
            launch_process(&job.processes[i], job, std_in, fd[1], STDERR_FILENO, 1);
            close(fd[1]);
            std_in = fd[0];
        } else {
            int std_out = STDOUT_FILENO;
            launch_process(&job.processes[i], job, std_in, std_out, STDERR_FILENO, 1);
            close(std_in);
        }
    }
}
} // namespace Main
