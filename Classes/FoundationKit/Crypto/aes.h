/*********************************************************************************

 Copyright 2006-2009 MakingThings

 Licensed under the Apache License, 
 Version 2.0 (the "License"); you may not use this file except in compliance 
 with the License. You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0 
 
 Unless required by applicable law or agreed to in writing, software distributed
 under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied. See the License for
 the specific language governing permissions and limitations under the License.

*********************************************************************************/
#pragma once

#include "FoundationKit/GenericPlatformMacros.h"  // for NS_FK_BEGIN NS_FK_END
NS_FK_BEGIN

#ifdef __cplusplus
extern "C" {
#endif

namespace aes{

/**
 * Encrypt a block of data using AES.
 * 
 * @param[out] output A pointer to the buffer into which the encrypted data will be written.
 * @param[out] outlen The maximum number of bytes to write into the output buffer.
 * @param[in]  input A pointer to the data to be encrypted.
 * @param[in]  inlen The number of bytes of data to encrypt.
 * @param[in]  password The password, or key, used to encrypt the data.  Must be 16 bytes long.
 * @return The number of bytes encrypted, or -1 on failure.
 *
 * @code
 * #define BUFF_SIZE 256
 * unsigned char cipherbuf[BUFF_SIZE]; // the buffer that will be written into
 * unsigned char plaintext[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // the text to encrypt
 * unsigned char secret[] = "A SECRET PASSWORD"; // 16 bytes long
 * int written = aesEncrypt(cipherbuf, BUFF_SIZE, plaintext, 26, secret);
 * @endcode
 */
int aesEncrypt(unsigned char* output, int outlen, unsigned char* input, int inlen, unsigned char* password);

/**
 * Decrypt a block of AES encrypted data.
 * 
 * @param[out] output A pointer to the buffer into which the decrypted data will be written.
 * @param[out] outlen The maximum number of bytes of data to write into the output buffer.
 * @param[in]  input A pointer to the encrypted block to be decrytped.
 * @param[in]  inlen The number of bytes to be decrypted.
 * @param[in]  password The password, or key, used to encrypt the data.  Must be 16 bytes long.
 * @return The number of bytes written, or -1 on failure.
 * 
 * @code
 * //first encrypt some data, then decrypt it and make sure we get out what we put in
 * unsigned char cipherbuf[256];
 * unsigned char plainbuf[256];
 * unsigned char plaintext[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // our plaintext to encrypt
 * unsigned char secret[] = "A SECRET PASSWORD"; // 16 bytes long
 * 
 * int written = aesEncrypt(cipherbuf, 256, plaintext, 26, secret);
 * written = aesDecrypt(plainbuf, 256, cipherbuf, written, secret);
 * //we now have our original plaintext in plainbuf
 * @endcode
 */
int aesDecrypt(unsigned char* output, int outlen, unsigned char* input, int inlen, unsigned char* password);

} //namespace aes

#ifdef __cplusplus
}
#endif

NS_FK_END

