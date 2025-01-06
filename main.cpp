#include <iostream>
#include "Libs/Labirynth/labirynth.h"

int main() {
    Labirynth labirynth(23, 23);
    labirynth.printLabirynth();
    labirynth.solveLabirynth();

    /*labirynth.readLabirynth("labirynth.txt");
    labirynth.printLabirynth();
    labirynth.solveLabirynth();*/

    return 0;
}
