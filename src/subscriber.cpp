#include <iostream>
#include <signal.h>

#include "NatsClient.h"

bool _running = true;

void sigint_handler(int s)
{
    _running = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);

    NatsClient* client = NatsClient::createNatsClient(false);

    client->start();

    client->subscribe("foo");

    while (_running)
    {
        usleep(1000);
    }

    client->stop();

    return 0;
}
