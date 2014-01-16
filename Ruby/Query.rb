=begin 

	Ruby JC2-MP server query function by King_Hual
	
	
	Arguments:
	
	address - The IP address
	port [optional] - The port
	
	Methods:
	
	update() - Updates the information
	isErroneous() - Returns true if any errors occured, false otherwise
	getHostName() - Returns the hostname of a server
	getPlayers() - Returns the amount of players currently connected to the server
	getMaxPlayers() - Returns the maximum amount of players that can be connected to the server

=end

require 'socket'

class JCMPQuery
  def initialize(address, port = 7777)
    @address = address
    @port = port
    update()
  end
  
  def update()
    sock = UDPSocket.new
    @error = false
    begin 
      sdata = [
        0xff,0xff,0xff,0xff,0x54,
        0x53,0x6f,0x75,0x72,0x63,
        0x65,0x20,0x45,0x6e,0x67,
        0x69,0x6e,0x65,0x20,0x51,
        0x75,0x65,0x72,0x79,0x00 
      ].pack("C*")
      sock.connect(@address, @port)
      sock.send(sdata, 0)
      rdata = sock.recvfrom(256)[0]
      data = rdata[6, rdata.length-6].split("\0", 3)
      pi = data[1][9, data[1].length-9].split("/", 2)
      @hostname = data[0]
      @players = Integer(pi[0])
      @max_players = Integer(pi[1])
    rescue Exception
      @error = true
    end
    sock.close()
    return @error
  end
  
  def isErroneous()
    return @error
  end
  
  def getHostName()
    return @hostname
  end
  
  def getPlayers()
    return @players
  end
  
  def getMaxPlayers()
    return @max_players
  end
end
