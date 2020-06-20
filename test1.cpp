#include "test1.h"
#include <iostream>
#include <random>
template <typename t>
void printVec(vector<t> v)
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
    asm volatile("" ::: "memory");
}

void validate(GeneMatrix gm)
{
    vector<double> correct_out = gm.makeOutVec(gm.mat, gm.vec);
    printVec(correct_out);
    printVec(gm.getOut());
    assert(correct_out == gm.getOut());

}

void test(GeneMatrix gm)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,gm.mat.size() * gm.mat[0].size()-1);
    std::uniform_real_distribution<double> vals(0.1,100.0);
    for(int i=0; i<100'000'000;i++)
    {
        int rand = distribution(generator);
        gm.update(rand/gm.mat[0].size(),rand%gm.mat[0].size() ,vals(generator));
    }
    validate(gm);
}

int main()
{
    std::cout.precision(15);
    vector<vector<double>> data = {{1.0, 2.0, 3.0, 4.0},
                                   {1.0, 2.0, 3.0, 4.0},
                                   {1.0, 2.0, 3.0, 4.0},
                                   {1.0, 2.0, 3.0, 4.0}

    };
    magic();
    vector<double> vec = {6.0, 7.0, 8.0, 9.0};
    GeneMatrix gm(data, vec);
    magic();
    
    test(gm);
    return 0;
}
