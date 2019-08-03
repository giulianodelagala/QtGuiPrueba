#pragma once

#include <vector>

#include <QPointF>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>

#include <Dense>

#include "Constantes.h"


//using namespace Constantes;
//using Eigen::VectorXf;
class Pipe;
using std::vector;

class Node : public QGraphicsEllipseItem
{
public:
	// Qt
	unsigned int id;
	QPointF posi;
	enum {Type = UserType + 1};
	vector <Pipe*> vec_pipe;

	// Hidraulica //
	///////////////
	bool is_fuente = false;
	double cota = 0; //en msnm
	double demanda = 0; // en m^3/s

	double piezometrica = 0;
	// Fin Hidraulica //

	int type() const
	{
		return Type;
	}

	Node(QPointF pt, QGraphicsItem *parent = nullptr) :
		QGraphicsEllipseItem(parent)
	{
		this->posi = pt;
		
		setVisible(true);
		setPen(QPen());
		setBrush(Qt::SolidPattern);
		setFlag(QGraphicsItem::ItemIsSelectable, true);
		setFlag(QGraphicsItem::ItemIsMovable, true);
	}
};

class Pipe : public QGraphicsLineItem
{
public:

	enum { Type = UserType + 2 };
	Node* nodos[2]{ nullptr, nullptr };
	unsigned int id;

	// Hidraulica //
	///////////////
	
	double diam = 0.250; //diametro tuberia en m
	double area = 0; //respecto a diam
	double longitud = 100; //longitud en m
	double ks = 0.06E-3; //rugosidad absoluta en m
	//Eigen::VectorXf km; //factores perdidas menores
	double sum_km = 0; //total perdidas menores

	double hf = 0; //perdidas carga por friccion
	double hm = 0; //perdidas carga menores
	double ht = 0; //perdidas carga totales

	// Fin Hidraulica //

	int type() const
	{
		return Type;
	}

	/*
	void SetSumKm()
	{
		sum_km = km.sum();
	}
	*/

	Pipe(Node* o1, Node* o2, QGraphicsItem* parent = nullptr) :
		QGraphicsLineItem(parent)
	{
		nodos[0] = o1; nodos[1] = o2;
		setLine(o1->posi.x(), o1->posi.y(), o2->posi.x(), o2->posi.y());
		setVisible(true);
		setPen(QPen());
		setFlag(QGraphicsItem::ItemIsSelectable, true);
		setFlag(QGraphicsItem::ItemIsMovable, true);
	}
	
private:

};

