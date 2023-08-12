/*
 * PwmManager.h
 *
 *  Created on: Aug 12, 2023
 *      Author: Agusin Ordoñez
 */

#ifndef DRIVERS_07_PWM_PWMMANAGER_H_
#define DRIVERS_07_PWM_PWMMANAGER_H_

#include "LPC845.h"
#include <vector>
#include <array>
#include "PWM.h"

class PwmManager {//Should be singleton, but im lazy
    protected:
        const uint8_t MAX_PWM = 7;
        static uint8_t m_pwmNumber;
        static uint32_t m_frequency;
    public:
        PwmManager();
        PwmManager(uint32_t frequency);
        PWM createPwm(const Gpio &outputPort, float duty);
        void initPwm()const;
};

#endif /* DRIVERS_07_PWM_PWMMANAGER_H_ */
