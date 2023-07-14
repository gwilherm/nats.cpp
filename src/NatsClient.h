#ifndef NATS_CLIENT
#define NATS_CLIENT

#include <string>
#include <functional>

class NatsClient
{
public:
    // Factory
    static NatsClient* createNatsClient(const bool statistics = false);

    virtual bool start() = 0;
    virtual void stop() = 0;

    virtual bool subscribe(const std::string subject, std::function<void(std::string)> callback) = 0;
};

#endif // NATS_CLIENT