#!/bin/bash

CUR_DIR=/usr/local/etc/a3ips/

echo "Clearing iptables"
/sbin/iptables -F
/sbin/iptables -X

echo "Scanning"
while true;
do
    inotifywait -q --event modify /var/log/secure
    cat /var/log/secure | grep sshd.\*Failed | awk -f $CUR_DIR/date_filter.awk | awk -f $CUR_DIR/ips.awk
done
