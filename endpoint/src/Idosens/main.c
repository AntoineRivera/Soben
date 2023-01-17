/*
 * Antoine Rivera
 * Leo Barbet
 * IESE5
 * 01/2023
 */


/* Include */
#include "config.h"


/* Defines */

/* Instance Timer */
ztimer_t timer;
/* Temperature driver instance */
at30tse75x_t temp_driver;

/* Sender Variables */
kernel_pid_t sender_pid;
char sender_stack[THREAD_STACKSIZE_MAIN / 2];



int main(void)
{
    /* Init UART */
    /* Init UART : 8 bits, pas de parité, stop bit 1 */
    /* UART0 : PC terminal */
    // uart_init(UART, BAUDRATE, NULL, (void *)UART);
    ztimer_sleep(ZTIMER_SEC, 1);

    /* Init Watchdog timer */
    wdt_setup_reboot(0, MAX_TIME_WDT);
    /* Enable Watchdog timer */
    wdt_start();

    /* Init Lora */
    init_lora(); // initialisation du module lora
    join_procedure(); // procedure de join

    /* Init sensors */
    puts("Starting temperature driver\r\n");
    while (at30tse75x_init(&temp_driver, PORT_A, AT30TSE75X_TEMP_ADDR) != 0) {
        puts("error temperature driver sensor\r\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    };
    puts("Starting init temperature driver succeeded\r\n");

    puts("Starting magnetometer driver\r\n");
    // uint8_t lsm303d_available = 0;
    // float lsm303d_data[3] = {0};
    if (lsm303d_who_i_am() == 0)
    {
        // lsm303d_available = 1;
        puts("Magnetometer available\r\n");    
        lsm303d_init();
    }
    puts("Starting init magnetometer driver succeeded\r\n");

    /* DEBUG */
    printf("Board powered\r\n");
    

    /* ================================================================ */

    /* ======================== Boucle infinie ======================== */
    while(1)
    {
       /* Reset the watchdog counter */
       wdt_kick();

       /* Create Sender thread */
       sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                   SENDER_PRIO, 0, sender, NULL, "sender");

        /* Trigger the first send */
        msg_t msg;
        msg_send(&msg, sender_pid);
    }
    return 0;
}