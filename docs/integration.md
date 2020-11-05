# Integrating Pottery into your project

Pottery is a header-only library, so you only need to put it on your include path to use it. Just [grab the source](https://github.com/ludocode/pottery/archive/master.tar.gz) and add the `include/` folder to the include paths in your project.

You should also add `-DPOTTERY_AVAILABLE=1` to your compiler options to enable optional dependencies on Pottery in other libraries. You can add `bindings/cxx/include` to the include path as well if you care for the C++ templates.



## Creating a dependency-fetching script

To avoid committing dependencies to your repository, you should make a dependency fetching script. You don't need a fancy dependency manager like npm or yarn. In fact you shouldn't use one. Dependency fetchers encourage you to ignore your dependencies' dependencies leading to an [explosion of modules](https://www.davidhaney.io/npm-left-pad-have-we-forgotten-how-to-program/).

Here's a simpler way. On Linux or macOS, make a shell script called `fetch-libs.sh` containing this:

```sh
#!/bin/sh
set -e
fetch_lib() {
    echo "Fetching $1 from $2"
    curl -Lo $1.tar.gz "$2"
    (mkdir $1 && cd $1 && tar -xzf ../$1.tar.gz --strip-components=1)
}
rm -rf lib && mkdir lib && cd lib
. ../fetch-libs.common
```

And/or on Windows, make a PowerShell script called `fetch-libs.ps1` containing this:

```powershell
$ErrorActionPreference = "Stop"
function fetch_lib($name, $url) {
    echo "Fetching $name from $url"
    Invoke-WebRequest -Uri "$url" -OutFile "$name.tar.gz"
    md $name | Out-Null ; cd $name
    tar -xzf ../$name.tar.gz --strip-components=1
    cd ..
}
if (Test-Path lib) {rm lib -R -Force}
md lib -ea 0 | Out-Null ; cd lib
Invoke-Expression ((Get-Content ../fetch-libs.common) | Out-String)
cd ..
```

Now make a text file called `fetch-libs.common` and list your libraries:

```sh
fetch_lib pottery https://github.com/ludocode/pottery/archive/master.tar.gz
```

Run `fetch-libs.sh` or `fetch-libs.ps1`. You now have Pottery in `lib/pottery/`. That's it! This script can be run from your `configure` script or however else you set up your project.

Now just add `-Ilib/pottery/include -DPOTTERY_AVAILABLE=1` to your compiler flags and `/lib/` to your `.gitignore`. This will work for whatever simple dependencies you want, whether or not they use Pottery.

Try adding a few more libraries. If you don't want to live at head, use the URL for a release tarball or a specific commit:

```sh
fetch_lib stb https://github.com/nothings/stb/archive/master.tar.gz
fetch_lib rapidjson https://github.com/Tencent/rapidjson/archive/v1.1.0.tar.gz
fetch_lib blake2 https://github.com/BLAKE2/BLAKE2/archive/b52178a376ca85a8ffe50492263c2a5bc0fa4f46.tar.gz
```

Instead of `lib/` you could call the folder `contrib/` or `third_party/` or whatever you want. You can also extend your script to cache or backup downloaded files, to support more archive types (like zip), to add authentication for private repos, etc. It's hardly necessary though. You'd be surprised how far you can go with such a simple dependency fetcher.



## System installation

_"Can't I just install Pottery into my system so it's available automatically?"_

Yes, but then you're forcing all collaborators on your project to install it as well. It's not easy to verify that everyone has the correct version when building your project (Pottery is not versioned and may never be.) You'll also need root to update it. I really recommend against doing this.

Besides, you should still add `-DPOTTERY_AVAILABLE=1` to your compiler flags to enable optional dependencies in other libraries. Since you're modifying your project's compiler options anyway, you might as well also add `-Ilib/pottery/include` and extract Pottery there as part of setting up your project.

_"But I need to install it into Buildroot for my embedded system!"_

No, you don't. I get that you want to make the same version available to all apps that will use it. Maybe you think that you can apply a bug fix everywhere by updating it. But updating a shared Pottery is far more likely to break code in all the apps that depend on it, especially during the beta phase when APIs may change.

It is far better if individual apps and shared libraries just fetch their own preferred version of Pottery.

_"But Boost installs itself into /usr/include!"_

If Boost told you to jump off a bridge, would you do it?

_"But I desperately want to install Pottery into my system headers!"_

If you insist. It's not rocket science:

```sh
sudo rsync -r --delete --mkpath --exclude '*.md' \
    include/pottery bindings/cxx/include/pottery_cxx /usr/local/include
```
