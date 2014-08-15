#ifndef GET_FUNCTION_TYPE_H
#define GET_FUNCTION_TYPE_H

template < typename T >
struct GetFuctionType {
    typedef void ReturnType;
};

template < typename R, typename ... S >
struct GetFuctionType<R(S ...)> {
    typedef R ReturnType;
};

template < typename C, typename R, typename ... S >
struct GetFuctionType<R(C::*)(S ...) const> {
    typedef R ReturnType;
};

template < typename C, typename R, typename ... S >
struct GetFuctionType<R(C::*)(S ...)> {
    typedef R ReturnType;
};

#endif // GET_FUNCTION_TYPE_H
