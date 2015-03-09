

#include "utils.h"
#include "CommandLine.h"
#include "Stopwatch.h"
#include "Numerical2.h"


using namespace std;
using namespace utils;




class quadratic
{
    double m_a, m_b, m_c;
    public:
        quadratic(double a, double b, double c) : m_a(a), m_b(b), m_c(c){}
        double operator()(const double& x) const { return m_a*x * x + m_b*x + m_c; }
};

class quadratic2d
{
    double m_a, m_b, m_c;
public:
    quadratic2d(double a, double b, double c) : m_a(a), m_b(b), m_c(c){}
    double operator()(const vector<double>& x) const { return m_a*x[0] * x[1] + m_b*x[0] + m_c; }
};


class CTestSubCommand : public CCommandBase
{
    public:
        CTestSubCommand() : CCommandBase("sub"), m_Path("-path", "", true), m_Number("-n", 0, 10, false), m_bFlag("-flag")
        {
            SetDescription      ("This is a test sub-command");
            m_Path.SetUsage     ("Path to a file");
            m_Number.SetUsage   ("Integer between 0 and 10");
            m_bFlag.SetUsage    ("Boolean example");
            
            m_Parser.AddArg(m_Path);
            m_Parser.AddArg(m_Number);
            m_Parser.AddArg(m_bFlag);
            
        }
        ~CTestSubCommand(){}
    
        int ExecuteMain()
        {

            cout<< "Executing command " << m_Name << endl;
            if(m_Path.IsInitialized())
                cout << "Path : "<< m_Path.Value() << endl;
            if(m_Number.IsInitialized())
                cout << "Number : "<< m_Number.Value() << endl;
            if(m_bFlag.IsInitialized())
                cout << "Flag : "<< (m_bFlag.Value() ? "true" : "false") << endl;
        
            return CMD_LINE_NO_ERROR;
        }
    private:
    // Arguments
        CCommandLineArgString       m_Path;
        CCommandLineArgNum<size_t>  m_Number;
        CCommandLineArgBool         m_bFlag;
    
};

class CTestCommand : public CCommandBase
{
    public:
        CTestCommand() : CCommandBase("test")
        {
            SetDescription("Command for all tests.");
            AddCommand(m_SubCommand);
        }
        ~CTestCommand() {}
    
        int ExecuteMain()
        {
            return CMD_LINE_PRINT_HELP; // this command is a no-op
        }
    
    private:
    // sub-commands
        CTestSubCommand     m_SubCommand;
};

class TestProgram : public CProgramBase
{
    public:
        TestProgram(const string& name, int argc, const char * argv[]) : CProgramBase(name, argc, argv)
        {
            SetDescription("This program demonstrates argparse");
            AddCommand(m_TestCommand);
        }
        ~TestProgram() {}
    private:
        CTestCommand m_TestCommand;
    
};
#define ALPHABET "01ABCDEFGHIJKLMNOPQRSTUVWXYZ"
int main(int argc, const char * argv[])
{
    
    set<int> intset;
    vector<size_t> v;
    vector<double> vf, vf2;
    vector< vector<double> > vff;
    vf2.push_back(1.0); vf2.push_back(1.0);
    vff.push_back(vf2);
    for(size_t i = 0; i < 10; i++) v.push_back(i);
    vf.push_back(1.00000000001);
    vf.push_back(0.99999999999);

    bool equal = 3 == 3;
    cout << "hello world!" << equal << endl;
    cout << boolalpha << "5 in vec = " << IsInVec<size_t, equal_to<size_t> >(v, size_t(5)) << endl;
    float_is_equal<double> f;
    float_vec_is_equal<double> ff;
    
    cout << boolalpha << "1.0 in vecF = " << IsInVec<double, float_is_equal<double> >(vf, double(1.0)) << endl;
    cout << boolalpha << "vf in vff = " << IsInVec<vector<double>, float_vec_is_equal<double> >(vff, vf) << endl;
    
    cout << "abs(-6 % 4)=" << -5%4 << endl;
    cout << "int(-6) % size_t(4)=" << int(-6)%size_t(4) << endl;
    
    for(int i = 0; i < 10; i++){
    cout << "d(0, "<< i <<") = " << mod_dist(0, i, 4) << endl;
    }

    for(int i = 0; i < 10; i++){
    cout << "d(0, "<< i <<") = " << mod_dist(0, i, 7) << endl;
    }
    string alpha(ALPHABET), str1 = "AAABBB", str2 = "BAAABB", str3 = "AABBBA", str4 = "ABABBA";
    
    for(size_t i = 0; i < alpha.length(); i++)
    {
        cout << alpha[i]<< " = " <<int(alpha[i]) << endl;
    }
    

    circular_string_equal_to eq;
    cout << str1 << " == " << str2 << " : " << boolalpha << eq(str1, str2, str1.length()) << endl;
    cout << str1 << " == " << str3 << " : " << boolalpha << eq(str1, str3, str1.length()) << endl;
    cout << str2 << " == " << str3 << " : " << boolalpha << eq(str2, str3, str2.length()) << endl;
    
    cout << str1 << " == " << str4 << " : " << boolalpha << eq(str1, str4, str4.length()) << endl;
    cout << str2 << " == " << str4 << " : " << boolalpha << eq(str2, str4, str4.length()) << endl;
    cout << str3 << " == " << str4 << " : " << boolalpha << eq(str3, str4, str4.length()) << endl;
    
    
    cout << "10 = "<< NumberToString(10, 4, '0') << endl;
    cout << "acos(1) = "<< acos(1)<< endl;
    cout << "acos(-1) = "<< acos(-1.0000000001)<< endl;
    
    std::mt19937_64 generator;
    cout<< "****************************************************************************************************************************************************"<<endl;
    cout << generator<< endl<< endl << endl;
//    rng_util::seed_generator(generator);
    cout<< "****************************************************************************************************************************************************"<<endl;
//    cout << generator<< endl<< endl << endl;

//    cout << "after RNG = " << generator << endl;
    
    std::uniform_real_distribution<double> u01(0,1);
    std::uniform_real_distribution<double> u81(0.8,1);
    
    auto rnd = std::bind(u01, generator);
    auto rnd2 = std::bind(u81, generator);
    for(size_t i = 0; i < 10; i++)
        cout << "rnd "<< rnd() << ", "<< rnd2() << endl;
    
    maximum(vector<size_t>());
    
    auto func = [](float i, float j)->float{ return i*i + j - 1; };
//
    quadratic quad(1.0, 0.0, -1.0);
//    Derivative< double, quadratic > D;
//    auto dx = std::bind(D, quad, std::placeholders::_1);
//    double eval = dx(1.0);


    Function<quadratic> func1(&quad);
    Function<decltype(func)> func2(&func);
    quadratic2d quad2d(1.0, 0.0, -1.0);
    vector<double> x = {0.5, 0.5};
    Function<quadratic2d> func3(&quad2d);
    
    cout << "func1 = " << func1(0.0) << " func2 = " << func2(2, 3) << " func3 = " << func3(x) <<endl;
    
    Derivative<double>  Ddouble(0.01);
    Derivative<float>   Dfloat(0.01);
    
    auto dfdx = std::bind(Ddouble, func1, 0, std::placeholders::_1);
    auto dgdx = std::bind(Dfloat, func2, 0, std::placeholders::_1, std::placeholders::_2);
    auto dgdy = std::bind(Dfloat, func2, 1, std::placeholders::_1, std::placeholders::_2);
    
    auto dhdx = std::bind(Ddouble, func3, 0, std::placeholders::_1);
    
    cout << "dfdx @ 1.0 = " << dfdx(1.0) << " dgdx @ (2, 3) = " << dgdx(2.0f, 3.0f)<< " dgdy @ (2, 3) = " << dgdy(2.0f, 3.0f) <<endl;
    cout << "dhdx @ (0.5, 0.5) = " << dhdx(x) << endl;
    
    
    
    // Partition Test:
    vector<size_t> p1 = {0, 1, 1, 2}, p2 = {0, 1, 2, 2}, pbar;
    pbar = math_util::PartitionUnion(p1, p2);
    for(auto p : pbar)
    {
        cout << p << ", ";
    }
    
    
    
    
    
    
    return TestProgram(argv[0], argc, argv).Main();
}









































