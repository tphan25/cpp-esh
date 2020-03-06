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

#include "c-esh.hpp"
#include "c-esh_datastructures.hpp"
#include "utils.hpp"

std::vector<DataStructures::Job> global_job_list;

int main()
{
    // TODO: write handlers
    signal(SIGABRT, Main::signal_handler);
    signal(SIGTSTP, Main::signal_handler);
    // signal(SIGCHLD, Main::signal_handler); Ends parent ATM
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
void process_command(std::string line)
{
    Utils::trim(line);
    std::vector<std::string> vec = Utils::split(line, ' ');
    DataStructures::Job job = Utils::get_job_from_line(line);
    // Gets the first process in the pipeline (multipiping soooon)
    char **executed_command = job.processes[0].argv;
    //cd is not built in
    if (!strcmp(executed_command[0], "cd"))
    {
        int err = chdir(executed_command[1]);
        if (err == -1)
        {
            std::cout << "error with cd" << errno << std::endl;
        }
        return;
    }

    int pid = fork();

    if (pid < 0)
    {
        std::cout << "error forking" << std::endl;
    }
    else if (pid == 0)
    {
        int exec = execvp(executed_command[0], executed_command);
        std::cout << "exec code " << exec << std::endl;
        exit(-1);
    }
    else
    {
        int status;
        waitpid(-1, &status, 0);
    }
    free(executed_command);
}

void signal_handler(int signum) {
    std::cout << "The signal called is " << signum << "." << std::endl;
    pid_t pid = getpid();
    pid_t pgid = getpgid(pid);
    if (pgid == -1) {
        std::cout << "The errno is " << errno << "." << std::endl;
    }
    std::cout << "You should return control to " << pgid << std::endl;
    tcsetpgrp(STDIN_FILENO, pgid);
    if (!signum == SIGTSTP)
        exit(signum);
}
} // namespace Main
