/*
	Java JC2-MP server query function by King_Hual
	
	
	Arguments:
	
	String address - The IP address
	int port [optional] - The port
	
	Methods:
	
	update() - Updates the information
	isErroneous() - Returns true if any errors occured, false otherwise
	getHostName() - Returns the hostname of a server
	getPlayers() - Returns the amount of players currently connected to the server
	getMaxPlayers() - Returns the maximum amount of players that can be connected to the server

*/

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.charset.StandardCharsets;

public class JCMPQuery
{
	private String
		_address,
		_hostname;
	private int
		_port,
		_players,
		_maxPlayers;
	private boolean
		_error;
	
	public JCMPQuery(String address, int port)
	{
		this.initialize(address, port);
	}
	
	public JCMPQuery(String address)
	{
		this.initialize(address, 7777);
	}
	
	private void initialize(String address, int port)
	{
		this._address = address;
		this._port = port;
		this.update();
	}
	
	public boolean update()
	{
		DatagramSocket socket = null;
		boolean error = false;
		try
		{
			socket = new DatagramSocket();
			final byte[]
				header = new byte[]
				{
					(byte)0xff, (byte)0xff, (byte)0xff, (byte)0xff, (byte)0x54,
					(byte)0x53, (byte)0x6f, (byte)0x75, (byte)0x72, (byte)0x63,
					(byte)0x65, (byte)0x20, (byte)0x45, (byte)0x6e, (byte)0x67, 
					(byte)0x69, (byte)0x6e, (byte)0x65, (byte)0x20, (byte)0x51,
					(byte)0x75, (byte)0x65, (byte)0x72, (byte)0x79, (byte)0x00
				},
				buf = new byte[256];
			
			DatagramPacket packet = new DatagramPacket(header, header.length,  InetAddress.getByName(this._address), this._port);
			socket.send(packet);
			packet = new DatagramPacket(buf, buf.length);
			socket.receive(packet);
			
			String[] data = new String(buf, StandardCharsets.UTF_8).substring(6).split("\0", 3);
			String[] pi = data[1].substring(9).split("/");
			this._hostname = data[0];
			this._players = Integer.parseInt(pi[0]);
			this._maxPlayers = Integer.parseInt(pi[1]);
		}
		catch(Exception e)
		{
			error = true;
		}
		if(socket != null)
			socket.close();
		return this._error = error;
	}
	
	public boolean isErroneous()
	{
		return this._error;
	}
	
	public String getHostName()
	{
		return this._hostname;
	}
	
	public int getPlayers()
	{
		return this._players;
	}
	
	public int getMaxPlayers()
	{
		return this._maxPlayers;
	}
}
