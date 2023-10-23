#include "CP_SDK/Misc/FastCancellationToken.hpp"

namespace CP_SDK::Misc {

    int FastCancellationToken::Serial() const
    {
        return m_Serial.load();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    FastCancellationToken::FastCancellationToken(CP_SDK_PRIV_TAG_ARG())
    {
        m_Serial.store(0);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    FastCancellationToken::Ptr FastCancellationToken::Make()
    {
        return std::make_shared<FastCancellationToken>(CP_SDK_PRIV_TAG_VAL());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Increment serial
    void FastCancellationToken::Cancel()
    {
        m_Serial.fetch_add(1);
    }
    /// @brief Compare serial
    /// @param p_OldSerial Old serial to compare to
    bool FastCancellationToken::IsCancelled(int p_OldSerial)
    {
        return m_Serial.load() > p_OldSerial;
    }

}   ///< namespace CP_SDK::Misc