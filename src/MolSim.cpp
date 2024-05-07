#include <iostream>
#include <boost/program_options.hpp>
#include "moleculeSimulator/Simulator.h"
#include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"
#include "utils/TimeMeasurement.h"

int main(int argc, char *argsv[]) {
    double endT;
    double deltaT;
    std::string inputFilePath;
    std::string logLevel;
    bool timeMeasurement = false;

    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "Provides some help.")
            ("tEnd,e", po::value<double>(&endT)->default_value(1000),
             "Time to which simulation will run (starting at 0).")
            ("deltaT,d", po::value<double>(&deltaT)->default_value(0.014), "Duration of one time step.")
            ("inputFilePath,f", po::value<std::string>(&inputFilePath), "Path to the input file. Caution: This "
             "is a required argument. In case it is not specified, the program will terminate immediately.")
            ("logLevel,l", po::value<std::string>(&logLevel)->default_value("info"), "Log level:"
             " Possible options are (off, critical, error, warn, info, debug, trace)")
            ("time,t", "Perform time measurement. Logging will be disabled.");

    po::variables_map vm;

    try {
        po::parsed_options parsed = parse_command_line(argc, argsv, desc);

        if (!collect_unrecognized(parsed.options, po::include_positional).empty()) {
            std::cout << "Positional arguments are not allowed! Please have a look on the usage!\n";
            std::cout << desc << "\n";
            return -1;
        }
        store(parsed, vm);
    } catch (boost::wrapexcept<po::unknown_option> &e) {
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

    if (!setLogLevel(logLevel)) {
        std::cout << "Please specify a valid trace option!\n";
        std::cout << desc << "\n";
        return -1;
    }

    if (vm.count("time")) {
        timeMeasurement = true;
    }

    spdlog::info("Hello from MolSim for PSE!");

    LeonardJonesForce lJF;
    Simulator simulator(inputFilePath, lJF, endT, deltaT);

    if (timeMeasurement) {
        spdlog::info("Starting time measurement...");
        measureTime(simulator);
        spdlog::info("No output written.");
    } else {
        spdlog::info("Running without time measurement...");
        simulator.run(false);
    }
    return 0;
}
