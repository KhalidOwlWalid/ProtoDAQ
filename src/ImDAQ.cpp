#include "../include/ImDAQ.hpp"

using namespace ProtoDAQ;

ImDAQ::ImDAQ() {
    spdlog::info("ImDAQ application");
}

ImDAQ::~ImDAQ() {
    spdlog::info("Closing ImDAQ");
}

bool ImDAQ::init(const std::vector<DAQ_Protocol> protocols) {
    for (size_t i = 0; i < protocols.size(); i++) {
        std::unique_ptr<DAQ_Backend> backend_ptr;
        backend_ptr = _create_backend(protocols[i]);

        if (backend_ptr == nullptr) {
            spdlog::warn("Fails to create backend for this protocol");
            continue;
        } else {
            backend_ptr->init();
        }

        _daq_backend_manager.push_back(std::move(backend_ptr));
    }

    _running = true;

    return true;
}

void ImDAQ::update() {
    for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
        _daq_backend_manager[i]->update();
    }
}

void ImDAQ::shutdown() {
    for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
        _daq_backend_manager[i]->shutdown();
    }
}
