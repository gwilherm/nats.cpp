#ifndef NATS_CLIENT_IMPL
#define NATS_CLIENT_IMPL

#include "NatsClient.h"
#include <nats/nats.h>

class NatsClientImpl : public NatsClient
{
public:
    NatsClientImpl(const bool statistics = false);
    virtual ~NatsClientImpl() = default;

    virtual bool start();
    virtual void stop();

    virtual bool subscribe(const std::string subject);

protected:
    bool initialize();
    static void asyncErrCb(::natsConnection *nc, ::natsSubscription *sub, ::natsStatus err, void *closure);

private:
    bool m_statistics;
    static int64_t m_dropped;

    ::natsConnection   *m_pConn;
    ::natsOptions      *m_pOpts;
    ::natsSubscription *m_pSub;
    ::natsStatistics   *m_pStats;
    ::natsMsg          *m_pMsg;
};

#endif // NATS_CLIENT_IMPL