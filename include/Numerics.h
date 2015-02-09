//
//  Numerics.h
//  utils
//
//  Created by pdodd on 2/7/15.
//  Copyright (c) 2015 pdodd. All rights reserved.
//

#ifndef utils_Numerics_h
#define utils_Numerics_h


#include <eigen3/Eigen/Dense>



namespace utils {


// This may be overkill:
// Function base describes a function, f:R^n -> R^m
// InputParam should be some type x \in R^n and should have [] operator available or is_arithmatic<InputParam>::type should be true.
// OutputParam should be some type y \in R^m and should have [] operator available. Also type must be returnable
// this interface should be used so we can take numerical derivates easily.
template<size_t N, size_t M, class InputParam, class OutputParam>
class FunctionBase
{

    static_assert(is_copy_constructible<OutputParam>::value, "Must supply a copy constructable type for OutpuParam");

    public:
        typedef InputParam InputParamType;
        typedef OutputParam OutputParamType;
    
        FunctionBase() {}
        ~FunctionBase() {}
        virtual OutputParam operator()  (const InputParam& ) const = 0;
    
    public:
    // Define some helper functions here.
        size_t getN() { return N; }
        size_t getM() { return M; }
    
};

template< class ParamType, class ScalarType >
class DefaultParameter
{
    static_assert(is_arithmetic<ScalarType>::value, "ScalarType must be arithmetic.");
    public:
        DefaultParameter() {}
        ~DefaultParameter() {}
    
        virtual ParamType Add(const ParamType& x, const ParamType& y) { return x + y; }
        virtual ParamType ScalarMultiply(const ParamType& x, const ScalarType& y) { return x*y; }
        virtual ParamType ScalarDivide(const ParamType& x, const ScalarType& y) { return ScalarMultiply(x, ScalarType(1.0)/y); }
        virtual ParamType Subtract(const ParamType& x, const ParamType& y) { return Add(x, ScalarMultiply(y, ScalarType(-1))); }
    
    // need a way to force implementation of these for non-standard types.
        virtual ParamType Ones(int i = -1) const { static_assert(is_arithmetic<ParamType>::value, "Must implement DefaultParameter::Ones() for non-arithetic types"); return ParamType(1); }
        virtual ParamType Zeros() const { static_assert(is_arithmetic<ParamType>::value, "Must implement DefaultParameter::Zeros() for non-arithetic types"); return ParamType(0); }
        virtual ParamType Identity(){ static_assert(is_arithmetic<ParamType>::value, "Must implement DefaultParameter::Zeros() for non-arithetic types"); return ParamType();}
};

// interface for operations... integration, differentiation, grad, ect.0
template <class PrecisionType, class FunctionType, class InputParamType = DefaultParameter<typename FunctionType::FunctionBase::InputParamType, PrecisionType>, class OutputParamType = DefaultParameter<typename FunctionType::FunctionBase::OutputParamType, PrecisionType> >
class OperatorBase
{
    protected:
        InputParamType  m_InputParam;
        OutputParamType m_OutputParam;
    public:
        OperatorBase() {}
        ~OperatorBase() {}
};

template <class PrecisionType, class FunctionType>
class Derivative : public OperatorBase<PrecisionType, FunctionType>
{
    using InputType = typename FunctionType::FunctionBase::InputParamType;
    using OutputType = typename FunctionType::FunctionBase::OutputParamType;
    
    using OperatorBase<PrecisionType, FunctionType>::m_InputParam;
    using OperatorBase<PrecisionType, FunctionType>::m_OutputParam;
    
public:
    enum DiffCode {
        ForwardDiff = 1,
        BackwardDiff = 2,
    };
    Derivative() {}
    ~Derivative() {}
    
    OutputType operator() (const FunctionType& f, const InputType& x, int i = 0)
    {
        PrecisionType h = 0.01; // pass this into the constructor
        OutputType fx = f(x), fplus;
        InputType xplus = m_InputParam.Add(x, m_InputParam.ScalarMultiply(m_InputParam.Ones(i), h));
        fplus = f(xplus);
        return m_InputParam.ScalarDivide(m_InputParam.Subtract(fplus,fx), h);

    }
    
    
};

template <class PrecisionType, class FunctionType>
class Jacobian : public OperatorBase<PrecisionType, FunctionType>
{
    using InputType = typename FunctionType::FunctionBase::InputParamType;
    using OutputType = typename FunctionType::FunctionBase::OutputParamType;
    
    using OperatorBase<PrecisionType, FunctionType>::m_InputParam;
    using OperatorBase<PrecisionType, FunctionType>::m_OutputParam;
    
public:
    enum DiffCode {
        ForwardDiff = 1,
        BackwardDiff = 2,
    };
    Jacobian() {}
    ~Jacobian() {}
    
    Eigen::Matrix<PrecisionType, Eigen::Dynamic, Eigen::Dynamic> operator() (const FunctionType& f, const InputType& x, int i = 0)
    {

    }
    
    
};
    





class SolverBase
{
    public:
        SolverBase() {}
        ~SolverBase() {}
};

template< class FunctionType >
class CFindRoot : SolverBase
{
public:
    CFindRoot() {}
    ~CFindRoot() {}
private:
    // what do we need.
};




}






#endif // end safe guard
