/**
 * @file gpio_defs.h
 * @brief
 */

#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

#define PA7_ENABLE  PA7  // PA7: Enable 0=displays 1=push-buttons

#define PB0_nEXEC   PB0
#define PB0_nWR     PB0
#define PB1_TBA     PB1
#define PB2_TBB     PB2
#define PB3_RCLK    PB3
#define PB4_nSTART  PB4
#define PB4_nFL     PB4
#define PB5_MOSI    PB5
#define PB6_MISO    PB6
#define PB7_SCK     PB7

#define PD0_RXD     PD0
#define PD1_TXD     PD1
#define PD2_INT0    PD2
#define PD4_ROTB    PD4
#define PD5_ROTA    PD5
#define PD6_SOCKET  PD6
#define PD7_nGATE   PD7  // PD7: Gate 0=open 1=closed

#define PD7_GATE_CLOSED ((PIND & (1 << PD7_nGATE)) >> PD7_nGATE)

#endif

