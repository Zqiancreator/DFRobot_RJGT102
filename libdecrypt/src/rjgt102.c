/**
 **************************************************************************************
 *                Copyright(c) 2015 Wuhan Runjet, All rights reserved.
 **************************************************************************************
 * @file     rjgt102.c
 * @author   Wuhan Runjet software and test team
 * @date     04-Feb-2015
 * @version  V1.00
 * @brief    Header file for RJGT102 chip.
 **************************************************************************************
 *
 */ 
#include <string.h>
#include "rjgt102.h"



/**@brief I2C������װ�궨��
 */
#define RJGT102_I2cOpen		       S_Rjgt102I2c->Open
#define RJGT102_I2cClose	       S_Rjgt102I2c->Close
#define RJGT102_I2cReadByte		   S_Rjgt102I2c->ReadByte
#define RJGT102_I2cWriteByte	   S_Rjgt102I2c->WriteByte
#define RJGT102_I2cRead            S_Rjgt102I2c->Read
#define RJGT102_I2cWrite		   S_Rjgt102I2c->Write


/**
 * @brief ��̬ȫ�ֱ�������
 */
static volatile uint32_t S_dwSeed = 0x0230A29F;
static volatile I2C_Driver_t *S_Rjgt102I2c = {0x00};



/**
 ***************************************************************************
 * @brief  ����һ��32λ�����������
 * @param  ��
 * @retval ����һ��32λ�����������
 ***************************************************************************
 */
static uint32_t Rand(void)
{
    S_dwSeed = S_dwSeed * 1103515245L + 12345;

    return (uint32_t)(S_dwSeed / 65536L) % 32768L;
}

/**
 ***************************************************************************
 * @brief  ����8���ֽڵ������
 * @param  [OUT] pRngBuf  ָ��洢8�ֽ��������������
 * @retval ��
 ***************************************************************************
 */
void RJGT102_GenerateRandom(uint8_t *pRngBuf)
{	
    uint32_t RngBuf[2];
    uint8_t  i;
	
    RngBuf[0] = Rand();
    RngBuf[1] = Rand();
    for (i = 0; i < 4; i++)
    {
        pRngBuf[i] = (uint8_t)((RngBuf[0] >> (8 * i)) & 0xFF);
        pRngBuf[i + 4] = (uint8_t)((RngBuf[1] >> (8 * i)) & 0xFF);
    }
}


/**
 ***************************************************************************
 * @brief  ��ȡִ��״̬
 * @param  ��
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 *         @arg 0x00 -������ִ����
 *         @arg 0x01 -������ȷִ�����
 *         @arg 0x11 -�������ִ�����
 *         @arg 0x10 -����ִ�зǷ�
 *         @arg 0xFF -����ִ�г�ʱ
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_GetExecutionStatus(void)
{
    uint8_t ExecutionStatus = 0x00;
    uint32_t TimeOut = CMD_EXECUTION_TIMEOUT;
	
    do
    {
        RJGT102_I2cReadByte(&ExecutionStatus, REG_ES);
        ExecutionStatus &= 0x11;
    } while((!ExecutionStatus) && (--TimeOut));
	
    if (0x00 == TimeOut)
    {
        ExecutionStatus = 0xFF;
    }

    return ((ExecutionStatus_t)ExecutionStatus);
}


/**
 ***************************************************************************
 * @brief  �汣оƬI2C����ע��
 * @param  [IN] pI2cDriver  ָ��I2C���� @ref I2C_Driver_t
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
void RJGT102_I2cDriverRegister(I2C_Driver_t *pI2cDriver)
{
	S_Rjgt102I2c = pI2cDriver;
}


/**
 ***************************************************************************
 * @brief  �汣оƬ��ʼ��
 * @param  ��
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_Init(void)
{
	ExecutionStatus_t ExecutionStatus = ES_ERROR;
	
	RJGT102_I2cOpen();
	
	/*!< �ϵ��,�����˾�汣оƬ */
	if (RJGT102_CheckVersion())
	{
		/*!< ָ����PAGE������ʱ��������� */
		ExecutionStatus = RJGT102_ConfigReadPageMode(PAGE_ENCRYPT_OUTPUT);
	}
	
	return ExecutionStatus;
}


/**
 ***************************************************************************
 * @brief  ��ʼ��USID�û�ID��
 * @param  [IN] pUsidBuf  ָ��8�ֽڳ�ʼ���û�ID������
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_InitUsid(const uint8_t *pUsidBuf)
{
    uint8_t UsidBuf[8];
	
    memcpy(UsidBuf, pUsidBuf, 8);
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWrite(REG_MEMBUF, UsidBuf, 8);
    RJGT102_I2cWriteByte(CMD_INITUSID, REG_MCMD);
	
    return (RJGT102_GetExecutionStatus());
}

/**
 ***************************************************************************
 * @brief  ��ʼ��ָ��PAGEx(x=0-3)��
 * @param  [IN] PageAddr  ָ������ʼ��PAGEx(x=0-3)�ĵ�ַ
 *         [IN] pPageBuf  ָ��32�ֽڳ�ʼ��PAGE������
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_InitPage(uint8_t PageAddr, const uint8_t *pPageBuf)
{
    uint8_t PageBuf[32];
	
    memcpy(PageBuf, pPageBuf, 32);
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWrite(REG_MEMBUF, PageBuf, 32);
    RJGT102_I2cWriteByte(PageAddr, REG_TAd);
    RJGT102_I2cWriteByte(CMD_INITPAGE, REG_MCMD);
	
    return (RJGT102_GetExecutionStatus());
}

/**
 ***************************************************************************
 * @brief  ��ʼ��KEY��Կ��
 * @param  [IN] pKeyBuf  ָ��8�ֽڳ�ʼ��KEY������
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_InitKey(const uint8_t *pKeyBuf)
{
    uint8_t KeyBuf[8];
	
    memcpy(KeyBuf, pKeyBuf, 8);
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWrite(REG_MEMBUF, KeyBuf, 8);	
    RJGT102_I2cWriteByte(CMD_INITKEY, REG_MCMD);
	
    return (RJGT102_GetExecutionStatus());
}

/**
 ***************************************************************************
 * @brief  ������֤оƬ�Դ�оƬ��ȡ32�ֽ�MACֵ
 * @param  [IN]  PageAddr  ָ������MAC�����PAGEx(x=0-3)�ĵ�ַ
 *         [IN]  pRngBuf   ָ�����MAC�����8�ֽ������
 *         [OUT] pMacBuf   ָ�������洢32�ֽ���Ϣ��֤��MAC��������
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_AuthenticationDevice(uint8_t PageAddr, const uint8_t *pRngBuf, uint8_t *pMacBuf)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t RngBuf[8];
	
    memcpy(RngBuf, pRngBuf, 8);
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWrite(REG_MEMBUF, RngBuf, 8);	
    RJGT102_I2cWriteByte(PageAddr, REG_TAr);
    RJGT102_I2cWriteByte(CMD_AUTHENTICATION, REG_MCMD);

    ExecutionStatus = RJGT102_GetExecutionStatus();
    if (ES_NORMAL == ExecutionStatus)
    {
        RJGT102_I2cRead(REG_MEMBUF, pMacBuf, 32);
    }
	
    return ExecutionStatus;
}

/**
 ***************************************************************************
 * @brief  ����KEY��Կ��
 * @param  [IN] PageAddr ָ������MAC�����PAGEx(x=0-3)�ĵ�ַ
 *         [IN] pRngBuf  ָ�����MAC�����8�ֽ�������洢��
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_UpdateKey(uint8_t PageAddr, const uint8_t *pRngBuf)
{
    uint8_t RngBuf[8];
	
    memcpy(RngBuf, pRngBuf, 8);
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWrite(REG_MEMBUF, RngBuf, 8);
    RJGT102_I2cWriteByte(PageAddr, REG_TAr);
    RJGT102_I2cWriteByte(CMD_GENERATEKEY, REG_MCMD);

    return (RJGT102_GetExecutionStatus());
}

/**
 ***************************************************************************
 * @brief  ��ָ���Ŀ��ƼĴ���(��ַ��Χ��0xA0~0xAE)д��1�ֽ�����
 * @param  [IN] Addr  	  ָ����д���ַ
 *         [IN] ByteData  ָ����д����ֽ�����
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_WriteCtrlReg(uint8_t Addr, uint8_t ByteData)
{
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWriteByte(ByteData, REG_MEMBUF);
    RJGT102_I2cWriteByte(Addr, REG_TAd);
    RJGT102_I2cWriteByte(CMD_WRITE, REG_MCMD);
	
    return (RJGT102_GetExecutionStatus());
}

/**
 ***************************************************************************
 * @brief  ��ָ���Ŀ��ƼĴ���(��ַ��Χ��0xA0-0xAF)����1�ֽ�����
 * @param  [IN]  Addr 	    ָ����������ַ	 
 *         [OUT] pByteData  ָ��������ֽ����ݴ洢��
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_ReadCtrlReg(uint8_t Addr, uint8_t *pByteData)
{
    ExecutionStatus_t ExecutionStatus;
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWriteByte(Addr, REG_TAd);
    RJGT102_I2cWriteByte(CMD_READ, REG_MCMD);

    ExecutionStatus = RJGT102_GetExecutionStatus();
    if (ES_NORMAL == ExecutionStatus)
    {
        RJGT102_I2cReadByte(pByteData, REG_MEMBUF);
    }
	
    return ExecutionStatus;
}

/**
 ***************************************************************************
 * @brief  ��ָ����Ŀ��PAGEx(x=0-3)��֤д��32�ֽ�����
 * @param  [IN] SrcPageAddr  ָ������MAC�����ԴPAGE��ַ	 
 *         [IN] DstPageAddr  ָ����д���Ŀ��PAGE��ַ
 *         [IN] pPageBuf     ָ���д���32�ֽ�PAGE����(ע��:���ݵ�8�ֽ���Ϊ
 *                           ���������,����MAC���㣬��Ӳ������MACʱ����ͬ
 *                           ��������Ϊ���������)
 *         [IN] pMacBuf	     ָ���д���32�ֽ�MAC����(��MAC������������ã�
 *                           д��оƬ�ڲ����Ƚ���֤����ȷ��ɽ�����д��Ŀ��
 *                           PAGE�������򱨴�)
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_WritePage(uint8_t SrcPageAddr, uint8_t DstPageAddr, const uint8_t *pPageBuf, const uint8_t *pMacBuf)
{
    uint8_t DataBuf[32];
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
	
    memcpy(DataBuf, pPageBuf, 32);
    RJGT102_I2cWrite(REG_MEMBUF, DataBuf, 32); 
    
    memcpy(DataBuf, pMacBuf, 32);
    RJGT102_I2cWrite(REG_MACBUF, DataBuf, 32);
	
    RJGT102_I2cWriteByte(SrcPageAddr, REG_TAr);
    RJGT102_I2cWriteByte(DstPageAddr, REG_TAd);
    RJGT102_I2cWriteByte(CMD_WRITE, REG_MCMD);
	
    return (RJGT102_GetExecutionStatus());
}

/**
 ***************************************************************************
 * @brief  ��ָ����Ŀ��PAGEx(x=0-3)��֤��������
 * @param  [IN]  SrcPageAddr  ָ������MAC�����ԴPAGE��ַ 
 *         [IN]  DstPageAddr  ָ����������Ŀ��PAGE��ַ  
 *         [IN]  pRngBuf      ָ�����MAC�����8�ֽ�������洢��
 *         [IN]  pMacBuf      ָ���д���32�ֽ�MAC����(��MAC������������ã�
 *                            д��оƬ�ڲ����Ƚ���֤�����ʱ�������PAGE����
 *                            �����ݣ����򱨴�
 *         [OUT] pPageBuf     ָ�������PAGE���ݴ洢��
 *         [IN]  LenOfPage    ָ�������������ݳ���
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_ReadPage(uint8_t SrcPageAddr,
                                   uint8_t DstPageAddr,
                                   const uint8_t *pRngBuf,
                                   const uint8_t *pMacBuf,
                                   uint8_t *pPageBuf,
                                   uint8_t LenOfPage)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t i;
    uint8_t TmpReg;
    uint8_t DataBuf[32];
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
	
    memcpy(DataBuf, pRngBuf, 8);
    RJGT102_I2cWrite(REG_MEMBUF, DataBuf, 8);
	
    memcpy(DataBuf, pMacBuf, 32);
    RJGT102_I2cWrite(REG_MACBUF, DataBuf, 32);
	
    RJGT102_I2cWriteByte(SrcPageAddr, REG_TAr);
    RJGT102_I2cWriteByte(DstPageAddr, REG_TAd);
    RJGT102_I2cWriteByte(CMD_READ, REG_MCMD);
	
    ExecutionStatus = RJGT102_GetExecutionStatus();
    if (ES_NORMAL == ExecutionStatus)
    {
        RJGT102_I2cRead(REG_MEMBUF, pPageBuf, LenOfPage);
        ExecutionStatus = RJGT102_ReadCtrlReg(REG_WDGRSTCTRL, &TmpReg);
        if (ES_NORMAL == ExecutionStatus)
        {
            if (0x00 == (TmpReg & SHA_RD_BYPASS))
            {
                for (i = 0; i < LenOfPage; i++)
                {
                    pPageBuf[i] ^= pMacBuf[i]; 
                }
            } 	
        }
    }
	
    return ExecutionStatus;
}

/**
 ***************************************************************************
 * @brief  ���Ź���ʼ��
 * @param  [IN] Period          ��λ����(��Χ��0x000000-0xFFFFFF)
 *         [IN] RstSignalWidth  ��λ��Ч�źſ��(��Χ��0x000000-0xFFFFFF)
 *         [IN] RstPolarity     ��λ�źż��� @ref WDG_RstPolarity_t
 *                              @arg LOW_VALID   ��λ�źŵ���Ч
 *                              @arg HIGH_VALID  ��λ�źŸ���Ч
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_WdogInit(uint32_t Period, uint32_t RstSignalWidth, WDG_RstPolarity_t RstPolarity)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t RegAddr;
    uint8_t TmpReg;
    uint8_t i;

    for (i = 0; i < 3; i++)
    {		
        TmpReg = (uint8_t)((Period >> 8*i) & 0xFF);
        RegAddr = REG_WDGCNT + i;
        ExecutionStatus = RJGT102_WriteCtrlReg(RegAddr, TmpReg);
        if (ES_NORMAL != ExecutionStatus)
        {
            goto RetExeStatus;
        }
        TmpReg = (uint8_t)((RstSignalWidth >> 8*i) & 0xFF);
        RegAddr = REG_RSTCNT + i;
        ExecutionStatus = RJGT102_WriteCtrlReg(RegAddr, TmpReg);
        if (ES_NORMAL != ExecutionStatus)
        {
            goto RetExeStatus;
        }
    }
	
    ExecutionStatus = RJGT102_ReadCtrlReg(REG_WDGRSTCTRL, &TmpReg);
    if (ES_NORMAL == ExecutionStatus)
    {
        TmpReg &= ~(RST_POLARITY | RST_EN_N);
        if (HIGH_VALID == RstPolarity)
        {
            TmpReg |= RST_POLARITY;
        }
        ExecutionStatus = RJGT102_WriteCtrlReg(REG_WDGRSTCTRL, TmpReg);
    }

RetExeStatus:	
    return ExecutionStatus;
}

/**
 ***************************************************************************
 * @brief  ���Ź�����
 * @param  [IN] NewState  ���Ź�����״̬
 *                        @arg WDG_ENABLE   ���Ź�����
 *                        @arg WDG_DISABLE  ���Ź�ֹͣ 
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_WdogCmd(WDG_CtrlState_t NewState)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t TmpReg;
    uint8_t TmpReg1;

    RJGT102_I2cRead(REG_SYSCTRL, &TmpReg1, 1);
    ExecutionStatus = RJGT102_ReadCtrlReg(REG_WDGRSTCTRL, &TmpReg);
    if (ES_NORMAL == ExecutionStatus)
    {
        TmpReg1 &= ~WDOG_EN_REG; 
        TmpReg &= ~WDOG_EN;
        if (WDG_DISABLE != NewState)
        {	
            TmpReg |= WDOG_EN;
            TmpReg1 |= WDOG_EN_REG;
        }
        ExecutionStatus = RJGT102_WriteCtrlReg(REG_WDGRSTCTRL, TmpReg);
        if (ES_NORMAL == ExecutionStatus)
        {
            RJGT102_I2cWriteByte(TmpReg1, REG_SYSCTRL);
        }
    }
	
    return ExecutionStatus;
}

/**
 ***************************************************************************
 * @brief  ���Ź�ι��
 * @param  ��
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_WdogFeed(void)
{
    return (RJGT102_WdogCmd(WDG_ENABLE)); 
}

/**
 ***************************************************************************
 * @brief  ��ȡоƬREG_USID�û�ID
 * @param  [OUT] pUsidBuf  ָ����8�ֽ��û�ID�����ݻ�����
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_ReadUsid(uint8_t *pUsidBuf)
{
    ExecutionStatus_t ExecutionStatus;
	
    RJGT102_I2cWriteByte(CMD_CLEARCMD, REG_MCMD);
    RJGT102_I2cWriteByte(REG_USID, REG_TAd);
    RJGT102_I2cWriteByte(CMD_READ, REG_MCMD);

    ExecutionStatus = RJGT102_GetExecutionStatus();
    if (ES_NORMAL == ExecutionStatus)
    {
        RJGT102_I2cRead(REG_MEMBUF, pUsidBuf, 8);	
    }
	
    return ExecutionStatus;
}

/**
 ***************************************************************************
 * @brief  ��ָ���������ñ���
 * @param  [IN] Addr  ָ������������ĵ�ַ
 *         @arg PAGExPRO(x=0-3)  PAGEx���豣��
 *         @arg KEYPRO           KEY���豣��
 *         @arg UIDPRO           REG_USID���豣��
 *         @arg PRT_CTRL         ���ƼĴ�����(0xA0~0xA6)�豣��
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_SetProtection(uint8_t Addr)
{
    RJGT102_WriteCtrlReg(Addr, REGIONAL_PROTECTION_CODE);
	
    return (RJGT102_GetExecutionStatus());
}

/**
 ***************************************************************************
 * @brief  ��ȡоƬ�汾��
 * @param  [IN] pVersionBuf  ָ��洢4�ֽڰ汾�ŵĻ�����
 * @retval ��
 ***************************************************************************
 */
void RJGT102_GetVersion(uint8_t *pVersionBuf)
{
    RJGT102_I2cRead(REG_VERSION0, pVersionBuf, 4);
}

/**
 ***************************************************************************
 * @brief  ����Ƿ�����˾�汣оƬ
 * @param  ��
 * @retval ����״ֵ̬
 *         @arg 0x00  ��ʾ���ǻ�û���ҵ���˾оƬ
 *         @arg 0x01  ��ʾ����˾оƬ
 ***************************************************************************
 */
uint8_t RJGT102_CheckVersion(void)
{
    const uint8_t Version[4] = {0x32, 0x30, 0x84, 0x71};
    uint8_t VersionBuf[4];
	
    RJGT102_I2cRead(REG_VERSION0, VersionBuf, 4);
	
    return (!memcmp(VersionBuf, Version, 4));
}

/**
 ***************************************************************************
 * @brief  ��PAGEʱ�����ģʽ����
 * @param  [IN] mode @ref PageOutputMode_t
 *         @arg PAGE_ENCRYPT_OUTPUT  ��PAGEʱ���������	
 *         @arg PAGE_DIRECT_OUTPUT   ��PAGEʱ��ֱ�����
 * @retval ����״ֵ̬ @ref ExecutionStatus_t
 ***************************************************************************
 */
ExecutionStatus_t RJGT102_ConfigReadPageMode(PageOutputMode_t mode)
{	
    ExecutionStatus_t ExecutionStatus;
    uint8_t TmpReg;

    ExecutionStatus = RJGT102_ReadCtrlReg(REG_WDGRSTCTRL, &TmpReg);
    if (ES_NORMAL == ExecutionStatus)
    {
        if (PAGE_ENCRYPT_OUTPUT != mode)
        {
            TmpReg |= SHA_RD_BYPASS;
        }
        else
        {
            TmpReg &= ~SHA_RD_BYPASS;
        }

        ExecutionStatus = RJGT102_WriteCtrlReg(REG_WDGRSTCTRL, TmpReg);	
    }
	
    return ExecutionStatus;
}

