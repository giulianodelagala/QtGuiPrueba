#pragma once

#include <qformlayout.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

class GroupFormPipe : public QGroupBox
{
	Q_OBJECT

public:
	QLineEdit* idLineEdit = new QLineEdit;
	QLineEdit* diamLineEdit = new QLineEdit;
	QLineEdit* longitudLineEdit = new QLineEdit;
	QLineEdit* ksLineEdit = new QLineEdit;
	QLineEdit* kmLineEdit = new QLineEdit;
	QLineEdit* hfLineEdit = new QLineEdit;
	QLineEdit* hmLineEdit = new QLineEdit;
	QLineEdit* htLineEdit = new QLineEdit;

	QPushButton* aceptarPushButton = new QPushButton("&Agregar");

	GroupFormPipe(QWidget* parent = nullptr) :
		QGroupBox(parent)
	{
		setTitle(tr("Tuberia"));
		formPipeLayout->addRow(new QLabel(tr("Id")), idLineEdit);
		formPipeLayout->addRow(new QLabel(tr("Diametro m")), diamLineEdit);
		formPipeLayout->addRow(new QLabel(tr("longitud m3/h")), longitudLineEdit);
		formPipeLayout->addRow(new QLabel(tr("Coef.Perd.Menores")), kmLineEdit);
		formPipeLayout->addRow(new QLabel(tr("Rugos. abs. x")), ksLineEdit);
		formPipeLayout->addRow(new QLabel(tr("Perdidas friccion")), hfLineEdit);
		formPipeLayout->addRow(new QLabel(tr("Perdidas friccion")), hmLineEdit);
		formPipeLayout->addRow(new QLabel(tr("Perdidas friccion")), htLineEdit);

		formPipeLayout->addWidget(aceptarPushButton);

		idLineEdit->setDisabled(true);
		hfLineEdit->setDisabled(true);
		hmLineEdit->setDisabled(true);
		htLineEdit->setDisabled(true);

		setLayout(formPipeLayout);
	}

private:

	QFormLayout* formPipeLayout = new QFormLayout;
};

