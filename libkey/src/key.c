#include "key.h"
#include "storeKey.h"
#include "sha256.h"
#include "rjgt102.h"
#include "rjgt102_i2c.h"
#include "rjgt102_security.h"
uint8_t pageDataBak[32] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                             0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                             0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                             0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

void* registerIIC(
  void (*Open)(void), 
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

SecurityContext_t *initSecurity(const char *PID)
{
  SecurityContext_t *pSecurityCtx = (SecurityContext_t *)malloc(sizeof(SecurityContext_t));
  if(!pSecurityCtx)
    return NULL;
  pSecurityCtx->SrcPage = 0x00;
  pSecurityCtx->DstPage = 0x00;
  for(uint8_t i=0;i<DEV_NUMS;i++)
  {
    if(strcmp((const char*)devices[i].PID, PID) == 0){
      pSecurityCtx->pKeyBuf = devices[i].Key;
      pSecurityCtx->pPageBuf = pageDataBak;
      pSecurityCtx->pUsidBuf = devices[i].Usid;
      return pSecurityCtx;
    }
  }
  free(pSecurityCtx);  
  return NULL;
}

void getPage(uint8_t nPage, uint8_t *PageData, const char *PID)
{
  uint8_t page_data[32];
  SecurityContext_t *pSecurityCtx = initSecurity(PID);
  if(!pSecurityCtx){
    memset(PageData, 0x11, 32);
    return;
  }
  pSecurityCtx->DstPage = nPage;
  uint8_t status = RJGT102_SecurityRead(pSecurityCtx, page_data, 32);
  if (status == 0x01)
    memcpy(PageData, page_data, 32);
  else
    memset(PageData, 0x00, 32);
  free(pSecurityCtx);
}
void getUsid(uint8_t *usid)
{
  RJGT102_ReadUsid(usid);
}
// 0b if any init error, bit is 1
uint8_t initAll(const char *PID)
{
  uint8_t status = 0x00;
  uint8_t curDev = DEV_NUMS;
  for(uint8_t i=0;i<DEV_NUMS;i++)
  {
    if(strcmp((const char*)devices[i].PID, PID) == 0){
      curDev = i;
      break;
    }
  }
  if(curDev >= DEV_NUMS){
    return curDev;
  }
  if (RJGT102_InitUsid(devices[curDev].Usid) != 0x01)
    status |= 1;
  if (RJGT102_InitKey(devices[curDev].Key) != 0x01)
    status |= (1 << 1);
  
  SecurityContext_t *pSecurityCtx = initSecurity(PID);
  pSecurityCtx->pPageBuf = pageDataBak;
  // pg0
  pSecurityCtx->DstPage = 0x00;
  if (RJGT102_SecurityWrite(pSecurityCtx, pageDataBak) != 0x01)
    status |= (1 << 2);
  // pg1
  pSecurityCtx->DstPage = 0x20;
  if (RJGT102_SecurityWrite(pSecurityCtx, devices[curDev].pageData1) != 0x01)
    status |= (1 << 3);
  // pg2
  pSecurityCtx->DstPage = 0x40;
  if (RJGT102_SecurityWrite(pSecurityCtx, devices[curDev].pageData2) != 0x01)
    status |= (1 << 4);
  // pg3
  pSecurityCtx->DstPage = 0x60;
  if (RJGT102_SecurityWrite(pSecurityCtx, devices[curDev].pageData3) != 0x01)
    status |= (1 << 5);

  free(pSecurityCtx);
#if 0 /* 熔断 */
  for(uint8_t i=0;i<7;i++){
    RJGT102_WriteCtrlReg(0xA8+i, 0x5A);
  }
#endif
  return status;
}
/// @brief board ececute to verify when on power
/// @return 0x01 success 0x00 error
uint8_t verifyKey(const char *PID)
{
  SecurityContext_t *pSecurityCtx = initSecurity(PID);
  uint8_t res =  RJGT102_SecurityCertificate(pSecurityCtx);
  free(pSecurityCtx);
  return res;
}

void getFuse(uint8_t *res)
{
  for (uint8_t i = 0; i < 8; i++)
    RJGT102_ReadCtrlReg(0xA8 + i, &res[i]);
}

