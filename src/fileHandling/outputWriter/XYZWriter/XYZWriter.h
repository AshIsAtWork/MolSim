/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 *      Adapted by daniel
 */

#pragma once

#include <fstream>
#include <iomanip>
#include <sstream>

#include "particleRepresentation/container/defaultParticleContainer/DefaultParticleContainer.h"

namespace outputWriter {

class XYZWriter {

public:
  XYZWriter();

  virtual ~XYZWriter();

  void plotParticles(ParticleContainer& particles, const std::string &filename,
                     int iteration);
};

} // namespace outputWriter
