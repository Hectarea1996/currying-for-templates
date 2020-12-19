# currying-for-templates

La currificación es la capacidad de una función de poder recibir los argumentos de uno en uno, de forma que para cada argumento se retorno una nueva función acepta el resto de argumento de la función original.

Esta librería permite currificar de forma sencilla metafunciones en C++ que aceptan un número variable de argumentos 'type' y retorna otro 'type'. Un ejemplo sencillo es el siguiente:

```C++
#include <type_traits>  // Para poder usar std::integral_constant
#include "currytemp.h"

/// Tipo de dato que representa un entero
template<int k>
using int_constant = std::integral_constant<int,k>;

/// Metafuncion para sumar dos enteros
template<typename N, typename M, typename P>
struct suma_triple : int_constant< N::value+M::value+P::value >;

/// Currificamos la metafuncion suma
using suma_c = hgs::curry_t<suma>;


/// Ejemplo de uso
using suma5_c = suma_c::type< int_constant<2> , int_constant<3> >;

/**
*   Observa que suma_c no es una metafuncion como tal.
*   En realidad se trata de una 'metafuncion perezosa'.
*/

int main(){

   // Sumamos 5 a int_constant<37> y despues obtenemos el entero.
   std::cout << suma1_c::type<int_constant<37>>::value << std::endl;
   
   return 0;

}
```

`Resultado: 42`

## ¿Por qué una metafunción perezosa?

Como se puede apreciar en el ejemplo de arriba, una metafunción perezosa recibe los argumentos una vez accedemos al interior del struct. Observa las diferencias:

```C++
/// Metafuncion normal
template<typename T, typename S>
struct func{
   using type = // ...
}

/// Metafuncion perezosa
struct delayed_func{
   template<typename T, typename S>
   using type = // ...
}


/// Uso de la metafuncion normal
using type = func<t1,t2>::type;

/// Uso de la metafuncion perezosa
using type = func::type<t1,t2>;
```

La diferencia crucial entre las dos metafunciones no es la forma en la que se usan, sino en que una metafunción normal es un 'template struct' y la metafuncion perezosa es un 'struct'. Fíjate que cualquiera de los dos tipos de metafunciones reciben como parámetro un tipo de dato, es decir, un class o un struct. Por tanto, no podemos pasar como argumento otra metafunción normal, pero sí otra metafunción perezosa. Esto nos abre paso a la programación funcional y el uso de metafunciones de [orden superior](https://es.wikipedia.org/wiki/Funci%C3%B3n_de_orden_superior).

Quizás en este momento has recordado que podemos crear metafunciones normales que reciben como argumento un 'template struct':

```C++
template<template<typename> typename T>
struct func{ //... }
```

El problema surge cuando debemos indicarle a C++ qué queremos retornar. Una metafunción currificada debería devolver una metafunción normal (un 'template struct') si no le hemos proporcionado sufientes argumentos, o el resultado (un tipo de dato, un struct):

```C++
/// Posible funcion curry
template<template<typename...> typename TS>
struct curry{
   // Opcion 1
   using type = typename ...
   // Opcion 2
   using type = template ...
}
```

Pero claro, necesitamos contemplar las dos opciones a la vez. Algo imposible para lo que buscamos. Como ya podrás observar, este problema no aparece con las metafunciones perezosas, pues curry sólo tiene que devolver un struct.

> Es posible crear una metafuncion curry que sí contemple estas dos opciones, pero perderíamos la opción de proporcionar varios argumentos a la metafuncion resultante.

## Ejemplo

En el ejemplo se usa el tipo `int_constant` usado más arriba y un tipo de dato `lista`. Se muestra la implementación de la función de [orden superior](https://es.wikipedia.org/wiki/Funci%C3%B3n_de_orden_superior) `map_list` y la usaremos para relizar una operación sobre cada tipo de la lista.

```C++
#include <type_traits>  // Para poder usar std::integral_constant
#include "currytemp.h"



/// Tipo que representa un entero
template<int k>
using int_constant = std::integral_constant<int,k>;

/// Metafuncion para sumar dos enteros
template<typename N, typename M>
struct suma : int_constant<N::value+M::value>;

// Currificamos la metafuncion suma
using suma_c = hgs::curry<suma>;



/// Tipo que guarda una cantidad variable de tipos
template<typename... TS>
struct lista{};

/// Metafuncion que ejecuta una metafuncion perezosa sobre cada elemento de una lista 
template<typename Func, typename List>
struct map_list{};

template<typename Func, typename... TS>
struct map_list<Func,lista<TS...>>{
   using type = lista<Func::type<TS>...>;
}



// Creamos una lista de enteros
using lista_enteros = lista<int_constant<1>,int_constant<5>,int_constant<7>>;

// Obtenemos nuevas listas usando map_list y suma_c 
using lista_enteros_mas4 = map_list< suma_c::type<int_constant<4>> , lista_enteros >;
using lista_enteros_mas10 = map_list< suma_c::type<int_constant<10>> , lista_enteros >;

```
`lista_enteros_mas4 == lista_enteros<int_constant<5>,int_constant<9>,int_constant<11>>`

`lista_enteros_mas10 == lista_enteros<int_constant<11>,int_constant<15>,int_constant<17>>`

