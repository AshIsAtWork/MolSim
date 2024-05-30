//
// Created by Ashutosh Solanki on 22.05.24.
//

#include "XMLReader.h"

//Reference: https://xerces.apache.org/xerces-c/program-dom-3.html
int XMLReader::readFile(ParticleContainer &particles, std::string &filename) {
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
            spdlog::info("OutputFileName: {}", molecules.OutputFileName());
        }

        // static_cast<int> is required because internally the value is stored as a custom type
        if (static_cast<int>(molecules.OutputFrequency() <= 0)) {
            spdlog::error("OutputFrequency is less than 0");
            return 1;
        } else {
            spdlog::info("OutputFrequency: {}", static_cast<int> (molecules.OutputFrequency()));
        }

        if (static_cast<double>(molecules.t_end()) < 0) {
            spdlog::error("t_end is less than 0");
            return 1;
        } else {
            spdlog::info("t_end: {}", static_cast<double>(molecules.t_end()));
        }

        if (static_cast<double>(molecules.delta_t()) <= 0) {
            spdlog::error("delta_t is less than 0");
            return 1;
        } else {
            spdlog::info("delta_t: {}", static_cast<double>(molecules.delta_t()));
        }

        //TODO: Use this force
        TypeOfForce force = setForce(molecules.force());
        if (force == TypeOfForce::invalid) {
            spdlog::error("Force is invalid");
            return 1;
        } else {
            spdlog::info("Force: {}", molecules.force());
        }

        //TODO: Use this Model
        TypeOfModel model = setModel(molecules.model());
        if (model == TypeOfModel::invalid) {
            spdlog::error("Model is invalid");
            return 1;
        } else {
            spdlog::info("Model: {}", molecules.model());
        }
        if (static_cast<double>(molecules.Sigma()) < 0) {
            spdlog::error("Sigma is less than 0");
            return 1;
        } else {
            spdlog::info("Sigma: {}", static_cast<double>(molecules.Sigma()));
        }

        if (static_cast<double>(molecules.Epsilon()) < 0) {
            spdlog::error("Epsilon is less than 0");
            return 1;
        } else {
            spdlog::info("Epsilon: {}", static_cast<double>(molecules.Epsilon()));
        }

        spdlog::info("DomainSize: {}, {}, {}",
                     static_cast<double>(molecules.DomainSize().First()),
                     static_cast<double>(molecules.DomainSize().Second()),
                     static_cast<double>(molecules.DomainSize().Third()));

        if (static_cast<double >(molecules.rCutOff()) < 0) {
            spdlog::error("rCutOff is less than 0");
            return 1;
        } else {
            spdlog::info("rCutOff: {}", static_cast<double>(molecules.rCutOff()));
        }

        spdlog::info("BoundaryCondition: Front: {}, Back: {}, Left: {}, Right: {}, Top: {}, Bottom: {}",
                     molecules.BoundaryCondition().boundaries().Front(),
                     molecules.BoundaryCondition().boundaries().Back(),
                     molecules.BoundaryCondition().boundaries().Left(),
                     molecules.BoundaryCondition().boundaries().Right(),
                     molecules.BoundaryCondition().boundaries().Top(),
                     molecules.BoundaryCondition().boundaries().Bottom());

        if (molecules.SingleParticles().present()) {
            spdlog::info("SingleParticles Size: {}", static_cast<int>(molecules.SingleParticles().get().Size()));
            if (molecules.SingleParticles().get().Size() != molecules.SingleParticles().get().SingleParticle().size()) {
                spdlog::error("Input Size and Number of SingleParticles Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.SingleParticles().get().Size(); i++) {
                spdlog::info("SingleParticle #{}: Position: {}, {}, {}",
                             i + 1 + 1,
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().X()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().Y()),
                             static_cast<double>(molecules.SingleParticles().get().SingleParticle().at(
                                     i).Position().Z()));
                spdlog::info("SingleParticle #{}: Velocity: {}, {}, {}",
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
            spdlog::info("Cuboids Size: {}", static_cast<int>(molecules.Cuboids().get().Size()));
            if (molecules.Cuboids().get().Size() != molecules.Cuboids().get().Cuboid().size()) {
                spdlog::error("Input Size and Number of Cuboids Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.Cuboids().get().Size(); i++) {
                spdlog::info("Cuboid #{}: Position: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().X()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().Y()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Position().Z()));
                spdlog::info("Cuboid #{}: Velocity: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().X()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().Y()),
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(
                                     i).Velocity().Z()));
                spdlog::info("Cuboid #{}: Mass: {}", i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Mass()));
                spdlog::info("Cuboid #{}: N1: {}", i + 1,
                             static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N1()));
                spdlog::info("Cuboid #{}: N2: {}", i + 1,
                             static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N2()));
                spdlog::info("Cuboid #{}: N3: {}", i + 1,
                             static_cast<int>(molecules.Cuboids().get().Cuboid().at(i).N3()));
                spdlog::info("Cuboid #{}: Distance: {}", i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Distance()));
                spdlog::info("Cuboid #{}: Brownian: {}", i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).Brownian()));
                spdlog::info("Cuboid #{}: DimensionBrownian: {}", i + 1,
                             static_cast<double>(molecules.Cuboids().get().Cuboid().at(i).DimensionBrownian()));
            }
        }

        if (molecules.Discs().present()) {
            spdlog::info("Discs Size: {}", static_cast<int>(molecules.Discs().get().Size()));
            if (molecules.Discs().get().Size() != molecules.Discs().get().Disc().size()) {
                spdlog::error("Input Size and Number of Discs Don't Match");
                return 1;
            }
            for (auto i = 0; i < molecules.Discs().get().Size(); i++) {
                spdlog::info("Disc #{}: Center: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().X()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().Y()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Center().Z()));
                spdlog::info("Disc #{}: Velocity: {}, {}, {}",
                             i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().X()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().Y()),
                             static_cast<double>(molecules.Discs().get().Disc().at(
                                     i).Velocity().Z()));
                spdlog::info("Disc #{}: Mass: {}", i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(i).Mass()));
                spdlog::info("InterParticleDistance: {}",
                             static_cast<double>(molecules.Discs().get().Disc().at(i).InterParticleDistance()));
                spdlog::info("Disc #{}: Radius: {}", i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(i).Radius()));
                spdlog::info("Disc #{}: Brownian: {}", i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(i).Brownian()));
                spdlog::info("Disc #{}: DimensionBrownian: {}", i + 1,
                             static_cast<double>(molecules.Discs().get().Disc().at(i).DimensionBrownian()));
            }
        }


        //TODO remove this
//        spdlog::debug("Particles: {}", molecules);
//        spdlog::debug("OutputFileName: {}", molecules.OutputFileName());
//        spdlog::debug("TimeStep: {}", molecules.TimeStep());
//        spdlog::debug("t_end: {}", molecules.t_end());
//        spdlog::debug("delta_t: {}", molecules.delta_t());
//
//        const auto &cuboids = molecules.Cuboid();

//        size_t i = 0;
//        for (const auto &cuboid: cuboids) {
//            std::array<double, 3> x{cuboid.Position().X(), cuboid.Position().Y(), cuboid.Position().Z()};
//            std::array<double, 3> v{cuboid.Velocity().X(), cuboid.Velocity().Y(), cuboid.Velocity().Z()};
//            double m = cuboid.Mass();
//
//            // TODO: Implement new add and add brownian motion
//            Particle newP{x, v, m, static_cast<int>(i)};
//            particles.add(newP);
//
//            spdlog::debug(
//                    "Cuboid Position X: {}\nCuboid Position Y: {}\nCuboid Position Z: {}\n"
//                    "Cuboid N1: {}\nCuboid N2: {}\nCuboid N3: {}\n"
//                    "Cuboid Distance: {}\nCuboid Mass: {}\n"
//                    "Cuboid Velocity X: {}\nCuboid Velocity Y: {}\nCuboid Velocity Z: {}\n"
//                    "Cuboid Brownian: {}",
//                    cuboid.Position().X(), cuboid.Position().Y(), cuboid.Position().Z(),
//                    cuboid.N1(), cuboid.N2(), cuboid.N3(),
//                    cuboid.Distance(), cuboid.Mass(),
//                    cuboid.Velocity().X(), cuboid.Velocity().Y(), cuboid.Velocity().Z(),
//                    cuboid.Brownian()
//            );
//            ++i;
//        }

//        TODO: SAME HERE, COULD BE MORE PERFORMANT BUT USES C++23
//        size_t i = 0;
//        for (const auto& cuboid : cuboids | std::views::transform([](const auto& cuboid) {
//            return std::make_tuple(
//                    cuboid.Position(),
//                    cuboid.Velocity(),
//                    cuboid.Mass(),
//                    cuboid.N1(),
//                    cuboid.N2(),
//                    cuboid.N3(),
//                    cuboid.Distance(),
//                    cuboid.Brownian()
//            );
//        })) {
//            const auto& [position, velocity, m, n1, n2, n3, distance, brownian] = cuboid;
//
//            std::array<double, 3> x{position.X(), position.Y(), position.Z()};
//            std::array<double, 3> v{velocity.X(), velocity.Y(), velocity.Z()};
//
//            // TODO: Implement new add and add brownian motion
//            Particle newP{x, v, m, static_cast<int>(i)};
//            particles.add(newP);
//
//            spdlog::debug(
//                    "Cuboid Position X: {}\nCuboid Position Y: {}\nCuboid Position Z: {}\n"
//                    "Cuboid N1: {}\nCuboid N2: {}\nCuboid N3: {}\n"
//                    "Cuboid Distance: {}\nCuboid Mass: {}\n"
//                    "Cuboid Velocity X: {}\nCuboid Velocity Y: {}\nCuboid Velocity Z: {}\n"
//                    "Cuboid Brownian: {}",
//                    position.X(), position.Y(), position.Z(),
//                    n1, n2, n3,
//                    distance, m,
//                    velocity.X(), velocity.Y(), velocity.Z(),
//                    brownian
//            );
//            ++i;
//        }

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