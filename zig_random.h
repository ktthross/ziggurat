#ifndef ZRAND_H
#define ZRAND_H

unsigned long splitmix64rng(unsigned long &);
unsigned long left_rotate(unsigned long, unsigned long);

class Xoshiro512StarStarRandom {
    private:
        unsigned long _s0, _s1, _s2, _s3, _s4, _s5, _s6, _s7;

    public:
        void initialize_state(unsigned long);
        unsigned long next_ulong(void);
};

#endif