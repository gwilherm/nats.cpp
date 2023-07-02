#include "NatsClient.h"
#include "NatsClientImpl.h"

NatsClient* NatsClient::createNatsClient(const bool statistics)
{
    return new NatsClientImpl(statistics);
}