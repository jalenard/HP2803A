/**
 * @file utils_crc.c
 * @brief
 */

#include <stdint.h>
#include "utils_crc.h"

/**
 * @note
 * An equivalent, less compact version of crc8():
 * @code
 * uint8_t crc8(const uint8_t *data, uint16_t len)
 * {
 *     uint8_t crc = 0x00;
 *     for (uint16_t i = 0; i < len; i++)
 *     {
 *         crc ^= data[i];
 *         for (uint8_t j = 0; j < 8; j++)
 *         {
 *             if (crc & 0x80)
 *             {
 *                 crc = (crc << 1) ^ 0x07;
 *             }
 *             else
 *             {
 *                 crc <<= 1;
 *             }
 *         }
 *     }
 *     return crc;
 * }
 * @endcode
 */
uint8_t crc8(const uint8_t * data, uint16_t len)
{
    uint8_t crc = 0x00;
    while (len--)
    {
        crc ^= *data++;
        for (uint8_t i = 0; i < 8; i++)
        {
            crc = (uint8_t)((crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1));
        }
    }
    return crc;
}

/**
 * @brief Compute a 16-bit CRC (CRC-16/CCITT-FALSE variant).
 *
 * Implements the standard CRC-16-CCITT algorithm using polynomial
 * 0x1021 (x^16 + x^12 + x^5 + 1), initial value 0xFFFF, and MSB-first
 * bit processing. Each input byte is XORed into the upper byte of the
 * 16-bit register and shifted bit-by-bit through the feedback taps.
 *
 * @param data Pointer to the input data buffer.
 * @param len  Number of bytes to process.
 * @return 16-bit CRC remainder.
 */
uint16_t crc16(const uint8_t * data, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    while (len--)
    {
        crc ^= (uint16_t)(*data++) << 8;
        for (uint8_t i = 0; i < 8; i++)
        {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
        }
    }
    return crc;
}

