import
    "Router",
     "StandardHost",
    "NetworkConfigurator",
	"BurstHost";


channel backboneline
    delay 10us;
    datarate 8*10*1024*1024; // 1 MB/s
endchannel

channel ethernetline
    delay 10us;
    datarate 8*8*1024*1024; // 8 MB/s
endchannel



simple IPMeshClientApp            
    gates:
        in: from_ip;
        out: to_ip;
endsimple

simple IPMeshServerApp            
    gates:
        in: from_ip;
        out: to_ip;
endsimple





module FlatNet
    submodules:
        configurator: FlatNetworkConfigurator;           
 		parameters:
				moduleTypes = "Router BurstHost",
				nonIPModuleTypes = "",
				networkAddress = "192.168.0.0",
				netmask = "255.255.0.0";
            display: "i=block/cogwheel";
        rte: Router[3];
            display: "i=misc/node_vs,green4";
      	cli: BurstHost[5];
            display: "i=device/laptop_s";
        server: BurstHost;
            display: "i=device/server_s";
    connections nocheck:
        cli[0].out++ --> ethernetline --> rte[0].in++;
        cli[0].in++ <-- ethernetline <-- rte[0].out++;
		cli[2].out++ --> ethernetline --> rte[0].in++;
        cli[2].in++ <-- ethernetline <-- rte[0].out++;

		cli[1].out++ --> ethernetline --> rte[1].in++;
    	cli[1].in++ <-- ethernetline <-- rte[1].out++;
		cli[3].out++ --> ethernetline --> rte[1].in++;
        cli[3].in++ <-- ethernetline <-- rte[1].out++;
  
        server.out++ --> ethernetline --> rte[2].in++;
        server.in++ <-- ethernetline <-- rte[2].out++;
		cli[4].out++ --> ethernetline --> rte[2].in++;
        cli[4].in++ <-- ethernetline <-- rte[2].out++;

		rte[0].out++ --> backboneline --> rte[1].in++;
		rte[0].in++	<-- backboneline <-- rte[1].out++;
		rte[1].out++ --> backboneline --> rte[2].in++;
		rte[1].in++	<-- backboneline <-- rte[2].out++;
		rte[2].out++ --> backboneline --> rte[0].in++;
		rte[2].in++	<-- backboneline <-- rte[0].out++;


endmodule

network flatNet : FlatNet
endnetwork

