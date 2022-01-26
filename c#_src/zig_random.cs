public class Utils {
    public static ulong splitmix64rng(ref ulong seed) {
        ulong ssplit = (seed += 0x9E3779B97F4A7C15UL);
        ssplit = (ssplit ^ (ssplit >> 30)) * 0xBF58476D1CE4E5B9UL;
        ssplit = (ssplit ^ (ssplit >> 27)) * 0x94D049BB133111EBUL;
        return ssplit ^ (ssplit >> 31);
    }

    public static ulong left_rotate(ulong src, int magnitude) {
        return (src << magnitude) | (src >> (64 - magnitude));
    }
}

public class Xoshiro512StarStarRandom {
    private ulong _s0, _s1, _s2, _s3, _s4, _s5, _s6, _s7;
    private const double incr_double = 1.0 / (1UL << 53);

    public Xoshiro512StarStarRandom(){
        initialize_state(30984686UL);
    }

    public Xoshiro512StarStarRandom(ulong _seed){
        initialize_state(_seed);
    }

    public void initialize_state(ulong _seed){
        _s0 = Utils.splitmix64rng(ref _seed);
        _s1 = Utils.splitmix64rng(ref _seed);
        _s2 = Utils.splitmix64rng(ref _seed);
        _s3 = Utils.splitmix64rng(ref _seed);
        _s4 = Utils.splitmix64rng(ref _seed);
        _s5 = Utils.splitmix64rng(ref _seed);
        _s6 = Utils.splitmix64rng(ref _seed);
        _s7 = Utils.splitmix64rng(ref _seed);
    }

    public ulong next_ulong(){
        /*Get 64 bits*/
        ulong result = Utils.left_rotate(_s1 * 5UL, 7) * 9UL;

        /*Update the state*/
        ulong tmp = _s1 << 11;
        _s2 ^= _s0;
        _s5 ^= _s1;
        _s1 ^= _s2;
        _s7 ^= _s3;
        _s3 ^= _s4;
        _s4 ^= _s5;
        _s0 ^= _s6;
        _s6 ^= _s7;
        _s6 ^= tmp;
        _s7 = Utils.left_rotate(_s7, 21);

        return result;
    }

    public uint next_uint() {
        /*next 32 random bits*/
        return (uint)next_ulong();
    }

    public double rand_0I1E() {
        /*Random double between [0, 1)*/
        return (double)next_ulong() * Xoshiro512StarStarRandom.incr_double;
    }

    public double rand_0E1I() {
        /*Random double between (0, 1]*/
        return rand_0I1E() + Xoshiro512StarStarRandom.incr_double;
    }
}