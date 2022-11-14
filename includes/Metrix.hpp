//
// Created by Zhe Xu on 11/13/22.
//

#ifndef ECE697LS_METRIX_HPP
#define ECE697LS_METRIX_HPP


class Metrix {
private:
    int wide;
    int height;
    int randomLowerLimit;
    int randomHigherLimit;
    int ** result;
public:
    Metrix(int wide, int height, int randomLowerLimit, int randomHigherLimit, bool buildArray);

    void build2DArray();

    void fill2DArrayWithRandomValue();

    int getWide() const;

    void setWide(int wide);

    int getHeight() const;

    void setHeight(int height);

    int getRandomLowerLimit() const;

    void setRandomLowerLimit(int randomLowerLimit);

    int getRandomHigherLimit() const;

    void setRandomHigherLimit(int randomHigherLimit);

    int **getResult() const;

    void checkIfDementionMatch_Mul(const Metrix &multiplier) const;

    Metrix operator*(const Metrix &multiplier) const;

    void setResultElement_plus(int height, int wide, int number);

    int getElement(int height, int wide) const;

    Metrix multiplayUsingAvx(const Metrix &multiplier) const;

    virtual ~Metrix();

    void setResult(int **result);

    bool operator==(const Metrix &rhs) const;

    bool operator!=(const Metrix &rhs) const;
};


#endif //ECE697LS_METRIX_HPP
