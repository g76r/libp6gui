/* Copyright 2024 Gregoire Barbier and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "util/percentevaluator.h"
#include "util/utf8stringlist.h"
#include <QIcon>

static int staticInit() {
  PercentEvaluator::register_function(
        "=icon", [](const Utf8String &key, const PercentEvaluator::EvalContext &context, int ml) -> QVariant {
    auto params = key.split_headed_list(ml);
    auto count = params.count();
    if (count == 0)
      return {};
    QIcon icon(Utf8String{params.value(0) % context});
    if (count == 1)
      return icon;
    icon.addFile(Utf8String{params.value(1) % context}, {}, QIcon::Disabled);
    if (count == 2)
      return icon;
    icon.addFile(Utf8String{params.value(2) % context}, {}, QIcon::Active);
    if (count == 3)
      return icon;
    icon.addFile(Utf8String{params.value(3) % context}, {}, QIcon::Selected);
    return icon;
  });
  return 0;
}
Q_CONSTRUCTOR_FUNCTION(staticInit)
