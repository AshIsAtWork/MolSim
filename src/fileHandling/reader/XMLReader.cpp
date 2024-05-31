//
// Created by Ashutosh Solanki on 22.05.24.
//

#include "XMLReader.h"

//Reference: https://xerces.apache.org/xerces-c/program-dom-3.html
int XMLReader::readFile(std::string &filename, enumsStructs::SimulationSettings& simulationSettings) {
    try {
        xercesc::XMLPlatformUtils::Initialize();
    } catch (const xercesc::XMLException &toCatch) {
        char *message = xercesc::XMLString::transcode(toCatch.getMessage());
        spdlog::error("Error during initialization! :\n{}", message);
        xercesc::XMLString::release(&message);
        return 1;
    }

    auto *parser = new xercesc::XercesDOMParser();
    parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);

    auto *errHandler = (xercesc::ErrorHandler *) new xercesc::HandlerBase();
    parser->setErrorHandler(errHandler);

    try {
        parser->parse(filename.c_str());
        xercesc::DOMDocument *xmlDoc = parser->getDocument();
        xercesc::DOMElement *elementRoot = xmlDoc->getDocumentElement();

        if (!elementRoot) {
            spdlog::error("empty document");
            return 1;
        }

        // Initialize the XML schema flags and container
        xml_schema::flags flags = xml_schema::flags::dont_initialize;
        xml_schema::container *container = nullptr;

        // Correctly create the object using the generated constructor
        Molecules molecules(*elementRoot, flags, container);

        // Accessing elements
        //TODO: Change to info -> debug
        if (molecules.OutputFileName().empty()) {
            spdlog::error("OutputFileName is empty");
            return 1;
        } else {
            simulationSettings.outputFileName = static_cast<std::string>(molecules.OutputFileName());
            spdlog::debug("OutputFileName: {}", molecules.OutputFileName());
        }

        // static_cast<int> is required because internally the value is stored as a custom type
        if (static_cast<int>(molecules.OutputFrequency() <= 0)) {
            spdlog::error("OutputFrequency is less than 0");
            return 1;
        } else {
            simulationSettings.outputFrequency = static_cast<int>(molecules.OutputFrequency());
            spdlog::debug("OutputFrequency: {}", static_cast<int> (molecules.OutputFrequency()));
        }

        if (static_cast<double>(molecules.t_end()) < 0) {
            spdlog::error("t_end is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.endT = static_cast<int>(molecules.t_end());
            simulationSettings.parametersLinkedCells.endT = static_cast<int>(molecules.t_end());
            spdlog::debug("t_end: {}", static_cast<double>(molecules.t_end()));
        }

        if (static_cast<double>(molecules.delta_t()) <= 0) {
            spdlog::error("delta_t is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.deltaT = static_cast<int>(molecules.delta_t());
            simulationSettings.parametersLinkedCells.deltaT = static_cast<int>(molecules.delta_t());
            spdlog::debug("delta_t: {}", static_cast<double>(molecules.delta_t()));
        }

        //TODO: Use this force
        enumsStructs::TypeOfForce force = enumsStructs::setForce(molecules.force());
        if (force == enumsStructs::TypeOfForce::invalid) {
            spdlog::error("Force is invalid");
            return 1;
        } else {
            simulationSettings.force = force;
            spdlog::debug("Force: {}", molecules.force());
        }

        //TODO: Use this Model
        enumsStructs::TypeOfModel model = enumsStructs::setModel(molecules.model());
        if (model == enumsStructs::TypeOfModel::invalid) {
            spdlog::error("Model is invalid");
            return 1;
        } else {
            simulationSettings.model = model;
            spdlog::debug("Model: {}", molecules.model());
        }
        if (static_cast<double>(molecules.Sigma()) < 0) {
            spdlog::error("Sigma is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.sigma = static_cast<double>(molecules.Sigma());
            simulationSettings.parametersLinkedCells.sigma = static_cast<double>(molecules.Sigma());
            spdlog::debug("Sigma: {}", static_cast<double>(molecules.Sigma()));
        }

        if (static_cast<double>(molecules.Epsilon()) < 0) {
            spdlog::error("Epsilon is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.epsilon = static_cast<double>(molecules.Sigma());
            simulationSettings.parametersLinkedCells.epsilon = static_cast<double>(molecules.Sigma());
            spdlog::debug("Epsilon: {}", static_cast<double>(molecules.Epsilon()));
        }

        simulationSettings.parametersLinkedCells.domainSize = {
            static_cast<double>(molecules.DomainSize().First()),
            static_cast<double>(molecules.DomainSize().Second()),
            static_cast<double>(molecules.DomainSize().Third())
        };

        spdlog::debug("DomainSize: {}, {}, {}",
                     static_cast<double>(molecules.DomainSize().First()),
                     static_cast<double>(molecules.DomainSize().Second()),
                     static_cast<double>(molecules.DomainSize().Third()));

        if (static_cast<double >(molecules.rCutOff()) < 0) {
            spdlog::error("rCutOff is less than 0");
            return 1;
        } else {
            simulationSettings.parametersLinkedCells.rCutOff = static_cast<double >(molecules.rCutOff());
            spdlog::debug("rCutOff: {}", static_cast<double>(molecules.rCutOff()));
        }

        simulationSettings.parametersLinkedCells.boundarySettings =  {
            std::pair{enumsStructs::Side::front, enumsStructs::setBoundaryCondition(molecules.BoundaryCondition().boundaries().Front())},
            std::pair{enumsStructs::Side::right, enumsStructs::setBoundaryCondition(molecules.BoundaryCondition().boundaries().Right())},
            std::pair{enumsStructs::Side::back, enumsStructs::setBoundaryCondition(molecules.BoundaryCondition().boundaries().Back())},
            std::pair{enumsStructs::Side::left, enumsStructs::setBoundaryCondition(molecules.BoundaryCondition().boundaries().Left())},
            std::pair{enumsStructs::Side::top, enumsStructs::setBoundaryCondition(molecules.BoundaryCondition().boundaries().Top())},
            std::pair{enumsStructs::Side::bottom, enumsStructs::setBoundaryCondition(molecules.BoundaryCondition().boundaries().Bottom())}
        };

        spdlog::debug("BoundaryCondition: Front: {}, Back: {}, Left: {}, Right: {}, Top: {}, Bottom: {}",
                     molecules.BoundaryCondition().boundaries().Front(),
                     molecules.BoundaryCondition().boundaries().Back(),
                     molecules.BoundaryCondition().boundaries().Left(),
                     molecules.BoundaryCondition().boundaries().Right(),
                     molecules.BoundaryCondition().boundaries().Top(),
                     molecules.BoundaryCondition().boundaries().Bottom());

        if (molecules.SingleParticles().present()) {
            spdlog::debug("SingleParticles Size: {}", static_cast<int>(molecules.SingleParticles().get().Size()));
            if (molecules.SingleParticles().get().Size() != molecules.SingleParticles().get().SingleParticle().size()) {
                spdlog::error("Input Size and Number of SingleParticles Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.SingleParticles().get().Size(); i++) {
                enumsStructs::ParticleType p{
                    {
                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().X()),
                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().Y()),
                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().Z())
                    },
                    {
                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Velocity().X()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Velocity().Y()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Velocity().Z())
                    },
                    static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(i).Mass())
                    };
                simulationSettings.particles.push_back(p);
                spdlog::debug("SingleParticle #{}: Position: {}, {}, {}",
                             i + 1 + 1,
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().X()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().Y()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().Z()));
                spdlog::debug("SingleParticle #{}: Velocity: {}, {}, {}",
                             i + 1 + 1,
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Velocity().X()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Velocity().Y()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Velocity().Z()));
            }
        }

        if (molecules.Cuboids().present()) {
            spdlog::debug("Cuboids Size: {}", static_cast<int>(molecules.Cuboids().get().Size()));
            if (molecules.Cuboids().get().Size() != molecules.Cuboids().get().Cuboid().size()) {
                spdlog::error("Input Size and Number of Cuboids Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.Cuboids().get().Size(); i++) {
                enumsStructs::Cuboid cuboid = {
                    {
                        static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().X()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().Y()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().Z())
                    },
                    {
                        static_cast<unsigned>(molecules.Cuboids().get().Cuboid().at(i).N1()),
                        static_cast<unsigned>(molecules.Cuboids().get().Cuboid().at(i).N2()),
                        static_cast<unsigned>(molecules.Cuboids().get().Cuboid().at(i).N3())
                    },
                    static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Distance()),
                    static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Mass()),
                    {
                        static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().X()),
                        static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().Y()),
                        static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().Z())
                    },
                    static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).DimensionBrownian()),
                    static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Brownian())
                };
                simulationSettings.cuboids.push_back(cuboid);
                spdlog::debug("Cuboid #{}: Position: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().X()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().Y()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().Z()));
                spdlog::debug("Cuboid #{}: Velocity: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().X()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().Y()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().Z()));
                spdlog::debug("Cuboid #{}: Mass: {}", i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Mass()));
                spdlog::debug("Cuboid #{}: N1: {}", i + 1,
                             static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N1()));
                spdlog::debug("Cuboid #{}: N2: {}", i + 1,
                             static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N2()));
                spdlog::debug("Cuboid #{}: N3: {}", i + 1,
                             static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N3()));
                spdlog::debug("Cuboid #{}: Distance: {}", i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Distance()));
                spdlog::debug("Cuboid #{}: Brownian: {}", i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Brownian()));
                spdlog::debug("Cuboid #{}: DimensionBrownian: {}", i + 1,
                             static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).DimensionBrownian()));
            }
        }

        if (molecules.Discs().present()) {
            spdlog::debug("Discs Size: {}", static_cast<int>(molecules.Discs().get().Size()));
            if (molecules.Discs().get().Size() != molecules.Discs().get().Disc().size()) {
                spdlog::error("Input Size and Number of Discs Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.Discs().get().Size(); i++) {
                enumsStructs::Disc disc = {
                    {
                        static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().X()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().Y()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().Z())
                    },
                    {
                        static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().X()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().Y()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().Z())
                    },
                    static_cast<int>(molecules.Discs().get().Disc().at(i).Radius()),
                    static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Distance()),
                    static_cast<double>(molecules.Discs().get().Disc().at(i).Mass()),
                    static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).DimensionBrownian()),
                    static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Brownian())
                };
                simulationSettings.discs.push_back(disc);
                spdlog::debug("Disc #{}: Center: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().X()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().Y()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().Z()));
                spdlog::debug("Disc #{}: Velocity: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().X()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().Y()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().Z()));
                spdlog::debug("Disc #{}: Mass: {}", i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(i).Mass()));
                spdlog::debug("InterParticleDistance: {}",
                             static_cast<double>(molecules.Discs().get().Disc().at(i).InterParticleDistance()));
                spdlog::debug("Disc #{}: Radius: {}", i + 1,
                             static_cast<int>(molecules.Discs().get().Disc().at(i).Radius()));
                spdlog::debug("Disc #{}: Brownian: {}", i + 1,
                             static_cast<int>(molecules.Discs().get().Disc().at(i).Brownian()));
                spdlog::debug("Disc #{}: DimensionBrownian: {}", i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(i).DimensionBrownian()));
            }
        }

    } catch (const xercesc::XMLException &toCatch) {
        char *message = xercesc::XMLString::transcode(toCatch.getMessage());
        spdlog::error("Exception message is: \n{}", message);
        xercesc::XMLString::release(&message);
        return 1;
    } catch (const xercesc::DOMException &toCatch) {
        char *message = xercesc::XMLString::transcode(toCatch.msg);
        spdlog::error("Exception message is: \n{}", message);
        xercesc::XMLString::release(&message);
        return 1;
    } catch (...) {
        spdlog::error("Encountered an exception during parsing");
        return 1;
    }

    spdlog::info("{}: parse OK", filename);

    delete parser;
    delete errHandler;
    return 0;
}