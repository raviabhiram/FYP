set a 1
while {$a == 1 } {
puts "Enter the Routing Agents in mobile networking"
set opt(chan)           Channel/WirelessChannel    ;# channel type 
set opt(prop)           Propagation/TwoRayGround   ;# radio-propagation model 
set opt(netif)          Phy/WirelessPhy            ;# network interface type 
set opt(mac)            Mac/802_11                 ;# MAC type 
set opt(ifq)            Queue/DropTail/PriQueue    ;# interface queue type 
set opt(ll)             LL                         ;# link layer type 
set opt(ant)            Antenna/OmniAntenna        ;# antenna model 
set opt(ifqlen)         6                        ;# max packet in ifq 
set opt(nn)             6                         ;# number of mobilenodes 
set opt(rp)             AODV                       ;# routing protocol 
set opt(x)              1800   			   ;# X dimension of topography 
set opt(y)              840   			   ;# Y dimension of topography   

### Setting The Simulator Objects
                  
      set ns_ [new Simulator]
#create the nam and trace file:
      set tracefd [open aodv.tr w]
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
      set n2 [$ns_ node]
      set n3 [$ns_ node]
      set n4 [$ns_ node]
      set n5 [$ns_ node]
      
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
      
      $n2 set X_ 350.0
      $n2 set Y_ 700.0
      $n2 set Z_ 0.0

      $n3 set X_ 450.0
      $n3 set Y_ 800.0
      $n3 set Z_ 0.0

      $n4 set X_ 550.0
      $n4 set Y_ 800.0
      $n4 set Z_ 0.0
      
      $n5 set X_ 700.0
      $n5 set Y_ 810.0
      $n5 set Z_ 0.0
      
            
      ## Setting The Node Size
                              
      $ns_ initial_node_pos $Server1 6
      $ns_ initial_node_pos $Server2 6
      $ns_ initial_node_pos $n2 6
      $ns_ initial_node_pos $n3 6
      $ns_ initial_node_pos $n4 6
      $ns_ initial_node_pos $n5 6
     
      

      #### Setting The Labels For Nodes
      
      $ns_ at 0.0 "$Server1 label Server1"
      $ns_ at 0.0 "$Server2 label Server2"
      
      $ns_ at 0.0 "$n2 label node2"
      $ns_ at 0.0 "$n3 label node3"
      $ns_ at 0.0 "$n4 label node4"
      $ns_ at 0.0 "$n5 label node5"
      
      $n2 color green
      $ns_ at 0.0 "$n2 color green"
      
      $n3 color green
      $ns_ at 0.0 "$n3 color green"
      
      $n4 color green
      $ns_ at 0.0 "$n4 color green"
      
      $n5 color green
      $ns_ at 0.0 "$n5 color green"
      
     
      $Server1 color maroon
      $ns_ at 0.0 "$Server1 color maroon"
      
      $Server2 color maroon
      $ns_ at 0.0 "$Server2 color maroon"

      ## SETTING ANIMATION RATE 
      $ns_ at 0.0 "$ns_ set-animation-rate 12.5ms"
       
####  Establishing Communication

      set udp0 [$ns_ create-connection UDP $Server2 LossMonitor $n4 0]
      $udp0 set fid_ 1
      set cbr0 [$udp0 attach-app Traffic/CBR]
      $cbr0 set packetSize_ 1000    
      $cbr0 set interopt_ .07
      $ns_ at 0.0 "$cbr0 start"
      $ns_ at 4.0 "$cbr0 stop"
      
      set udp1 [$ns_ create-connection UDP $n2 LossMonitor $Server1 0]
      $udp1 set fid_ 1
      set cbr1 [$udp1 attach-app Traffic/CBR]
      $cbr1 set packetSize_ 1000    
      $cbr1 set interopt_ .07
      $cbr1 set rate_ 1mb
      $ns_ at 0.1 "$cbr1 start"
      $ns_ at 10.1 "$cbr1 stop"
      
      
      #ANNOTATIONS DETAILS 

#      $ns_ at 0.0 "$ns_ trace-annotate \"MOBILE NODE MOVEMENTS\""
#      $ns_ at 4.1 "$ns_ trace-annotate \"NODE27 CACHE THE DATA FRO SERVER\""
#      #$ns_ at 4.59 "$ns_ trace-annotate \"PACKET LOSS AT NODE27\""     
#      $ns_ at 4.71 "$ns_ trace-annotate \"NODE10 CACHE THE DATA\""      
            
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
puts "want to continue (0/1)" 
set a [gets stdin]
}
