/*!
 * @file DS3231.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 18:55:36
 * @brief Real Time Clock (RTC) DS3231 Class API (Application Programming Interface).
 */

#ifndef DS3231_H_
#define DS3231_H_

// #include <stdio.h>
#include "I2C.h"
#include "ProgramConfig.h"

#define DS3231_ADDR_REG (0x68U)

#define DS3231_SEC_REG		(0x00U)
#define DS3231_MIN_REG		(0x01U)
#define DS3231_HOUR_REG		(0x02U)
#define DS3231_DAY_REG		(0x03U)
#define DS3231_DATE_REG		(0x04U)
#define DS3231_MTH_CENT_REG	(0x05U)
#define DS3231_YEAR_REG		(0x06U)

#define DS3231_FST_ALR_SEC_REG		(0x07U)
#define DS3231_FST_ALR_MIN_REG		(0x08U)
#define DS3231_FST_ALR_HOUR_REG		(0x09U)
#define DS3231_FST_ALR_DAY_DATE_REG	(0x0AU)

#define DS3231_SND_ALR_MIN_REG		(0x0BU)
#define DS3231_SND_ALR_HOUR_REG		(0x0CU)
#define DS3231_SND_ALR_DAY_DATE_REG	(0x0DU)

#define DS3231_12HOURS_MODE_SHIFT	(6U)
#define DS3231_12HOURS_AMPM_SHIFT	(5U)

//! @hideinitializer Binary Coded Decimal (<tt>BCD</tt>) code shift value.
#define BCD_SHIFT 4 //!< <pre><strong>Value:</strong> 4
//! @hideinitializer Binary Coded Decimal (<tt>BCD</tt>) code mask value.
#define BCD_MASK (0x0FU) //!< <pre><strong>Value:</strong> 0x0FU
//! @hideinitializer Binary Coded Decimal (<tt>BCD</tt>) multiplier and divider factor value.
#define BCD_FACTOR 10 //!< <pre><strong>Value:</strong> 10

//! @hideinitializer Maximum number allowed in the Current Second Register.
#define MAX_SEC 59 //!< <pre><strong>Value:</strong> 59
//! @hideinitializer Maximum number allowed in the Current Minute Register.
#define MAX_MIN 59 //!< <pre><strong>Value:</strong> 59
//! @hideinitializer Maximum number allowed in the Current Hour Register.
#define MAX_HOUR 23 //!< <pre><strong>Value:</strong> 23
//! @hideinitializer Maximum number allowed in the Current Date Register.
#define MAX_DATE 31 //!< <pre><strong>Value:</strong> 31
//! @hideinitializer Maximum number allowed in the Current Month Register.
#define MAX_MONTH 12 //!< <pre><strong>Value:</strong> 12
//! @hideinitializer Maximum number allowed in the Current Year Register.
#define MAX_YEAR 2099 //!< <pre><strong>Value:</strong> 2099

//! @hideinitializer Number of characters to store the time captured by the Real Time Clock (<tt>RTC</tt>).
#define RTC_STR_SIZE 21 //!< <pre><strong>Value:</strong> 21

//! @brief <b>RTC_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the Real Time Clock (<tt>RTC</tt>) operations can be found.
typedef enum {
	RTC_OK,				//!< Successful operation.
	RTC_UPDATE_ERR,		//!< No valid data has been acquired or transmitted via i2C communication
	RTC_TIME_INVALID,	//!< Time Register has contained invalid data since it was last programmed.
	RTC_CALR_INVALID,	//!< Calendar Register has contained invalid data since it was last programmed.
	RTC_SEC_INVALID,	//!< No valid data has been detected in Current Second Register.
	RTC_MIN_INVALID,	//!< No valid data has been detected in Current Minute Register.
	RTC_HOUR_INVALID,	//!< No valid data has been detected in Current Hour Register.
	RTC_DATE_INVALID,	//!< No valid data has been detected in Current Date Register.
	RTC_MONTH_INVALID,	//!< No valid data has been detected in Current Month Register.
	RTC_YEAR_INVALID	//!< No valid data has been detected in Current Year Register.
} RTC_result_t;

//! @brief <b>RTC_time_st</b> structure has the units of time that make up the current date.
typedef struct {
	uint8_t SEC : 6; //!< Stores <tt>6</tt> bit fields of the current measurement expressed in seconds.
	uint8_t MIN : 6; //!< Stores <tt>6</tt> bit fields of the current measurement expressed in minutes.
	uint8_t HOUR : 7; //!< Stores <tt>7</tt> bit fields of the current measurement expressed in hours.
} RTC_time_st;

//! @brief <b>RTC_calendar_st</b> structure has the units of time that make up the current calendar without the precision of the hours.
typedef struct {
	uint8_t DATE : 6; //!< Stores <tt>6</tt> bit fields of the current measurement expressed in days of the month.
	uint8_t MONTH : 5; //!< Stores <tt>5</tt> bit fields of the current measurement expressed in months.
	uint16_t YEAR : 13;	//!< Stores <tt>13</tt> bit fields of the current measurement expressed in years.
} RTC_calendar_st;

//! @brief <b>RTC_st</b> structure has all the attributes that make up the current date with precision.
typedef struct {
	RTC_time_st TIME; //!< Current date stored in the Time Register.
	RTC_calendar_st CALENDAR; //!< Current calendar stored in the Calendar Register.
} RTC_st;

// TODO (Mayor priority): Analyze the use of the "print" method along with its implementation including the <stdio.h> header.
// It may be wise to create a private method with the same functionality as "sprintf" standard function.
// Print a different message depending on the hours mode used.
// TODO (Minor priority): Inherit from CALLBACK and update a global variable every one second.
// If communication with I2C is not achieved, its variables must be reinitialized.

class DS3231 : protected I2C {
public:
	// TWENTY_FOUR_HOURS_MODE: Indicates that the clock time is set to 24-hour mode.
	// TWELVE_HOURS_MODE: Indicates that the clock time is set to 12-hour mode.
	enum hoursMode_t { TWENTY_FOUR_HOURS_MODE, TWELVE_HOURS_MODE };
private:
	RTC_st m_RTC;
	RTC_result_t m_statusRTC;
	hoursMode_t m_hoursMode;
	bool m_flagPM;

	// Acquire or transmit current time values.
	statusComm_t acquireTime(void);
	statusComm_t transmitTime(uint8_t second, uint8_t minute, uint8_t hour);
	// Acquire or transmit current calendar values.
	statusComm_t acquireCalendar(void);
	statusComm_t transmitCalendar(uint8_t date, uint8_t month, uint8_t year);
public:
	DS3231();

	RTC_st get(void);
	RTC_time_st getTime(void);
	RTC_calendar_st getCalendar(void);
	RTC_result_t getStatus(void) const;

	RTC_result_t set(uint8_t second, uint8_t minute, uint8_t hour, uint8_t date, uint8_t month, uint16_t year);
	RTC_result_t setTime(uint8_t second, uint8_t minute, uint8_t hour);
	RTC_result_t setCalendar(uint8_t date, uint8_t month, uint16_t year);

	void changeHoursMode(hoursMode_t hoursMode);
//	char *print(char RTCstr[RTC_STR_SIZE]);
	virtual ~DS3231();
};

extern DS3231 *g_ds3231;

void initDS3231(void);

#endif /* DS3231_H_ */
