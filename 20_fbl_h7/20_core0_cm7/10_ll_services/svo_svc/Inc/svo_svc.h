// include/svo_svc.h
#ifndef SERIAL_HAL_SVC_H
#define SERIAL_HAL_SVC_H

#include "stm32h7xx_hal.h"

/*
 * Initialize SWO for ITM
 */
void serial_hal_svc_init(void);

/*
 * Send a string over SWO
 */
void serial_hal_svc_send(const char *message);

/*
 * Send a string + numeric parameter over SWO
 */
void serial_hal_svc_send_param(const char *message, long long param);

#endif // SERIAL_HAL_SVC_H
