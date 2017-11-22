export COLUMNS=80
sudo xcode-select --install || true
curl -LO https://raw.githubusercontent.com/GiovanniBussi/macports-ci/master/macports-ci
chmod +x ./macports-ci
./macports-ci install
PATH="/opt/local/bin:$PATH"
sudo ./macports-ci ccache
sudo port install qt5-qtbase bzip2 qt5-qtmacextras qt5-qtwebkit qt5-qtscript zlib
sudo ./macports-ci ccache --save
brew update && brew install clucene quazip
