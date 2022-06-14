sudo apt install python3.8-venv -y

curl -sSL https://install.python-poetry.org | python3 -

# pigpio install
https://abyz.me.uk/rpi/pigpio/download.html

wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install