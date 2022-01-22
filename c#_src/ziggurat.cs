using Math;
class Ziggurat{
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

    Ziggurat(){
        this.prepare_stack();
        rn_src = new Xoshiro512StarStarRandom();
    }

    Ziggurat(ulong _seed) {
        this.prepare_stack();
        rn_src = new Xoshiro512StarStarRandom(_seed);
    }

    private void prepare_stack(){
        this.x_c[0] = this.x_r;
        this.y_c[0] = this.gaussian_unorm_pdf(this.x_c[0]);

        this.x_c[1] = this.x_r;
        this.y_c[1] = this.y_c[0] + (this.block_area / this.x_c[1]);

        for(int itr = 2; itr < this.block_count; itr++) {
            this.x_c[itr] = this.guassian_unorm_pdf_inv(this.y_c[itr - 1]);
            this.y_c[itr] = this.y_c[itr - 1] + (this.block_area / this.x_c[itr]);
        }

        /*The box with no area*/
        this.x_c[this.block_count] = 0.0;

        this.a_div_y0 = this.block_area / this.y_c[0];

        this.block_prob[0] = (ulong)(((this.x_r * this.y_c[0]) / this.block_area) * (double)this.max_int);

        for(int itr = 1; itr < this.block_count - 1; itr++) {
            this.block_prob[itr] = (ulong)((this.x_c[itr + 1] / this.x_c[itr]) * (double)this.max_int);
        }

        this.block_prob[this.block_count - 1] = 0UL;
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
            x = Math.Log(this.rn_src.rand_0E1I()) / this.x_r;
            y = -Math.Log(this.rn_src.rand_0E1I());
        } while (y + y < x * x);
        return x + this.x_r;
    }

    public double gen_instance(){
        bool is_set;
        int block_idx;
        double test;
        ulong rnd, sample;
        while (true){
            rnd = this.rn_src.next_ulong();

            block_idx = (int)((rnd >> 3) & 0x7f);

            is_set = true; /*TODO*/

            sample = rnd >> 11;

            if (block_idx == 0) {
                if (sample < this.block_prob[block_idx]) {
                    if(is_set) {
                        return (double)(sample * this.convert_double * this.a_div_y0);
                    } else {
                        return -(double)(sample * this.convert_double * this.a_div_y0);
                    }
                } else {
                    if (is_set) {
                        return this.sample_tail();
                    } else {
                        return -this.sample_tail();
                    }
                }
            }

            if (sample < this.block_prob[block_idx]) {
                if(is_set) {
                    return (double)(sample * this.convert_double * this->x_c[block_idx]);
                } else {
                    return -(double)(sample * this.convert_double * this->x_c[block_idx]);
                }
            }

            test = sample * this.convert_double * this.x_c[block_idx];
            if (this.y_c[block_idx - 1] + (this.y_c[block_idx] - this.y_c[block_idx - 1]) * this.rn_src.rand_0I1E() < this.guassian_unorm_pdf(test)) {
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
}
