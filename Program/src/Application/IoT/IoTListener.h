/*!
 * @file IoTListener.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/01/2024 12:23:56
 * @brief IoT Listener Abstract Class API (Application Programming Interface).
 */

#ifndef IOT_LISTENER_H_
#define IOT_LISTENER_H_

#include "utils.h"

class IoTListener {
public:
	IoTListener() = default;
    virtual bool initializeConnection()=0;
	virtual void processIoTMessage(char* message) = 0;
	virtual ~IoTListener() = default;
};

#endif // IOT_LISTENER_H_
