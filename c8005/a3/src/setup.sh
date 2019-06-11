#!/bin/bash
if [[ $EUID -ne 0 ]]; then
    sudo ${0}
    exit 0
fi

dnf install python-yaml -y
