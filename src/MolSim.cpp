#include "fileHandling/reader/FileReader.h"
#include "fileHandling/outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"
#include "fileHandling/outputWriter/VTKWriter.h"

#include <iostream>
#include <boost/program_options.hpp>




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
void plotParticlesXYZ(int iteration);

void plotParticlesVTK(int iteration);

std::vector<Particle> particles;

double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;
std::string inputFilePath;

int main(int argc, char *argsv[]) {

  namespace po = boost::program_options;


  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Provides some help.")
    ("t_end,e", po::value<double>(&end_time)->default_value(1000),"Time to which simulation will run (starting at 0).")
    ("delta_t,d",po::value<double>(&delta_t)->default_value(0.014),"Duration of one time step.")
    ("input_file_path,f", po::value<std::string>(&inputFilePath),"Path to the input file. Caution: This "
    "is a required argument. In case it is not specified, the program will terminate immediately.");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc,argsv,desc),vm);
  }
  catch(boost::wrapexcept<po::unknown_option> &e) {
    std::cout << "Something went wrong while parsing your arguments: "<< e.what()
    << "\nPlease have a look on the usage!\n" << std::endl;

    std::cout << desc << "\n";
    return -1;
  }
  catch(...) {
    std::cout << "Something went wrong while parsing your arguments. Please have a look on the usage!\n" << std::endl;
    std::cout << desc << "\n";
    return -1;
  }

  po::notify(vm);

  std::cout << "Hello from MolSim for PSE!" << std::endl;

  if (argc <= 1 || vm.count("help")) {
    std::cout << desc << "\n";
    return -1;
  }

  if(!vm.count(inputFilePath)) {
    std::cout << "Please specify a path to your input file!" << "\n";
    std::cout << desc << "\n";
    return -1;
  }

  FileReader fileReader;
  fileReader.readFile(particles, inputFilePath);

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

void plotParticlesXYZ(int iteration) {

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
