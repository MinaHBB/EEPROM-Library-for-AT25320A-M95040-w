/**
 * @file eeprom.c
 *
 * @brief This file contains the implementation of functions for controlling EEPROM.
 *
 * @details The functions allow configuring and accessing the EEPROM using SPI.
 * The functions handle enabling and disabling write operations, reading and writing data,
 * and waiting for write operations to complete.
 *
 * @date Feb 18, 2024
 * @author Mina Habibi
 */


#include "EEPROM.h"

// Define various commands for the EEPROM
#define EEPROM_SPI_ENABLE_WRITE			0x06	///< Write Enable
#define EEPROM_SPI_DISABLE_WRITE        0x04	///< Write Disable
#define EEPROM_SPI_READ_STATUS_REG      0x05	///< Read Status Register
#define EEPROM_SPI_WRITE_STATUS_REG     0x01	///< Write Status Register
#define EEPROM_SPI_READ_DATA			0x03	///< Read Data from Memory Array
#define EEPROM_SPI_WRITE_DATA	        0x02	///< Write Data to Memory Array

//---------------------------------------HAL Functions---------------------------------------
static uint8_t EEPROM_exchangeSpi(SPIDriver *spip, uint16_t frame)
{
	return (spi_lld_polled_exchange(spip, frame));
}

void EEPROM_startSpi(SPIDriver* spip, SPIConfig* pConfig)
{
	spi_lld_start(spip, pConfig);
}

void EEPROM_stopSpi(SPIDriver* spip)
{
	spi_lld_stop(spip);
}

static void EEPROM_setChipSelect(void)
{
	pal_lld_setpad(PORT6, PIN_DSPI_CS_EEPROM);
}

static void EEPROM_clearChipSelect(void)
{
	pal_lld_clearpad(PORT6, PIN_DSPI_CS_EEPROM);
}
//-------------------------------------------------------------------------------------------

void EEPROM_enableWrite(SPIDriver* spip)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_ENABLE_WRITE);
	EEPROM_setChipSelect();
}

void EEPROM_disableWrite(SPIDriver* spip)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_DISABLE_WRITE);
	EEPROM_setChipSelect();
}

uint8_t EEPROM_readStatusReg(SPIDriver* spip)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_READ_STATUS_REG);
	uint8_t retVal = EEPROM_exchangeSpi(spip, 0);
	EEPROM_setChipSelect();
	return retVal;
}

void EEPROM_writeStatusReg(SPIDriver* spip, uint8_t cmd)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_WRITE_STATUS_REG);
	EEPROM_exchangeSpi(spip, cmd);
	EEPROM_setChipSelect();
}

uint8_t EEPROM_readByte(SPIDriver* spip, uint16_t addr)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_READ_DATA);
	EEPROM_exchangeSpi(spip, (addr >> 8));
	EEPROM_exchangeSpi(spip, (addr & 0xFF));
	uint8_t retVal = EEPROM_exchangeSpi(spip, 0);
	EEPROM_setChipSelect();
	return retVal;
}

void EEPROM_writeByte(SPIDriver* spip, uint16_t addr, uint8_t data)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_WRITE_DATA);
	EEPROM_exchangeSpi(spip, (addr >> 8));
	EEPROM_exchangeSpi(spip, (addr & 0xFF));
	EEPROM_exchangeSpi(spip, data);
	EEPROM_setChipSelect();
}

void EEPROM_readRange(SPIDriver* spip, uint16_t startAddr, uint8_t* data, uint32_t length)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_READ_DATA);
	EEPROM_exchangeSpi(spip, (startAddr >> 8));
	EEPROM_exchangeSpi(spip, (startAddr & 0xFF));

	for (uint32_t i = 0; i < length; i++)
	{
		data[i] = EEPROM_exchangeSpi(spip, 0);
	}
	EEPROM_setChipSelect();
}

void EEPROM_writeRange(SPIDriver* spip, uint16_t startAddr, uint8_t* data, uint32_t length)
{
	EEPROM_clearChipSelect();
	EEPROM_exchangeSpi(spip, EEPROM_SPI_WRITE_DATA);
	EEPROM_exchangeSpi(spip, (startAddr >> 8));
	EEPROM_exchangeSpi(spip, (startAddr & 0xFF));

	for (uint32_t i = 0; i < length; i++)
	{
		EEPROM_exchangeSpi(spip, data[i]);
	}
	EEPROM_setChipSelect();
}

void EEPROM_wait(SPIDriver* spip)
{
	while(1)
	{
		uint8_t status;
		status = EEPROM_readStatusReg(spip);

		if ((status & EEPROM_STATUS_BIT_RDY) == 0)
		{
			break;
		}

		// wait for 1us
		osalThreadDelayMicroseconds(1);
	}
}
