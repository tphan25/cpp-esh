#include <string>
#include <vector>
#include "c-esh_datastructures.hpp"
namespace Main
{
void process_command(std::string line);
void signal_handler(int signum);
void
launch_process (DataStructures::Process *p, DataStructures::Job &job,
                int infile, int outfile, int errfile,
                int foreground);
void launch_job(DataStructures::Job &job);

}
