#include <vector>

#include "c-esh_datastructures.hpp"

// https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell
namespace DataStructures
{
/* Find the active Job with the indicated pgid.  */
Job *find_job(pid_t pgid, std::vector<Job> vec)
{
    for (auto j = 0; j < vec.size(); j++)
    {
        if ((vec[j]).pgid == pgid)
        {
            return &vec[j];
        }
    }
    return NULL;
}

/* Return true if all processes in the Job have stopped or completed.  */
int job_is_stopped(Job *j)
{
    Process *p;

    for (p = j->first_process; p; p = p->next)
        if (!p->completed && !p->stopped)
            return 0;
    return 1;
}

/* Return true if all processes in the Job have completed.  */
int job_is_completed(Job *j)
{
    Process *p;

    for (p = j->first_process; p; p = p->next)
        if (!p->completed)
            return 0;
    return 1;
}
} // namespace DataStructures
