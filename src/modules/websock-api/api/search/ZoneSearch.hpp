/*
    Copyright (C) 2014-2017 Leosac

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

#pragma once

#include "api/MethodHandler.hpp"
#include <json.hpp>

namespace Leosac
{
namespace Module
{
namespace WebSockAPI
{
using json = nlohmann::json;

/**
 * Search zones by name.
 *
 * Request:
 *     + 'partial_name': A part of the name we are looking for.
 *
 * Response:
 *     A list of {id,name} for zones that match the partial name.
 *     [
 *       {id: $ZONE_ID,
 *       alias: $ZONE_ALIAS},
 *       {...}
 *     ]
 */
class ZoneSearch : public MethodHandler
{
  public:
    explicit ZoneSearch(RequestContext ctx);

    static MethodHandlerUPtr create(RequestContext);

  protected:
    std::vector<ActionActionParam>
    required_permission(const json &req) const override;

  private:
    virtual json process_impl(const json &req) override;
};
}
}
}
