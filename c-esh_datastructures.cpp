#include <vector>

#include "c-esh_datastructures.h"

// https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell

/* Find the active job with the indicated pgid.  */
job *find_job(pid_t pgid, std::vector<job> vec)
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

/* Return true if all processes in the job have stopped or completed.  */
int job_is_stopped(job *j)
{
    process *p;

    for (p = j->first_process; p; p = p->next)
        if (!p->completed && !p->stopped)
            return 0;
    return 1;
}

/* Return true if all processes in the job have completed.  */
int job_is_completed(job *j)
{
    process *p;

    for (p = j->first_process; p; p = p->next)
        if (!p->completed)
            return 0;
    return 1;
}