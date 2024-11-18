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
 * @brief ����궨��
 */
#define RJGT102_I2C_ADDR          ((uint8_t)0x68)         /*!< оƬI2C�ӿڵ�ַ                */
#define RJGT102_VERSION           ((uint32_t)0x71843032); /*!< оƬ�汾��                     */
#define RJGT102_WATCHDOG_CLK      ((uint32_t)3500000)     /*!< оƬ���Ź�ʱ�ӣ�3.5MHz         */
#define	REGIONAL_PROTECTION_CODE  ((uint8_t)0x5A)         /*!< ���򱣻���                     */
#define CMD_EXECUTION_TIMEOUT     ((uint32_t)0xF0000);    /*!< ����ִ�еĳ�ʱ����(��Ҫʱ�ɸ�) */


/**
 * @brief ����ִ��״̬
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
 * @brief ��PAGEʱ�����ģʽ
 */
typedef enum
{
    PAGE_ENCRYPT_OUTPUT = 0x00,
    PAGE_DIRECT_OUTPUT  = !PAGE_ENCRYPT_OUTPUT
} PageOutputMode_t;

/**
 * @brief ���Ź�����
 */
typedef enum
{
    WDG_DISABLE = 0,
    WDG_ENABLE  = !WDG_DISABLE
} WDG_CtrlState_t;

/**
 * @brief ���Ź���λ�źż���
 */
typedef enum 
{
    LOW_VALID  = 0,
    HIGH_VALID = !LOW_VALID
} WDG_RstPolarity_t;


/*** @brief �������� ###################################################################
  */
#define	CMD_INITUSID            ((uint8_t)0xAA)         /*!< ��ʼ���û�ID����         */
#define	CMD_INITPAGE            ((uint8_t)0xA5)         /*!< ��ʼ��PAGE����           */
#define	CMD_INITKEY             ((uint8_t)0x5A)         /*!< ��ʼ��KEY����            */
#define	CMD_AUTHENTICATION      ((uint8_t)0x55)         /*!< ��֤�豸����             */
#define	CMD_READ                ((uint8_t)0x0F)         /*!< ������                   */
#define	CMD_WRITE               ((uint8_t)0xF0)         /*!< д����                   */ 
#define	CMD_GENERATEKEY         ((uint8_t)0x33)         /*!< ����KEY����              */
#define CMD_CLEARCMD            ((uint8_t)0x00)         /*!< ������Ĵ�������         */

/*** @brief �Ĵ������� #################################################################
  */
#define	REG_PAGE0               ((uint8_t)0x00)         /*!< PAGE0�Ĵ���              */
#define	REG_PAGE1               ((uint8_t)0x20)         /*!< PAGE1�Ĵ���              */
#define	REG_PAGE2               ((uint8_t)0x40)         /*!< PAGE2�Ĵ���              */
#define	REG_PAGE3               ((uint8_t)0x60)         /*!< PAGE3�Ĵ���              */
#define	REG_KEY                 ((uint8_t)0x80)         /*!< KEY�Ĵ���                */  
#define	REG_USID                ((uint8_t)0x90)         /*!< �û�ID�Ĵ���             */  
#define	REG_WDGCNT              ((uint8_t)0xA0)         /*!< 24λ���Ź�������         */  	
#define	REG_WDGRSTCTRL          ((uint8_t)0xA3)         /*!< ���Ź���λ����ƼĴ���   */  
#define	REG_RSTCNT              ((uint8_t)0xA4)         /*!< ���Ź���λ�źſ�ȼĴ��� */													
#define	REG_PAGE0PRO            ((uint8_t)0xA8)         /*!< PAGE0�����Ĵ���          */  
#define	REG_PAGE1PRO            ((uint8_t)0xA9)         /*!< PAGE1�����Ĵ���          */  
#define	REG_PAGE2PRO            ((uint8_t)0xAA)         /*!< PAGE2�����Ĵ���          */  
#define	REG_PAGE3PRO            ((uint8_t)0xAB)         /*!< PAGE3�����Ĵ���          */  
#define	REG_KEYPRO              ((uint8_t)0xAC)         /*!< KEY�����Ĵ���            */  
#define	REG_UIDPRO              ((uint8_t)0xAD)         /*!< USID�����Ĵ���           */  
#define	REG_PRT_CTRL            ((uint8_t)0xAE)         /*!< (0xA0~0xA6)�����Ĵ���    */  
#define	REG_DISABLE_INIT_PAGE   ((uint8_t)0xAF)         /*!< InitPageʧЧ�Ĵ���(ֻ��) */ 
#define	REG_MCMD                ((uint8_t)0xB0)         /*!< ����Ĵ���               */
#define	REG_TAr                 ((uint8_t)0xB1)         /*!< ԴPAGE�Ĵ���             */
#define	REG_TAd                 ((uint8_t)0xB2)         /*!< Ŀ��PAGE�Ĵ���           */
#define	REG_ES                  ((uint8_t)0xB3)         /*!< ����״̬�Ĵ���           */
#define	REG_SYSCTRL             ((uint8_t)0xB4)         /*!< ϵͳ���ƼĴ���           */
#define REG_VERSION0            ((uint8_t)0xB8)         /*!< оƬ�汾�żĴ���         */
#define	REG_MEMBUF              ((uint8_t)0xC0)         /*!< ���ݽ������Ĵ���         */	
#define	REG_MACBUF              ((uint8_t)0xE0)         /*!< MAC����Ĵ���            */

/*** @brief �Ĵ���λ���� ###############################################################
  */

/** @brief ���Ź���λ����ƼĴ���REG_WDGRSTCTRL(0xA3)λ����
 *
 *######################################################################################
 *  @����λ      @λ����        @��λֵ                      @����
 *--------------------------------------------------------------------------------------
 *    0           WDOG_EN          0           0�����Ź�ֹͣ        1�����Ź�����
 *    1           RFU              0           x: δ����        
 *    2           SHA_RD_BYPASS    0           0����PAGEʱ�������  1����PAGEʱֱ�����  
 *    3           RFU              0           x: δ����         
 *    4           RST_EN_N         0           0����λ���ʹ��      1����λ���ʧ�� 
 *    5           RST_POLARITY     0           0����λ����Ч        1����λ����Ч
 *    6           RFU              0           x: δ����        
 *    7           RFU              0           x: δ����        
 *######################################################################################
 */
#define	WDOG_EN                 ((uint8_t)(1u << 0))    /*!< ���Ź�ʹ�ܿ���λ         */
#define	SHA_RD_BYPASS           ((uint8_t)(1u << 2))    /*!< ��PAGEʱ�����ʽ����λ   */
#define RST_EN_N                ((uint8_t)(1u << 4))    /*!< ���Ź���λ�ź��������λ */
#define RST_POLARITY            ((uint8_t)(1u << 5))    /*!< ���Ź���λ�źż��Կ���λ */

/** @brief ִ��״̬�Ĵ���REG_ES(0xB3)λ����
 *
 *######################################################################################
 *  @����λ      @λ����        @��λֵ                      @����
 *--------------------------------------------------------------------------------------
 *    0          ES_DONE           0           0��������ִ����        1������ִ����� 
 *    1          RFU               0           x: δ����        
 *    2          RFU               0           x: δ����
 *    3          RFU               0           x: δ����         
 *    4          ES_ERR            0           0������                1������ 
 *    5          RFU               0           x: δ����    
 *    6          RFU               0           x: δ����        
 *    7          RFU               0           x: δ����        
 *######################################################################################
 */
#define ES_DONE                 ((uint8_t)(1u << 0))    /*!< ����ִ�����״̬λ       */
#define ES_ERR                  ((uint8_t)(1u << 4))    /*!< ����ִ�д���״̬λ       */
                                                                                     
/** @brief ϵͳ���ƼĴ���REG_SYSCTRL(0xB4)λ����
 *
 *######################################################################################
 *  @����λ      @λ����        @��λֵ                      @����
 *--------------------------------------------------------------------------------------
 *    0          LowPower_Bypass   0           0����������״̬        1������͹��� 
 *    1          RFU               0           x: δ����        
 *    2          RFU               0           x: δ����
 *    3          RFU               0           x: δ����         
 *    4          Wdog_En_Reg       0           0�����Ź�ʧ��          1�����Ź�ʹ��
 *    5          RFU               0           x: δ����    
 *    6          RFU               0           x: δ����        
 *    7          RFU               0           x: δ����        
 *######################################################################################
 */
#define LOWPOWER_EN             ((uint8_t)(1u << 0))    /*!< �͹���ʹ��λ             */
#define WDOG_EN_REG             ((uint8_t)(1u << 4))    /*!< ���Ź�ʹ��λ             */



/**
  *@brief ��������
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

