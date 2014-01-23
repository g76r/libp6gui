/* Copyright 2014 Hallowyn and others. */
#ifndef LIBH6NCSU_GLOBAL_H
#define LIBH6NCSU_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBH6NCSU_LIBRARY)
#  define LIBH6NCSUSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBH6NCSUSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBH6NCSU_GLOBAL_H
