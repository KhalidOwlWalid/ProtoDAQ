#pragma once

#include "../protodaq_core.hpp"

namespace ProtoDAQ {

class Mavlink: public DAQ_Backend {

public:
    Mavlink() = default;
    ~Mavlink() = default;

    // Overriding the virtual function from parent class
    void setup() override {};

    bool init() override {
        spdlog::info("{} init() function. Initialize protocol specific socket/implementation.", protocol_type());
        return true;
    }
    void update() override {
        _backend_counter++;
        spdlog::info("{} update() function. Process data here. Backend Counter: {}", protocol_type(), _backend_counter);
    };
    void shutdown() override {
        spdlog::info("{} shutdown() function. Implement protocol specific cleanup here.", protocol_type());
    };

    const char *protocol_type() const override { return "Mavlink"; }

    void send_heartbeat() {};
    void listen_to_heartbeat() {};
    void send_mavlink_packets() {};
};

}
