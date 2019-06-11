#https://learnxinyminutes.com/docs/awk/
#initialize data before processing
BEGIN {
    BAN_NUMBER=5
    BAN_DURATION=1
}  

#process data
{
    match($0,/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+/)
    ip = substr($0,RSTART,RLENGTH)
    if (length(ip) != 0 ) {
        #print ip
        fails[ip]++;
    }
}


#finish up
END {
    for (ip in fails) {
        if (fails[ip] > BAN_NUMBER) {

            print ip, "failed", fails[ip], "times and is being banned for", BAN_DURATION, "minutes"
            system("iptables -A INPUT -s " ip " -j DROP" )
            system("bash -c 'iptables -D INPUT -s " ip " -j DROP | sudo at now +" BAN_DURATION " minutes'")
        }
    }
}