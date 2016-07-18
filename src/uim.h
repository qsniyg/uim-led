#ifndef __UIMLED_UIM_H__
#define __UIMLED_UIM_H__

void uimled_uim_connect(void);
void uimled_uim_disconnect(void);
void uimled_uim_init(void);

void uimled_uim_proplist(void);

char* uimled_uim_getcurrent_lock(void);
void uimled_uim_getcurrent_unlock(void);

#endif
