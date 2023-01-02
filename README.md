# Empresa telefónica.
En este problema, tenemos N líneas telefónicas entre dos ciudades, A y B. Son
bidireccionales pero sólo admiten una llamada al mismo tiempo. Si una persona
quiere hacer una llamada a la otra ciudad se verifica si hay una línea disponible,
en caso positivo se acepta la llamada. En caso de estén ocupadas la llamada se pierde
directamente. Nuestro objetivo es simular cuántas llamadas se realizan entre ambas ciudades y cuántas
líneas sería necesario implementar para optimizar el número de llamadas perdidas.

# Distribuciones de probabilidad:
1. Se recibirá una llamada cada 12 segundas, en media, desde la ciudad A hacia la B.
2. Se recibirá una llamada cada 10 segundos, en media, desde la ciudad B hacia la A.
3. Indiferentemente a la dirección, las llamadas durarán 4 minutos, en media.

En los tres casos hablamos de la media de un generador exponencial.
Las simulaciones son de 12 horas.

# Como ejecutar.
- `bin/telefon-line 1 50 1000 1`
    - `streambuffer`: El primer argumento controla la salida del programa. 0:terminal, 1:archivo.
        - Cada archivo tendrá un nombre único según los argumentos y se guardarán en `resultados/`.
    - `num_lineas`: El segundo se refiere al número de líneas de nuestra empresa.
    - `simulaciones`: El tercer es el número de simulaciones de 12 horas.
    - [OPCIONAL]: El siguiente término es opcional:
        - `MEAN`: Número de veces que se repitirá las N simulaciones para estudiar la media.
