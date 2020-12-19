# currying-for-templates

La currificación es la capacidad de una función de poder recibir los argumentos de uno en uno, de forma que para cada argumento se retorno una nueva función acepta el resto de argumento de la función original.

Esta librería permite currificar de forma sencilla metafunciones en C++ que aceptan un número variable de argumentos 'type' y retorna otro 'type'. Un ejemplo sencillo es el siguiente:

```C++
#include <type_traits>
#include "currytemp.h"

// Tipo de dato que representa un entero
template<int k>
using int_constant = std::integral_constant<int,k>;

// Metafuncion para sumar dos enteros
template<typename N, typename M>
struct suma : std::int_constant<int,N::value+M::value>;

// Currificamos la metafuncion
using suma_c = hgs::curry_t<suma>;


// Ejemplo de uso
using suma1_c = suma_c::type<int_constant<1>>;

/*
*   Observa que suma_c no es una metafuncion como tal.
*   En realidad se trata de una 'metafuncion perezosa'.
*/
```

## Metafunciones perezosas
