#ifndef STRUCTDEF_H
#define STRUCTDEF_H

#include <QByteArray>
#include <QtEndian>
#include <QDebug>
#include <QDateTime>

#include <enumdef.h>
#pragma pack(1)

typedef struct _FRAME_OPERTATION_MODE
{
//    [01] FModeB1
//    [10] FModeB2
//    [11] FMode(B4)
    quint16 usMF:2; // 16~17
//    [01] reserved
//    [10] Reduction by 4
//    [11] Reduction by 16
    quint16 usRM:2; // 18~19
//    [000] beta
//    [001] zeta
//    [100] gamma
//    [101] delta
//    [111] iota
    quint16 usSY:3; // 20~22
    quint16 reserved:2;// 23/24
//    [0] TimeSync unlocked
//    [1] TimeSync locked
    quint16 usTS:1; // 25
//    [0] standard sample scheme
//    [1] optional sample scheme
    quint16 usSM:1; // 26
// reserved
    quint16 usDM:1; // 27
// reserved
    quint16 usABCD:4; // 28~31
    void show()
    {
        QString strMsg;
        if(usMF == 1) strMsg = "MF = 0(FData)";
        if(usMF == 1) strMsg = "MF = 1(FModeB1)";
        if(usMF == 2) strMsg = "MF = 2(FModeB2)";
        if(usMF == 3) strMsg = "MF = 3(FModeB4)";
        qDebug() << strMsg;

        if(usRM == 0) strMsg = "RM = 0(no Reduction)";
        if(usRM == 1) strMsg = "RM = 1(Reserved)";
        if(usRM == 2) strMsg = "RM = 2(Reduction by 4)";
        if(usRM == 3) strMsg = "RM = 3(Reduction by 6)";
        qDebug() << strMsg;


        if(usSY == 0) strMsg = "SY = 0(beta)";
        if(usSY == 1) strMsg = "SY = 1(zata)";
        if(usSY == 4) strMsg = "SY = 4(gamma)";
        if(usSY == 5) strMsg = "SY = 5(delta)";
        if(usSY == 7) strMsg = "SY = 7(iota)";
        qDebug() << strMsg;

        if(usTS == 0) strMsg = "TS = 0(TimeSync unlocked)";
        if(usTS == 1) strMsg = "TS = 1(TimeSync locked)";
        qDebug() << strMsg;

        if(usSM == 0) strMsg = "SM = 0(TimeSync unlocked)";
        if(usSM == 1) strMsg = "SM = 1(TimeSync locked)";
        qDebug() << strMsg;

    }
}stOperationMode;

typedef struct _FRAME_SIZE_TYPE
{
    quint16 usFrameSize:10;
    quint16 usDataFrameType:6;
    stOperationMode stOperatorMode;
    void show()
    {
        QString strMsg;
        qDebug() << "FrameSize = " << usFrameSize;

        if(usDataFrameType == 4) strMsg = "DataFrameType = 4(Proto0 No FModeB No Data)";
        if(usDataFrameType == 5) strMsg = "DataFrameType = 5(Proto0 FMode B1)";
        if(usDataFrameType == 6) strMsg = "DataFrameType = 6(Proto0 FMode B2)";
        if(usDataFrameType == 7) strMsg = "DataFrameType = 7(Proto0 FMode B3)";
        if(usDataFrameType == 8) strMsg = "DataFrameType = 8(Type8)";
        qDebug() << strMsg;

        stOperatorMode.show();
    }
}stFrameSizeType;

typedef struct _FRAME_STREAM_ID
{
    quint16 usFrameID;
    quint16 usDataStreamID;
    void show()
    {
        QString strTemp = QString("DataStreamID = 0x%1").arg(usDataStreamID,4,16,QChar('0'));
        qDebug() << strTemp;

        strTemp = QString("FrameID = 0x%1").arg(usFrameID,4,16,QChar('0'));
        qDebug() << strTemp;

    }
}stFrameStreamID;

typedef struct _TC_CONTEXT_DESCRIPTOR
{
    quint8 cTmptModule:2; //temperature Module 0/1/2/3;
    quint8 cModeData  :2; // 0:Raw 1:calibrated 2:invalid 3:compensated
    quint8 cRcv       :2; // RX0/1/2/3;
    quint8 cTorC      :1; // temperature or control;
    quint8 cEthCh     :1; // ethernet channle 0 / 1;
    void show()
    {
        QString str;
        qDebug() << "Temperature Module = " << cTmptModule;
        if(cModeData == 0) str = "DataType = 0(Raw)";
        if(cModeData == 1) str = "DataType = 1(calibrated)";
        if(cModeData == 2) str = "DataType = 2(invalid)";
        if(cModeData == 3) str = "DataType = 3(compensated)";
        qDebug() << str;
        qDebug() << "Ethernet Channel = " << cRcv;
        qDebug() << "Temperature(0) or Control(1) = " << cTorC;
        qDebug() << "ethernet channle 0 / 1 = " << cEthCh;
    }
} stTCContextTM0;

typedef struct _TC_CONTEXT_DESCRIPTOR_TYPEC
{
    quint8 cBlockAddr:3;
    quint8 reserved  :2;
    quint8 cRcv       :2; // RXa/b/c/d;
    quint8 cEthCh     :1; // ethernet channle 0 / 1;
    void show()
    {
        qDebug() << "Block address = " << cBlockAddr;
        qDebug() << "RX = " << cRcv;
        qDebug() << "ethernet channle 0 / 1= " << cEthCh;
    }

} stTCContextTypeC;

typedef union _STTCCONTEXTDESC
{
    stTCContextTM0 tm0;
    stTCContextTypeC typeC;
}stTCContextDesc;

typedef struct _FRAMETCDATA
{
    quint16 usTCOffset:12;
    quint16 usTCFormat:4;
    quint8  ucTCSize;
    stTCContextDesc stTCDesc;
    void show()
    {
        qDebug() << "TCOffset = " << usTCOffset << " Bytes";
        if(usTCFormat == 0xA)
        {
            qDebug() << "TCFormat = 0xA";
            stTCDesc.tm0.show();
        }
        else if(usTCFormat == 0xC)
        {
            qDebug() << "TCFormat = 0xC";
            stTCDesc.typeC.show();
        }
        else
        {
            qDebug() << "Error:Invalid TCFormat.";
        }

        qDebug() << "TCSize = " << ucTCSize << " Bytes";


    }
}stFrameTC;

typedef struct _FRAMETIMESTAMPSEC
{
    quint32 uiDaySec:25; // 0~24
    quint32 uireserved1:1; // 25
    quint32 uiYear:5; // 26~30;
    quint32 uireserved2:1; // 31
    void show()
    {
        QDateTime dt,dt2;
        dt.setTime_t(uiDaySec);
        dt2 = dt.addYears(30).addYears(uiYear);
        qDebug() << dt2.toLocalTime().toString("yyyy-MM-dd hh:mm:ss");


    }
} stFrameTimeStampSec;

typedef struct _FRAMETIMESTAMPFRACTION
{
    quint32 uiTimeStampFraction;
    void show()
    {
        qDebug() << "TimeStamp Fraction = " << uiTimeStampFraction;
    }
}stFrameTimeStampFraction;



typedef struct
{
    unsigned char ch[FOUR_CHANNLE];
}stCH4;

typedef struct
{
    stCH4 tenRoute[TEN_ROUTE];
}st10Route;

typedef struct
{
    st10Route hexFrame[HEX_FRAME];
    void show()
    {

    }
    bool decode(char *pStart, unsigned int iMax)
    {

    }
}stB1_Beta;


typedef struct
{
    unsigned char ra;
    unsigned char rb;
    unsigned char rc;
    unsigned char rd;
}stCH;

typedef union
{
    unsigned int aiSignal[5];
    unsigned char acSignal[20];
}unRevData;

typedef struct
{
    stCH Ch10[40];
    unRevData unRcv;

    void translate()
    {
        unsigned char acByte,acTemp;
        for(int i= 0; i< 20; i++)
        {
            acByte = unRcv.acSignal[i];
            acTemp = acByte & 0xF;

            Ch10[2*i].ra = ((unsigned char)acTemp & (unsigned char)0x1);
            Ch10[2*i].rb = ((unsigned char)acTemp & (unsigned char)0x2) >> 1;
            Ch10[2*i].rc = ((unsigned char)acTemp & (unsigned char)0x4) >> 2;
            Ch10[2*i].rd = ((unsigned char)acTemp & (unsigned char)0x8) >> 3;

            acTemp = acByte & 0xF0;
            Ch10[2*i+1].ra = ((unsigned char)acTemp & (unsigned char)0x10) >> 4;
            Ch10[2*i+1].rb = ((unsigned char)acTemp & (unsigned char)0x20) >> 5;
            Ch10[2*i+1].rc = ((unsigned char)acTemp & (unsigned char)0x40) >> 6;
            Ch10[2*i+1].rd = ((unsigned char)acTemp & (unsigned char)0x80) >> 7;
        }
    }
    void show()
    {
        QString str;
        for(int i = 0; i < 20; i++)
        {
            str += QString("%1 ").arg((uint)unRcv.acSignal[i],2,16,QChar('0'));
        }

        qDebug() << str;
    }
}stB4;

typedef struct
{
    stB4 stRcv4[4]; // s0,s1,s2,s3;
    bool decode(char **pData,int &iLength)
    {
        unsigned int iTemp = 0;
        //20 bytes
        char *p = *pData;
        for(int i= 0; i < 4; i++)
        {
            for(int j=0; j< 5; j++)
            {
                memcpy(&iTemp,(*pData),4);
                stRcv4[i].unRcv.aiSignal[j] = qFromLittleEndian(iTemp);
                *pData += 4;
                iLength += 4;
            }
            stRcv4[i].translate();
            stRcv4[i].show();

        }
        return true;
    }
}stB4_Gamma;


typedef struct _STFDATA
{
//    union
//    {
//    //unsigned int uiBuffer[20];
//    //unsigned short usBuffer[40];
//    stB1_Beta B1_Beta;
    stB4_Gamma B4_Gamma[16]; // L0.0~L3.3
//    }unData;
    _STFDATA()
    {
        memset(&B4_Gamma,0x00,sizeof(stB4_Gamma)*16);
    }
    void show()
    {

    }
    bool decode(char **pData,int & iLength)
    {
        char *pPos = *pData;
        for(int i= 0; i < 16; i++)
        {
            // size = 80bytes
            if(!B4_Gamma[i].decode(pData, iLength))
            {
                return false;
            }
        }
        return true;
    }


}stFData;

typedef struct _FRAME_HEAD
{
    stFrameSizeType stSizeType;
    stFrameStreamID stFrameID;
    stFrameTimeStampSec stTimeStampSeconds;
    stFrameTimeStampFraction stFrameTimeStampFract;
    stFrameTC stTCFrame;
    _FRAME_HEAD()
    {

    }
    void show()
    {
        stSizeType.show();
        stFrameID.show();
        stTimeStampSeconds.show();
        stFrameTimeStampFract.show();
        stTCFrame.show();
    }
    bool decode(char **pData, int &iLength)
    {
//        quint16 usTemp = 0;
        quint32 uiTemp = 0;
        quint32 uiTemp2 = 0;
        iLength = 0;
//        char &pStart = ;

//        if(iSize < 20)
//        {
//            return NULL;
//        }
        //DWORD0
        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        memcpy(&stSizeType,&uiTemp,4);
//        if(stSizeType.usFrameSize*2 != iSize)
//        {
//            qDebug() << "DataSize ERROR." ;
//            return NULL;
//        }

        if(stSizeType.usDataFrameType != DFTModeType8)
        {
            qDebug() << "DataFrameType ERROR." ;
            return false;
        }

        // dword1
        memcpy(&uiTemp,(*pData),4);
        uiTemp2 = qFromLittleEndian(uiTemp);
        memcpy(&stFrameID,&uiTemp2,4);
        (*pData) += 4;
        iLength +=4;

        qint64 mil;
        // dword2
        memcpy(&uiTemp,(*pData),4);
        uiTemp2 = qFromLittleEndian(uiTemp);
        memcpy(&stTimeStampSeconds,&uiTemp2,4);
        (*pData) += 4;
        iLength +=4;
        mil = stTimeStampSeconds.uiDaySec ;
        // dword3
        memcpy(&uiTemp,(*pData),4);
        uiTemp2 = qFromLittleEndian(uiTemp);
        memcpy(&stFrameTimeStampFract,&uiTemp2,4);
        (*pData) += 4;
        iLength +=4;
//        mil = mil << 32;
//        mil += stFrameTimeStampFract.uiTimeStampFraction;
//        QDateTime dt1,dt2;
//        dt1.setTime_t(stTimeStampSeconds.uiDaySec);
//        qDebug() << "1:" << dt1.toLocalTime().toString("yyyy-MM-dd hh:mm:ss");
        //dt2.setMSecsSinceEpoch(mil);
        //qDebug() << "2:" <<dt2.toLocalTime().toString("yyyy-MM-dd hh:mm:ss");

        // dword4
        memcpy(&uiTemp,(*pData),4);
        uiTemp2 = qFromLittleEndian(uiTemp);
        memcpy(&stTCFrame,&uiTemp2,4);
        (*pData) += 4;
        iLength +=4;
        show();
        return true;

    }

}stFrameHead;

typedef struct _TCDATA_MODEA
{
    quint16 sTCh0;
    quint16 sTCh1;
    quint16 sTCh2;
    quint16 sTCh3;
    quint16 sTCh4;
    quint16 sTCh5;
    quint16 sTCh6;
    quint16 sTCh7;
    quint16 sTCh8;
    quint16 sTCh9;
    quint16 sNullTC;
    quint16 sRef;
    quint16 sRTDi;
    quint16 sRTDx;
    quint16 sNullRTDi;
    quint16 sNullRTDx;
    void show()
    {
        QString str;
        qDebug() << "sTCh0 = "     <<   str.setNum(sTCh0,16);
        qDebug() << "sTCh1 = "     <<   str.setNum(sTCh1,16);
        qDebug() << "sTCh2 = "     <<   str.setNum(sTCh2,16);
        qDebug() << "sTCh3 = "     <<   str.setNum(sTCh3,16);
        qDebug() << "sTCh4 = "     <<   str.setNum(sTCh4,16);
        qDebug() << "sTCh5 = "     <<   str.setNum(sTCh5,16);
        qDebug() << "sTCh6 = "     <<   str.setNum(sTCh6,16);
        qDebug() << "sTCh7 = "     <<   str.setNum(sTCh7,16);
        qDebug() << "sTCh8 = "     <<   str.setNum(sTCh8,16);
        qDebug() << "sTCh9 = "     <<   str.setNum(sTCh9,16);
        qDebug() << "sNullTC = "   <<   str.setNum(sNullTC,16);
        qDebug() << "sRef = "      <<   str.setNum(sRef,16);
        qDebug() << "sRTDi = "     <<   str.setNum(sRTDi,16);
        qDebug() << "sRTDx = "     <<   str.setNum(sRTDx,16);
        qDebug() << "sNullRTDi = " <<   str.setNum(sNullRTDi,16);
        qDebug() << "sNullRTDx = " <<   str.setNum(sNullRTDx,16);
    }
    bool decode(char **pData, int &iLength)
    {
        quint32 uiTemp = 0;

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sTCh0 = (quint16)(uiTemp & 0xFFFF);
        sTCh1 = (quint16)((uiTemp & 0xFFFF0000) >> 16);

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sTCh2 = (quint16)(uiTemp & 0xFFFF);
        sTCh3 = (quint16)((uiTemp & 0xFFFF0000) >> 16);

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sTCh4 = (quint16)(uiTemp & 0xFFFF);
        sTCh5 = (quint16)((uiTemp & 0xFFFF0000) >> 16);

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sTCh6 = (quint16)(uiTemp & 0xFFFF);
        sTCh7 = (quint16)((uiTemp & 0xFFFF0000) >> 16);

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sTCh8 = (quint16)(uiTemp & 0xFFFF);
        sTCh9 = (quint16)((uiTemp & 0xFFFF0000) >> 16);

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sNullTC = (quint16)(uiTemp & 0xFFFF);
        sRef =    (quint16)((uiTemp & 0xFFFF0000) >> 16);

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sRTDi = (quint16)(uiTemp & 0xFFFF);
        sRTDx = (quint16)((uiTemp & 0xFFFF0000) >> 16);

        memcpy(&uiTemp,(*pData),4);
        (*pData) += 4;
        iLength +=4;
        uiTemp = qFromLittleEndian(uiTemp);
        sNullRTDi = (quint16)(uiTemp & 0xFFFF);
        sNullRTDx = (quint16)((uiTemp & 0xFFFF0000) >> 16);
        return true;
    }
}stModeA;

typedef struct _TCDATA_MODEC
{
    quint16 data[16];
    void show()
    {
        QString str;
        for(int i = 0; i < 16; i++)
        {
            qDebug() << QString("Block[%1] = %2").arg(i+1).arg(data[i]);
        }
    }
    bool decode(char **pData, int &iLength)
    {
        memset(&data,0x00,sizeof(quint16)*16);
        quint32 uiTemp = 0;
        for(int i=0; i < 8; i++)
        {
            memcpy(&uiTemp,(*pData),4);
            (*pData) += 4;
            iLength +=4;
            uiTemp = qFromLittleEndian(uiTemp);
            data[2*i] =   (quint16)(uiTemp & 0xFFFF);
            data[2*i+1] = (quint16)((uiTemp & 0xFFFF0000) >> 16);
        }
        return true;

    }
}stModeC;
typedef union _UNTCDATA
{
    stModeA modeA;
    stModeC modeC;
}unTCData;

typedef struct _NFRAME_DATA
{
    stFrameHead   stMsgHead;
    stFData       uiFdata;
    unTCData      uTCData;
    void show()
    {

    }
    bool decode(QByteArray info)
    {
        int iLength = 0;
        if(info.size() != INFRAME_LENGTH)
        {

            return false;
        }
        char *pPos = info.data();
        QString str = QString("*******************Start Postion = 0x%1").arg((quint64)pPos,8,16,QChar('0'));
        qDebug() << str;
        // decode head
        if(!stMsgHead.decode(&pPos,iLength))
        {
            qDebug() << "decode heard faild.";
            return false;
        }
        str = QString("*******************Start Postion = 0x%1").arg((quint64)pPos,8,16,QChar('0'));
        qDebug() << str;
        iLength = 0;
        // decode data
        if(stMsgHead.stSizeType.stOperatorMode.usMF == FMTnoFDATA )
        {
            qDebug() << "Invalid Frame Moudle.";
            return false;
        }
        else if(stMsgHead.stSizeType.stOperatorMode.usMF == FMTModeB1 )
        {

        }
        else if(stMsgHead.stSizeType.stOperatorMode.usMF == FMTModeB2 )
        {
        }
        else if(stMsgHead.stSizeType.stOperatorMode.usMF == FMTModeB4 )
        {
            if(stMsgHead.stSizeType.stOperatorMode.usSY == gamma)
            {
                if(!uiFdata.decode(&pPos,iLength))
                {
                    return false;
                }
            }
        }
        else
        {
            qDebug() <<"MF is invalid.";
            return false;
        }


        // decode TC
        if(stMsgHead.stTCFrame.ucTCSize != 0x20)
        {
            qDebug() << "TC Size = " << stMsgHead.stTCFrame.ucTCSize;
            return false;
        }

        if(stMsgHead.stTCFrame.usTCOffset != 0x500)
        {
            qDebug() << "TC offset = " << stMsgHead.stTCFrame.usTCOffset;
            return false;
        }
        if(stMsgHead.stTCFrame.usTCFormat == 0xA)
        {
            uTCData.modeA.decode(&pPos, iLength);
            uTCData.modeA.show();
        }
        else if(stMsgHead.stTCFrame.usTCFormat == 0xC)
        {
            uTCData.modeC.decode(&pPos, iLength);
            uTCData.modeC.show();
        }
        else
        {
            return false;
        }

        return true;
    }
}stNFMsg;
#pragma pack()
#endif // STRUCTDEF_H
