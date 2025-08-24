#include <spdlog/spdlog.h>

#include <vector>
#include <memory>
#include <spdlog/spdlog.h>

// Forward declaration
class DAQ_Backend;
class DAQ_Mavlink;
class DAQ_UDP;

enum class DAQ_Protocol {
    INVALID = -1,
    MAVLINK = 0,
    UDP = 1,
};

enum class DAQ_Mode {
    INVALID=-1,
    REAL_TIME=0,
    DATA_PLAYBACK = 1,
    SIMULATION = 2,
};

class DAQ_App {

public:

    // DAQ state implementation
    DAQ_App() = default;

    bool init(const std::vector<DAQ_Protocol> protocols);

    void update();
    void shutdown();

    bool is_running() { return _is_running; };

    void set_running_flag(const bool flag) {
        _is_running = flag;
    }

    // API
    const std::vector<float> &get_latest_data() const {};
    uint32_t get_sample_rate_ms() {};

    std::unique_ptr<DAQ_Backend> _create_backend(DAQ_Protocol protocol);

    std::vector<std::unique_ptr<DAQ_Backend>> _daq_backend_manager;
    

private:

    // Variable that stores the backend pointer
    DAQ_Mode _current_daq_mode;
    bool _is_running = false;

};

class DAQ_Backend {

// friend is important to be able to access protected members of the backend base class
friend class DAQ_UDP;
friend class DAQ_Mavlink;

public:
    DAQ_Backend() = default;
    ~DAQ_Backend() = default;

    // Abstract function that each function should have in their backend implementation
    virtual void setup() = 0;
    virtual bool init() {
        spdlog::info("Using default init() implementation for {}", protocol_type());
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

class DAQ_Mavlink: public DAQ_Backend {

public:
    DAQ_Mavlink() = default;
    ~DAQ_Mavlink() = default;

    // Overriding the virtual function from parent class
    void setup() override {};

    bool init() override {
        spdlog::info("{} init() function. Initialize protocol specific socket/implementation.", protocol_type());
    }
    void update() override {
        spdlog::info("{} update() function. Process data here.", protocol_type());
    };
    void shutdown() override {
        spdlog::info("{} shutdown() function. Implement protocol specific cleanup here.", protocol_type());
    };

    const char *protocol_type() const override { return "Mavlink"; }

    void send_heartbeat() {};
    void listen_to_heartbeat() {};
    void send_mavlink_packets() {};

};

std::unique_ptr<DAQ_Backend> DAQ_App::_create_backend(DAQ_Protocol protocol) {
    switch(protocol) {
        case DAQ_Protocol::MAVLINK:
            return std::make_unique<DAQ_Mavlink>();
        case DAQ_Protocol::UDP:
            return std::make_unique<DAQ_UDP>();
        default:
            spdlog::info("Protocol provided either does not exist or has not been implemented");
            return nullptr;
    }
}

bool DAQ_App::init(const std::vector<DAQ_Protocol> protocols) {

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

    _is_running = true;

    return true;
}

void DAQ_App::update() {
    for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
        _daq_backend_manager[i]->update();
    }
}

void DAQ_App::shutdown() {
    for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
        _daq_backend_manager[i]->shutdown();
    }
}