//
// Created by Boris N on 1/22/25.
//

#ifndef COORD_H
#define COORD_H
#include <ostream>

class coord {
public:
    coord(int x, int y): u(x), v(y) {}

    int x() const { return u; }
    int y() const { return v; }

private:
    int u;
    int v;
};

inline std::ostream &operator<<(std::ostream &lhs, const coord &rhs) {
    lhs << "(" << rhs.x() << ", " << rhs.y() << ")";
    return lhs;
}

#endif //COORD_H
