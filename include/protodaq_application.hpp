#pragma once

#include "protodaq_core.hpp"

// Backends
#include "backends/protodaq_mavlink.hpp"

namespace ProtoDAQ {

class App {

public:
    App() = default;
    ~App() = default;

    virtual bool init(const std::vector<DAQ_Protocol> protocols);
    virtual void update();
    virtual void shutdown();

    void set_running_flag(const bool flag);
    bool is_running() const;

protected:
    std::unique_ptr<DAQ_Backend> _create_backend(DAQ_Protocol protocol);
    std::vector<std::unique_ptr<DAQ_Backend>> _daq_backend_manager;

    bool _running = false;
    DAQ_Mode _current_daq_mode;
};

}
