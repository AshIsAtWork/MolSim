//
// Created by daniel on 24.04.24.
//

#pragma once
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "reader/FileReader.h"

class FileHandler {
private:

    const std::string fileName{"MD_vtk"};

    //write
    outputWriter::VTKWriter vtkWriter;
    outputWriter::XYZWriter xyzWriter;

    //read
    FileReader fileReader;
public:
    enum class outputFormat {vtk, xyz };

    void readFile(ParticleContainer &particles, std::string& filePath);
    void writeToFile(ParticleContainer& particles, int iteration, outputFormat format);
};




