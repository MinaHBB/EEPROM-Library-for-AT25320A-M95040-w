/*
 * eeprom.h
 *
 *  Created on: Feb 18, 2024
 *      Author: Mina Habibi
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "stdint.h"
#include "components.h"
#include "spi_lld.h"

/**
 * @brief The RDY (READY) bit of the status register
 */
#define EEPROM_STATUS_BIT_RDY 	0x01

/**
 * @brief The WEN (write enabled) bit of the status register
 */
#define EEPROM_STATUS_BIT_WEN 	0x02

/**
 * @brief The BP (Block Protection) bits of the status register
 */
#define EEPROM_STATUS_BIT_BP  	0x0C

/**
 * @brief The WPEN (Write Protect Enable) bit of status register
 */
#define EEPROM_STATUS_BIT_WPEN	0x80

/**
 * @brief Block protection settings for EEPROM.
 */
typedef enum
{
	BlockProtection_None		= 0,	/**< None			*/
	BlockProtection_Quarter		= 1,	/**< 0x0C00 - 0x0FFF*/
	BlockProtection_Half		= 2,	/**< 0x0800 - 0x0FFF*/
	BlockProtection_WholeMemory	= 3,	/**< 0x0000 - 0x0FFF*/
}EEPROM_BlockProtection;

/**
 * @brief Structure representing the EEPROM status register.
 */
typedef struct _EEPROM_STAT_REG
{
	union
	{
		uint8_t R;
		struct
		{
			uint8_t WPEN 	:1;
			uint8_t UNUSED	:3;
			uint8_t BP		:2;
			uint8_t WEN 	:1;
			uint8_t RDY		:1;
		}B;
	}SR;
}EEPROM_StatusRegister;

/**
 * @brief Configures and activates the SPI peripheral.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 * @param[in] pConfig Pointer to the SPIConfig structure.
 *
 * @return None.
 */
void EEPROM_startSpi(SPIDriver* spip, SPIConfig* pConfig);

/**
 * @brief Deactivates the SPI peripheral.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 *
 * @return None.
 */
void EEPROM_stopSpi(SPIDriver* spip);

/**
 * @brief Enable EEPROM write operations.
 *
 * This function sends the SPI command to enable write operations on the EEPROM.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 *
 * @return None.
 */
void EEPROM_enableWrite(SPIDriver* spip);

/**
 * @brief Disable EEPROM write operations.
 *
 * This function sends the SPI command to disable write operations on the M95040 EEPROM.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 *
 * @return None.
 */
void EEPROM_disableWrite(SPIDriver* spip);

/**
 * @brief Read the EEPROM status register.
 *
 * This function sends the SPI command to read the status register of the EEPROM
 * and returns the value read.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 *
 * @return The value read from the status register.
 */
uint8_t EEPROM_readStatusReg(SPIDriver* spip);


/**
 * @brief Write to the EEPROM status register.
 *
 * This function sends the SPI command and data to write to the status register of the EEPROM.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 * @param[in] cmd The data to be written to the status register.
 *
 * @return None.
 */
void EEPROM_writeStatusReg(SPIDriver* spip, uint8_t cmd);

/**
 * @brief Read a byte from the EEPROM at the specified address.
 *
 * This function sends the SPI command and address to read a byte from the EEPROM
 * at the specified address.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 * @param[in] addr The address from which to read the byte.
 *
 * @return The byte read from the EEPROM.
 */
uint8_t EEPROM_readByte(SPIDriver* spip, uint16_t addr);

/**
 * @brief Write a byte to the EEPROM at the specified address.
 *
 * This function writes a byte of data to the EEPROM at the given address.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 * @param[in] addr The address where the data will be written.
 * @param[in] data The byte of data to be written.
 *
 * @return None.
 */
void EEPROM_writeByte(SPIDriver* spip, uint16_t addr, uint8_t data);

/**
 * @brief Read a range of bytes from the EEPROM starting from the specified address.
 *
 * This function sends the SPI command and starting address to read a range of bytes from
 * the EEPROM and stores the data in the provided data buffer.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 * @param[in] startAddr The starting address from where to read the data.
 * @param[out] data Pointer to the data buffer where the read data will be stored.
 * @param[in] length The number of bytes to read.
 *
 * @return None.
 */
void EEPROM_readRange(SPIDriver* spip, uint16_t startAddr, uint8_t* data, uint32_t length);

/**
 * @brief Write a range of bytes to the EEPROM starting from the specified address.
 *
 * This function sends the SPI command, starting address, and data to write the data range
 * to the EEPROM.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 * @param[in] startAddr The starting address where the data will be written.
 * @param[in] data Pointer to the data buffer containing the data to be written.
 * @param[in] length The number of bytes to write.
 *
 * @return None.
 */
void EEPROM_writeRange(SPIDriver* spip, uint16_t startAddr, uint8_t* data, uint32_t length);

/**
 * @brief Wait for the EEPROM's write operation to complete.
 *
 * This function continuously reads the status register of the EEPROM and waits
 * until the <span style="text-decoration: overline;">RDY</span> bit becomes 0, indicating that the write operation is complete.
 *
 * @param[in] spip Pointer to the SPIDriver structure.
 *
 * @return None.
 */
void EEPROM_wait(SPIDriver* spip);

#endif /* EEPROM_H_ */
