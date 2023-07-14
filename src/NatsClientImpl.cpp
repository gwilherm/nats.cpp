#include "NatsClientImpl.h"
#include "NatsSubscription.h"

#include <iostream>
#include <nats/status.h>

int64_t NatsClientImpl::m_dropped = 0;

NatsClientImpl::NatsClientImpl(const bool statistics) :
    m_statistics(statistics),
    m_pConn(NULL),
    m_pOpts(NULL),
    m_pStats(NULL),
    m_pMsg(NULL)
{
}

void NatsClientImpl::asyncErrCb(natsConnection *nc, natsSubscription *m_pSub, natsStatus err, void *closure)
{
    std::cerr << "Async error: " 
        << err << " - "
        << ::natsStatus_GetText(err)
        << std::endl;

    ::natsSubscription_GetDropped(m_pSub, (int64_t*) &m_dropped);
}

bool NatsClientImpl::start()
{
    std::cout << "Start" << std::endl;
    ::natsStatus s = NATS_OK;

    if (::natsOptions_Create(&m_pOpts) != NATS_OK)
        s = NATS_NO_MEMORY;

    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;

    s = ::natsOptions_SetErrorHandler(m_pOpts, asyncErrCb, this);
    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;


    s = ::natsConnection_Connect(&m_pConn, m_pOpts);

    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;
    
    if (m_statistics && (s == NATS_OK))
        s = ::natsStatistics_Create(&m_pStats);

    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;

    return (s == NATS_OK);
}

void NatsClientImpl::stop()
{
    std::cout << "Stop" << std::endl;
    // Destroy all our objects to avoid report of memory leak
    if (m_statistics)
        ::natsStatistics_Destroy(m_pStats);

    auto it = m_Subscriptions.begin();
    while (it != m_Subscriptions.end())
    {
        if (it->second != nullptr)
            delete it->second;

        it = m_Subscriptions.erase(it);
    }
    ::natsConnection_Destroy(m_pConn);
    ::natsOptions_Destroy(m_pOpts);

    // To silence reports of memory still in used with valgrind
    ::nats_Close();
}

bool NatsClientImpl::subscribe(const std::string subject, std::function<void(std::string)> callback)
{
    std::cout << "Subscribe" << std::endl;
    ::natsStatus s;

    std::cout << "Listening asynchronously on " << subject << std::endl;

    m_Subscriptions[subject] = new NatsSubscription(m_pConn, subject, callback);

    return (s == NATS_OK);
}
