#ifndef ZIG_H
#define ZIG_H

class Xoshiro512StarStarRandom;

class Ziggurat {
    private:
        const int block_count = 512;
        const double block_area = 2.4567663515413529e-3;
        const double x_r = 3.8520461503683916;
        double x_c[513];
        double y_c[512];
        unsigned long box_prob[512]; // Probability the point is in the rectangular part of the box
        const unsigned long max_int = (1UL << 53) - 1;
        Xoshiro512StarStarRandom* rn_src;

        double guassian_unorm_pdf(double);
        double guassian_unorm_pdf_inv(double);

};

#endif