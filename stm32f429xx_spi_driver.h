/*
 * stm32f429xx_spi_driver.h
 *
 *  Created on: 29-May-2026
 *      Author: shivam
 */

#ifndef INC_STM32F429XX_SPI_DRIVER_H_
#define INC_STM32F429XX_SPI_DRIVER_H_

#include "stm32f429xx.h"

/* Configuration structure for SPIx peripheral */

typedef struct
{
	uint8_t SPI_DeviceMode;   /*!< Specifies SPI mode (Master/Slave) */
	uint8_t SPI_BusConfig;    /*!< Specifies SPI bus configuration */
	uint8_t SPI_SclkSpeed;    /*!< Specifies SPI serial clock speed */
	uint8_t SPI_DFF;          /*!< Specifies data frame format (8-bit/16-bit) */
	uint8_t SPI_CPOL;         /*!< Specifies clock polarity */
	uint8_t SPI_CPHA;         /*!< Specifies clock phase */
	uint8_t SPI_SSM;          /*!< Specifies software slave management */
} SPI_Config_t;


/* Handle structure for SPIx peripheral */

typedef struct
{
	SPI_RegDef_t *pSPIx;      /*!< Pointer to SPI peripheral base address */
	SPI_Config_t SPIConfig;   /*!< SPI configuration settings */

	uint8_t *pTxBuffer;       /*!< Pointer to application transmit buffer */
	uint8_t *pRxBuffer;       /*!< Pointer to application receive buffer */

	uint32_t TxLen;           /*!< Number of bytes remaining for transmission */
	uint32_t RxLen;           /*!< Number of bytes remaining for reception */

	uint8_t TxState;          /*!< Current transmission state */
	uint8_t RxState;          /*!< Current reception state */

} SPI_Handle_t;


/* SPI application states */

#define SPI_READY       0   /*!< SPI peripheral is ready */
#define SPI_BUSY_IN_RX  1   /*!< SPI peripheral is busy receiving data */
#define SPI_BUSY_IN_TX  2   /*!< SPI peripheral is busy transmitting data */

/* Possible SPI Application events */

#define SPI_EVENT_TX_CMPLT   1   /*!< Transmission completed event */
#define SPI_EVENT_RX_CMPLT   2   /*!< Reception completed event */
#define SPI_EVENT_OVR_ERR    3   /*!< Overrun error event */
#define SPI_EVENT_CRC_ERR    4   /*!< CRC error event */


/*
 * @SPI_DeviceMode
 */

#define SPI_DEVICE_MODE_MASTER   1   /*!< Configure SPI as Master */
#define SPI_DEVICE_MODE_SLAVE    0   /*!< Configure SPI as Slave */


/*
 * @SPI_BusConfig
 */

#define SPI_BUS_CONFIG_FD               1   /*!< Full duplex communication */
#define SPI_BUS_CONFIG_HD               2   /*!< Half duplex communication */
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY   3   /*!< Simplex receive-only communication */

/*
 * @SPI_SclkSpeed
 */

#define SPI_SCLK_SPEED_DIV2      0   /*!< SPI clock = PCLK/2 */
#define SPI_SCLK_SPEED_DIV4      1   /*!< SPI clock = PCLK/4 */
#define SPI_SCLK_SPEED_DIV8      2   /*!< SPI clock = PCLK/8 */
#define SPI_SCLK_SPEED_DIV16     3   /*!< SPI clock = PCLK/16 */
#define SPI_SCLK_SPEED_DIV32     4   /*!< SPI clock = PCLK/32 */
#define SPI_SCLK_SPEED_DIV64     5   /*!< SPI clock = PCLK/64 */
#define SPI_SCLK_SPEED_DIV128    6   /*!< SPI clock = PCLK/128 */
#define SPI_SCLK_SPEED_DIV256    7   /*!< SPI clock = PCLK/256 */

/*
 * @SPI_DFF
 */

#define SPI_DFF_8BITS    0   /*!< 8-bit data frame format */
#define SPI_DFF_16BITS   1   /*!< 16-bit data frame format */

/*
 * @CPOL
 */

#define SPI_CPOL_HIGH    1   /*!< Clock remains HIGH when idle */
#define SPI_CPOL_LOW     0   /*!< Clock remains LOW when idle */

/*
 * @CPHA
 */

#define SPI_CPHA_HIGH    1   /*!< Data sampled on second clock edge */
#define SPI_CPHA_LOW     0   /*!< Data sampled on first clock edge */

/*
 * @SPI_SSM
 */

#define SPI_SSM_EN       1   /*!< Software slave management enabled */
#define SPI_SSM_DI       0   /*!< Software slave management disabled */


/*
 * SPI related status flags definitions
 */

#define SPI_TXE_FLAG     (1 << SPI_SR_TXE)   /*!< Transmit buffer empty flag */
#define SPI_RXNE_FLAG    (1 << SPI_SR_RXNE)  /*!< Receive buffer not empty flag */
#define SPI_BUSY_FLAG    (1 << SPI_SR_BSY)   /*!< SPI busy status flag */



/******************************************************************************************
 *								APIs supported by this driver
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);  /*!< * Enables or disables the peripheral clock for SPIx.*/

/*
 * Init and De-init
 */

void SPI_Init(SPI_Handle_t *pSPIHandle);  /*!< Initializes SPI peripheral according to user configuration */
void SPI_DeInit(SPI_RegDef_t *pSPIx);     /*!< Resets SPI peripheral registers to default values */


/*
 * Data Send and Receive
 */

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len);        /*!< Sends data over SPI in blocking mode */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);    /*!< Receives data over SPI in blocking mode */

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len);   /*!< Sends data over SPI using interrupts */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);   /*!< Receives data over SPI using interrupts */

/*
 * IRQ Configuration and ISR handling
 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);   /*!< Enables or disables a given SPI interrupt in NVIC */

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);   /*!< Configures priority of a given SPI interrupt */

void SPI_IRQHandling(SPI_Handle_t *pHandle);                      /*!< Handles SPI interrupt events */

/*
 * Other Peripheral Control APIs
 */

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);  /*!< Enables or disables SPI peripheral */

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);          /*!< Configures SSI bit for software slave management */

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);         /*!< Configures SS output enable */

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);  /*!< Returns current status of specified SPI flag */

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);                       /*!< Clears SPI overrun flag */

void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle);              /*!< Terminates ongoing SPI transmission */

void SPI_CloseReception(SPI_Handle_t *pSPIHandle);                /*!< Terminates ongoing SPI reception */

/*
 * Application callback
 */

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv);   /*!< Callback function invoked on SPI events, User application can override this weak function */

#endif /* INC_STM32F429XX_SPI_DRIVER_H_ */
