#include "../include/protodaq_application.hpp"

using namespace ProtoDAQ;

bool App::init(const std::vector<DAQ_Protocol> protocols) {
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

void App::run() {
    for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
        _daq_backend_manager[i]->update();
    }
}

void App::shutdown() {
    for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
        _daq_backend_manager[i]->shutdown();
    }
}

void App::set_running_flag(const bool flag) {
    _running = flag;
}

bool App::is_running() const {
    return _running;
}

std::unique_ptr<DAQ_Backend> App::_create_backend(DAQ_Protocol protocol) {
    switch(protocol) {
        case DAQ_Protocol::MAVLINK:
            return std::make_unique<ProtoDAQ::Mavlink>();
        // case DAQ_Protocol::UDP:
        //     return std::make_unique<DAQ_UDP>();
        default:
            spdlog::info("Protocol provided either does not exist or has not been implemented");
            return nullptr;
    }
}
