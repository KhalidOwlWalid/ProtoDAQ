#pragma once

#include "protodaq_application.hpp"

namespace ProtoDAQ {

class ImDAQ: public App {

public:
    ImDAQ();
    ~ImDAQ();

    bool init(const std::vector<DAQ_Protocol> protocols) override;
    void update() override;
    void shutdown() override;
};

}