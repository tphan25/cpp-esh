#include <vector>
#include <string>

namespace Utils
{
char **vector_to_char_pointers(std::vector<std::string> line);
std::vector<std::string> split(const std::string &s, char delim);

template <typename Out>
void split(const std::string &s, char delim, Out result);
} // namespace Utils
