#ifndef SBUS_DEFINES_H
#define SBUS_DEFINES_H

#include <stdint.h>

static constexpr uint32_t SBUS_BAUD_RATE = 100000;

static constexpr uint8_t SBUS_FRAME_SIZE	=	25;
static constexpr uint8_t SBUS_FLAGS_BYTE	=	23;
static constexpr uint8_t SBUS_FAILSAFE_BIT =	3;
static constexpr uint8_t SBUS_FRAMELOST_BIT	= 2;
static constexpr uint8_t PAYLOAD_LEN_   =     23;
static constexpr uint8_t HEADER_LEN_    =     1;
static constexpr uint8_t FOOTER_LEN_    =     1;

/* define range mapping here, -+100% -> 1000..2000 */
static constexpr uint8_t SBUS_RANGE_MIN   =   192;
static constexpr uint16_t SBUS_RANGE_MAX   =   1792;
static constexpr uint16_t SBUS_RANGE_RANGE (SBUS_RANGE_MAX - SBUS_RANGE_MIN);

static constexpr uint16_t SBUS_TARGET_MIN   =  1000;
static constexpr uint16_t SBUS_TARGET_MAX   =  2000;
static constexpr uint16_t SBUS_TARGET_RANGE = (SBUS_TARGET_MAX - SBUS_TARGET_MIN);

static constexpr uint8_t HEADER_      =       0x0F;
static constexpr uint8_t FOOTER_      =       0x00;
static constexpr uint8_t FOOTER2_     =       0x04;
static constexpr uint8_t CH17_MASK_   =       0x01;
static constexpr uint8_t CH18_MASK_   =       0x02;
static constexpr uint8_t LOST_FRAME_MASK_  =   0x04;
static constexpr uint8_t FAILSAFE_MASK_  =    0x08;

// this is 875
static constexpr uint16_t SBUS_SCALE_OFFSET = (SBUS_TARGET_MIN - ((SBUS_TARGET_RANGE * SBUS_RANGE_MIN / SBUS_RANGE_RANGE)));

#endif