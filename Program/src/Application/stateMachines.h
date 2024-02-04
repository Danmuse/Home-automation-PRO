/*!
 * @file stateMachines.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/08/2023 01:02:47
 * @brief Semaphore API (Application Programming Interface).
 */

#ifndef STATE_MACHINES_H_
#define STATE_MACHINES_H_

#include "ProgramConfig.h"
#include "Timers.h"
#include "board.h"

enum class UserRegistrationState {
        WAITING_FOR_PASSWORD,
        WAITING_FOR_USER,
};

void userRegistrationStateMachine(UserRegistrationState& state);


#endif /* STATE_MACHINES_H_ */
