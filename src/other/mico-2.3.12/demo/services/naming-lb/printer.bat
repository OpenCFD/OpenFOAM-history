@rem This file is part of the load-balancing example

@rem ==================

echo "Launching naming service without load-balancing option ..."
start nsd -ORBIIOPAddr inet:127.0.0.1:12556

echo "Wait 2 seconds for the naming service to be ready ..."
sleep 2

echo "Launching servers ..."

@rem This server is named Laser_Admin.Printer using the name format id.kind
start .\Server Laser_Admin.Printer -ORBNamingAddr inet:127.0.0.1:12556

@rem This server is named Injet_Marketing.Printer using the name format id.kind
start .\Server InkJet_Marketing.Printer -ORBNamingAddr inet:127.0.0.1:12556

echo "Wait 2 seconds for the servers to be ready ..."
sleep 2

echo "Launching client ..."

@rem The client looks for the server named Laser_Admin.Printer.
@rem If the naming service does not do load-balancing, the reference to the
@rem server named Laser_Admin.Printer will be given to the client.  However,
@rem if the naming service is launched using the load-balancing option of either
@rem "--lb round_robin" or "--lb random", the reference to one of the servers
@rem with the kind value of Printer will be given to the client.
.\Client Laser_Admin.Printer 10 -ORBNamingAddr inet:127.0.0.1:12556

echo "======== Execution ends ========"


@rem ==================

echo "Launching naming service with round-robin load-balancing option ..."
start nsd --lb round_robin -ORBIIOPAddr inet:127.0.0.1:12556

echo "Wait 2 seconds for the naming service to be ready ..."
sleep 2

echo "Launching servers ..."

@rem This server is named Laser_Admin.Printer using the name format id.kind
start .\Server Laser_Admin.Printer -ORBNamingAddr inet:127.0.0.1:12556

@rem This server is named Injet_Marketing.Printer using the name format id.kind
start .\Server InkJet_Marketing.Printer -ORBNamingAddr inet:127.0.0.1:12556

echo "Wait 2 seconds for the servers to be ready ..."
sleep 2

echo "Launching client ..."

@rem The client looks for the server named Laser_Admin.Printer.
@rem If the naming service does not do load-balancing, the reference to the
@rem server named Laser_Admin.Printer will be given to the client.  However,
@rem if the naming service is launched using the load-balancing option of either
@rem "--lb round_robin" or "--lb random", the reference to one of the servers
@rem with the kind value of Printer will be given to the client.
.\Client Laser_Admin.Printer 10 -ORBNamingAddr inet:127.0.0.1:12556

echo "======== Execution ends ========"


@rem ==================

echo "Launching naming service with random load-balancing option ..."
start nsd --lb random -ORBIIOPAddr inet:127.0.0.1:12556

echo "Wait 2 seconds for the naming service to be ready ..."
sleep 2

echo "Launching servers ..."

@rem This server is named Laser_Admin.Printer using the name format id.kind
start .\Server Laser_Admin.Printer -ORBNamingAddr inet:127.0.0.1:12556

@rem This server is named Injet_Marketing.Printer using the name format id.kind
start .\Server InkJet_Marketing.Printer -ORBNamingAddr inet:127.0.0.1:12556

echo "Wait 2 seconds for the servers to be ready ..."
sleep 2

echo "Launching client ..."

@rem The client looks for the server named Laser_Admin.Printer.
@rem If the naming service does not do load-balancing, the reference to the
@rem server named Laser_Admin.Printer will be given to the client.  However,
@rem if the naming service is launched using the load-balancing option of either
@rem "--lb round_robin" or "--lb random", the reference to one of the servers
@rem with the kind value of Printer will be given to the client.
.\Client Laser_Admin.Printer 10 -ORBNamingAddr inet:127.0.0.1:12556

echo "======== Execution ends ========"
