#pragma once

#include "protodaq_common.hpp"

namespace ProtoDAQ {

class DAQ_Backend {

// friend is important to be able to access protected members of the backend base class
// friend class DAQ_UDP;
friend class DAQ_Mavlink;

public:
    DAQ_Backend() = default;
    ~DAQ_Backend() = default;

    // Abstract function that each function should have in their backend implementation
    virtual void setup() = 0;
    virtual bool init() {
        spdlog::info("Using default init() implementation for {}", protocol_type());
        return true;
    };
    virtual void update() {
        spdlog::info("Backend for {} has not been implemented. Using the default update() function.", protocol_type());
        ++_backend_counter;
        spdlog::info("Backend counter for {}: {}", protocol_type(), _backend_counter);
    };

    virtual void shutdown() {
        spdlog::info("Using default shutdown() implementation for {}", protocol_type());
    };

    virtual const char *protocol_type() const = 0;

protected:
    size_t _backend_counter;
};

}
