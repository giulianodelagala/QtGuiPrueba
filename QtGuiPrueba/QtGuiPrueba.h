#pragma once

//#include <iostream>
#include <vector>

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <qflags.h>
#include <qtoolbar.h>
#include <qgraphicsview.h>
#include <QtWidgets>
#include <qstringlist.h>
#include <qdebug.h>
#include <qstandarditemmodel.h>
#include <qtablewidget.h>
#include <qformlayout.h>
#include <qcolor.h>

#include "DiagramScene.h"
#include "Node.h"
#include "FormNode.h"
#include "FormPipe.h"
#include "Red.h"

//#include "ui_QtGuiPrueba.h"
//using std::cout;
using std::vector;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = Q_NULLPTR);
		
	vector<Node*> vec_nodo;
	vector<Pipe*> vec_pipe;
	vector<Node*> vec_fuente;

private slots:

	void AddNode();
	void AddPipe();
	void SelectItem();
	void Calcular();
	bool CreacionVectores();

public slots:
	void ActualizarNodo();
	void ActualizarPipe();
	void nodoSeleccionado(QGraphicsItem* it);
	void pipeSeleccionado(QGraphicsItem* it);
	void sinSeleccion();

private:
	//Ui::QtGuiPruebaClass ui;
	Node* nodo_sele; //Nodo y Tuberia seleccionados
	Pipe* pipe_sele;
	Red* red = nullptr;

	QWidget* widget;

	GroupFormNode* groupFormNode;
	GroupFormPipe* groupFormPipe;

	QToolBar *editToolBar;
	QTableWidget* tabla;

	QAction* addNodeAction;
	QAction* addPipeAction;
	QAction* selectItem;
	QAction* calcularAction;
	
	DiagramScene* scene;
	QGraphicsView* view;

	void createActions();
	void createToolbar();
	void createTable();

	void PublicarResultados();

	
};
