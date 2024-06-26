/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"

class Particle {
private:
    /**
     * Position of the particle
     */
    std::array<double, 3> x{};

    /**
     * Velocity of the particle
     */
    std::array<double, 3> v{};

    /**
     * Force effective on this particle
     */
    std::array<double, 3> f{};

    /**
     * Force which was effective on this particle
     */
    std::array<double, 3> old_f{};

    /**
     * Mass of this particle
     */
    double m{};

    /**
     * Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on)
     */
    int type;

public:
    explicit Particle(int type = 0);

    Particle(const Particle &other);

    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
        int type = 0);

    void resetForce();

    virtual ~Particle();

    [[nodiscard]] const std::array<double, 3> &getX() const;

    [[nodiscard]] const std::array<double, 3> &getV() const;

    [[nodiscard]] const std::array<double, 3> &getF() const;

    [[nodiscard]] const std::array<double, 3> &getOldF() const;

    [[nodiscard]] double getM() const;

    [[nodiscard]] int getType() const;

    void setOldF(const std::array<double, 3> &oldF);

    void setF(const std::array<double, 3> &f);

    void setX(const std::array<double, 3> &x);

    void setV(const std::array<double, 3> &v);

    void setType(const int type);

    bool operator==(Particle &other);

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
