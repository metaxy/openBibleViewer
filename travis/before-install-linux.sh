export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:$(pwd)/usr/lib/pkgconfig/" 
export LIBRARY_PATH="${LIBRARY_PATH}:$(pwd)/usr/lib/" 
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$(pwd)/usr/lib/";
echo "deb http://download.opensuse.org/repositories/openSUSE:/Tools/xUbuntu_$(lsb_release -rs) ./" | sudo tee /etc/apt/sources.list.d/suse.list
curl -s "http://download.opensuse.org/repositories/openSUSE:/Tools/xUbuntu_$(lsb_release -rs)/Release.key" | sudo apt-key add -

sudo apt-get update -qq
sudo apt-get install -qq --no-install-recommends clang osc libquazip5-dev zlib1g-dev libclucene-dev
