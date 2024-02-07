/*!
 * @file stateMachines.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/02/2024 17:06:04
 * @brief State Machines API (Application Programming Interface).
 */

#ifndef STATE_MACHINES_H_
#define STATE_MACHINES_H_

#include "board.h"
#include "KeyboardPassword.h"

#define RFID_USER_UID_SIZE 4
#define USERS_INIT_POSITION 2

enum class UserRegistrationState {
        WAITING_FOR_PASSWORD,
        WAITING_FOR_USER
};

enum class DoorOpeningState {
        WAITING_FOR_RFID,
        CHECKING_USER,
        DOOR_OPEN
};

void userRegistrationStateMachine(UserRegistrationState& state);

void doorOpeningStateMachine(DoorOpeningState& state);


#endif /* STATE_MACHINES_H_ */
