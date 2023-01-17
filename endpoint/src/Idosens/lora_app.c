/*
 *  author : rivera
 *  date : 01/2023
 *  file : lora_app.c
 */

/* Include */
#include "config.h"
#include "debug.h"

/* Variable globale */
/* ID Lora */
static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

/* Module lora utilise */
static semtech_loramac_t loramac;
static sx127x_t sx127x;
static cayenne_lpp_t lpp;

extern kernel_pid_t sender_pid;
extern at30tse75x_t temp_driver;

/* Prototypes */
static void _send_message(void);

static void _alarm_cb(void *arg);
static void _prepare_next_alarm(void);



/* Initialise le module lora sx127x, les parametres Lora et definit les parametres : devEUI, appEUI, appKey */
uint8_t init_lora(void)
{
    puts("Starting init lora\r\n");

    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

    /* print ID : devEUI, AppEUI, AppKey */
    printf("[otaa] DevEUI: %s\n", CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    printf("[otaa] AppEUI: %s\n", CONFIG_LORAMAC_APP_EUI_DEFAULT);
    printf("[otaa] AppKey: %s\n", CONFIG_LORAMAC_APP_KEY_DEFAULT);

    /* Initialize the radio driver */
    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = &sx127x.netdev;
    loramac.netdev->driver = &sx127x_driver;

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    /* Defini la modulation, facteur d'etalement et la bande passante 
     * LORAMAC_DR_5 : Modulation : ISM EU863-870, Facteur d'etalement : SF7, Bande passante : BW125
     */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    return 0;
}


/* Realisation de la procedure de join 1 fois, au lancement du minitel. 
 * Le join etablie la connexion entre la passerelle et carte idosens pour un devEUI.
 */
uint8_t join_procedure (void)
{
    /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
     * generated device address and to get the network and application session
     * keys.
     */
    uint8_t ret = 0;
    /* Get spread factor parameter */
    uint8_t loramac_dr = semtech_loramac_get_dr(&loramac);

    puts("Starting join procedure\r\n");
    do 
    {
        /* try to join gateway network */
        ret = semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA);

        if (ret == SEMTECH_LORAMAC_JOIN_FAILED)
        {
            printf("\r\nCODE ERROR %d : Echec dans la procedure !\r\n", ret);
        }
        else if (ret == SEMTECH_LORAMAC_ALREADY_JOINED)
        {
            printf("\r\nCODE ERROR %d : Procedure join deja effectuee !\r\n", ret);
        }
        else if (ret == SEMTECH_LORAMAC_BUSY)
        {
            printf("\r\nCODE ERROR %d : Module Lora en cours d'utilisation !\r\n", ret);
        }
        else
        {
            printf("\r\nCODE ERROR %d !\r\n", ret);
        }

        ztimer_sleep(ZTIMER_SEC, JOIN_RETRY_PERIOD);
        // ztimer_sleep(ZTIMER_USEC, JOIN_RETRY_PERIOD * US_PER_SEC);

        // Decrement DR before retrying
        semtech_loramac_set_dr(&loramac, --loramac_dr);
        if (loramac_dr > LORAMAC_DR_15) return SEMTECH_LORAMAC_JOIN_FAILED;
    }
    while (ret != SEMTECH_LORAMAC_JOIN_SUCCEEDED);
    puts("Join procedure succeeded\r\n");

    return 0;
}


static void _send_message(void)
{
    float temp = 0.0f;
    float lsm303d_data[3] = {0.0f};

    /* read temp value */
    if (at30tse75x_get_temperature(&temp_driver, &temp) == 0){
        printf("Temperature: %f\r\n", temp);
        cayenne_lpp_reset(&lpp);
        /* add at30tse75x data to lpp */
        cayenne_lpp_add_temperature(&lpp, 3, temp);
    }
    else {
        puts("Fail to read temperature\r\n");
    }
    
    /* read mag value */
    if (lsm303d_who_i_am() == 0){
        /* get mag value */
        lsm303d_read_float_data(lsm303d_data);
        // add lsm303d data to lpp
        cayenne_lpp_add_accelerometer(&lpp, 4, lsm303d_data[0], lsm303d_data[1], lsm303d_data[2]);
    }

    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac,
                                        (uint8_t *) &lpp.buffer, sizeof(lpp.cursor));
    if (ret != SEMTECH_LORAMAC_TX_DONE)  {
        puts("Fail to send message\r\n");
    } else {
        puts("Message sent\r\n");
    }

    return;
}


void *sender(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);

        /* Trigger the message send */
        _send_message();

        /* Schedule the next wake-up alarm */
        _prepare_next_alarm();

        /* Reset the watchdog counter */
        wdt_kick();
    }

    /* this should never be reached */
    return NULL;
}



static void _alarm_cb(void *arg)
{
    (void) arg;
    msg_t msg;
    msg_send(&msg, sender_pid);
}

static void _prepare_next_alarm(void)
{
    timer.callback = _alarm_cb;
    ztimer_set(ZTIMER_MSEC, &timer, TXPERIOD * MS_PER_SEC);
}