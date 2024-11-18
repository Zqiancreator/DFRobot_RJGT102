/**
 **************************************************************************************
 *                Copyright(c) 2015 Wuhan Runjet, All rights reserved.
 **************************************************************************************
 * @file     rjgt102_security.h
 * @author   Wuhan Runjet software and test team
 * @date     04-Feb-2015
 * @version  V1.00
 * @brief    �汣оƬ��ȫ���ʹ�������
 **************************************************************************************
 *
 */
#ifndef __RJGT102_SECURITY_H__
#define	__RJGT102_SECURITY_H__

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include <stdint.h>



/**@brief ��ȫ�����Ľṹ
 */
typedef struct
{
    uint8_t SrcPage;
    uint8_t DstPage;
    uint8_t *pKeyBuf;
    uint8_t *pPageBuf;
    uint8_t *pUsidBuf;
} SecurityContext_t;



/**@brief ��������
 */ 
uint8_t RJGT102_SecurityInit(void);
uint8_t RJGT102_SecurityCertificate(const SecurityContext_t *pSecurityCtx);
uint8_t RJGT102_SecurityRead(const SecurityContext_t *pSecurityCtx, uint8_t *pPageBuf, uint8_t len);
uint8_t RJGT102_SecurityWrite(const SecurityContext_t *pSecurityCtx, const uint8_t *pPageBuf);
uint8_t RJGT102_SecurityUpdateKey(const SecurityContext_t *pSecurityCtx, uint8_t *pNewKeyBuf);
void MCU_CalculateMac(const SecurityContext_t *pSecurityCtx, const uint8_t *pRngBuf, uint8_t *pMacBuf);
	 
#ifdef __cplusplus
}
#endif

#endif /* end of __RJGT102_SECURITY_H__ */

