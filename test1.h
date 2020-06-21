

#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>
using namespace std;
class GeneMatrix
{

    using matrix = vector<vector<double>>;

public:
    GeneMatrix(const matrix data, const vector<double> vec): GeneMatrix(data, vec, makeOutVec(data, vec)){};
    // deleted to stop stupid copying. Please pass by reference for performance reasons
    GeneMatrix(const GeneMatrix& other) = delete;
    GeneMatrix(GeneMatrix&& other) = default;
    const vector<double>& getOut() const
    {
        return outVec;
    }
    void update(int row, int col, double val)
    {
        auto diff = mat[row][col] - val;
        mat[row][col] = val;
        outVec[row] -= diff * vec[col];
    }
    // GeneMatrix(matrix &&data) : mat(data){};
    // GeneMatrix(const matrix &data) : mat(data){};
    friend void validate(const GeneMatrix& gm);
    friend void test(GeneMatrix& gm);
private:
    GeneMatrix(const matrix data, const vector<double> vec, const vector<double> outvec) : mat(data), vec(vec), outVec(outvec){};

    matrix mat;
    const vector<double> vec;
    vector<double> outVec;

    vector<double> makeOutVec(const matrix data, const vector<double> vec) const
    {
        assert(data.size() >0);
        // assert(data[0].size() == vec.size());
        vector<double> out;
        out.reserve(vec.size());
        for(int i=0; i<(int)data.size(); i++)
        {
            assert(data[i].size() == vec.size());
            out.push_back(std::inner_product(data[i].begin(), data[i].end(), vec.begin(), 0.0));
        }
        return out;
    }
    
};
