using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

class JCMPQuery
{
    private String
        _address,
        _hostname;
    private UInt32
        _players,
        _max_players;
    private Int32
        _port;
    private Boolean
        _error;

    public JCMPQuery(String address, Int32 port = 7777)
    {
        this._address = address;
        this._port = port;
        this.update();
    }

    public bool update()
    {
        UdpClient con = null;
        Boolean error = false;
        try
        {
            con = new UdpClient(this._address, this._port);
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse(this._address), this._port);
            Byte[] header =  new Byte[]
            {
                0xff, 0xff, 0xff, 0xff, 0x54,
                0x53, 0x6f, 0x75, 0x72, 0x63,
                0x65, 0x20, 0x45, 0x6e, 0x67, 
                0x69, 0x6e, 0x65, 0x20, 0x51,
                0x75, 0x65, 0x72, 0x79, 0x00
            };
            if (con.Send(header, header.Length) != header.Length)
                throw new Exception();
            String[] data = Encoding.UTF8.GetString(con.Receive(ref endpoint)).Substring(6).Split('\0');
            String[] pi = data[1].Substring(9).Split('/');
            this._hostname = data[0];
            this._players = UInt32.Parse(pi[0]);
            this._max_players = UInt32.Parse(pi[1]);
        }
        catch (Exception)
        {
            error = true;
        }
        if (con != null)
            con.Close();
        return (this._error = error);
    }

    public Boolean isErroneous()
    {
        return this._error;
    }

    public String getHostName()
    {
        return this._hostname;
    }

    public UInt32 getPlayers()
    {
        return this._players;
    }

    public UInt32 getMaxPlayers()
    {
        return this._max_players;
    }
}
