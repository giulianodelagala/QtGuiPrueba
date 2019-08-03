#include "QtGuiPrueba.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	
	
	//createFormNode();

	scene = new DiagramScene(this);
	scene->setSceneRect(QRectF(0, 0, 2000, 2000));
	

	QHBoxLayout* layout = new QHBoxLayout;
	view = new QGraphicsView(scene);
	layout->addWidget(view);

	//tabla = new QTableWidget(4, 1);
	//layout->addWidget(tabla);

	groupFormNode = new GroupFormNode();
	groupFormNode->setVisible(false);
	layout->addWidget(groupFormNode);

	groupFormPipe = new GroupFormPipe();
	groupFormPipe->setVisible(false);
	layout->addWidget(groupFormPipe);

	widget = new QWidget;
	widget->setLayout(layout);

	createActions();
	createToolbar();
	

	setCentralWidget(widget);
	setWindowTitle(tr("Calculo de Tuberias"));
	//setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::AddNode()
{
	scene->setMode(DiagramScene::InsertNode);
}

void MainWindow::AddPipe()
{
	scene->setMode(DiagramScene::InsertPipe);
	
	if (scene->selectedItems().count() != 2)
		return;

	if (scene->selectedItems().first()->type() != 65537 ||
		scene->selectedItems().last()->type() != 65537)
		return;

	Node* o1 = dynamic_cast<Node*>(scene->selectedItems().first());
	Node* o2 = dynamic_cast<Node*>(scene->selectedItems().last());

	Pipe* pipe = new Pipe(o1, o2);
	scene->addItem(pipe);

	//Agregar data de tuberia a cada nodo
	o1->vec_pipe.push_back(pipe);
	o2->vec_pipe.push_back(pipe);
	
}

void MainWindow::SelectItem()
{
	scene->setMode(DiagramScene::SelectItem);
	/*
	if (scene->selectedItems().isEmpty())
		return;

	QGraphicsItem* item = scene->selectedItems().first();
	
	switch (item->type())
	{
	case 65537: //Node
		nodo_sele = dynamic_cast<Node*>(item);
		GroupNodo(nodo_sele);
		break;

	case 65538: //Pipe
		pipe_sele = dynamic_cast<Pipe*>(item);
		GroupPipe(pipe_sele);
		break;
	default:
		break;
	}
	*/
}

void MainWindow::Calcular()
{
	// Creacion de Vectores
	if (!CreacionVectores())
		qDebug() << "Error";

	//Creacion de matriz de conectividad
	if (red != nullptr)
		delete red;
	red = new Red(vec_fuente, vec_nodo, vec_pipe);
	
	//qDebug() << "pipe" << vec_pipe.size();
	//qDebug() << "nodo" << vec_nodo.size();
	//qDebug() << "fuente" << vec_fuente.size();

	PublicarResultados();

}

bool MainWindow::CreacionVectores()
{
	//Retornar si creacion es correcta
	
	vec_fuente.clear();
	vec_nodo.clear();
	vec_pipe.clear();
	int nodo_count, fuente_count, pipe_count;
	nodo_count = fuente_count = pipe_count = 0;
	
	for each (QGraphicsItem * item in scene->items())
	{	
		if (item->type() == 65537) //Nodos
		{
			//Nodos
			Node* node = dynamic_cast<Node*>(item);
			if (node->is_fuente)
			{
				node->id = fuente_count;
				vec_fuente.push_back(node);
				fuente_count++;
			}
			else
			{
				node->id = nodo_count;
				vec_nodo.push_back(node);
				nodo_count++;
			}
		}
		else if (item->type() == 65538)
		{	//Tuberias
			Pipe* pipe = dynamic_cast<Pipe*>(item);
			pipe->id = pipe_count;
			vec_pipe.push_back(pipe);
			pipe_count++;
		}
	
	}
	return true;
}

void MainWindow::ActualizarPipe()
{
	pipe_sele->diam = groupFormPipe->diamLineEdit->text().toDouble();
	pipe_sele->longitud = groupFormPipe->longitudLineEdit->text().toDouble();
	pipe_sele->sum_km = groupFormPipe->kmLineEdit->text().toDouble();
	pipe_sele->ks = groupFormPipe->ksLineEdit->text().toDouble();
}

void MainWindow::ActualizarNodo()
{
	nodo_sele->cota = groupFormNode->cotaLineEdit->text().toDouble();
	nodo_sele->demanda = groupFormNode->demandaLineEdit->text().toDouble();
	nodo_sele->is_fuente = groupFormNode->checkIsFuente->isChecked();
}

void MainWindow::createActions()
{
	selectItem = new QAction(QIcon("Resources/select.png"), tr("Seleccionar Item"), this);
	connect(selectItem, &QAction::triggered, this, &MainWindow::SelectItem);

	addNodeAction = new QAction(QIcon("Resources/node.png"), tr("Añadir Nodo"), this);
	connect(addNodeAction, &QAction::triggered, this, &MainWindow::AddNode);

	addPipeAction = new QAction(QIcon("Resources/pipe.png"), tr("Añadir Tuberia"), this);
	connect(addPipeAction, &QAction::triggered, this, &MainWindow::AddPipe);

	calcularAction = new QAction(QIcon("Resources/start.png"), tr("Calcular"), this);
	connect(calcularAction, &QAction::triggered, this, &MainWindow::Calcular);

	//Conexiones de Propiedades de Nodo y Tuberia
	connect(groupFormNode->aceptarPushButton, &QPushButton::clicked, this, &MainWindow::ActualizarNodo);
	connect(groupFormPipe->aceptarPushButton, &QPushButton::clicked, this, &MainWindow::ActualizarPipe);

	connect(scene, &DiagramScene::nodoSeleccionado, this, &MainWindow::nodoSeleccionado);
	connect(scene, &DiagramScene::pipeSeleccionado, this, &MainWindow::pipeSeleccionado);
	connect(scene, &DiagramScene::sinSeleccion, this, &MainWindow::sinSeleccion);
}

void MainWindow::createToolbar()
{
	editToolBar = addToolBar(tr("Edición"));
	editToolBar->addAction(selectItem);
	editToolBar->addAction(addNodeAction);
	editToolBar->addAction(addPipeAction);
	editToolBar->addAction(calcularAction);

}

void MainWindow::createTable()
{
	tabla->setFixedWidth(200);
	tabla->setColumnWidth(0,150);
	
}

void MainWindow::PublicarResultados()
{
	//Resultados en Nodos
	for (auto it = vec_nodo.begin(); it != vec_nodo.end(); ++it)
	{

	}
	for (int i = 0; i < red->Hi.size(); ++i)
	{
		vec_nodo[i]->piezometrica = red->Hi[i];

	}


}

/*
void MainWindow::TablaNodo(Node* it)
{
	tabla->setRowCount(5);
	tabla->setHorizontalHeaderLabels({ "Nodo" });
	tabla->setVerticalHeaderLabels({ "",
		"Cota m.s.n.m",
		"Demanda m3/h",
		"Coord. x",
		"Coord. y" });
	
	tabla->setItem(1, 0, new QTableWidgetItem(QString::number(it->cota)));
	tabla->setItem(2, 0, new QTableWidgetItem(QString::number(it->demanda)));
	tabla->setItem(3, 0, new QTableWidgetItem(QString::number(it->posi.x())));
	tabla->setItem(4, 0, new QTableWidgetItem(QString::number(it->posi.y())));

	//qDebug() << item->type();
}
*/

/*
void MainWindow::TablaPipe(Pipe* it)
{
	tabla->setRowCount(5);
	tabla->setHorizontalHeaderLabels({ "Tuberia" });
	tabla->setVerticalHeaderLabels({ "",
		"Diametro m",
		"longitud m",
		"ks",
		"hf" });

	tabla->setItem(1, 0, new QTableWidgetItem(QString::number(it->diam)));
	tabla->setItem(2, 0, new QTableWidgetItem(QString::number(it->longitud)));
	tabla->setItem(3, 0, new QTableWidgetItem(QString::number(it->ks)));
	tabla->setItem(4, 0, new QTableWidgetItem(QString::number(it->hf)));
}
*/

void MainWindow::nodoSeleccionado(QGraphicsItem* item)
{
	Node* it = dynamic_cast<Node*>(item);
	nodo_sele = it;
	
	//if (it->isSelected())
		//it->setPen(QColor(255,0,0));

	groupFormNode->setVisible(true);
	groupFormPipe->setVisible(false);

	groupFormNode->cotaLineEdit->setText(QString::number(it->cota));
	groupFormNode->demandaLineEdit->setText(QString::number(it->demanda));
	groupFormNode->coordxLineEdit->setText(QString::number(it->posi.x()));
	groupFormNode->coordyLineEdit->setText(QString::number(it->posi.y()));

	if (it->is_fuente)
		groupFormNode->checkIsFuente->setChecked(true);
	else
		groupFormNode->checkIsFuente->setChecked(false);

	//Resultados
	groupFormNode->idLineEdit->setText(QString::number(it->id));
	groupFormNode->piezometricaLineEdit->setText(QString::number(it->piezometrica));
	
}

void MainWindow::pipeSeleccionado(QGraphicsItem* item)
{
	Pipe* it = dynamic_cast<Pipe*>(item);
	pipe_sele = it;

	groupFormPipe->setVisible(true);
	groupFormNode->setVisible(false);

	groupFormPipe->diamLineEdit->setText(QString::number(it->diam));
	groupFormPipe->longitudLineEdit->setText(QString::number(it->longitud));
	groupFormPipe->kmLineEdit->setText(QString::number(it->sum_km));
	groupFormPipe->ksLineEdit->setText(QString::number(it->ks));
	groupFormPipe->hfLineEdit->setText(QString::number(it->hf));
	groupFormPipe->hmLineEdit->setText(QString::number(it->hm));
	groupFormPipe->htLineEdit->setText(QString::number(it->ht));
}

void MainWindow::sinSeleccion()
{
	nodo_sele = nullptr;
	pipe_sele = nullptr;
	groupFormNode->setVisible(false);
	groupFormPipe->setVisible(false);
}

