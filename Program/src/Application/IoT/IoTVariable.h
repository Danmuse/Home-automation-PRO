//
// Created by Usuario on 05/01/2024.
//

#ifndef IOTVARIABLE_H
#define IOTVARIABLE_H

#include "utils.h"

struct IoTVariable {
    char *name;
    //TODO: MAYBE NOT INTEGRER
    int &variable;
    int uploadPeriod;
    int uploadCounter;
};


#endif //IOTVARIABLE_H
