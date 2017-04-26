#ifndef ROTULO
#define ROTULO

class Rotulo {
    public:
        int id;
        int vezes_utilizado;
        bool operator < (const Rotulo& r) const
        {
            return (vezes_utilizado < r.vezes_utilizado);
        }
        bool operator > (const Rotulo& r) const
        {
            return (vezes_utilizado > r.vezes_utilizado);
        }
};

#endif