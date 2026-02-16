/**
 * @file input_mcal.c
 * @brief
 */
#define DEBUG_INPUT_MCAL 0

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include "hw_input.h"
#include "gpio_defs.h"
#include "gpio_utils.h"
#include "input_mcal.h"
#include "error.h"

#if DEBUG_INPUT_MCAL
#include "sys_output.h"
#endif

volatile HWKey int0_key = HW_KEY_NULL;
volatile HWKey isr_key  = HW_KEY_NULL;
volatile bool request_int0_enable = false;
static uint8_t button_idx_shadow = 0x00;

void input_mcal_init(void)
{
    /* External interrupt 0 (PD2) for push-buttons */
    EICRA &= (uint8_t)~((1 << ISC00) | (1 << ISC01));     // Low-level trigger
    EIFR  |= (uint8_t)(1 << INTF0);                       // Clear pending flag
    EIMSK |= (uint8_t)(1 << INT0);                        // Enable INT0

    /* Pin-change interrupts for rotary encoder (PD4/PD5) */
    PCICR  |= (uint8_t)(1 << PCIE3);                      // Enable PCINT[31:24]
    PCMSK3 |= (uint8_t)((1 << PCINT28) | (1 << PCINT29)); // PD4/PD5 enable
    PCIFR  |= (uint8_t)(1 << PCIF3);                      // Clear pending flag
}

void input_mcal_enable(void)
{
    if ((PORTA & (1 << PA7_ENABLE)) != 0)
    {
        return;
    }

    uint8_t mask  = (1 << PA7_ENABLE);
    uint8_t value = (1 << PA7_ENABLE);
    gpio_write_masked(mask, value, GPIO_PORTA);
}

static inline void start_timer0_if_idle(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (!(TIMSK0 & (1 << OCIE0A)))
        {
            TCNT0 = 0;
            TIFR0 = (1 << OCF0A);
            TIMSK0 |= (1 << OCIE0A);
        }
    }
}

ISR(INT0_vect)
{
    /* Snapshot of PORTA */
    uint8_t pa = PINA;

    /* Disable INT0 */
    EIMSK &= (uint8_t)~(1 << INT0);
    EIFR  |= (uint8_t)(1 << INTF0);

    /* Decode PA7:5 = 0b1xx */
    uint8_t code = (pa >> 5) & 0x07;
    switch (code)
    {
    case 0x04:
        int0_key = HW_KEY_CANCEL;  // Lower push-button
        break;
    case 0x05:
        int0_key = HW_KEY_BACK;    // Middle push-button
        break;
    case 0x06:
        int0_key = HW_KEY_SELECT;  // Upper push-button
        break;
    case 0x07:
        int0_key = HW_KEY_UNUSED;  // Unused slot
        break;
    default:
        int0_key = HW_KEY_INVALID; // 0b0xx -> PA7=0 -> switches electrically disabled
        break;
    }

    /* Start timer 0 */
    start_timer0_if_idle();
}

ISR(PCINT3_vect)
{
    static uint8_t last_state = 0;  // 2-bit: ((A << 1) | B)

    uint8_t pd = PIND;
    uint8_t rota = (pd >> PD5_ROTA) & 0x01;
    uint8_t rotb = (pd >> PD4_ROTB) & 0x01;

    uint8_t new_state = (uint8_t)((rota << 1) | rotb);

    /* Rotation direction determined when lines A and B return to low */
    if (new_state == 0 && last_state != 0)
    {
        if (last_state == 2)        // 0b10 -> 0b00 => A leads
        {
            isr_key = HW_KEY_ROTA;
        }
        else if (last_state == 1)   // 0b01 -> 0b00 => B leads
        {
            isr_key = HW_KEY_ROTB;
        }
        else
        {
            isr_key = HW_KEY_INVALID;
        }
    }

    last_state = new_state;
}

void cycle_button_input_line(void)
{
    button_idx_shadow = (button_idx_shadow + 1) & 0x03;  // Increment modulo 4

    uint8_t mask  = (1 << PA6) | (1 << PA5);             // 0b01100000
    uint8_t value = (uint8_t)(button_idx_shadow << PA5); // 0b0xx00000
    gpio_write_masked(mask, value, GPIO_PORTA);
}

static HWKey input_mcal_get_key(void)
{
    uint8_t key;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        key = isr_key;
        isr_key = HW_KEY_NULL;
    }
#if DEBUG_INPUT_MCAL
    switch (key)
    {
    case HW_KEY_NULL:
        break;
    case HW_KEY_SELECT:
        sys_output_string("input_mcal: Select");
        break;
    case HW_KEY_BACK:
        sys_output_string("input_mcal: Back");
        break;
    case HW_KEY_CANCEL:
        sys_output_string("input_mcal: Cancel");
        break;
    case HW_KEY_ROTA:
        sys_output_string("input_mcal: Rot-A");
        break;
    case HW_KEY_ROTB:
        sys_output_string("input_mcal: Rot-B");
        break;
    default:
        sys_output_string("input_mcal: Other");
        break;
    }
#endif
    return key;
}

HWKey input_mcal_poll_key(void)
{
    cycle_button_input_line();
    HWKey key = input_mcal_get_key();
    return (HWKey)key;
}

