//
// Created by Ashutosh Solanki on 23.05.24.
//

#include <fstream>
#include "XMLWriter.h"

void XMLWriter::writeToFile(std::string &filename, int OutputFrequency, double t_end, double delta_t,
                        enumsStructs::TypeOfForce &forceType,
                        enumsStructs::TypeOfModel &modelType, double Sigma, double Epsilon,
                        std::array<int, 3> DomainSize,
                        double rCutOff,
                        std::array<std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition>, 6> BoundaryCondition) {

// Obtain the object model.
//
    auto modelObject = model(enumsStructs::getModel(modelType), t_end, delta_t, enumsStructs::getForce(forceType),
                             Sigma, Epsilon);
    auto moleculesObject = new Molecules(filename, OutputFrequency, modelObject);
    //TODO: Set Disc and other stuff


//    std::string temp = "abc";
//    enumsStructs::TypeOfForce tempForce = enumsStructs::TypeOfForce::leonardJonesForce;
//    enumsStructs::TypeOfModel tempModel = enumsStructs::TypeOfModel::directSum;
//    XMLWriter::writeToFile(temp,
//                           1,
//                           0.1,
//                           0.1,
//                           tempForce,
//                           tempModel,
//                           0.1,
//                           0.1,
//                           {1,1,1},
//                           0.1,
//                           {cFront, cRight, cBack, cLeft, cTop,
//                            cBottom});

//
    xml_schema::namespace_infomap map;
    map[""].schema = "ConfigurationFile.xsd";
    std::ofstream ofs("../../src/fileHandling/reader/XMLHandling/ConfigurationFile.xsd");
    Molecules_(ofs, *moleculesObject, map);

}
