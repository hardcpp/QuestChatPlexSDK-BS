#pragma once

#include "../Utils/MonoPtr.hpp"
#include "../Utils/Json.hpp"

#include <UnityEngine/Networking/UnityWebRequest.hpp>
#include <System/Net/HttpStatusCode.hpp>
#include <System/String.hpp>

#include <memory>
#include <optional>

namespace CP_SDK::Network {

    namespace _u
    {
        using namespace System::Net;
        using namespace UnityEngine::Networking;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Web Response class
    class WebResponse
    {
        CP_SDK_NO_COPYMOVE_CTORS(WebResponse);
        CP_SDK_PRIV_TAG();

        private:
            /// @brief Result code
            _u::HttpStatusCode                  m_StatusCode;
            /// @brief Reason phrase
            std::u16string                      m_ReasonPhrase;
            /// @brief Is success
            bool                                m_IsSuccessStatusCode;
            /// @brief Should retry
            bool                                m_ShouldRetry;
            /// @brief Response bytes
            _v::MonoPtr<::Array<uint8_t>>       m_BodyBytes;
            /// @brief Body string
            std::optional<std::u16string>       m_BodyString;

        public:
            /// @brief Result code
            _u::HttpStatusCode StatusCode();
            /// @brief Reason phrase
            std::u16string_view ReasonPhrase();
            /// @brief Is success
            bool IsSuccessStatusCode();
            /// @brief Should retry
            bool ShouldRetry();
            /// @brief Response bytes
            _v::MonoPtr<::Array<uint8_t>>& BodyBytes();
            /// @brief Response bytes
            std::u16string_view BodyString();

        public:
            using Ptr = std::shared_ptr<WebResponse>;

            /// Constructor
            /// @p_Request: Reply status
            WebResponse(_u::UnityWebRequest* p_Request);

        public:
            /// @brief Get JObject from serialized JSON
            /// @param p_Object Result object
            /// @return True or false
            bool TryAsJObject(std::shared_ptr<_v::Json::U16Document>& p_Object)
            {
                p_Object = nullptr;

                auto l_New = std::make_shared<_v::Json::U16Document>();
                try
                {
                    if (!_v::Json::TryFromU16String(BodyString(), *l_New.get()))
                        return false;

                    p_Object = l_New;
                }
                catch (const std::exception& l_Exception)
                {
                    p_Object = nullptr;
                    return false;
                }

                return p_Object != nullptr;
            }

            /// @brief Get Object from serialized JSON
            /// @tparam t_Type Object type
            /// @param p_Object Result object
            /// @return True or false
            template<typename t_Type>
            bool TryGetObject(std::shared_ptr<t_Type>& p_Object)
            {
                p_Object = nullptr;
                try
                {
                    _v::Json::U16Document l_Document;
                    _v::Json::TryFromU16String(BodyString(), l_Document);

                    p_Object = std::make_shared<t_Type>();
                    p_Object->Unserialize(l_Document);
                }
                catch (const std::exception& l_Exception)
                {
                    p_Object = nullptr;
                    return false;
                }

                return p_Object != nullptr;
            }

    };

}   ///< namespace CP_SDK::Network