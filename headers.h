#ifndef HEADERS_H
#define HEADERS_H

#define HAVE_REMOTE
#include <pcap.h>

#define MAX_BUFFER_SIZE 1024 //Eap头中可携带的最大字节数

//LinkerLayer Header
typedef struct EthernetHeader_
{
  u_int8_t dst[6];  //目的Mac地址
  u_int8_t src[6];  //源Mac地址
  u_int16_t type;   //协议类型
}EthernetHeader;    //长度14字节

//EapolHeader中的type字段
typedef enum EapolTypes_{Eap_Packet,Eapol_Start,Eapol_Logoff}EapolType;
//EAPOL Header http://blog.csdn.net/printfmylife20140214/article/details/19419159
typedef struct EapolHeader_{
    u_int8_t version;       //Eapol版本
    u_int8_t type;          //EAPOL数据帧类型
    u_int16_t length;       //表示数据长度，也就是“Packet Body”字段的长度，单位为字节。如果为0，则表示没有后面的数据域。
}EapolHeader;               //长度4字节

//EapHeader中Code字段
typedef enum EapCodes_{Request=1,Response,Success,Failure}EapCode;
//EapHeader
typedef struct EapHeader_{
    u_int8_t code;          //一个字节，指明EAP包的类型
    u_int8_t identifier;    //用于应答报文和请求报文之间进行匹配
    u_int16_t length;       //两个字节，EAP包的长度，包含Code、Identifier、Length和Data域，单位为字节
}EapHeader;                 //长度4字节

//Data中的Type字段
typedef enum DataTypes_{Identifier=1,Notification,ResponseOnly,
                        MD5Challenge,OneTimePassword,GenericTokenCard,
                       ExpandedTypes=254,ExperimentalUse=255}DataType;
typedef struct Data_{                   //零个或多个字节，EAP包的内容，由Code类型决定
    u_int8_t type;                      //一个字节，标识EAP的认证类型。
    u_int8_t buffer[MAX_BUFFER_SIZE];   //存数据的位置
}Data;
//
typedef struct{
    EthernetHeader ethernetHeader;
    EapolHeader eapolHeader;
    EapHeader eapHeader;
    Data data;
}EapolPacket;
#endif // HEADERS_H
