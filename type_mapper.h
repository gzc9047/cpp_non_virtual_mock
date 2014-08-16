#ifndef TYPE_MAPPER_H
#define TYPE_MAPPER_H

template < typename T >
struct TypeContainer {
    typedef T Type;
};

template < typename ... T >
struct TypePackage {
    typedef void Type(T ...);
};

template < typename T >
struct AppendType {
};

template < typename A, typename ... T >
struct AppendType<A(void(T ...))> {
    typedef typename TypePackage<A, T ...>::Type Type;
};

template < template<typename> class M, typename F, typename ... T >
struct TypeMapper {
    typedef typename M<F>::Type MF;
    typedef typename AppendType<MF(typename TypeMapper<M, T ...>::Result)>::Type Result;
};

template < template<typename> class M, typename F >
struct TypeMapper<M, F> {
    typedef typename M<F>::Type MF;
    typedef typename TypePackage<MF>::Type Result;
};

#endif // TYPE_MAPPER_H
