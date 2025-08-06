#include "CP_SDK/Network/JsonRPCClient.hpp"
#include "CP_SDK/Utils/Json.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/rapidjson.h"

#include <System/Text/Encoding.hpp>
#include <System/Text/UTF8Encoding.hpp>

using namespace System::Text;

namespace CP_SDK::Network {

    /// @brief Constructor
    /// @param webClient Web client instance
    JsonRPCClient::JsonRPCClient(CP_SDK_PRIV_TAG_ARG(), WebClientCore::Ptr& webClient)
    {
        m_WebClient = webClient;
    }
    /// @brief Destructor
    JsonRPCClient::~JsonRPCClient()
    {
        m_WebClient = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create
    /// @param webClient Web client instance
    JsonRPCClient::Ptr JsonRPCClient::Create(WebClientCore::Ptr& webClient)
    {
        return std::make_shared<JsonRPCClient>(CP_SDK_PRIV_TAG_VAL(), webClient);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do a RPC request
    /// @param method Target method
    /// @param parameters Request parameters
    /// @param dontRetry Should not retry?
    JsonRPCResult::Ptr JsonRPCClient::Request(std::u16string_view method, std::shared_ptr<_v::Json::U16Value> parameters, bool dontRetry)
    {
        auto l_Method = std::u16string(method);

        std::shared_ptr<_v::Json::U16Document> l_Content(new _v::Json::U16Document());
        l_Content->SetObject();
        l_Content->AddMember(u"id", _v::Json::U16Value(1), l_Content->GetAllocator());
        l_Content->AddMember(u"jsonrpc", _v::Json::U16Value(u"2.0", l_Content->GetAllocator()), l_Content->GetAllocator());
        l_Content->AddMember(u"method", _v::Json::U16Value(l_Method.c_str(), l_Method.length(), l_Content->GetAllocator()), l_Content->GetAllocator());

        if (parameters)
        {
            _v::Json::U16Value l_Copy;
            l_Copy.CopyFrom(*parameters, l_Content->GetAllocator());

            l_Content->AddMember(u"params", l_Copy, l_Content->GetAllocator());
        }
        else
            l_Content->AddMember(u"params", _v::Json::U16Value(rapidjson::kNullType), l_Content->GetAllocator());

        return DoRequest(l_Method, l_Content, dontRetry);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do a RPC request
    /// @param method Target method
    /// @param parameters Request parameters
    /// @param token Cancellation token
    /// @param callback Callback
    /// @param dontRetry Should not retry?
    void JsonRPCClient::RequestAsync(
            std::u16string_view                 method,
            std::shared_ptr<_v::Json::U16Value> parameters,
            _u::CancellationToken               token,
            _v::CActionRef<JsonRPCResult::Ptr>  callback,
            bool                                dontRetry
        )
    {
        auto l_Method = std::u16string(method);

        std::shared_ptr<_v::Json::U16Document> l_Content(new _v::Json::U16Document());
        l_Content->SetObject();
        l_Content->AddMember(u"id", _v::Json::U16Value(1), l_Content->GetAllocator());
        l_Content->AddMember(u"jsonrpc", _v::Json::U16Value(u"2.0", l_Content->GetAllocator()), l_Content->GetAllocator());
        l_Content->AddMember(u"method", _v::Json::U16Value(l_Method.c_str(), l_Method.length(), l_Content->GetAllocator()), l_Content->GetAllocator());

        if (parameters)
        {
            _v::Json::U16Value l_Copy;
            l_Copy.CopyFrom(*parameters, l_Content->GetAllocator());

            l_Content->AddMember(u"params", l_Copy, l_Content->GetAllocator());
        }
        else
            l_Content->AddMember(u"params", _v::Json::U16Value(rapidjson::kNullType), l_Content->GetAllocator());

        DoRequestAsync(l_Method, l_Content, token, callback, dontRetry);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do a RPC request
    /// @param method Target method
    /// @param parameters Request content
    /// @param dontRetry Should not retry?
    JsonRPCResult::Ptr JsonRPCClient::DoRequest(
            std::u16string                          method,
            std::shared_ptr<_v::Json::U16Document>& content,
            bool                                    dontRetry
        )
    {
        auto l_Response = m_WebClient->Post(
            u"",
            WebContent::FromJson(content),
            dontRetry
        );
        return HandleWebResponse(method, l_Response);
    }
    /// @brief Do a RPC request
    /// @param method Target method
    /// @param content Request content
    /// @param token Cancellation token
    /// @param callback Callback
    /// @param dontRetry Should not retry?
    void JsonRPCClient::DoRequestAsync(
            std::u16string                          method,
            std::shared_ptr<_v::Json::U16Document>& content,
            _u::CancellationToken                   token,
            _v::CActionRef<JsonRPCResult::Ptr>      callback,
            bool                                    dontRetry
        )
    {
        auto l_Self = shared_from_this();
        m_WebClient->PostAsync(
            u"",
            WebContent::FromJson(content),
            token,
            [=](CP_SDK::Network::WebResponse::Ptr webResponse) -> void {
                callback(l_Self->HandleWebResponse(method, webResponse));
            },
            dontRetry
        );
    }
    /// @brief Do a RPC request
    /// @param method Target method
    /// @param rawResponse Web response
    JsonRPCResult::Ptr JsonRPCClient::HandleWebResponse(std::u16string method, WebResponse::Ptr& rawResponse)
    {
        if (!rawResponse)
            return JsonRPCResult::Create(rawResponse, nullptr, nullptr);

        try
        {
            auto l_JsonResult = std::make_shared<_v::Json::U16Document>();
            if (!_v::Json::TryFromU16String(rawResponse->BodyString(), *l_JsonResult.get()))
                return nullptr;

            auto l_ResultDocument = std::shared_ptr<_v::Json::U16Document>(nullptr);
            if (l_JsonResult->HasMember(u"result"))
            {
                l_ResultDocument = std::make_shared<_v::Json::U16Document>();
                l_ResultDocument->CopyFrom(l_JsonResult->FindMember(u"result")->value, l_ResultDocument->GetAllocator());
            }

            auto l_ErrorDocument = std::shared_ptr<_v::Json::U16Document>(nullptr);
            if (l_JsonResult->HasMember(u"error"))
            {
                l_ErrorDocument = std::make_shared<_v::Json::U16Document>();
                l_ErrorDocument->CopyFrom(l_JsonResult->FindMember(u"error")->value, l_ErrorDocument->GetAllocator());
            }

            return JsonRPCResult::Create(
                rawResponse,
                l_ResultDocument,
                l_ErrorDocument
            );
        }
        catch (std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_API_SDK.Network][JsonRPCClient.HandleResponse] Request " + method + u" failed parsing response:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        return nullptr;
    }

}   ///< namespace CP_SDK::Network