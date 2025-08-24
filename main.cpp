#include "include/protodaq_application.hpp"

int main() {
    DAQ_App daq_app;

    std::vector<DAQ_Protocol> protocols_to_use {DAQ_Protocol::UDP, DAQ_Protocol::MAVLINK};

    if (!daq_app.init(protocols_to_use)) {
        spdlog::error("DAQ fails to initialize due to invalid protocol. Force closing the application.");
        return 0;
    }

    if (daq_app.is_running()) {
        for (size_t i = 0; i < 5; i++) {
            daq_app.update();
        }
        daq_app.set_running_flag(false);
    }

    daq_app.shutdown();

    return 0;
}