#include <cmath>
#include "zig_random.h"
#include "ziggurat.h"

Ziggurat::Ziggurat(void){
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

    /*Set probabilites that point is in rectangle part of the box*/
    this->box_prob[0] = (unsigned long)((this->x_r * this->y_c[0]) / this->block_area) * (double)this->max_int;

    for (int itr = 1; itr < this->block_count-1; itr++) {
        this->box_prob[itr] = (unsigned long)((this->x_c[itr + 1] / this->x_c[itr]) * (double)this->max_int);
    }

    this->box_prob[this->block_count - 1] = 0;

    this->rn_src = new Xoshiro512StarStarRandom();
}

double Ziggurat::guassian_unorm_pdf(double x) {
    return std::exp(-(x * x * 0.5));
}

double Ziggurat::guassian_unorm_pdf_inv(double y) {
    return std::sqrt(-2.0 * std::log(y));
}