#include "CP_SDK/Network/WebResponse.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <System/Text/Encoding.hpp>
#include <System/Text/UTF8Encoding.hpp>
#include <UnityEngine/Networking/DownloadHandler.hpp>
#include <UnityEngine/Networking/UnityWebRequest.hpp>

bool StartWith(::Array<uint8_t>* p_Array, ArrayW<uint8_t>& p_Pattern, int p_PatternSize);

using namespace System::Text;
using namespace System::Net;
using namespace UnityEngine::Networking;

namespace CP_SDK::Network {

    /// @brief Result code
    System::Net::HttpStatusCode WebResponse::StatusCode()
    {
        return m_StatusCode;
    }
    /// @brief Reason phrase
    std::u16string_view WebResponse::ReasonPhrase()
    {
        return m_ReasonPhrase;
    }
    /// @brief Is success
    bool WebResponse::IsSuccessStatusCode()
    {
        return m_IsSuccessStatusCode;
    }
    /// @brief Should retry
    bool WebResponse::ShouldRetry()
    {
        return m_ShouldRetry;
    }
    /// @brief Response bytes
    Utils::MonoPtr<::Array<uint8_t>>& WebResponse::BodyBytes()
    {
        return m_BodyBytes;
    }
    /// @brief Response bytes
    std::u16string_view WebResponse::BodyString()
    {
        if (m_BodyString.has_value())
            return m_BodyString.value();

        if (m_BodyBytes && m_BodyBytes->get_Length() > 0)
        {
            auto l_UTF8Encoding = Encoding::get_UTF8();
            auto l_Preamble     = l_UTF8Encoding->GetPreamble();
            if (l_Preamble.size() > 0 && m_BodyBytes->get_Length() >= l_Preamble.size() && StartWith(m_BodyBytes.Ptr(), l_Preamble, l_Preamble.size()))
                m_BodyString = l_UTF8Encoding->GetString(m_BodyBytes.Ptr(), l_Preamble.size(), m_BodyBytes->get_Length() - l_Preamble.size());
            else
                m_BodyString = l_UTF8Encoding->GetString(m_BodyBytes.Ptr());
        }
        else
            m_BodyString = u"";

        return m_BodyString.value();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Request: Reply status
    WebResponse::WebResponse(UnityWebRequest * p_Request)
    {
        m_StatusCode          = (HttpStatusCode)p_Request->get_responseCode();
        m_IsSuccessStatusCode = !(p_Request->get_result() == UnityWebRequest::Result::ProtocolError && p_Request->get_result() == UnityWebRequest::Result::ConnectionError);
        m_ShouldRetry         = IsSuccessStatusCode() ? false : (p_Request->get_responseCode() < 400 || p_Request->get_responseCode() >= 500);
        m_BodyBytes           = reinterpret_cast<::Array<uint8_t>*>(p_Request->get_downloadHandler()->GetData().convert());

        if (p_Request->get_result() == UnityWebRequest::Result::ConnectionError || p_Request->get_result() == UnityWebRequest::Result::ProtocolError)
        {
            if (p_Request->get_result() == UnityWebRequest::Result::ProtocolError)
                m_ReasonPhrase = u"HTTP/1.1 " + std::to_string(m_StatusCode.value__) + u" " + p_Request->GetHTTPStatusString(m_StatusCode.value__);
            else
                m_ReasonPhrase = p_Request->GetWebErrorString(p_Request->GetError());
        }
    }

}   ///< namespace CP_SDK::Network

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool StartWith(::Array<uint8_t>* p_Array, ArrayW<uint8_t>& p_Pattern, int p_PatternSize)
{
    auto l_PatternPosition = 0;
    for (int l_I = 0; l_I < p_PatternSize; ++l_I)
    {
        if (p_Array->_values[l_I] == p_Pattern[l_I])
            continue;

        return false;
    }

    return true;
}