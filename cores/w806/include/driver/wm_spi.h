#ifndef __WM_SPI_H__
#define __WM_SPI_H__

#include "wm_hal.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct
{
	uint32_t Mode;                /*!< Specifies the SPI operating mode.
									 This parameter can be a value of @ref SPI_Mode */

	uint32_t CLKPolarity;         /*!< Specifies the serial clock steady state.
									 This parameter can be a value of @ref SPI_Clock_Polarity */

	uint32_t CLKPhase;            /*!< Specifies the clock active edge for the bit capture.
									 This parameter can be a value of @ref SPI_Clock_Phase */

	uint32_t NSS;                 /*!< Specifies whether the NSS signal is managed by
									 hardware (NSS pin) or by software using the SSI bit.
									 This parameter can be a value of @ref SPI_Slave_Select_management */

	uint32_t BaudRatePrescaler;   /*!< Specifies the Baud Rate prescaler value which will be
									 used to configure the transmit and receive SCK clock.
									 This parameter can be a value of @ref SPI_BaudRate_Prescaler
									 @note The communication clock is derived from the master
									 clock. The slave clock does not need to be set. */
									 
	uint32_t FirstByte;			  /* Specifies whether data transfers start from high byte or low byte.
									 This parameter can be a value of @ref SPI_HBYTE_LBYTE_transmission */
									 

} SPI_InitTypeDef;

typedef enum
{
	HAL_SPI_STATE_RESET      = 0x00U,    /*!< Peripheral not Initialized                         */
	HAL_SPI_STATE_READY      = 0x01U,    /*!< Peripheral Initialized and ready for use           */
	HAL_SPI_STATE_BUSY       = 0x02U,    /*!< an internal process is ongoing                     */
	HAL_SPI_STATE_BUSY_TX    = 0x03U,    /*!< Data Transmission process is ongoing               */
	HAL_SPI_STATE_BUSY_RX    = 0x04U,    /*!< Data Reception process is ongoing                  */
	HAL_SPI_STATE_BUSY_TX_RX = 0x05U,    /*!< Data Transmission and Reception process is ongoing */
	HAL_SPI_STATE_ERROR      = 0x06U,    /*!< SPI error state                                    */
	HAL_SPI_STATE_ABORT      = 0x07U     /*!< SPI abort is ongoing                               */
} HAL_SPI_StateTypeDef;

typedef struct __SPI_HandleTypeDef
{
	SPI_TypeDef                *Instance;      /*!< SPI registers base address               */

	SPI_InitTypeDef            Init;           /*!< SPI communication parameters             */

	uint8_t                    *pTxBuffPtr;    /*!< Pointer to SPI Tx transfer Buffer        */

	uint32_t                   TxXferSize;     /*!< SPI Tx Transfer size                     */

	__IO uint32_t              TxXferCount;    /*!< SPI Tx Transfer Counter                  */

	uint8_t                    *pRxBuffPtr;    /*!< Pointer to SPI Rx transfer Buffer        */

	uint32_t                   RxXferSize;     /*!< SPI Rx Transfer size                     */

	__IO uint32_t              RxXferCount;    /*!< SPI Rx Transfer Counter                  */

	HAL_LockTypeDef            Lock;           /*!< Locking object                           */

	__IO HAL_SPI_StateTypeDef  State;          /*!< SPI communication state                  */

	__IO uint32_t              ErrorCode;      /*!< SPI Error code                           */

} SPI_HandleTypeDef;


#define HSPI_BASE                ((SPI_TypeDef *)SPI_BASE)

#define HAL_SPI_ERROR_NONE              (0x00000000U)   /*!< No error                               */

// SPI_Mode
#define SPI_MODE_SLAVE                  (0x00000000U)
#define SPI_MODE_MASTER                 (SPI_SPI_CFG_MASTER)

// SPI_Clock_Polarity
#define SPI_POLARITY_LOW                (0x00000000U)
#define SPI_POLARITY_HIGH               SPI_SPI_CFG_CPOL

// SPI_Clock_Phase
#define SPI_PHASE_1EDGE                 (0x00000000U)
#define SPI_PHASE_2EDGE                 SPI_SPI_CFG_CPHA

// SPI_Slave_Select_management
#define SPI_NSS_HARD             		(0x00000000U)
#define SPI_NSS_SOFT                    SPI_CH_CFG_CSSEL

// SPI_HBYTE_LBYTE_transmission
#define SPI_LITTLEENDIAN				(0x00000000U)
#define SPI_BIGENDIAN					SPI_SPI_CFG_BIGENDIAN

// fclk = 40MHz / (2 * (div + 1))
#define SPI_BAUDRATEPRESCALER_2         (0x00000000U)	// 40M / 2 = 20M
#define SPI_BAUDRATEPRESCALER_4         (0x00000001U)	// 40M / 4 = 10M
#define SPI_BAUDRATEPRESCALER_8         (0x00000003U)	// 40M / 8 = 5M
#define SPI_BAUDRATEPRESCALER_10        (0x00000004U)	// 40M / 10 = 4M
#define SPI_BAUDRATEPRESCALER_20        (0x00000009U)	// 40M / 20 = 2M
#define SPI_BAUDRATEPRESCALER_40        (0x00000013U)	// 40M / 40 = 1M


#define BLOCK_SIZE (8 * 1024 - 4)

#define IS_SPI_ALL_INSTANCE(INSTANCE) ((INSTANCE) == SPI1)

#define IS_SPI_MODE(__MODE__)      ((__MODE__) == SPI_MODE_MASTER)

#define IS_SPI_NSS(__NSS__)        (((__NSS__) == SPI_NSS_SOFT)       || \
                                    ((__NSS__) == SPI_NSS_HARD_INPUT) || \
                                    ((__NSS__) == SPI_NSS_HARD_OUTPUT))
									
#define IS_SPI_BIG_OR_LITTLE(__ENDIAN__)	   (((__ENDIAN__) == SPI_LITTLEENDIAN) || \
												((__ENDIAN__) == SPI_BIGENDIAN))
									
#define __HAL_SPI_ENABLE_TX(__HANDLE__)  SET_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_TXON)

#define __HAL_SPI_ENABLE_RX(__HANDLE__)  SET_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_RXON)

#define __HAL_SPI_ENABLE(__HANDLE__)  SET_BIT((__HANDLE__)->Instance->CH_CFG, (SPI_CH_CFG_RXON | SPI_CH_CFG_TXON))

#define __HAL_SPI_DISABLE(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->CH_CFG, (SPI_CH_CFG_RXON | SPI_CH_CFG_TXON | SPI_CH_CFG_START))

#define __HAL_SPI_CLEAR_FIFO(__HANDLE__) do{SET_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_CLEARFIFOS);\
											while(READ_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_CLEARFIFOS));}while(0U);

#define __HAL_SPI_GET_TXFIFO(__HANDLE__) (((__HANDLE__)->Instance->STATUS) & SPI_STATUS_TXFIFO)

#define __HAL_SPI_GET_RXFIFO(__HANDLE__) ((((__HANDLE__)->Instance->STATUS) & SPI_STATUS_RXFIFO) >> SPI_STATUS_RXFIFO_Pos)

#define __HAL_SPI_SET_CLK_NUM(__HANDLE__, NUM)	(MODIFY_REG((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_LEN, NUM << SPI_CH_CFG_LEN_Pos))

#define __HAL_SPI_SET_START(__HANDLE__) (SET_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_START))

#define __HAL_SPI_GET_START(__HANDLE__) (READ_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_START))

#define __HAL_SPI_GET_BUSY_STATUS(__HANDLE__)	(READ_BIT((__HANDLE__)->Instance->STATUS, SPI_STATUS_BUSY))

#define __HAL_SPI_SET_CS_LOW(__HANDLE__)	CLEAR_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_CSLEVEL)

#define __HAL_SPI_SET_CS_HIGH(__HANDLE__)	SET_BIT((__HANDLE__)->Instance->CH_CFG, SPI_CH_CFG_CSLEVEL)

#define __HAL_SPI_GET_FLAG(__HANDLE__, FLAG)	READ_BIT((__HANDLE__)->Instance->INT_SRC, FLAG)

#define __HAL_SPI_CELAR_FLAG(__HANDLE__, FLAG)	SET_BIT((__HANDLE__)->Instance->INT_SRC, FLAG)

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint32_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint32_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint32_t Size,
                                          uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif