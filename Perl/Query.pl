=begin legal
        	Perl JC2-MP server query function by King_Hual, Djole1337
        

        Arguments:

        string address - The IP address
        int port - The port

        Methods:

        update() - Updates the information
        isErroneous() - Returns true if any errors occured, false otherwise
        getHostName() - Returns the hostname of a server
        getPlayers() - Returns the amount of players currently connected to the server
        getMaxPlayers() - Returns the maximum amount of players that can be connected to the server

*/
=cut

package JCMPQuery;

use strict;
use IO::Socket;

sub new
{
	my $class = shift;
	my $self =
	{
		address	=> shift,
		port 	=> shift,
		hostname => "",
		players => -1,
		max_players => -1,
		error => 1
	};
	bless $self, $class;
	$self->update();
	return $self;
}

sub update
{
	my($self) = @_;
	my $sock = IO::Socket::INET->new(Proto => 'udp', PeerPort => $self->{port}, PeerAddr => $self->{address}, Timeout => 2) or return($self->{error} = 1);
	my $sdata = pack("C*", 
		0xff,0xff,0xff,0xff,0x54,
		0x53,0x6f,0x75,0x72,0x63,
		0x65,0x20,0x45,0x6e,0x67,
		0x69,0x6e,0x65,0x20,0x51,
		0x75,0x65,0x72,0x79,0x00);
	my $rawdata;
	$sock->send($sdata) or return($self->{error} = 1);
	$sock->recv($rawdata, 256) or return($self->{error} = 1);
	my @data = split('\x00', substr($rawdata, 6));
	my @pi = split('/', substr($data[1], 9));
	$self->{hostname} = $data[0];
	$self->{players} = $pi[0];
	$self->{max_players} = $pi[1];
	return($self->{error} = 0);
}

sub isErroneous()
{
	my($self) = @_;
	return $self->{error};
}

sub getHostName()
{
	my($self) = @_;
	return $self->{hostname};
}

sub getPlayers()
{
	my($self) = @_;
	return $self->{players};
}

sub getMaxPlayers()
{
	my($self) = @_;
	return $self->{max_players};
}
