/*
** EPITECH PROJECT, 2018
** CPP_zia_2019
** File description:
** Hello
*/

#pragma once

#include <openssl/ssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>
#include <iostream>

class SSLModule : public oZ::IModule {
public:
    SSLModule();
    ~SSLModule() override = default;

    SSLModule(const SSLModule &) = delete;
    SSLModule &operator=(const SSLModule &) = delete;

    [[nodiscard]] const char *getName() const override
    { return "SSLModule"; };
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
    void onConnection(oZ::Context &context) override;
    bool onRegister(oZ::Context &);
private:
    SSL_CTX *ctx{nullptr};
    ::SSL *ssl{nullptr};

    void configureContext();
    void createContext();
    void cleanupSSL();
    void initOpenssl();

};

OPEN_ZIA_MAKE_ENTRY_POINT(SSLModule);
