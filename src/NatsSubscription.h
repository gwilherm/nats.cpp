#ifndef NATS_SUBSCRIPTION_H
#define NATS_SUBSCRIPTION_H

#include <nats/nats.h>
#include <string>
#include <functional>

class NatsSubscription
{
public:
    NatsSubscription(::natsConnection* pConn, std::string subject, std::function<void(std::string)> callback);
    virtual ~NatsSubscription();

public:
    std::function<void(std::string)> m_callback;
private:
    ::natsSubscription* m_pSub;
};
#endif // NATS_SUBSCRIPTION_H