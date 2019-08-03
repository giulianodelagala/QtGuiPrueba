#pragma once

namespace Constantes
{
	static double visco = 1.141E-6;;  // viscosidad cinematica agua m^2/s
	static double PI = 3.141592653589793;
	static double gravedad = 9.81; // m/s^2

};

using namespace Constantes;

class Hidra
{
public:
	static double Velocidad(double caudal, double area);
	static double Area(double diam);
	static double Reynolds(double diam, double velo);
	static long double FactorFDarcy(double seed, double Re, double ks, double diam);
	static double CalculoPerdidasFriccion(double f, double longitud, double velo, double diam);
	static double CalculoPerdidasMenores(double sum_km, double velo);

private:

};

inline double Hidra::Area(double diam)
{
	return (double)PI * diam * diam / 4;
}

inline double Hidra::Velocidad(double caudal, double area)
{
	return caudal / area;
}

inline double Hidra::Reynolds(double diam, double velo)
{// Calculo del numero de Reynolds
	//diam : diametro en m
	//velo : velocidad en m/s
	//visco : viscocidad cinematica m^2/s
	return diam * velo / visco;
}

inline long double Hidra::FactorFDarcy(double seed, double Re, double ks, double diam)
{// Calculo del factor de friccion f Darcy Weisbach
	//Metodo Newton Raphson

	if (Re <= 2200)
		return 64 / Re;
	else
	{
		double f = seed; //Semilla inicial
		double xi = 0;
		double xj = 1 / sqrt(f); //infinito
		double Fx, Fpx = 0;

		do 
		{
			xi = xj;
			Fx = -2 * log10(ks / 3.7 / diam + 2.51 * xi / Re);
			Fpx = (-2 / log(10)) * ((2.51 / Re) / (ks / 3.7 / diam + 2.51 * xi / Re));
			xj = xi - (Fx - xi) / (Fpx - 1);
			//if (xj < 0.000001)
				//xj = 0.000001;
		} while (abs(xj - xi) >= 0.0000001);
		//if ( std::isnan (1 / (xj * xj)))
			//return 0.00001;
		return 1 / (xj * xj);
	}
}

inline double Hidra::CalculoPerdidasFriccion(double f, double longitud, double velo, double diam)
{
	return f * longitud * velo * velo / diam / 2 / gravedad;
}

inline double Hidra::CalculoPerdidasMenores(double sum_km, double velo)
{
	return sum_km * velo * velo / 2 / gravedad;
}