#!/bin/bash

if [ -z $1 ]; then
	$1='.'
fi

trap "exit" INT TERM ERR
trap "kill 0" EXIT

mkdir -p $1/runtime
mkdir -p $1/downloads
cd $1/downloads

function download_windows() {
	echo Downloading Windows 7
	wget https://az792536.vo.msecnd.net/vms/VMBuild_20180306/VMWare/IE8/IE8.Win7.VMWare.zip -O windows7.zip
	echo Unzipping windows7.zip
	unzip windows7.zip
}

function convert_image() {
	echo Converting OVF file to QCOW2 this may take a while.
	qemu-img convert -f vmdk -O qcow2 IE8-Win7-VMWare/IE8-Win7-VMWare-disk1.vmdk w7.qcow2
	../scripts/host_script.sh ../ &
	virt-install --import --vcpus 2 --memory 2048 --name win7-goat --os-variant=win7 --network network=default --disk device=disk,path=./w7.qcow2,bus=sata
	
	mkdir -p ../config/xml
	virsh dumpxml win7-goat > ../config/xml/win7-goat.xml
}

function try_install() {
	../scripts/host_script.sh ../ &
	virt-install --import --vcpus 2 --memory 2048 --name win7-goat --os-variant=win7 --network network=default --disk device=disk,path=./w7.qcow2,bus=sata
	
	mkdir -p ../config/xml
	virsh dumpxml win7-goat > ../config/xml/win7-goat.xml
}

function run_image() {
	echo w7.qcow2 file already present.
	virsh start win7-goat
	../scripts/host_script.sh ../ &
	virt-viewer win7-goat
}

function extract_image() {
	rm -r IE8-Win7-VMWare
	clear
	echo Extracting the ZIP
	unzip "windows7.zip"
}


if [ -f w7.qcow2 ]; then
	echo Image w7.qcow2 exists. Try using -start flag. If that doesnt work delete downloads/w7.qcow2 and run this again.
	try_install
else
	if [ -f IE8-Win7-VMWare/IE8-Win7-VMWare-disk1.vmdk ]; then
		echo "Found: IE8-Win7-VMWare/IE8-Win7-VMWare-disk1.vmdk"
		echo "Checking SHA256"
		sha256=`sha256sum IE8-Win7-VMWare/IE8-Win7-VMWare-disk1.vmdk | awk '{ print $1 }'`
		if [ "fbe0411d21f1b7b41d9651c246bf5084e06d447bcfc706641b9f1ec061b7df26" != $sha256 ]; then
			extract_image
		fi
		convert_image
	else
		if [ -f windows7.zip ]; then
			echo Found windows7
		else
			download_windows
		fi
		extract_image
		convert_image		
	fi
fi

