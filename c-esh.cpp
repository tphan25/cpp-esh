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

#include "c-esh.h"
#include "c-esh_datastructures.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;
    std::string line;
    std::vector<struct job> job_list;

    while (1)
    {
        std::cout << "cppShell > ";
        std::getline(std::cin, line);
        if (line == "exit")
        {
            break;
        }
        process_command(line, job_list);
    }
}

// TODO: work with job_list
void process_command(std::string line, std::vector<struct job> &job_list)
{
    std::vector<std::string> vec = split(line, ' ');
    char **executed_command = vector_to_char_pointers(vec);

    int pid = fork();

    if (pid == 0)
    {
        execvp(executed_command[0], executed_command);
    }
    else
    {
        int status;
        waitpid(-1, &status, 0);
    }
    free(executed_command);
}

char **vector_to_char_pointers(const std::vector<std::string> line)
{
    char **ret = (char **)malloc(sizeof(char *) * line.size());
    for (uint32_t i = 0; i < line.size(); i++)
    {
        char *curr = (char *)malloc(sizeof(char) * line[i].length());
        strcpy(curr, &line[i][0]);
        *(ret + i) = curr;
    }
    return ret;
}

// Returns a new vector split
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

template <typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        if (!item.empty())
        {
            *result++ = item;
        }
    }
}
