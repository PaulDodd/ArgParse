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
/*
 **
 ** Copy from eigen::vector to various data types.
 ** Add support cases here.
 **
 */

template<class Precision, class T>
class copy_from
{
    public:
        size_t copy(const VectorXX<Precision>& src, T&& dest) // trivially copyable. i.e. T = VectorXX<Precision>
        {
            dest = src;
            return src.rows();
        }
};

/* Specializations defined here */
template<class Precision>
class copy_from_floating_point
{
    public:
        size_t copy(const VectorXX<Precision>& src, Precision&& dest)
        {
            if(src.rows()){
                dest = src[0];
                return 1;
            }
            return src.rows();
        }
};

template<class Precision>
class copy_from_iterable
{
    public:
        template<class IteratorType>
        size_t copy(const VectorXX<Precision>& src, IteratorType first, IteratorType last)
        {
            size_t i = 0;
            while(first != last)
            {
                *first = src[i++];
                first++;
            }
            
            return i;
        }
};

template<class Precision, class Container>
class copy_from_stl : public copy_from_iterable<Precision>
{
    public:
        size_t copy(const VectorXX<Precision>& src, Container&& dest)
        {
            typedef typename Container::iterator IteratorType;
            return copy_from_iterable<Precision>::template copy<IteratorType>(src, dest.begin(), dest.end());
        }
};

template<class Precision>
class copy_from<Precision, double> : public copy_from_floating_point<double>{};

template<class Precision>
class copy_from<Precision, float> : public copy_from_floating_point<float>{};

template<class Precision>
class copy_from<Precision, std::vector<Precision> > : public copy_from_stl<Precision, std::vector<Precision> > {};


template<class Precision, class... T>
class from_eigen_to_callable
{
    tuple<T...> m_Tuple;
    public:
        from_eigen_to_callable(const T& ...args)
        {
            m_Tuple = std::make_tuple(args...); // initialized tuple with a valid set of parameters.
        }
    
        tuple<T...> get(VectorXX<Precision> v)
        {
            copy_element<0, T...>(v);
            return m_Tuple;
        }
    private:
        template<size_t I = 0, class Type, class... Others>
        typename enable_if< 0 == sizeof...(Others), void >::type copy_element(VectorXX<Precision> v)
        {
            copy_from<Precision, Type> c;
            c.copy(v, std::forward<Type>(std::get<I>(m_Tuple))); // copy to the I-th tuple element.
        }
    
        template<size_t I = 0, class Type, class... Others>
        typename enable_if< 0 < sizeof...(Others), void >::type copy_element(VectorXX<Precision> v)
        {
            copy_from<Precision, Type> c;
            size_t i = c.copy(v, std::forward<Type>(std::get<I>(m_Tuple))); // copy to the I-th tuple element.
            VectorXX<Precision> next = v.block(i, 0, v.rows()-i, 1);
            copy_element<I+1, Others...>(next);
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
            from_eigen_to_callable<Precision, InputParam... > callable(args...);

            
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
class SolverBase
{
public:
    SolverBase() {}
    ~SolverBase() {}
};

template< class Precision >
class CFindRoot : SolverBase
{
public:
    CFindRoot(const Precision& tol, const size_t& maxIterations) : m_tol(tol), m_MaxIterations(maxIterations) { }
    ~CFindRoot() {}
    
    template< class FunctionType, class... InputParam >
    VectorXX<Precision> FixedPoint(Function<FunctionType>& g, InputParam... guess)
    {
        typedef typename std::result_of<Function<FunctionType>(InputParam...)>::type ReturnType;
        VectorXX<Precision> result, ro, delta;
        size_t i = 0;
        from_eigen_to_callable<VectorXX<Precision>, InputParam... > callable;
        to_eigen_data_type<Precision, ReturnType> output_picker;
        to_eigen_data_type<Precision, InputParam...> input_picker;
        
        result = input_picker.get(guess...);
        while(i++ < m_MaxIterations)
        {
            ReturnType x = f(callable.get(result));
            ro = result;
            result = output_picker.get(x);
            if((result - ro).norm() < m_tol)
            {
                break;
            }
        }
        return result;
    }
    
private:
    Precision m_tol;
    size_t    m_MaxIterations;
};

    
    
    
}




#endif
