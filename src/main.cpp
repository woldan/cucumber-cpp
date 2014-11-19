#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

namespace {

void acceptWireProtocol(int port) {
    using namespace ::cucumber::internal;
    CukeEngineImpl cukeEngine;
    JsonSpiritWireMessageCodec wireCodec;
    WireProtocolHandler protocolHandler(&wireCodec, &cukeEngine);
    SocketServer server(&protocolHandler);
    server.listen(port);
    server.acceptOnce();
}

namespace po = boost::program_options;

class command_line {
  public:
    command_line(int argc, char** argv)
        : vars_() {
      po::options_description description("Options");
      description.add_options()
        ("help", "produce help message")
        ("port", po::value<int>()->default_value(3902), "set port to listen on")
        ("frontend", po::value<std::string>()->default_value("wire"), "the frontend to use: [wire], builtin")
        ("features", po::value<std::string>()->default_value("features"), "the directory where the cucumber features are searched for: [features]")
      ;

      po::store(po::parse_command_line(argc, argv, description), vars_);

      if (vars_.count("help")) {
        std::cout << description << std::endl;
      }
    }

    const po::variable_value& operator[](const std::string& key) const {
      return vars_[key];
    }

  private:
    po::variables_map vars_;
};

}

int main(int argc, char **argv) {
    try {
        command_line cli(argc, argv);
        auto port = cli["port"].as<int>();
        auto frontend = cli["frontend"].as<std::string>();

        if (frontend == "wire") {
          acceptWireProtocol(port);
        } else if (frontend == "builtin") {
          throw std::runtime_error("TODO: dispatch to builtin frontend not yet implemented!");
        } else {
          throw std::runtime_error("Unrecognized frontend '" + frontend + "'.");
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return 0;
}
