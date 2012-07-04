/*********************** Information *************************\
| $HeadURL$
|
| Author: Jo2003
|
| Begin: 11.02.2011 / 15:00
|
| Last edited by: $Author$
|
| $Id$
\*************************************************************/
#include "cshortcutgrabber.h"

/* -----------------------------------------------------------------\
|  Method: CShortCutGrabber / constructor
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: construct widget, init values
|
|  Parameters: pointer to parent widget
|
|  Returns: --
\----------------------------------------------------------------- */
CShortCutGrabber::CShortCutGrabber(QWidget *parent, int row)
    : QWidget(parent), m_num(0), m_lineEdit(new QLineEdit(this))
{
   QHBoxLayout *layout = new QHBoxLayout(this);
   iRow = row;
   layout->addWidget(m_lineEdit);
   layout->setMargin(0);
   m_lineEdit->installEventFilter(this);
   m_lineEdit->setReadOnly(true);
   m_lineEdit->setFocusProxy(this);
   setFocusPolicy(m_lineEdit->focusPolicy());
   setAttribute(Qt::WA_InputMethodEnabled);
}

/* -----------------------------------------------------------------\
|  Method: eventFilter
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: catch right click on lineEdit
|
|  Parameters: pointer to object and event
|
|  Returns: bool
\----------------------------------------------------------------- */
bool CShortCutGrabber::eventFilter(QObject *o, QEvent *e)
{
   if ((o == m_lineEdit) && (e->type() == QEvent::ContextMenu))
   {
      QContextMenuEvent *c = static_cast<QContextMenuEvent *>(e);
      QMenu *menu = m_lineEdit->createStandardContextMenu();
      const QList<QAction *> actions = menu->actions();
      QListIterator<QAction *> itAction(actions);

      while (itAction.hasNext())
      {
         QAction *action = itAction.next();
         action->setShortcut(QKeySequence());
         QString actionString = action->text();
         const int pos = actionString.lastIndexOf(QLatin1Char('\t'));

         if (pos > 0)
         {
            actionString.remove(pos, actionString.length() - pos);
         }
         action->setText(actionString);
      }

      QAction *actionBefore = 0;

      if (actions.count() > 0)
      {
         actionBefore = actions[0];
      }

      QAction *clearAction = new QAction(tr("Clear Shortcut"), menu);
      menu->insertAction(actionBefore, clearAction);
      clearAction->setEnabled(!m_keySequence.isEmpty());
      connect(clearAction, SIGNAL(triggered()), this, SLOT(slotClearShortcut()));
      QAction *undoAction = new QAction(tr("Undo ShortCut"), menu);
      menu->insertAction(actionBefore, undoAction);
      undoAction->setEnabled(true);
      connect(undoAction, SIGNAL(triggered()), this, SLOT(slotUndoShortCut()));
      QAction *resetAction = new QAction(tr("Reset ShortCut"), menu);
      menu->insertAction(actionBefore, resetAction);
      resetAction->setEnabled(true);
      connect(resetAction, SIGNAL(triggered()), this, SLOT(slotResetShortCut()));
      menu->insertSeparator(actionBefore);
      menu->exec(c->globalPos());
      delete menu;
      e->accept();
      return true;
   }

   return QWidget::eventFilter(o, e);
}

/* -----------------------------------------------------------------\
|  Method: slotClearShortcut [slot]
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: clear stored shortcut
|
|  Parameters: --
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::slotClearShortcut()
{
   if (m_keySequence.isEmpty())
   {
      return;
   }
   setKeySequence(QKeySequence());
   emit keySequenceChanged(m_keySequence, m_currkeySequence, iRow);
}

/* -----------------------------------------------------------------\
|  Method: handleKeyEvent
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: key pressed
|
|  Parameters: key event
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::handleKeyEvent(QKeyEvent *e)
{
   int nextKey = e->key();
   if ((nextKey == Qt::Key_Control) || (nextKey == Qt::Key_Shift) ||
       (nextKey == Qt::Key_Meta)    || (nextKey == Qt::Key_Alt)   ||
       (nextKey == Qt::Key_Super_L) || (nextKey == Qt::Key_AltGr))
   {
      return;
   }

   nextKey |= translateModifiers(e->modifiers(), e->text());
   int k0 = m_keySequence[0];
   int k1 = m_keySequence[1];
   int k2 = m_keySequence[2];
   int k3 = m_keySequence[3];

   switch (m_num)
   {
   case 0:
      k0 = nextKey;
      k1 = 0;
      k2 = 0;
      k3 = 0;
      break;

   case 1:
      k1 = nextKey;
      k2 = 0;
      k3 = 0;
      break;

   case 2:
      k2 = nextKey;
      k3 = 0;
      break;

   case 3:
      k3 = nextKey;
      break;

   default:
      break;
    }

   ++m_num;

   if (m_num > 3)
   {
      m_num = 0;
   }

   m_keySequence = QKeySequence(k0, k1, k2, k3);
   m_lineEdit->setText(m_keySequence.toString(QKeySequence::NativeText));
   e->accept();
   m_undokeySequence = m_currkeySequence; // remember previos keySequence for undo
   emit keySequenceChanged(m_keySequence, m_currkeySequence, iRow);
}

/* -----------------------------------------------------------------\
|  Method: setKeySequence
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: set internal key sequence, update lineEdit
|
|  Parameters: new key sequence, original key sequence
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::setKeySequence (const QKeySequence &costSeq, const QKeySequence &orgSeq)
{
   if (!orgSeq.isEmpty())
   {
      m_orgkeySequence = orgSeq;
   }

   if (costSeq == m_keySequence)
   {
      return;
   }
   m_num = 0;
   m_keySequence = costSeq;
   m_lineEdit->setText(m_keySequence.toString(QKeySequence::NativeText));
}

/* -----------------------------------------------------------------\
|  Method: keySequence
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: export internal stored key sequence
|
|  Parameters: new key sequence
|
|  Returns: key sequence
\----------------------------------------------------------------- */
QKeySequence CShortCutGrabber::keySequence() const
{
   return m_keySequence;
}

/* -----------------------------------------------------------------\
|  Method: shortCutString
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: get shortcut in string form
|
|  Parameters:
|
|  Returns: shortcut string
\----------------------------------------------------------------- */
QString CShortCutGrabber::shortCutString() const
{
   return m_keySequence.toString(QKeySequence::NativeText);
}

/* -----------------------------------------------------------------\
|  Method: translateModifiers
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: which modifiers are pressed
|
|  Parameters: KeyEvent modifiers, keyEvent text
|
|  Returns: modifier flags
\----------------------------------------------------------------- */
int CShortCutGrabber::translateModifiers(Qt::KeyboardModifiers state, const QString &text) const
{
   int result = 0;
   if ((state & Qt::ShiftModifier) && ((text.size() == 0) || !text.at(0).isPrint() || text.at(0).isLetter() || text.at(0).isSpace()))
   {
      result |= Qt::SHIFT;
   }

   if (state & Qt::ControlModifier)
   {
      result |= Qt::CTRL;
   }

   if (state & Qt::MetaModifier)
   {
      result |= Qt::META;
   }

   if (state & Qt::AltModifier)
   {
      result |= Qt::ALT;
   }

   return result;
}

/* -----------------------------------------------------------------\
|  Method: focusInEvent
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: line gets focus
|
|  Parameters: focus event
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::focusInEvent(QFocusEvent *e)
{
   m_lineEdit->event(e);
   m_lineEdit->selectAll();
   m_lineEdit->setStyleSheet("QLineEdit {background-color: #c3ffc7}");
   QWidget::focusInEvent(e);
   m_currkeySequence = m_keySequence; // remember current keySquence
}

/* -----------------------------------------------------------------\
|  Method: focusOutEvent
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: line loses focus
|
|  Parameters: focus event
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::focusOutEvent(QFocusEvent *e)
{
   m_num = 0;
   m_lineEdit->event(e);
   m_lineEdit->setStyleSheet("QLineEdit {background-color: white}");
   QWidget::focusOutEvent(e);
   emit emptyFocusOut(m_keySequence, m_currkeySequence, iRow); //check for empty keySequence
}

/* -----------------------------------------------------------------\
|  Method: keyPressEvent
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: key pressed
|
|  Parameters: key event
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::keyPressEvent(QKeyEvent *e)
{
   handleKeyEvent(e);
   e->accept();
}

/* -----------------------------------------------------------------\
|  Method: keyReleaseEvent
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: key released
|
|  Parameters: key event
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::keyReleaseEvent(QKeyEvent *e)
{
   m_lineEdit->event(e);
}

/* -----------------------------------------------------------------\
|  Method: event
|  Begin: 11.02.2011 / 15:05
|  Author: Jo2003
|  Description: catch incoming events
|
|  Parameters: event
|
|  Returns: --
\----------------------------------------------------------------- */
bool CShortCutGrabber::event(QEvent *e)
{
   if ((e->type() == QEvent::Shortcut)
      || (e->type() == QEvent::ShortcutOverride)
      || (e->type() == QEvent::KeyRelease))
   {
      e->accept();
      return true;
   }

   return QWidget::event(e);
}

/* -----------------------------------------------------------------\
|  Method: setTarget
|  Begin: 15.03.2011 / 15:10
|  Author: Jo2003
|  Description: set target this shortcut belongs to
|
|  Parameters: target string
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::setTarget(const QString &str)
{
   sTarget = str;
}

/* -----------------------------------------------------------------\
|  Method: setSlot
|  Begin: 15.03.2011 / 15:10
|  Author: Jo2003
|  Description: set slot this shortcut belongs to
|
|  Parameters: slot string
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::setSlot(const QString &str)
{
   sSlot = str;
}

/* -----------------------------------------------------------------\
|  Method: target
|  Begin: 15.03.2011 / 15:10
|  Author: Jo2003
|  Description: get target string belonging to this shortcut
|
|  Parameters: --
|
|  Returns: target string
\----------------------------------------------------------------- */
QString CShortCutGrabber::target() const
{
   return sTarget;
}

/* -----------------------------------------------------------------\
|  Method: slot
|  Begin: 15.03.2011 / 15:10
|  Author: Jo2003
|  Description: get slot string belonging to this shortcut
|
|  Parameters: --
|
|  Returns: slot string
\----------------------------------------------------------------- */
QString CShortCutGrabber::slot() const
{
   return sSlot;
}

/* -----------------------------------------------------------------\
|  Method: revert
|  Begin: 30.03.2011 / 16:34
|  Author: Jo2003
|  Description: revert shortcut to original value
|
|  Parameters: --
|
|  Returns: --
\----------------------------------------------------------------- */
void CShortCutGrabber::revert()
{
   if (!m_orgkeySequence.isEmpty())
   {
      m_keySequence = m_orgkeySequence;
      m_lineEdit->setText(m_keySequence.toString(QKeySequence::NativeText));
   }
}

void CShortCutGrabber::slotResetShortCut()
{
    revert();
}

void CShortCutGrabber::slotUndoShortCut()
{
    if (!m_undokeySequence.isEmpty())
    {
       m_keySequence = m_undokeySequence; // reset to previos keySequence
       m_currkeySequence = m_undokeySequence;
       m_lineEdit->setText(m_keySequence.toString(QKeySequence::NativeText));
    }
}

void CShortCutGrabber::rollback(const QKeySequence &seq)
{
   if (!seq.isEmpty())
   {
      m_keySequence = seq;
      m_currkeySequence = seq; // set seq current keySequence
      m_lineEdit->setText(m_keySequence.toString(QKeySequence::NativeText));
   }
}

void CShortCutGrabber::markRed()
{
   m_lineEdit->setStyleSheet("QLineEdit {background-color: red}");
}

void CShortCutGrabber::unMark()
{
   if (hasFocus())
   {
      m_lineEdit->setStyleSheet("QLineEdit {background-color: #c3ffc7}");
   }
   else
   {
      m_lineEdit->setStyleSheet("QLineEdit {background-color: white}");
   }
}