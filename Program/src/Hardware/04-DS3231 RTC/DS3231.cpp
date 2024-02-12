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
		g_timers_list.timerEvents();
		delay(1000);
	}
}
******************** RTC Testing END *******************/

DS3231 *g_ds3231 = nullptr;

DS3231::DS3231(const Gpio& SCL, const Gpio& SDA, channelTWI_t channel) : I2C(SCL, SDA, channel),
m_statusRTC{RTC_OK},
m_hoursMode{TWENTY_FOUR_HOURS_MODE},
m_flagPM{false} { }

/*!
 * @brief Acquire the time from the RTC and load it into internal variable.
 * @return TWI_SUCCESS if the time is acquired successfully, otherwise TWI_FAILURE.
 */
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

/*!
 * @brief Transmit the time to the RTC.
 * @param second Seconds to be transmitted.
 * @param minute Minutes to be transmitted.
 * @param hour Hours to be transmitted.
 * @return TWI_SUCCESS if the time is transmitted successfully, otherwise TWI_FAILURE.
 */
SyncCommTWI::statusComm_t DS3231::transmitTime(uint8_t second, uint8_t minute, uint8_t hour) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_SEC_REG, second)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_MIN_REG, minute)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_HOUR_REG, hour)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

/*!
 * @brief Acquire the calendar from the RTC and load it into internal variable.
 * @return TWI_SUCCESS if the calendar is acquired successfully, otherwise TWI_FAILURE.
 */
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

/*!
 * @brief Transmit a date to the RTC.
 * @param date Day to be transmitted.
 * @param month Month to be transmitted.
 * @param year Year to be transmitted.
 * @return TWI_SUCCESS if the date is transmitted successfully, otherwise TWI_FAILURE.
 */
SyncCommTWI::statusComm_t DS3231::transmitCalendar(uint8_t date, uint8_t month, uint8_t year) {
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_DATE_REG, date)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_MTH_CENT_REG, month)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	if (!(this->transmitByte(DS3231_ADDR_REG, DS3231_YEAR_REG, year)) && !(this->getStatus())) this->m_statusRTC = RTC_OK;
	else this->m_statusRTC = RTC_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

/*!
 * @brief Returns the current time and date from the RTC.
 * @return RTC struct that contains the current time and date.
 */
RTC_st DS3231::get(void) {
	this->getTime();
	if (this->getStatus()) return this->m_RTC; // Returns the RTC structure with some errors.
	this->getCalendar();
	if (this->getStatus()) return this->m_RTC; // Returns the RTC structure with some errors.
	return this->m_RTC;
}

/*!
 * @brief Returns the current time from the RTC.
 * @return RTC struct that contains the current time.
 */
RTC_time_st DS3231::getTime(void) {
	this->m_statusRTC = RTC_OK;
	this->m_RTC.TIME.SEC = 0; this->m_RTC.TIME.MIN = 0; this->m_RTC.TIME.HOUR = 0;
	if (this->acquireTime()) return this->m_RTC.TIME; // Returns the Time RTC structure with some errors.
	return this->m_RTC.TIME;
}

/*!
 * @brief Returns the current date from the RTC.
 * @return RTC struct that contains the current date.
 */
RTC_calendar_st DS3231::getCalendar(void) {
	this->m_statusRTC = RTC_OK;
	this->m_RTC.CALENDAR.DATE = 0; this->m_RTC.CALENDAR.MONTH = 0; this->m_RTC.CALENDAR.YEAR = 0;
	if (this->acquireCalendar()) return this->m_RTC.CALENDAR; // Returns the Calendar RTC structure with some errors.
	return this->m_RTC.CALENDAR;
}

/*!
 * @brief Returns the current status of the RTC.
 * @return RTC status
 */
RTC_result_t DS3231::getStatus(void) const {
	return this->m_statusRTC;
}

/*!
 * @brief Set the current time and date to the RTC.
 * @param second Seconds to be set.
 * @param minute Minutes to be set.
 * @param hour Hours to be set.
 * @param date Day to be set.
 * @param month Month to be set.
 * @param year Year to be set.
 * @return status of the RTC. RTC_OK if successful.
 */
RTC_result_t DS3231::set(uint8_t second, uint8_t minute, uint8_t hour, uint8_t date, uint8_t month, uint16_t year) {
	if (this->setTime(second, minute, hour)) return this->getStatus(); // Returns with some errors during the configuration.
	if (this->setCalendar(date, month, year)) return this->getStatus(); // Returns with some errors during the configuration.
	return this->getStatus();
}

/*!
 * @brief Set the specified time to the RTC.
 * @param second Seconds to be set.
 * @param minute Minutes to be set.
 * @param hour Hours to be set.
 * @return status of the RTC. RTC_OK if successful.
 */
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

/*!
 * @brief Set the specified date to the RTC.
 * @param date Day to be set.
 * @param month Month to be set.
 * @param year Year to be set.
 * @return status of the RTC. RTC_OK if successful.
 */
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

	if (year < MIN_YEAR || year > MAX_YEAR) {
		this->m_statusRTC = RTC_YEAR_INVALID;
		return this->getStatus(); // Returns with some errors during the configuration.
	} else auxiliarYear |= (uint8_t)((((year % 100) / BCD_FACTOR) << BCD_SHIFT) | ((year % 100) % BCD_FACTOR));

	this->transmitCalendar(auxiliarDate, auxiliarMonth, auxiliarYear);
	return this->getStatus();
}

/*!
 * @brief Change the hours mode of the RTC (24-hour or 12-hour).
 * @param hoursMode Mode to set the RTC into.
 */
void DS3231::changeHoursMode(hoursMode_t hoursMode) {
	this->m_hoursMode = hoursMode;
	this->m_flagPM = false;
}

/*!
 * @brief Get a formatted string with current time and date.
 * @return Formatted string with current time and date.
 */
char* DS3231::printTimestamp(void) {
	static char RTCstr[RTC_STR_SIZE];
    RTCstr[0] = this->m_RTC.TIME.HOUR / 10 + '0'; RTCstr[1] = this->m_RTC.TIME.HOUR % 10 + '0'; RTCstr[2] = ':';
    RTCstr[3] = this->m_RTC.TIME.MIN / 10 + '0'; RTCstr[4] = this->m_RTC.TIME.MIN % 10 + '0'; RTCstr[5] = ':';
    RTCstr[6] = this->m_RTC.TIME.SEC / 10 + '0'; RTCstr[7] = this->m_RTC.TIME.SEC % 10 + '0'; RTCstr[8] = ' ';
    RTCstr[9] = this->m_RTC.CALENDAR.DATE / 10 + '0'; RTCstr[10] = this->m_RTC.CALENDAR.DATE % 10 + '0'; RTCstr[11] = '/';
    RTCstr[12] = this->m_RTC.CALENDAR.MONTH / 10 + '0'; RTCstr[13] = this->m_RTC.CALENDAR.MONTH % 10 + '0'; RTCstr[14] = '/';
    RTCstr[15] = this->m_RTC.CALENDAR.YEAR / 1000 + '0'; RTCstr[16] = (this->m_RTC.CALENDAR.YEAR / 100) % 10 + '0';
    RTCstr[17] = (this->m_RTC.CALENDAR.YEAR / 10) % 10 + '0'; RTCstr[18] = this->m_RTC.CALENDAR.YEAR % 10 + '0'; RTCstr[19] = '\n'; RTCstr[20] = '\0';
	return RTCstr;
}

/////////////////////////////
/// DS3231 initialization ///
/////////////////////////////

void initDS3231(void) {
	#if defined(I2C0_PINS)

	static DS3231 ds3231(I2C0_SCL, I2C0_SDA, I2C::TWI0);
	ds3231.set(0, 0, 0, 1, 1, 2000);

	g_ds3231 = &ds3231;

	#endif // defined(I2C0_PINS)
}
