/**
 * @file hw_store.c
 * @brief AVR implementation of persistent storage backend.
 */

#include <stdbool.h>
#include "sys_settings_types.h"
#include "hw_store.h"
#include "eeprom_drv.h"

static Settings saved_settings;

void hw_store_init(void)
{
    eeprom_drv_init();
}

bool hw_store_read(uint16_t addr, void *data, size_t len)
{
    return eeprom_drv_read(addr, data, len);
}

bool hw_store_write(uint16_t addr, const void *data, size_t len)
{
    return eeprom_drv_write(addr, data, len);
}

void hw_store_commit(void)
{
    eeprom_drv_commit();
}

bool hw_store_settings_init(Settings * dst)
{
    return eeprom_drv_read_settings(dst);
}

bool hw_store_read_settings(Settings * dst)
{
    return eeprom_drv_read_settings(dst);
}

void hw_store_write_settings(const Settings * src)
{
    eeprom_drv_write_settings(src);
}

const Settings * get_saved_settings(void)
{
    return &saved_settings;
}

Settings * get_saved_settings_mutable(void)
{
    return &saved_settings;
}

