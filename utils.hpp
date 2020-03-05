#include <vector>
#include <string>

namespace Utils
{
char **vector_to_char_pointers(const std::vector<std::string> line);
std::vector<std::string> split(const std::string &s, char delim);

template <typename Out>
void split(const std::string &s, const char delim, Out result);

DataStructures::Job get_job_from_line(const std::string line);
char **string_to_char_arr(const std::string str);
} // namespace Utils
