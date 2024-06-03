//
// Created by Ashutosh Solanki on 23.05.24.
//

#include "particleRepresentation/container/ParticleContainer.h"
#include "fileHandling/reader/XMLHandling/ConfigurationFile.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "utils/enumsStructs.h"

class XMLWriter {

public:

    static void writeToFile(std::string &filename, int OutputFrequency, double t_end, double delta_t,
                            enumsStructs::TypeOfForce &forceType,
                            enumsStructs::TypeOfModel &modelType, double Sigma, double Epsilon,
                            std::array<int, 3> DomainSize,
                            double rCutOff,
                            std::array<std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition>, 6> BoundaryCondition);

};
