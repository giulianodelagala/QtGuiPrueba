#pragma once

#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <QPointF>
#include <QGraphicsEllipseItem>
#include <qgraphicsitem.h>
#include <QGraphicsSceneMouseEvent>
#include <qevent.h>
#include <qdebug.h>


class DiagramScene : public QGraphicsScene 
{
	Q_OBJECT

public:

	enum Mode {InsertNode, InsertPipe, SelectItem};

	explicit DiagramScene(QObject* parent = nullptr);

protected:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

signals:
	void nodoSeleccionado(QGraphicsItem* item);
	void pipeSeleccionado(QGraphicsItem* item);
	void sinSeleccion();

public slots:
	void setMode(Mode mode);


private:
	Mode myMode;

};

