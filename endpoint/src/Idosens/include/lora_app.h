/*
 *  author : rivera
 *  date : 01/2023
 *  file : lora_app.h
 */

#ifndef LORA_APP_H_
#define LORA_APP_H_

/* Prototypes */
uint8_t init_lora(void);
uint8_t join_procedure (void);


void *sender(void *arg);



#endif