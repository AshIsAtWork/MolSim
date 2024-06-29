/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>
#include <algorithm>
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"

class Particle {

private:
    /**
     * Id of the particle that is created next.
     */
 static int nextId;

    /**
     * Position of the particle.
     */
    std::array<double, 3> x{};

    /**
     * Velocity of the particle.
     */
    std::array<double, 3> v{};

    /**
     * Force effective on this particle.
     */
    std::array<double, 3> f{};

    /**
     * Force which was effective on this particle.
     */
    std::array<double, 3> old_f{};

    /**
     * Mass of this particle.
     */
    double m{};

    /**
     * Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on).
     */
    int type;

    /**
     * Lennard-Jones-Parameter epsilon.
     */
    double epsilon;

    /**
     * Lennard-Jones-Parameter sigma.
     */

    double sigma;

    /**
     * Mark particles to which a special force should be applied.
     */

    bool marked;

    /**
     * Id to set neighbor relationships between particles.
     */
    int id;

    /**
     * Direct neighbors within a membrane.
     */
    std::vector<int> directNeighbors;

    /**
     * Diagonal neighbors within a membrane.
     */
    std::vector<int> diagonalNeighbors;

    /**
     * Fixed particles belong to a wall. Needed for study nanofluidics.
     */
    bool fixed;

public:

    explicit Particle(int type = 0);

    Particle(const Particle &other);

    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, int type = 0, double epsilon_arg = 5, double sigma_arg = 1);

    /**
     * @brief Set force to oldForce and set force to 0.
     */
    void resetForce();

    /**
     * @brief Calculate the current kinetic energy of the particle.
     *
     * @return kinetic energy of the particle.
     */
    double calculateEKin() const;

    /**
     * @brief Calculate the current kinetic energy of the particle within a flow simulation.
     *
     * @param avgVelocity Average velocity of the particles within the system.
     *
     * @return kinetic energy of the particle.
     */
    double calculateEKinFlow(std::array<double, 3>& avgVelocity) const;

    /**
     * Make particle p a direct neighbor of this particle.
     *
     * @param idToAdd New direct neighbor of this particle.
     */
    void addDirectNeighbor(int idToAdd);

    /**
     * Make particle p a diagonal neighbor of this particle.
     *
     * @param idToAdd New diagonal neighbor of this particle.
     */
    void addDiagonalNeighbor(int idToAdd);

    /**
     * @brief Check, if particle neighbor is a direct neighbor of this particle.
     *
     * @param neighbor Particle to check.
     * @return True, if the specified particle is a direct neighbor. False otherwise.
     */
    bool isDirectNeighbor(Particle& neighbor);

    /**
     * @brief Check, if particle neighbor is a diagonal neighbor of this particle.
     *
     * @param neighbor Particle to check.
     * @return True, if the specified particle is a diagonal neighbor. False otherwise.
     */
    bool isDiagonalNeighbor(Particle& neighbor);

    virtual ~Particle();

    [[nodiscard]] const std::array<double, 3> &getX() const;

    [[nodiscard]] const std::array<double, 3> &getV() const;

    [[nodiscard]] const std::array<double, 3> &getF() const;

    [[nodiscard]] const std::array<double, 3> &getOldF() const;

    [[nodiscard]] double getM() const;

    [[nodiscard]] int getType() const;

    [[nodiscard]] double getEpsilon() const;

    [[nodiscard]] double getSigma() const;

    [[nodiscard]] int getId() const;

    [[nodiscard]] bool isMarked()const;

    [[nodiscard]] std::vector<int> & getDirectNeighbors();

    [[nodiscard]] std::vector<int> & getDiagonalNeighbors();

    [[nodiscard]] bool isFixed() const;

    void setOldF(const std::array<double, 3> &oldF);

    void setF(const std::array<double, 3> &f);

    void setX(const std::array<double, 3> &x);

    void setV(const std::array<double, 3> &v);

    void setType(int type);

    void setMarked(bool status);

    void setFixed(bool status);

    bool operator==(Particle &other) const;

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
