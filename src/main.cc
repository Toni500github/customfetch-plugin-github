#include <cufetch/common.hpp>
#include <cufetch/cufetch.hh>

#include <cpr/cpr.h>
#include <rapidjson/document.h>
#include <string>

#define MODFUNC(name) const std::string name(__attribute__((unused)) const callbackInfo_t* callbackInfo = nullptr)
static rapidjson::Document json;

MODFUNC(github_followers)
{ return std::to_string(json["followers"].GetUint()); }

MODFUNC(github_bio)
{ return json["bio"].GetString(); }

MODFUNC(github_name)
{ return json["login"].GetString(); }

APICALL EXPORT MOD_INIT(void *handle)
{
    const cpr::Response& resp = cpr::Get(cpr::Url("https://api.github.com/users/Toni500github"));
    if (resp.status_code != 200)
        die("is github down?");
    json.Parse(resp.text.c_str());

    module_t test = {"test", {}, [](const callbackInfo_t* callback) {return parse("Testing with os ${auto}$<os.name>", callback->modulesInfo, callback->config);}};
    module_t github_name_module {"name", {}, github_name};
    module_t github_followers_module {"followers", {}, github_followers};
    module_t github_bio_module {"bio", {}, github_bio};
    module_t github_module {"github", {
        github_name_module,
        test,
        std::move(github_followers_module),
        std::move(github_bio_module)
    }, NULL};

    cfRegisterModule(github_module);
}
