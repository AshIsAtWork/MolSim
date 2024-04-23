#include "fileHandling/reader/FileReader.h"
#include "fileHandling/outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>

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

constexpr double start_time = 0;
constexpr double end_time = 1000;
constexpr double delta_t = 0.014;

// TODO: what data structure to pick?
//TODO: Change to std::vector
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
    if (iteration % 10 == 0) {
      plotParticles(iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

void calculateF() {
  std::vector<Particle>::iterator iterator;
  iterator = particles.begin();

  for (auto &p_i : particles) {
    p_i.setOld_f(p_i.getF());
    std::array<double, 3> f_i{{0,0,0}};
    for (auto &p_j : particles) {
      if(&p_i != &p_j) {
        //scalar here (m_i*m_j) / (|x_i - x_j|_2³)
        double scalar = (p_i.getM() * p_j.getM()) / pow( (ArrayUtils::L2Norm(p_i.getX() - p_j.getX())),3.0);

        std::array<double,3> f_ij{scalar * (p_i.getX() - p_j.getX())};
        f_i = f_i + f_ij;
      }
    }
    p_i.setF(f_i);
  }
}

void calculateX() {
  for (auto &p : particles) {
    p.setX(p.getX() + delta_t * p.getV() + ((delta_t * delta_t) / (2.0 * p.getM())) * p.getOldF());
  }
}

void calculateV() {
  for (auto &p : particles) {
    p.setV(p.getV() + (delta_t / (2 * p.getM())) * (p.getOldF()) + p.getF());
  }
}

void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::XYZWriter writer;
  writer.plotParticles(particles, out_name, iteration);
}
