export COLUMNS=80
xcode-select --install || true
curl -LO https://raw.githubusercontent.com/GiovanniBussi/macports-ci/master/macports-ci
chmod +x ./macports-ci
./macports-ci install
PATH="/opt/local/bin:$PATH"
sudo ./macports-ci ccache
sudo port install qt5 qt5-qtwebkit qt5-qtscript
sudo ./macports-ci ccache --save
brew update && brew install clucene
