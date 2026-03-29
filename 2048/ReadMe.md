# 2048
---


## Description:

A C-language based implementation of tile game 2048.
Both MinGW-w64/MSYS2 and Ubuntu Linux platforms are supported.

## Build:

### Build the 2048 under MinGW-w64/MSYS2

Prerequisite: *Git*, *MinGW-w64 toolchain*, *MSYS2 environment*, *GTK library*, and *Inno Setup tool* should be installed first.
#### Launch command prompt to build the 2048
```bat
set PATH=C:\mingw64\bin;%PATH%
```
```bat
make
```
Suppose MinGW-w64 is installed under *C:\mingw64*
#### Launch command prompt to package the 2048
```bat
set PATH=C:\msys64\usr\bin;%PATH%
```
```bat
make release
```
Suppose MSYS2 is installed under *C:\msys64*

### Build the 2048 under Ubuntu Linux
Prerequisite: *git*, *build-essential*, *libgtk2.0-dev*, and *libcanberra-gtk-module* should be installed first.
#### Launch shell to build the 2048
```sh
make
```
#### Launch shell to package the 2048
```bat
make release
```

## Notice:
Above commands are tested under *Windows 10* and *Ubuntu 20.04*.

## Reference:
[2048 Wikipedia](https://en.wikipedia.org/wiki/2048_(video_game))

[64-bit Git for Windows Portable](https://git-scm.com/download/win)

[MinGW-w64 toolchain](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z)

[MSYS2 *portable* environment](https://sourceforge.net/projects/msys2-portable/)

[Prebuilt *GTK w64 library*](https://download.gnome.org/binaries/win64/gtk%2B/2.22/gtk%2B-bundle_2.22.1-20101229_win64.zip)

[Inno Setup tool](https://files.jrsoftware.org/is/5/innosetup-5.6.1.exe)

[Prebuilt *Ubuntu 20.04* VMware Image](https://www.osboxes.org/ubuntu/)

### Appendix

<details>
<summary>Using the <b>Node-API</b> of <b>Node.js</b></summary>

Prerequisite: *nvm* and *node* should be installed first.
```sh
sudo apt-get update
apt policy libssl-dev
apt policy build-essential
apt policy make
apt policy curl
curl https://raw.githubusercontent.com/creationix/nvm/v0.40.4/install.sh | bash
source ~/.profile
nvm --version
nvm ls-remote
nvm install v24.14.1
nvm use 24.14.1
nvm alias default 24.14.1
```

Related packages should be installed either.
```sh
npm install
```

To build native code (i.e. *C* language here).
```sh
npx node-gyp rebuild
```

Then, perform the test i.e. application.
```sh
npm test
```

Above commands are tested under *Ubuntu 24.04*.
</details>

