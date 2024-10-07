#define _POSIX_C_SOURCE 199309L
#define ASSERT(cond)                                                           \
  do {                                                                         \
    if (!(cond))                                                               \
      return -1;                                                               \
  } while (0)

#define TEST_FUNC(func, passes, max)                                           \
  do {                                                                         \
    if (func() == -1) {                                                        \
      print_test_results(passes, max);                                         \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
    passes++;                                                                  \
  } while (0)

#include "bits/time.h"
#include "bits/types/struct_itimerspec.h"
#include "unistd.h"
#include <EventLoop.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>

void callback(int fd, void *data) {
  uint64_t expirations = 0;
  read(fd, &expirations, sizeof(expirations));

  int *num = data;
  *num = 10;
}

int event_loop_test(void) {
  EventLoop event_loop = {0};
  event_loop_init(&event_loop);

  int number = 0;
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

  event_loop_insert_source(&event_loop, fd, &callback, &number, 0);

  ASSERT(event_loop.pollfds.len == 1 || event_loop.polldata.len == 1 ||
         event_loop_poll(&event_loop) == EVENT_LOOP_OK || number == 10);

  event_loop_deinit(&event_loop);

  return 0;
}

int event_loop_test_single_repeat(void) {
  EventLoop event_loop = {0};
  event_loop_init(&event_loop);

  int number = 0;
  int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
  struct itimerspec timerspec = {
      .it_value =
          {
              .tv_sec = 1,
              .tv_nsec = 0,
          },
      .it_interval =
          {
              .tv_sec = 1,
              .tv_nsec = 0,
          },
  };
  timerfd_settime(fd, 0, &timerspec, NULL);

  event_loop_insert_source(&event_loop, fd, &callback, &number, 1);

  ASSERT(event_loop.pollfds.len == 1 || event_loop.polldata.len == 1);

  event_loop_poll(&event_loop);

  ASSERT(event_loop.pollfds.len == 0 || event_loop.polldata.len == 0 ||
         number == 10);

  event_loop_deinit(&event_loop);

  return 0;
}

int event_loop_test_re(void) {
  EventLoop event_loop = {0};
  event_loop_init(&event_loop);

  int number = 0;
  int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
  struct itimerspec timerspec = {
      .it_value =
          {
              .tv_sec = 1,
              .tv_nsec = 0,
          },
      .it_interval =
          {
              .tv_sec = 1,
              .tv_nsec = 0,
          },
  };
  timerfd_settime(fd, 0, &timerspec, NULL);

  event_loop_insert_source(&event_loop, fd, &callback, &number, 10);

  ASSERT(event_loop.pollfds.len == 1 || event_loop.polldata.len == 1);

  for (int i = 0; i < 10; i++) {
    event_loop_poll(&event_loop);
  }

  ASSERT(event_loop.pollfds.len == 0 || event_loop.polldata.len == 0 ||
         number != 10);

  return 0;
}

void print_test_results(uint32_t passes, uint32_t max) {
  printf("EventLoop: Passed %d/%d tests\n", passes, max);
}

int main(void) {
  uint32_t passes = 0;
  uint32_t max = 3;

  TEST_FUNC(event_loop_test, passes, max);
  TEST_FUNC(event_loop_test_single_repeat, passes, max);
  TEST_FUNC(event_loop_test_re, passes, max);

  print_test_results(passes, max);
}
