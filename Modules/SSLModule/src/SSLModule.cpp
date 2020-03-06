/*
** EPITECH PROJECT, 2018
** CPP_zia_2019
** File description:
** Hello
*/

#include "SSLModule.hpp"

SSLModule::SSLModule()
{
    std::cout << "ctor => SSLModule" << std::endl;

    ctx = nullptr;
    ssl = nullptr;
    initOpenssl();
    createContext();
    configureContext();
}

void SSLModule::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::BeforeParse,
            oZ::Priority::ASAP,
            this, &SSLModule::onRegister);
}

void SSLModule::configureContext()
{
    SSL_CTX_set_ecdh_auto(ctx, 1);
    if (SSL_CTX_use_certificate_file(ctx, "Certificates/cert.pem",
                                     SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("error loading certificate.");
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "Certificates/key.pem",
                                    SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("error loading key.");
    }
}

void SSLModule::createContext()
{
    const SSL_METHOD *method;

    method = SSLv23_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("error creating context for SSL.");
    }
}

void SSLModule::cleanupSSL()
{
    EVP_cleanup();
}

void SSLModule::initOpenssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void SSLModule::onConnection(oZ::Context &context)
{
    int error;

    if (!context.getPacket().hasEncryption() || ctx == nullptr)
        return;
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, static_cast<int>(context.getPacket().getFileDescriptor()));
    error = SSL_accept(ssl);
    while (error <= 0) {
        if (SSL_get_error(ssl, error) == SSL_ERROR_WANT_READ) {
            error = SSL_accept(ssl);
        } else {
            ERR_print_errors_fp(stderr);
            SSL_shutdown(ssl);
            SSL_free(ssl);
            return;
        }
    }
}

bool SSLModule::onRegister(oZ::Context &context)
{
    std::vector<char> buff;
    int read_chars;

    if (!context.getPacket().hasEncryption() || ctx == nullptr)
        return true;
    buff.reserve(2048);
    auto &request = context.getPacket().getByteArray();
    do {
        read_chars = SSL_read(ssl, buff.data(), 2048);
        if (read_chars < 0) {
            ERR_print_errors_fp(stderr);
            return false;
        }
        for (int i = 0; i < read_chars; i++) {
            std::cout << buff[i];
        }
        request.reserve(request.size() + read_chars);
        request.insert(request.end(), buff.begin(), buff.begin() + read_chars);
    } while (read_chars == 2048);
    return true;
}
