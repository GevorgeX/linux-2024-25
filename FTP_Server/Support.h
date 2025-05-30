#ifndef SUPPORT_H
#define SUPPORT_H

constexpr uint32_t BUFFER_SIZE = 4096;

enum Command: uint32_t {
    LIST = 1 ,
    RETR,
    STOR ,
    PWD ,
    CWD ,
    QUIT ,
    MKD ,
    RMD ,
    DELE
};

uint32_t OK = 1;
uint32_t ERROR = 2;

#endif //SUPPORT_H
