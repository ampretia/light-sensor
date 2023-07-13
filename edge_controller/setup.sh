#!/bin/bash

set -ex -o pipefail

sudo apt update
sudo apt install python3.8-venv -y
curl -sSL https://install.python-poetry.org | python3 -

mkdir -p github.com && cd github.com
git clone https://github.com/ampretia/light-sensor.git

wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
cd -
