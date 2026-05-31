/*
 * stm32f429xx.h
 *
 *  Created on: 19-Mar-2026
 *      Author: shivam
 */

#ifndef INC_STM32F429XX_H_
#define INC_STM32F429XX_H_

#include<stddef.h>
#include<stdint.h>

#define __vo volatile                         /*!< Volatile qualifier */
#define __weak __attribute__((weak))          /*!< Weak linkage attribute */



/****START:Processor Specific Details ****/
/* ARM Cortex Mx Processor NVIC ISERx register Addresses */

#define NVIC_ISER0 ((__vo uint32_t*)0xE000E100)   /*!< NVIC Interrupt Set-Enable Register 0 */
#define NVIC_ISER1 ((__vo uint32_t*)0xE000E104)   /*!< NVIC Interrupt Set-Enable Register 1 */
#define NVIC_ISER2 ((__vo uint32_t*)0xE000E108)   /*!< NVIC Interrupt Set-Enable Register 2 */


/* ARM Cortex Mx Processor NVIC ICERx register Addresses */

#define NVIC_ICER0 ((__vo uint32_t*)0xE000E180)   /*!< NVIC Interrupt Clear-Enable Register 0 */
#define NVIC_ICER1 ((__vo uint32_t*)0xE000E184)   /*!< NVIC Interrupt Clear-Enable Register 1 */
#define NVIC_ICER2 ((__vo uint32_t*)0xE000E188)   /*!< NVIC Interrupt Clear-Enable Register 2 */


/* ARM Cortex Mx Processor Priority Register Address Calculation */

#define NVIC_PR_BASE_ADDR ((__vo uint32_t*)0xE000E400)   /*!< Base address of NVIC priority registers */


/* ARM Cortex Mx Processor number of priority bits implemented in Priority Register */

#define NO_PR_BITS_IMPLEMENTED 4   /*!< Number of implemented interrupt priority bits */


/* base addresses of Flash and SRAM memories */

#define FLASH_BASEADDR      0x08000000U       /*!< Start address of internal Flash memory */
#define SRAM1_BASEADDR      0x20000000U       /*!< Start address of SRAM1 memory */
#define SRAM2_BASEADDR      0x2001C000U       /*!< Start address of SRAM2 memory */
#define ROM_BASEADDR        0x1FFF0000U       /*!< System memory (Bootloader ROM) base address */
#define SRAM                SRAM1_BASEADDR    /*!< Alias for SRAM1 base address */


/* AHBx and APBx Bus Peripheral base addresses */

#define PERIPH_BASEADDR         0x40000000U       /*!< Base address of peripheral memory space */
#define APB1PERIPH_BASEADDR     PERIPH_BASEADDR   /*!< Base address of APB1 peripherals */
#define APB2PERIPH_BASEADDR     0x40010000U       /*!< Base address of APB2 peripherals */
#define AHB1PERIPH_BASEADDR     0x40020000U       /*!< Base address of AHB1 peripherals */
#define AHB2PERIPH_BASEADDR     0x50000000U       /*!< Base address of AHB2 peripherals */


/* Base addresses of peripherals which are hanging on APB1 bus */

#define SPI2_BASEADDR   (APB1PERIPH_BASEADDR + 0x3800)   /*!< SPI2 peripheral base address */
#define SPI3_BASEADDR   (APB1PERIPH_BASEADDR + 0x3C00)   /*!< SPI3 peripheral base address */

/* Base addresses of peripherals which are hanging on APB2 bus */

#define SPI1_BASEADDR   (APB2PERIPH_BASEADDR + 0x3000) /*!< SPI1 peripheral base address */

/* peripheral register definition structure for SPI */

typedef struct
{
    __vo uint32_t CR1;      /*!< SPI Control Register 1,                 Offset: 0x00 */
    __vo uint32_t CR2;      /*!< SPI Control Register 2,                 Offset: 0x04 */
    __vo uint32_t SR;       /*!< SPI Status Register,                    Offset: 0x08 */
    __vo uint32_t DR;       /*!< SPI Data Register,                      Offset: 0x0C */
    __vo uint32_t CRCPR;    /*!< SPI CRC Polynomial Register,            Offset: 0x10 */
    __vo uint32_t RXCRCR;   /*!< SPI RX CRC Register,                    Offset: 0x14 */
    __vo uint32_t TXCRCR;   /*!< SPI TX CRC Register,                    Offset: 0x18 */
    __vo uint32_t I2SCFGR;  /*!< SPI/I2S Configuration Register,         Offset: 0x1C */
    __vo uint32_t I2SPR;    /*!< SPI/I2S Prescaler Register,             Offset: 0x20 */

} SPI_RegDef_t;


/* peripheral definitions ( Peripheral base addresses typecasted to xxx_RegDef_t) */

#define SPI1    ((SPI_RegDef_t*)SPI1_BASEADDR)       /*!< SPI1 peripheral definition */
#define SPI2    ((SPI_RegDef_t*)SPI2_BASEADDR)       /*!< SPI2 peripheral definition */
#define SPI3    ((SPI_RegDef_t*)SPI3_BASEADDR)       /*!< SPI3 peripheral definition */

/* Clock Enable Macros for SPIx peripheralsbu */

#define SPI1_PCLK_EN()   (RCC->APB2ENR |= (1 << 12))    /*!< Enable SPI1 peripheral clock */
#define SPI2_PCLK_EN()   (RCC->APB1ENR |= (1 << 14))    /*!< Enable SPI2 peripheral clock */
#define SPI3_PCLK_EN()   (RCC->APB1ENR |= (1 << 15))    /*!< Enable SPI3 peripheral clock */
#define SPI4_PCLK_EN()   (RCC->APB2ENR |= (1 << 13))    /*!< Enable SPI4 peripheral clock */

/* Clock Disable Macros for SPIx peripherals */

#define IRQ_NO_SPI1   35     /*!< SPI1 interrupt number */
#define IRQ_NO_SPI2   36     /*!< SPI2 interrupt number */
#define IRQ_NO_SPI3   51     /*!< SPI3 interrupt number */
#define IRQ_NO_SPI4   84     /*!< SPI4 interrupt number */

/* some generic macros */

#define ENABLE      1        /*!< Enable functionality */
#define DISABLE     0        /*!< Disable functionality */

#define SET         ENABLE   /*!< Logic set state */
#define RESET       DISABLE  /*!< Logic reset state */

#define FLAG_SET    SET      /*!< Flag is set */
#define FLAG_RESET  RESET    /*!< Flag is reset */


/******************************************************************************************
 *Bit position definitions of SPI peripheral
 ******************************************************************************************/
/* Bit position definitions SPI_CR1 */

#define SPI_CR1_CPHA       0        /*!< Clock phase selection */
#define SPI_CR1_CPOL       1        /*!< Clock polarity selection */
#define SPI_CR1_MSTR       2        /*!< Master mode selection */
#define SPI_CR1_BR         3        /*!< Baud rate control */
#define SPI_CR1_SPE        6        /*!< SPI peripheral enable */
#define SPI_CR1_LSBFIRST   7        /*!< Frame format selection */
#define SPI_CR1_SSI        8        /*!< Internal slave select */
#define SPI_CR1_SSM        9        /*!< Software slave management */
#define SPI_CR1_RXONLY     10       /*!< Receive-only mode */
#define SPI_CR1_DFF        11       /*!< Data frame format */
#define SPI_CR1_CRCNEXT    12       /*!< CRC transfer next */
#define SPI_CR1_CRCEN      13       /*!< Hardware CRC enable */
#define SPI_CR1_BIDIOE     14       /*!< Output enable in bidirectional mode */
#define SPI_CR1_BIDIMODE   15       /*!< Bidirectional data mode enable */


/* Bit position definitions SPI_CR2 */

#define SPI_CR2_RXDMAEN   0         /*!< RX buffer DMA enable */
#define SPI_CR2_TXDMAEN   1         /*!< TX buffer DMA enable */
#define SPI_CR2_SSOE      2         /*!< SS output enable */
#define SPI_CR2_FRF       4         /*!< Frame format selection */
#define SPI_CR2_ERRIE     5         /*!< Error interrupt enable */
#define SPI_CR2_RXNEIE    6         /*!< RX buffer not empty interrupt enable */
#define SPI_CR2_TXEIE     7         /*!< TX buffer empty interrupt enable */


/* Bit position definitions SPI_SR */

#define SPI_SR_RXNE     0           /*!< Receive buffer not empty flag */
#define SPI_SR_TXE      1           /*!< Transmit buffer empty flag */
#define SPI_SR_CHSIDE   2           /*!< Channel side flag */
#define SPI_SR_UDR      3           /*!< Underrun flag */
#define SPI_SR_CRCERR   4           /*!< CRC error flag */
#define SPI_SR_MODF     5           /*!< Mode fault flag */
#define SPI_SR_OVR      6           /*!< Overrun flag */
#define SPI_SR_BSY      7           /*!< SPI busy flag */
#define SPI_SR_FRE      8           /*!< Frame format error flag */

#include "stm32f429xx_spi_driver.h"

#endif /* INC_STM32F429XX_H_ */
