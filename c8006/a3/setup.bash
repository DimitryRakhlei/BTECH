#!/bin/bash

IPS_DIR=/usr/local/etc/a3ips/

echo "Setting up IPS"
dnf install inotify-tools -y
atq

mkdir -p $IPS_DIR
cp ./src/runner.sh $IPS_DIR
cp ./src/ips.awk $IPS_DIR
cp ./src/date_filter.awk $IPS_DIR
chmod +x $IPS_DIR*

cp ips.service /etc/systemd/system/
chmod +x /etc/systemd/system/ips.service
systemctl daemon-reload
systemctl enable ips.service
systemctl start ips.service
echo "service started"

echo "Setup complete"
