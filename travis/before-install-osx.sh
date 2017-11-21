export COLUMNS=80
curl -LO https://raw.githubusercontent.com/GiovanniBussi/macports-ci/master/macports-ci
chmod +x ./macports-ci
./macports-ci install
./macports-ci ccache
PATH="/opt/local/bin:$PATH"
./macports-ci ccache
sudo port install qt5
 ./macports-ci ccache --save
port contents qt5-qtbase
