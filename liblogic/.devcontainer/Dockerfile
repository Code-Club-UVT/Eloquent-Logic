FROM opensuse/leap:16.0
RUN useradd -ms /bin/bash vscode
RUN zypper --gpg-auto-import-keys addrepo -f https://download.opensuse.org/repositories/Kernel:tools/16.0/Kernel:tools.repo && zypper --gpg-auto-import-keys refresh && zypper --gpg-auto-import-keys install -y icu libicu-devel libboost_headers-devel gdb cmake git openssh gcc gcc-c++ python313 python313-pip python313-setuptools
USER vscode
RUN pip3 install --user conan