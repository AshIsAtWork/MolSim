/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include <iostream>

int Particle::nextId = 1;

Particle::Particle(int type_arg) : f{0., 0., 0.}, old_f{0., 0., 0.}, type{type_arg}, epsilon{5}, sigma{1} {
    std::stringstream stream;
    id = nextId++;
    marked = false;
    fixed = false;
    spdlog::trace(
        "Particle generated with the following parameters: X={}, v={}, f={}, type={}, epsilon{}, sigma{}",
        ArrayUtils::to_string(x), ArrayUtils::to_string(v), ArrayUtils::to_string(f), type, epsilon, sigma);
}

Particle::Particle(const Particle &other) : x{other.x}, v{other.v}, f{other.f}, old_f{other.old_f}, m{other.m},
                                            type{other.type}, epsilon{other.epsilon}, sigma{other.sigma}, marked{other.marked},
                                            id{other.id}, directNeighbors{other.directNeighbors},
                                            diagonalNeighbors{other.diagonalNeighbors}, fixed{other.fixed} {
    spdlog::trace(
        "Particle generated by copy with the following parameters: X={}, v={}, f={}, type={} epsilon={}, sigma={}",
        ArrayUtils::to_string(x), ArrayUtils::to_string(v), ArrayUtils::to_string(f), type, epsilon, sigma);
}

Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                   double m_arg, int type_arg, double epsilon_arg, double sigma_arg) : x{x_arg}, v{v_arg},
    f{{0, 0, 0}}, old_f{0, 0, 0}, m{m_arg}, type{type_arg}, epsilon{epsilon_arg}, sigma{sigma_arg} {
    marked = false;
    id = nextId++;
    fixed = false;
    spdlog::trace("Particle generated with the following parameters: X={}, v={}, f={}, type={}, epsilon={}, sigma={}",
                  ArrayUtils::to_string(x), ArrayUtils::to_string(v), ArrayUtils::to_string(f), type, epsilon, sigma);
}

void Particle::resetForce() {
    old_f = f;
    f = {0, 0, 0};
}

double Particle::calculateEKin() const {
    return 0.5 * m * (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

double Particle::calculateEKinFlow(std::array<double, 3>& avgVelocity) const {
    auto vHat = v - avgVelocity;
    return 0.5 * m * (vHat[0] * vHat[0] + vHat[1] * vHat[1] + vHat[2] * vHat[2]);
}

void Particle::addDirectNeighbor(int idToAdd) {
    directNeighbors.push_back(idToAdd);
}

void Particle::addDiagonalNeighbor(int idToAdd) {
    diagonalNeighbors.push_back(idToAdd);
}

bool Particle::isDirectNeighbor(Particle &neighbor) {
    //Check, if p points at neighbor, or neighbor points at p
    return std::find(directNeighbors.begin(), directNeighbors.end(), neighbor.getId()) != directNeighbors.end()
           or std::find(neighbor.directNeighbors.begin(), neighbor.directNeighbors.end(), id) != neighbor.
           directNeighbors.end();
}

bool Particle::isDiagonalNeighbor(Particle &neighbor) {
    //Check, if p points at neighbor, or neighbor points at p
    return std::find(diagonalNeighbors.begin(), diagonalNeighbors.end(), neighbor.getId()) != diagonalNeighbors.end()
           or std::find(neighbor.diagonalNeighbors.begin(), neighbor.diagonalNeighbors.end(), id) != neighbor.
           diagonalNeighbors.end();
}


Particle::~Particle() { spdlog::trace("Particle destructed"); }

const std::array<double, 3> &Particle::getX() const { return x; }

const std::array<double, 3> &Particle::getV() const {return v;}

const std::array<double, 3> &Particle::getF() const { return f; }

const std::array<double, 3> &Particle::getOldF() const { return old_f; }

double Particle::getM() const { return m; }

int Particle::getType() const { return type; }

double Particle::getEpsilon() const {
    return epsilon;
}

double Particle::getSigma() const {
    return sigma;
}

int Particle::getId() const {
    return id;
}

bool Particle::isMarked() const {
    return marked;
}

std::vector<int> &Particle::getDirectNeighbors() {
    return directNeighbors;
}

std::vector<int> &Particle::getDiagonalNeighbors() {
    return diagonalNeighbors;
}

bool Particle::isFixed() const {
    return fixed;
}

void Particle::setOldF(const std::array<double, 3> &oldF) {
    old_f = oldF;
}

void Particle::setF(const std::array<double, 3> &f) {
    this->f = f;
}

void Particle::setX(const std::array<double, 3> &x) {
    this->x = x;
}

void Particle::setV(const std::array<double, 3> &v) {
    this->v = v;
}

void Particle::setType(const int type) {
    this->type = type;
}

void Particle::setMarked(bool status) {
    marked = status;
}

void Particle::setFixed(bool status) {
    fixed = status;
}

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X:" << x << " v: " << v << " f: " << f
            << " old_f: " << old_f << " type: " << type << " epsilon: " << epsilon << " sigma: " << sigma;
    return stream.str();
}

bool Particle::operator==(Particle &other) const {
    return (x == other.x) && (v == other.v) && (f == other.f) &&
           (type == other.type) && (m == other.m) && (old_f == other.old_f) &&
           (epsilon == other.epsilon) && (sigma == other.sigma);
}

std::ostream &operator<<(std::ostream &stream, Particle &p) {
    stream << p.toString();
    return stream;
}
