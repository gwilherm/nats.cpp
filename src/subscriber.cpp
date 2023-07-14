#include <iostream>
#include <signal.h>

#include "NatsClient.h"

bool _running = true;

void sigint_handler(int s)
{
    _running = false;
}

void onFoo(std::string message)
{
    std::cout << "Got the foo! [" << message << "]" << std::endl;
}

void onBar(std::string message)
{
    std::cout << "Got the bar! [" << message << "]" << std::endl;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);

    NatsClient* client = NatsClient::createNatsClient(false);

    client->start();

    client->subscribe("foo", onFoo);
    client->subscribe("bar", onBar);

    while (_running)
    {
        usleep(1000);
    }

    client->stop();

    delete client;

    return 0;
}
