#define _POSIX_C_SOURCE 199309L

#include "bits/time.h"
#include "bits/types/struct_itimerspec.h"
#include "unistd.h"
#include <EventLoop.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/timerfd.h>
#include <time.h>

void callback(int fd, void *data) {
  uint64_t expirations = 0;
  read(fd, &expirations, sizeof(expirations));

  int *num = data;
  *num = 10;
}

int main(void) {

  EventLoop event_loop = {0};
  event_loop_init(&event_loop);

  int number = 9;
  {
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
    struct itimerspec timerspec = {
        .it_value =
            {
                .tv_sec = 1,
                .tv_nsec = 0,
            },
        .it_interval =
            {
                .tv_sec = 0,
                .tv_nsec = 0,
            },
    };
    timerfd_settime(fd, 0, &timerspec, NULL);

    event_loop_insert_source(&event_loop, fd, &callback, &number, 1);
  }

  assert(event_loop.pollfds.len == 1);
  assert(event_loop_poll(&event_loop) == EVENT_LOOP_OK);
  assert(number == 10);
  assert(event_loop.pollfds.len == 0);

  {
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
    struct itimerspec timerspec = {
        .it_value =
            {
                .tv_sec = 1,
                .tv_nsec = 0,
            },
        .it_interval =
            {
                .tv_sec = 0,
                .tv_nsec = 0,
            },
    };
    timerfd_settime(fd, 0, &timerspec, NULL);

    event_loop_insert_source(&event_loop, fd, &callback, &number, 1);
  }

  number = 0;

  assert(event_loop.pollfds.len == 1);
  assert(event_loop_poll(&event_loop) == EVENT_LOOP_OK);
  assert(event_loop.pollfds.len == 0);
  assert(number == 10);

  event_loop_deinit(&event_loop);
}
