# customfetch-plugin-github

![image](https://github.com/user-attachments/assets/5ac3fbf4-4ad9-41a3-830e-734f15ead547)
POC inspired by https://github.com/isa-programmer/githubfetch

## Installation
automated using `cufetchpm`
```sh
$ cufetchpm install https://github.com/Toni500github/customfetch-plugin-github/
```

## Configurations examples
Put the following config examples into your prefered customfetch `config.toml`\
For less crashes, export your `GITHUB_TOKEN`.
```toml
[plugin.github-user-fetch]
# Username used to query user its infos
username = "foo"

[plugin.github-repo-fetch]
# Repository used to query its infos
# Can be a number or "foo/repo"
repo = "foo/repo"
```
