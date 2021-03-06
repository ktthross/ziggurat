#include "zig_random.h"

unsigned long splitmix64rng(unsigned long &seed) {
    unsigned long ssplit = (seed += 0x9E3779B97F4A7C15UL);
    ssplit = (ssplit ^ (ssplit >> 30)) * 0xBF58476D1CE4E5B9UL;
    ssplit = (ssplit ^ (ssplit >> 27)) * 0x94D049BB133111EBUL;
    return ssplit ^ (ssplit >> 31);
}

unsigned long left_rotate(unsigned long src, int magnitude) {
    return (src << magnitude) | (src >> (64 - magnitude));
}

Xoshiro512StarStarRandom::Xoshiro512StarStarRandom() {
    this->initialize_state(30984686UL);
}

Xoshiro512StarStarRandom::Xoshiro512StarStarRandom(unsigned long seed) {
    this->initialize_state(seed);
}

void Xoshiro512StarStarRandom::initialize_state(unsigned long seed){
    /*hash the seed to set the intial state*/
    this->_s0 = splitmix64rng(seed);
    this->_s1 = splitmix64rng(seed);
    this->_s2 = splitmix64rng(seed);
    this->_s3 = splitmix64rng(seed);
    this->_s4 = splitmix64rng(seed);
    this->_s5 = splitmix64rng(seed);
    this->_s6 = splitmix64rng(seed);
    this->_s7 = splitmix64rng(seed);
    return; 
};

unsigned long Xoshiro512StarStarRandom::next_ulong(void) {
    /*new set of 64 random bits*/
    unsigned long result = left_rotate(this->_s1 * 5UL, 7) * 9UL;

    /*Update the state*/
    unsigned long tmp = this->_s1 << 11;
    this->_s2 ^= this->_s0;
    this->_s5 ^= this->_s1;
    this->_s1 ^= this->_s2;
    this->_s7 ^= this->_s3;
    this->_s3 ^= this->_s4;
    this->_s4 ^= this->_s5;
    this->_s0 ^= this->_s6;
    this->_s6 ^= this->_s7;
    this->_s6 ^= tmp;
    this->_s7 = left_rotate(this->_s7, 21);

    return result;
};

unsigned int Xoshiro512StarStarRandom::next_uint(void) {
    /*New set of 32 random bits*/
    return (unsigned int)this->next_ulong();
}

double Xoshiro512StarStarRandom::rand_0I1E(void) {
    /*Return a number between [0, 1)*/
    return this->next_ulong() * this->incr_double;
}

double Xoshiro512StarStarRandom::rand_0E1I(void) {
    /*Return a number between (0, 1]*/
    return this->rand_0I1E() + this->incr_double;
}