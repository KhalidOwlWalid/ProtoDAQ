#pragma once

#include "../protodaq_core.hpp"

class DAQ_UDP: public DAQ_Backend {

public:
    DAQ_UDP() = default;
    ~DAQ_UDP() = default;

    // Overriding the virtual function from parent class
    void setup() override {};


    bool init() override {
        spdlog::info("{} init() function. Initialize protocol specific socket/implementation.", protocol_type());
    };
    void update() override {
        spdlog::info("{} update() function. Process data here.", protocol_type());
    };
    void shutdown() override {
        spdlog::info("{} shutdown() function. Implement protocol specific cleanup here.", protocol_type());
    };

    const char *protocol_type() const override { return "UDP"; }

    // Protocol specific functions or variables
    // Please read my notes on protocol specific functions in DAQ_Mavlink
    void parse_udp_packets();
    void receive_udp_packets();

};

