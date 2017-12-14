#ifndef _spi_h
#define _spi_h

#include "stm32f30x.h"

#define SPI3_PORT GPIOB
//#define SPI3_NSS_PORT	GPIOC

#define SPI2_PORT GPIOB

#define SPI3_SCK_PIN	GPIO_Pin_3
#define SPI3_MOSI_PIN	GPIO_Pin_5
#define SPI3_MISO_PIN	GPIO_Pin_4
#define SPI3_NSS_PIN	GPIO_Pin_15

#define SPI_SPI3_SCK_SOURCE 	GPIO_PinSource3
#define SPI_SPI3_MOSI_SOURCE 	GPIO_PinSource5
#define SPI_SPI3_MISO_SOURCE 	GPIO_PinSource4

#define SPI2_SCK_PIN	GPIO_Pin_13
#define SPI2_MOSI_PIN	GPIO_Pin_15
#define SPI2_MISO_PIN	GPIO_Pin_14

#define SPI_SPI2_SCK_SOURCE 	GPIO_PinSource13
#define SPI_SPI2_MOSI_SOURCE 	GPIO_PinSource15
#define SPI_SPI2_MISO_SOURCE 	GPIO_PinSource14

extern void SPI_Config(void);
extern uint16_t spi_rw_16(SPI_TypeDef* SPIx, uint16_t out);
extern uint8_t spi_rw_8(SPI_TypeDef* SPIx, uint8_t out);
extern void spi_config_datasize(uint8_t datasize);

#endif
