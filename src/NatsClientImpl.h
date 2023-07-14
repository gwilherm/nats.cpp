#ifndef NATS_CLIENT_IMPL
#define NATS_CLIENT_IMPL

#include "NatsClient.h"
#include "NatsSubscription.h"

#include <string>
#include <nats/nats.h>
#include <unordered_map>

class NatsClientImpl : public NatsClient
{
public:
    NatsClientImpl(const bool statistics = false);
    virtual ~NatsClientImpl() = default;

    virtual bool start();
    virtual void stop();

    virtual bool subscribe(const std::string subject, std::function<void(std::string)> callback);

protected:
    static void asyncErrCb(::natsConnection *nc, ::natsSubscription *sub, ::natsStatus err, void *closure);

private:
    bool m_statistics;
    static int64_t m_dropped;

    ::natsConnection   *m_pConn;
    ::natsOptions      *m_pOpts;
    ::natsStatistics   *m_pStats;
    ::natsMsg          *m_pMsg;

    std::unordered_map<std::string, NatsSubscription*> m_Subscriptions;
};

#endif // NATS_CLIENT_IMPL