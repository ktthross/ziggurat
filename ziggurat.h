#ifndef ZIG_H
#define ZIG_H

class Xoshiro512StarStarRandom;

class Ziggurat {
    private:
        const int block_count = 512;
        const double block_area = 2.4567663515413529e-3;
        const double x_r = 3.8520461503683916;
        const unsigned long max_int = (1UL << 53) - 1;
        const double convert_double = 1.0 / max_int;
        double a_div_y0;

        double x_c[513];
        double y_c[512];
        unsigned long box_prob[512]; // Probability the point is in the rectangular part of the box
        Xoshiro512StarStarRandom* rn_src;

        void prepare_stack(void);
        double guassian_unorm_pdf(double);
        double guassian_unorm_pdf_inv(double);
        double sample_tail(void);
        void set_sign_bit(double&, unsigned long&);

    public:
        Ziggurat();
        Ziggurat(unsigned long);

        double gen_instance(void);

};

#endif