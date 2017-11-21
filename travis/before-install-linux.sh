echo "Buildung CLucene"
wget http://downloads.sourceforge.net/clucene/clucene-core-2.3.3.4.tar.gz 
tar -zxvf clucene-core-2.3.3.4.tar.gz 
cd clucene-core-2.3.3.4/ 
mkdir build 
cd build 
cmake -DCMAKE_INSTALL_PREFIX='../../usr' -DBUILD_CONTRIBS_LIB=ON .. 
make 
make install 

cd .. 
cd .. 
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:$(pwd)/usr/lib/pkgconfig/" 
export LIBRARY_PATH="${LIBRARY_PATH}:$(pwd)/usr/lib/" 
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$(pwd)/usr/lib/";
echo "deb http://download.opensuse.org/repositories/openSUSE:/Tools/xUbuntu_$(lsb_release -rs) ./" | sudo tee /etc/apt/sources.list.d/suse.list
curl -s "http://download.opensuse.org/repositories/openSUSE:/Tools/xUbuntu_$(lsb_release -rs)/Release.key" | sudo apt-key add -

sudo apt-get update -qq
sudo apt-get install -qq --no-install-recommends clang osc libquazip-dev libquazip-dev zlib1g-dev qt5-default qttools5-dev-tools qtscript5-dev libqt5webkit5-dev
