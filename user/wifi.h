#ifndef _WIFI_H
#define _WIFI_H

#include "head_file.h"

void wifi_init(void);
void wifi_send(unsigned char*send_buf,int n);

extern int wifi_set_flag;
extern char wifi_value[8];
#endif
