#pragma once

#include "../Utils/Il2cpp.hpp"

#include <atomic>
#include <memory>

namespace CP_SDK::Misc {

    /// @brief Fast cancellation token
    class FastCancellationToken
    {
        DISABLE_CONSTRUCTORS(FastCancellationToken);
        PRIVATE_TAG();

        private:
            std::atomic_int32_t m_Serial;

        public:
            int Serial() const;

        public:
            using Ptr = std::shared_ptr<FastCancellationToken>;

            /// @brief Constructor
            FastCancellationToken(PRIVATE_TAG_ARG());

            /// @brief Constructor
            static Ptr Make();

        public:
            /// @brief Increment serial
            void Cancel();
            /// @brief Compare serial
            /// @param p_OldSerial Old serial to compare to
            bool IsCancelled(int p_OldSerial);

    };

}   ///< namespace CP_SDK::Misc