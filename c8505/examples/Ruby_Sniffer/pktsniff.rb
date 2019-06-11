#!/usr/bin/ruby
require 'rubygems'
require 'packetfu'

# Simple program to sniff all packets on the default NIC. 
# User can use the "v" switch for dumping the packet contents in hex
# The "tcp", "udp", "icmp"  and "u" switches can ne used to dump specific 
# packets contents in hex. 

puts "Sniffing Packets............:"
include PacketFu
iface = @interface

def sniff(iface)
  cap = Capture.new(:iface => iface, :start => true)
  cap.stream.each do |p|
    pkt = Packet.parse p
    if pkt.is_tcp?
      next if pkt.ip_saddr == Utils.ifconfig(iface)[:ip_saddr]
      packet_info = [pkt.ip_saddr, pkt.tcp_sport, pkt.ip_daddr, pkt.tcp_dport, pkt.size, pkt.proto.last]
      puts "\n"
      puts "%-15s:%-4d -> %10s:%d %-4d %s" % packet_info
      if ARGV[0] == "v" && ARGV[1] == "tcp"
	puts pkt.inspect_hex(:layers)
      end
    elsif pkt.is_udp?
       packet_info = [pkt.ip_saddr, pkt.udp_sport, pkt.ip_daddr, pkt.udp_dport, pkt.size, pkt.proto.last]
       puts "\n"
       puts "%-15s:%-4d -> %10s:%d %-4d %s" % packet_info
      if ARGV[0] == "v" && ARGV[1] == "udp"
	puts pkt.inspect_hex(:layers)
      end
    elsif pkt.is_icmp?
      packet_info = [pkt.ip_saddr, pkt.ip_daddr, pkt.size, pkt.proto.last]
      puts "\n"
      puts "%-15s -> %-15s %-4d %s" % packet_info
     if ARGV[0] == "v" && ARGV[1] == "icmp"
	puts pkt.inspect_hex(:layers)
      end
    else
      puts "\n"
      puts "Unknown Protocol - Probably Link Layer!"
      if ARGV[0] == "v" && ARGV[1] == "u"
	puts pkt.inspect_hex(:layers)
      end
    end
  end
end

begin
    sniff(iface)
    # ^C to stop sniffing
    rescue Interrupt
    puts "\nPacket Capture stopped by interrupt signal."
    exit 0
end
