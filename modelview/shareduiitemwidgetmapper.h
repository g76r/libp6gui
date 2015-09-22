/* Copyright 2014-2015 Hallowyn and others.
 * This file is part of libh6ncsu, see <https://gitlab.com/g76r/libh6ncsu>.
 * Libh6ncsu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libh6ncsu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libh6ncsu.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef UIITEMWIDGETMAPPER_H
#define UIITEMWIDGETMAPPER_H

#include <QObject>
#include "modelview/shareduiitem.h"
#include "libh6ncsu_global.h"

class QWidget;
class SharedUiItemDocumentManager;

/** Similar to QDataWidgetMapper for event-driven SharedUiItem display.
 *
 * Supports any QWidget with user property (i.e. most Qt edition and display
 * widgets, @see QMetaObject::userProperty()) as read-only (displaying an
 * item data in the widget).
 *
 * Supports following widgets (and their subclasees) as read-write: QLineEdit,
 * EnhancedTextEdit, EnhancedPlainTextEdit, QComboBox, QAbstractSpinBox,
 * QAbstractButton (incl. checkboxes).
 *
 * @see QDataWidgetMapper
 * @see SharedUiItem
 * @see EnhancedTextEdit
 * @see EnhancedPlainTextEdit
 */
class LIBH6NCSUSHARED_EXPORT SharedUiItemWidgetMapper : public QObject {
  Q_OBJECT
  SharedUiItem _item;
  QHash<int,QWidget*> _sectionToWidget;
  QHash<int,QVariant> _sectionToDefaultValue;
  QHash<QWidget*,int> _widgetToSection;
  SharedUiItemDocumentManager *_documentManager;

public:
  explicit SharedUiItemWidgetMapper(QObject *parent = 0);
  void addMapping(QWidget *widget, int section,
                  QVariant valueWhenNull = QVariant());
  void addReadOnlyMapping(QWidget *widget, int section,
                          QVariant valueWhenNull = QVariant());
  void clearMapping();
  void removeMapping(QWidget *widget);
  void removeMapping(int section);
  void setItem(SharedUiItem item);
  void clearItem() { setItem(SharedUiItem()); }
  SharedUiItem item() const { return _item; }
  SharedUiItemDocumentManager *documentManager() const {
    return _documentManager; }
  void setDocumentManager(SharedUiItemDocumentManager *documentManager);

private slots:
  void widgetEdited();
  void itemChanged(SharedUiItem newItem, SharedUiItem oldItem);

private:
  void populate();
  void populate(int section);
  void connectEditionSignals(int section);
};

#endif // UIITEMWIDGETMAPPER_H
