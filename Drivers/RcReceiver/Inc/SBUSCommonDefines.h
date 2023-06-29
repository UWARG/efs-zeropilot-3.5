#ifndef SBUS_COMMON_DEFINES_H
#define SBUS_COMMON_DEFINES_H

#define BAUD_RATE           100000

#define SBUS_FRAME_SIZE		25
#define SBUS_FLAGS_BYTE		23
#define SBUS_FAILSAFE_BIT	3
#define SBUS_FRAMELOST_BIT	2
#define PAYLOAD_LEN_        23
#define HEADER_LEN_         1
#define FOOTER_LEN_         1

/* define range mapping here, -+100% -> 1000..2000 */
#define SBUS_RANGE_MIN      192
#define SBUS_RANGE_MAX      1792
#define SBUS_RANGE_RANGE (SBUS_RANGE_MAX - SBUS_RANGE_MIN)

#define SBUS_TARGET_MIN     1000
#define SBUS_TARGET_MAX     2000
#define SBUS_TARGET_RANGE (SBUS_TARGET_MAX - SBUS_TARGET_MIN)

#define HEADER_             0x0F
#define FOOTER_             0x00
#define FOOTER2_            0x04
#define CH17_MASK_          0x01
#define CH18_MASK_          0x02
#define LOST_FRAME_MASK_    0x04
#define FAILSAFE_MASK_      0x08

// this is 875
#define SBUS_SCALE_OFFSET (SBUS_TARGET_MIN - ((SBUS_TARGET_RANGE * SBUS_RANGE_MIN / SBUS_RANGE_RANGE)))

#endif