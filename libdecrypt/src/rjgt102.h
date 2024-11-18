/**
 **************************************************************************************
 *                Copyright(c) 2015 Wuhan Runjet, All rights reserved.
 **************************************************************************************
 * @file     rjgt102.h
 * @author   Wuhan Runjet software and test team
 * @date     04-Feb-2015
 * @version  V1.00
 * @brief    Header file for RJGT102 chip.
 **************************************************************************************
 *
 */
#ifndef __RJGT102_H__
#define __RJGT102_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "rjgt102_i2c.h"
	
	 

/**
 * @brief 杂项宏定义
 */
#define RJGT102_I2C_ADDR          ((uint8_t)0x68)         /*!< 芯片I2C接口地址                */
#define RJGT102_VERSION           ((uint32_t)0x71843032); /*!< 芯片版本号                     */
#define RJGT102_WATCHDOG_CLK      ((uint32_t)3500000)     /*!< 芯片看门狗时钟：3.5MHz         */
#define	REGIONAL_PROTECTION_CODE  ((uint8_t)0x5A)         /*!< 区域保护码                     */
#define CMD_EXECUTION_TIMEOUT     ((uint32_t)0xF0000);    /*!< 命令执行的超时周期(必要时可改) */


/**
 * @brief 命令执行状态
 */
typedef enum
{
    ES_NORMAL     = 0x01,
    ES_OPERATING  = 0x00, 
    ES_ERROR      = 0x11,
    ES_INVALID    = 0x10,
    ES_TIMEOUT    = 0xFF
} ExecutionStatus_t;

/**
 * @brief 读PAGE时的输出模式
 */
typedef enum
{
    PAGE_ENCRYPT_OUTPUT = 0x00,
    PAGE_DIRECT_OUTPUT  = !PAGE_ENCRYPT_OUTPUT
} PageOutputMode_t;

/**
 * @brief 看门狗控制
 */
typedef enum
{
    WDG_DISABLE = 0,
    WDG_ENABLE  = !WDG_DISABLE
} WDG_CtrlState_t;

/**
 * @brief 看门狗复位信号极性
 */
typedef enum 
{
    LOW_VALID  = 0,
    HIGH_VALID = !LOW_VALID
} WDG_RstPolarity_t;


/*** @brief 功能命令 ###################################################################
  */
#define	CMD_INITUSID            ((uint8_t)0xAA)         /*!< 初始化用户ID命令         */
#define	CMD_INITPAGE            ((uint8_t)0xA5)         /*!< 初始化PAGE命令           */
#define	CMD_INITKEY             ((uint8_t)0x5A)         /*!< 初始化KEY命令            */
#define	CMD_AUTHENTICATION      ((uint8_t)0x55)         /*!< 认证设备命令             */
#define	CMD_READ                ((uint8_t)0x0F)         /*!< 读命令                   */
#define	CMD_WRITE               ((uint8_t)0xF0)         /*!< 写命令                   */ 
#define	CMD_GENERATEKEY         ((uint8_t)0x33)         /*!< 更新KEY命令              */
#define CMD_CLEARCMD            ((uint8_t)0x00)         /*!< 清命令寄存器命令         */

/*** @brief 寄存器定义 #################################################################
  */
#define	REG_PAGE0               ((uint8_t)0x00)         /*!< PAGE0寄存器              */
#define	REG_PAGE1               ((uint8_t)0x20)         /*!< PAGE1寄存器              */
#define	REG_PAGE2               ((uint8_t)0x40)         /*!< PAGE2寄存器              */
#define	REG_PAGE3               ((uint8_t)0x60)         /*!< PAGE3寄存器              */
#define	REG_KEY                 ((uint8_t)0x80)         /*!< KEY寄存器                */  
#define	REG_USID                ((uint8_t)0x90)         /*!< 用户ID寄存器             */  
#define	REG_WDGCNT              ((uint8_t)0xA0)         /*!< 24位看门狗计数器         */  	
#define	REG_WDGRSTCTRL          ((uint8_t)0xA3)         /*!< 看门狗复位与控制寄存器   */  
#define	REG_RSTCNT              ((uint8_t)0xA4)         /*!< 看门狗复位信号宽度寄存器 */													
#define	REG_PAGE0PRO            ((uint8_t)0xA8)         /*!< PAGE0保护寄存器          */  
#define	REG_PAGE1PRO            ((uint8_t)0xA9)         /*!< PAGE1保护寄存器          */  
#define	REG_PAGE2PRO            ((uint8_t)0xAA)         /*!< PAGE2保护寄存器          */  
#define	REG_PAGE3PRO            ((uint8_t)0xAB)         /*!< PAGE3保护寄存器          */  
#define	REG_KEYPRO              ((uint8_t)0xAC)         /*!< KEY保护寄存器            */  
#define	REG_UIDPRO              ((uint8_t)0xAD)         /*!< USID保护寄存器           */  
#define	REG_PRT_CTRL            ((uint8_t)0xAE)         /*!< (0xA0~0xA6)保护寄存器    */  
#define	REG_DISABLE_INIT_PAGE   ((uint8_t)0xAF)         /*!< InitPage失效寄存器(只读) */ 
#define	REG_MCMD                ((uint8_t)0xB0)         /*!< 命令寄存器               */
#define	REG_TAr                 ((uint8_t)0xB1)         /*!< 源PAGE寄存器             */
#define	REG_TAd                 ((uint8_t)0xB2)         /*!< 目的PAGE寄存器           */
#define	REG_ES                  ((uint8_t)0xB3)         /*!< 错误状态寄存器           */
#define	REG_SYSCTRL             ((uint8_t)0xB4)         /*!< 系统控制寄存器           */
#define REG_VERSION0            ((uint8_t)0xB8)         /*!< 芯片版本号寄存器         */
#define	REG_MEMBUF              ((uint8_t)0xC0)         /*!< 数据交换区寄存器         */	
#define	REG_MACBUF              ((uint8_t)0xE0)         /*!< MAC输入寄存器            */

/*** @brief 寄存器位定义 ###############################################################
  */

/** @brief 看门狗复位与控制寄存器REG_WDGRSTCTRL(0xA3)位定义
 *
 *######################################################################################
 *  @比特位      @位定义        @复位值                      @含义
 *--------------------------------------------------------------------------------------
 *    0           WDOG_EN          0           0：看门狗停止        1：看门狗启动
 *    1           RFU              0           x: 未定义        
 *    2           SHA_RD_BYPASS    0           0：读PAGE时加密输出  1：读PAGE时直接输出  
 *    3           RFU              0           x: 未定义         
 *    4           RST_EN_N         0           0：复位输出使能      1：复位输出失能 
 *    5           RST_POLARITY     0           0：复位低有效        1：复位高有效
 *    6           RFU              0           x: 未定义        
 *    7           RFU              0           x: 未定义        
 *######################################################################################
 */
#define	WDOG_EN                 ((uint8_t)(1u << 0))    /*!< 看门狗使能控制位         */
#define	SHA_RD_BYPASS           ((uint8_t)(1u << 2))    /*!< 读PAGE时输出方式控制位   */
#define RST_EN_N                ((uint8_t)(1u << 4))    /*!< 看门狗复位信号输出控制位 */
#define RST_POLARITY            ((uint8_t)(1u << 5))    /*!< 看门狗复位信号极性控制位 */

/** @brief 执行状态寄存器REG_ES(0xB3)位定义
 *
 *######################################################################################
 *  @比特位      @位定义        @复位值                      @含义
 *--------------------------------------------------------------------------------------
 *    0          ES_DONE           0           0：命令正执行中        1：命令执行完成 
 *    1          RFU               0           x: 未定义        
 *    2          RFU               0           x: 未定义
 *    3          RFU               0           x: 未定义         
 *    4          ES_ERR            0           0：正常                1：错误 
 *    5          RFU               0           x: 未定义    
 *    6          RFU               0           x: 未定义        
 *    7          RFU               0           x: 未定义        
 *######################################################################################
 */
#define ES_DONE                 ((uint8_t)(1u << 0))    /*!< 命令执行完成状态位       */
#define ES_ERR                  ((uint8_t)(1u << 4))    /*!< 命令执行错误状态位       */
                                                                                     
/** @brief 系统控制寄存器REG_SYSCTRL(0xB4)位定义
 *
 *######################################################################################
 *  @比特位      @位定义        @复位值                      @含义
 *--------------------------------------------------------------------------------------
 *    0          LowPower_Bypass   0           0：正常工作状态        1：进入低功耗 
 *    1          RFU               0           x: 未定义        
 *    2          RFU               0           x: 未定义
 *    3          RFU               0           x: 未定义         
 *    4          Wdog_En_Reg       0           0：看门狗失能          1：看门狗使能
 *    5          RFU               0           x: 未定义    
 *    6          RFU               0           x: 未定义        
 *    7          RFU               0           x: 未定义        
 *######################################################################################
 */
#define LOWPOWER_EN             ((uint8_t)(1u << 0))    /*!< 低功耗使能位             */
#define WDOG_EN_REG             ((uint8_t)(1u << 4))    /*!< 看门狗使能位             */



/**
  *@brief 函数声明
  */
void RJGT102_I2cDriverRegister(I2C_Driver_t *pI2cDriver);
ExecutionStatus_t RJGT102_Init(void);
ExecutionStatus_t RJGT102_InitUsid(const uint8_t *pUsidBuf);
ExecutionStatus_t RJGT102_InitPage(uint8_t PageAddr, const uint8_t *pPageBuf);
ExecutionStatus_t RJGT102_InitKey(const uint8_t *pKeyBuf);
ExecutionStatus_t RJGT102_AuthenticationDevice(uint8_t PageAddr, const uint8_t *pRngBuf, uint8_t *pMacBuf); 
ExecutionStatus_t RJGT102_UpdateKey(uint8_t PageAddr, const uint8_t *pRngBuf);
ExecutionStatus_t RJGT102_WriteCtrlReg(uint8_t Addr, uint8_t ByteData);
ExecutionStatus_t RJGT102_ReadCtrlReg(uint8_t Addr, uint8_t *pByteData); 
ExecutionStatus_t RJGT102_WritePage(uint8_t SrcPageAddr, uint8_t DstPageAddr, const uint8_t *pPageBuf, const uint8_t *pMacBuf);
ExecutionStatus_t RJGT102_ReadPage(uint8_t SrcPageAddr, uint8_t DstPageAddr, const uint8_t *pRngBuf, const uint8_t *pMacBuf, uint8_t *pPageBuf, uint8_t LenOfPage); 
ExecutionStatus_t RJGT102_WdogInit(uint32_t Period, uint32_t RstSignalWidth, WDG_RstPolarity_t RstPolarity); 
ExecutionStatus_t RJGT102_WdogCmd(WDG_CtrlState_t NewState);
ExecutionStatus_t RJGT102_WdogFeed(void);
ExecutionStatus_t RJGT102_SetProtection(uint8_t Addr);
ExecutionStatus_t RJGT102_ReadUsid(uint8_t *pUsidBuf);
ExecutionStatus_t RJGT102_ConfigReadPageMode(PageOutputMode_t mode);
ExecutionStatus_t RJGT102_GetExecutionStatus(void);	
void RJGT102_GenerateRandom(uint8_t *pRngBuf);
void RJGT102_GetVersion(uint8_t *pVersionBuf);
uint8_t RJGT102_CheckVersion(void);

		 
#ifdef __cplusplus
}
#endif

#endif /* end of __RJGT102_H__ */

