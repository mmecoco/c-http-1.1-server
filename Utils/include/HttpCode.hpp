/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** HttpCode.hpp
*/

#pragma once

#include <openZia/BaseHTTP.hpp>

namespace Utils {
    constexpr const char *codeToString(oZ::HTTP::Code code)
    {
        switch (code) {
            case oZ::HTTP::Code::Undefined: return "Undefined";
            case oZ::HTTP::Code::Continue: return "Continue";
            case oZ::HTTP::Code::SwitchingProtocols: return "SwitchingProtocols";
            case oZ::HTTP::Code::OK: return "OK";
            case oZ::HTTP::Code::Created: return "Created";
            case oZ::HTTP::Code::Accepted: return "Accepted";
            case oZ::HTTP::Code::NonAuthoritativeInformation: return "NonAuthoritativeInformation";
            case oZ::HTTP::Code::NoContent: return "NoContent";
            case oZ::HTTP::Code::ResetContent: return "ResetContent";
            case oZ::HTTP::Code::PartialContent: return "PartialContent";
            case oZ::HTTP::Code::MultipleChoices: return "MultipleChoices";
            case oZ::HTTP::Code::MovedPermanently: return "MovedPermanently";
            case oZ::HTTP::Code::Found: return "Found";
            case oZ::HTTP::Code::SeeOther: return "SeeOther";
            case oZ::HTTP::Code::NotModified: return "NotModified";
            case oZ::HTTP::Code::UseProxy: return "UseProxy";
            case oZ::HTTP::Code::TemporaryRedirect: return "TemporaryRedirect";
            case oZ::HTTP::Code::BadRequest: return "BadRequest";
            case oZ::HTTP::Code::Unauthorized: return "Unauthorized";
            case oZ::HTTP::Code::PaymentRequired: return "PaymentRequired";
            case oZ::HTTP::Code::Forbidden: return "Forbidden";
            case oZ::HTTP::Code::NotFound: return "NotFound";
            case oZ::HTTP::Code::MethodNotAllowed: return "MethodNotAllowed";
            case oZ::HTTP::Code::NotAcceptable: return "NotAcceptable";
            case oZ::HTTP::Code::ProxyAuthenticationRequired: return "ProxyAuthenticationRequired";
            case oZ::HTTP::Code::RequestTimeOut: return "RequestTimeOut";
            case oZ::HTTP::Code::Conflict: return "Conflict";
            case oZ::HTTP::Code::Gone: return "Gone";
            case oZ::HTTP::Code::LengthRequired: return "LengthRequired";
            case oZ::HTTP::Code::PreconditionFailed: return "PreconditionFailed";
            case oZ::HTTP::Code::RequestEntityTooLarge: return "RequestEntityTooLarge";
            case oZ::HTTP::Code::RequestURITooLarge: return "RequestURITooLarge";
            case oZ::HTTP::Code::UnsupportedMediaType: return "UnsupportedMediaType";
            case oZ::HTTP::Code::RequestedRangeNotSatisfiable: return "RequestedRangeNotSatisfiable";
            case oZ::HTTP::Code::ExpectationFailed: return "ExpectationFailed";
            case oZ::HTTP::Code::InternalServerError: return "InternalServerError";
            case oZ::HTTP::Code::NotImplemented: return "NotImplemented";
            case oZ::HTTP::Code::BadGateway: return "BadGateway";
            case oZ::HTTP::Code::ServiceUnavailable: return "ServiceUnavailable";
            case oZ::HTTP::Code::GatewayTimeOut: return "GatewayTimeOut";
            case oZ::HTTP::Code::HTTPVersionNotSupported: return "HTTPVersionNotSupported";
            default: return nullptr;
        }
    }

    constexpr inline const auto *codeToString(int code)
    {
        return codeToString(static_cast<oZ::HTTP::Code>(code));
    }
}
