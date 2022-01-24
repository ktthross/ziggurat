using System;
public class Ziggurat{
    private const int block_count = 128;
    private const double block_area = 9.91256303526217e-3;
    private const double x_r = 3.442619855899;
    private const ulong max_int = (1UL << 53) - 1;
    private const double convert_double = 1.0 / max_int;
    private double a_div_y0;
    private double[] x_c = new double[129];
    private double[] y_c = new double[128];
    private ulong[] block_prob = new ulong[128];
    private Xoshiro512StarStarRandom rn_src;

    public Ziggurat(){
        prepare_stack();
        rn_src = new Xoshiro512StarStarRandom();
    }

    public Ziggurat(ulong _seed) {
        prepare_stack();
        rn_src = new Xoshiro512StarStarRandom(_seed);
    }

    private void prepare_stack(){
        x_c[0] = x_r;
        y_c[0] = gaussian_unorm_pdf(x_c[0]);

        x_c[1] = x_r;
        y_c[1] = y_c[0] + (block_area / x_c[1]);

        for(int itr = 2; itr < block_count; itr++) {
            x_c[itr] = gaussian_unorm_pdf_inv(y_c[itr - 1]);
            y_c[itr] = y_c[itr - 1] + (block_area / x_c[itr]);
        }

        /*The box with no area*/
        x_c[block_count] = 0.0;

        a_div_y0 = block_area / y_c[0];

        block_prob[0] = (ulong)(((x_r * y_c[0]) / block_area) * (double)max_int);

        for(int itr = 1; itr < block_count - 1; itr++) {
            block_prob[itr] = (ulong)((x_c[itr + 1] / x_c[itr]) * (double)max_int);
        }

        block_prob[block_count - 1] = 0UL;
    }

    private double gaussian_unorm_pdf(double x){
        return Math.Exp(-(x * x * -0.5));
    }

    private double gaussian_unorm_pdf_inv(double y){
        return Math.Sqrt(-2.0 * Math.Log(y));
    }

    private double sample_tail() {
        double y, x;
        do {
            x = Math.Log(rn_src.rand_0E1I()) / x_r;
            y = -Math.Log(rn_src.rand_0E1I());
        } while (y + y < x * x);
        return x + x_r;
    }

    public double gen_instance(){
        bool is_set;
        int block_idx;
        double test;
        ulong rnd, sample;
        while (true){
            rnd = rn_src.next_ulong();

            block_idx = (int)((rnd >> 3) & 0x7f);

            is_set = is_bit_set(rnd, 10);

            sample = rnd >> 11;

            if (block_idx == 0) {
                if (sample < block_prob[block_idx]) {
                    if(is_set) {
                        return (double)(sample * convert_double * a_div_y0);
                    } else {
                        return -(double)(sample * convert_double * a_div_y0);
                    }
                } else {
                    if (is_set) {
                        return sample_tail();
                    } else {
                        return -sample_tail();
                    }
                }
            }

            if (sample < block_prob[block_idx]) {
                if(is_set) {
                    return (double)(sample * convert_double * x_c[block_idx]);
                } else {
                    return -(double)(sample * convert_double * x_c[block_idx]);
                }
            }

            test = sample * convert_double * x_c[block_idx];
            if (y_c[block_idx - 1] + (y_c[block_idx] - y_c[block_idx - 1]) * rn_src.rand_0I1E() < gaussian_unorm_pdf(test)) {
                if(is_set) {
                    return test;
                } else {
                    return -test;
                }
            }
        }
    }

    public double gen_instance(double mean, double stddev) {
        return this.gen_instance() * stddev + mean;
    }

    public double gen_instance_mean(double mean) {
        return this.gen_instance() + mean;
    }

    public double gen_instance_stddev(double stddev) {
        return this.gen_instance() * stddev;
    }

    private bool is_bit_set(ulong source, int position) {
        return (source & (1UL << position)) != 0;
    }
}
