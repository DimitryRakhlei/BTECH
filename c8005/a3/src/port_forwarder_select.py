#!/bin/python3
from sock_lib.connection import connection
from sys import argv
from selectors import select
import sys
import queue
import socket
import yaml

#test config
test = {
    "127.0.0.1" : 
    { 
        80 : ["192.168.1.70", 9005],
        9005 : ["192.168.1.70", 8005]
    },
}

# port : ("ip", port)
forward_dict = {}

def load_settings():
    with open("data.yaml", "r") as f:
        return yaml.load(f)

def save_settings(data):
    with open("data.yaml", "w") as f:
        yaml.dump(data, f, default_flow_style=False)

def parse_settings(settings:dict):
    return test

def forwarder_loop(connection_map:dict):
    connection_sockets = []
    source_sockets = {}
    destination_sockets = {}

    inputs = []
    outputs = []

    message_queues = {}

    print("starting forwarder loop")
    for source, destination in connection_map.items():
        print("creating listener socket on port", source[1])
        conn = connection()
        listen_sock = conn.create_listener(source[1])
        inputs.append(listen_sock)
        connection_sockets.append(listen_sock)
    try:
        
        while inputs:
            readable, writable, exceptional = select.select(inputs, outputs, inputs)

            # Handle inputs
            for s in readable:

                if s in connection_sockets:
                    try:
                        # A "readable" server socket is ready to accept a connection
                        conn, client_address = s.accept()
                        #print( 'new connection from', client_address)
                        conn.setblocking(0)
                        inputs.append(conn)
                        outputs.append(conn)
                        
                        # Give the connection a queue for data we want to send
                        message_queues[conn] = queue.Queue()

                        dest_sock = connection().create_client()
                        dest_sock.setblocking(1)
                        dest_sock.connect(connection_map[(client_address[0], conn.getsockname()[1])])
                        dest_sock.setblocking(0)
                        message_queues[dest_sock] = queue.Queue()

                        inputs.append(dest_sock)
                        outputs.append(dest_sock)

                        destination_sockets[conn] = dest_sock
                        source_sockets[dest_sock] = conn
                        
                    except Exception:
                        pass
                else:
                    data = s.recv(1024)
                    if data:
                        if s in destination_sockets:
                            # A readable client socket has data
                            #print ('received "%s" from %s' % (data, s.getpeername()))
                            message_queues[destination_sockets[s]].put(data)
                            # Add output channel for response
                            if destination_sockets[s] not in outputs:
                                outputs.append(destination_sockets[s])
                        elif s in source_sockets:
                            # A readable client socket has data
                            #print ('received "%s" from %s' % (data, s.getpeername()))
                            message_queues[source_sockets[s]].put(data)
                            # Add output channel for response
                            if source_sockets[s] not in outputs:
                                outputs.append(source_sockets[s])
                    else:
                        # Interpret empty result as closed connection
                        #print ('closing', client_address, 'after reading no data')
                        # Stop listening for input on the connection
                        if s in outputs:
                            outputs.remove(s)
                        inputs.remove(s)
                        s.close()

                        # Remove message queue
                        del message_queues[s]
            # Handle outputs
            for s in writable:
                try:
                    next_msg = message_queues[s].get_nowait()
                except queue.Empty: 
                    # No messages waiting so stop checking for writability.
                    #print ('output queue for', s.getpeername(), 'is empty')
                    outputs.remove(s)
                except KeyError:
                    #Does not exist yet
                    message_queues[s] = queue.Queue()

                else:
                    #print ('sending "%s" to %s' % (next_msg, s.getpeername()))
                    s.send(next_msg)

            # Handle "exceptional conditions"
            for s in exceptional:
                #print('handling exceptional condition for', s.getpeername())
                # Stop listening for input on the connection
                inputs.remove(s)
                if s in outputs:
                    outputs.remove(s)
                s.close()

                # Remove message queue
                del message_queues[s]


    except KeyboardInterrupt:
        print("\nselect interrupted via keyboard")
    finally:
        print("done")
        


if __name__ == "__main__":
    forward_dict = load_settings()
    connection_map = {}
    for k1, v1 in forward_dict.items():
        for k2, v2 in v1.items():
            connection_map[(k1,k2)] = (v2[0], v2[1])
    forwarder_loop(connection_map)