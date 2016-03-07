BEGIN{
	a
	b
}
{
	a=$2;
	b=$3;
	if($1=="s") print ("the packet from the source node is sent at time ",a );
	if($1=="D") print ("the packet is dropped at node",b," at time ",a);
		
}
END{

}
