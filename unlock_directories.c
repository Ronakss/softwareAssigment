
 #include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>

void unlock_directories() {
    char* dirs[] = {"./shared", "./reporting"};  // list of directories to unlock
    int i, num_dirs = sizeof(dirs) / sizeof(char*);

    for (i = 0; i < num_dirs; i++) {
        if (chmod(dirs[i], 0777) == -1) {  // attempt to unlock directory
            syslog(LOG_ERR, "Error unlocking directory %s: %s", dirs[i], strerror(errno));
        }
    }

    // check if the directories are unlocked
    struct stat st;
    for (i = 0; i < num_dirs; i++) {
        if (stat(dirs[i], &st) == -1) {  // failed to get file info
            syslog(LOG_ERR, "Error getting info for directory %s: %s", dirs[i], strerror(errno));
        } else if ((st.st_mode & S_IFMT) != S_IFDIR) {  // not a directory
            syslog(LOG_ERR, "%s is not a directory", dirs[i]);
        } else if ((st.st_mode & S_IRWXU) != 0700) {  // not unlocked
            syslog(LOG_ERR, "%s is not unlocked", dirs[i]);
        } else {  // directory is unlocked
            syslog(LOG_INFO, "%s is unlocked", dirs[i]);
        }
    }
}
