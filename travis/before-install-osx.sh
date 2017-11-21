export COLUMNS=80
curl -LO https://raw.githubusercontent.com/GiovanniBussi/macports-ci/master/macports-ci
chmod +x ./macports-ci
./macports-ci install
./macports-ci ccache
PATH="/opt/local/bin:$PATH"
sudo ./macports-ci ccache
sudo port install qt5 qt5-qtwebkit qt5-qtscript
sudo ./macports-ci ccache --save
sudo port contents qt5-qtbase
