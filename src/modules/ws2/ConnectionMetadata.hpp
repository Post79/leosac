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

#pragma once

#include "LeosacFwd.hpp"
#include "MyWSServer.hpp"
#include "modules/ws2/Parser.hpp"
#include <queue>
#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

namespace Leosac
{
namespace Module
{
namespace WS2
{

/**
 * Metadata for a websocket connection.
 *
 * Each websocket connection has an associated
 * ConnectionMetadata object.
 */
struct ConnectionMetadata
{
    ConnectionMetadata();
    std::string uuid_;

    std::queue<ClientMessage> messages_;

    /**
     * Check if the connection associated with this metadata
     * object has pending messages (store in this metadata object)
     * that needs to be processed.
     *
     * This apply for message whose processing policy is QUEUE.
     */
    bool has_pending_messages() const;

    /**
     * Add `msg` to the queue of messages to be processed.
     */
    void enqueue(const ClientMessage &msg);

    ClientMessage dequeue();

    /**
     * Are we busy handling any queued message at this point ?
     * @return
     */
    bool busy_with_queued_msg() const;

    /**
     * A boolean to keep track of wether or not
     * we are currently handling a message with
     * QUEUE processing policy.
     *
     * This is not related to parallelism, if a sleeping
     * coroutine is working on a message, this flag
     * shall be true.
     */
    bool busy_handling_queued_message_{false};

    SecurityContextCPtr security_;
    std::shared_ptr<boost::asio::strand> connection_strand_;

    MessageProcessingPolicy processing_policy{MessageProcessingPolicy::QUEUED};

    size_t request_count_{0};
};
}
}
}