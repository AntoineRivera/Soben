/*
 *  author : rivera
 *  date : 01/2023
 *  file : lsm303d.c 
 *  
 *  Driver for lsm303d sensor	
*/

#include "config.h"
#include "lsm303d.h"

#define ENABLE_DEBUG    1
#include "debug.h"


/*	SPI transmission
 *  RIOT SPI config par defaut en LSB first !!!!!
 */


/*  READ MODE
 *
 *	bit 0: RW bit. When 0, the data DI(7:0) is written to the device. When 1, the data DO(7:0)
 *	from the device is read. In the latter case the chip drives SDO at the start of bit 8.
 *
 *	bit 1: MS bit. When 0, the address remains unchanged in multiple read/write commands.
 *	When 1, the address is auto-incremented in multiple read/write commands.
 *
 *	bit 2-7: address AD(5:0). This is the address field of the indexed register.
 *
 *	bit 8-15: data DI(7:0) (write mode). This is the data that is written to the device (MSb first).
 *	bit 8-15: data DO(7:0) (read mode). This is the data that is read from the device (MSb first).
 */



/* WRITE MODE
 * bit 0: WRITE bit. The value is 0.
 *
 * bit 1: MS bit. When 0, do not increment address; when 1, increment address in multiple
 * writing.
 *
 * bit 2 -7: address AD(5:0). This is the address field of the indexed register.
 *
 * bit 8-15: data DI(7:0) (write mode). This is the data that is written to the device (MSb first).
 */

/* sensor available ? */
int lsm303d_who_i_am(void)
{
    uint8_t tmp[2] = {0};
    uint8_t res[2] = {0};

	DEBUG("lsm303d available ?...\r\n");
    /* Acquire exclusive access to the bus. */
    spi_acquire(LSM303D_PARAM_SPI, LSM303D_PARAM_SPI_CS, SPI_MODE_0, SPI_CLK_100KHZ);
    
    /* bit0 - 5 = 0x0F ; bit 6 = 0 ; bit 7 = 1 */
    tmp[0] |= (0x1 << 7); // READ
    tmp[0] |= (LSM303D_REG_WHO_AM_I);
    /* read the WHO I AM register */
    spi_transfer_bytes (LSM303D_PARAM_SPI, (spi_cs_t) LSM303D_PARAM_SPI_CS, 0, tmp, res, 2);

    /* Release the SPI periph */
	spi_release(LSM303D_PARAM_SPI);

    // printf("tmp : 0x%x%x, res : 0x%x%x\r\n", tmp[1], tmp[0], res[1], res[0]);

	if (IS_ACTIVE(ENABLE_DEBUG) && res[1] == 0x49) {
        DEBUG("[OK]\r\n");
    }
    else {
        DEBUG("[Failed]\r\n");
    }

    return (res[1] == 0x49) ? 0 : -1;
}




/* Init mag register */
void lsm303d_init(void)
{
    uint8_t tmp[2] = {0};

    DEBUG("lsm303d init...\r\n");
    /* Acquire exclusive access to the bus. */
    spi_acquire(LSM303D_PARAM_SPI, LSM303D_PARAM_SPI_CS, SPI_MODE_0, SPI_CLK_100KHZ);
    
    /* CTRL5 : 0x00 pour resolution minimale et freq minimale 3.125 Hz */

    /* CTRL6 */
    /* bit0 - 5 = 0x1F ; bit 6 = 0 ; bit 7 = 0 */
    tmp[0] |= (LSM303D_REG_CTRL6);
    /* register value */
    tmp[1] |= (0x3 << 5); // MFS[5:6]=11 => MAX RESOLUTION +-12g

    /* write the CTRL6 register */
    spi_transfer_bytes (LSM303D_PARAM_SPI, (spi_cs_t) LSM303D_PARAM_SPI_CS, 0, tmp, NULL, 2);

    tmp[0] = 0;
    tmp[1] = 0;
    /* CTRL7 */
    /* bit0 - 5 = 0x1F ; bit 6 = 0 ; bit 7 = 0 */
    tmp[0] |= (LSM303D_REG_CTRL7);
    /* register value */
    tmp[1] |= (0x0 << 0); // MD[1:0]=00 => Single Conversion Mode
    tmp[1] |= (0x1 << 2); // MLP=1 => Low Power

    /* write the CTRL7 register */
    spi_transfer_bytes (LSM303D_PARAM_SPI, (spi_cs_t) LSM303D_PARAM_SPI_CS, 0, tmp, NULL, 2);

    /* Release the SPI periph */
    spi_release(LSM303D_PARAM_SPI);

    if (IS_ACTIVE(ENABLE_DEBUG)) {
        DEBUG("[OK]\r\n");
    }
    else {
        DEBUG("[Failed]\r\n");
    }
}




/* Is magnetic data available ? */
int lsm303d_is_data_available(void)
{
    uint8_t tmp[2] = {0};
    uint8_t res[2] = {0};

    DEBUG("lsm303d is data available...\r\n");
    /* Acquire exclusive access to the bus. */
    spi_acquire(LSM303D_PARAM_SPI, LSM303D_PARAM_SPI_CS, SPI_MODE_0, SPI_CLK_100KHZ);
    
    /* bit0 - 5 = 0x0F ; bit 6 = 0 ; bit 7 = 1 */
    tmp[0] |= (0x1 << 7); // READ
    tmp[0] |= (LSM303D_REG_STATUS_M);
    /* read the STATUS_M register */
    spi_transfer_bytes (LSM303D_PARAM_SPI, (spi_cs_t) LSM303D_PARAM_SPI_CS, 0, tmp, res, 2);

    /* Release the SPI periph */
    spi_release(LSM303D_PARAM_SPI);

    if (IS_ACTIVE(ENABLE_DEBUG) && ((res[1] & 0x7) != 0)) {
        DEBUG("[OK]\r\n");
    }
    else {
        DEBUG("[Failed]\r\n");
    }

    return ((res[1] & 0x7) != 0) ? 1 : 0;
}




/* Read magnetic data */
void lsm303d_read_raw_data(uint8_t *res)
{
    uint8_t tmp[7] = {0};

    DEBUG("lsm303d read data...\r\n");
    /* Acquire exclusive access to the bus. */
    spi_acquire(LSM303D_PARAM_SPI, LSM303D_PARAM_SPI_CS, SPI_MODE_0, SPI_CLK_100KHZ);
    
    /* bit0 - 5 = 0x0F ; bit 6 = 0 ; bit 7 = 1 */
    tmp[0] |= (0x1 << 7); // READ
    tmp[0] |= (0x1 << 6); // INCREMENTE ADDRESS
    tmp[0] |= (LSM303D_REG_OUT_X_L_M);
    /* read the OUT_X_L_M register */
    spi_transfer_bytes (LSM303D_PARAM_SPI, (spi_cs_t) LSM303D_PARAM_SPI_CS, 0, tmp, res, 7);

    /* Release the SPI periph */
    spi_release(LSM303D_PARAM_SPI);
}




/* Read magnetic data */
void lsm303d_read_float_data(float *res)
{
    uint8_t raw_res[7] = {0};

    DEBUG("lsm303d float data...\r\n");

    /* get raw data */
    lsm303d_read_raw_data(raw_res);

    /* convert raw data to gauss value */
    res[0] = ((raw_res[2] << 8) | raw_res[1]) * LSM303D_M_SENSITIVITY_12G;
    res[1] = ((raw_res[4] << 8) | raw_res[3]) * LSM303D_M_SENSITIVITY_12G;
    res[2] = ((raw_res[6] << 8) | raw_res[5]) * LSM303D_M_SENSITIVITY_12G;

    if (IS_ACTIVE(ENABLE_DEBUG)) {
        DEBUG("[OK]\r\n");
    }
    else {
        DEBUG("[Failed]\r\n");
    }
}



