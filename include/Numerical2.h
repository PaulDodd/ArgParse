//
//  Numerical2.h
//  utilities
//
//  Created by Paul Dodd on 2/11/15.
//  Copyright (c) 2015 Paul Dodd. All rights reserved.
//

#ifndef utilities_Numerical2_h
#define utilities_Numerical2_h
#include <eigen3/Eigen/Dense>

namespace utils {



// Naming scheme change proposal:
/*

template<...>
class copy_to{};

template<...>
class copy_from{};

*/

// Concerns about efficiency:
/*
1.  The current model is to copy the input/output of the function calls into eigen data structures.
    This works well and I think it will be able to handle a wide range of use cases. 
    I am concerned that this will make the code very slow. 
 
2.  Make a more efficient version??

3.  How is the efficency 

*/





template<class Precision>
using VectorXX = Eigen::Matrix<Precision, Eigen::Dynamic, 1>;

template<class Precision>
using MatrixXX = Eigen::Matrix<Precision, Eigen::Dynamic, Eigen::Dynamic>;

template<class T, class S>
struct scalar_multiplies: binary_function<T, S, T>{
    static_assert(std::is_floating_point<S>::value, "S template parameter must be a floating point type.");
    T operator() (const T& x, const S& y) const {return x*y;}
};
template<class T, class S>
struct scalar_divides: binary_function<T, S, T>{
    static_assert(std::is_floating_point<S>::value, "S template parameter must be a floating point type.");
    T operator() (const T& x, const S& y) const {scalar_multiplies<T, S>m; return m(x, S(1)/y);}
};


// add support cases here.

template< class Precision, class...T >
class to_eigen_data_type
{
    VectorXX<Precision> m_Vector;
    
    void fill(void) { /** Nothing to do **/  }
    template<class... U>
    void fill(Precision x, U... args)
    {
        int i = int(m_Vector.rows()) - int(sizeof...(args)) - 1;
        m_Vector[i] = x;
        fill(args...);
    }
    
    public:
        VectorXX<Precision> get(T... args)
        {
            m_Vector.resize(sizeof...(args));
            fill(args...);
            return m_Vector;
        }
};

template< class Precision >
class to_eigen_data_type_from_iterable
{
    VectorXX<Precision> m_Vector;
    public:
        template<class InputIterator>
        VectorXX<Precision> get(const size_t& N, const InputIterator& begin, const InputIterator& end)
        {
            m_Vector.resize(N);
            InputIterator it = begin;
            for(size_t i = 0; i < N; i++, it++)
                m_Vector[i] = *it;
            return m_Vector;
        }
};

template< class Precision, class Container >
class to_eigen_data_type_from_stl : public to_eigen_data_type_from_iterable<Precision>
{
    public:
        VectorXX<Precision> get(const Container& v){ return to_eigen_data_type_from_iterable<Precision>::get(v.size(), v.begin(), v.end()); }
};

//template< class Precision, class Pointer >
//class to_eigen_data_type_from_pointer : public to_eigen_data_type_from_iterable<Precision>
//{
//    public:
//        VectorXX<Precision> get(const size_t& N, const Pointer& v){ return to_eigen_data_type_from_iterable<Precision>::get(N, v, v+N); }
//};

template<class Precision>
class to_eigen_data_type<Precision, vector<Precision> > : public to_eigen_data_type_from_stl<Precision, vector<Precision> > {};

//template<class Precision>
//class to_eigen_data_type<Precision, Precision* > : public to_eigen_data_type_from_pointer<Precision, Precision* > {};
//


template<class M, class... T>
class from_eigen_to_callable
{
    tuple<T...> m_Tuple;
    public:
        tuple<T...> get(M& v)
        {
            for(size_t i = 0; i < v.rows(); i++)
                utils::put(m_Tuple, v[i], i);
            return m_Tuple;
        }
};

// end support cases.


// This may be overkill:
// Function base describes a function, f:R^n -> R^m
// InputParam should be some type x \in R^n and should have [] operator available or is_arithmatic<InputParam>::type should be true.
// OutputParam should be some type y \in R^m and should have [] operator available. Also type must be returnable
// this interface should be used so we can take numerical derivates easily.
template<class FunctionType>
class Function
{
    public:
        Function(const FunctionType* const pf) : m_pF(pf) {}
        ~Function() {}
        template<class... InputParam>
        typename std::result_of<FunctionType(InputParam...)>::type operator()  (InputParam... args ) const { return (*m_pF)(args...); }
    
        template<class... InputParam>
        typename std::result_of<FunctionType(InputParam...)>::type operator()  (std::tuple<InputParam...>&& t) const
        {
            return call_fn(std::forward< tuple<InputParam...> >(t));
        }
    
    private:
        template<size_t... N, class... InputParam>
        typename enable_if< sizeof...(N) == sizeof...(InputParam),typename std::result_of<FunctionType(InputParam...)>::type>::type call_fn(std::tuple<InputParam...>&& t) const
        {
            return (*m_pF)(get<N>(std::forward< tuple<InputParam...> >(t))...);
        }
        
        template<size_t... N, class... InputParam>
        typename enable_if< sizeof...(N) < sizeof...(InputParam),typename std::result_of<FunctionType(InputParam...)>::type>::type call_fn(std::tuple<InputParam...>&& t) const
        {
            return call_fn<N..., sizeof...(N)>(std::forward< std::tuple<InputParam...> >(t));
        }
    
    private:
        const FunctionType* const m_pF;
};

// interface for operations... integration, differentiation, grad, ect.0
template <class PrecisionType >
class OperatorBase
{
    
public:
    OperatorBase() {}
    ~OperatorBase() {}
};

template <class Precision >
class Derivative : public OperatorBase< Precision >
{
    public:
        enum DiffCode {
            ForwardDiff = 1,
            BackwardDiff = 2,
        };
        Derivative(const Precision& h = 0.001) : m_h(h) {}
        ~Derivative() {}
    
        template<class FunctionType, class... InputParam>
        typename std::result_of<Function<FunctionType>(InputParam...)>::type operator() (const Function<FunctionType>& f, int Dim, InputParam... args)
        {
            typedef typename std::result_of<Function<FunctionType>(InputParam...)>::type ReturnType;
            ReturnType fx, fplus;
            minus<ReturnType> sub;
            scalar_divides<ReturnType, Precision> div;
            to_eigen_data_type<Precision, InputParam...> picker;
            VectorXX<Precision> vargs = picker.get(args...);
            from_eigen_to_callable<VectorXX<Precision>, InputParam... > callable;

            
            fx = f(args...);
            vargs[Dim]+=m_h;
            fplus = f(callable.get(vargs));
            
            return div(sub(fplus, fx), m_h);
        }
    private:
        Precision m_h;
};

//
template < class Precision >
class Jacobian : public OperatorBase<Precision>
{
private:
    size_t m_Ndim;
    size_t m_Nfunc;
    MatrixXX<Precision> m_Jacobian;
public:
    Jacobian(size_t Ndim, size_t Nfunc): m_Ndim(Ndim), m_Nfunc(Nfunc) { m_Jacobian.resize(m_Nfunc, m_Ndim); }
    ~Jacobian() {}
    
    template<class FunctionType, class... InputParam>
    const MatrixXX<Precision>& operator() (const Function<FunctionType>& f, InputParam... args)
    {
        typedef typename std::result_of<Function<FunctionType>(InputParam...)>::type ReturnType;
    }
};
//
//
//
//
//
//
//class SolverBase
//{
//public:
//    SolverBase() {}
//    ~SolverBase() {}
//};
//
//template< class FunctionType >
//class CFindRoot : SolverBase
//{
//public:
//    CFindRoot() {}
//    ~CFindRoot() {}
//private:
//    // what do we need.
//};

    
    
    
}




#endif
