<?php
$nam = $_GET["n"];
$sco = $_GET["s"];
//echo $nam . " got score " . $sco;
$con = mysql_connect("fdb4.runhosting.com","1823820_scores","94857GLOW423895");
if (!$con) {
	die('Could not connect: ' . mysql_error());
}
//echo '<br>';
//echo 'Successfully connected to database!';
mysql_select_db("1823820_scores", $con);
$result = mysql_query( "INSERT INTO Scores (Name, Score)
VALUES ( '" . $nam . "', '" . $sco . "' ) " );
mysql_close($con);
?>