/**
  ********************************************************************************
  *			    Copyright(c) 2015 Wuhan Runjet, All Rights Reserved.
  ********************************************************************************
  * @file    sha256.h
  * @author  Wuhan Runjet software and test team
  * @version V1.00
  * @date    04-Feb-2015
  * @brief   This file contains all the functions prototypes for SHA-256 algorithm.
  ********************************************************************************
  * @attention 
  *
  * 1> This code is presented "as is" without any guarantees.
  *
  * 2> To use this module, first call @ref sha256_init on a @ref sha256_ctx_t instance. 
  * Then call @ref sha256_update with the data to be hashed. This step can optionally be
  * done with multiple calls to @ref sha256_update, each with a section of the data (in 
  * the correct order). After all data has been passed to @ref sha256_update, call @ref 
  * sha256_final to finalize and extract the hash value.
  * 
  * This implementation uses little endian byte order, Algorithm specification can 
  * be found here:
  * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
  *
  * This code is adapted from code by Brad Conte, retrieved from
  * https://github.com/B-Con/crypto-algorithms.
  ******************************************************************************
  */
#ifndef __SHA256_H__
#define __SHA256_H__

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include <stdint.h>
#include <stddef.h>



/**
 *@biref SHA256上下文结构
 */	 
typedef struct 
{
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} sha256_ctx_t;


/**
 *@brief 函数声明
 */
void sha256_init(sha256_ctx_t *ctx);
void sha256_update(sha256_ctx_t *ctx, const uint8_t *data, const size_t len);
void sha256_final(sha256_ctx_t *ctx, uint8_t *hash);


#ifdef __cplusplus
}
#endif

#endif /* end of __SHA256_H__*/

