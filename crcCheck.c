好的文章一篇就够了，博客：
https://blog.csdn.net/liyuanbhu/article/details/7882789#commentsedit
-----
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

static uint8_t OTP_CRC8_TAB[256] = { 0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F,
        0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48,
        0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
        0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD, 0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82,
        0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC,
        0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE, 0xAB,
        0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A, 0x27, 0x20, 0x29, 0x2E,
        0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59,
        0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E,
        0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4, 0xF9,
        0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
        0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43,
        0x44, 0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A,
        0x33, 0x34, 0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A,
        0x6D, 0x64, 0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F,
        0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98,
        0x9F, 0x8A, 0x8D, 0x84, 0x83, 0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1,
        0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3 };
/**
 * Calculating CRC-8 in 'C'
 * @para ucPtr, start of data
 * @para ucLen, length of data
 * @para crc, incoming CRC
 * 使用查表法计算CRC-8 
 */
uint8_t crc8_caculate(uint8_t *ucPtr, uint8_t ucLen) {

    uint8_t ucIndex;
    uint8_t ucCRC8 = 0;
    while (ucLen--) {
        ucIndex = ucCRC8 ^ (*ucPtr++);
        ucCRC8 = OTP_CRC8_TAB[ucIndex];
    }

    return (~ucCRC8);
}

#define CRC_16_POLYNOMIALS   0x8005
/**
 * Calculating CRC-16 in 'C'
 * @para pchMsg, start of data
 * @para wDataLen, length of data
 * @para crc, incoming CRC
 * CRC16,校验和位宽W=16, 生成多项式x16+x15+x2+1
 * 除数（多项式）0x8005,余数初始值0x0000,结果异或值0x0000
 */
uint16_t crc16_caculate(uint8_t *pchMsg, uint16_t wDataLen) {
    uint8_t i;
    uint8_t chChar;
    uint16_t wCRC = 0xFFFF; //g_CRC_value;

    while (wDataLen--) {                     /* Step through bytes in memory */
        chChar = *pchMsg++;
        wCRC ^= (((uint16_t) chChar) << 8);  /* Fetch byte from memory, XOR into CRC top byte*/

        for (i = 0; i < 8; i++) {            /* Prepare to rotate 8 bits */
            if (wCRC & 0x8000) {             /* b15 is set... */
                wCRC = (wCRC << 1) ^ CRC_16_POLYNOMIALS;   /* rotate and XOR with polynomic */
            } else {                         /* just rotate */
                wCRC <<= 1;                  /* Loop for 8 bits */
            }
        }
        
        wCRC &= 0xFFFF;                      /* Ensure CRC remains 16-bit value */
    }                                        /* Loop until wDataLen=0 */

    return (wCRC);                           /* Return updated CRC */
}




#define CRC_32_POLYNOMIALS  0x04C11DB7

uint32_t DTable_CRC32[256]; //CRC32 Direct table

void crc32_table_init(void) {
    uint32_t i32, j32;
    uint32_t nData32;
    uint32_t nAccum32;

    for (i32 = 0; i32 < 256; i32++) {
        nData32 = (uint32_t) (i32 << 24);
        nAccum32 = 0;
        for (j32 = 0; j32 < 8; j32++) {
            if ((nData32 ^ nAccum32) & 0x80000000) {
                nAccum32 = (nAccum32 << 1) ^ CRC_32_POLYNOMIALS;
            } else {
                nAccum32 <<= 1;
            }
            nData32 <<= 1;
        }
        DTable_CRC32[i32] = nAccum32;
    }

}
/**
 * Calculating CRC-32 in 'C'
 * @para pchMsg, start of data
 * @para wDataLen, length of data
 * @para crc, incoming CRC
 * 前提:按字节查表的快速算法,因此在调用crc32_caculate前必须先调用crc32_table_init()
 * CRC32,校验和位宽W=32, 生成多项式x32+x26+x23+x22+x16+
								   x12+x11+x10+x8+x7+x5+
								   x4+x2+x1+1
 * 除数（多项式）0x04C11DB7,余数初始值0xFFFFFFFF,结果异或值0xFFFFFFFF
 */
uint32_t crc32_caculate(uint8_t *pchMsg, uint16_t wDataLen) {
    uint8_t chChar = 0x00;
    uint32_t dwCRC = 0xFFFFFFFF;

    while (wDataLen--) {
        chChar = *pchMsg++;
        dwCRC = DTable_CRC32[((dwCRC >> 24) ^ chChar) & 0xff] ^ (dwCRC << 8);
    }

    return dwCRC;
}




int main (int argc , char* argv[])
{
    unsigned char data1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};  
    
	  //crc8
    printf("crc8  %08x\n", crc8_caculate(data1,9)); 
	  //crc16 
    printf("crc16 %08x\n", crc16_caculate(data1,9));  
    
    //crc32
    crc32_table_init();
    printf("crc32 %08x\n", crc32_caculate(data1,9));  
    printf("hello world\n");
}
