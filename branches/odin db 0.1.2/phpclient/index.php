<html>

<form name=think action="index.php" method="post">
Input: <input type=text name=inputtext>
<input type=submit value="Think">
</form>
<br><br>
<?php
error_reporting(E_ALL);
$inputtext = isset($_POST['inputtext']) && !empty($_POST['inputtext']) ? $_POST['inputtext'] : 0;

if ($inputtext != "") {
	$service_port = 11202;

	$address = "192.168.0.35";

	$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

	if ($socket === false) {
	    echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
	} 

	$result = socket_connect($socket, $address, $service_port);
	if ($result === false) {
	    echo "socket_connect() failed.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
	} 

	$in = $inputtext . "\r\n";
	$out = '';

	socket_write($socket, $in, strlen($in));
	$out = socket_read($socket, 2048);
    	echo "Output: " . $out;
	

	socket_close($socket);
	
}

?>
</html>