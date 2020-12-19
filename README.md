# currying-for-templates

La currificación es la capacidad de una función de poder recibir los argumentos de uno en uno, de forma que para cada argumento se retorno una nueva función acepta el resto de argumento de l función original. En el lenguaje Haskell es el pan de cada día:

```Haskell
-- Funcion suma de toda la vida
suma :: Int -> Int -> Int
suma x y = x + y

-- Funcion que devuelve la entrada + 1
suma1 :: Int -> Int
suma1 = suma 1     -- Le damos un solo argumento a suma
```
