#include "NatsSubscription.h"

#include <nats/status.h>
#include <iostream>

static void _onMsg(::natsConnection *nc, ::natsSubscription *m_pSub, ::natsMsg *msg, void *closure)
{
    std::cout << "Received msg: "
                << ::natsMsg_GetSubject(msg) << " - "
                << ::natsMsg_GetData(msg)
                << std::endl;

    NatsSubscription* _this = (NatsSubscription *)closure;

    std::string subject = ::natsMsg_GetSubject(msg);
    std::string message(reinterpret_cast<const char*>(::natsMsg_GetData(msg)), ::natsMsg_GetDataLength(msg));
    
    if (_this && _this->m_callback)
        _this->m_callback(message);

    ::natsMsg_Destroy(msg);
}


NatsSubscription::NatsSubscription(::natsConnection* pConn, std::string subject, std::function<void(std::string)> callback) :
    m_callback(callback)
{
    ::natsStatus s;

    s = ::natsConnection_Subscribe(&m_pSub, pConn, subject.c_str(), _onMsg, this);
    if (s != NATS_OK)
        std::cerr << "*Error: " << s << std::endl;

    // For maximum performance, set no limit on the number of pending messages.
    if (s == NATS_OK)
        s = ::natsSubscription_SetPendingLimits(m_pSub, -1, -1);

    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;
    // if (s == NATS_OK)
    //     s = ::natsSubscription_AutoUnsubscribe(m_pSub, (int) total);
}

NatsSubscription::~NatsSubscription()
{
    if (m_pSub)
    {
        ::natsSubscription_Unsubscribe(m_pSub);
        ::natsSubscription_Destroy(m_pSub);
    }
}