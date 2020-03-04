#include <sstream>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "c-esh_datastructures.hpp"
#include "utils.hpp"

namespace Utils
{
char **vector_to_char_pointers(const std::vector<std::string> line)
{
    char **ret = (char **)malloc(sizeof(char *) * line.size() + 1);
    for (uint32_t i = 0; i < line.size(); i++)
    {
        int length = line[i].length();
        char *curr = (char *)malloc(sizeof(char) * length + 1);
        strcpy(curr, &line[i][0]);
        curr[length] = '\0';
        *(ret + i) = curr;
    }
    ret[line.size()] = NULL;
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
void split(const std::string &s, const char delim, Out result)
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

//TODO
//Split line i.e. cat foo.txt | grep 'hello'
//Iterate through, appending to a process until | is met, then trim
DataStructures::Job get_job_from_line(const std::vector<std::string> line)
{
    DataStructures::Job job;
    std::vector<DataStructures::Process> proc_list;
    DataStructures::Process *curr_process = new DataStructures::Process();
    std::vector<std::string>::const_iterator iter;
    std::ostringstream str_builder;

    for (iter = line.begin(); iter < line.end(); iter++)
    {
        if (*iter != "|")
        {
            str_builder << *iter;
            str_builder << " ";
        }
    }
    std::cout << str_builder.str();
    return job;
}

} // namespace Utils