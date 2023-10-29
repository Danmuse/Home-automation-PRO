/*!
 * @file SPI.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/09/2023 11:00:42
 * @brief Serial Peripheral Interface Class API (Application Programming Interface).
 */

#ifndef SPI_H_
#define SPI_H_

#include <vector>
#include "systick.h"
#include "SyncCommSPI.h"
#include "GPIO.h"

#if defined (__cplusplus)
extern "C" {
	void SPI0_IRQHandler(void);
	void SPI1_IRQHandler(void);
}
#endif

#define SCK_IDX 0
#define MOSI_IDX 1
#define MISO_IDX 2
#define SSEL_IDX 3

#define MAX_SPI_CHANNELS 2

#define SPI_OFFSET_BASE (0x4000U)

#define SCK_TICKS 30

#define	ON 1
#define	OFF	0

#define	COMMAND_DISABLE_QIO	0xFF
#define	COMMAND_WRITE_UNLOCK_PROTECTION	0x98
#define	COMMAND_ENABLE 0x06
#define	COMMAND_DISABLE	0x04
#define	COMMAND_READ_STATUS	0x05
#define	COMMAND_WRITE_STATUS 0x01
#define	COMMAND_WRITE_BLOCK_PROTECTION 0x42
#define	COMMAND_WRITE_UNLOCK_PROTECTION	0x98
#define	COMMAND_READ_CONF 0x35

class SPI : protected std::vector<Gpio>, public SyncCommSPI, Callback {
private:
	SPI_Type* m_SPI;
	uint8_t m_ticks;
protected:
	enum channel_t { FST_SPI, SND_SPI };
public:
	SPI() = delete;
	SPI(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SSEL, channel_t channel = FST_SPI);
	void Memory_Inicialization(void); // UNUSED METHOD
	void HW_Enable(void);
	void HW_Disable(void);
	void HW_Write(uint8_t data);
	uint8_t HW_Read(void);
	void Write_Enable(void);
	void Write_Disable(void);
	void CallbackMethod(void);
	void SW_Reset(void);
	uint8_t Read_Configuration(uint8_t command);
	void Save_Configuration(uint8_t command, uint8_t *data, uint32_t size);
	void Wait_Busy(void);
	void WriteBlock_Protection(void);
	void Jedec_Id_Read(void);
	virtual ~SPI();
private:
	void EnableSWM(void);
	void EnableClock(void);
	void Config(void);
	void SPI_IRQHandler(void) override;

	void EnableInterrupt(void);
	void DisableInterrupt(void);
};

#endif /* SPI_H_ */
