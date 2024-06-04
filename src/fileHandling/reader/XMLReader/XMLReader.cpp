//
// Created by Ashutosh Solanki on 22.05.24.
//

#include "XMLReader.h"

//Reference: https://xerces.apache.org/xerces-c/program-dom-3.html
int XMLReader::readFile(std::string &filename, enumsStructs::SimulationSettings &simulationSettings) {
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

    auto *errHandler = (xercesc::ErrorHandler *)
            new xercesc::HandlerBase();
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
        if (molecules.OutputFileName().empty()) {
            spdlog::error("OutputFileName is empty");
            return 1;
        } else if (std::any_of(molecules.OutputFileName().begin(), molecules.OutputFileName().end(), ::isdigit)) {
            spdlog::error("OutputFileName contains a number");
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
            spdlog::debug("OutputFrequency: {}", static_cast<int>(molecules.OutputFrequency()));
        }

        //TODO: Use this Model
        enumsStructs::TypeOfModel model = enumsStructs::setModel(molecules.model().Name());
        if (model == enumsStructs::TypeOfModel::invalid) {
            spdlog::error("Model is invalid");
            return 1;
        } else {
            simulationSettings.model = model;
            spdlog::debug("Model: {}", molecules.model().Name());
        }

        if (static_cast<double>(molecules.model().t_end()) < 0) {
            spdlog::error("t_end is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.endT = static_cast<double>(molecules.model().t_end());
            simulationSettings.parametersLinkedCells.endT = static_cast<double>(molecules.model().t_end());
            spdlog::debug("t_end: {}", static_cast<double>(molecules.model().t_end()));
        }

        if (static_cast<double>(molecules.model().delta_t()) <= 0) {
            spdlog::error("delta_t is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.deltaT = static_cast<double>(molecules.model().delta_t());
            simulationSettings.parametersLinkedCells.deltaT = static_cast<double>(molecules.model().delta_t());
            spdlog::debug("delta_t: {}", static_cast<double>(molecules.model().delta_t()));
        }

        enumsStructs::TypeOfForce force = enumsStructs::setForce(molecules.model().force());
        if (force == enumsStructs::TypeOfForce::invalid) {
            spdlog::error("Force is invalid");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.force = force;
            simulationSettings.parametersLinkedCells.force = force;
            spdlog::debug("Force: {}", molecules.model().force());
        }

        if (static_cast<double>(molecules.model().Sigma()) < 0) {
            spdlog::error("Sigma is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.sigma = static_cast<double>(molecules.model().Sigma());
            simulationSettings.parametersLinkedCells.sigma = static_cast<double>(molecules.model().Sigma());
            spdlog::debug("Sigma: {}", static_cast<double>(molecules.model().Sigma()));
        }

        if (static_cast<double>(molecules.model().Epsilon()) < 0) {
            spdlog::error("Epsilon is less than 0");
            return 1;
        } else {
            simulationSettings.parametersDirectSum.epsilon = static_cast<double>(molecules.model().Epsilon());
            simulationSettings.parametersLinkedCells.epsilon = static_cast<double>(molecules.model().Epsilon());
            spdlog::debug("Epsilon: {}", static_cast<double>(molecules.model().Epsilon()));
        }

        if (molecules.model().Name() == "LinkedCells") {
            if (molecules.model().DomainSize().present()) {
                if (static_cast<double>(molecules.model().DomainSize().get().First()) < 0) {
                    spdlog::error("Domain Size First is negative");
                    return 1;
                }
                if (static_cast<double>(molecules.model().DomainSize().get().Second()) < 0) {
                    spdlog::error("Domain Size Second is negative");
                    return 1;
                }
                if (static_cast<double>(molecules.model().DomainSize().get().Third()) < 0) {
                    spdlog::error("Domain Size Third is negative");
                    return 1;
                }
                simulationSettings.parametersLinkedCells.domainSize = {
                        static_cast<double>(molecules.model().DomainSize().get().First()),
                        static_cast<double>(molecules.model().DomainSize().get().Second()),
                        static_cast<double>(molecules.model().DomainSize().get().Third())
                };
                spdlog::debug("DomainSize: {}, {}, {}",
                              static_cast<double>(molecules.model().DomainSize().get().First()),
                              static_cast<double>(molecules.model().DomainSize().get().Second()),
                              static_cast<double>(molecules.model().DomainSize().get().Third()));
            } else {
                spdlog::error("DomainSize is not present");
                return 1;
            }
            if (molecules.model().rCutOff().present()) {
                if (static_cast<double>(molecules.model().rCutOff().get()) < 0) {
                    spdlog::error("rCutOff is less than 0");
                    return 1;
                } else {
                    simulationSettings.parametersLinkedCells.rCutOff = static_cast<double>(molecules.model().rCutOff().
                            get());
                    spdlog::debug("rCutOff: {}", static_cast<double>(molecules.model().rCutOff().get()));
                }
            } else {
                spdlog::error("rCutOff is not present");
                return 1;
            }

            if (molecules.model().BoundaryCondition().present()) {
                std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition> cFront{
                        enumsStructs::Side::front,
                        enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Front())
                };
                std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition> cRight{
                        enumsStructs::Side::right,
                        enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Right())
                };
                std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition> cBack{
                        enumsStructs::Side::back,
                        enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Back())
                };
                std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition> cLeft{
                        enumsStructs::Side::left,
                        enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Left())
                };
                std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition> cTop{
                        enumsStructs::Side::top,
                        enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Top())
                };
                std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition> cBottom{
                        enumsStructs::Side::bottom,
                        enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Bottom())
                };

                simulationSettings.parametersLinkedCells.boundarySettings = {
                        cFront, cRight, cBack, cLeft, cTop,
                        cBottom
                };

                spdlog::debug("BoundaryCondition: Front: {}, Back: {}, Left: {}, Right: {}, Top: {}, Bottom: {}",
                              molecules.model().BoundaryCondition().get().boundaries().Front(),
                              molecules.model().BoundaryCondition().get().boundaries().Back(),
                              molecules.model().BoundaryCondition().get().boundaries().Left(),
                              molecules.model().BoundaryCondition().get().boundaries().Right(),
                              molecules.model().BoundaryCondition().get().boundaries().Top(),
                              molecules.model().BoundaryCondition().get().boundaries().Bottom());
            } else {
                spdlog::error("BoundaryCondition is not present");
                return 1;
            }
        }

        if (molecules.SingleParticles().present()) {
            spdlog::debug("SingleParticles Size: {}", static_cast<int>(molecules.SingleParticles().get().Size()));
            if (static_cast<size_t>(molecules.SingleParticles().get().Size()) != static_cast<size_t>(molecules.
                    SingleParticles().get().SingleParticle().size())) {
                spdlog::error("Input Size and Number of SingleParticles Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.SingleParticles().get().Size(); i++) {
                if (static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(i).Mass()) < 0) {
                    spdlog::error("Mass of SingleParticle is negative");
                    return 1;
                }
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
            if (static_cast<size_t>(molecules.Cuboids().get().Size()) != static_cast<size_t>(molecules.Cuboids().get().
                    Cuboid().size())) {
                spdlog::error("Input Size and Number of Cuboids Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.Cuboids().get().Size(); i++) {
                if (static_cast<double >(molecules.Cuboids().get().Cuboid().at(i).Distance()) < 0) {
                    spdlog::error("Distance of Cuboid is negative");
                    return 1;
                }
                if (static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Mass()) < 0) {
                    spdlog::error("Mass of Cuboid is negative");
                    return 1;
                }
                if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).DimensionBrownian()) < 0) {
                    spdlog::error("DimensionBrownian is negative");
                    return 1;

                }
                if (static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Brownian()) < 0) {
                    spdlog::error("Brownian for Cuboid is negative");
                    return 1;
                }
                if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N1()) < 0) {
                    spdlog::error("N1 for cuboid is negative");
                    return 1;
                }
                if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N2()) < 0) {
                    spdlog::error("N2 for cuboid is negative");
                    return 1;
                }
                if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N3()) < 0) {
                    spdlog::error("N3 for cuboid is negative");
                    return 1;
                }
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
            if (static_cast<size_t>(molecules.Discs().get().Size()) != static_cast<size_t>(molecules.Discs().get().
                    Disc().size())) {
                spdlog::error("Input Size and Number of Discs Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.Discs().get().Size(); i++) {
                if (static_cast<int>(molecules.Discs().get().Disc().at(i).Radius()) < 0) {
                    spdlog::error("Discs Radius is negative");
                    return 1;
                }
                if (static_cast<double>(molecules.Discs().get().Disc().at(i).InterParticleDistance()) < 0) {
                    spdlog::error("Discs InterParticleDistance is negative");
                    return 1;
                }
                if (static_cast<double>(molecules.Discs().get().Disc().at(i).Mass()) < 0) {
                    spdlog::error("Discs Mass is negative");
                    return 1;
                }
                if (static_cast<int>(molecules.Discs().get().Disc().at(i).DimensionBrownian()) < 0) {
                    spdlog::error("DimensionBrownian is negative");
                    return 1;
                }
                if (static_cast<double>(molecules.Discs().get().Disc().at(i).Brownian()) < 0) {
                    spdlog::error("Brownian for Disc is negative");
                    return 1;
                }
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
                        static_cast<double>(molecules.Discs().get().Disc().at(i).InterParticleDistance()),
                        static_cast<double>(molecules.Discs().get().Disc().at(i).Mass()),
                        static_cast<int>(molecules.Discs().get().Disc().at(i).DimensionBrownian()),
                        static_cast<double>(molecules.Discs().get().Disc().at(i).Brownian())
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
    } catch (std::exception &e) {
        spdlog::error("Exception message is: \n{}", e.what());
        return 1;
    } catch (...) {
        spdlog::error("Encountered an exception during parsing, Most likely missing Parameter(s).");
        return 1;
    }

    spdlog::info("{}: parse OK", filename);

    delete parser;
    delete errHandler;
    xercesc::XMLPlatformUtils::Terminate();
    return 0;
}
