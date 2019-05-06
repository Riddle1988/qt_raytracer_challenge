#ifndef VECTOR_H
#define VECTOR_H

#include "tuple.h"
#include "point.h"

class Vector : public Tuple
{
public:
    Vector(double x, double y, double z);
    Vector(const Tuple& tuple) : Tuple(tuple) {}

    double magnitude() const;
    Vector normalize() const;
    double dot(const Vector& b) const;
    Vector cross(const Vector& b) const;
};

#endif // VECTOR_H