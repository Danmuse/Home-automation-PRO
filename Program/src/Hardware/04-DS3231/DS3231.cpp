/*/*!
 * @file DS3231.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 18:55:36
 */

#include "DS3231.h"

/****************** RTC Testing START ******************
int main(void) {
	initDevice();
	initDisplay();
	initDS3231();

	RTC_st rtc;
	if (g_ds3231->set(0, 0, 0, 1, 1, 2000)) return EXIT_FAILURE;

	while (1) {
		rtc = g_ds3231->get();
		if (g_ds3231->getStatus()) return EXIT_FAILURE;
		g_display->set(rtc.TIME.MIN, 0);
		g_display->set(rtc.TIME.SEC, 1);
		g_timers_list.TimerEvents();
		delay(1000);
	}
}
******************** RTC Testing END *******************/

DS3231 *g_ds3231 = nullptr;

DS3231::DS3231() : I2C(I2C0_SCL, I2C0_SDA),
m_statusRTC{RTC_OK},
m_hoursMode{TWENTY_FOUR_HOURS_MODE},
m_flagPM{false} { }

SyncCommTWI::statusComm_t DS3231::acquireTime(void) {
	uint8_t auxiliarSecond = 0, auxiliarMinute = 0, auxiliarHour = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_SEC_REG, &auxiliarSecond)) && !(this->getStatus())) {
		this->m_RTC.TIME.SEC = (auxiliarSecond >> BCD_SHIFT) * BCD_FACTOR + (auxiliarSecond & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_MIN_REG, &auxiliarMinute)) && !(this->getStatus())) {
		this->m_RTC.TIME.MIN = (auxiliarMinute >> BCD_SHIFT) * BCD_FACTOR + (auxiliarMinute & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_HOUR_REG, &auxiliarHour)) && !(this->getStatus()) && this->m_hoursMode == TWENTY_FOUR_HOURS_MODE) {
		this->m_RTC.TIME.HOUR = (auxiliarHour >> BCD_SHIFT) * BCD_FACTOR + (auxiliarHour & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_HOUR_REG, &auxiliarHour)) && !(this->getStatus()) && this->m_hoursMode != TWENTY_FOUR_HOURS_MODE) {
		if ((auxiliarHour & (1 << DS3231_12HOURS_AMPM_SHIFT)) >> DS3231_12HOURS_AMPM_SHIFT) this->m_flagPM = true;
		else this->m_flagPM = false;
		auxiliarHour &= ~((1 << DS3231_12HOURS_MODE_SHIFT) | (1 << DS3231_12HOURS_AMPM_SHIFT));
		this->m_RTC.TIME.HOUR = (auxiliarHour >> BCD_SHIFT) * BCD_FACTOR + (auxiliarHour & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitTime(uint8_t second, uint8_t minute, uint8_t hour) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_SEC_REG, second)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_MIN_REG, minute)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_HOUR_REG, hour)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::acquireCalendar(void) {
	uint8_t auxiliarDate = 0, auxiliarMonth = 0, auxiliarYear = 0;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_DATE_REG, &auxiliarDate)) && !(this->getStatus())) {
		this->m_RTC.CALENDAR.DATE = (auxiliarDate >> BCD_SHIFT) * BCD_FACTOR + (auxiliarDate & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_MTH_CENT_REG, &auxiliarMonth)) && !(this->getStatus())) {
		this->m_RTC.CALENDAR.MONTH = (auxiliarMonth >> BCD_SHIFT) * BCD_FACTOR + (auxiliarMonth & BCD_MASK);
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->receiveByte(DS3231_ADDR_REG, DS3231_YEAR_REG, &auxiliarYear)) && !(this->getStatus())) {
		this->m_RTC.CALENDAR.YEAR = (auxiliarYear >> BCD_SHIFT) * BCD_FACTOR + (auxiliarYear & BCD_MASK) + 2000;
		this->m_statusRTC = RTC_OK;
	} else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t DS3231::transmitCalendar(uint8_t date, uint8_t month, uint8_t year) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_DATE_REG, date)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_MTH_CENT_REG, month)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_YEAR_REG, year)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

RTC_st DS3231::get(void) {
	this->getTime();
	if (this->getStatus()) return this->m_RTC; // Returns the RTC structure with some errors.
	this->getCalendar();
	if (this->getStatus()) return this->m_RTC; // Returns the RTC structure with some errors.
	return this->m_RTC;
}

RTC_time_st DS3231::getTime(void) {
	this->m_statusRTC = RTC_OK;
	this->m_RTC.TIME.SEC = 0; this->m_RTC.TIME.MIN = 0; this->m_RTC.TIME.HOUR = 0;
	if (this->acquireTime()) return this->m_RTC.TIME; // Returns the Time RTC structure with some errors.
	return this->m_RTC.TIME;
}

RTC_calendar_st DS3231::getCalendar(void) {
	this->m_statusRTC = RTC_OK;
	this->m_RTC.CALENDAR.DATE = 0; this->m_RTC.CALENDAR.MONTH = 0; this->m_RTC.CALENDAR.YEAR = 0;
	if (this->acquireCalendar()) return this->m_RTC.CALENDAR; // Returns the Calendar RTC structure with some errors.
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
	uint8_t auxiliarSecond = 0, auxiliarMinute = 0, auxiliarHour = 0;
	this->m_statusRTC = RTC_OK;

	if (second > MAX_SEC) {
		this->m_statusRTC = RTC_SEC_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliarSecond |= (((second / BCD_FACTOR) << BCD_SHIFT) | (second % BCD_FACTOR));

	if (minute > MAX_MIN) {
		this->m_statusRTC = RTC_MIN_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliarMinute |= (((minute / BCD_FACTOR) << BCD_SHIFT) | (minute % BCD_FACTOR));

	if (hour < MAX_HOUR + 1) {
		if (this->m_hoursMode == TWELVE_HOURS_MODE) {
			if (hour > 12) {
				hour -= 12;
				auxiliarHour |= (((hour / BCD_FACTOR) << BCD_SHIFT) | (hour % BCD_FACTOR));
				auxiliarHour |= ((1 << DS3231_12HOURS_AMPM_SHIFT) | (1 << DS3231_12HOURS_MODE_SHIFT));
				this->m_flagPM = true;
			} else {
				auxiliarHour |= (((hour / BCD_FACTOR) << BCD_SHIFT) | (hour % BCD_FACTOR));
				auxiliarHour |= (1 << DS3231_12HOURS_MODE_SHIFT);
				this->m_flagPM = false;
			}
		} else {
			this->m_flagPM = false;
			// auxiliarHour &= ~((1 << DS3231_12HOURS_MODE_SHIFT) | (1 << DS3231_12HOURS_AMPM_SHIFT));
			auxiliarHour |= (((hour / BCD_FACTOR) << BCD_SHIFT) | (hour % BCD_FACTOR));
		}
	} else {
		this->m_statusRTC = RTC_HOUR_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	}

	this->transmitTime(auxiliarSecond, auxiliarMinute, auxiliarHour);
	return this->getStatus();
}

RTC_result_t DS3231::setCalendar(uint8_t date, uint8_t month, uint16_t year) {
	uint8_t auxiliarDate = 0, auxiliarMonth = 0, auxiliarYear = 0;
	this->m_statusRTC = RTC_OK;

	if (date > MAX_DATE) {
		this->m_statusRTC = RTC_DATE_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliarDate |= (((date / BCD_FACTOR) << BCD_SHIFT) | (date % BCD_FACTOR));

	if (month > MAX_MONTH) {
		this->m_statusRTC = RTC_MONTH_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliarMonth |= (((month / BCD_FACTOR) << BCD_SHIFT) | (month % BCD_FACTOR));

	if (year > MAX_YEAR) {
		this->m_statusRTC = RTC_YEAR_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliarYear |= (uint8_t)((((year % 100) / BCD_FACTOR) << BCD_SHIFT) | ((year % 100) % BCD_FACTOR));

	this->transmitCalendar(auxiliarDate, auxiliarMonth, auxiliarYear);
	return this->getStatus();
}

void DS3231::changeHoursMode(hoursMode_t hoursMode) {
	this->m_hoursMode = hoursMode;
	this->m_flagPM = false;
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
	#if defined(I2C0_PINS) || defined(I2C1_PINS) || defined(I2C2_PINS) || defined(I2C3_PINS)

	static DS3231 ds3231;
	ds3231.set(0, 0, 0, 1, 1, 2000);

	g_ds3231 = &ds3231;

	#endif // defined(I2C0_PINS) || defined(I2C1_PINS)
}
