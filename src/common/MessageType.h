//
// Created by Noy on 2023/12/23.
//

#ifndef LAB6SOCKET_MESSAGETYPE_H
#define LAB6SOCKET_MESSAGETYPE_H


enum class MessageType : char {
    UNKNOWN,
    HEARTBEAT,
    CONNECT,
    DISCONNECT,
    SEND_MESSAGE,
    GET,
};


#endif //LAB6SOCKET_MESSAGETYPE_H
