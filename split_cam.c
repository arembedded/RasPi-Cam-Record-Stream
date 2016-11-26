/* This file implements a data splitter
 * for data coming in to STDIN.
 * Data is split to be written to a 
 * file as well as written back out
 * to STDOUT for streaming.
 *
 * All code provided is as is and
 * is not fully tested.
 *
 * Author: Aadil Rizvi
 * Date Created: 11/25/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stropts.h>
#include <signal.h>

#define MAX_FILENAME_SIZE 100

//File descriptor for currently open data file
static int fd_out;

void sig_handler(int signum) {

  if (fd_out >= 0) {    
    // Flush all write queues
    ioctl(fd_out, I_FLUSH, FLUSHW);

    // Close data file
    close(fd_out);
  }

  exit(0);
}

int main(void) {
  char in_array[1024];
  int nBytes;
  char filename[MAX_FILENAME_SIZE];
  struct timeval ts;  

  // Register signal handler
  signal(SIGINT, sig_handler);

  // Initialize fd_out
  fd_out = -1;

  // Get current time for unique filename
  gettimeofday(&ts, NULL);

  // Generate filename
  snprintf(filename, MAX_FILENAME_SIZE, "raspiVid-%u-%u.h264", ts.tv_sec, ts.tv_usec);

  // Open file to write data
  fd_out = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

  // Read data from STDIN
  nBytes = read(STDIN_FILENO, in_array, sizeof(in_array));

  // Read data from stdin in a loop
  // while writing data to file and
  // stdout as well
  while(nBytes > 0) {

    // Write data to file
    write(fd_out, (const void*)in_array, nBytes);

    // Write data to stdout as well
    write(STDOUT_FILENO, (const void*)in_array, nBytes);

    // Flush all write queues
    ioctl(fd_out, I_FLUSH, FLUSHW);

    // Read next available chunk of data from stdin
    nBytes = read(STDIN_FILENO, in_array, sizeof(in_array)); 
  }

  // Close data file
  close(fd_out);
  fd_out = -1;

  return 0;
}
