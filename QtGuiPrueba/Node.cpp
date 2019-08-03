#include "Node.h"


double Pipe::Area(double diam)
{
	return (double)Constantes::PI * diam * diam / 4;
}

void Pipe::SetSumKm()
{
	sum_km = km.sum();
}

double Pipe::Velocidad(double caudal, double area)
{
	return caudal / area;
}

double Pipe::Reynolds(double diam, double velo)
{// Calculo del numero de Reynolds
	//diam : diametro en m
	//velo : velocidad en m/s
	//visco : viscocidad cinematica m^2/s
	return diam * velo / Constantes::visco;
}

long double Pipe::FactorFDarcy(double seed, double Re, double ks, double diam)
{// Calculo del factor de friccion f Darcy Weisbach
	//Metodo Newton Raphson

	if (Re <= 2200)
		return 64 / Re;
	else
	{
		double f = seed; //Semilla inicial
		double xi = 1 / sqrt(f);
		double xj = 10000; //infinito
		double Fx, Fpx = 0;

		while (abs(xj - xi) >= 0.0000001)
		{
			xi = xj;
			Fx = -2 * log10(ks / 3.7 / diam + 2.51 * xi / Re);
			Fpx = (-2 / log(10)) * ((2.51 / Re) / (ks / 3.7 / diam + 2.51 * xi / Re));
			xj = xi - (Fx - xi) / (Fpx - 1);
			//if (xj < 0.000001)
				//xj = 0.000001;
		}
		//if ( std::isnan (1 / (xj * xj)))
			//return 0.00001;
		return 1 / (xj * xj);
	}
}

double Pipe::CalculoPerdidasFriccion(double f, double longitud, double velo, double diam)
{
	return f * longitud * velo * velo / diam / 2 / Constantes::gravedad;
}

double Pipe::CalculoPerdidasMenores(double sum_km, double velo)
{
	return sum_km * velo * velo / 2 / Constantes::gravedad;
}
