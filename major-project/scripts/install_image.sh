#!/bin/sh

#to install a windows 7 image
sudo virt-install --import --vcpus 2 --memory 4096 --name win7-goat --os-variant=win7  --network network=default --disk device=disk,path=./w7.qcow2,bus=sata
