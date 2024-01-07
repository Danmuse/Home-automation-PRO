//
// Created by Usuario on 05/01/2024.
//

#ifndef IOTVARIABLE_H
#define IOTVARIABLE_H


struct IotVariable {
    char *name;
    //TODO: MAYBE NOT INTEGER
    int &variable;
    int uploadPeriod;
    int uploadCounter;
};


#endif //IOTVARIABLE_H
