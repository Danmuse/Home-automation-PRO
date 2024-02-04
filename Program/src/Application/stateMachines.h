/*!
 * @file stateMachines.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/08/2023 01:02:47
 * @brief State Machines API (Application Programming Interface).
 */

#ifndef STATE_MACHINES_H_
#define STATE_MACHINES_H_

#include "board.h"
#include "QTConnection.h"
#include "IoTManager.h"
#include "IoTActions.h"

#define RFID_USER_UID_SIZE 4
#define USERS_INIT_POSITION 2

enum class UserRegistrationState {
	WAITING_FOR_PASSWORD,
	WAITING_FOR_USER
};

void userRegistrationStateMachine(UserRegistrationState& state);


#endif /* STATE_MACHINES_H_ */