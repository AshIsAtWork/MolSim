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
        spdlog::debug("Particles: {}", molecules.Particles());
        spdlog::debug("OutputFileName: {}", molecules.OutputFileName());
        spdlog::debug("TimeStep: {}", molecules.TimeStep());
        spdlog::debug("t_end: {}", molecules.t_end());
        spdlog::debug("delta_t: {}", molecules.delta_t());

        const auto &cuboids = molecules.Cuboid();

        size_t i = 0;
        for (const auto &cuboid: cuboids) {
            std::array<double, 3> x{cuboid.Position().X(), cuboid.Position().Y(), cuboid.Position().Z()};
            std::array<double, 3> v{cuboid.Velocity().X(), cuboid.Velocity().Y(), cuboid.Velocity().Z()};
            double m = cuboid.Mass();

            // TODO: Implement new add and add brownian motion
            Particle newP{x, v, m, static_cast<int>(i)};
            particles.add(newP);

            spdlog::debug(
                    "Cuboid Position X: {}\nCuboid Position Y: {}\nCuboid Position Z: {}\n"
                    "Cuboid N1: {}\nCuboid N2: {}\nCuboid N3: {}\n"
                    "Cuboid Distance: {}\nCuboid Mass: {}\n"
                    "Cuboid Velocity X: {}\nCuboid Velocity Y: {}\nCuboid Velocity Z: {}\n"
                    "Cuboid Brownian: {}",
                    cuboid.Position().X(), cuboid.Position().Y(), cuboid.Position().Z(),
                    cuboid.N1(), cuboid.N2(), cuboid.N3(),
                    cuboid.Distance(), cuboid.Mass(),
                    cuboid.Velocity().X(), cuboid.Velocity().Y(), cuboid.Velocity().Z(),
                    cuboid.Brownian()
            );
            ++i;
        }

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