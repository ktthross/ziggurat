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
    this->y_c[0] = guassian_unorm_pdf(this->x_c[0]);

    this->x_c[1] = this->x_r;
    this->y_c[1] = this->y_c[0] + (this->block_area / this->x_c[1]);

    for(int itr = 2; itr < this->block_count; itr++) {
        this->x_c[itr] = guassian_unorm_pdf_inv(this->y_c[itr - 1]);
        this->y_c[itr] = this->y_c[itr - 1] + (this->block_area / this->x_c[itr]);
    }

    /*The box with no area*/
    this->x_c[this->block_count] = 0.0;
    this->a_div_y0 = this->block_area / this->y_c[0];

    /*Set probabilites that point is in rectangle part of the box*/
    this->box_prob[0] = (unsigned long)((this->x_r * this->y_c[0]) / this->block_area) * (double)this->max_int;

    for (int itr = 1; itr < this->block_count-1; itr++) {
        this->box_prob[itr] = (unsigned long)((this->x_c[itr + 1] / this->x_c[itr]) * (double)this->max_int);
    }

    this->box_prob[this->block_count - 1] = 0;

}

double Ziggurat::gen_instance(void) {
    double sample;
    while (true)
    {
        /*Get random bits to work with*/
        unsigned long rnd = this->rn_src->next_ulong();

        /*Select a block number between 0 and 511 to sample*/
        int block_idx = rnd & 0x1ff;

        /*Select the sign bit*/
        bool is_set = sign_bit(rnd).test(10);

        /*Generate random 53 bit number*/
        unsigned long uni_rand = rnd >> 11;

        /*Handle the base segment*/
        if (block_idx == 0) {
            if(uni_rand < this->box_prob[0]) {
                /*Generated number is in the block*/
                sample = uni_rand * this->convert_double * this->a_div_y0;
            } else {
                /*Generated number is in the tail*/
                sample = this->sample_tail();
            }
            /*Set the sign bit*/
            if(!is_set){sample = -sample;}
            return sample;
        }

        /*Handle other segments*/
        if (uni_rand < this->box_prob[block_idx]) {
            /*number is inside the rectangle*/
            sample = uni_rand * this->convert_double * this->x_c[block_idx];
            if(!is_set){sample = -sample;}
            return sample;
        }

        sample = uni_rand * this->convert_double * this->x_c[block_idx];
        if (this->y_c[block_idx - 1] + ((this->y_c[block_idx] - this->y_c[block_idx - 1]) * this->rn_src->next_double()) < guassian_unorm_pdf(sample)) {
            if(!is_set){sample = -sample;}
            return sample;
        }
    }
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
        x = -std::log(this->rn_src->rand_0E1I()) / this->x_r;
        y = -std::log(this->rn_src->rand_0E1I());
    } while (y + y < x * x);
    return x + this->x_r;
}

void Ziggurat::set_sign_bit(double &x, unsigned long &sign_bit) {
    x != sign_bit;
}