#include "test1.h"
#include <iostream>
#include <random>
#include <chrono>

// template <typename t>
// void printVec(vector<t> v)
// {
//     for (auto &e : v)
//     {
//         cout << e << ", ";
//     }
//     cout << '\n';
// }

template <typename t>
void printVec(const vector<t> &v)
{
    for (auto &e : v)
    {
        cout << e << ", ";
    }
    cout << '\n';
}

// does nothing, but forces an actual compute. Stops the compiler from proving my code away
void magic()
{
    asm volatile("" ::
                     : "memory");
}

bool close(double d1, double d2)
{
    // return d1 == d2;
    return std::abs((d1 - d2) / d1) < 0.00001;
}

vector<vector<double>> makeTestData(int rows, int cols)
{
    vector<vector<double>> result;
    for (int i = 0; i < rows; i++)
    {
        vector<double> row;
        for (int j = 0; j < cols; j++)
            row.push_back((double)(i + j));
        result.push_back(row);
    }
    return result;
}

vector<double> makeTestVec(double size)
{

    vector<double> row;
    for (int j = 0; j < size; j++)
        row.push_back((double)(size));
    return row;
}

void validate(const GeneMatrix &gm)
{
    vector<double> correct_out = gm.makeOutVec(gm.mat, gm.vec);
    // printVec(correct_out);
    // printVec(gm.getOut());

    // forgive me for this abuse of c++. It checks if each of the pair are close
    assert(
        std::inner_product(
            std::begin(gm.getOut()),
            std::end(gm.getOut()),
            std::begin(correct_out),
            false,
            std::logical_or<bool>(),
            [](double d1, double d2) { return close(d1, d2); }));
}

void test(GeneMatrix &gm)
{
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, gm.mat.size() * gm.mat[0].size() - 1);
    std::uniform_real_distribution<double> vals(0.1, 100.0);

    magic();
    for (int i = 0; i < 100'000'000; i++)
    {
        magic();
        int rand = distribution(generator);
        gm.update(rand / gm.mat[0].size(), rand % gm.mat[0].size(), vals(generator));
        magic();
    }
    magic();
}

int main()
{
    std::cout.precision(15);
    vector<vector<double>> data = makeTestData(400, 1000);

    vector<double> vec = makeTestVec(1000);
    // for (auto &row : data)
    // {
    //     printVec(row);
    // }
    auto t1 = std::chrono::high_resolution_clock::now();
    GeneMatrix gm(data, vec);
    test(gm);
    auto t2 = std::chrono::high_resolution_clock::now();
    validate(gm);
    cout << "it took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds" << '\n';
    return 0;
}
