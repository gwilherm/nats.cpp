set(CPPNATS_SOURCES ${PROJECT_SOURCE_DIR}/src/NatsClient.cpp ${PROJECT_SOURCE_DIR}/src/NatsClientImpl.cpp  ${PROJECT_SOURCE_DIR}/src/NatsSubscription.cpp)

add_executable(pub ${PROJECT_SOURCE_DIR}/src/publisher.c ${CPPNATS_SOURCES})
target_link_libraries(pub nats ${NATS_EXTRA_LIB} ${NATS_ASYNC_IO_LIB})

add_executable(sub ${PROJECT_SOURCE_DIR}/src/subscriber.cpp ${CPPNATS_SOURCES})
target_link_libraries(sub nats ${NATS_EXTRA_LIB} ${NATS_ASYNC_IO_LIB})
