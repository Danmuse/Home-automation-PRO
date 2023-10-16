/*/*!
 * @file DS3231.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 18:55:36
 */

#include "DS3231.h"

/****************** RTC Testing START ******************
RTC_t rtc_t;
RTC_result_t set_status, read_status;

g_ds3231->set(0, 0, 0, 31, 6, 12, 2022);
set_status = g_ds3231->getStatus();

uint8_t secondCount = 0;
char RTCstr[RTC_STR_SIZE];

while (secondCount < 5) {
	g_ds3231->read(&rtc_t);
	read_status = g_ds3231->getStatus();
	printStringUSB(g_ds3231->print());
	set_status ? printStringUSB("FAILURE - RTC_set result\n") : printStringUSB("SUCCESS - RTC_set result\n");
	read_status ? printStringUSB("FAILURE - RTC_read result\n") : printStringUSB("SUCCESS - RTC_read result\n");

	delay(1000); // Waiting for a second
}
******************** RTC Testing END *******************/

DS3231 *g_ds3231 = nullptr;

DS3231::DS3231() : I2C(TWI_CHANNEL, I2C0_SCL, I2C0_SDA),
m_statusRTC{RTC_OK} { }

SyncCommTWI::statusComm_t DS3231::acquireSec(void) {
	uint8_t auxiliar = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_SEC_REG, &auxiliar))) {
		this->m_RTC.TIME.SEC = (auxiliar >> BCD_SHIFT) * BCD_FACTOR + (auxiliar & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::acquireMin(void) {
	uint8_t auxiliar = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_MIN_REG, &auxiliar))) {
		this->m_RTC.TIME.MIN = (auxiliar >> BCD_SHIFT) * BCD_FACTOR + (auxiliar & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::acquireHour(void) {
	//////////////////////////////////////////
	// Check the following fragment of code //
	//////////////////////////////////////////

	uint8_t auxiliar = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_HOUR_REG, &auxiliar))) {
		this->m_RTC.TIME.HOUR = (auxiliar >> BCD_SHIFT) * BCD_FACTOR + (auxiliar & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitSec(uint8_t second) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_SEC_REG, second))) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitMin(uint8_t minute) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_MIN_REG, minute))) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitHour(uint8_t hour) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_HOUR_REG, hour))) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::acquireDate(void) {
	uint8_t auxiliar = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_DATE_REG, &auxiliar))) {
		this->m_RTC.CALENDAR.DATE = (auxiliar >> BCD_SHIFT) * BCD_FACTOR + (auxiliar & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::acquireMonth(void) {
	uint8_t auxiliar = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_MTH_CENT_REG, &auxiliar))) {
		this->m_RTC.CALENDAR.MONTH = (auxiliar >> BCD_SHIFT) * BCD_FACTOR + (auxiliar & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::acquireYear(void) {
	uint8_t auxiliar = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_YEAR_REG, &auxiliar))) {
		this->m_RTC.CALENDAR.YEAR = (auxiliar >> BCD_SHIFT) * BCD_FACTOR + (auxiliar & BCD_MASK) + 2000;
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitDate(uint8_t date) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_DATE_REG, date))) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitMonth(uint8_t month) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_MTH_CENT_REG, month))) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitYear(uint8_t year) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_YEAR_REG, year))) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

RTC_t DS3231::get(void) {
	this->getTime();
	if (this->getStatus()) return this->m_RTC; // Returns the RTC structure with some errors.
	this->getCalendar();
	if (this->getStatus()) return this->m_RTC; // Returns the RTC structure with some errors.
	return this->m_RTC;
}

RTC_time_t DS3231::getTime(void) {
	this->m_statusRTC = RTC_OK;
	this->m_RTC.TIME.SEC = 0; this->m_RTC.TIME.MIN = 0; this->m_RTC.TIME.HOUR = 0;
	if (this->acquireSec()) return this->m_RTC.TIME; // Returns the Time RTC structure with some errors.
	if (this->acquireMin()) return this->m_RTC.TIME; // Returns the Time RTC structure with some errors.
	if (this->acquireHour()) return this->m_RTC.TIME; // Returns the Time RTC structure with some errors.
	return this->m_RTC.TIME;
}

RTC_calendar_t DS3231::getCalendar(void) {
	this->m_statusRTC = RTC_OK;
	this->m_RTC.CALENDAR.DATE = 0; this->m_RTC.CALENDAR.MONTH = 0; this->m_RTC.CALENDAR.YEAR = 0;
	if (this->acquireDate()) return this->m_RTC.CALENDAR; // Returns the Calendar RTC structure with some errors.
	if (this->acquireMonth()) return this->m_RTC.CALENDAR; // Returns the Calendar RTC structure with some errors.
	if (this->acquireYear()) return this->m_RTC.CALENDAR; // Returns the Calendar RTC structure with some errors.
	return this->m_RTC.CALENDAR;
}

RTC_result_t DS3231::getStatus(void) const {
	return this->m_statusRTC;
}

RTC_result_t DS3231::set(uint8_t second, uint8_t minute, uint8_t hour, uint8_t date, uint8_t month, uint16_t year) {
	if (this->setTime(second, minute, hour)) return this->getStatus(); // Returns with some errors during the configuration.
	if (this->setCalendar(date, month, year)) return this->getStatus(); // Returns with some errors during the configuration.
	return this->getStatus();
}

RTC_result_t DS3231::setTime(uint8_t second, uint8_t minute, uint8_t hour) {
	uint8_t auxiliar = 0;
	this->m_statusRTC = RTC_OK;

	if (second > MAX_SEC) {
		this->m_statusRTC = RTC_SEC_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliar |= (((second / BCD_FACTOR) << BCD_SHIFT) | (second % BCD_FACTOR));

	if (this->transmitSec(auxiliar)) return this->getStatus(); // Returns with some errors during the configuration.

	if (minute > MAX_MIN) {
		this->m_statusRTC = RTC_MIN_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliar |= (((minute / BCD_FACTOR) << BCD_SHIFT) | (minute % BCD_FACTOR));

	if (this->transmitMin(auxiliar)) return this->getStatus(); // Returns with some errors during the configuration.

	if (hour < MAX_HOUR + 1) {
		auxiliar = hour;
		if (HOURS_MODE) {
			if (hour > 12) {
				auxiliar -= 12;
				auxiliar |= (((auxiliar / BCD_FACTOR) << BCD_SHIFT) | (auxiliar % BCD_FACTOR));
				auxiliar |= ((1 << DS3231_12HOURS_AMPM_SHIFT) | (1 << DS3231_12HOURS_MODE_SHIFT));
			} else {
				auxiliar |= (((auxiliar / BCD_FACTOR) << BCD_SHIFT) | (auxiliar % BCD_FACTOR));
				auxiliar |= (1 << DS3231_12HOURS_MODE_SHIFT);
			}
		} else auxiliar |= (((auxiliar / BCD_FACTOR) << BCD_SHIFT) | (auxiliar % BCD_FACTOR));
	} else {
		this->m_statusRTC = RTC_HOUR_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	}

	if (this->transmitHour(auxiliar)) return this->getStatus(); // Returns with some errors during the configuration.
	return this->getStatus();
}

RTC_result_t DS3231::setCalendar(uint8_t date, uint8_t month, uint16_t year) {
	uint8_t auxiliar = 0;
	this->m_statusRTC = RTC_OK;

	if (date > MAX_DATE) {
		this->m_statusRTC = RTC_DATE_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliar |= (((date / BCD_FACTOR) << BCD_SHIFT) | (date % BCD_FACTOR));

	if (this->transmitDate(auxiliar)) return this->getStatus(); // Returns with some errors during the configuration.

	if (month > MAX_MONTH) {
		this->m_statusRTC = RTC_MONTH_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliar |= (((month / BCD_FACTOR) << BCD_SHIFT) | (month % BCD_FACTOR));

	if (this->transmitMonth(auxiliar)) return this->getStatus(); // Returns with some errors during the configuration.

	if (year > MAX_YEAR) {
		this->m_statusRTC = RTC_YEAR_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliar |= (uint8_t)((((year % 100) / BCD_FACTOR) << BCD_SHIFT) | ((year % 100) % BCD_FACTOR));

	if (this->transmitYear(auxiliar)) return this->getStatus(); // Returns with some errors during the configuration.
	return this->getStatus();
}

//char *DS3231::print(char RTCstr[RTC_STR_SIZE]) {
//	sprintf(RTCstr, "%02u:%02u:%02u %02u/%02u/%04u\n", this->m_RTC.TIME.HOUR, this->m_RTC.TIME.MIN, this->m_RTC.TIME.SEC,
//			this->m_RTC.CALENDAR.DATE, this->m_RTC.CALENDAR.MONTH, this->m_RTC.CALENDAR.YEAR);
//	return RTCstr;
//}

DS3231::~DS3231() { }

/////////////////////////////
/// DS3231 initialization ///
/////////////////////////////

void initDS3231(void) {
	#ifdef I2C0_PINS

	static DS3231 ds3231;
	ds3231.set(0, 0, 0, 1, 1, 2000);

	g_ds3231 = &ds3231;

	#endif // I2C0_PINS
}
