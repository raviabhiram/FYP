BEGIN {
	seqno = 0;
	droppedPackets = 0;
	receivedPackets = 0;
	sentPackets = 0;
	pkts = 0;
	delay = 0;
}
{
	#packet delivery ratio
	if(($1 == "r")) {
		receivedPackets++;
	} 
	else if ($1 == "d"){
		droppedPackets++;
	}

	#end-to-end delay
	
	if($1 == "a") {
		pkts++;
		start_time[$4][$3] = $2;
	} 
	else if($1 == "r") {
		end_time[$4][$3] = $2;
	}
	else if($1 == "d") {
		end_time[$4][$3] = -1;
	}
	if($1 == "a") {
		sentPackets++;
	}
}
END {
	for(i in start_time) {
		for(j in start_time[i]) {
			if(end_time[i][j]!=-1 && end_time[i][j]!=NULL) {
				delay = delay + end_time[i][j] - start_time[i][j];
			}
		}
	}
	n_to_n_delay = delay/sentPackets;
	print "\n";
	print "Delivery Ratio = " receivedPackets/sentPackets*100 "%";
	print "" receivedPackets/sentPackets*100 >> "sir_throughput.xg";
	print "Average End-to-End Delay = " n_to_n_delay * 1000 " ms";
	print "" n_to_n_delay * 1000 >> "sir_delay.xg";
	print "\n";
}
