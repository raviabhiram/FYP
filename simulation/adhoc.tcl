set ns [new Simulator]
set tf [open adhoc.tr w]
set nf [open adhoc.nam w]
set opt(x) 750
set opt(y) 750
$ns trace-all $tf
$ns namtrace-all-wireless $nf $opt(x) $opt(y)

$ns color 1 Red
$ns color 2 Green

set num [lindex $argv 0]

set topo [new Topography]
$topo load_flatgrid $opt(x) $opt(y)
set god [create-god $num]

$ns node-config -channelType Channel/WirelessChannel
$ns node-config -propType Propagation/TwoRayGround
$ns node-config -llType LL
$ns node-config -ifqType Queue/DropTail/PriQueue
$ns node-config -ifqLen 50
$ns node-config -antType Antenna/OmniAntenna
$ns node-config -macType Mac/802_11
$ns node-config -phyType Phy/WirelessPhy
$ns node-config -adhocRouting AODV
$ns node-config -topoInstance $topo
$ns node-config -agentTrace ON
$ns node-config -routerTrace ON
$ns node-config -macTrace OFF
$ns node-config -movementTrace OFF

for {set i 0} {$i<$num} {incr i} {
	set n($i) [$ns node]
	$n($i) random-motion 1
	set xx [expr rand() * $opt(x)]
	set yy [expr rand() * $opt(y)]
	$n($i) set X_ $xx
	$n($i) set Y_ $yy
	$ns initial_node_pos $n($i) 40
}

set udp0 [new Agent/UDP]
$udp0 set class_ 1
set null0 [new Agent/Null]
set cbr0 [new Application/Traffic/CBR]
$cbr0 set interval_ 0.001
set usnode [expr { round(rand()*$num)}]
puts "UDP Source is $usnode."
set udnode [expr { round(rand()*$num)}]
puts "UDP Destination is $udnode."
$ns attach-agent $n($usnode) $udp0
$ns attach-agent $n($udnode) $null0
$cbr0 attach-agent $udp0
$ns connect $udp0 $null0

set tcp0 [new Agent/TCP]
$tcp0 set class_ 2
set sink0 [new Agent/TCPSink]
set ftp0 [new Application/FTP]
while 1 {
	set tsnode [expr { round(rand()*$num)}]
	if { $tsnode != $usnode } break
}
puts "TCP Source is $tsnode."
set tdnode [expr { round(rand()*$num)}]
puts "TCP Destination is $tdnode."
$ns attach-agent $n($tsnode) $tcp0
$ns attach-agent $n($tdnode) $sink0
$ftp0 attach-agent $tcp0
$ns connect $tcp0 $sink0

proc finish {} {
	global ns nf tf
	$ns flush-trace
	exit 0
}

proc movement {} {
	global ns n opt num
	set interval 5.0
	set now [$ns now]
	set now [expr $now + $interval]
	for {set i 0} {$i<$num} {incr i} {
		set xx [expr rand() * $opt(x)]
		set yy [expr rand() * $opt(y)]
		$ns at $now "$n($i) setdest $xx $yy 50.0"
	}
	$ns at $now "movement"
}

$ns at 0.0 "movement"
$ns at 1.0 "$cbr0 start"
$ns at 2.0 "$ftp0 start"
$ns at 94.0 "$ftp0 stop"
$ns at 95.0 "$cbr0 stop"
$ns at 100.0 "finish"
$ns run
