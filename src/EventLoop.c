#include "ArrayList.h"
#include <EventLoop.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>

/// Must be freed with event_loop_deinit
void event_loop_init(EventLoop *event_loop) {
  ArrayList pollfds = {0};
  array_list_init(&pollfds);

  ArrayList polldata = {0};
  array_list_init(&polldata);

  event_loop->pollfds = pollfds;
  event_loop->polldata = polldata;
}

enum EventLoopResult event_loop_insert_source(EventLoop *event_loop, int32_t fd,
                                              void (*callback)(int fd,
                                                               void *data),
                                              void *data, uint32_t repeats) {
  PollData *polldata = malloc(sizeof(PollData));
  if (polldata == NULL) {
    return EVENT_LOOP_OOM;
  }

  polldata->data = data;
  polldata->callback = callback;
  polldata->repeats = repeats;

  struct pollfd *pollfd = malloc(sizeof(struct pollfd));
  if (pollfd == NULL) {
    free(polldata);
    return EVENT_LOOP_OOM;
  }

  pollfd->fd = fd;
  pollfd->events = POLLIN;
  pollfd->revents = 0;

  if (array_list_append(&event_loop->pollfds, pollfd) == ARRAY_LIST_OOM) {
    free(polldata);
    free(pollfd);
    return EVENT_LOOP_OOM;
  }
  if (array_list_append(&event_loop->polldata, polldata) == ARRAY_LIST_OOM) {
    free(polldata);
    free(array_list_pop(&event_loop->pollfds));
    return EVENT_LOOP_OOM;
  }

  return EVENT_LOOP_OK;
}

enum EventLoopResult event_loop_poll(EventLoop *event_loop) {
  int32_t events =
      poll(*event_loop->pollfds.items, event_loop->pollfds.len, -1);
  if (events == -1) {
    return EVENT_LOOP_POLL_ERR;
  }

  if (events > 0) {
    for (uint32_t i = 0; i < event_loop->pollfds.len; i++) {
      struct pollfd *pollfd = event_loop->pollfds.items[i];
      if (pollfd->revents & POLLIN) {
        PollData *polldata = event_loop->polldata.items[i];
        polldata->callback(pollfd->fd, polldata->data);
        if (polldata->repeats == 1) {
          free(array_list_swap_remove(&event_loop->polldata, i));
          free(array_list_swap_remove(&event_loop->pollfds, i));
        } else if (polldata->repeats != 0) {
          polldata->repeats--;
        }
      }
    }
  }

  return EVENT_LOOP_OK;
}

void event_loop_deinit(EventLoop *event_loop) {
  for (uint32_t i = 0; i < event_loop->polldata.len; i++) {
    PollData *polldata = event_loop->polldata.items[i];
    free(polldata);
    struct pollfd *pollfd = event_loop->pollfds.items[i];
    free(pollfd);
  }

  array_list_deinit(&event_loop->polldata);
  array_list_deinit(&event_loop->pollfds);
}
