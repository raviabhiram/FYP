BEGIN {
	seqno = -1;
	droppedPackets = 0;
	receivedPackets = 0;
	sentPackets = 0;
	count = 0;
}
{
	#packet delivery ratio
	if($1 == "a" && seqno < $6) {
		seqno = $3;
	} 
	else if(($1 == "r")) {
		receivedPackets++;
	} 
	else if ($1 == "d"){
		droppedPackets++;
	}
	#end-to-end delay
	if($1 == "a") {
		start_time[$6] = $2;
	} 
	else if($1 == "r") {
		end_time[$6] = $2;
	}
	else if($1 == "d") {
		end_time[$6] = -1;
	}
	if($1 == "a") {
		sentPackets++;
	}
}
END {
	for(i=0; i<=seqno; i++) {
		if(end_time[i] > 0) {
			delay[i] = end_time[i] - start_time[i];
			count++;
		}
		else
		{
			delay[i] = -1;
		}
	}
	count = 10;
	for(i=0; i<count; i++) {
		if(delay[i] > 0) {
			n_to_n_delay = n_to_n_delay + delay[i];
		}
	}
	n_to_n_delay = n_to_n_delay/count;
	print "\n";
	print "ReceivedPackets = " receivedPackets;
	print "Total Packets Sent = " sentPackets;
	print "Delivery Ratio = " receivedPackets/sentPackets*100 "%";
	print "" receivedPackets/sentPackets*100 >> "sir_throughput.xg";
	print "Average End-to-End Delay = " n_to_n_delay * 1000 " ms";
	print "" n_to_n_delay * 1000 >> "sir_delay.xg";
	print "\n";
}