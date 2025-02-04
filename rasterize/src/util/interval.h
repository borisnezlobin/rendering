//
// Created by Boris N on 1/20/25.
//

#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    double min;
    double max;

    interval(const double min, const double max) : min { min }, max { max } {}

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
};

#endif //INTERVAL_H
