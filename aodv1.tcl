puts "Enter the Routing Agents in mobile networking"
set opt(chan)           Channel/WirelessChannel    ;# channel type 
set opt(prop)           Propagation/TwoRayGround   ;# radio-propagation model 
set opt(netif)          Phy/WirelessPhy            ;# network interface type 
set opt(mac)            Mac/802_11                 ;# MAC type 
set opt(ifq)            Queue/DropTail/PriQueue    ;# interface queue type 
set opt(ll)             LL                         ;# link layer type 
set opt(ant)            Antenna/OmniAntenna        ;# antenna model 
set opt(ifqlen)         50                         ;# max packet in ifq 
set opt(nn)             50                         ;# number of mobilenodes 
set opt(rp)             AODV                       ;# routing protocol 
set opt(x)              1800           ;# X dimension of topography 
set opt(y)              840            ;# Y dimension of topography   

puts "Enter number of source nodes:- "
set nsrc [gets stdin]

### Setting The Simulator Objects
                  
      set ns_ [new Simulator]
#create the nam and trace file:
      set tracefd [open aodv1.tr w]
      $ns_ trace-all $tracefd

      set namtrace [open aodv.nam w]
      $ns_ namtrace-all-wireless $namtrace  $opt(x) $opt(y)

      set topo [new Topography]
      $topo load_flatgrid $opt(x) $opt(y)
      create-god $opt(nn)
      set chan_1_ [new $opt(chan)]
      
####  Setting The Distance Variables
                       
      # For model 'TwoRayGround'
      set dist(5m)  7.69113e-06
      set dist(9m)  2.37381e-06
      set dist(10m) 1.92278e-06
      set dist(11m) 1.58908e-06
      set dist(12m) 1.33527e-06
      set dist(13m) 1.13774e-06
      set dist(14m) 9.81011e-07
      set dist(15m) 8.54570e-07
      set dist(16m) 7.51087e-07
      set dist(20m) 4.80696e-07
      set dist(25m) 3.07645e-07
      set dist(30m) 2.13643e-07
      set dist(35m) 1.56962e-07
      set dist(40m) 1.56962e-10
      set dist(45m) 1.56962e-11
      set dist(50m) 1.20174e-13
      Phy/WirelessPhy set CSThresh_ $dist(50m)
      Phy/WirelessPhy set RXThresh_ $dist(50m)

#  Defining Node Configuration
                        
                  $ns_ node-config -adhocRouting $opt(rp) \
                   -llType $opt(ll) \
                   -macType $opt(mac) \
                   -ifqType $opt(ifq) \
                   -ifqLen $opt(ifqlen) \
                   -antType $opt(ant) \
                   -propType $opt(prop) \
                   -phyType $opt(netif) \
                   -topoInstance $topo \
                   -agentTrace ON \
                   -routerTrace ON \
                   -macTrace ON \
                   -movementTrace ON \
                   -channel $chan_1_

###  Creating The WIRELESS NODES
                  
      set Server1 [$ns_ node]
      set Server2 [$ns_ node]
      for {set i 0} {$i<48} {incr i} {
        set n($i) [$ns_ node]
      }      
      set opt(seed) 0.1
      set a [ns-random $opt(seed)]
      set i 0
      while {$i < 5} {
      incr i
      }
            

###  Setting The Initial Positions of Nodes

      $Server1 set X_ 200.0
      $Server1 set Y_ 200.0
      $Server1 set Z_ 0.0
      
      $Server2 set X_ 300.0
      $Server2 set Y_ 400.0
      $Server2 set Z_ 0.0
      
      for {set i 0} {$i<48} {incr i} {
        $n($i) set X_ rand()*1000
        $n($i) set Y_ rand()*1000
        $n($i) set Z_ rand()*1000
      }
      ## Setting The Node Size
      $ns_ initial_node_pos $Server1 50
      $ns_ initial_node_pos $Server2 50
      for {set i 0} {$i<48} {incr i} {
        $ns_ initial_node_pos $n($i) 50
      }
      ## SETTING ANIMATION RATE 
      $ns_ at 0.0 "$ns_ set-animation-rate 12.5ms"
####  Establishing Communication
      for {set i 0} {$i<$nsrc} {incr i} {
        set vals rand()*1000
        set vals [expr int($vals) % 48]
        set vald rand()*1000
        set vald [expr int($vald) % 48]
        set udp($i) [$ns_ create-connection UDP $n($vals) LossMonitor $n($vald) 0]
        $udp($i) set fid_ 1
        set cbr($i) [$udp($i) attach-app Traffic/CBR]
        $cbr($i) set packetSize_ 1000    
        $cbr($i) set interopt_ .07
        $ns_ at 0.0 "$cbr($i) start"
        $ns_ at 10.0 "$cbr($i) stop"
      }
      ### PROCEDURE TO STOP 
      proc stop {} {
            
                        global ns_ tracefd
                        $ns_ flush-trace
                        close $tracefd           
                        exit 0

                   }
      puts "Starting Simulation........"
      $ns_ at 25.0 "stop"
      $ns_ run 
}
