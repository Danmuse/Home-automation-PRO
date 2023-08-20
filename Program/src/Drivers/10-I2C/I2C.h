/*!
 * @file I2C.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 * @brief Inter-Integrated Circuit Class API (Application Programming Interface).
 */

#ifndef I2C_H_
#define I2C_H_

#include "SyncComm.h"
#include "GPIO.h"

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

#define TWI_I2C		I2C0
#define PORT_SCL	Gpio::PORT0
#define PORT_SDA	Gpio::PORT0
#define PIN_SCL		10
#define PIN_SDA		11

class I2C : public SyncComm {
public:
	enum statusComm_t { SUCCESS, ACK_FAULT, NACK_FAULT };
private:
	const Gpio m_SCL;
	const Gpio m_SDA;
	I2C_Type* m_TWI;
	statusComm_t m_statusComm;
public:
	I2C();
	void SendData(uint32_t address, const uint8_t value) override;
	void ReadData(uint32_t address, const uint8_t *value) override;
	virtual ~I2C();
private:
	void EnableSWM(void);
	void EnableClock(void);
	void Config(void);
	void I2C_IRQHandler(void) override;

	void EnableInterupt(void);
	void DisableInterupt(void);
};

#endif /* I2C_H_ */
