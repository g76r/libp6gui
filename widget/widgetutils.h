#ifndef WIDGETUTILS_H
#define WIDGETUTILS_H

#include "libh6ncsu_global.h"

class QWidget;

class LIBH6NCSUSHARED_EXPORT WidgetUtils {
  Q_DISABLE_COPY(WidgetUtils)
  WidgetUtils() = delete;

public:
  /** Convenience for activateWindow() + ensure visible + ensure raised. */
  static void activateAndShowWindow(QWidget *widget);
  /** activateAndShowWindow() + ensure the widget and its parent are visible in
   * every containers they belong to (set current indexes on QStackedWidgets and
   * QTabWidgets, etc.). */
  static void ensureWidgetIsVisible(QWidget *widget);
};

#endif // WIDGETUTILS_H
