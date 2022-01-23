using ziggurat;

class ZigHandler {
    static public Main() {
        int sections = 10;
        double[] count = new double[sections];
        double generated;
        int samples = 10000;
        Ziggurat zig = new Ziggurat();
        printf("\n\n\n\n\n");
        printf("          Entering Ziggurat distribution generator\n");

        for (int itr = 0; itr < sections; itr++) {
            count[itr] = 0.0;
        }

        for (int itr = 0; itr < samples; itr++) {
            generated = zig.gen_instance(0.0, 1.0);
            if (generated < -4.0) {
              count[0] += 1.0;
            } else if (generated < -3.0) {
              count[1] += 1.0;
            } else if (generated < -2.0) {
              count[2] += 1.0;
            } else if (generated < -1.0) {
              count[3] += 1.0;
            } else if (generated < 0.0) {
              count[4] += 1.0;
            } else if (generated < 1.0) {
              count[5] += 1.0;
            } else if (generated < 2.0) {
              count[6] += 1.0;
            } else if (generated < 3.0) {
              count[7] += 1.0;
            } else if (generated < 4.0) {
              count[8] += 1.0;
            } else {
              count[9] += 1.0;
            }
        }

        for (int itr = 0; itr < sections; itr++) {
          count[itr] /= samples;
        }

        Console.WriteLine("          Sampled " + samples + " points\n");
        for (int itr = 0; itr < sections; itr++) {
          if (itr == 0) {
            Console.WriteLine("           <-4         " + count[itr] + "\n");
          } else if (itr == 1) { 
            Console.WriteLine("            -4         " + count[itr] + "\n");
          } else if (itr == 2) { 
            Console.WriteLine("            -3         " + count[itr] + "\n");
          } else if (itr == 3) { 
            Console.WriteLine("            -2         " + count[itr] + "\n");
          } else if (itr == 4) { 
            Console.WriteLine("            -1         " + count[itr] + "\n");
          } else if (itr == 5) { 
            Console.WriteLine("            +1         " + count[itr] + "\n");
          } else if (itr == 6) { 
            Console.WriteLine("            +2         " + count[itr] + "\n");
          } else if (itr == 7) { 
            Console.WriteLine("            +3         " + count[itr] + "\n");
          } else if (itr == 8) { 
            Console.WriteLine("            +4         " + count[itr] + "\n");
          } else if (itr == 9) { 
            Console.WriteLine("           >+4         " + count[itr] + "\n");
          }
        }
        Console.WriteLine("\n\n\n\n\n");
    }
}