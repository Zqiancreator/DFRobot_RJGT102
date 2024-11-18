#ifndef KEY_H
#define KEY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* registerIIC(
  void(*Open)(void), 
  void (*ReadByte)(uint8_t *pByteData, uint8_t AddrToRead), 
  void (*WriteByte)(uint8_t ByteData, uint8_t AddrToWrite), 
  void (*Read)(uint8_t AddrToRead, uint8_t *pData, uint16_t Length),
  void (*Write)(uint8_t AddrToWrite, uint8_t *pData, uint16_t Length),
	void (*Close)(void)
);
void getPage(uint8_t nPage, uint8_t *PageData, const char *PID);
void getUsid(uint8_t *usid);
uint8_t verifyKey(const char *PID);
uint8_t initAll(const char *PID);
void getFuse(uint8_t *res);
#endif