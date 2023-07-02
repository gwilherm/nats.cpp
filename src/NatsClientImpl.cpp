#include "NatsClientImpl.h"

#include <iostream>
#include <nats/status.h>

int64_t NatsClientImpl::m_dropped = 0;

static void
onMsg(natsConnection *nc, natsSubscription *m_pSub, natsMsg *msg, void *closure)
{
    std::cout << "Received msg: "
                << ::natsMsg_GetSubject(msg) << " - "
                << ::natsMsg_GetData(msg)
                << std::endl;

    ::natsMsg_Destroy(msg);
}

NatsClientImpl::NatsClientImpl(const bool statistics) :
    m_statistics(statistics),
    m_pConn(NULL),
    m_pOpts(NULL),
    m_pSub(NULL),
    m_pStats(NULL),
    m_pMsg(NULL)
{
    if (!initialize())
        std::cerr << "Error at initialization" << std::endl;
}

void NatsClientImpl::asyncErrCb(natsConnection *nc, natsSubscription *m_pSub, natsStatus err, void *closure)
{
    std::cerr << "Async error: " 
        << err << " - "
        << ::natsStatus_GetText(err)
        << std::endl;

    ::natsSubscription_GetDropped(m_pSub, (int64_t*) &m_dropped);
}


bool NatsClientImpl::initialize()
{
    natsStatus s = NATS_OK;

    if (::natsOptions_Create(&m_pOpts) != NATS_OK)
        s = NATS_NO_MEMORY;

    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;

    s = ::natsOptions_SetErrorHandler(m_pOpts, asyncErrCb, NULL);
    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;

    return (s == NATS_OK);
}

bool NatsClientImpl::start()
{
    std::cout << "Start" << std::endl;
    natsStatus s;

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
    ::natsSubscription_Destroy(m_pSub);
    ::natsConnection_Destroy(m_pConn);
    ::natsOptions_Destroy(m_pOpts);

    // To silence reports of memory still in used with valgrind
    ::nats_Close();
}

bool NatsClientImpl::subscribe(const std::string subject)
{
    std::cout << "Subscribe" << std::endl;
    natsStatus s;

    std::cout << "Listening asynchronously on " << subject << std::endl;

    s = ::natsConnection_Subscribe(&m_pSub, m_pConn, subject.c_str(), onMsg, NULL);
    if (s != NATS_OK)
        std::cerr << "*Error: " << s << std::endl;

    // For maximum performance, set no limit on the number of pending messages.
    if (s == NATS_OK)
        s = ::natsSubscription_SetPendingLimits(m_pSub, -1, -1);

    if (s != NATS_OK)
        std::cerr << "Error: " << s << std::endl;
    // if (s == NATS_OK)
    //     s = ::natsSubscription_AutoUnsubscribe(m_pSub, (int) total);

    return (s == NATS_OK);
}