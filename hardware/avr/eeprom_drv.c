/**
 * @file eeprom_drv.c
 * @brief Handles EEPROM storage and integrity of system settings.
 */

#define SETTINGS_MAGIC  0xBEEF

#include <stdbool.h>
#include <avr/eeprom.h>
#include "sys_settings_types.h"
#include "utils_crc.h"
#include "eeprom_drv.h"

typedef struct
{
    uint16_t magic;
    uint16_t crc;
    Settings data;
} EEPROMSettings;

static EEPROMSettings EEMEM eeprom_settings_block;

void eeprom_drv_init(void)
{
    /* no-op */
}

bool eeprom_drv_read(uint16_t addr, void *data, size_t len)
{
    if (!data)
    {
        return false;
    }

    uint8_t *dst = (uint8_t *)data;
    for (size_t i = 0; i < len; ++i)
    {
        dst[i] = eeprom_read_byte((const uint8_t *)(addr + i));
    }

    return true;
}

bool eeprom_drv_write(uint16_t addr, const void *data, size_t len)
{
    if (!data)
    {
        return false;
    }

    const uint8_t *src = (const uint8_t *)data;
    for (size_t i = 0; i < len; ++i)
    {
        eeprom_update_byte((uint8_t *)(addr + i), src[i]);
    }

    return true;
}

void eeprom_drv_commit(void)
{
    /* no-op */
}

/* ========================================================================== */

static uint16_t calc_settings_crc(const Settings * s)
{
    return crc16((const uint8_t *)s, sizeof(Settings));
}

bool eeprom_drv_read_settings(Settings * dst)
{
    EEPROMSettings block;
    eeprom_read_block(&block, &eeprom_settings_block, sizeof(block));

    if (block.magic != SETTINGS_MAGIC)
    {
        return false;
    }

    uint16_t expected_crc = calc_settings_crc(&block.data);
    if (block.crc != expected_crc)
    {
        return false;
    }

    *dst = block.data;
    return true;
}

void eeprom_drv_write_settings(const Settings * src)
{
    EEPROMSettings block;
    block.magic = SETTINGS_MAGIC;
    block.data  = *src;
    block.crc   = calc_settings_crc(src);

    eeprom_update_block(&block, &eeprom_settings_block, sizeof(block));
}

