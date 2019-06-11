#!/bin/sh
#
# Author Dimitry Rakhlei @ A00849367
#
INTER=wlp3s0
IPT=/sbin/iptables

# FIRST WE CLEAR RULES
$IPT -F
$IPT -X

# SET DEFAULT POLICIES TO DROP
$IPT -P INPUT DROP
$IPT -P FORWARD DROP
$IPT -P OUTPUT DROP

# CLEAR SCREEN
reset


# CREATE ACCOUNTING TABLES
    # SSH TRAFFIC ACCOUNTING
    $IPT -N SSH_TRAFFIC
    $IPT -A SSH_TRAFFIC -p tcp -j ACCEPT
    $IPT -A SSH_TRAFFIC -p udp -j ACCEPT
    # WEB TRAFFIC ACCOUNTING
    $IPT -N WWW_TRAFFIC
    $IPT -A WWW_TRAFFIC -p tcp -j ACCEPT
    $IPT -A WWW_TRAFFIC -p udp -j ACCEPT
    # ALL OTHER TRAFFIC ACCOUNTING
    $IPT -N OTHER_TRAFFIC
    $IPT -A OTHER_TRAFFIC -j ACCEPT  
    

# CREATE CUSTOM TABLES
    # TCP INPUT RULES
    $IPT -N TCP_INPUT_RULES
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --dport 0 -j DROP # tcp [ 0 ]
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --dport 80 --sport 0:1023 -j DROP  # HTTP  [0 - 1024]
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --dport 443 --sport 0:1023 -j DROP # HTTPS [0 - 1024]
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --dport 22 -j SSH_TRAFFIC  # SSH
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --dport 80 -j WWW_TRAFFIC  # HTTP
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --dport 443 -j WWW_TRAFFIC # HTTPS
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --sport 22 -j SSH_TRAFFIC  # SSH
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --sport 80 -j WWW_TRAFFIC  # HTTP
    $IPT -A TCP_INPUT_RULES -i $INTER -p tcp --sport 443 -j WWW_TRAFFIC # HTTPS
    
    # UDP INPUT RULES
    $IPT -N UDP_INPUT_RULES
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --dport 0 -j DROP # udp [ 0 ]
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --dport 80 --sport 0:1023 -j DROP  # HTTP  [0 - 1024]
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --dport 443 --sport 0:1023 -j DROP # HTTPS [0 - 1024]
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --dport 22 -j SSH_TRAFFIC  # SSH
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --dport 80 -j WWW_TRAFFIC  # HTTP
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --dport 443 -j WWW_TRAFFIC # HTTPS
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --sport 22 -j SSH_TRAFFIC  # SSH
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --sport 80 -j WWW_TRAFFIC  # HTTP
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --sport 443 -j WWW_TRAFFIC # HTTPS
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp -m udp --dport 53 -j ACCEPT # DNS
    $IPT -A UDP_INPUT_RULES -i $INTER -p udp --dport 67:68 --sport 67:68 -j ACCEPT # DHCP
    
    # TCP OUTPUT RULES
    $IPT -N TCP_OUTPUT_RULES
    $IPT -A TCP_OUTPUT_RULES -p tcp --dport 22 --sport 22 -j SSH_TRAFFIC
    $IPT -A TCP_OUTPUT_RULES -p tcp --dport 80 -j WWW_TRAFFIC
    $IPT -A TCP_OUTPUT_RULES -p tcp --dport 443 -j WWW_TRAFFIC
    $IPT -A TCP_OUTPUT_RULES -p tcp --sport 80 -j WWW_TRAFFIC
    $IPT -A TCP_OUTPUT_RULES -p tcp --sport 443 -j WWW_TRAFFIC
    
    # UDP OUTPUT RULES
    $IPT -N UDP_OUTPUT_RULES
    $IPT -A UDP_OUTPUT_RULES -p udp --dport 22 --sport 22 -j SSH_TRAFFIC
    $IPT -A UDP_OUTPUT_RULES -p udp --dport 80 -j WWW_TRAFFIC
    $IPT -A UDP_OUTPUT_RULES -p udp --dport 443 -j WWW_TRAFFIC
    $IPT -A UDP_OUTPUT_RULES -p udp --sport 80 -j WWW_TRAFFIC
    $IPT -A UDP_OUTPUT_RULES -p udp --sport 443 -j WWW_TRAFFIC
    $IPT -A UDP_OUTPUT_RULES -p udp -m udp --sport 53 -j ACCEPT # DNS
    

  

# [ INPUT REGION ]
    # [ TCP ]
    $IPT -A INPUT -j TCP_INPUT_RULES
    # [ UDP ]
    $IPT -A INPUT -j UDP_INPUT_RULES
    
# [ FORWARD REGION ]
    # [ TCP ]
    # [ UDP ]
    
# [ OUTPUT REGION ]
    # [ TCP ]
    $IPT -A OUTPUT -j TCP_OUTPUT_RULES
    # [ UDP ]                
    $IPT -A OUTPUT -j UDP_OUTPUT_RULES
    

# DISPLAY CHANGES
$IPT -L -v -n -x
