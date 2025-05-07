/* Copyright 2024-2025 Grégoire Barbier and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * Libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin. If not, see <http://www.gnu.org/licenses/>.
 */
#include "indexparamsprovider.h"
#include "util/radixtree.h"

using IndexFunction = std::function<QVariant(
const Utf8String &key,const QVariant &def,
const ParamsProvider::EvalContext &context, const QModelIndex &index)>;

/** return index.parent() but using column 0 instead of index.column(), because
  * Qt convention for tree models is that only column 0 index have parents. */
static inline QModelIndex parent0(const QModelIndex &index) {
  if (index.column())
    return index.siblingAtColumn(0).parent();
  return index.parent();
};

static const RadixTree<IndexFunction> _functions {
  { "row", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return index.row();
    } },
  { "column", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return index.column();
    } },
  { "internal_id", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return index.internalId();
    } },
  { "is_valid", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return index.isValid();
    } },
  { "children_count", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return index.model()->rowCount(index.siblingAtColumn(0));
    } },
  { "parent_row", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return parent0(index).row();
    } },
  { "parent_column", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return parent0(index).column();
    } },
  { "parent_is_valid", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return parent0(index).isValid();
    } },
  { "parent_internal_id", [](const Utf8String &,const QVariant &, const ParamsProvider::EvalContext &, const QModelIndex &index) STATIC_LAMBDA -> QVariant {
      return parent0(index).internalId();
    } },
};

QVariant IndexParamsProvider::paramRawValue(
    const Utf8String &key, const QVariant &def,
    const EvalContext &context) const {
  auto f = _functions.value(key);
  return f ? f(key, def, context, index) : def;
}

Utf8StringSet IndexParamsProvider::paramKeys(const EvalContext &) const {
  return _functions.keys();
}

Utf8String IndexParamsProvider::paramScope() const {
  return "index"_u8;
}
