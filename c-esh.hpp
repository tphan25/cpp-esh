#include <string>
#include <vector>
#include "c-esh_datastructures.hpp"
namespace Main
{
void process_command(std::string line);
void signal_handler(int signum);
void
launch_process (DataStructures::Process *p, pid_t pgid,
                int infile, int outfile, int errfile,
                int foreground);
}
