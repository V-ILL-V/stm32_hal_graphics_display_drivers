/*
 * 16 bit paralell LCD GPIO driver
 * 5 controll pins (CS, RS, WR, RD, RST) + 16 data pins + backlight pin (BL)
 */

#ifndef __LCD_IO_GPIO16_H
#define __LCD_IO_GPIO16_H

//=============================================================================
/* Data direction
   - 0: only draw mode
   - 1: bidirectional mode */
#define LCD_DATADIR           0

/* RGB565 to RGB888 and RGB888 to RGB565 convert byte order
   - 0: forward direction
   - 1: back direction
   note: If the red and blue colors are reversed and used 24bit mode, change this value */
#define LCD_RGB24_ORDER       0

/*=============================================================================
I/O group optimization so that GPIO operations are not performed bit by bit:
Note: If the pins are in order, they will automatically optimize.

The example belongs to the following pins:
  LCD_D0<-E0, LCD_D1<-E1, LCD_D2<-E2, LCD_D3<-E3, LCD_D4<-E4, LCD_D5<-E5, LCD_D6<-E6, LCD_D7<-E7
  LCD_D8<-E8, LCD_D9<-E9, LCD_D10<-E10, LCD_D11<-E11, LCD_D12<-E12, LCD_D13<-E13, LCD_D14<-E14, LCD_D15<-E15 */
#if 0
/* datapins setting to output (data direction: STM32 -> LCD) */
#define LCD_DIRWRITE { /* E0..E15 <- 0b01 */ \
  GPIOE->MODER = 0x55555555; }
/* datapins setting to input (data direction: STM32 <- LCD) */
#define LCD_DIRREAD { /* E0..B15 <- 0b00 */ \
  GPIOE->MODER = 0x00000000; }
/* datapins write, STM32 -> LCD (write I/O pins from dt data) */
#define LCD_WRITE(dt) { /* E0..15 <- dt0..15 */ \
  GPIOE->ODR = dt; }
/* datapins read, STM32 <- LCD (read from I/O pins and store to dt data) */
#define LCD_READ(dt) { /* dt0..15 <- E0..15 */ \
  dt = GPIOE->IDR; }
#endif

/* The example belongs to the following pins:
  LCD_D0<-D14,  LCD_D1<-D15,  LCD_D2<-D0,   LCD_D3<-D1
  LCD_D4<-E7,   LCD_D5<-E8,   LCD_D6<-E9,   LCD_D7<-E10
  LCD_D8<-E11,  LCD_D9<-E12,  LCD_D10<-E13, LCD_D11<-E14
  LCD_D12<-E15, LCD_D13<-D8,  LCD_D14<-D9,  LCD_D15<-D10 */
#if 0
// datapins setting to output (data direction: STM32 -> LCD)
#define LCD_DIRWRITE { /* D0..D1, D8..D10, D14..D15, E7..E15 <- 0b01 */ \
  GPIOD->MODER = (GPIOD->MODER & ~0b11110000001111110000000000001111) | 0b01010000000101010000000000000101; \
  GPIOE->MODER = (GPIOE->MODER & ~0b11111111111111111100000000000000) | 0b01010101010101010100000000000000; }
// datapins setting to input (data direction: STM32 <- LCD)
#define LCD_DIRREAD { /* D0..D1, D8..D10, D14..D15, E7..E15 <- 0b00 */ \
  GPIOD->MODER = (GPIOD->MODER & ~0b11110000001111110000000000001111); \
  GPIOE->MODER = (GPIOE->MODER & ~0b11111111111111111100000000000000); }
// datapins write, STM32 -> LCD (write I/O pins from dt data)
#define LCD_WRITE(dt) { /* D14..15 <- dt0..1, D0..1 <- dt2..3, D8..10 <- dt13..15, E7..15 <- dt4..12 */ \
  GPIOD->ODR = (GPIOD->ODR & ~0b1100011100000011) | (((dt & 0b11) << 14) | ((dt & 0b1100) >> 2) | ((dt & 0b1110000000000000) >> 5)); \
  GPIOE->ODR = (GPIOE->ODR & ~0b1111111110000000) | ((dt & 0b0001111111110000) << 3); }
// datapins read, STM32 <- LCD (read from I/O pins and store to dt data)
#define LCD_READ(dt) { /* dt0..1 <- D14..15, dt2..3 <- D0..1, dt13..15 <- D8..10, dt4..12 <- E7..15 */ \
  dt = ((GPIOD->IDR & 0b1100000000000000) >> 14) | ((GPIOD->IDR & 0b0000000000000011) << 2) | \
       ((GPIOD->IDR & 0b0000011100000000) << 5)  | ((GPIOE->IDR & 0b1111111110000000) >> 3); }
/* Note: the keil compiler cannot use binary numbers, convert it to hexadecimal */
#endif

#endif // __LCD_IO_GPIO16_H