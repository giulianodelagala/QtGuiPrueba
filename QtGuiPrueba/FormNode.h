#pragma once

//#include <qobject.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qframe.h>

class GroupFormNode : public QGroupBox
{
	Q_OBJECT

public:

	QLineEdit* idLineEdit = new QLineEdit;
	QLineEdit* cotaLineEdit = new QLineEdit;
	QLineEdit* demandaLineEdit = new QLineEdit;
	QLineEdit* coordxLineEdit = new QLineEdit;
	QLineEdit* coordyLineEdit = new QLineEdit;

	QLabel* demandaLabel = new QLabel("Demanda m3/h");

	QPushButton* aceptarPushButton = new QPushButton("&Agregar");
	QCheckBox* checkIsFuente = new QCheckBox("Es fuente?");
	// Resultados
	QFrame* line = new QFrame;
	
	QLineEdit* piezometricaLineEdit = new QLineEdit;

	GroupFormNode(QWidget* parent = nullptr) :
		QGroupBox(parent)
	{
		setTitle(tr("Nodo"));
		formNodeLayout->addRow(new QLabel(tr("Id")), idLineEdit);
		formNodeLayout->addRow(new QLabel(tr("Cota m")), cotaLineEdit);
		formNodeLayout->addWidget(checkIsFuente);
		formNodeLayout->addRow(demandaLabel, demandaLineEdit);
		formNodeLayout->addRow(new QLabel(tr("Coord. x")), coordxLineEdit);
		formNodeLayout->addRow(new QLabel(tr("Coord. y")), coordyLineEdit);

		line->setFrameShape(QFrame::HLine); line->setFrameShadow(QFrame::Sunken);
		formNodeLayout->addWidget(line);
		formNodeLayout->addRow(new QLabel(tr("Cota piezom.")), piezometricaLineEdit);

		formNodeLayout->addWidget(aceptarPushButton);

		idLineEdit->setDisabled(true);
		coordxLineEdit->setDisabled(true);
		coordyLineEdit->setDisabled(true);
		piezometricaLineEdit->setDisabled(true);

		setLayout(formNodeLayout);

		connect(checkIsFuente, &QCheckBox::stateChanged, this, &GroupFormNode::ActualizarIsFuente);

		//QSpinBox* ageSpinBox = new QSpinBox;
		//formNode->addRow(new QLabel(tr("Coord. x")), ageSpinBox);
	}

private:

	QFormLayout* formNodeLayout = new QFormLayout;

private slots:
	void ActualizarIsFuente()
	{//Actualizar Checkbox y Linea de Demanda
		if (checkIsFuente->isChecked())
		{
			demandaLineEdit->setVisible(false);
			demandaLabel->setVisible(false);
		}
		else
		{
			demandaLineEdit->setVisible(true);
			demandaLabel->setVisible(true);
		}
			
	}
};

