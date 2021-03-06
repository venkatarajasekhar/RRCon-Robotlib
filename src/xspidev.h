/**
    @file xspidev.h
    
    @section DESCRIPTION    
    
    Robotics library for the Autonomous Robotics Development Platform  
    
    @brief [HEADER] SPIDEV primitive functions (full/half duplex)
    
*/

#ifndef __SPIDEV_H__
#define __SPIDEV_H__

#include <native/mutex.h>

#define SPI_DEFAULT_SPEED 500000
//TODO: IS THIS DELAY CORRECT?
#define SPI_DEFAULT_DELAY 0
#define SPI_DEFAULT_BITS 8 

#define DEVBUFFERSIZE 25

typedef struct {
    char device[DEVBUFFERSIZE]; ///< File name of the device
    int fd; ///< File descriptor
    uint8_t mode; ///< SPI Mode
    uint8_t bits; ///< Number of bits at a time
    uint32_t speed; ///< SPI frequency speed
    uint16_t delay; ///< Communication delay
    RT_MUTEX mutex; ///< Xenomai Mutex
} XSPIDEV;

int spi_init(	XSPIDEV* xspi, 
		const char* devname, // DEVICE NAME 
		uint32_t speed, // SPEED IN HZ
		uint16_t delay, // DELAY IN ¿¿??
		uint8_t bits, // BITS AT A TIME
		uint8_t loop, // LOOPBACK MODE
		uint8_t cpha, // CPHA
		uint8_t cpol, // CPOL 
		uint8_t lsb_first, // LSB FIRST
		uint8_t cs_high, // CS HIGH
		uint8_t spi_3wire); // 3 WIRE SPI MODE

int spi_clean(XSPIDEV* xspi);
//transfer full-duplex
int spi_full_transfer(XSPIDEV* xspi, uint8_t* tx, uint8_t* rx, int len);
// write only
int spi_half_transfer(XSPIDEV* xspi, uint8_t* data, int len);
//read read only
int spi_half_read(XSPIDEV* xspi , uint8_t* data, int len);

#endif
