/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "QmitkEditPointDialog.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>

static void EmitWarning(const QString& message, const QString& title)
{
  MITK_WARN << message.toStdString();
  QMessageBox::warning(nullptr, title, message);
}

static bool ValidatePrecision(double value)
{
  return std::log10(std::abs(value)) + 1.0 <= std::numeric_limits<double>::digits10;
}

static bool ValidateCoordinate(const QString& name, double value)
{
  auto hasValidPrecision = ValidatePrecision(value);
  if (!hasValidPrecision)
    EmitWarning(QmitkEditPointDialog::tr("Point set %1 coordinate is outside double precision range.").arg(name), QmitkEditPointDialog::tr("Invalid point set input"));

  return hasValidPrecision;
}

struct QmitkEditPointDialogData
{
  mitk::PointSet* m_PointSet;
  mitk::PointSet::PointIdentifier m_PointId;
  QLineEdit* m_XCoord;
  QLineEdit* m_YCoord;
  QLineEdit* m_ZCoord;
  int m_Timestep;
};

QmitkEditPointDialog::QmitkEditPointDialog( QWidget * parent, Qt::WindowFlags f)
: QDialog(parent, f)
, d(new QmitkEditPointDialogData)
{
  this->setWindowTitle(tr("Edit Point Dialog"));
  d->m_PointSet = nullptr;
  d->m_Timestep = 0;
  d->m_XCoord = new QLineEdit;
  d->m_YCoord = new QLineEdit;
  d->m_ZCoord = new QLineEdit;
  QPushButton* _OKButton = new QPushButton(tr("OK"));
  connect( _OKButton, SIGNAL(clicked(bool)), this, SLOT(OnOkButtonClicked(bool)) );

  auto  layout = new QGridLayout;
  layout->addWidget(new QLabel("X: "), 0,0,1,1);
  layout->addWidget(d->m_XCoord, 0,1,1,1);
  layout->addWidget(new QLabel("Y: "), 1,0,1,1);
  layout->addWidget(d->m_YCoord, 1,1,1,1);
  layout->addWidget(new QLabel("Z: "), 2,0,1,1);
  layout->addWidget(d->m_ZCoord, 2,1,1,1);
  layout->addWidget(_OKButton, 3,0,2,1);
  this->setLayout(layout);
}

QmitkEditPointDialog::~QmitkEditPointDialog()
{
  delete d;
}

void QmitkEditPointDialog::SetPoint( mitk::PointSet* _PointSet, mitk::PointSet::PointIdentifier _PointId
                                    , int timestep)
{
  d->m_PointSet = _PointSet;
  d->m_PointId = _PointId;
  d->m_Timestep = timestep;
  mitk::PointSet::PointType p = d->m_PointSet->GetPoint(d->m_PointId, d->m_Timestep);
  d->m_XCoord->setText( QString::number( p.GetElement(0), 'f', 3 ) );
  d->m_YCoord->setText( QString::number( p.GetElement(1), 'f', 3 ) );
  d->m_ZCoord->setText( QString::number( p.GetElement(2), 'f', 3 ) );
}


void QmitkEditPointDialog::OnOkButtonClicked(bool)
{
  if(d->m_PointSet == nullptr)
  {
    MITK_WARN << tr("Pointset is 0.").toStdString();
    this->reject();
  }
  // check if digits of input value exceed double precision
  auto x = d->m_XCoord->text().toDouble();
  auto y = d->m_YCoord->text().toDouble();
  auto z = d->m_ZCoord->text().toDouble();

  if (ValidateCoordinate("X", x) && ValidateCoordinate("Y", y) && ValidateCoordinate("Z", z))
  {
    auto point = d->m_PointSet->GetPoint(d->m_PointId, d->m_Timestep);
    point.SetElement(0, x);
    point.SetElement(1, y);
    point.SetElement(2, z);
    d->m_PointSet->SetPoint(d->m_PointId, point);
    this->accept();
  }
}
