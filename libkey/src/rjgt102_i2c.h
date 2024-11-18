/**
 **************************************************************************************
 *                Copyright(c) 2015 Wuhan Runjet, All rights reserved.
 **************************************************************************************
 * @file     rjgt102_i2c.h
 * @author   Wuhan Runjet software and test team
 * @date     04-Feb-2015
 * @version  V1.00
 * @brief    版保芯片的I2C接口硬件抽象层
 **************************************************************************************
 *
 */
#ifndef __RJGT102_I2C_H__
#define __RJGT102_I2C_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>


	 
/**@brief I2C驱动结构体
 */
typedef struct I2C_DRIVER
{
	void (*Open)(void);
	void (*ReadByte)(uint8_t *pByteData, uint8_t AddrToRead);			  	            			
    void (*WriteByte)(uint8_t ByteData, uint8_t AddrToWrite);
    void (*Read)(uint8_t AddrToRead, uint8_t *pData, uint16_t Length);
    void (*Write)(uint8_t AddrToWrite, uint8_t *pData, uint16_t Length);
	void (*Close)(void);
} I2C_Driver_t;

		 
#ifdef __cplusplus
}
#endif

#endif /* end of __RJGT102_I2C_H__ */

