sudo apt-get install libseccomp-dev
mkdir build && cd build && cmake .. && make && sudo make install

sudo python3 /bindings/python/setup.py install

# SandboxCommandRunner
