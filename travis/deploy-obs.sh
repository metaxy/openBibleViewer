cat <<EOF > ~/.oscrc
[general]
apiurl = https://api.opensuse.org
[https://api.opensuse.org]
user = ${OBS_USER}
pass = ${OBS_PASSWORD}
EOF

mkdir -p ~/build
cd ~/build
osc co "home:${OBS_USER}:unstable"
osc co "home:${OBS_USER}:stable"
 
