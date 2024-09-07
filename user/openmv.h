#ifndef _OPENMV_H
#define _OPENMV_H

#include "head_file.h"
extern int openmv_cmd;
extern int QR_value[8],qr_flag;
extern int CMR_value[13],cmr_flag;

void QR_read(void);
void Camera_on(void);
void Camera_off(void);
void QR_send(void);
void CMR_send(void);
void wifi_back(void);
#endif
