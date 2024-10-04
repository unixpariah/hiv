#include "bits/types/struct_itimerspec.h"
#include <EventLoop.h>
#include <assert.h>
#include <stdio.h>
#include <sys/timerfd.h>

void callback(void *data) {
  int *num = data;
  *num = 10;
}

int main(void) {
  int fd = timerfd_create(1, TFD_CLOEXEC);
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

  EventLoop event_loop = {0};
  event_loop_init(&event_loop);

  int number = 9;
  event_loop_insert_source(&event_loop, fd, &callback, &number);

  assert(event_loop_poll(&event_loop) == EVENT_LOOP_OK);

  assert(number == 10);

  event_loop_deinit(&event_loop);
}
