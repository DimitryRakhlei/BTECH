#!/bin/bash

sudo virsh list --all | gawk '{ if ($2 != "Manjaro" && $2 != "win7-goat" && $2 != "Name") {print $2} }' | xargs -n1 sudo virsh snapshot-delete --current
sleep 1
sudo virsh list --all | gawk '{ if ($2 != "Manjaro" && $2 != "win7-goat" && $2 != "Name") {print $2} }' | xargs -n1 sudo virsh undefine
sleep 1
sudo virsh net-list --all | gawk '{ if ($1 != "default") {print $1} }' | xargs -n1 sudo virsh net-destroy
