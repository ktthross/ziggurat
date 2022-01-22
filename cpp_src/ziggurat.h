#ifndef ZIG_H
#define ZIG_H

class Xoshiro512StarStarRandom;

class Ziggurat {
    private:
        const int block_count = 128;
        const double block_area = 9.91256303526217e-3;
        const double x_r = 3.442619855899;
        const unsigned long max_int = (1UL << 53) - 1;
        const double convert_double = 1.0 / max_int;
        double a_div_y0;
        double x_c[129];
        double y_c[128];
        unsigned long block_prob[128];
        Xoshiro512StarStarRandom* rn_src;

        void prepare_stack(void);
        double guassian_unorm_pdf(double);
        double guassian_unorm_pdf_inv(double);
        double sample_tail(void);

    public:
        Ziggurat();
        Ziggurat(unsigned long);

        double gen_instance(void);
        double gen_instance(double, double);
        double gen_instance_mean(double);
        double gen_instance_stddev(double);

};

#endif