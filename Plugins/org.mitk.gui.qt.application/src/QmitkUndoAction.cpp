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

#include "QmitkUndoAction.h"

#include "internal/org_mitk_gui_qt_application_Activator.h"

#include <mitkWorkbenchUtil.h>

#include <mitkUndoController.h>
#include <mitkVerboseLimitedLinearUndo.h>

#include <berryIPreferences.h>

#include <QApplication>

class QmitkUndoActionPrivate
{
public:

  void init ( berry::IWorkbenchWindow* window, QmitkUndoAction* action )
  {
    m_Window = window;
    action->setText(QApplication::translate("QmitkUndoAction","&Undo"));
    action->setToolTip(QApplication::translate("QmitkUndoAction","Undo the last action (not supported by all modules)"));

    QObject::connect(action, SIGNAL(triggered(bool)), action, SLOT(Run()));
  }

  berry::IWorkbenchWindow* m_Window;
};

QmitkUndoAction::QmitkUndoAction(berry::IWorkbenchWindow::Pointer window)
  : QAction(0), d(new QmitkUndoActionPrivate)
{
  d->init(window.GetPointer(), this);
}

QmitkUndoAction::QmitkUndoAction(const QIcon & icon, berry::IWorkbenchWindow::Pointer window)
  : QAction(0), d(new QmitkUndoActionPrivate)
{
  d->init(window.GetPointer(), this);
  this->setIcon(icon);
}

QmitkUndoAction::QmitkUndoAction(const QIcon& icon, berry::IWorkbenchWindow* window)
  : QAction(0), d(new QmitkUndoActionPrivate)
{
  d->init(window, this);
  this->setIcon(icon);
}

QmitkUndoAction::~QmitkUndoAction()
{
}

void QmitkUndoAction::Run()
{
  mitk::UndoModel* model = mitk::UndoController::GetCurrentUndoModel();
  if (model)
  {
    if (mitk::VerboseLimitedLinearUndo* verboseundo = dynamic_cast<mitk::VerboseLimitedLinearUndo*>( model ))
    {
      mitk::VerboseLimitedLinearUndo::StackDescription descriptions =
        verboseundo->GetUndoDescriptions();
      if (descriptions.size() >= 1)
      {
        MITK_INFO << tr("Undo ").toStdString() << descriptions.front().second;
      }
    }
    model->Undo();
  }
  else
  {
    MITK_ERROR << tr("No undo model instantiated").toStdString();
  }
}