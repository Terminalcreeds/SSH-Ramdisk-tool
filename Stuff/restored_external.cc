#include <iostream>
#include <spawn.h>
#include <unistd.h>

#define RESET   "\033[0m"
#define THICKRED     "\033[1m\033[31m"

int main(int argc, char const *argv[]) {

  std::cout << THICKRED << "[i] Starting restored external..." << RESET << '\n';

  int pid, i;
  std::cout << THICKRED << "[i] Mounting Rootfs..." << RESET << '\n';
   char *arg[] = {"mount_apfs", "/dev/disk0s1s1", "/mnt1", NULL}; // /dev/disk0s1s1 == rootfs
   posix_spawn(&pid, "/System/Library/Filesystems/apfs.fs/mount_apfs", NULL, NULL, (char* const*)arg, NULL);
   waitpid(pid, &i, 0);
   std::cout << THICKRED << "[i] Done?" << RESET << '\n';
   //
   std::cout << THICKRED << "[i] Mounting data partition..." << RESET << '\n';
   char *arg2[] = {"mount_apfs", "/dev/disk0s1s2", "/mnt2", NULL}; // /dev/disk0s1s1 == rootfs
   posix_spawn(&pid, "/System/Library/Filesystems/apfs.fs/mount_apfs", NULL, NULL, (char* const*)arg2, NULL);
   waitpid(pid, &i, 0);
   std::cout << THICKRED << "[i] Done?" << RESET << '\n';
   sleep(1);
   //
   char *arg3[] = {"restored_external", "-server", NULL};
   posix_spawn(&pid, "/usr/local/bin/restored_external_original", NULL, NULL, (char* const*)arg3, NULL);
   waitpid(pid, &i, 0);
   sleep(1);
   sleep(99999);
  return 0;
}
