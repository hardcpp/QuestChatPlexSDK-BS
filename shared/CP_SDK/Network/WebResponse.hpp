#pragma once

#include "../Utils/MonoPtr.hpp"
#include "../Utils/Json.hpp"

#include <UnityEngine/Networking/UnityWebRequest.hpp>
#include <System/Net/HttpStatusCode.hpp>
#include <System/String.hpp>

#include <memory>
#include <optional>
#include <vector>

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
    class CP_SDK_EXPORT WebResponse
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
            /// @param request: Reply status
            WebResponse(_u::UnityWebRequest* request);
            /// @brief Constructor
            /// @param curlPerformResult CURL perform result
            /// @param curlInstance CURL instance
            /// @param data Response data
            WebResponse(long curlPerformResult, void* curlInstance, std::vector<uint8_t>* data);

        public:
            /// @brief Get JObject from serialized JSON
            /// @param object Result object
            /// @return True or false
            bool TryAsJObject(std::shared_ptr<_v::Json::U16Document>& object)
            {
                object = nullptr;

                auto l_New = std::make_shared<_v::Json::U16Document>();
                try
                {
                    if (!_v::Json::TryFromU16String(BodyString(), *l_New.get()))
                        return false;

                    object = l_New;
                }
                catch (const std::exception& l_Exception)
                {
                    object = nullptr;
                    return false;
                }

                return object != nullptr;
            }

            /// @brief Get Object from serialized JSON
            /// @tparam t_Type Object type
            /// @param object Result object
            /// @return True or false
            template<typename t_Type>
            bool TryGetObject(std::shared_ptr<t_Type>& object)
            {
                object = nullptr;
                try
                {
                    _v::Json::U16Document l_Document;
                    _v::Json::TryFromU16String(BodyString(), l_Document);

                    object = std::make_shared<t_Type>();
                    object->Unserialize(l_Document);
                }
                catch (const std::exception& l_Exception)
                {
                    object = nullptr;
                    return false;
                }

                return object != nullptr;
            }

    };

}   ///< namespace CP_SDK::Network