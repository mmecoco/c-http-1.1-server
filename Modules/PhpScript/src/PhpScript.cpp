
#include "PhpScript.hpp"
#include "ExeCommand.hpp"

PhpScript::PhpScript()
        : _fileToParse("")
{
    std::cout << "ctor => PhpScript" << std::endl;
}

void PhpScript::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::Interpret,
            oZ::Priority::High,
            this, &PhpScript::onParseScript
    );
}

bool PhpScript::onParseScript(oZ::Context &context)
{
    bool isPhp = this->setFileFromURI(context.getRequest().getURI());

    if (!isPhp || !(context.getResponse().getBody().empty())) {
        return true;
    }
    if (context.getRequest().getMethod() == oZ::HTTP::Method::Get) {
        return this->interpretGet(context);
    }
    if (context.getRequest().getMethod() == oZ::HTTP::Method::Post) {
        return this->interpretPost(context);
    }
    return true;
}

bool PhpScript::interpretGet(oZ::Context &context)
{
    ExeCommand phpExe;
    std::string scriptHeader;
    std::string page;

    phpExe.reConf("php-cgi " + _fileToParse + this->queryParamsToStr(context), ExeCommand::READ);
    page = phpExe.exec();
    scriptHeader = page.substr(0, page.find("<!DOCTYPE html>") - 2);
    page.erase(0, page.find("<!DOCTYPE html>"));
    auto &body = context.getResponse().getBody();
    body = page;
    auto &header = context.getResponse().getHeader();
    this->fillHeaderFromString(header, scriptHeader, page.length());
    context.getResponse().setCode(oZ::HTTP::Code::OK);
    return true;
}

bool PhpScript::interpretPost(oZ::Context &context)
{
    ExeCommand phpExe;
    std::string scriptHeader;
    std::string page;

    phpExe.reConf("php-cgi " + _fileToParse + this->getBodyQuery(context), ExeCommand::READ);
    page = phpExe.exec();
    scriptHeader = page.substr(0, page.find("<!DOCTYPE html>") - 2);
    page.erase(0, page.find("<!DOCTYPE html>"));
    auto &body = context.getResponse().getBody();
    body = page;
    auto &header = context.getResponse().getHeader();
    this->fillHeaderFromString(header, scriptHeader, page.length());
    context.getResponse().setCode(oZ::HTTP::Code::OK);
    return true;
}

bool PhpScript::setFileFromURI(const std::string &URI)
{
    _fileToParse = URI;
    size_t extPos = _fileToParse.find(".php");

    return extPos != std::string::npos;
}

std::string PhpScript::queryParamsToStr(oZ::Context &context)
{
    oZ::HTTP::QueryParameters params = context.getRequest().getQueryParameters();
    std::string strParams = "";         ////FIXED BY HAND (ELIE)

    for (auto const &param : params) {
        strParams += (param.first + "=" + param.second + " ");
    }

    return strParams;
}

std::string PhpScript::getBodyQuery(oZ::Context &context)
{
    oZ::HTTP::Body params = context.getRequest().getBody();
    std::string strParams = params;

    std::replace(strParams.begin(), strParams.end(), '&',' ');

    return strParams;
}

void PhpScript::fillHeaderFromString(oZ::HTTP::Header &header, const std::string &scriptHeader, size_t length)
{
    std::istringstream iss(scriptHeader);
    std::string line{};
    std::string key;
    std::string value;

    std::cout << "PARSED THIS HEADER : \n'" << scriptHeader << "'" <<std::endl;
    while (std::getline(iss, line)) {
      if (line.find(':') != std::string::npos) {
        key = line.substr(0, line.find(':'));
        value = line.substr(line.find(':') + 1, std::string::npos);
        header.set(key, value);
        std::cout << "ADDING THIS KEY TO HEADER : " << key <<std::endl;
        std::cout << "ADDING THIS VALUE TO HEADER : " << value <<std::endl;
        line.clear();
      }
    }
    header.set("Content-Length", std::to_string(length));
}
