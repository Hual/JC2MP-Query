<?php
/*
	PHP JC2-MP server query function by King_Hual
	

	Arguments:

	$address - The IP address
	$port [optional] - The port

	Methods:

	update() - Updates the information
	isErroneous() - Returns true if any errors occured, false otherwise
	getHostName() - Returns the hostname of a server
	getPlayers() - Returns the amount of players currently connected to the server
	getMaxPlayers() - Returns the maximum amount of players that can be connected to the server

*/
class JCMPQuery
{	
	private
			$info, $error,
			$address, $port;

	function __construct($address, $port = 7777)
	{
		$this->address = $address;
		$this->port = $port;
		$this->update();
	}

	public function update()
	{
		$handle = pfsockopen("udp://".$this->address, $this->port, $errno, $errstr);
		stream_set_timeout($handle, 2);
		if (!$handle) return ($this->error = true);
		$data = pack("C*",
			0xff,0xff,0xff,0xff,0x54,
			0x53,0x6f,0x75,0x72,0x63,
			0x65,0x20,0x45,0x6e,0x67,
			0x69,0x6e,0x65,0x20,0x51,
			0x75,0x65,0x72,0x79,0x00
		);
		if(!fwrite($handle, $data)) return ($this->error = true);
		$response = fread($handle, 256);
		if(!$response) return ($this->error = true);
		$rdata = explode("\x00", substr($response, 6));
		$pi = explode('/', substr($rdata[1], 9));
		fclose($handle);
		$this->info = array(
			"hostname" => $rdata[0],
			"players" => $pi[0],
			"max-players" => $pi[1]
		);
		return ($this->error = false);
	}

	public function isErroneous()
	{
		return $this->error;
	}

	public function getHostName()
	{
		return $this->info["hostname"];
	}

	public function getPlayers()
	{
		return $this->info["players"];
	}

	public function getMaxPlayers()
	{
		return $this->info["max-players"];
	}
}

?>
