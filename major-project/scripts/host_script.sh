#!/bin/bash

if [ -z $1 ]; then
    echo "Pass the path to the src/scripts directory in the arguments."
    exit
fi

HOST_IP=$(ip addr | grep virbr0 | grep -E -o "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)" | head -1)


python -m "http.server" --bind $HOST_IP -d $1 80