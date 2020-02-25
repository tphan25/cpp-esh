#include <string>
#include <vector>

void process_command(std::string line, std::vector<struct job> &job_list);
char **vector_to_char_pointers(std::vector<std::string> line);
std::vector<std::string> split(const std::string &s, char delim);

template <typename Out>
void split(const std::string &s, char delim, Out result);