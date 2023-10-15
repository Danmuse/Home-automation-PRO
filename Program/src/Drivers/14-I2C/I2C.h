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
#include "GPIO.h"
#include "SyncCommTWI.h"

#if defined (__cplusplus)
extern "C" {
	void I2C0_IRQHandler(void);
}
extern "C" {
	void I2C1_IRQHandler(void);
}
extern "C" {
	void I2C2_IRQHandler(void);
}
extern "C" {
	void I2C3_IRQHandler(void);
}
#endif

#define SCL_IDX 0
#define SDA_IDX 1

#define TWI_CHANNEL I2C0

#define SLOW_BAUDRATE 100000 // Clock period 100 Kb/s
#define STD_BAUDRATE 400000 // Clock period 400 Kb/s
#define FAST_BAUDRATE 1000000 // Clock period 1 Kb/s

class I2C : protected std::vector<Gpio>, public SyncCommTWI {
private:
	I2C_Type* m_TWI;
	statusComm_t m_statusComm = TWI_SUCCESS;
	const uint32_t m_baudRate;

	void EnableSWM(void);
	void EnableClock(void);
	void Config(void);
	void EnableInterrupt(void);
	void DisableInterrupt(void);

	statusComm_t CheckData(void);
protected:
	statusComm_t SendByte(uint32_t address, const uint8_t value) override;
	statusComm_t ReadByte(uint32_t address, uint8_t *value) override;
	statusComm_t SendBytes(uint32_t address, const uint8_t *value) override;
	statusComm_t ReadBytes(uint32_t address, uint8_t *value) override;

	void I2C_IRQHandler(void) override;
public:
	I2C() = delete;
	I2C(const Gpio& SCL, const Gpio& SDA, const uint32_t baudRate);
	statusComm_t transmitByte(uint32_t address, uint8_t value, const actionComm_t action);
	statusComm_t transmitBytes(uint32_t address, uint8_t *value, const actionComm_t action);
	virtual ~I2C();
};

#endif /* I2C_H_ */
