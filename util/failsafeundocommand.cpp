#include "failsafeundocommand.h"

FailsafeUndoCommand::FailsafeUndoCommand(FailsafeUndoCommand *parent)
  : QUndoCommand(parent), _hasFailed(false) {
  if (parent)
    parent->_children.append(this);
}

FailsafeUndoCommand::FailsafeUndoCommand(const QString &text,
                                         FailsafeUndoCommand *parent)
  : QUndoCommand(text, parent), _hasFailed(false) {
  if (parent)
    parent->_children.append(this);
}


void FailsafeUndoCommand::undo() {
  _hasFailed = failsafeUndo();
}

void FailsafeUndoCommand::redo() {
  _hasFailed = failsafeRedo();
}

bool FailsafeUndoCommand::failsafeRedo() {
  QList<FailsafeUndoCommand*> succesfulChildren;
  foreach (FailsafeUndoCommand *child, _children)
    if (child->failsafeRedo()) {
      succesfulChildren.append(child);
    } else {
      setErrorString(child->errorString());
      foreach (FailsafeUndoCommand *child, succesfulChildren)
        child->failsafeUndo();
      return false;
    }
  return true;
}

bool FailsafeUndoCommand::failsafeUndo() {
  QList<FailsafeUndoCommand*> succesfulChildren;
  foreach (FailsafeUndoCommand *child, _children)
    if (child->failsafeUndo()) {
      succesfulChildren.append(child);
    } else {
      setErrorString(child->errorString());
      foreach (FailsafeUndoCommand *child, succesfulChildren)
        child->failsafeRedo();
      return false;
    }
  return true;
}
