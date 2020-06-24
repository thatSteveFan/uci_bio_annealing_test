
#include <algorithm>
#include <cassert>
#include <chrono>
#include <numeric>
#include <random>
#include <vector>
#include <iostream>

#include "GeneMatrix.h"

#ifndef ALGORITHM_STATE_H
#define ALGORITHM_STATE_H

constexpr int P_BAD_WINDOW = 1000;

template <typename f>
class AlgorithmState {
  private:
    GeneMatrix G;
    GeneMatrix E;
    f fitnessFunction;
    double currentFitness = 0;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> indexes = std::uniform_int_distribution<int>(0, G.getSize().first *G.getSize().second + E.getSize().first * E.getSize().second - 1);
    std::uniform_real_distribution<double> vals = std::uniform_real_distribution<double>(0, 1.0);
    // bool accepted_change[P_BAD_WINDOW] = {false};
    // int head = 0;
  public:
    // AlgorithmState(const GeneMatrix& G, const GeneMatrix& E, f fit): G(G), E(E), fitnessFunction{fitness}, currentFitness(fit(this->G.getOut(), this->E.getOut())){};
    // no copying
    // AlgorithmState(const GeneMatrix& G, const GeneMatrix& E, f fit) :
    //     G(G),
    //     E(E),
    //     fitnessFunction{fit},
    //     currentFitness(
    //         fit(this->G.getOut(),
    //             this->E.getOut())
    //     )
    //     {};
    AlgorithmState(GeneMatrix &&G, GeneMatrix &&E, f &&fit, int seed = std::chrono::system_clock::now().time_since_epoch().count())
        : G(std::move(G)), E(std::move(E)), fitnessFunction{(fit)}, currentFitness(fit(this->G.getOut(), this->E.getOut())), generator(seed){};
    AlgorithmState(
        vector<vector<double>> Gdata,
        vector<double> Gvec,
        vector<vector<double>> Edata,
        vector<double> Evec,
        f &&fit,
        int seed = std::chrono::system_clock::now().time_since_epoch().count())
        : G(Gdata, Gvec), E(Edata, Evec), fitnessFunction{fit}, currentFitness(fit(this->G.getOut(), this->E.getOut())), generator(seed){};
    ~AlgorithmState() = default;

    double fitness() const {
        return currentFitness;
    }
    void randomChange(double p) {
        assert(p >= 0);
        assert(p <= 1);
        auto idx = indexes(generator);
        // crazy hack to avoid doing a default init on a matrix
        auto [index, mat] = [&]() {
            if(idx < G.getSize().first * G.getSize().second)
            {
                return std::pair<int, GeneMatrix&>(idx, G);
            } 
            else
            {
                return std::pair<int, GeneMatrix&>(idx - (G.getSize().first * G.getSize().second), E);
            }}();
        auto old_fit = currentFitness;
        auto old_val = mat.getMat(index / mat.getSize().second,index % mat.getSize().second);
        mat.update(index / mat.getSize().second,index % mat.getSize().second, vals(generator));
        auto newFitness = fitnessFunction(G.getOut(), E.getOut());
        if(newFitness < old_fit)
        {
            if(vals(generator) > p)
            {
                mat.update(index / mat.getSize().second,index % mat.getSize().second, old_val);
                // cout << "rejected" << endl;
            }
            else
            {
                // cout << "accepted" << endl;
            }
            
        }
        // std::cout << (&mat == &G? "G ":"E ")<< index << '\n';
        // if (mat == E)
        //     mat.update(index / gm.mat[0].size(), index % gm.mat[0].size(), vals(generator));
        // if()
        // {

        // }
    }
    // void try_change_G(int row, int col, double pbad)
    // {

    // }
    // void try_change_E(int row, int col, double pbad)
    // {

    // }
};
// template<typename f>
// AlgorithmState<f>::AlgorithmState(GeneMatrix G, GeneMatrix E)
// {
// }
// template<typename f>
// AlgorithmState<f>::~AlgorithmState()
// {
// }

#endif