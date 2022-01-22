#include <bitset>
#include <cmath>
#include "zig_random.h"
#include "ziggurat.h"

typedef std::bitset<64> sign_bit;

Ziggurat::Ziggurat(void){
    prepare_stack();
    this->rn_src = new Xoshiro512StarStarRandom();
}

Ziggurat::Ziggurat(unsigned long seed){
    prepare_stack();
    this->rn_src = new Xoshiro512StarStarRandom(seed);
}

void Ziggurat::prepare_stack(void) {
    /*Set the coordinate corners of the boxes*/
    this->x_c[0] = this->x_r;
    this->y_c[0] = this->guassian_unorm_pdf(this->x_c[0]);

    this->x_c[1] = this->x_r;
    this->y_c[1] = this->y_c[0] + (this->block_area / this->x_c[1]);

    for(int itr = 2; itr < this->block_count; itr++) {
        this->x_c[itr] = this->guassian_unorm_pdf_inv(this->y_c[itr - 1]);
        this->y_c[itr] = this->y_c[itr - 1] + (this->block_area / this->x_c[itr]);
    }

    /*The box with no area*/
    this->x_c[this->block_count] = 0.0;

    this->a_div_y0 = this->block_area / this->y_c[0];

    this->block_prob[0] = (unsigned long)(((this->x_r * this->y_c[0]) / this->block_area) * (double)this->max_int);

    for(int itr = 1; itr < this->block_count - 1; itr++) {
        this->block_prob[itr] = (unsigned long)((this->x_c[itr + 1] / this->x_c[itr]) * (double)this->max_int);
    }

    this->block_prob[this->block_count - 1] = 0UL;
}

double Ziggurat::gen_instance(void) {
    bool is_set;
    int block_idx;
    double test;
    unsigned long rnd, sample;
    while (true)
    {
        /*Get random bits to work with. Eventually we can grab 53 bits for the random number*/
        rnd = this->rn_src->next_ulong();

        /*Select a block number between 0 and 128 to sample*/
        block_idx = (int)((rnd >> 3) & 0x7f);

        is_set = sign_bit(rnd).test(10);

        sample = rnd >> 11;

        if (block_idx == 0) {
            if (sample < this->block_prob[block_idx]) {
                if(is_set) {
                    return (double)(sample * this->convert_double * this->a_div_y0);
                } else {
                    return -(double)(sample * this->convert_double * this->a_div_y0);
                }
            } else {
                if(is_set) {
                    return this->sample_tail();
                } else {
                    return -this->sample_tail();
                }
            }
        }

        if (sample < this->block_prob[block_idx]) {
            if(is_set) {
                return (double)(sample * this->convert_double * this->x_c[block_idx]);
            } else {
                return -(double)(sample * this->convert_double * this->x_c[block_idx]);
            }
        }

        test = sample * this->convert_double * this->x_c[block_idx];
        if (this->y_c[block_idx - 1] + (this->y_c[block_idx] - this->y_c[block_idx - 1]) * this->rn_src->rand_0I1E() < this->guassian_unorm_pdf(test)) {
            if(is_set) {
                return test;
            } else {
                return -test;
            }
        }
    }
}

double Ziggurat::gen_instance(double mean, double stddev) {
    return this->gen_instance() * stddev + mean;
}

double Ziggurat::gen_instance_mean(double mean) {
    return this->gen_instance() + mean;
}

double Ziggurat::gen_instance_stddev(double stddev) {
    return this->gen_instance() * stddev;
}

double Ziggurat::guassian_unorm_pdf(double x) {
    return std::exp(-(x * x * 0.5));
}

double Ziggurat::guassian_unorm_pdf_inv(double y) {
    return std::sqrt(-2.0 * std::log(y));
}

double Ziggurat::sample_tail(void) {
    double y, x;
    do {
        x = std::log(this->rn_src->rand_0E1I()) / this->x_r;
        y = -std::log(this->rn_src->rand_0E1I());
    } while (y + y < x * x);
    return x + this->x_r;
}
