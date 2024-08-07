#include <iostream>
#include <boost/program_options.hpp>

#include "fileHandling/reader/XMLReader/XMLReader.h"
#include "moleculeSimulator/Simulator.h"
#include "utils/Benchmark.h"
#include "utils/Logging.h"
#include "utils/InputHandler.h"
#include "utils/OutputHandler.h"

int main(int argc, char *argsv[]) {
    //Parameters for simulation
    try {
        double endT;
        double deltaT;
        std::string inputFilePath;
        std::string inputFileFormatString;
        std::string outputFileFormatString;
        std::string pathToMolecules;
        FileHandler::outputFormat outputFormat;
        FileHandler::inputFormat inputFormat;
        enumsStructs::TypeOfForce force;
        std::string logLevel;
        std::string selectedForce;
        std::string outputFileName = "file";
        bool benchmark = false;
        bool saveState = false;
        bool loadState = false;
        int outputFrequency;

        //Parsing of the command line arguments

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
                ("inputFileFormatString,i", po::value<std::string>(&inputFileFormatString),
                 "Format of the input file. Supported formats are txt and xml.")
                ("outputFileFormatString,o", po::value<std::string>(&outputFileFormatString)->default_value("vtk"),
                 "Format of the output file. Supported formats are vtk and xyz. Default is vtk.")
                ("time,t", "Perform time measurement. Logging will be disabled.")
                ("force,c", po::value<std::string>(&selectedForce)->default_value("ljf"),
                 "Force to use: Possible options are (gravity, ljf)")
                ("freq", po::value<int>(&outputFrequency)->default_value(50), "Output frequency.")
                ("baseName,b", po::value<std::string>(&outputFileName)->default_value("MD_vtk_"),
                 "Base name of the output files.")
                ("loadState", po::value<std::string>(&pathToMolecules),
                 "Load molecules from a checkpoint into your program")
                ("saveState", "Save state of molecules to a txt file after the simulation is done");;

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

        notify(vm);

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

        if (outputFormat = setOutputFormat(outputFileFormatString); outputFormat ==
                                                                    FileHandler::outputFormat::invalid) {
            std::cout << "Please specify the format of the output file!" << "\n";
            std::cout << desc << "\n";
            return -1;
        }

        if (vm.count("time")) {
            benchmark = true;
        }

        if (vm.count("saveState")) {
            saveState = true;
        }

        if (vm.count("loadState")) {
            loadState = true;
        }

        spdlog::info("Hello from MolSim for PSE!");

        std::unique_ptr<Simulator> simulator;

        //If an xml file is used parameters are taken from xml file. Simulation parameters specified over the command line will be ignored.
        if (inputFormat == FileHandler::inputFormat::xml) {
            SimulationSettings simulationSettings;
            const auto returnedErrorHandlingInt = XMLReader::readFile(inputFilePath, simulationSettings);
            if (returnedErrorHandlingInt != 0) {
                throw std::invalid_argument("Error while reading the XML file. Please check the file and try again. Exiting...");
            }
            simulator = std::make_unique<Simulator>(simulationSettings, outputFormat);
        }
            //Legacy input over the command line
        else {
            if (selectedForce == "gravity") {
                force = TypeOfForce::gravity;
            } else if (selectedForce == "ljf") {
                force = TypeOfForce::leonardJonesForce;
            } else {
                std::cout << "Please specify a valid force option!\n";
                std::cout << desc << "\n";
                return -1;
            }
            DirectSumSimulationParameters parameters = {deltaT, endT, force};
            simulator = std::make_unique<Simulator>(parameters, inputFilePath, outputFormat, outputFrequency,
                                                    outputFileName);
        }

        //Load state of molecules of a previous simulation if specified
        if (loadState) {
            spdlog::info("Loading state of molecules...");
            simulator->loadState(pathToMolecules);
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

        //Writing state of the molecules to a file if specified
        if (saveState) {
            simulator->saveState();
            spdlog::info("Saving state of molecules...");
        }
        return 0;
    }
    catch (const std::exception &e) {
        spdlog::error(e.what());
        return EXIT_FAILURE;
    }
}
