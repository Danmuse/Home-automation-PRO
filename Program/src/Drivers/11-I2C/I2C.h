/*!
 * @file I2C.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 * @brief Inter-Integrated Circuit Class API (Application Programming Interface).
 */

#ifndef I2C_H_
#define I2C_H_

#include <vector>
#include "SyncCommTWI.h"
#include "GPIO.h"

#if defined (__cplusplus)
extern "C" {
	void I2C0_IRQHandler(void);
	void I2C1_IRQHandler(void);
	void I2C2_IRQHandler(void);
	void I2C3_IRQHandler(void);
}
#endif

#define SCL_IDX 0
#define SDA_IDX 1

#define MAX_I2C_CHANNELS 4

#define I2C_STAT_MSTSTATE_NACK_ADDR_MASK (0x3U << I2C_STAT_MSTSTATE_SHIFT)
#define I2C_STAT_MSTSTATE_NACK_DATA_MASK (0x4U << I2C_STAT_MSTSTATE_SHIFT)

#define I2C_OFFSET_BASE (0x4000U)

class I2C : protected std::vector<Gpio>, public SyncCommTWI {
private:
	I2C_Type* m_TWI;
	statusComm_t m_statusComm = TWI_SUCCESS;
	frequencyComm_t m_frequency;

	void EnableSWM(void);
	void EnableClock(void);
	void Config(void);
	void EnableInterrupt(void);
	void DisableInterrupt(void);

	statusComm_t awaitACK(void);
	statusComm_t awaitNACK(void);
	statusComm_t prepareConditions(const uint8_t address, const uint8_t regOffset, actionComm_t action) override;
	statusComm_t transmitStopBit(void) override;
protected:
	statusComm_t transmitByte(const uint8_t address, const uint8_t regOffset, uint8_t value, actionComm_t action = WRITE) override;
	statusComm_t receiveByte(const uint8_t address, const uint8_t regOffset, uint8_t* value, actionComm_t action = READ) override;
	statusComm_t transmitBytes(const uint8_t address, const uint8_t regOffset, uint8_t values[], size_t numBytes, actionComm_t action = WRITE) override;
	statusComm_t receiveBytes(const uint8_t address, const uint8_t regOffset, uint8_t values[], size_t numBytes, actionComm_t action = READ) override;

	void I2C_IRQHandler(void) override;
public:
	enum channelTWI_t { TWI0, TWI1, TWI2, TWI3 };

	I2C() = delete;
	I2C(const Gpio& SCL, const Gpio& SDA, channelTWI_t channel = TWI0, frequencyComm_t frequency = STD_FREQUENCY);
	virtual ~I2C();
};

#endif /* I2C_H_ */
