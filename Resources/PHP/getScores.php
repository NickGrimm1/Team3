<?
$con = mysql_connect("fdb4.runhosting.com","1823820_scores","94857GLOW423895");
if (!$con)
{
die('Could not connect: ' . mysql_error());
}
//echo 'Successfully connected to database!';
mysql_select_db("1823820_scores", $con);
$result = mysql_query("SELECT * FROM Scores ORDER BY Score DESC LIMIT 10");
while($row = mysql_fetch_array($result))
{
echo $row['Name'] . "|" . $row['Score'];
echo ";";
}
mysql_close($con);
?>