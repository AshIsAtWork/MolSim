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
                    throw std::runtime_error("Empty document at time of parsing XML");
                }

                // Initialize the XML schema flags and container
                xml_schema::flags flags = xml_schema::flags::dont_initialize;
                xml_schema::container *container = nullptr;

                // Correctly create the object using the generated constructor
                Molecules molecules(*elementRoot, flags, container);

                // Accessing elements
                if (molecules.OutputFileName().empty()) {
                    throw std::runtime_error("OutputFileName is empty");
                } else if (std::any_of(molecules.OutputFileName().begin(), molecules.OutputFileName().end(),
                                       ::isdigit)) {
                    throw std::runtime_error("OutputFileName contains a number");
                } else {
                    simulationSettings.outputFileName = static_cast<std::string>(molecules.OutputFileName());
                    spdlog::debug("OutputFileName: {}", molecules.OutputFileName());
                }

                // static_cast<int> is required because internally the value is stored as a custom type
                if (static_cast<int>(molecules.OutputFrequency() <= 0)) {
                    throw std::invalid_argument("OutputFrequency is less than 0");
                } else {
                    simulationSettings.outputFrequency = static_cast<int>(molecules.OutputFrequency());
                    spdlog::debug("OutputFrequency: {}", static_cast<int>(molecules.OutputFrequency()));
                }

                if (molecules.ThermostatConfig().present()) {
                    spdlog::debug("Thermostat is used");

                    simulationSettings.thermostatParameters.typeOfThermostat = enumsStructs::setTypeOfThermostat(molecules.ThermostatConfig().get().Type());
                    spdlog::debug("Type of thermostat: {}", molecules.ThermostatConfig().get().Type());

                    simulationSettings.thermostatParameters.useThermostat = true;
                    if (static_cast<double>(molecules.ThermostatConfig().get().InitialTemperature()) < 0) {
                        throw std::runtime_error("Initial Temperature is less than 0");
                    } else {
                        simulationSettings.thermostatParameters.initialTemperature = static_cast<double>(molecules.ThermostatConfig().get().InitialTemperature());
                        spdlog::debug("Initial Temperature: {}",
                                      static_cast<double>(molecules.ThermostatConfig().get().InitialTemperature()));
                    }

                    if (static_cast<double>(molecules.ThermostatConfig().get().TargetTemperature()) < 0) {
                        throw std::runtime_error("Target Temperature is less than 0");
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
                    if(simulationSettings.thermostatParameters.applyScalingGradually) {
                        if(!molecules.ThermostatConfig().get().MaximumTemperatureChange().present()) {
                            throw std::runtime_error("Maximum temperature change must be specified, if scaling is applied gradually!");
                        }else {
                            simulationSettings.thermostatParameters.maxTemperatureChange =
                                static_cast<double>(molecules.ThermostatConfig().get().MaximumTemperatureChange().get());
                            spdlog::debug("Maximum Temperature Change: {}",
                                     static_cast<double>(molecules.ThermostatConfig().get().MaximumTemperatureChange().get()));
                        }
                    }

                    if (static_cast<double>(molecules.ThermostatConfig().get().ApplyAfterHowManySteps()) < 0) {
                        throw std::runtime_error("Apply After How Many Steps is less than 0");
                    } else {
                        simulationSettings.thermostatParameters.applyAfterHowManySteps = static_cast<int>(molecules.ThermostatConfig().get().ApplyAfterHowManySteps());
                        spdlog::debug("Apply After How Many Steps: {}",
                                      static_cast<int>(molecules.ThermostatConfig().get().ApplyAfterHowManySteps()));
                    }

                    if (static_cast<double>(molecules.ThermostatConfig().get().Dimension()) < 0) {
                        throw std::runtime_error("Dimension is less than 0");
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
                        simulationSettings.gravityVector =
                            {
                            static_cast<double>(molecules.GravityConfig().get().X()),
                            static_cast<double>(molecules.GravityConfig().get().Y()),
                            static_cast<double>(molecules.GravityConfig().get().Z())
                            };
                        spdlog::debug("Gravity: {}",ArrayUtils::to_string(simulationSettings.gravityVector));
                } else {
                    simulationSettings.gravityOn = false;
                    spdlog::debug("Gravity was set to false");
                }


                enumsStructs::TypeOfModel model = enumsStructs::setModel(molecules.model().Name());
                if (model == enumsStructs::TypeOfModel::invalid) {
                    throw std::runtime_error("Model is invalid");
                } else {
                    simulationSettings.model = model;
                    spdlog::debug("Model: {}", molecules.model().Name());
                }

                if (static_cast<double>(molecules.model().t_end()) < 0) {
                    throw std::runtime_error("t_end is less than 0");
                } else {
                    simulationSettings.parametersDirectSum.endT = static_cast<double>(molecules.model().t_end());
                    simulationSettings.parametersLinkedCells.endT = static_cast<double>(molecules.model().t_end());
                    spdlog::debug("t_end: {}", static_cast<double>(molecules.model().t_end()));
                }

                if (static_cast<double>(molecules.model().delta_t()) <= 0) {
                    throw std::runtime_error("delta_t is less than 0");
                } else {
                    simulationSettings.parametersDirectSum.deltaT = static_cast<double>(molecules.model().delta_t());
                    simulationSettings.parametersLinkedCells.deltaT = static_cast<double>(molecules.model().delta_t());
                    spdlog::debug("delta_t: {}", static_cast<double>(molecules.model().delta_t()));
                }

                enumsStructs::TypeOfForce force = enumsStructs::setForce(molecules.model().force());
                if (force == enumsStructs::TypeOfForce::invalid) {
                    throw std::runtime_error("Force is invalid");
                } else {
                    simulationSettings.parametersDirectSum.force = force;
                    simulationSettings.parametersLinkedCells.force = force;
                    spdlog::debug("Force: {}", molecules.model().force());
                }

                if (molecules.model().Name() == "LinkedCells") {
                    if (molecules.model().DomainSize().present()) {
                        if (static_cast<double>(molecules.model().DomainSize().get().First()) < 0) {
                            throw std::runtime_error("Domain Size First is negative");
                        }
                        if (static_cast<double>(molecules.model().DomainSize().get().Second()) < 0) {
                            throw std::runtime_error("Domain Size Second is negative");
                        }
                        if (static_cast<double>(molecules.model().DomainSize().get().Third()) < 0) {
                            throw std::runtime_error("Domain Size Third is negative");
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
                        throw std::runtime_error("DomainSize is not present");
                    }
                    if (molecules.model().rCutOff().present()) {
                        if (static_cast<double>(molecules.model().rCutOff().get()) < 0) {
                            throw std::runtime_error("rCutOff is less than 0");
                        } else {
                            simulationSettings.parametersLinkedCells.rCutOff = static_cast<double>(molecules.model().rCutOff().
                                    get());
                            spdlog::debug("rCutOff: {}", static_cast<double>(molecules.model().rCutOff().get()));
                        }
                    } else {
                        throw std::runtime_error("rCutOff is not present");
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
                        throw std::runtime_error("BoundaryCondition is not present");
                    }
                }

                if (molecules.SingleParticles().present()) {
                    spdlog::debug("SingleParticles Size: {}",
                                  static_cast<int>(molecules.SingleParticles().get().Size()));
                    if (static_cast<size_t>(molecules.SingleParticles().get().Size()) != static_cast<size_t>(molecules.
                            SingleParticles().get().SingleParticle().size())) {
                        throw std::runtime_error("Input Size and Number of SingleParticles Don't Match");
                    }
                    for (auto i = 0; i < molecules.SingleParticles().get().Size(); i++) {
                        if (static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(i).Mass()) < 0) {
                            throw std::runtime_error("Mass of SingleParticle is negative");
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
                        throw std::runtime_error("Input Size and Number of Cuboids Don't Match");
                    }
                    for (auto i = 0; i < molecules.Cuboids().get().Size(); i++) {
                        if (static_cast<double >(molecules.Cuboids().get().Cuboid().at(i).Distance()) < 0) {
                            throw std::runtime_error("Distance of Cuboid is negative");
                        }
                        if (static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Mass()) < 0) {
                            throw std::runtime_error("Mass of Cuboid is negative");
                        }
                        if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).DimensionBrownian()) < 0) {
                            throw std::runtime_error("DimensionBrownian is negative");

                        }
                        if (static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Brownian()) < 0) {
                            throw std::invalid_argument("Brownian for Cuboid is negative");
                        }
                        if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N1()) < 0) {
                            throw std::runtime_error("N1 for cuboid is negative");
                        }
                        if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N2()) < 0) {
                            throw std::runtime_error("N2 for cuboid is negative");
                        }
                        if (static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N3()) < 0) {
                            throw std::runtime_error("N3 for cuboid is negative");
                        }

                        double epsilon = 5;
                        double sigma = 1;
                        bool fixed = false;
                        if (molecules.Cuboids().get().Cuboid().at(i).Epsilon().present()) {
                            epsilon = static_cast<double >(molecules.Cuboids().get().Cuboid().at(
                                    i).Epsilon().get());
                        }
                        if (molecules.Cuboids().get().Cuboid().at(i).Sigma().present()) {
                            sigma = static_cast<double >(molecules.Cuboids().get().Cuboid().at(
                                    i).Sigma().get());
                        }

                        if (molecules.Cuboids().get().Cuboid().at(i).Fixed().present()) {
                            fixed = molecules.Cuboids().get().Cuboid().at(i).Fixed();
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
                                sigma,
                                fixed
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
                        spdlog::debug("Cuboid #{}: Fixed: {}", i + 1, fixed);
                    }
                }

                if (molecules.Discs().present()) {
                    spdlog::debug("Discs Size: {}", static_cast<int>(molecules.Discs().get().Size()));
                    if (static_cast<size_t>(molecules.Discs().get().Size()) !=
                        static_cast<size_t>(molecules.Discs().get().
                                Disc().size())) {
                        throw std::runtime_error("Input Size and Number of Discs Don't Match");
                    }
                    for (auto i = 0; i < molecules.Discs().get().Size(); i++) {
                        if (static_cast<int>(molecules.Discs().get().Disc().at(i).Radius()) < 0) {
                            throw std::runtime_error("Discs Radius is negative");
                        }
                        if (static_cast<double>(molecules.Discs().get().Disc().at(i).InterParticleDistance()) < 0) {
                            throw std::runtime_error("Discs InterParticleDistance is negative");
                        }
                        if (static_cast<double>(molecules.Discs().get().Disc().at(i).Mass()) < 0) {
                            throw std::runtime_error("Discs Mass is negative");
                        }
                        if (static_cast<int>(molecules.Discs().get().Disc().at(i).DimensionBrownian()) < 0) {
                            throw std::runtime_error("DimensionBrownian is negative");
                        }
                        if (static_cast<double>(molecules.Discs().get().Disc().at(i).Brownian()) < 0) {
                            throw std::invalid_argument("Brownian for Disc is negative");
                        }

                        double epsilon = 5;
                        double sigma = 1;
                        bool fixed = false;
                        if (molecules.Discs().get().Disc().at(i).Epsilon().present()) {
                            epsilon = static_cast<double >(molecules.Discs().get().Disc().at(
                                    i).Epsilon().get());
                        }
                        if (molecules.Discs().get().Disc().at(i).Sigma().present()) {
                            sigma = static_cast<double >(molecules.Discs().get().Disc().at(
                                    i).Sigma().get());
                        }

                        if (molecules.Discs().get().Disc().at(i).Fixed().present()) {
                            fixed = molecules.Discs().get().Disc().at(i).Fixed();
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
                                sigma,
                                fixed
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
                        spdlog::debug("Disc #{}: Fixed: {}", i + 1, fixed);
                    }
                }

                if (molecules.Spheres().present()) {
                    spdlog::debug("Spheres Size: {}", static_cast<int>(molecules.Spheres().get().Size()));
                    if (static_cast<size_t>(molecules.Spheres().get().Size()) !=
                        static_cast<size_t>(molecules.Spheres().get().
                                Sphere().size())) {
                        throw std::runtime_error("Input Size and Number of Spheres Don't Match");
                    }
                    for (auto i = 0; i < molecules.Spheres().get().Size(); i++) {

                        if (static_cast<double>(molecules.Spheres().get().Sphere().at(i).Mass()) < 0) {
                            throw std::runtime_error("Spheres Mass is negative");
                        }
                        if (static_cast<double>(molecules.Spheres().get().Sphere().at(i).InterParticleDistance()) < 0) {
                            throw std::runtime_error("Spheres InterParticleDistance is negative");
                        }
                        if (static_cast<int>(molecules.Spheres().get().Sphere().at(i).Radius()) < 0) {
                            throw std::runtime_error("Spheres Radius is negative");
                        }
                        if (static_cast<double>(molecules.Spheres().get().Sphere().at(i).Brownian()) < 0) {
                            throw std::invalid_argument("Brownian for Sphere is negative");
                        }
                        if (static_cast<int>(molecules.Spheres().get().Sphere().at(i).DimensionBrownian()) < 0) {
                            throw std::invalid_argument("DimensionBrownian is negative");
                        }

                        double epsilon = 5;
                        double sigma = 1;
                        bool fixed = false;
                        if (molecules.Spheres().get().Sphere().at(i).Epsilon().present()) {
                            epsilon = static_cast<double >(molecules.Spheres().get().Sphere().at(
                                    i).Epsilon().get());
                        }
                        if (molecules.Spheres().get().Sphere().at(i).Sigma().present()) {
                            sigma = static_cast<double >(molecules.Spheres().get().Sphere().at(
                                    i).Sigma().get());
                        }

                        if (molecules.Spheres().get().Sphere().at(i).Fixed().present()) {
                            fixed = molecules.Spheres().get().Sphere().at(i).Fixed();
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
                                sigma,
                                fixed
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
                        spdlog::debug("Sphere #{}: Fixed: {}", i + 1, fixed);
                    }
                }

                if (molecules.Membrane().present()) {
                    spdlog::debug("Membrane is present");
                    simulationSettings.membraneParameters.membraneSetting = true;

                    simulationSettings.membraneParameters.pull = molecules.Membrane().get().Pull();
                    spdlog::debug("Pull: {}",simulationSettings.membraneParameters.pull);
                    if (molecules.Membrane().get().PullingActiveUntil().present()) {
                        if(simulationSettings.membraneParameters.pull) {
                            simulationSettings.membraneParameters.pullingActiveUntil = static_cast<int>(molecules.Membrane().get().PullingActiveUntil().get());
                            spdlog::debug("Pulling active until: {}",simulationSettings.membraneParameters.pullingActiveUntil);
                        }else {
                            throw std::runtime_error("Pull is not active and pullingActiveUntil was specified!");
                        }
                    }
                    else if(simulationSettings.membraneParameters.pull) {
                        throw std::runtime_error("PullingActiveUntil is required, when pull is activated");
                    }

                    if (molecules.Membrane().get().PullingForce().present()) {
                        if(simulationSettings.membraneParameters.pull) {
                            simulationSettings.membraneParameters.pullingForce[0] = static_cast<double>(molecules.Membrane().get().PullingForce().get().X());
                            simulationSettings.membraneParameters.pullingForce[1] = static_cast<double>(molecules.Membrane().get().PullingForce().get().Y());
                            simulationSettings.membraneParameters.pullingForce[2] = static_cast<double>(molecules.Membrane().get().PullingForce().get().Z());
                            spdlog::debug("Pulling force: {}",ArrayUtils::to_string(simulationSettings.membraneParameters.pullingForce));
                        }else {
                            throw std::runtime_error("Pull is not active and pullingForce was specified!");
                        }
                    }
                    else if(simulationSettings.membraneParameters.pull) {
                        throw std::runtime_error("PullingForce is required, when pull is activated");
                    }

                    simulationSettings.membraneParameters.r0 = static_cast<double>(molecules.Membrane().get().r0());
                    spdlog::debug("r0: {}", simulationSettings.membraneParameters.r0);
                    simulationSettings.membraneParameters.k = static_cast<double>(molecules.Membrane().get().k());
                    spdlog::debug("k: {}", simulationSettings.membraneParameters.k);

                    simulationSettings.membraneParameters.initialVelocity[0] = static_cast<double>(molecules.Membrane().get().Velocity().X());
                    simulationSettings.membraneParameters.initialVelocity[1] = static_cast<double>(molecules.Membrane().get().Velocity().Y());
                    simulationSettings.membraneParameters.initialVelocity[2] = static_cast<double>(molecules.Membrane().get().Velocity().Z());
                    spdlog::debug("Inital Velocity: {}",ArrayUtils::to_string(simulationSettings.membraneParameters.initialVelocity));

                    simulationSettings.membraneParameters.position[0] = static_cast<double>(molecules.Membrane().get().Position().X());
                    simulationSettings.membraneParameters.position[1] = static_cast<double>(molecules.Membrane().get().Position().Y());
                    simulationSettings.membraneParameters.position[2] = static_cast<double>(molecules.Membrane().get().Position().Z());
                    spdlog::debug("Position: {}",ArrayUtils::to_string(simulationSettings.membraneParameters.position));

                    simulationSettings.membraneParameters.N1 = static_cast<unsigned>(molecules.Membrane().get().N1());
                    spdlog::debug("N1: {}", simulationSettings.membraneParameters.N1);
                    simulationSettings.membraneParameters.N2 = static_cast<unsigned>(molecules.Membrane().get().N2());
                    spdlog::debug("N2: {}", simulationSettings.membraneParameters.N2);
                    simulationSettings.membraneParameters.mass = static_cast<double>(molecules.Membrane().get().Mass());
                    spdlog::debug("Mass: {}", simulationSettings.membraneParameters.mass);
                    simulationSettings.membraneParameters.h = static_cast<double>(molecules.Membrane().get().InterParticleDistance());
                    spdlog::debug("h: {}", simulationSettings.membraneParameters.h);
                    simulationSettings.membraneParameters.epsilon = static_cast<double>(molecules.Membrane().get().Epsilon());
                    spdlog::debug("epsilon: {}", simulationSettings.membraneParameters.epsilon);
                    simulationSettings.membraneParameters.sigma = static_cast<double>(molecules.Membrane().get().Sigma());
                    spdlog::debug("sigma: {}", simulationSettings.membraneParameters.sigma);

                }
                else {
                simulationSettings.membraneParameters.membraneSetting = false;
                spdlog::debug("Membrane is not present");
                }

            } catch (const xercesc::XMLException &toCatch) {
                char *message = xercesc::XMLString::transcode(toCatch.getMessage());
                std::string error(message);
                spdlog::error("Exception message is: {}", message);
                xercesc::XMLString::release(&message);
                throw std::runtime_error("Encountered Error during XML parsing: " + error);
            } catch (const xercesc::DOMException &toCatch) {
                char *message = xercesc::XMLString::transcode(toCatch.msg);
                std::string error(message);
                xercesc::XMLString::release(&message);
                throw std::runtime_error("Encountered Error during XML parsing: " + error);
            } catch (std::exception &e) {
                throw std::runtime_error("Exception message is: " + std::string(e.what()));
            } catch (...) {
                throw std::runtime_error("Encountered an exception during parsing, Most likely missing Parameter(s).");
            }
        }

    } catch (const xercesc::XMLException &toCatch) {
        char *message = xercesc::XMLString::transcode(toCatch.getMessage());
        std::string error(message);
        xercesc::XMLString::release(&message);
        throw std::runtime_error("Encountered Error during XML parsing: " + error);
    }
    spdlog::info("{}: parse OK", filename);
    xercesc::XMLPlatformUtils::Terminate();
    return 0;
}
