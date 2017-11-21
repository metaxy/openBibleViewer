export COLUMNS=80
curl -LO https://raw.githubusercontent.com/GiovanniBussi/macports-ci/master/macports-ci
chmod +x ./macports-ci
./macports-ci install
PATH="/opt/local/bin:$PATH"
brew update && brew install clucene

sudo port install qt5
port contents qt5
ls /usr/local/opt/qt5
