#pragma once
#include <vector>

#include "private/interpretation.h"
#include <cmath>
namespace eloquent::logic {
    struct interpretation_generator {
        /**
         * Finds out the iteration threshold where the value for a given atom should change
         * @param atc Total atom count
         * @param index Index of current atom (left-right)
         * @return The modulo
         */
        static size_t compute_inflexion_point(const size_t atc, const size_t index) {
            return static_cast<size_t>(std::pow(2, atc - index));
        }

        /**
         *
         * @param atoms The name of the atoms for the table
         * @return The rows of the table
         */
        static std::vector<interpretation> get_interpretations(const std::vector<std::string>& atoms) {
            size_t atc = atoms.size();
            std::vector<interpretation> interpretations;
            const auto steps = static_cast<size_t>(std::pow(2, atc));
            interpretations.reserve(steps);
            std::vector<size_t> thresholds(atc);
            for (size_t i = 0; i < atc; i++) {
                thresholds[i] = compute_inflexion_point(atc,i);
            }
            for (size_t i = 0; i < steps; i++) {
                interpretation interp;

                for (size_t j = 0; j < atc; j++) {
                    interp[atoms[j]] = 1 - i/thresholds[j]%2;
                }
                interpretations[i] = interp;
            }
            return interpretations;
        }

    };
}