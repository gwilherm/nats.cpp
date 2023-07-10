#ifndef NATS_CLIENT
#define NATS_CLIENT

#include <string>
#include <functional>

class NatsClient
{
public:
    // NatsClient(const bool statistics = false);
    // virtual ~NatsClient() = 0;
    // Factory
    static NatsClient* createNatsClient(const bool statistics = false);

    virtual bool start() = 0;
    virtual void stop() = 0;

    virtual bool subscribe(const std::string subject, std::function<void(void)> callback) = 0;
};

#endif // NATS_CLIENT