#ifndef CURRYTEMP_H_INCLUDED
#define CURRYTEMP_H_INCLUDED

namespace hgs::detail{

    /**
    *  Lista de tipos
    */
    template<typename... XS>
    struct type_list{};



    /**
    *  Inserta un elemento al inicio de una lista.
    */
    template<typename X, typename lista>
    struct cons{};

    template<typename X, typename... XS>
    struct cons<X,type_list<XS...>>{
        using type = type_list<X,XS...>;
    };

    template<typename X, typename lista>
    using cons_t = typename cons<X,lista>::type;



    /**
    *  Concatena dos listas
    */
    template<typename lista1, typename lista2>
    struct append{};

    template<typename... XS, typename... YS>
    struct append<type_list<XS...>,type_list<YS...>>{
        using type = type_list<XS...,YS...>;
    };

    template<typename lista1, typename lista2>
    using append_t = append<lista1,lista2>::type;



    /**
    *  Invierte el orden de los elementos de una lista
    */
    template<typename lista>
    struct reverse_list{};

    template<>
    struct reverse_list<type_list<>>{
        using type = type_list<>;
    };

    template<typename X, typename... XS>
    struct reverse_list<type_list<X,XS...>>{
        using type = append_t<typename reverse_list<type_list<XS...>>::type,type_list<X>>;
    };

    template<typename lista>
    using reverse_list_t = reverse_list<lista>::type;



    /**
    *  Evalua una metafuncion perezosa con los argumentos de una lista
    */
    template<typename func, typename lista>
    struct apply_func{};

    template<typename func, typename... XS>
    struct apply_func<func,type_list<XS...>>{
        using type = typename func::type<XS...>;
    };

    template<typename func, typename lista>
    using apply_func_t = typename apply_func<func,lista>::type;



    /**
    *  Declaracion adelantada de curry_aux.
    */
    template<typename func, typename lista>
    struct curry_aux;



    /**
    *  Recolecta un argumento.
    */
    template<typename func, typename lista>
    struct curry_collect{
        template<typename X>
        using type = typename curry_aux<func,cons_t<X,lista>>::type;
    };



    /**
    *  Concepto para comprobar si es posible ejecutar una metafuncion perezosa.
    */
    template<typename T, typename... XS>
    concept Callable = requires{
        typename T::type<XS...>;
    };




    /**
    *  Comprueba si hay pocos argumentos para una metafuncion perezosa.
    */
    template<typename T, typename L, typename... XS>
    struct few_arguments : std::true_type {};

    template<typename T, typename... YS, typename X, typename... XS>
    requires Callable<T,YS...>
    struct few_arguments<T,type_list<YS...>,X,XS...> : std::false_type {};

    template<typename T, typename... YS, typename X, typename... XS>
    requires (!Callable<T,YS...>)
    struct few_arguments<T,type_list<YS...>,X,XS...> : few_arguments<T,type_list<X,YS...>,XS...> {};



    /**
    *  Concepto para comprobar si hay pocos argumentos.
    */
    template<typename T, typename L, typename... XS>
    concept FewArguments = few_arguments<T,L,XS...>::value;



    /**
    *  Pasa de uno en uno varios argumentos al recolector individual de argumentos.
    */
    template<typename func, typename lista, typename X, typename... XS>
    requires FewArguments<func,lista,X,XS...>
    struct curry_collect_many_aux{
        using type = typename curry_collect<func,lista>::type<X>;
    };

    template<typename func, typename lista, typename X, typename Y, typename... XS>
    struct curry_collect_many_aux<func,lista,X,Y,XS...>{
        using type = curry_collect_many_aux<typename curry_collect<func,lista>::type<X>,lista,Y,XS...>::type;
    };

    template<typename func, typename lista, typename X, typename... XS>
    using curry_collect_many_aux_t = curry_collect_many_aux<func,lista,X,XS...>::type;



    /**
    *  Recolecta varios argumentos.
    */
    template<typename func, typename lista>
    struct curry_collect_many{
        template<typename X, typename... XS>
        using type = curry_collect_many_aux_t<func,lista,X,XS...>;
    };



    /**
    *  Si una metafuncion perezosa se puede ejecutar, se ejecuta.
    *  En otro caso se llama al recolector de argumentos.
    */
    template<typename func, typename lista>
    struct curry_aux{
        using type = curry_collect_many<func,lista>;
    };

    template<typename func, typename... XS>
    requires Callable<func,XS...>
    struct curry_aux<func,type_list<XS...>>{
        using type = apply_func_t<func,reverse_list_t<type_list<XS...>>>;
    };

    template<typename func, typename lista>
    using curry_aux_t = curry_aux<func,lista>::type;



    /**
    *  Convierte una metafuncion en una metafuncion perezosa.
    */
    template<template<typename...> typename func>
    struct delay{
        template<typename... XS>
        using type = func<XS...>::type;
    };

}

namespace hgs{

    /**
    *  Convierte una metafuncion en una metafuncion perezosa curryficada.
    */
    template<template<typename...> typename func>
    struct curry{
        using type = detail::curry_aux_t<detail::delay<func>,detail::type_list<>>;
    };

    template<template<typename...> typename func>
    using curry_t = curry<func>::type;

}


#endif // CURRYTEMP_H_INCLUDED
