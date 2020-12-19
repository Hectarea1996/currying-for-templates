# currying-for-templates

La currificación es la capacidad de una función de poder recibir los argumentos de uno en uno, de forma que para cada argumento se retorno una nueva función acepta el resto de argumento de la función original.

Esta librería permite currificar de forma sencilla metafunciones en C++ que aceptan un número variable de argumentos 'type' y retorna otro 'type'. Un ejemplo sencillo es el siguiente:

```C++
#include <type_traits>
#include "currytemp.h"

/// Tipo de dato que representa un entero
template<int k>
using int_constant = std::integral_constant<int,k>;

/// Metafuncion para sumar dos enteros
template<typename N, typename M, typename P>
struct suma_triple : std::int_constant< int , N::value+M::value+P::value >;

/// Currificamos la metafuncion
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



