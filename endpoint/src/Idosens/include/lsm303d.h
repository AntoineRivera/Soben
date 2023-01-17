/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSM303D_H
#define __LSM303D_H

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


#define LSM303D_REG_TEMP_OUT_L    	0x05
#define LSM303D_REG_TEMP_OUT_H    	0x06

#define LSM303D_REG_STATUS_M      	0x07
#define LSM303D_REG_OUT_X_L_M     	0x08
#define LSM303D_REG_OUT_X_H_M     	0x09
#define LSM303D_REG_OUT_Y_L_M     	0x0a
#define LSM303D_REG_OUT_Y_H_M     	0x0b
#define LSM303D_REG_OUT_Z_L_M     	0x0c
#define LSM303D_REG_OUT_Z_H_M     	0x0d

#define LSM303D_REG_WHO_AM_I      	0x0f

#define LSM303D_REG_INT_CTRL_M    	0x12
#define LSM303D_REG_INT_SRC_M     	0x13
#define LSM303D_REG_INT_THS_L_M   	0x14
#define LSM303D_REG_INT_THS_H_M   	0x15
#define LSM303D_REG_OFFSET_X_L_M  	0x16
#define LSM303D_REG_OFFSET_X_M_M  	0x17
#define LSM303D_REG_OFFSET_Y_L_M  	0x18
#define LSM303D_REG_OFFSET_Y_M_M  	0x19
#define LSM303D_REG_OFFSET_Z_L_M  	0x1a
#define LSM303D_REG_OFFSET_Z_M_M  	0x1b

#define LSM303D_REG_REFERENCE_X   	0x1c
#define LSM303D_REG_REFERENCE_Y   	0x1d
#define LSM303D_REG_REFERENCE_Z   	0x1e

#define LSM303D_REG_CTRL0         	0x1f
#define LSM303D_REG_CTRL1         	0x20
#define LSM303D_REG_CTRL2         	0x21
#define LSM303D_REG_CTRL3         	0x22
#define LSM303D_REG_CTRL4         	0x23
#define LSM303D_REG_CTRL5         	0x24
#define LSM303D_REG_CTRL6         	0x25
#define LSM303D_REG_CTRL7         	0x26

#define LSM303D_REG_STATUS_A      	0x27
#define LSM303D_REG_OUT_X_L_A     	0x28
#define LSM303D_REG_OUT_X_H_A     	0x29
#define LSM303D_REG_OUT_Y_L_A     	0x2a
#define LSM303D_REG_OUT_Y_H_A     	0x2b
#define LSM303D_REG_OUT_Z_L_A     	0x2c
#define LSM303D_REG_OUT_Z_H_A     	0x2d

#define LSM303D_REG_FIFO_CTRL     	0x2e
#define LSM303D_REG_FIFO_SRC      	0x2f

#define LSM303D_REG_IG_CFG1       	0x30
#define LSM303D_REG_IG_SRC1       	0x31
#define LSM303D_REG_IG_THS1       	0x32
#define LSM303D_REG_IG_DUR1       	0x33
#define LSM303D_REG_IG_CFG2       	0x34
#define LSM303D_REG_IG_SRC2       	0x35
#define LSM303D_REG_IG_THS2       	0x36
#define LSM303D_REG_IG_DUR2       	0x37
#define LSM303D_REG_CLICK_CFG     	0x38
#define LSM303D_REG_CLICK_SRC     	0x39
#define LSM303D_REG_CLICK_THS     	0x3a
#define LSM303D_REG_TIME_LIMIT    	0x3b
#define LSM303D_REG_TIME_LATENCY  	0x3c
#define LSM303D_REG_TIME_WINDOW   	0x3d

#define LSM303D_M_SENSITIVITY_2G  	(0.080/1000)
#define LSM303D_M_SENSITIVITY_4G  	(0.160/1000)
#define LSM303D_M_SENSITIVITY_8G  	(0.320/1000)
#define LSM303D_M_SENSITIVITY_12G 	(0.479/1000)


/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/* sensor available ? */
int lsm303d_who_i_am(void);
/* Init mag register */
void lsm303d_init(void);
/* Is magnetic data available ? */
int lsm303d_is_data_available(void);
/* Read magnetic data */
void lsm303d_read_raw_data(uint8_t *res);
/* Read magnetic data */
void lsm303d_read_float_data(float *res);

#endif /* __LSM303D_H */
