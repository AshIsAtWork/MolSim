//
// Created by daniel on 24.04.24.
//

#pragma once
#include "fileHandling/outputWriter/VTKWriter/VTKWriter.h"
#include "fileHandling/outputWriter/XYZWriter/XYZWriter.h"
#include "particleRepresentation/container/ParticleContainer.h"
#include "fileHandling/reader/TxtReader/TxtReader.h"

/**
 * @brief Wrapper class for file handling.
 *
 * The main goal of this class is to abstract the file handling to make it convenient
 * in the future to add more input and output methods.
 *
 */

class FileHandler {
private:

    outputWriter::VTKWriter vtkWriter;
    outputWriter::XYZWriter xyzWriter;

public:
    /**
     * @brief Supported output formats.
     *
     * This enum class enables the user to select the desired output format in the writeToFile method.
     */
    enum class outputFormat { vtk, xyz, xml, invalid };

    /**
     * @brief Supported input formats.
     *
     * This enum class enables the user to select the desired input format.
     */
    enum class inputFormat { txt, xml, invalid };

    /**
     * @brief Read particles from a txt-file.
     *
     * @param particles Particle container in which the newly read-in particles will be stored.
     * @param filePath File path to the input txt-file of the particles to be read.
     * @param format Type of the input file.
     *
     * Supported txt file formats at the moment: Particle, Cuboid
     */
    static void readFile(ParticleContainer &particles, std::string &filePath, inputFormat format);

    /**
     * @brief Write particles to a file.
     *
     * @param particles Particles which will be written to the file.
     * @param iteration Current iteration step of the simulation.
     * @param format Type of the output file.
     * @param baseName Base name of the output file.
     *
     * Write particles to a file. You can choose between different output formats. The file will be created in the directory,
     * in which this program was executed.
     */
    void writeToFile(ParticleContainer &particles, int iteration, outputFormat format, std::string& baseName);
};
