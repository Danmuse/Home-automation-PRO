#ifndef SERVODOOR_H
#define SERVODOOR_H

#include "SG90S.h"

#define OPEN_POS 0 //Could use enumn and also open/close with set(OPEN) or smth, but ugly API
#define CLOSE_POS 90

class ServoDoor {
    private:
        SG90S m_servo;
        int m_currentPos;

    public:
        ServoDoor(Gpio &controlPin);

        void open();
        void close();
        void toggle();

        ~ServoDoor()=default;

};


#endif //SERVODOOR_H
