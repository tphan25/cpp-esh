#include <unistd.h>
#include <termios.h>

namespace DataStructures
{
// https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell
class Process
{
public:
    Process *next;  /* next process in pipeline */
    char **argv;    /* for exec */
    pid_t pid;      /* process ID */
    char completed; /* true if process has completed */
    char stopped;   /* true if process has stopped */
    int status;     /* reported status value */
};

class Job
{
public:
    char *command;             /* command line, used for messages */
    Process *first_process;    /* list of processes in this job */
    pid_t pgid;                /* process group ID */
    struct termios tmodes;     /* saved terminal modes */
    int stdin, stdout, stderr; /* standard i/o channels */
};
} // namespace DataStructures
