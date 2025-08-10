#include <libcufetch/common.hh>
#include <libcufetch/config.hh>
#include <libcufetch/cufetch.hh>

#include <cpr/cpr.h>
#include <cstdlib>
#include <rapidjson/document.h>
#include <string>

#define MODFUNC(name) std::string name(__attribute__((unused)) const callbackInfo_t* callbackInfo = nullptr)
static rapidjson::Document json;
static std::string repo;

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
        const cpr::Response& resp = cpr::Get(cpr::Url("https://api.github.com/repositories/"+repo),
                                         get_github_token());
        if (resp.status_code != 200)
            die("github-repo-fetch: Failed to retrieve repository infos for '{}'", repo);
        json.Parse(resp.text.c_str());
    }
}

MODFUNC(github_repo_stargazers)
{
    assert_json();
    return std::to_string(json["stargazers_count"].GetUint());
}

MODFUNC(github_repo_description)
{
    assert_json();
    return json["description"].GetString();
}

MODFUNC(github_repo_name)
{
    assert_json();
    return json["name"].GetString();
}

MODFUNC(github_repo_fullname)
{
    assert_json();
    return json["full_name"].GetString();
}

APICALL EXPORT PLUGIN_INIT(void *handle, const ConfigBase& config)
{
    repo = config.getValue<std::string>("plugin.github-repo-fetch.repo", MAGIC_LINE);
    if (repo == MAGIC_LINE)
    {
        warn("Repository not set.");
        return;
    }

    module_t github_repo_name_full_module {"full", "repo full name", {}, github_repo_fullname};
    module_t github_repo_name_module {"name", "repo short name", {std::move(github_repo_name_full_module)}, github_repo_name};
    module_t github_repo_followers_module {"stars", "repo stargazers", {}, github_repo_stargazers};
    module_t github_repo_bio_module {"description", "repo description", {}, github_repo_description};
    module_t github_repo_module {"github-repo", "Github repo modules", {
        std::move(github_repo_name_module),
        std::move(github_repo_followers_module),
        std::move(github_repo_bio_module)
    }, NULL};

    cfRegisterModule(github_repo_module);
}

APICALL EXPORT PLUGIN_FINISH(void *handle)
{

}
