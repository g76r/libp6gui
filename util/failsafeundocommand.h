#ifndef FAILSAFEUNDOCOMMAND_H
#define FAILSAFEUNDOCOMMAND_H

#include <QUndoCommand>

// FIXME doc
// must never be added a non FailsafeUndoCommand child
class FailsafeUndoCommand : public QUndoCommand {
  QList<FailsafeUndoCommand*> _children;
  bool _hasFailed;
  QString _errorString;

public:
  FailsafeUndoCommand(FailsafeUndoCommand *parent = 0);
  FailsafeUndoCommand(const QString &text, FailsafeUndoCommand *parent = 0);
  // no need for a destructor since ~QUndoCommand deletes children
  bool hasFailed() const { return _hasFailed; }
  QString errorString() const { return _errorString; }
  void setErrorString(QString errorString) { _errorString = errorString; }
  /** SHOULD set error string if return false.
   * Default implementation: calls every child until one of them returns false.
   * If this happens, calls failsafeUndo() on every child that returned true,
   * then return false.
   * @return true on success
   */
  virtual bool failsafeRedo();
  /** SHOULD set error string if return false.
   * Default implementation: calls every child until one of them returns false.
   * If this happens, calls failsafeRedo() on every child that returned true,
   * then return false.
   * @return true on success
   */
  virtual bool failsafeUndo();

private:
  void undo();
  void redo();
};

#endif // FAILSAFEUNDOCOMMAND_H
