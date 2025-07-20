#include <libcufetch/common.hh>
#include <libcufetch/config.hh>
#include <libcufetch/cufetch.hh>

#include <cpr/cpr.h>
#include <cstdlib>
#include <rapidjson/document.h>
#include <string>

#define MODFUNC(name) std::string name(__attribute__((unused)) const callbackInfo_t* callbackInfo = nullptr)
static rapidjson::Document json;
static std::string username;

static cpr::Header get_github_token()
{
    const char *github_token = std::getenv("GITHUB_TOKEN");
    if (github_token && github_token[0] != '\0')
        return {{"Autorization", github_token}};
    return {{}};
}

static void assert_json()
{
    if (json.IsNull())
    {
        const cpr::Response& resp = cpr::Get(cpr::Url("https://api.github.com/users/"+username),
                                         get_github_token());
        if (resp.status_code != 200)
            die("is github down?");
        json.Parse(resp.text.c_str());
    }
}

MODFUNC(github_followers)
{
    assert_json();
    return std::to_string(json["followers"].GetUint());
}

MODFUNC(github_bio)
{
    assert_json();
    return json["bio"].GetString();
}

MODFUNC(github_name)
{
    assert_json();
    return json["login"].GetString();
}

APICALL EXPORT PLUGIN_INIT(void *handle, const ConfigBase& config)
{
    username = config.getValue<std::string>("plugin.githubfetch.username", MAGIC_LINE);
    if (username == MAGIC_LINE)
    {
        warn("Username not set.");
        return;
    }

    module_t github_name_module {"name", "profile username", {}, github_name};
    module_t github_followers_module {"followers", "profile followers", {}, github_followers};
    module_t github_bio_module {"bio", "profile bio", {}, github_bio};
    module_t github_module {"github", "Github modules", {
        std::move(github_name_module),
        std::move(github_followers_module),
        std::move(github_bio_module)
    }, NULL};

    cfRegisterModule(github_module);
}

APICALL EXPORT PLUGIN_FINISH(void *handle)
{

}
