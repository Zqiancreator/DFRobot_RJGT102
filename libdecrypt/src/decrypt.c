#include "decrypt.h"
#include "storeKey.h"
#include "sha256.h"
#include "rjgt102.h"
#include "rjgt102_i2c.h"
#include "rjgt102_security.h"

void* registerIIC(
  void(*Open)(void), 
  void (*ReadByte)(uint8_t *pByteData, uint8_t AddrToRead), 
  void (*WriteByte)(uint8_t ByteData, uint8_t AddrToWrite), 
  void (*Read)(uint8_t AddrToRead, uint8_t *pData, uint16_t Length),
  void (*Write)(uint8_t AddrToWrite, uint8_t *pData, uint16_t Length),
	void (*Close)(void)
)
{
  static I2C_Driver_t myDriver;
  myDriver.Open = Open;
  myDriver.Close = Close;
  myDriver.Read = Read;
  myDriver.Write = Write;
  myDriver.ReadByte = ReadByte;
  myDriver.WriteByte = WriteByte;
  RJGT102_I2cDriverRegister(&myDriver);
  return &myDriver;
}
SecurityContext_t *initSecurity()
{
  SecurityContext_t *pSecurityCtx = (SecurityContext_t *)malloc(sizeof(SecurityContext_t));
  pSecurityCtx->SrcPage = 0x00;
  pSecurityCtx->DstPage = 0x00;
  pSecurityCtx->pKeyBuf = Key;
  pSecurityCtx->pPageBuf = pageData0;
  pSecurityCtx->pUsidBuf = Usid;
  return pSecurityCtx;
}

void getPage(uint8_t nPage, uint8_t *PageDataRes)
{
  if (nPage == 0x00)
  {
    memcpy(PageDataRes, pageData0, 32);
    return;
  }
  uint8_t pageData[32];
  SecurityContext_t *pSecurityCtx = initSecurity();
  if(!pSecurityCtx){
    memset(PageDataRes, 0x00, 32);
    return;
  }
  pSecurityCtx->DstPage = nPage;
  uint8_t status = RJGT102_SecurityRead(pSecurityCtx, pageData, 32);
  if (status == 0x01)
    memcpy(PageDataRes, pageData, 32);
  else
    memset(PageDataRes, 0x00, 32);
  free(pSecurityCtx);
}
void getUsid(uint8_t *usid)
{
  RJGT102_ReadUsid(usid);
}

/// @brief board ececute to verify
/// @return 0x01 success 0x00 error
uint8_t verifyKey()
{
  SecurityContext_t *pSecurityCtx = initSecurity();
  uint8_t res = RJGT102_SecurityCertificate(pSecurityCtx);
  free(pSecurityCtx);
  return res;
}

void getFuse(uint8_t *res)
{
  for (uint8_t i = 0; i < 8; i++)
    RJGT102_ReadCtrlReg(0xA8 + i, &res[i]);
}

