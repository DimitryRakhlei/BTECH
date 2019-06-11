#!/bin/python3
from sock_lib.connection import connection
from sys import argv
from threading import Thread
from selectors import select
import socket

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
    with open("file.yaml", "r") as f:
        return yaml.load(f)

def save_settings(data):
    with open("file.yaml", "w") as f:
        yaml.dump(data, f, default_flow_style=False)
        
def parse_settings(settings:dict):
    return test

def forwarder_loop(connection_map:dict):
    epoll = select.epoll()

    connection_sockets = {}
    source_sockets = {}
    destination_sockets = {}

    pending_destination_data = {}
    pending_source_data = {}

    print("starting forwarder loop")
    for source, destination in connection_map.items():
        print("creating listener socket on port", source[1])
        conn = connection()
        listen_sock = conn.create_listener(source[1])
        connection_sockets[listen_sock.fileno()] = (listen_sock , destination)
        epoll.register(listen_sock.fileno() , select.EPOLLIN | select.EPOLLET | select.EPOLLHUP)
    try:
        while True:
            events = epoll.poll(1)
            for fileno, event in events:
                if fileno in connection_sockets.keys():
                    #print("new")
                    #print("found one", connection_sockets[fileno], "with event", event)
                    try:
                        while True:
                            source_sock, _ = connection_sockets[fileno][0].accept()
                            source_sock.setblocking(0)
                            epoll.register(source_sock.fileno(), select.EPOLLIN  | select.EPOLLET | select.EPOLLHUP)
                            print(source_sock)


                            destination_sock = connection().create_client()
                            epoll.register(destination_sock.fileno(), select.EPOLLIN | select.EPOLLOUT | select.EPOLLET | select.EPOLLHUP)
                            
                            destination_sockets[destination_sock.fileno()] = (destination_sock, source_sock.fileno())    
                            source_sockets[source_sock.fileno()] = (source_sock, destination_sock.fileno())
                            
                            destination_sock.connect(connection_sockets[fileno][1])

                    except socket.error:
                        pass
                elif event & select.EPOLLIN:
                    #print("epollin", fileno)
                    try:
                        if fileno in source_sockets:
                            data = source_sockets[fileno][0].recv(1024)
                            print("data => ", len(data))
                            if not data:
                                print("no more data")
                            elif source_sockets[fileno][1] in pending_destination_data:
                                pending_destination_data[source_sockets[fileno][1]] += data
                                epoll.modify(source_sockets[fileno][1], select.EPOLLOUT)
                            else:
                                pending_destination_data[source_sockets[fileno][1]] = data
                                epoll.modify(source_sockets[fileno][1], select.EPOLLOUT)
                            
                        elif fileno in destination_sockets:
                            data = destination_sockets[fileno][0].recv(1024)
                            print("data <= ", len(data))
                            source_sockets[destination_sockets[fileno][1]][0].send(data)

                    except Exception as e:
                        print("Error:", str(e))

                elif event & select.EPOLLOUT:
                    #print("epollout", fileno)
                    if fileno in pending_destination_data and len(pending_destination_data[fileno]) > 0:
                        sent = destination_sockets[fileno][0].send(pending_destination_data[fileno])
                        pending_destination_data[fileno] = pending_destination_data[fileno][sent:]
                    else:
                        epoll.unregister(fileno)
                        epoll.register(fileno, select.EPOLLIN | select.EPOLLOUT | select.EPOLLET | select.EPOLLHUP)
                        pass

                elif event & select.EPOLLHUP:
                    print("connection is over")
                    epoll.unregister(fileno)
                    
                else: print("didnt find any")
    except KeyboardInterrupt:
        print("\nepoll interrupted via keyboard")
    finally:
        for tup in connection_sockets.values():
            print("closing", tup)
            epoll.unregister(tup[0].fileno())
            tup[0].close()
        print("closing", epoll)
        epoll.close()


if __name__ == "__main__":
    forward_dict = parse_settings(load_settings())
    connection_map = {}
    for k1, v1 in forward_dict.items():
        for k2, v2 in v1.items():
            connection_map[(k1,k2)] = (v2[0], v2[1])
    forwarder_loop(connection_map)