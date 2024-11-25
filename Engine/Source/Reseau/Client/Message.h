#pragma once
#include <vector>
#include <WinSock2.h>

namespace Reseau
{
    namespace Client
    {
        #define DECLARE_MESSAGE(name) friend class Message; static const Reseau::Client::Message::Type StaticType = Reseau::Client::Message::Type::name
        /**
         * @brief Base class for network messages.
         */
        class Message
        {
        public:
            /**
             * @brief Checks if the message is of a specific type.
             * @tparam M Type of message to check.
             * @return True if the message is of type M, false otherwise.
             */
            template<class M>
            bool is() const { return mType == M::StaticType; }

            /**
             * @brief Converts the message to a specific type.
             * @tparam M Type to convert to.
             * @return Pointer to the converted message if successful, nullptr otherwise.
             */
            template<class M>
            M* as() { return static_cast<M*>(this); }

            /**
             * @brief Converts the message to a specific type (const version).
             * @tparam M Type to convert to.
             * @return Const pointer to the converted message if successful, nullptr otherwise.
             */
            template<class M>
            const M* as() const { return static_cast<const M*>(this); }

            /**
             * @brief ID of the sender.
             */
            uint64_t idFrom;

            /**
             * @brief Information about the sender.
             */
            sockaddr_in from;

        protected:
            /**
             * @brief Enumeration representing the type of message.
             */
            enum class Type {
                Connection,
                ConnectionFailed,
                Disconnection,
                UserData,
            };

            /**
             * @brief Constructs a Message object with the specified type.
             * @param type Type of the message.
             */
            Message(Type type) : mType(type) {}

        private:
            /**
             * @brief Type of the message.
             */
            Type mType;
        };

        /**
         * @brief Message indicating a successful connection.
         */
        class Connection : public Message
        {
            DECLARE_MESSAGE(Connection);
        public:
            /**
             * @brief Enumeration representing the result of the connection attempt.
             */
            enum class Result {
                Success,
                Failed,
            };

            /**
             * @brief Constructs a Connection message with the specified result.
             * @param r Result of the connection attempt.
             */
            Connection(Result r)
                : Message(Type::Connection)
                , result(r)
            {}

            /**
             * @brief Result of the connection attempt.
             */
            Result result;
        };

        /**
         * @brief Message indicating a disconnection event.
         */
        class Disconnection : public Message
        {
            DECLARE_MESSAGE(Disconnection);
        public:
            /**
             * @brief Enumeration representing the reason for the disconnection.
             */
            enum class Reason {
                Disconnected,
                Lost,
            };

            /**
             * @brief Constructs a Disconnection message with the specified reason.
             * @param r Reason for the disconnection.
             */
            Disconnection(Reason r)
                : Message(Type::Disconnection)
                , reason(r)
            {}

            /**
             * @brief Reason for the disconnection.
             */
            Reason reason;
        };

        /**
         * @brief Message containing user data.
         */
        class UserData : public Message
        {
            #define DECLARE_MESSAGE(UserData);

        public:
            /**
             * @brief Constructs a UserData message with the specified data.
             * @param d Data to be included in the message.
             */
            UserData(std::vector<unsigned char>&& d) : Message(Type::UserData), data(std::move(d)) {}

            /**
             * @brief Data included in the message.
             */
            std::vector<unsigned char> data;
        };
#undef DECLARE_MESSAGE
    }
}