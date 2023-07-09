#ifndef NATS_CLIENT
#define NATS_CLIENT

#include <string>

class NatsClient
{
public:
    // NatsClient(const bool statistics = false);
    // virtual ~NatsClient() = 0;
    // Factory
    static NatsClient* createNatsClient(const bool statistics = false);

    virtual bool start() = 0;
    virtual void stop() = 0;

    virtual bool subscribe(const std::string subject, void(*cb)(void)) = 0;
};

#endif // NATS_CLIENT