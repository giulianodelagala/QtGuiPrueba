#pragma once


#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
//#include <limits>

#include <Sparse>

#include "Node.h"


typedef Eigen::SparseMatrix<double, Eigen::RowMajor> SpMat;
typedef Eigen::Triplet<double> TripletD;

using std::vector;
using std::cout;
using Eigen::Array; using Eigen::Dynamic;
using Eigen::VectorXd; using Eigen::DiagonalMatrix;
using Eigen::MatrixXd;

/*
double Redondeo(double x)
{
	return ceilf(x * 1000000) / 1000000;
}
*/

class Red
{
public:

	int nt; //numero de tuberias en la red
	int nn; //numero de nodos con altura piezometrica desconocida
	int ns; //numero de nodos altura piezometrica fija

	SpMat A12; //matriz de conectividad nt x nn
	SpMat A21; //matriz transpuesta de A12
	SpMat A10; //matriz tramo a nodos ns piezo. fija nt x ns
	//SpMat A11; //matriz de coeficientes alpha beta ...

	SpMat N; //matriz DarcyWeisbach nt x nt;
	SpMat I;

	VectorXd Qi; // vector caudal nt x 1
	VectorXd Hi; //vector alturas piezometricas desconocidas nn x 1
	VectorXd Hj;
	VectorXd Ho; //vector alturas piezometricas conocidas ns x 1
	VectorXd q; //vector de consumo nn x 1;

	vector<Pipe*> vec_pipe;
	vector<Node*> vec_node;
	vector<Node*> vec_fuente;

	void Imprimir();
	void SetMatrices(SpMat A12, SpMat A10, VectorXd q, VectorXd Ho);

	Red() {}
	Red(vector<Node*> vec_fuente, vector<Node*> vec_node, vector<Pipe*> vec_pipe)
	{
		this->vec_fuente = vec_fuente;
		this->vec_node = vec_node;
		this->vec_pipe = vec_pipe;

		nt = vec_pipe.size();
		nn = vec_node.size();
		ns = vec_fuente.size();

		Qi.resize(nt);
		Hi.resize(nn); Hj.resize(nn) ;
		Ho.resize(ns);		q.resize(nn);
		N.resize(nt, nt);
		N.setIdentity();
		N = N * 2;
		I.resize(nt, nt);
		I.setIdentity();

		if (CreacionA10A12())
			Imprimir();
		if (CreacionVectores())
		{
			cout << "\n..q..\n" << q << "\n..Ho..\n" << Ho;
		}
		CalculoMetGradiente();
			
	}
	~Red() {}

	//private:

	double Alpha(double factorF, double longitud, double diam, double sum_km, double area);

	void CalculoMetGradiente();

private:
	bool CreacionA10A12();
	bool CreacionVectores();
};

inline void Red::Imprimir()
{
	cout << "A12\n" << A12;
	//cout << A21;
	cout << "\nA10\n" << A10;
	//cout << q;
	//cout << N;

}

inline void Red::SetMatrices(SpMat A12, SpMat A10, VectorXd q, VectorXd Ho)
{
	//Cuando la creacion de las matrices es manual en main
	//TODO Eliminar cuando no sea necesario
	this->A12 = A12;
	A21 = this->A12.transpose();
	this->A10 = A10;
	this->q = q;
	this->Ho = Ho;
}


inline double Red::Alpha(double factorF, double longitud, double diam, double sum_km, double area)
{
	//Calculo del factor alpha
	//sum_kn : sumatoria de perdidas menores
	return (factorF * longitud / diam + sum_km) / (2 * gravedad * area * area);
}

inline void Red::CalculoMetGradiente()
{
	VectorXd vec_Q, vec_f, vec_velo, vec_hf, vec_hm,
		vec_ht, vec_ks, vec_diam, vec_long, vec_sumkm,
		vec_alpha;

	//Inicializar vectores de caudal longitud y diametro
	vec_long.resize(nt); vec_diam.resize(nt);
	vec_Q.resize(nt); vec_ks.resize(nt);
	vec_velo.resize(nt); vec_f.resize(nt);
	vec_hf.resize(nt); vec_hm.resize(nt);
	vec_sumkm.resize(nt); vec_ht.resize(nt);
	vec_alpha.resize(nt);

	for (int i = 0; i < nt; ++i)
	{
		vec_Q[i] = 0.1;
		vec_f[i] = 0.001;
		
		vec_long[vec_pipe[i]->id] = vec_pipe[i]->longitud;
		vec_diam[vec_pipe[i]->id] = vec_pipe[i]->diam;
		vec_ks[vec_pipe[i]->id] = vec_pipe[i]->ks;
		vec_sumkm[vec_pipe[i]->id] = vec_pipe[i]->sum_km;
	}

	cout << "\nvec_long\n" << vec_long;
	cout << "\nvec_diam\n" << vec_diam;
	cout << "\nvec_ks\n" << vec_ks;
	cout << "\nvec_sumkm\n" << vec_sumkm;

	// Inicio Primera Iteracion
	int it = 0;
	double error = 1;
	do
	{
		for (int i = 0; i < nt; ++i)
		{
			vec_velo[i] = Hidra::Velocidad(vec_Q[i], Hidra::Area(vec_diam[i]));
			vec_f[i] = Hidra::FactorFDarcy(vec_f[i], Hidra::Reynolds(vec_diam[i], vec_velo[i]), vec_ks[i], vec_diam[i]);
			vec_hf[i] = Hidra::CalculoPerdidasFriccion(vec_f[i], vec_long[i], vec_velo[i], vec_diam[i]);
			vec_hm[i] = Hidra::CalculoPerdidasMenores(vec_sumkm[i], vec_velo[i]);
			vec_ht = vec_hf + vec_hm;

			vec_alpha[i] = (vec_f[i] * vec_long[i] / vec_diam[i] + vec_sumkm[i]) /
				(2 * gravedad * pow(Hidra::Area(vec_diam[i]), 2));
		}

		//vec_f = vec_f.unaryExpr(&Redondeo);

		cout << "\nItera" << it << "\n";
		cout << "vec_Q\n" << vec_Q << "\n";
		cout << "vec_velo\n" << vec_velo << "\n";
		cout << "vec_f\n" << vec_f << "\n";
		cout << "alpha\n" << vec_alpha << "\n";

		SpMat A11; A11.resize(nt, nt);
		A11 = vec_alpha.cwiseProduct(vec_Q).asDiagonal();

		cout << "A11\n" << A11;
		

		MatrixXd temp0 = N * A11;
		temp0 = temp0.inverse();

		MatrixXd temp1 = A21 * temp0 * A12;
		temp1 = -1 * temp1.inverse();

		VectorXd temp2 = (A21 * temp0) * (A11 * vec_Q + A10 * Ho);


		VectorXd temp3;
		if (it == 0)
		{
			temp3 = temp2 - ((A21 * vec_Q) - q);
			Hi = temp1 * temp3; //vector resultado alturas piezometricas
		}
		else
			Hi = temp1 * temp2;

		VectorXd temp10 = temp0 * (A12 * Hi + A10 * Ho);



		Qi = ((I - (temp0 * A11)) * vec_Q) - temp10;
		//Qi = Qi.unaryExpr(&Redondeo);


		error = (Hj - Hi).cwiseAbs().sum();
		vec_Q = Qi.cwiseAbs();
		
		Hj = Hi;
		//Fin Primera Iteracion
		cout << "Qi\n" << Qi;
		cout << "\nvec_Q\n" << vec_Q << "\n";
		cout << "Hi" << Hi << "\n";
		cout << "error" << error << "\n";
		it++;

	} while (error > 0.00001 && it != 20);

}

inline bool Red::CreacionA10A12()
{
	if (nt < (nn + ns) - 1) //Existe Nodo no conectado
		return false;

	if (ns == 0)
		return false; //No Existe fuente

	A10.resize(nt, ns);
	A12.resize(nt, nn);

	vector<TripletD> vec_A10;
	vector<TripletD> vec_A12;

	for (auto i = vec_pipe.begin(); i != vec_pipe.end(); ++i)
	{
		if ((*i)->nodos[0]->is_fuente || (*i)->nodos[1]->is_fuente)
		{
			//La tuberia esta conectada a la fuente
			if ((*i)->nodos[0]->is_fuente)
			{
				vec_A10.push_back(TripletD((*i)->id, (*i)->nodos[0]->id, -1));
				vec_A12.push_back(TripletD((*i)->id, (*i)->nodos[1]->id, 1));
			}
			else
			{
				vec_A12.push_back(TripletD((*i)->id, (*i)->nodos[0]->id, 1));
				vec_A10.push_back(TripletD((*i)->id, (*i)->nodos[1]->id, -1));
			}
		}
		else
		{	//No esta conectada a la fuente
			vec_A12.push_back(TripletD((*i)->id, (*i)->nodos[0]->id, -1));
			vec_A12.push_back(TripletD((*i)->id, (*i)->nodos[1]->id, 1));
		}
	}
	
	A10.setFromTriplets(vec_A10.begin(), vec_A10.end());
	A12.setFromTriplets(vec_A12.begin(), vec_A12.end());
	A21 = A12.transpose();
	return true;
}

inline bool Red::CreacionVectores()
{
	for (auto i = vec_node.begin(); i != vec_node.end(); ++i)
	{
		q((*i)->id) = (*i)->demanda;
	}

	for (auto i = vec_fuente.begin(); i != vec_fuente.end(); ++i)
	{
		Ho((*i)->id) = (*i)->cota;
	}

	return true;
}


