/**
 **************************************************************************************
 *                Copyright(c) 2015 Wuhan Runjet, All rights reserved.
 **************************************************************************************
 * @file     rjgt102_security.c
 * @author   Wuhan Runjet software and test team
 * @date     04-Feb-2015
 * @version  V1.00
 * @brief    �汣оƬ��ȫ���ʹ���ʵ��
 **************************************************************************************
 *
 */
#include <string.h>
#include "rjgt102.h"
#include "sha256.h"
#include "rjgt102_security.h"



/**
 ***************************************************************************
 * @brief  ���CPU�Ķ�ģʽ
 * @param  ��
 * @retval ����ֵ��
 *         @arg 0x00  -С��ģʽ
 *         @arg 0x01  -���ģʽ
 ***************************************************************************
 */
static uint8_t CheckCpuEndian(void)
{
	uint32_t x = 0x04030201;
	
	return (*(uint8_t*)&x == 0x01) ?(0x00):(0x01);
}


/**
 ***************************************************************************
 * @brief  ���ض˼�����Ϣ��֤��MAC
 * @note   ��8�ֽ���Կ���ݡ�32�ֽ�PAGE���ݡ�8�ֽ��û�ID���ݡ�8�ֽ��������
 *         8�ֽڳ��������һ��64�ֽ���Ϣ����й�ϣ�������32�ֽ�MAC
 * @param  [IN]  pSecurityCtx  ָ��ȫ�����ı��� @ref SecurityContext_t
 *         [IN]  pRngBuf       ָ����8�ֽ��������������
 *         [OUT] pMacBuf       ָ��洢�����32�ֽ���Ϣ��֤��MAC������
 * @retval ��
 ***************************************************************************
 */
void MCU_CalculateMac(const SecurityContext_t *pSecurityCtx, const uint8_t *pRngBuf, uint8_t *pMacBuf)
{
    sha256_ctx_t ctx;
	uint8_t EndianMode;
    uint8_t i;
    uint8_t tmp;
    uint8_t MsgBuf[64];
    union convert
    {
        uint8_t byte[64];
        uint32_t word[16];
    } Msg;
    
    const uint8_t ConstData[8] = \
    {							 \
        0x80, 0x00, 0x00, 0x00,  \
        0x00, 0x00, 0x01, 0xB8   \
    };
    
    memcpy(&MsgBuf[0], pSecurityCtx->pKeyBuf, 8);
    memcpy(&MsgBuf[8], pSecurityCtx->pPageBuf, 32);
    memcpy(&MsgBuf[40], pSecurityCtx->pUsidBuf, 8);
    memcpy(&MsgBuf[48], pRngBuf, 8);
    memcpy(&MsgBuf[56], ConstData, 8);

	/*!< ���CPU��ģʽ */
    EndianMode = CheckCpuEndian();
	
	/*!< CPUΪС��ģʽ */
	if (0x00 == EndianMode)
	{
		for (i = 0; i < 14; i++)
		{
			Msg.word[15-i]  = (uint32_t)(MsgBuf[i * 4]) 	<< 24;
			Msg.word[15-i] |= (uint32_t)(MsgBuf[i * 4 + 1]) << 16;
			Msg.word[15-i] |= (uint32_t)(MsgBuf[i * 4 + 2]) << 8;
			Msg.word[15-i] |= (uint32_t)(MsgBuf[i * 4 + 3]);	
		}

		/*!< Ϊ����оƬ�ڲ�����������һ�£���Ҫ����Ӧ˳����������� */
		Msg.word[1] = 0x00000080;
		Msg.word[0] = 0xB8010000;	
	}
	else
	{
		/*!< CPUΪ���ģʽ */
		for (i = 0; i < 14; i++)
		{
			Msg.word[15-i]  = (uint32_t)(MsgBuf[i * 4 + 3])	<< 24;
			Msg.word[15-i] |= (uint32_t)(MsgBuf[i * 4 + 2]) << 16;
			Msg.word[15-i] |= (uint32_t)(MsgBuf[i * 4 + 1]) << 8;
			Msg.word[15-i] |= (uint32_t)(MsgBuf[i * 4]);	
		}

		/*!< Ϊ����оƬ�ڲ�����������һ�£���Ҫ����Ӧ˳����������� */
		Msg.word[1] = 0x80000000;
		Msg.word[0] = 0x000001B8;
	}	
    
    sha256_init(&ctx);
    sha256_update(&ctx, Msg.byte, 64);
    sha256_final(&ctx, pMacBuf);
    
    for (i = 0; i < 16; i++)
    {
        tmp = pMacBuf[i];
        pMacBuf[i] = pMacBuf[31 - i];
        pMacBuf[31 - i] = tmp;
    }
}


/**
 ***************************************************************************
 * @brief  RJGT102��ȫ��ʼ��
 * @param  ��
 * @retval ����״ֵ̬��
 *         @arg 0x00  -��ʼ��ʧ��
 *         @arg 0x01  -��ʼ���ɹ�
 ***************************************************************************
 */
uint8_t RJGT102_SecurityInit(void)
{	
    ExecutionStatus_t ExecutionStatus;
    uint8_t IsOK;
	
    ExecutionStatus = RJGT102_Init();
    IsOK = (ES_NORMAL == ExecutionStatus) ? (0x01):(0x00);
	
    return IsOK;
}


/**
 ***************************************************************************
 * @brief  ��ȫ��֤
 * @param  [IN] pSecurityCtx  ָ��ȫ�����ı��� @ref SecurityContext_t
 * @retval ����״ֵ̬��
 *         @arg 0x00  -��֤ʧ��
 *         @arg 0x01  -��֤�ɹ� 
 ***************************************************************************
 */
uint8_t RJGT102_SecurityCertificate(const SecurityContext_t *pSecurityCtx)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t IsOK = 0x00;
    uint8_t Mac1Buf[32];
    uint8_t Mac2Buf[32];
    uint8_t RandomBuf[8];
	
    /*!< ��������� */
    RJGT102_GenerateRandom(RandomBuf);
	
    /*!< оƬ����MAC */
    ExecutionStatus = RJGT102_AuthenticationDevice(pSecurityCtx->SrcPage, RandomBuf, Mac1Buf);
    if (ES_NORMAL == ExecutionStatus)
    {
        /*!< ���ؼ���MAC */
        MCU_CalculateMac(pSecurityCtx, RandomBuf, Mac2Buf);
        if (!memcmp(Mac1Buf, Mac2Buf, 32))
        {
            IsOK = 0x01;
        }
    }
	
    return IsOK;
}


/**
 ***************************************************************************
 * @brief  ��ȫ��PAGE��
 * @param  [IN]  pSecurityCtx  ָ��ȫ�����ı��� @ref SecurityContext_t
 *         [OUT] pPageBuf      ָ�������PAGE���ݴ洢��
 *         [IN]  Length        ָ�������������ݳ���(С�ڵ���32�ֽ�)
 * @retval ����״ֵ̬��
 *         @arg 0x00  -��ȫ��ʧ��
 *         @arg 0x01  -��ȫ���ɹ� 
 ***************************************************************************
 */
uint8_t RJGT102_SecurityRead(const SecurityContext_t *pSecurityCtx, uint8_t *pPageBuf, uint8_t Length)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t IsOK;
    uint8_t RandomBuf[8];
    uint8_t MacBuf[32];
    
    RJGT102_GenerateRandom(RandomBuf);	                                 		
    MCU_CalculateMac(pSecurityCtx , RandomBuf, MacBuf);
    
    ExecutionStatus = RJGT102_ReadPage(pSecurityCtx->SrcPage, pSecurityCtx->DstPage, RandomBuf, MacBuf, pPageBuf, Length);
    IsOK = (ES_NORMAL == ExecutionStatus) ? (0x01):(0x00);
    
    return IsOK;
}


/**
 ***************************************************************************
 * @brief  ��ȫдPAGE��
 * @param  [IN] pSecurityCtx  ָ��ȫ�����ı��� @ref SecurityContext_t
 *         [IN] pPageBuf      ָ���д��32�ֽ�PAGE���ݻ�����
 * @retval ����״ֵ̬��
 *         @arg 0x00  -��ȫдʧ��
 *         @arg 0x01  -��ȫд�ɹ� 
 ***************************************************************************
 */
uint8_t RJGT102_SecurityWrite(const SecurityContext_t *pSecurityCtx, const uint8_t *pPageBuf)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t IsOK;
    uint8_t RandomBuf[8];
    uint8_t MacBuf[32];
    
    memcpy(RandomBuf, pPageBuf, 8);
    MCU_CalculateMac(pSecurityCtx, RandomBuf, MacBuf);
    
    ExecutionStatus = RJGT102_WritePage(pSecurityCtx->SrcPage, pSecurityCtx->DstPage, pPageBuf, MacBuf);
    IsOK = (ES_NORMAL == ExecutionStatus) ? (0x01):(0x00);
    
    return IsOK;
}


/**
 ***************************************************************************
 * @brief  ������оƬ��ͬ��������Կ
 * @param  [IN]  pSecurityCtx  ָ��ȫ�����ı��� @ref SecurityContext_t
 *         [OUT] pNewKeyBuf    ָ������Կ������
 * @retval ����״ֵ̬��
 *         @arg 0x00  -������Կʧ��
 *         @arg 0x01  -������Կ�ɹ� 
 ***************************************************************************
 */
uint8_t RJGT102_SecurityUpdateKey(const SecurityContext_t *pSecurityCtx, uint8_t *pNewKeyBuf)
{
    ExecutionStatus_t ExecutionStatus;
    uint8_t IsOK = 0x00;
    uint8_t Mac1Buf[32];
    uint8_t Mac2Buf[32];
    uint8_t RandomBuf[8];
	
    /*!< ��������� */
    RJGT102_GenerateRandom(RandomBuf);
	
    /*!< оƬ����MAC */
    ExecutionStatus = RJGT102_AuthenticationDevice(pSecurityCtx->SrcPage, RandomBuf, Mac1Buf);
    if (ES_NORMAL != ExecutionStatus)
    {
        return IsOK; 
    }
	
	/*!< ���ؼ���MAC */
	MCU_CalculateMac(pSecurityCtx, RandomBuf, Mac2Buf);
	if (memcmp(Mac1Buf, Mac2Buf, 32))
	{
		return IsOK;
	}
	memcpy(pNewKeyBuf, Mac2Buf, 8);
    
    ExecutionStatus = RJGT102_UpdateKey(pSecurityCtx->SrcPage, RandomBuf);
    IsOK = (ES_NORMAL == ExecutionStatus) ? (0x01):(0x00);
    
    return IsOK;
}

