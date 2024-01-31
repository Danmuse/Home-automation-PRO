/*!
 * @file IoTConnection.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/01/2024 12:23:56
 * @brief IoT Connection Abstract Class API (Application Programming Interface).
 */

#ifndef IOT_CONNECTION_H_
#define IOT_CONNECTION_H_

#include "utils.h"
#include "IoTListener.h"
#include "IoTVariable.h"

class IoTConnection {
public:
	IoTConnection() = default;
	virtual void receiveMessage() = 0; //TODO: This returned char*, maybe I'll need it
	virtual void uploadVariable(IoTVariable_st variable) = 0;
	virtual void suscribeListener(IoTListener *listener) = 0;
	virtual ~IoTConnection() = default;
};

#endif // IOT_CONNECTION_H_
