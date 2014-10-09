#pragma once
#include "Fields.cpp"

class ECPoint
{
public:
    ECPoint(int field);
    ~ECPoint();

    typedef Field<unsigned int> Field;

    static void add(const ECPoint& augend,
                    const ECPoint& addend,
                          ECPoint& sum);

    static void pointDouble(ECPoint& point);

    static void multiply(const ECPoint& multiplicand,
                               Field&   scale,
                               ECPoint& product);

    bool operator==(const ECPoint& cmp) const;

    void O();

    Field x;
    Field y;

};