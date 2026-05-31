/*
 * stm32f429xx_spi_driver.c
 *
 *  Created on: 29-May-2026
 *      Author: shivam
 */
#include "stm32f429xx_spi_driver.h"

static void  spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void  spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void  spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - Enables or disables the peripheral clock for the
 *                      selected SPI peripheral through RCC registers.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral (SPI1/SPI2/SPI3)
 * @param[in]         - EnorDi : ENABLE or DISABLE macro
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Peripheral clock must be enabled before accessing
 *                      any SPI registers.
 *
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}
}


/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             - Initializes the SPI peripheral according to the
 *                      configuration parameters provided in the handle.
 *
 * @param[in]         - pSPIHandle : Pointer to SPI handle structure
 * @param[in]         - None
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Configures Device Mode, Bus Configuration,
 *                      Baud Rate, Data Frame Format, CPOL, CPHA and SSM.
 *
 */

void SPI_Init(SPI_Handle_t *pSPIHandle)
{

	//peripheral clock enable

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//first lets configure the SPI_CR1 register

	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR ;

	//2. Configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//bidi mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);

	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//bidi mode should be set
		tempreg |= ( 1 << SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= ( 1 << SPI_CR1_RXONLY);
	}

	// 3. Configure the spi serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4.  Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5. configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6 . configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;

}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             - Resets the selected SPI peripheral registers to
 *                      their default reset values.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - None
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Uses RCC peripheral reset mechanism.
 *
 */

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
 //todo
}

/*********************************************************************
 * @fn      		  - SPI_GetFlagStatus
 *
 * @brief             - Checks whether a specific SPI status flag is set
 *                      or reset.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - FlagName : SPI status flag to be checked
 * @param[in]         - None
 *
 * @return            - FLAG_SET or FLAG_RESET
 *
 * @Note              - Used for polling-based communication.
 *
 */

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             - Sends data over SPI using polling method.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - pTxBuffer : Pointer to transmit buffer
 * @param[in]         - Len : Number of bytes to transmit
 *
 * @return            - None
 *
 * @Note              - Blocking API. Function returns only after all
 *                      data has been transmitted.
 *
 */

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG)  == FLAG_RESET );

		//2. check the DFF bit in CR1
		if( (pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
		{
			//16 bit DFF
			//1. load the data in to the DR
			pSPIx->DR =   *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			((uint16_t*)pTxBuffer)++;
		}else
		{
			//8 bit DFF
			pSPIx->DR =   *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             - Receives data from SPI using polling method.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - pRxBuffer : Pointer to receive buffer
 * @param[in]         - Len : Number of bytes to receive
 *
 * @return            - None
 *
 * @Note              - Blocking API. Waits until requested data is
 *                      completely received.
 *
 */

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
		{
			//1. wait until RXNE is set
			while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG)  == (uint8_t)FLAG_RESET );

			//2. check the DFF bit in CR1
			if( (pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
			{
				//16 bit DFF
				//1. load the data from DR to Rxbuffer address
				 *((uint16_t*)pRxBuffer) = pSPIx->DR ;
				Len--;
				Len--;
				((uint16_t*)pRxBuffer)++;
			}else
			{
				//8 bit DFF
				*(pRxBuffer) = pSPIx->DR ;
				Len--;
				pRxBuffer++;
			}
		}

}


/*********************************************************************
 * @fn      		  - SPI_PeripheralControl
 *
 * @brief             - Enables or disables the SPI peripheral by
 *                      controlling the SPE bit.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - EnOrDi : ENABLE or DISABLE
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - SPI must be enabled before communication starts.
 *
 */

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &=  ~(1 << SPI_CR1_SPE);
	}


}


/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             - Configures the Internal Slave Select (SSI) bit.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - EnOrDi : ENABLE or DISABLE
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Used when software slave management (SSM) is enabled.
 *
 */

void  SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &=  ~(1 << SPI_CR1_SSI);
	}


}


/*********************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             - Enables or disables automatic SS output
 *                      management in master mode.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - EnOrDi : ENABLE or DISABLE
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Controls SSOE bit in CR2 register.
 *
 */

void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |=  (1 << SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &=  ~(1 << SPI_CR2_SSOE);
	}


}



/*********************************************************************
 * @fn      		  - SPI_IRQInterruptConfig
 *
 * @brief             - Enables or disables SPI interrupt in NVIC.
 *
 * @param[in]         - IRQNumber : Interrupt number
 * @param[in]         - EnorDi : ENABLE or DISABLE
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Configures NVIC ISER or ICER registers.
 *
 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER2 |= ( 1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER2 |= ( 1 << (IRQNumber % 64) );
		}
	}

}


/*********************************************************************
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             - Configures interrupt priority for a given IRQ.
 *
 * @param[in]         - IRQNumber : Interrupt number
 * @param[in]         - IRQPriority : Priority value
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Updates NVIC priority registers (IPR).
 *
 */

void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}


/*********************************************************************
 * @fn      		  - SPI_SendDataIT
 *
 * @brief             - Sends data using interrupt-driven communication.
 *
 * @param[in]         - pSPIHandle : Pointer to SPI handle structure
 * @param[in]         - pTxBuffer : Pointer to transmit buffer
 * @param[in]         - Len : Number of bytes to transmit
 *
 * @return            - Current SPI transmission state
 *
 * @Note              - Enables TXE interrupt and returns immediately.
 *
 */

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		//1 . Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;
		//2.  Mark the SPI state as busy in transmission so that
		//    no other code can take over same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= ( 1 << SPI_CR2_TXEIE );

	}


	return state;
}


/*********************************************************************
 * @fn      		  - SPI_ReceiveDataIT
 *
 * @brief             - Receives data using interrupt-driven communication.
 *
 * @param[in]         - pSPIHandle : Pointer to SPI handle structure
 * @param[in]         - pRxBuffer : Pointer to receive buffer
 * @param[in]         - Len : Number of bytes to receive
 *
 * @return            - Current SPI reception state
 *
 * @Note              - Enables RXNE interrupt and returns immediately.
 *
 */

uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1 . Save the Rx buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		//2.  Mark the SPI state as busy in reception so that
		//    no other code can take over same SPI peripheral until reception is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3. Enable the RXNEIE control bit to get interrupt whenever RXNEIE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= ( 1 << SPI_CR2_RXNEIE );

	}


	return state;

}

/*********************************************************************
 * @fn      		  - SPI_IRQHandling
 *
 * @brief             - Handles SPI interrupt events such as TXE,
 *                      RXNE and OVR.
 *
 * @param[in]         - pHandle : Pointer to SPI handle structure
 * @param[in]         - None
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Should be called from SPI ISR routine.
 *
 */

void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

	uint8_t temp1 , temp2;
	//first lets check for TXE
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_TXEIE);

	if( temp1 && temp2)
	{
		//handle TXE
		spi_txe_interrupt_handle(pHandle);
	}

	// check for RXNE
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_RXNEIE);

	if( temp1 && temp2)
	{
		//handle RXNE
		spi_rxne_interrupt_handle(pHandle);
	}

	// check for ovr flag
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_ERRIE);

	if( temp1 && temp2)
	{
		//handle ovr error
		spi_ovr_err_interrupt_handle(pHandle);
	}


}


//some helper function implementations

static void  spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	// check the DFF bit in CR1
	if( (pSPIHandle->pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
	{
		//16 bit DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR =   *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}else
	{
		//8 bit DFF
		pSPIHandle->pSPIx->DR =   *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(! pSPIHandle->TxLen)
	{
		//TxLen is zero , so close the spi transmission and inform the application that
		//TX is over.

		//this prevents interrupts from setting up of TXE flag
		SPI_CloseTransmisson(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}

}


static void  spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//do rxing as per the dff
	if(pSPIHandle->pSPIx->CR1 & ( 1 << 11))
	{
		//16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer++;
		pSPIHandle->pRxBuffer++;

	}else
	{
		//8 bit
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(! pSPIHandle->RxLen)
	{
		//reception is complete
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}

}


static void  spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{

	uint8_t temp;
	//1. clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	//2. inform the application
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);

}

/*********************************************************************
 * @fn      		  - SPI_CloseTransmisson
 *
 * @brief             - Terminates ongoing SPI transmission and restores
 *                      transmit state variables.
 *
 * @param[in]         - pSPIHandle : Pointer to SPI handle structure
 * @param[in]         - None
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Disables TXE interrupt source.
 *
 */

void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;

}

/*********************************************************************
 * @fn      		  - SPI_CloseReception
 *
 * @brief             - Terminates ongoing SPI reception and restores
 *                      receive state variables.
 *
 * @param[in]         - pSPIHandle : Pointer to SPI handle structure
 * @param[in]         - None
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Disables RXNE interrupt source.
 *
 */

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;

}

/*********************************************************************
 * @fn      		  - SPI_ClearOVRFlag
 *
 * @brief             - Clears the SPI overrun (OVR) error flag.
 *
 * @param[in]         - pSPIx : Base address of SPI peripheral
 * @param[in]         - None
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - OVR flag is cleared by reading DR followed by SR.
 *
 */

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;

}

/*********************************************************************
 * @fn      		  - SPI_ApplicationEventCallback
 *
 * @brief             - Weak callback function used to notify the
 *                      application about SPI events.
 *
 * @param[in]         - pSPIHandle : Pointer to SPI handle structure
 * @param[in]         - AppEv : Application event identifier
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - User application can override this weak function
 *                      to handle TX complete, RX complete and error events.
 *
 */

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{

	//This is a weak implementation . the user application may override this function.
}







