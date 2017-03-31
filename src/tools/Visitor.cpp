/*
    Copyright (C) 2014-2016 Leosac

    This file is part of Leosac.

    Leosac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leosac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "tools/Visitor.hpp"
#include "tools/IVisitable.hpp"
#include "tools/log.hpp"
#include <boost/type_index.hpp>

namespace Leosac
{

namespace Tools
{
void BaseVisitor::cannot_visit(const IVisitable &obj)
{
    auto type_index_visitable = boost::typeindex::type_id_runtime(obj);
    auto type_index_visitor   = boost::typeindex::type_id_runtime(*this);
    ERROR("Cannot visit object of type " << type_index_visitable
                                         << " from visitor of type "
                                         << type_index_visitor.pretty_name());
    assert(0 && "Cannot visit");
}
}
}
