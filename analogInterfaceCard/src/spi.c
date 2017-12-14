#include "spi.h"
#include "stm32f30x.h"

void SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;

    /* Configure SPI3 pins: SCK and MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI3_SCK_PIN | SPI3_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(SPI3_PORT, &GPIO_InitStructure);

	/* Configure MISO as Input Floating */
	GPIO_InitStructure.GPIO_Pin = SPI3_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SPI3_PORT, &GPIO_InitStructure);

	/* Configure SPI3 pins: CS or NSS */
	GPIO_InitStructure.GPIO_Pin = SPI3_NSS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_PinAFConfig(SPI3_PORT, SPI_SPI3_SCK_SOURCE, GPIO_AF_6);
    GPIO_PinAFConfig(SPI3_PORT, SPI_SPI3_MISO_SOURCE, GPIO_AF_6);
    GPIO_PinAFConfig(SPI3_PORT, SPI_SPI3_MOSI_SOURCE, GPIO_AF_6);

    /* Enable SPI3 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitStructure);
    /* Enable SPI3 */
    SPI_Cmd(SPI3, ENABLE);




	/* Configure SPI2 pins: SCK and MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI2_SCK_PIN | SPI2_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

	/* Configure MISO as Input Floating */
	GPIO_InitStructure.GPIO_Pin = SPI2_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure);


    // Connect SPI pins to Alternate Function
    GPIO_PinAFConfig(SPI2_PORT, SPI_SPI2_SCK_SOURCE, GPIO_AF_5);
    GPIO_PinAFConfig(SPI2_PORT, SPI_SPI2_MISO_SOURCE, GPIO_AF_5);
    GPIO_PinAFConfig(SPI2_PORT, SPI_SPI2_MOSI_SOURCE, GPIO_AF_5);

	/* Enable SPI3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI2 */
	SPI_Cmd(SPI2, ENABLE);
}

//void spi_config_datasize(uint8_t datasize)
//{
//	SPI_InitTypeDef   SPI_InitStructure;
//
//	SPI_Cmd(SPI3, DISABLE);
//
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
//	if(datasize == 16)
//		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
//	else
//		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	SPI_Init(SPI3, &SPI_InitStructure);
//
//	SPI_Cmd(SPI3, ENABLE);
//}


uint16_t spi_rw_16(SPI_TypeDef* SPIx, uint16_t out)
{
   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

    /* Send byte through the SPI peripheral */
    SPI_I2S_SendData16(SPIx , out);

    /* Wait to receive a byte */
   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData16(SPIx);
}


uint8_t spi_rw_8(SPI_TypeDef* SPIx, uint8_t out)
{
   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

    /* Send byte through the SPI peripheral */
    SPI_I2S_SendData8(SPIx , out);

    /* Wait to receive a byte */
   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData8(SPIx);
}



