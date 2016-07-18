#include "uim.h"
#include <uim/uim.h>
#include <uim/uim-helper.h>
#include <uim/uim-scm.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

static int uimled_uim_fd = -1;
static pthread_t uimled_uim_read_id = 0;

static char* uimled_uim_text = NULL;
static pthread_mutex_t uimled_uim_text_mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t uimled_uim_text_cond = PTHREAD_COND_INITIALIZER;

void* uimled_uim_read_thread(void* data)
{
  char* s;
  char* s1;
  char* s2;
  char c;

  int fd = *((int*)data);

  while (1)
    {
      /* sleep until message */
      recv(fd, &c, 1, MSG_PEEK);

      uim_helper_read_proc(fd);
      while ((s = uim_helper_get_message()))
        {
          if (strncmp(s, "prop_list_update", sizeof("prop_list_update") - 1))
            goto endloop;

          s1 = s;

          while (1)
            {
              s1 = strstr(s1, "\nbranch\t");
              if (!s1)
                break;

              s1 += sizeof("\nbranch\t") - 1;
              s2 = strchr(s1, '\t');
              s2[0] = 0;

              pthread_mutex_lock(&uimled_uim_text_mut);
              uimled_uim_text = s1;
              pthread_mutex_unlock(&uimled_uim_text_mut);
              pthread_cond_signal(&uimled_uim_text_cond);

              s1 = s2 + 1;
            }

        endloop:
          free(s);
        }
    }

  return NULL;
}


char* uimled_uim_getcurrent_lock(void)
{
  pthread_mutex_lock(&uimled_uim_text_mut);
  pthread_cond_wait(&uimled_uim_text_cond, &uimled_uim_text_mut);

  return uimled_uim_text;
}

void uimled_uim_getcurrent_unlock(void)
{
  pthread_mutex_unlock(&uimled_uim_text_mut);
}


void uimled_uim_connect(void)
{
  if (uimled_uim_fd >= 0)
    return;

  uimled_uim_fd = uim_helper_init_client_fd(uimled_uim_disconnect);

  if (uimled_uim_fd < 0)
    return;

  if (!uimled_uim_read_id)
    {
      if (pthread_create(&uimled_uim_read_id, NULL,
                         uimled_uim_read_thread,
                         &uimled_uim_fd) == -1)
        {
          perror("pthread_create");
          return;
        }
    }
}

void uimled_uim_disconnect(void)
{
  uimled_uim_fd = -1;
}

void uimled_uim_init(void)
{
  uim_init();

  pthread_mutex_init(&uimled_uim_text_mut, NULL);
  pthread_cond_init(&uimled_uim_text_cond, NULL);

  uimled_uim_connect();
}


void uimled_uim_proplist(void)
{
  uim_helper_send_message(uimled_uim_fd, "prop_list_get\n");
}
