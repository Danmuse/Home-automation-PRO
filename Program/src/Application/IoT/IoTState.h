#ifndef IOT_STATE_H
#define IOT_STATE_H

struct IoTState {
        const char* name;
        void* data;
        std::vector<const char*> stringsRepr;
};


#endif //IOT_STATE_H
