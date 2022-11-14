#include <iostream>
#include <immintrin.h>
#include "includes/Metrix.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;


int main() {
    int N = 1024;

    Metrix metrixA = Metrix(N, N, 1, 100, true);
    Metrix metrixB = Metrix(N, N, 1, 100, true);

    auto start = chrono::steady_clock::now();

    Metrix metrixC = metrixA * metrixB;

    auto stop = chrono::steady_clock::now();
    auto diff = stop - start;
    cout << "Metrix multiply time before using AVX: " << chrono::duration<double, nano>(diff).count() << " ns" << endl;

    start = chrono::steady_clock::now();

    Metrix metrixC_AVX = metrixA.multiplayUsingAvx(metrixB);

    stop = chrono::steady_clock::now();
    diff = stop - start;
    cout << "Metrix multiply time after using AVX: " << chrono::duration<double, nano>(diff).count() << " ns" << endl;

    if (metrixC == metrixC_AVX) {
        cout << "metrix multiply result before using AVX equals it after using AVX!" << endl;
    }
    return 0;
}