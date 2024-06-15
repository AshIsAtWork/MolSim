//
// Created by Ashutosh Solanki on 22.05.24.
//

#include "XMLReader.h"

//Reference: https://xerces.apache.org/xerces-c/program-dom-3.html
int XMLReader::readFile(std::string &filename, enumsStructs::SimulationSettings &simulationSettings) {
    try {
        xercesc::XMLPlatformUtils::Initialize();
        {
            xercesc::XercesDOMParser parser;

            parser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
            parser.setDoNamespaces(true);

            auto errHandler = std::make_unique<xercesc::HandlerBase>();
            parser.setErrorHandler(errHandler.get());

            try {
                parser.parse(filename.c_str());
                xercesc::DOMDocument *xmlDoc = parser.getDocument();
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
                } else if (std::any_of(molecules.OutputFileName().begin(), molecules.OutputFileName().end(),
                                       ::isdigit)) {
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

                if (molecules.ThermostatConfig().present()) {
                    spdlog::debug("Thermostat is used");
                    simulationSettings.thermostatParameters.useThermostat = true;
                    if (static_cast<double>(molecules.ThermostatConfig().get().InitialTemperature()) < 0) {
                        spdlog::error("Initial Temperature is less than 0");
                        return 1;
                    } else {
                        simulationSettings.thermostatParameters.initialTemperature = static_cast<double>(molecules.ThermostatConfig().get().InitialTemperature());
                        spdlog::debug("Initial Temperature: {}",
                                      static_cast<double>(molecules.ThermostatConfig().get().InitialTemperature()));
                    }

                    if (static_cast<double>(molecules.ThermostatConfig().get().TargetTemperature()) < 0) {
                        spdlog::error("Target Temperature is less than 0");
                        return 1;
                    } else {
                        simulationSettings.thermostatParameters.targetTemperature = static_cast<double>(molecules.ThermostatConfig().get().TargetTemperature());
                        spdlog::debug("Target Temperature: {}",
                                      static_cast<double>(molecules.ThermostatConfig().get().TargetTemperature()));
                    }


                    if(!molecules.ThermostatConfig().get().InitialBrownian()){
                        simulationSettings.thermostatParameters.initialiseSystemWithBrownianMotion = false;
                        spdlog::debug("Initial Brownian: {}", false);
                    } else {
                        simulationSettings.thermostatParameters.initialiseSystemWithBrownianMotion = true;
                        spdlog::debug("Initial Brownian: {}", true);
                    }

                    if (!molecules.ThermostatConfig().get().ScalingGradually()){
                        simulationSettings.thermostatParameters.applyScalingGradually = false;
                        spdlog::debug("Scaling Gradually: {}", false);
                    } else {
                        simulationSettings.thermostatParameters.applyScalingGradually = true;
                        spdlog::debug("Scaling Gradually: {}", true);
                    }

                    if (static_cast<double>(molecules.ThermostatConfig().get().MaximumTemperatureChange()) < 0) {
                        spdlog::error("Maximum Temperature Change is less than 0");
                        return 1;
                    } else {
                        simulationSettings.thermostatParameters.maxTemperatureChange = static_cast<double>(molecules.ThermostatConfig().get().MaximumTemperatureChange());
                        spdlog::debug("Maximum Temperature Change: {}",
                                      static_cast<double>(molecules.ThermostatConfig().get().MaximumTemperatureChange()));
                    }

                    if (static_cast<double>(molecules.ThermostatConfig().get().ApplyAfterHowManySteps()) < 0) {
                        spdlog::error("Apply After How Many Steps is less than 0");
                        return 1;
                    } else {
                        simulationSettings.thermostatParameters.applyAfterHowManySteps = static_cast<int>(molecules.ThermostatConfig().get().ApplyAfterHowManySteps());
                        spdlog::debug("Apply After How Many Steps: {}",
                                      static_cast<int>(molecules.ThermostatConfig().get().ApplyAfterHowManySteps()));
                    }

                    if (static_cast<double>(molecules.ThermostatConfig().get().Dimension()) < 0) {
                        spdlog::error("Dimension is less than 0");
                        return 1;
                    } else {
                        simulationSettings.thermostatParameters.dimensions = static_cast<int>(molecules.ThermostatConfig().get().Dimension());
                        spdlog::debug("Dimension: {}", static_cast<int>(molecules.ThermostatConfig().get().Dimension()));
                    }
                } else {
                    simulationSettings.thermostatParameters.useThermostat = false;
                    spdlog::debug("Thermostat was set to false");
                }

                if (molecules.GravityConfig().present()) {
                        simulationSettings.gravityOn = true;
                        simulationSettings.gravityFactor = static_cast<double>(molecules.GravityConfig().get().GravitationFactor());
                        spdlog::debug("Gravity: {}",static_cast<double>(molecules.GravityConfig().get().GravitationFactor()));
                } else {
                    simulationSettings.gravityOn = false;
                    spdlog::debug("Gravity was set to false");
                }


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
                        simulationSettings.parametersLinkedCells.boundaryConditions.front = enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Front());
                        simulationSettings.parametersLinkedCells.boundaryConditions.right = enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Right());
                        simulationSettings.parametersLinkedCells.boundaryConditions.back = enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Back());
                        simulationSettings.parametersLinkedCells.boundaryConditions.left = enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Left());
                        simulationSettings.parametersLinkedCells.boundaryConditions.top = enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Top());
                        simulationSettings.parametersLinkedCells.boundaryConditions.bottom = enumsStructs::setBoundaryCondition(
                                molecules.model().BoundaryCondition().get().boundaries().Bottom());

                        spdlog::debug(
                                "BoundaryCondition: Front: {}, Back: {}, Left: {}, Right: {}, Top: {}, Bottom: {}",
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
                    spdlog::debug("SingleParticles Size: {}",
                                  static_cast<int>(molecules.SingleParticles().get().Size()));
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

                        double epsilon = 5;
                        double sigma = 1;
                        if (molecules.SingleParticles().get().SingleParticle().at(i).Epsilon().present()) {
                            epsilon = static_cast<double >(molecules.SingleParticles().get().SingleParticle().at(
                                    i).Epsilon().get());
                        }
                        if (molecules.SingleParticles().get().SingleParticle().at(i).Sigma().present()) {
                            sigma = static_cast<double >(molecules.SingleParticles().get().SingleParticle().at(
                                    i).Sigma().get());
                        }
                        enumsStructs::ParticleType p{
                                {
                                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                                i).Position().X()),
                                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                                i).Position().Y()),
                                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                                i).Position().Z()),
                                },
                                {
                                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                                i).Velocity().X()),
                                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                                i).Velocity().Y()),
                                        static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                                i).Velocity().Z())
                                },
                                static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(i).Mass()),
                                epsilon,
                                sigma
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
                        spdlog::debug("SingleParticle #{}: Mass: {}", i + 1 + 1,
                                      static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                              i).Mass()));
                        spdlog::debug("SingleParticle #{}: Epsilon: {}", i + 1 + 1, epsilon);
                        spdlog::debug("SingleParticle #{}: Sigma: {}", i + 1 + 1, sigma);
                    }
                }

                if (molecules.Cuboids().present()) {
                    spdlog::debug("Cuboids Size: {}", static_cast<int>(molecules.Cuboids().get().Size()));
                    if (static_cast<size_t>(molecules.Cuboids().get().Size()) !=
                        static_cast<size_t>(molecules.Cuboids().get().
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

                        double epsilon = 5;
                        double sigma = 1;
                        if (molecules.Cuboids().get().Cuboid().at(i).Epsilon().present()) {
                            epsilon = static_cast<double >(molecules.Cuboids().get().Cuboid().at(
                                    i).Epsilon().get());
                        }
                        if (molecules.Cuboids().get().Cuboid().at(i).Sigma().present()) {
                            sigma = static_cast<double >(molecules.Cuboids().get().Cuboid().at(
                                    i).Sigma().get());
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
                                static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Brownian()),
                                epsilon,
                                sigma
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
                        spdlog::debug("Cuboid #{}: Epsilon: {}", i + 1,
                                      epsilon);
                        spdlog::debug("Cuboid #{}: Sigma: {}", i + 1,
                                      sigma);
                    }
                }

                if (molecules.Discs().present()) {
                    spdlog::debug("Discs Size: {}", static_cast<int>(molecules.Discs().get().Size()));
                    if (static_cast<size_t>(molecules.Discs().get().Size()) !=
                        static_cast<size_t>(molecules.Discs().get().
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

                        double epsilon = 5;
                        double sigma = 1;
                        if (molecules.Discs().get().Disc().at(i).Epsilon().present()) {
                            epsilon = static_cast<double >(molecules.Cuboids().get().Cuboid().at(
                                    i).Epsilon().get());
                        }
                        if (molecules.Discs().get().Disc().at(i).Sigma().present()) {
                            sigma = static_cast<double >(molecules.Cuboids().get().Cuboid().at(
                                    i).Sigma().get());
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
                                static_cast<double>(molecules.Discs().get().Disc().at(i).Brownian()),
                                epsilon,
                                sigma
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
                                      static_cast<double>(molecules.Discs().get().Disc().at(
                                              i).InterParticleDistance()));
                        spdlog::debug("Disc #{}: Radius: {}", i + 1,
                                      static_cast<int>(molecules.Discs().get().Disc().at(i).Radius()));
                        spdlog::debug("Disc #{}: Brownian: {}", i + 1,
                                      static_cast<int>(molecules.Discs().get().Disc().at(i).Brownian()));
                        spdlog::debug("Disc #{}: DimensionBrownian: {}", i + 1,
                                      static_cast<double>(molecules.Discs().get().Disc().at(i).DimensionBrownian()));
                        spdlog::debug("Disc #{}: Epsilon: {}", i + 1, epsilon);
                        spdlog::debug("Disc #{}: Sigma: {}", i + 1, sigma);
                    }
                }

                if (molecules.Spheres().present()) {
                    spdlog::debug("Spheres Size: {}", static_cast<int>(molecules.Spheres().get().Size()));
                    if (static_cast<size_t>(molecules.Spheres().get().Size()) !=
                        static_cast<size_t>(molecules.Spheres().get().
                                Sphere().size())) {
                        spdlog::error("Input Size and Number of Spheres Don't Match");
                        return 1;
                    }
                    for (auto i = 0; i < molecules.Spheres().get().Size(); i++) {

                        if (static_cast<double>(molecules.Spheres().get().Sphere().at(i).Mass()) < 0) {
                            spdlog::error("Spheres Mass is negative");
                            return 1;
                        }
                        if (static_cast<double>(molecules.Spheres().get().Sphere().at(i).InterParticleDistance()) < 0) {
                            spdlog::error("Spheres InterParticleDistance is negative");
                            return 1;
                        }
                        if (static_cast<int>(molecules.Spheres().get().Sphere().at(i).Radius()) < 0) {
                            spdlog::error("Spheres Radius is negative");
                            return 1;
                        }
                        if (static_cast<double>(molecules.Spheres().get().Sphere().at(i).Brownian()) < 0) {
                            spdlog::error("Brownian for Sphere is negative");
                            return 1;
                        }
                        if (static_cast<int>(molecules.Spheres().get().Sphere().at(i).DimensionBrownian()) < 0) {
                            spdlog::error("DimensionBrownian is negative");
                            return 1;
                        }

                        double epsilon = 5;
                        double sigma = 1;
                        if (molecules.Spheres().get().Sphere().at(i).Epsilon().present()) {
                            epsilon = static_cast<double >(molecules.Spheres().get().Sphere().at(
                                    i).Epsilon().get());
                        }
                        if (molecules.Spheres().get().Sphere().at(i).Sigma().present()) {
                            sigma = static_cast<double >(molecules.Spheres().get().Sphere().at(
                                    i).Sigma().get());
                        }

                        enumsStructs::Sphere sphere = {
                                {
                                        static_cast<double>(molecules.Spheres().get().Sphere().at(
                                                i).Center().X()),
                                        static_cast<double>(molecules.Spheres().get().Sphere().at(
                                                i).Center().Y()),
                                        static_cast<double>(molecules.Spheres().get().Sphere().at(
                                                i).Center().Z()),
                                },
                                {
                                        static_cast<double>(molecules.Spheres().get().Sphere().at(
                                                i).Velocity().X()),
                                        static_cast<double>(molecules.Spheres().get().Sphere().at(
                                                i).Velocity().Y()),
                                        static_cast<double>(molecules.Spheres().get().Sphere().at(
                                                i).Velocity().Z())
                                },
                                static_cast<int>(molecules.Spheres().get().Sphere().at(i).Radius()),
                                static_cast<double>(molecules.Spheres().get().Sphere().at(i).InterParticleDistance()),
                                static_cast<double>(molecules.Spheres().get().Sphere().at(i).Mass()),
                                static_cast<int>(molecules.Spheres().get().Sphere().at(i).DimensionBrownian()),
                                static_cast<double>(molecules.Spheres().get().Sphere().at(i).Brownian()),
                                epsilon,
                                sigma
                        };
                        simulationSettings.spheres.push_back(sphere);
                        spdlog::debug("Sphere #{}: Center: {}, {}, {}",
                                      i + 1,
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).Center().X()),
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).Center().Y()),
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).Center().Z()));
                        spdlog::debug("Sphere #{}: Velocity: {}, {}, {}",
                                      i + 1,
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).Velocity().X()),
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).Velocity().Y()),
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).Velocity().Z()));
                        spdlog::debug("Sphere #{}: Mass: {}", i + 1,
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(i).Mass()));
                        spdlog::debug("InterParticleDistance: {}",
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).InterParticleDistance()));
                        spdlog::debug("Sphere #{}: Radius: {}", i + 1,
                                      static_cast<int>(molecules.Spheres().get().Sphere().at(i).Radius()));
                        spdlog::debug("Sphere #{}: Brownian: {}", i + 1,
                                      static_cast<int>(molecules.Spheres().get().Sphere().at(i).Brownian()));
                        spdlog::debug("Sphere #{}: DimensionBrownian: {}", i + 1,
                                      static_cast<double>(molecules.Spheres().get().Sphere().at(
                                              i).DimensionBrownian()));
                        spdlog::debug("Sphere #{}: Epsilon: {}", i + 1, epsilon);
                        spdlog::debug("Sphere #{}: Sigma: {}", i + 1, sigma);
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
        }

    } catch (const xercesc::XMLException &toCatch) {
        char *message = xercesc::XMLString::transcode(toCatch.getMessage());
        spdlog::error("Error during initialization! :\n{}", message);
        xercesc::XMLString::release(&message);
        return 1;
    }
    spdlog::info("{}: parse OK", filename);
    xercesc::XMLPlatformUtils::Terminate();
    return 0;
}
