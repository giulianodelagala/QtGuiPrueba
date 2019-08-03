#include "DiagramScene.h"
#include "Node.h"

DiagramScene::DiagramScene(QObject* parent)
	: QGraphicsScene(parent)
{
	
	/*
	myItemMenu = itemMenu;
	myMode = MoveItem;
	myItemType = DiagramItem::Step;
	line = 0;
	textItem = 0;
	myItemColor = Qt::white;
	myTextColor = Qt::black;
	myLineColor = Qt::black;
	*/
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return;

	Node* node;
	QPointF pt = mouseEvent->scenePos();
	double rad = 10;
	QGraphicsItem* item;
	
	Pipe* pipe;
	Node* o1, * o2;
	
	switch (myMode)
	{
	case DiagramScene::InsertNode:		
		node = new Node(pt);
		node->setBrush(Qt::SolidPattern);
		node->setRect(pt.x() - rad, pt.y() - rad, rad * 2.0, rad * 2.0);
		addItem(node);
		break;
	
	case DiagramScene::InsertPipe:
		/*
		if (selectedItems().count() != 2)
			return;

		if (selectedItems().first()->type() != 65537 ||
			selectedItems().last()->type() != 65537)
			return;

		o1 = dynamic_cast<Node*>(selectedItems().first());
		o2 = dynamic_cast<Node*>(selectedItems().last());

		pipe = new Pipe(o1, o2);
		addItem(pipe);

		//Agregar data de tuberia a cada nodo
		o1->vec_pipe.push_back(pipe);
		o2->vec_pipe.push_back(pipe);
		*/
		break;

	case DiagramScene::SelectItem:
		
		if (selectedItems().isEmpty())
		{
			emit sinSeleccion();
			return;
		}

		item = selectedItems().first();
		
		if (item->type() == 65537) //Node
			emit nodoSeleccionado(item);
		else if (item->type() == 65538) //Pipe
			emit pipeSeleccionado(item);
		else
			emit sinSeleccion();
		break;
		
	default:
		;
	}
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//qDebug() << "View" << mouseEvent->scenePos();
	
}

void DiagramScene::setMode(Mode mode)
{
	myMode = mode;
}

