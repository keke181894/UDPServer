#ifndef ENUMDEF_H
#define ENUMDEF_H

const unsigned int INFRAME_LENGTH = 1332;
const unsigned int FOUR_CHANNLE = 4;
const unsigned int TEN_ROUTE = 10;
const unsigned int HEX_FRAME = 16;
typedef enum
{
    DFTModeB = 4,
    DFTModeB1 = 5,
    DFTModeB2 = 6,
    DFTModeB4 = 7,
    DFTModeType8 = 8
}EDataFrameType;

typedef enum
{
    FMTnoFDATA = 0,
    FMTModeB1 = 1,
    FMTModeB2 = 2,
    FMTModeB4 = 3
}EFrameType;

typedef enum
{
    beta = 0,
    zeta = 1,
    gamma = 4,
    delta = 5,
    iota = 7
}ESYType;

#endif // ENUMDEF_H
