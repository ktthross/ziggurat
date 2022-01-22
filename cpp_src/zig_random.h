#ifndef ZRAND_H
#define ZRAND_H

unsigned long splitmix64rng(unsigned long &);
unsigned long left_rotate(unsigned long, int);

class Xoshiro512StarStarRandom {
    private:
        unsigned long _s0, _s1, _s2, _s3, _s4, _s5, _s6, _s7;
        const double incr_double = 1.0 / (1UL << 53);

    public:
        Xoshiro512StarStarRandom(void);
        Xoshiro512StarStarRandom(unsigned long);
        void initialize_state(unsigned long);
        unsigned long next_ulong(void);
        unsigned int next_uint(void);
        double rand_0E1I(void);
        double rand_0I1E(void);
};

#endif