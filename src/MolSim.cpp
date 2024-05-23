#include <iostream>
#include <boost/program_options.hpp>
#include "moleculeSimulator/Simulator.h"
#include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"
#include "utils/Benchmark.h"
#include "utils/Logging.h"
#include "utils/InputHander.h"
#include "utils/OutputHandler.h"

int main(int argc, char *argsv[]) {
    double endT;
    double deltaT;
    std::string inputFilePath;
    std::string inputFileFormatString;
    std::string outputFileFormatString;
    FileHandler::outputFormat outputFormat;
    FileHandler::inputFormat inputFormat;
    std::string logLevel;
    bool benchmark = false;

    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    //TODO: Move Write Frequency of Output, T-end and delta-t to XML file
    desc.add_options()
            ("help,h", "Provides some help.")
            ("tEnd,e", po::value<double>(&endT)->default_value(1000),
             "Time to which simulation will run (starting at 0).")
            ("deltaT,d", po::value<double>(&deltaT)->default_value(0.014), "Duration of one time step.")
            ("inputFilePath,f", po::value<std::string>(&inputFilePath), "Path to the input file. Caution: This "
             "is a required argument. In case it is not specified, the program will terminate immediately.")
            ("logLevel,l", po::value<std::string>(&logLevel)->default_value("info"), "Log level:"
             " Possible options are (off, critical, error, warn, info, debug, trace)")
            ("inputFileFormatString,i", po::value<std::string>(&inputFileFormatString),
                    "Format of the input file. Supported formats are txt and xml.")
            ("outputFileFormatString,o", po::value<std::string>(&outputFileFormatString)->default_value("vtk"),
                    "Format of the output file. Supported formats are vtk and xyz. Default is vtk.")
            ("time,t", "Perform time measurement. Logging will be disabled.");

    po::variables_map vm;

    // Choice of using cout in place of spdlog, because this is before our simulation starts and we choose to only
    // use spdlog after that.
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

    if(inputFormat = setInputFormat(inputFileFormatString); inputFormat == FileHandler::inputFormat::invalid){
        std::cout << "Please specify the format of the input file!" << "\n";
        std::cout << desc << "\n";
        return -1;
    }

    if(outputFormat = setOutputFormat(outputFileFormatString); outputFormat == FileHandler::outputFormat::invalid){
        std::cout << "Please specify the format of the output file!" << "\n";
        std::cout << desc << "\n";
        return -1;
    }

    if (vm.count("time")) {
        benchmark = true;
    }

    spdlog::info("Hello from MolSim for PSE!");

    LeonardJonesForce lJF;
    Simulator simulator(inputFilePath, lJF, endT, deltaT, inputFormat, outputFormat);

    if (benchmark) {
        spdlog::info("Starting time measurement...");
        performBenchmark(simulator);
        spdlog::info("No output written.");
    } else {
        spdlog::info("Running without time measurement...");
        simulator.run(false);
    }
    return 0;
}
