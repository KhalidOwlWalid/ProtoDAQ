#include "include/ImDAQ.hpp"

int main() {
    ProtoDAQ::ImDAQ imdaq;

    // std::vector<DAQ_Protocol> protocols_to_use {DAQ_Protocol::UDP, DAQ_Protocol::MAVLINK};
    std::vector<DAQ_Protocol> protocols_to_use {DAQ_Protocol::MAVLINK};

    if (!imdaq.init(protocols_to_use)) {
        spdlog::error("DAQ fails to initialize due to invalid protocol. Force closing the application.");
        return 0;
    }

    if (imdaq.is_running()) {
        for (size_t i = 0; i < 5; i++) {
            imdaq.update();
        }
        imdaq.set_running_flag(false);
    }

    imdaq.shutdown();

    return 0;
}
