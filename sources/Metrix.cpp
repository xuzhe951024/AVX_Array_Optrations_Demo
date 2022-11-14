//
// Created by Zhe Xu on 11/13/22.
//

#include "../includes/Metrix.hpp"
#include <random>
#include <iostream>
#include <immintrin.h>

using namespace std;
random_device rd;
mt19937 gen(rd());

Metrix::Metrix(int wide, int height, int randomLowerLimit, int randomHigherLimit, bool buildArray)
        : wide(wide), height(height), randomLowerLimit(randomLowerLimit), randomHigherLimit(randomHigherLimit){
    this->wide = wide;
    this->height = height;
    this->randomHigherLimit = randomHigherLimit;
    this->randomLowerLimit = randomLowerLimit;
    if(buildArray){
        build2DArray();
        fill2DArrayWithRandomValue();
    }
}

void Metrix::build2DArray() {
    this->result = new int*[this->height];
    for (int i = 0; i < this->height; ++i) {
        result[i] = new int[this->wide];
    }
}

void Metrix::fill2DArrayWithRandomValue() {
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->wide; ++j) {
            std::uniform_int_distribution<> dist(this->randomLowerLimit, this->randomHigherLimit);
            this->result[i][j] = dist(gen);
        }
    }
}

int Metrix::getWide() const {
    return wide;
}

void Metrix::setWide(int wide) {
    Metrix::wide = wide;
}

int Metrix::getHeight() const {
    return height;
}

void Metrix::setHeight(int height) {
    Metrix::height = height;
}

int Metrix::getRandomLowerLimit() const {
    return randomLowerLimit;
}

void Metrix::setRandomLowerLimit(int randomLowerLimit) {
    Metrix::randomLowerLimit = randomLowerLimit;
}

int Metrix::getRandomHigherLimit() const {
    return randomHigherLimit;
}

void Metrix::setRandomHigherLimit(int randomHigherLimit) {
    Metrix::randomHigherLimit = randomHigherLimit;
}

void Metrix::checkIfDementionMatch_Mul(const Metrix &multiplier) const {
    if (this->wide != multiplier.getHeight()){
        printf("Dementions (%d, %d) dose not match (%d, %d)",
               this->height,
               this->wide,
               multiplier.getHeight(),
               multiplier.getWide());
        throw "Dementions not match!";
    }
}

void Metrix::setResultElement_plus(int height, int wide, int number) {
    this->result[height][wide] += number;
}

int Metrix::getElement(int height, int wide) const {
    return this->result[height][wide];
}

Metrix Metrix::operator*(const Metrix &multiplier) const {
    checkIfDementionMatch_Mul(multiplier);
    Metrix metrixResult = Metrix(this->height, multiplier.getWide(), 0, 0, false);
    metrixResult.build2DArray();
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < multiplier.getWide(); ++j) {
            for (int k = 0; k < this->wide; ++k) {
                metrixResult.setResultElement_plus(i, j, this->result[i][k] * multiplier.getElement(k, j));
            }
        }
    }
    return metrixResult;
}

Metrix Metrix::multiplayUsingAvx(const Metrix &multiplier) const {
    checkIfDementionMatch_Mul(multiplier);
    Metrix metrixResult = Metrix(this->height, multiplier.getWide(), 0, 0, false);
    metrixResult.build2DArray();

    int **mat1 = this->result;
    int **mat2 = multiplier.getResult();
    int **result = metrixResult.getResult();

    __m256i vec_multi_res = _mm256_setzero_si256(); //Initialize vector to zero
    __m256i vec_mat1 = _mm256_setzero_si256(); //Initialize vector to zero
    __m256i vec_mat2 = _mm256_setzero_si256(); //Initialize vector to zero

    int i, j, k;
    for (i = 0; i < this->height; i++)
    {
        for (j = 0; j < multiplier.getWide(); ++j)
        {
            //Stores one element in mat1 and use it in all computations needed before proceeding
            //Stores as vector to increase computations per cycle
            vec_mat1 = _mm256_set1_epi32(mat1[i][j]);

            for (k = 0; k < this->getWide(); k += 8)
            {
                vec_mat2 = _mm256_loadu_si256((__m256i*)&mat2[j][k]); //Stores row of second matrix (eight in each iteration)
                vec_multi_res = _mm256_loadu_si256((__m256i*)&result[i][k]); //Loads the result matrix row as a vector
                vec_multi_res = _mm256_add_epi32(vec_multi_res ,_mm256_mullo_epi32(vec_mat1, vec_mat2));//Multiplies the vectors and adds to th the result vector

                _mm256_storeu_si256((__m256i*)&result[i][k], vec_multi_res); //Stores the result vector into the result array
            }
        }
    }

    metrixResult.setResult(result);

    return metrixResult;
}

Metrix::~Metrix() {
    delete this->result;
}

int **Metrix::getResult() const {
    return result;
}

void Metrix::setResult(int **result) {
    Metrix::result = result;
}

bool Metrix::operator==(const Metrix &rhs) const {

    bool resultEqualsFlag = true;
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->wide; ++j) {
            if(this->result[i][j] != rhs.getElement(i, j)){
                resultEqualsFlag = false;
            }
        }
    }

    return wide == rhs.wide &&
           height == rhs.height &&
            resultEqualsFlag;
}

bool Metrix::operator!=(const Metrix &rhs) const {
    return !(rhs == *this);
}
