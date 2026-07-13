|-------------------------|----------------------------------------------------------------------------------------------------------|                         

|      **#CRITERIO**          |                                           **#DESCRIPCIÓN**                                                   |

|-------------------------|----------------------------------------------------------------------------------------------------------|                        

| **Nombre del juego**        | Titulo del juego: Tic Tac Toe                                                                            |

|                         | Integrantes: Cristian Guamán, Derlis Veneras, Jhoann Chiles, Guillermo Calvache                          |

|------------------------ |----------------------------------------------------------------------------------------------------------|                      

| **Descripción de la lógica**| Utilizamos un bucle do-while con el fin de que el menú del juego y el propio juego se ejecute            |

|                         | almenos una vez en pantalla y se sale del bucle cuando la variable opción es igual a 3.                  |

|                         | Los condicionales que determinan los resultados se encuentran en la función verificarGanador             |

|                         | donde acompañado de un bucle for que ira verificando que haya la misma ficha repetida 3 veces de         |

|                         | manera horizontal, vertical, horizontal                                                                  |

|-------------------------|----------------------------------------------------------------------------------------------------------|                             

| **Uso de estructuras**      | ***Bucles Anidado***                                                                                           |

|                         | Procedimiento inicializarTablero: Aquí usamos un bucle for anidado para imprimir en pantalla el          | 

|                         | tablero del juego.                                                                                       | 

|                         |                                                                                                          |

|                         | ***Estructuras if/else-if/else***                                                                              |

|                         | Función verificarGanador: Aquí se utilizo el condicional if dentro de un for que recorre por cada        |

|                         | espacio de la matriz para que luego entre el if y vaya verificando que hay 3 fichas iguales tanto        |

|                         | horizontal, vertical y diagonal.                                                                         |

|                         |                                                                                                          |

|                         | Procedimiento juegoNormal: En esta parte del código tenemos 2 estructuras if, la primera es para         |

|                         | evaluar que la casilla que haya ingresado el usuario se encuentre en el intervalo \[1,9], este intervalo  |

|                         | significa los espacios que tiene la matriz y también valida que esa casilla no este ocupada por la       | 

|                         | ficha 'X' o 'O'.                                                                                         |

|                         | La segunda estructura if ayuda con el cambio del jugador, si la condición de que hay un jugador ganador  |

|                         | se cumple el ganador será el jugador con las 3 fichas alineadas, caso contrario  que no haya ningún      |

|                         | ganador se cambia el turno al otro jugador para que coloque su ficha y así seguir hasta que exista       |  

|                         | un ganador                                                                                               |

|-------------------------|----------------------------------------------------------------------------------------------------------|  

| **Conclusiones**            | 1.- Resulta que es mejor resolver el problema por partes ya que al tratar de resolver todo de una sola   |

|                         |     puede generar confusión y no se llega a una solución concreta.                                       |

|                         | 2.-  Se cumple el objetivo de aplicar estructuras básicas como son los condicionales, bucles, funciones  |                                                                                       |                         |      y procedimientos.                                                                                   |   

|                         | 3.- Se implemento el concepto de arreglo para este para poder manejar el tablero de manera mas fácil     |

|                         |     debido a que un arreglo tiene tamaño fijo que es perfecto para nuestro tablero.                      |                                                                                                    |                         | 4.- Exploramos otra forma de resolver el problema que seria aplicando matrices para los dos tipos de     |                                                                                                 |                         |     fichas ('X' y 'O').                                                                                  |          

|-------------------------|----------------------------------------------------------------------------------------------------------|                                                                                                                                    

