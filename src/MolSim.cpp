#include <iostream>
#include <boost/program_options.hpp>

#include "models/LinkedCells.h"
#include "moleculeSimulator/Simulator.h"
#include "particleRepresentation/container/LinkedCellsContainer.h"
#include "utils/Benchmark.h"
#include "utils/Logging.h"
#include "utils/InputHander.h"
#include "utils/OutputHandler.h"

int main(int argc, char *argsv[]) {
    //Parameters for simulation
    double endT;
    double deltaT;
    double rCutOff;
    //Todo: At the moment hard coded. Should be configurable within the xml
    double epsilon = 5;
    double sigma = 1;
    TypeOfForce force;
    std::string inputFilePath;
    std::string inputFileFormatString;
    std::string outputFileFormatString;
    FileHandler::outputFormat outputFormat;
    FileHandler::inputFormat inputFormat;
    std::string logLevel;
    std::string selectedForce;
    std::string selectedModel;
    std::string filename = "Experiment";
    std::vector<double> domain;
    bool benchmark = false;
    int outputFrequency = 50;

    //Parsing of the command line arguments

    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    //TODO: Move Write Frequency of Output, T-end and delta-t to XML file
    //TODO: Move to txt reader
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
            ("time,t", "Perform time measurement. Logging will be disabled.")
            ("force,c", po::value<std::string>(&selectedForce)->default_value("ljf"),
             "Force to use: Possible options are (gravity, ljf)")
            ("model,m", po::value<std::string>(&selectedModel)->default_value("lc"),
             "Model to use: Possible options are (ds, lc)")
            ("domain", po::value<std::vector<double> >(&domain)->multitoken(),
             "Size of the domain: The following format is expected: --domain N1 N2 N3")
            ("cutoff,r", po::value<double>(&rCutOff)->default_value(3.0),
             "The cutoff radius. Is only considered, when the Linked Cell Model is used.");

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

    if (inputFormat = setInputFormat(inputFileFormatString); inputFormat == FileHandler::inputFormat::invalid) {
        std::cout << "Please specify the format of the input file!" << "\n";
        std::cout << desc << "\n";
        return -1;
    }

    if (outputFormat = setOutputFormat(outputFileFormatString); outputFormat == FileHandler::outputFormat::invalid) {
        std::cout << "Please specify the format of the output file!" << "\n";
        std::cout << desc << "\n";
        return -1;
    }

    if (vm.count("time")) {
        benchmark = true;
    }

    spdlog::info("Hello from MolSim for PSE!");

    if (selectedForce == "gravity") {
        force = TypeOfForce::gravity;
    } else if (selectedForce == "ljf") {
        force = TypeOfForce::leonardJonesForce;
    } else {
        std::cout << "Please specify a valid force option!\n";
        std::cout << desc << "\n";
        return -1;
    }

    std::unique_ptr<Simulator> simulator;

    if (selectedModel == "ds") {
        DirectSumSimulationParameters parameters = {deltaT, endT, epsilon, sigma, force};
        simulator = std::make_unique<Simulator>(parameters, inputFilePath, inputFormat, outputFormat, outputFrequency, filename);
    } else if (selectedModel == "lc") {
        if (domain.size() != 3) {
            std::cout << "Please specify a valid domain option!\n";
            std::cout << desc << "\n";
            return -1;
        }
        //Todo: At the moment hard coded, should be configurable within the xml file
        static std::array<std::pair<LinkedCellsContainer::Side, LinkedCells::BoundaryCondition>, 6> boundarySettings = {
            std::pair{LinkedCellsContainer::Side::front, LinkedCells::BoundaryCondition::reflective},
            std::pair{LinkedCellsContainer::Side::right, LinkedCells::BoundaryCondition::reflective},
            std::pair{LinkedCellsContainer::Side::back, LinkedCells::BoundaryCondition::reflective},
            std::pair{LinkedCellsContainer::Side::left, LinkedCells::BoundaryCondition::reflective},
            std::pair{LinkedCellsContainer::Side::top, LinkedCells::BoundaryCondition::reflective},
            std::pair{LinkedCellsContainer::Side::bottom, LinkedCells::BoundaryCondition::reflective}
        };
        LinkedCellsSimulationParameters parameters = {
            deltaT, endT, epsilon, sigma, force, rCutOff, {domain[0], domain[1], domain[2]}, boundarySettings
        };
        simulator = std::make_unique<Simulator>(parameters, inputFilePath, inputFormat, outputFormat, outputFrequency, filename);
    } else {
        std::cout << "Please specify a valid model option!\n";
        std::cout << desc << "\n";
        return -1;
    }

    //Starting simulation with or without time measurement

    if (benchmark) {
        spdlog::info("Starting time measurement...");
        performBenchmark(*simulator);
        spdlog::info("No output written.");
    } else {
        spdlog::info("Running without time measurement...");
        simulator->run(false);
    }
    return 0;
}
