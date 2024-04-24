#include "fileHandling/reader/FileReader.h"
#include "fileHandling/outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>

#include "fileHandling/outputWriter/VTKWriter.h"

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

void plotParticlesVTK(int iteration);

constexpr double start_time = 0;
constexpr double end_time = 1000;
constexpr double delta_t = 0.014;

std::vector<Particle> particles;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 2) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "./molsym filename" << std::endl;
  }

  FileReader fileReader;
  fileReader.readFile(particles, argsv[1]);

  double current_time = start_time;

  int iteration = 0;

  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {
    // calculate new x
    calculateX();
    // calculate new f
    calculateF();
    // calculate new v
    calculateV();

    iteration++;
    if (iteration % 50 == 0) {
      plotParticlesVTK(iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}



void calculateF() {

  for (auto &p_i : particles) {
    p_i.setOldF(p_i.getF());
    p_i.setF({0,0,0});
    for (auto &p_j : particles) {
      if(&p_i != &p_j) {
        double distance = ArrayUtils::L2Norm(p_i.getX() - p_j.getX());
        double scalar = (p_i.getM() * p_j.getM()) / (distance * distance * distance);

        std::array<double,3> f_ij = scalar * (p_j.getX() - p_i.getX());
        p_i.setF(p_i.getF() + f_ij);
      }
    }
  }
}

void calculateX() {
  for (auto &p : particles) {
    std::array<double, 3> xNew = p.getX() + delta_t * p.getV() + ((delta_t * delta_t) / (2.0 * p.getM())) * p.getOldF();
    p.setX(xNew);
  }
}

void calculateV() {
  for (auto &p : particles) {
    std::array<double, 3> vNew = p.getV() + (delta_t / (2 * p.getM())) * (p.getOldF() + p.getF());
    p.setV(vNew);
  }
}

void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::XYZWriter writer;
  writer.plotParticles(particles, out_name, iteration);

}

void plotParticlesVTK(int iteration) {
  std::string out_name("MD_vtk");

  outputWriter::VTKWriter writer;
  writer.initializeOutput(static_cast<int>(particles.size()));
  for(Particle &p : particles) {
    writer.plotParticle(p);
  }
   writer.writeFile(out_name, iteration);
}
