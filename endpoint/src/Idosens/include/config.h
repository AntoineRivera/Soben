/*
 *  author : rivera
 *  date : 01/2023
 *  file : config.h
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Includes elementaires */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Include Thread */
#include "msg.h"
#include "thread.h"
#include "fmt.h"

/* Include GPIO */
#include "board.h"
#include "periph/spi.h"
#include "periph_conf.h"

/* Include timers */
// #include "clk.h"
#include "periph/rtc.h"
#include "timex.h"
#include "ztimer.h"

/* Include LoRa */
#include "lora_app.h"
#include "net/netdev/lora.h"
#include "net/netdev.h"
#include "cayenne_lpp.h"
#include "net/loramac.h"
#include "semtech_loramac.h"
#include "sx127x.h"
#include "sx127x_internal.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"

/* include capteurs */
/* Capteur de temperature */
#include "at30tse75x.h"
/* Capteur magnetometre */
#include "lsm303d.h"

/* Include Watchdog timer */
#include "periph/wdt.h"
#include "xtimer.h"





/* Defines */
#ifndef TXPERIOD
#define TXPERIOD                (60U)
#endif

#ifndef JOIN_RETRY_PERIOD
#define JOIN_RETRY_PERIOD       (60U)
#endif

#define SENDER_PRIO         	(THREAD_PRIORITY_MAIN - 1)


// 1 Day in ms
#ifndef MAX_TIME_WDT
#define MAX_TIME_WDT			(86400000U)
#endif

/* Variables */
/* Instance Timer */
extern ztimer_t timer;


#endif
