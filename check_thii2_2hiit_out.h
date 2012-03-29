#include <fstream>
#include <time.h>

void DoTheFileThing () {

    int n = 0;

    char TheGame[10][81] = {"You Are POOP.",
                            "A beautiful black file who don't need no editor",
                            "help im trapped in a computer factory",
                            "A wise  man once said - \"FCG: SDJS;ALSKJFSA;JK\"",
                            "That's a nice C: drive you have there. It'd be a shame if anythWHOOPS",
                            "Hey, you know what's fun? Uploading all your files to github. What a great site.",
                            "side_view? sounds unimportant, let me just get that off your disk",
                            "if I were you, I wouldn't turn around\n\n\n\nbecause there would be nothing to see",
                            "GIR. GET OFF THAT TACO AND GET BACK ON THIS COMPUTER. OUR TALLEST NEED US",
                            "laliens."
                           };

    using::ofstream YouArePoop ("YouArePoop.~ATH");

    for (n = 0; n < 10; n++)
        YouArePoop << TheGame[ (time (NULL) * n) % 10] << endl;

}
