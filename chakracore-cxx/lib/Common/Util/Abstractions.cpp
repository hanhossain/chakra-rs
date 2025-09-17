#include "Abstractions.h"
#include <unistd.h>

#if defined (__LINUX__)
#include <cstdlib>
#include <cstring>
#include <sys/fcntl.h>
#elif defined(__APPLE__)
#include <sys/proc.h>
#include <sys/sysctl.h>
#endif

// Used to determine if a debugger is attached to the process.
bool Abstractions::IsDebuggerPresent()
{
#if defined (__LINUX__)
    bool debugger_present = false;
    char buf[2048];

    int status_fd = open("/proc/self/status", O_RDONLY);
    if (status_fd == -1)
    {
        return false;
    }

    ssize_t num_read = read(status_fd, buf, sizeof(buf) - 1);

    if (num_read > 0)
    {
        static const char TracerPid[] = "TracerPid:";
        char *tracer_pid;

        buf[num_read] = '\0';
        tracer_pid = std::strstr(buf, TracerPid);
        if (tracer_pid)
        {
            debugger_present = std::atoi(tracer_pid + sizeof(TracerPid) - 1);
        }
    }

    close(status_fd);
    return debugger_present;
#elif defined(__APPLE__)
    struct kinfo_proc info = {};
    size_t size = sizeof(info);
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    int ret = sysctl(mib, sizeof(mib)/sizeof(*mib), &info, &size, nullptr, 0);

    if (ret == 0)
    {
        return ((info.kp_proc.p_flag & P_TRACED) != 0);
    }
    
    return false;
#else
    return false;
#endif
}