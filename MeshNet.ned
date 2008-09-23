import
    "Router",
     "StandardHost",
    "NetworkConfigurator",
	"BurstHost";


channel backboneline
    delay 10us;
    datarate 8*10*1024*1024; // 10 MB/s
endchannel

channel ethernetline
    delay 10us;
    datarate 8*5*100*1024; // 500 KB/s
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
        configurator: NetworkConfigurator;           
 		parameters:
			moduleTypes = "Router BurstHost";
            display: "i=block/cogwheel";
        rte: Router;
            display: "i=misc/node_vs,green4";
      	cli: BurstHost[2];
            display: "i=device/laptop_s";
        server: BurstHost;
            display: "i=device/server_s";
    connections nocheck:
        cli[0].out++ --> ethernetline --> rte.in++;
        cli[0].in++ <-- ethernetline <-- rte.out++;

	cli[1].out++ --> ethernetline --> rte.in++;
        cli[1].in++ <-- ethernetline <-- rte.out++;
  

        server.out++ --> ethernetline --> rte.in++;
        server.in++ <-- ethernetline <-- rte.out++;

endmodule

network flatNet : FlatNet
endnetwork

