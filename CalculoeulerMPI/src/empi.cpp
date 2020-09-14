#include <math.h>
#include "mpi.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;


int main(int argc, char *argv[]) {
/******* Declaramos variables para el proceso , identificador de proceso y numero de procesos  *********/
	int n, k, // tamaño del proceso
			rank, // Identificador de proceso
			size; // Número de procesos
	double E25DT = 2.718281828459045534884808;
	double mye, // Valor local de e
		ee;   // Valor global de e

	MPI_Init(&argc, &argv); // Inicializamos los procesos
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador

	// Solo en el proceso 0 se va a conocer el número de iteraciones que vamos a
	// ejecutar para la aproximación de e
	if (rank == 0) {
		cout << "Introduce el valor de  K (k >2), para la precisión : ";
		cin >> n;
	}

	// El proceso 0 reparte al resto de procesos el número de iteraciones
	// que calcularemos para la aproximación de ee
	MPI_Bcast(&n, // Puntero al dato que vamos a enviar
			1,  // Número de datos a los que apunta el puntero
			MPI_INT, // Tipo del dato a enviar
			0, // Identificacion del proceso que envia el dato
			MPI_COMM_WORLD);

	if (n <= 0) {
		MPI_Finalize();
		exit(0);
	} else {
		// Calculo de e

		unsigned long long fact = 1;
		k = n;

		double e = 2.0, e0;
		int m = 2, i;
		for (i = rank + 1; i < k; i += size) {
			e0 = e;
			fact *= m++;   //fact =fact * n++
			e += 1.0 / fact;
		};
		mye = e;
		// Todos los procesos ahora comparten su valor local de PI,
		// lo hacen reduciendo su valor local a un proceso
		// seleccionada a traves de una operacion aritmetica.
		MPI_Reduce(&mye, // Valor local de e
				&ee,  // Dato sobre el que vamos a reducir el resto
				1,	  // Numero de datos que vamos a reducir
				MPI_DOUBLE,  // Tipo de dato que vamos a reducir
				MPI_SUM,  // Operacion que aplicaremos
				0, // proceso que va a recibir el dato reducido
				MPI_COMM_WORLD);

		// Solo el proceso 0 imprime el mensaje, ya que es la unica que
		// conoce el valor de PI aproximado.
		if (rank == 0) {
			printf("E25DT: %.30f \n", E25DT);
			printf("My E: %.30f \n \n", ee);
			printf("El valor aproximado de e es: %f, con un error de %.30f \n",
					ee, fabs(ee - e0));

		}
	}

	// Terminamos la ejecucion de los procesos, despues de esto solo existira
	// el proceso 0
	// ¡Ojo! Esto no significa que los demas procesos no ejecuten el resto
	// de codigo despues de "Finalize", es conveniente asegurarnos con una
	// condicion si vamos a ejecutar mas codigo (Por ejemplo, con "if(rank==0)".
	MPI_Finalize();
	return 0;
}
