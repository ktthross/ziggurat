
ulong splitmix64rng(ref ulong seed) {
    ulong ssplit = (seed += 0x9E3779B97F4A7C15UL);
    ssplit = (ssplit ^ (ssplit >> 30)) * 0xBF58476D1CE4E5B9UL;
    ssplit = (ssplit ^ (ssplit >> 27)) * 0x94D049BB133111EBUL;
    return ssplit ^ (ssplit >> 31);
}

ulong left_rotate(ulong src, int magnitude) {
    return (src << magnitude) | (src >> (64 - magnitude));
}

class Xoshiro512StarStarRandom {

    private ulong _s0, _s1, _s2, _s3, _s4, _s5, _s6, _s7;
    private const double incr_double = 1.0 / (1UL << 53);

    Xoshiro512StarStarRandom(){
        initialize_state(30984686UL);
    }

    Xoshiro512StarStarRandom(ulong _seed){
        initialize_state(_seed);
    }

    public void initialize_state(ulong _seed){
        this._s0 = splitmix64rng(ref _seed);
        this._s1 = splitmix64rng(ref _seed);
        this._s2 = splitmix64rng(ref _seed);
        this._s3 = splitmix64rng(ref _seed);
        this._s4 = splitmix64rng(ref _seed);
        this._s5 = splitmix64rng(ref _seed);
        this._s6 = splitmix64rng(ref _seed);
        this._s7 = splitmix64rng(ref _seed);
        return;
    }

    public ulong next_ulong(){
        /*Get 64 bits*/
        ulong result = left_rotate(this._s1 * 5UL, 7) * 9UL;

        /*Update the state*/
        ulong tmp = this._s1 << 11;
        this._s2 ^= this._s0;
        this._s5 ^= this._s1;
        this._s1 ^= this._s2;
        this._s7 ^= this._s3;
        this._s3 ^= this._s4;
        this._s4 ^= this._s5;
        this._s0 ^= this._s6;
        this._s6 ^= this._s7;
        this._s6 ^= tmp;
        this._s7 = left_rotate(this._s7, 21);

        return result;
    }

    uint next_uint() {
        /*next 32 random bits*/
        return (uint)this.next_ulong();
    }

    double rand_0I1E() {
        /*Random double between [0, 1)*/
        return this.next_ulong() * this.incr_double;
    }

    double rand_0E1I() {
        /*Random double between (0, 1]*/
        return this.rand_0I1E() + this.incr_double;
    }
}