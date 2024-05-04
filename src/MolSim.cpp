#include <iostream>
#include <boost/program_options.hpp>
#include "spdlog/spdlog.h"

#include "moleculeSimulator/Simulator.h"

int main(int argc, char *argsv[]) {

  double endT;
  double deltaT;
  std::string inputFilePath;

    spdlog::info("Hello from MolSim for PSE!");

    namespace po = boost::program_options;

    //TODO: Parse Log level here + change all cout to spdlog or remove them
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "Provides some help.")
            ("tEnd,e", po::value<double>(&endT)->default_value(1000),
             "Time to which simulation will run (starting at 0).")
            ("deltaT,d", po::value<double>(&deltaT)->default_value(0.014), "Duration of one time step.")
            ("inputFilePath,f", po::value<std::string>(&inputFilePath), "Path to the input file. Caution: This "
                                                                        "is a required argument. In case it is not specified, the program will terminate immediately.");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argsv, desc), vm);
    }
    catch (boost::wrapexcept<po::unknown_option> &e) {
        std::cout << "Something went wrong while parsing your arguments: " << e.what()
                  << "\nPlease have a look on the usage!\n" << std::endl;

        std::cout << desc << "\n";
        return -1;
    }
    catch (...) {
        std::cout << "Something went wrong while parsing your arguments. Please have a look on the usage!\n"
                  << std::endl;
        std::cout << desc << "\n";
        return -1;
    }

    po::notify(vm);

    if (argc <= 1 || vm.count("help")) {
        std::cout << desc << "\n";
        return -1;
    }

    if (!vm.count("inputFilePath")) {
        std::cout << "Please specify a path to your input file!" << "\n";
        std::cout << desc << "\n";
        return -1;
    }

    std::cout << "Hello from MolSim for PSE!" << std::endl;

    Gravity gravity;
    Simulator simulator(inputFilePath, gravity, endT, deltaT);
    simulator.run();

    return 0;
}
