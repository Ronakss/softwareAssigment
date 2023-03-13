
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>

void generate_reports() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        syslog(LOG_ERR, "fork failed: %m");
        exit(1);
    } else if (pid == 0) {
        // In child process
        if (execl("/usr/bin/generate_reports", "generate_reports", NULL) == -1) {
            syslog(LOG_ERR, "execl failed: %m");
            exit(1);
        }
    } else {
        // In parent process
        if (waitpid(pid, &status, 0) == -1) {
            syslog(LOG_ERR, "waitpid failed: %m");
            exit(1);
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            syslog(LOG_INFO, "generate_reports succeeded");
        } else {
            syslog(LOG_ERR, "generate_reports failed");
        }
    }
}



////////The CTO has offered a list of desired functionality for the new website management model:
// 1.	The company will offer a new shared directory for the 4 departments to upload their xml reports. Authorised users can make changes to directory. The reports will be collected on a nightly basis at 1am and moved to the directory for the dashboard system for processing.
// 2.	The dashboard directory content should be backed up every night.
// 3.	The changes made to the department managers upload directory needs to documented. The username of the user, the file they modified and the timestamp should be recorded.
// 4.	No changes should be allowed to be made to the directories (upload and reporting) while the backup/transfer is happening.
// 5.	If a change needs to be urgently made to the live site, it should be possible to make the changes. (No users should have write access to the new reporting directory)


//requirements:
// a.	Create a daemon to continually manage the operation of the requirements listed by the CTO above.
// b.	Identify new or modified xml reports and log details of who made the changes, this should be generated as a text file report and stored on the server.
// c.	The department managers must upload their xml report file by 11.30pm each night. The uploads to the shared directory must be moved to the reporting directory. This should happen at 1am. (this functionality needs to be self contained in the solution. Don’t use cron)
// d.	If a file wasn’t uploaded this should be logged in the system. (A naming convention can be used to help with this task.)
// e.	When the backup/transfer begins no user should be able to modify either the upload or reporting directory.
// f.	It must be possible to ask the daemon to backup and transfer at any time.
// g.	IPC should be setup to allow all processes to report in on completion of a task. (success or failure)
// h.	Error logging and reporting should be included for all main tasks in the solution.
//i.	Create a makefile to manage the creation of the executables