#ifndef BOUSK_DVP_COURS_ERRORS_HPP
#define BOUSK_DVP_COURS_ERRORS_HPP

/**
 * @file Errors.hpp
 * @brief Defines error handling utilities for network sockets.
 */

#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <cerrno>
#define SOCKET int
#define INVALID_SOCKET ((int)-1)
#define SOCKET_ERROR (int(-1))
#endif

namespace Reseau
{
    namespace Sockets
    {
        namespace Error
        {
            /**
             * @brief Gets the last error code.
             * @return The last error code.
             */
            int GetError();

            /**
             * @enum Errors
             * @brief Enumerates common socket errors.
             */
            enum class Errors {
#ifdef _WIN32
                WOULDBLOCK = WSAEWOULDBLOCK, /**< Operation would block. */
                INPROGRESS = WSAEINPROGRESS, /**< Operation in progress. */
                AGAIN = WSAEWOULDBLOCK /**< Operation would block. */
#else
                WOULDBLOCK = EWOULDBLOCK, /**< Operation would block. */
                INPROGRESS = EINPROGRESS, /**< Operation in progress. */
                AGAIN = EAGAIN /**< Operation would block. */
#endif
            };
        }
    }
}
#endif // BOUSK_DVP_COURS_ERRORS_HPP
