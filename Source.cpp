/*
Escuela Superior de Cómputo
Instituto Politécnico Nacional

Visión artificial
Primer evaluación práctica

Luis Gerardo Ortiz Cruz
5BM1
*/

// Bibliotecas
#include <opencv2/opencv.hpp> // Para los elementos de OpenCV
#include <iostream> // Para la entrada y salida de datos
#include <cmath> // Para las funciones matematicas

// Espacios de nombres
using namespace cv; // de OpenCV
using namespace std; // de la Libreria estandar

/**
* Crea una ventana con los coeficientes del filtro gaussiano
* @param filasFiltro: numero de filas de la ventana, debe ser impar
* @param columnasFiltro: numero de columnas de la ventana, debe ser impar
* @param sigma: el valor de la desviacion estandar de la distribucion
* return: el arreglo bidimensional de la ventana con los coeficientes
*/
float** creaVentanaFiltroGussiano(int filasFiltro, int columnasFiltro, float sigma)
{
	// Constantes matematicas
	const float pi = acos(-1);
	// Variables de la ventana
	float** ventana;
	// Variables de las operaciones
	int anchoBordeHorizontal = (filasFiltro - 1) / 2;
	int anchoBordeVertical = (columnasFiltro - 1) / 2;
	int x = -anchoBordeVertical;
	int y = anchoBordeHorizontal;

	// Crea las filas de la ventana
	ventana = new float* [filasFiltro];
	for (int i = 0; i < filasFiltro; i++) // Para cada fila de la ventana
	{
		// Crea las columnas de la ventana
		ventana[i] = new float[columnasFiltro];
		for (int j = 0; j < columnasFiltro; j++) // Para cada columna de la ventana
		{
			// Asigna los valores del filtro
			ventana[i][j] = exp(-((x * x) + (y * y)) / (2 * sigma * sigma)) / (2 * pi * sigma * sigma);
			// Modifica la coordenada x
			x++;
		}
		// Reinicializa la coordenada x
		x = -anchoBordeVertical;
		// Modifica la coordenada y
		y--;
	}
	return ventana;
}

/**
* Muestra en consola los valores de una ventana
* @param ventana: la matriz de la ventana
* @param filas: el numero de filas de la ventana
* @param columnas: el numero de columnas de la ventana
*/
void muestraVentana(float** ventana, int filas, int columnas)
{
	// Muestra las dimensiones
	cout << "\nDimensiones ventana:" << '\n';
	cout << filas << '\n';
	cout << columnas << '\n';
	for (int i = 0; i < filas; i++) // Para cada fila
	{
		for (int j = 0; j < columnas; j++) // Para cada columna
		{
			// Muestra los valores
			cout << ventana[i][j] << ' ';
		}
		cout << '\n';
	}
}

/**
* Convierte una imagen a escala de grises por NTSC
* @param imagenColor: la imagen a color
* return: la imagen en ecala de grises
*/
Mat convierteEscalaGrisesNTSC(Mat imagenColor)
{
	// Variables de la imagen en escala de grises
	int filas = imagenColor.rows;
	int columnas = imagenColor.cols;
	Mat imagenGrises(filas, columnas, CV_8UC1);

	for (int i = 0; i < filas; i++) // Para cada fila
	{
		for (int j = 0; j < columnas; j++) // Para cada columna
		{
			// Variables de los pixeles
			float valorPixelRojo = imagenColor.at<Vec3b>(Point(j, i)).val[2];
			float valorPixelVerde = imagenColor.at<Vec3b>(Point(j, i)).val[1];
			float valorPixelAzul = imagenColor.at<Vec3b>(Point(j, i)).val[0];
			float valorPixel;

			// Convierte a escala de grises por NTSC
			valorPixel = 0.299 * valorPixelRojo + 0.587 * valorPixelVerde + 0.114 * valorPixelAzul;
			imagenGrises.at<uchar>(Point(j, i)) = valorPixel;
		}
	}
	return imagenGrises;
}

/**
* Copia una imagen en escala de grises completa a otra que pueda contenerla iniciando en el punto indicado
* @param imagen: la imagen donde se copiara la otra
* @param filaInicio: fila desde la cual se comienza a escribir la copia
* @param columnaInicio: columna desde la cual se comienza a escribir la copia
* @param imagenACopiar: imagen que se copiara completa
*/
void copiaImagenEscalaGrises(Mat imagen, int filaInicio, int columnaInicio, Mat imagenACopiar)
{
	for (int i = 0; i < imagenACopiar.rows; i++) // Para cada fila de la imagen a copiar
	{
		for (int j = 0; j < imagenACopiar.cols; j++) // Para cada columna de la imagen a copiar
		{
			// Copia la imagen
			imagen.at<uchar>(Point(columnaInicio + j, filaInicio + i)) = imagenACopiar.at<uchar>(Point(j, i));
		}
	}
}

/**
* Rellena la seccion de la imagen en escala de grises con el valor especificado
* @param imagen: la imagen a rellenar
* @param filaInicio: la fila de inicio de la seccion (inclusiva)
* @param filaFin: la fila de fin de la seccion (exclusiva)
* @param columnaInicio: la columna de inicio de la seccion (inclusiva)
* @param columnaFin: la columna de fin de la seccion (exclusiva)
* @param valor: el valor con el cual rellenar
*/
void rellenaSeccionImagenEscalaGrises(Mat imagen, int filaInicio, int filaFin, int columnaInicio, int columnaFin, uint valor)
{
	for (int i = filaInicio; i < filaFin; i++) // Para cada fila de la seccion
	{
		for (int j = columnaInicio; j < columnaFin; j++) // Para cada columna de la seccion
		{
			// Relllena la imagen
			imagen.at<uchar>(Point(j, i)) = valor;
		}
	}
}

/**
* Agrega los bordes correspondientes a una imagen en escala de grises para aplicarle una ventana
* @param imagen: la imagen a utilizar
* @param filasVentana: numero de filas de la ventana a aplicar (debe ser impar)
* @param columnasVentana: numero de columnas de la ventana a aplicar (debe ser impar)
* return: la nueva imagen con los bordes para aplicar la ventana
*/
Mat agregaBordesEscalaGrises(Mat imagen, int filasVentana, int columnasVentana)
{
	// Variables de la imagen
	int filasImagen = imagen.rows;
	int columnasImagen = imagen.cols;
	// Variables de los bordes
	int filasBorde = (filasVentana - 1) / 2;
	int columnasBorde = (columnasVentana - 1) / 2;
	// Variables de la imagen con bordes
	int filasImagenBordes = filasBorde + filasImagen + filasBorde;
	int columnasImagenBordes = columnasBorde + columnasImagen + columnasBorde;
	Mat imagenBordes(filasImagenBordes, columnasImagenBordes, CV_8UC1);

	// Copia la imagen
	copiaImagenEscalaGrises(imagenBordes, filasBorde, columnasBorde, imagen);
	// Rellena el borde superior
	rellenaSeccionImagenEscalaGrises(imagenBordes, 0, filasBorde, 0, columnasImagenBordes, 0);
	// Rellena el borde inferior
	rellenaSeccionImagenEscalaGrises(imagenBordes, filasBorde + filasImagen, filasImagenBordes, 0, columnasImagenBordes, 0);
	// Rellena el borde izquierdo
	rellenaSeccionImagenEscalaGrises(imagenBordes, 0, filasImagenBordes, 0, columnasBorde, 0);
	// Rellena el borde derecho
	rellenaSeccionImagenEscalaGrises(imagenBordes, 0, filasImagenBordes, columnasBorde + columnasImagen, columnasImagenBordes, 0);
	return imagenBordes;
}

/**
* Aplica una ventana por convolucion a una imagen en escala de grises con bordes
* @param ventana: la matriz de la ventana
* @param filasVentana: numero de filas de la ventana, debe ser impar
* @param columnasVentana: numero de columnas de la ventana, debe ser impar
* @param imagen: la imagen en escala de grises con bordes a la que se le aplicara la ventana
* return: la imagen con la ventana aplicada
*/
Mat aplicaVentanaEscalaGrises(float** ventana, int filasVentana, int columnasVentana, Mat imagen)
{
	// Variables de la imagen con el filtro
	int filas = imagen.rows - filasVentana + 1;
	int columnas = imagen.cols - columnasVentana + 1;
	Mat imagenResultado(filas, columnas, CV_8UC1);

	for (int i = 0; i < filas; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnas; j++) // Para cada columna de la imagen
		{
			// Variables para la convolucion
			float convolucion = 0;

			for (int k = 0; k < filasVentana; k++) // Para cada fila de la ventana
			{
				for (int l = 0; l < columnasVentana; l++) // Para cada columna de la ventana
				{
					// Suma el valor del producto de los pixeles de la imagen con bordes y el filtro
					convolucion += ventana[k][l] * imagen.at<uchar>(Point(j + l, i + k)); // i + a - a + k
				}
			}
			// Guarda el valor de la covolucion en el pixel
			convolucion = abs(convolucion);
			imagenResultado.at<uchar>(Point(j, i)) = (convolucion > 255 ? 255 : convolucion);
		}
	}
	return imagenResultado;
}

/**
* Ecualiza una imagen en escala de grises al rango especificado
* @param imagen: la imagena en escala de grises a ecualizar
* @param minimoNivelGris: el menor nivel de gris posible [0, 255]
* @param maximoNivelGris: el mayor nivel de gris posible [0, 255]
* return: la imagen ecualizada
*/
Mat ecualizaImagenEscalaGrises(Mat imagen, int minimoNivelGris, int maximoNivelGris)
{
	// Variables imagen ecualizada
	int filasImagen = imagen.rows;
	int columnasImagen = imagen.cols;
	Mat imagenEcualizada(filasImagen, columnasImagen, CV_8UC1);
	// Variables de la ecualizacion
	int rangoNivelesGris = maximoNivelGris - minimoNivelGris;
	int totalPixeles = filasImagen * columnasImagen;
	int frecuenciaNivelesGris[255];
	int frecuenciaAcumuladaNivelesGris[255];
	float probabilidadAcumulada[255];

	for (int i = 0; i < 255; i++) // Para cada nivel de gris
	{
		// Inicializa los arreglos de los niveles de gris
		frecuenciaNivelesGris[i] = 0;
	}
	for (int i = 0; i < filasImagen; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnasImagen; j++) // Para cada columna de la imagen
		{
			// Variables del pixel
			unsigned char valorPixel = imagen.at<uchar>(Point(j, i));

			// Obtiene la frecuencia del nivel de gris
			frecuenciaNivelesGris[valorPixel]++;
		}
	}
	// Obtiene la frecuencia acumulada
	frecuenciaAcumuladaNivelesGris[0] = frecuenciaNivelesGris[0];
	// Obtiene la probabilidad acumulada
	probabilidadAcumulada[0] = (float) frecuenciaAcumuladaNivelesGris[0] / totalPixeles;
	for (int i = 1; i < 255; i++) // Para cada nivel de gris mayor a 0
	{
		// Obtiene la frecuencia acumulada
		frecuenciaAcumuladaNivelesGris[i] = frecuenciaAcumuladaNivelesGris[i - 1] + frecuenciaNivelesGris[i];
		// Obtiene la probabilidad acumulada
		probabilidadAcumulada[i] = (float) frecuenciaAcumuladaNivelesGris[i] / totalPixeles;
	}
	for (int i = 0; i < filasImagen; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnasImagen; j++) // Para cada columna de la imagen
		{
			// Variables del pixel
			unsigned char valorPixel = imagen.at<uchar>(Point(j, i));

			// Ecualiza y guarda el valor del pixel
			valorPixel = minimoNivelGris + probabilidadAcumulada[valorPixel] * rangoNivelesGris;
			imagenEcualizada.at<uchar>(Point(j, i)) = valorPixel;
		}
	}
	return imagenEcualizada;
}

/**
* Muestra una imagen en una ventana e imprime su tamaño en la consola
* @param imagen: la imagen a mostrar
* @param nombreVentana: el nombre de la ventana
*/
void muestraImagen(Mat imagen, string nombreVentana)
{
	cout << "\nDimensiones:" << ' ';
	cout << nombreVentana << '\n';
	cout << imagen.rows << '\n';
	cout << imagen.cols << '\n';
	namedWindow(nombreVentana, WINDOW_AUTOSIZE);
	imshow(nombreVentana, imagen);
}

/**
* Crea una nueva ventana 3 x 3 de Sobel para el eje x
* return: la ventana de Sobelpara el eje x
*/
float** creaVentanaXSobel()
{
	// Variables de la ventana
	float** ventana;

	// Crea las filas de la ventana
	ventana = new float* [3];
	for (int i = 0; i < 3; i++) // Para cada fila de la ventana
	{
		// Crea las columnas de la ventana
		ventana[i] = new float[3];
	}
	// Guarda los valores de la ventana
	ventana[0][0] = -1;
	ventana[0][1] = 0;
	ventana[0][2] = 1;
	ventana[1][0] = -2;
	ventana[1][1] = 0;
	ventana[1][2] = 2;
	ventana[2][0] = -1;
	ventana[2][1] = 0;
	ventana[2][2] = 1;
	return ventana;
}

/**
* Crea una nueva ventana 3 x 3 de Sobel para el eje y
* return: la ventana de Sobelpara el eje y
*/
float** creaVentanaYSobel()
{
	// Variables de la ventana
	float** ventana;

	// Crea las filas de la ventana
	ventana = new float* [3];
	for (int i = 0; i < 3; i++) // Para cada fila de la ventana
	{
		// Crea las columnas de la ventana
		ventana[i] = new float[3];
	}
	// Guarda los valores de la ventana
	ventana[0][0] = -1;
	ventana[0][1] = -2;
	ventana[0][2] = -1;
	ventana[1][0] = 0;
	ventana[1][1] = 0;
	ventana[1][2] = 0;
	ventana[2][0] = 1;
	ventana[2][1] = 2;
	ventana[2][2] = 1;
	return ventana;
}

/**
* Crea la imagen de la magnitud del gradiente
* @param parcialX: la imagen con los valores de la derivada parcial de la componente x
* @param parcialY: la imagen con los valores de la derivada parcial de la componente y
*/
Mat creaImagenMagnitudGradiente(Mat parcialX, Mat parcialY)
{
	// Variables imagen magnitud gradiente
	int filasImagen = parcialX.rows;
	int columnasImagen = parcialX.cols;
	Mat imagenMagnitud(filasImagen, columnasImagen, CV_8UC1);

	for (int i = 0; i < filasImagen; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnasImagen; j++) // Para cada columna de la imagen
		{
			// Variables del pixel
			float pixelGx = parcialX.at<uchar>(Point(j, i));
			float pixelGy = parcialY.at<uchar>(Point(j, i));
			float valorPixel = sqrt(pixelGx * pixelGx + pixelGy * pixelGy);

			// Obtiene la magnitud del gradiente
			imagenMagnitud.at<uchar>(Point(j, i)) = (valorPixel > 255 ? 255 : valorPixel);
		}
	}
	return imagenMagnitud;
}

/**
* Crea la imagen de la direccion del gradiente
* @param parcialX: la imagen con los valores de la derivada parcial de la componente x
* @param parcialY: la imagen con los valores de la derivada parcial de la componente y
*/
Mat creaImagenDireccionGradiente(Mat parcialX, Mat parcialY)
{
	// Constantes matematicas
	const float pi = acos(-1);
	// Variables imagen direccion gradiente
	int filasImagen = parcialX.rows;
	int columnasImagen = parcialX.cols;
	Mat imagenDireccion(filasImagen, columnasImagen, CV_8UC1);

	for (int i = 0; i < filasImagen; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnasImagen; j++) // Para cada columna de la imagen
		{
			// Variables del pixel
			float pixelGx = parcialX.at<uchar>(Point(j, i));
			float pixelGy = parcialY.at<uchar>(Point(j, i));
			float valorPixel = atan2(pixelGy, pixelGx);

			// Obtiene la direccion del gradiente en grados
			valorPixel = valorPixel * 180 / pi;
			imagenDireccion.at<uchar>(Point(j, i)) = (valorPixel > 255 ? 255 : valorPixel);
		}
	}
	return imagenDireccion;
}

/**
* Suprime los pixeles de los bordes que no son maximos
* @param magnitudConBordes: la imagen con la magnitud del gradiente y los bordes para la vecindad
* @param direccion: la imagen con la direccion del gradiente
* return: la imagen con los pixeles de los bordes suprimidos
*/
Mat suprimePixelesNoMaximos(Mat magnitudConBordes, Mat direccion)
{
	// Variables imagen bordes suprimidos
	int filasImagen = direccion.rows;
	int columnasImagen = direccion.cols;
	int filasBorde = (magnitudConBordes.rows - filasImagen) / 2;
	int columnasBorde = (magnitudConBordes.cols - columnasImagen) / 2;
	Mat bordesSuprimidos(filasImagen, columnasImagen, CV_8UC1);

	for (int i = 0; i < filasImagen; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnasImagen; j++) // Para cada columna de la imagen
		{
			// Variables de la direccion
			unsigned char valorDireccion = direccion.at<uchar>(Point(j, i));
			// Variables de la posicion en la imagen con bordes
			int iBordes = i + filasBorde;
			int jBordes = j + columnasBorde;
			// Variables de la magnitud
			unsigned char valorMagnitud = magnitudConBordes.at<uchar>(Point(jBordes, iBordes));
			// Variables del grosor del borde en la vecindad
			unsigned char magnitudVecinoAnterior = 0;
			unsigned char magnitudVecinoSiguiente = 0;

			if (valorDireccion < 22.5 or valorDireccion > 157.5) // Si la transicion es horizontal (borde vertical)
			{
				magnitudVecinoAnterior = magnitudConBordes.at<uchar>(Point(jBordes, iBordes - 1));
				magnitudVecinoSiguiente = magnitudConBordes.at<uchar>(Point(jBordes, iBordes + 1));
			}
			if (valorDireccion > 22.5 or valorDireccion < 67.5) // Si la transicion es diagonal (borde diagonal invertida)
			{
				magnitudVecinoAnterior = magnitudConBordes.at<uchar>(Point(jBordes - 1, iBordes - 1));
				magnitudVecinoSiguiente = magnitudConBordes.at<uchar>(Point(jBordes + 1, iBordes + 1));
			}
			if (valorDireccion > 67.5 or valorDireccion < 112.5) // Si la transicion es vertical (borde horizontal)
			{
				magnitudVecinoAnterior = magnitudConBordes.at<uchar>(Point(jBordes - 1, iBordes));
				magnitudVecinoSiguiente = magnitudConBordes.at<uchar>(Point(jBordes + 1, iBordes));
			}
			if (valorDireccion > 112.5 or valorDireccion < 157.5) // Si la transicion es diagonal invertida (borde diagonal)
			{
				magnitudVecinoAnterior = magnitudConBordes.at<uchar>(Point(jBordes - 1, iBordes + 1));
				magnitudVecinoSiguiente = magnitudConBordes.at<uchar>(Point(jBordes + 1, iBordes - 1));
			}
			if (valorMagnitud < magnitudVecinoAnterior or valorMagnitud < magnitudVecinoSiguiente) // Si el pixel del borde no es maximo
			{
				// Suprime el borde no maximo
				valorMagnitud = 0;
			}
			// Guarda el valor del borde
			bordesSuprimidos.at<uchar>(Point(j, i)) = valorMagnitud;
		}
	}
	return bordesSuprimidos;
}

/**
* Umbraliza los valores de la imagen de acuerdo con los umbrales
* @param imagen: la imagen a umbralizar
* @param porcentajeUmbralMenor: el porcentaje del umbral mayor con respecto a la mayor intensidad
* @param porcentajeUmbralMayor: el procentaje del umbral menor con respecto del umbral mayor
*/
Mat umbralizaPixelesImagen(Mat imagen, float porcentajeUmbralMenor, float porcentajeUmbralMayor)
{
	// Variables de los umbrales
	unsigned char umbralMenor;
	unsigned char umbralMayor;
	unsigned char maximo = 0;
	// Variables imagen direccion gradiente
	int filasImagen = imagen.rows;
	int columnasImagen = imagen.cols;
	Mat imagenUmbral(filasImagen, columnasImagen, CV_8UC1);

	for (int i = 0; i < filasImagen; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnasImagen; j++) // Para cada columna de la imagen
		{
			// Variables del pixel
			unsigned char valorPixel = imagen.at<uchar>(Point(j, i));

			// Obtiene el maximo nivel de gris
			maximo = max(maximo, valorPixel);
		}
	}
	// Obtiene los valores de los umbrales
	umbralMayor = porcentajeUmbralMayor * maximo;
	umbralMenor = porcentajeUmbralMenor * umbralMayor;
	for (int i = 0; i < filasImagen; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnasImagen; j++) // Para cada columna de la imagen
		{
			// Variables del pixel
			unsigned char valorPixel = imagen.at<uchar>(Point(j, i));

			// Realiza el umbralizado
			if (valorPixel >= umbralMayor)
			{
				valorPixel = 255;
			}
			else
			{
				if (valorPixel > umbralMenor)
				{
					valorPixel = umbralMenor;
				}
				else
				{
					valorPixel = 0;
				}
			}
			imagenUmbral.at<uchar>(Point(j, i)) = valorPixel;
		}
	}
	return imagenUmbral;
}

/**
* Realiza la histerizacion de los bordes en una imagen umbralizada
* @param imagenConBordes: la imagen umbralizada con bordes
* @param filasVentana: numero de filas de la ventana, debe ser impar
* @param columnasVentana: numero de columnas de la ventana, debe ser impar
* return: la imagen con los bordes hizterizados
*/
Mat histerizaBordesImagen(Mat imagenConBordes, int filasVentana, int columnasVentana)
{
	// Variables de la imagen con el filtro
	int filas = imagenConBordes.rows - filasVentana + 1;
	int columnas = imagenConBordes.cols - columnasVentana + 1;
	Mat imagenHizterizada(filas, columnas, CV_8UC1);
	// Variables del borde
	int filasBorde = (filasVentana - 1) / 2;
	int columnasBorde = (columnasVentana - 1) / 2;

	for (int i = 0; i < filas; i++) // Para cada fila de la imagen
	{
		for (int j = 0; j < columnas; j++) // Para cada columna de la imagen
		{
			// Variables de la posicion en la imagen con bordes
			int iBordes = i + filasBorde;
			int jBordes = j + columnasBorde;
			// Variables del valor del pixel 
			unsigned char valorPixel = imagenConBordes.at<uchar>(Point(jBordes, iBordes));
			
			if (valorPixel > 0 and valorPixel < 255) // Si es un borde debil
			{
				// Variables de la conectividad
				bool esConexoBorde = false;

				for (int k = -filasBorde; k < filasBorde; k++) // Para cada fila de la vecindad
				{
					for (int l = -columnasBorde; l < columnasBorde; l++) // Para cada columna de la vecindad
					{
						if (imagenConBordes.at<uchar>(Point(jBordes + l, iBordes + k)) == 255) // Si el pixel es conexo con un borde
						{
							// Actualiza la conectividad
							esConexoBorde = true;
						}
					}
				}
				if (esConexoBorde) // Si se conecto con algun borde
				{
					// Actualiza su valor como un borde fuerte
					valorPixel = 255;
				}
				else
				{
					// Actualiza su valor como irrelevante
					valorPixel = 0;
				}
			}
			// Guarda el nuevo valor del borde
			imagenHizterizada.at<uchar>(Point(j, i)) = valorPixel;
		}
	}
	return imagenHizterizada;
}

int main()
{
	// Variables de la imagen
	Mat imagenOriginal;
	// Variables de la ventana
	int filasGauss = 5;
	int columnasGauss = 5;
	float sigmaGauss = 1;
	float** ventanaGauss;

	// Lee los parametros de la ventana
	cout << "Filas kernel filtro gaussiano:" << ' ';
	cin >> filasGauss;
	cout << "Columnas kernel filtro gaussiano:" << ' ';
	cin >> columnasGauss;
	cout << "Sigma kernel filtro gaussiano:" << ' ';
	cin >> sigmaGauss;
	// Crea la ventana y la muestra
	ventanaGauss = creaVentanaFiltroGussiano(filasGauss, columnasGauss, sigmaGauss);
	muestraVentana(ventanaGauss, filasGauss, columnasGauss);
	// Lee la imagen
	imagenOriginal = imread("piramide.png", IMREAD_UNCHANGED);
	if (imagenOriginal.data != NULL) // Si la imagen se leyo correctamente
	{
		// Variables imagen en escala de grises
		Mat imagenGrises;
		// Variables imagen con bordes
		Mat imagenGrisesBordes;
		Mat imagenEcualizadaBordes;
		Mat imagenMagnitudBordes;
		Mat imagenSuprimidaBordes;
		Mat imagenUmbralizadaBordes;
		// Variables imagen con filtro
		Mat imagenSuavizada;
		// Variables imagen ecualizada
		Mat imagenEcualizada;
		// Variables de los gradientes
		float** ventanaSobelGx;
		float** ventanaSobelGy;
		int filasSobel = 3;
		int columnasSobel = 3;
		Mat imagenGx;
		Mat imagenGy;
		Mat imagenMagnitudG;
		Mat imagenDireccionG;
		// Variables deteccion de borde Canny
		int filasVecindadCanny = 3;
		int columnasVecindadCanny = 3;
		Mat imagenSuprimida;
		Mat imagenUmbralizada;
		Mat imagenDeteccionBorde;

		// Crea la imagen en escala de grises (NTSC)
		imagenGrises = convierteEscalaGrisesNTSC(imagenOriginal);

		// Crea la imagen con bordes a partir de la de escala de grises 
		imagenGrisesBordes = agregaBordesEscalaGrises(imagenGrises, filasGauss, columnasGauss);

		// Aplica el filtro de suavizado
		imagenSuavizada = aplicaVentanaEscalaGrises(ventanaGauss, filasGauss, columnasGauss, imagenGrisesBordes);

		// Ecualiza la imagen
		imagenEcualizada = ecualizaImagenEscalaGrises(imagenSuavizada, 0, 255);

		// Crea la imagen con bordes a partir de la ecualizada
		imagenEcualizadaBordes = agregaBordesEscalaGrises(imagenEcualizada, filasSobel, columnasSobel);

		// Obtiene la magnitud y direccion de los gradientes de la imagen
		ventanaSobelGx = creaVentanaXSobel();
		muestraVentana(ventanaSobelGx, filasSobel, columnasSobel);
		ventanaSobelGy = creaVentanaYSobel();
		muestraVentana(ventanaSobelGy, filasSobel, columnasSobel);
		imagenGx = aplicaVentanaEscalaGrises(ventanaSobelGx, filasSobel, columnasSobel, imagenEcualizadaBordes);
		imagenGy = aplicaVentanaEscalaGrises(ventanaSobelGy, filasSobel, columnasSobel, imagenEcualizadaBordes);
		imagenMagnitudG = creaImagenMagnitudGradiente(imagenGx, imagenGy);
		imagenDireccionG = creaImagenDireccionGradiente(imagenGx, imagenGy);

		// Detecta los bordes mediante Canny
		imagenMagnitudBordes = agregaBordesEscalaGrises(imagenMagnitudG, filasVecindadCanny, columnasVecindadCanny);
		imagenSuprimida = suprimePixelesNoMaximos(imagenMagnitudBordes, imagenDireccionG);
		imagenUmbralizada = umbralizaPixelesImagen(imagenSuprimida, 0.35, 0.9);
		imagenUmbralizadaBordes = agregaBordesEscalaGrises(imagenUmbralizada, filasVecindadCanny, columnasVecindadCanny);
		imagenDeteccionBorde = histerizaBordesImagen(imagenUmbralizadaBordes, filasVecindadCanny, columnasVecindadCanny);

		// Muestra las imagenes
		muestraImagen(imagenOriginal, "Imagen original");
		muestraImagen(imagenGrises, "Imagen escala de grises");
		 muestraImagen(imagenGrisesBordes, "Escala de grises con bordes");
		muestraImagen(imagenSuavizada, "Imagen suavizada");
		muestraImagen(imagenEcualizada, "Imagen ecualizada");
		 muestraImagen(imagenEcualizadaBordes, "Imagen ecualizada con bordes");
		 muestraImagen(imagenGx, "Imagen Gx");
		 muestraImagen(imagenGy, "Imagen Gy");
		muestraImagen(imagenMagnitudG, "Imagen resultado de |G|");
		 muestraImagen(imagenDireccionG, "Imagen direccion G");
		 muestraImagen(imagenMagnitudBordes, "Imagen magnitud G con bordes");
		 muestraImagen(imagenSuprimida, "Imagen supresion no maximos");
		 muestraImagen(imagenUmbralizada, "Imagen umbralizada");
		 muestraImagen(imagenUmbralizadaBordes, "Imagen umbralizada con bordes");
		muestraImagen(imagenDeteccionBorde, "Imagen deteccion de borde Canny");

		// Guarda las imagenes
		imwrite("imagenGrises.png", imagenGrises);
		imwrite("imagenSuavizada.png", imagenSuavizada);
		imwrite("imagenEcualizada.png", imagenEcualizada);
		imwrite("imagenMagnitudGradiente.png", imagenMagnitudG);
		imwrite("imagenDeteccionBordesCanny.png", imagenDeteccionBorde);
	}
	else // Si la imagen no se leyo correctamente
	{
		cout << "Error al cargar la imagen";
		exit(1);
	}
	// Espera
	waitKey(0);
	return 0;
}