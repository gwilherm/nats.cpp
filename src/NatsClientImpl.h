#ifndef NATS_CLIENT_IMPL
#define NATS_CLIENT_IMPL

#include "NatsClient.h"

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

    virtual bool subscribe(const std::string subject, std::function<void(void)> callback);

    void onMsg(const std::string subject, const char* msg, const size_t msg_len);

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

    std::unordered_map<std::string, std::function<void(void)>> m_Callbacks;
};

#endif // NATS_CLIENT_IMPL