//
// Created by Usuario on 28/01/2024.
//

#include "ServoDoor.h"

ServoDoor::ServoDoor(Gpio& controlPin) : m_servo(controlPin), m_currentPos(CLOSE_POS) {
    m_servo.setAngle(m_currentPos);
}

void ServoDoor::open() {
    m_currentPos = OPEN_POS;
    m_servo.setAngle(m_currentPos);
}

void ServoDoor::close() {
    m_currentPos = CLOSE_POS;
    m_servo.setAngle(m_currentPos);
}

void ServoDoor::toggle() {
    if (m_currentPos == OPEN_POS) {
        close();
    } else {
        open();
    }
}
