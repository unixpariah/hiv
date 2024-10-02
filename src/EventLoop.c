#include "ArrayList.h"
#include "sys/poll.h"
#include <EventLoop.h>
#include <poll.h>
#include <stdint.h>
#include <stdlib.h>

void event_loop_init(EventLoop *event_loop) {
  ArrayList pollfds = {0};
  array_list_init(&pollfds);

  ArrayList polldata = {0};
  array_list_init(&polldata);

  event_loop->pollfds = pollfds;
  event_loop->polldata = polldata;
}

enum EventLoopResult event_loop_insert_source(EventLoop *event_loop, int32_t fd,
                                              void (*callback)(void *data),
                                              void *data) {
  PollData *polldata = malloc(sizeof(PollData));
  if (polldata == NULL) {
    return EVENT_LOOP_OOM;
  }

  polldata->data = data;
  polldata->callback = callback;

  struct pollfd *pollfd = malloc(sizeof(struct pollfd));
  if (pollfd == NULL) {
    return EVENT_LOOP_OOM;
  }

  pollfd->fd = fd;
  pollfd->events = POLLIN;
  pollfd->revents = 0;

  if (array_list_append(&event_loop->pollfds, &pollfd) == ARRAY_LIST_OOM) {
    return EVENT_LOOP_OOM;
  }
  if (array_list_append(&event_loop->polldata, &polldata) == ARRAY_LIST_OOM) {
    return EVENT_LOOP_OOM;
  }

  return EVENT_LOOP_OK;
}

enum EventLoopResult event_loop_poll(EventLoop *event_loop) {
  int32_t events = poll((struct pollfd *)event_loop->pollfds.items,
                        event_loop->pollfds.len, -1);
  if (events == -1) {
    return EVENT_LOOP_POLL_ERR;
  }

  for (int32_t i = 0; i < events; i++) {
    struct pollfd *pollfd = event_loop->pollfds.items[i];
    if (pollfd->revents & POLLIN) {
      PollData *polldata = event_loop->polldata.items[i];
      polldata->callback(polldata->data);
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
