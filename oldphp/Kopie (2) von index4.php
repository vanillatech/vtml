<?php
error_reporting (E_ALL);


$DBSERVER="localhost";
$DATABASENAME = "cortex";
$USERNAME = "root";
$PASSWORD = "fisch95";
$db=mysql_connect($DBSERVER, $USERNAME, $PASSWORD) or die ("Couldnt connect")	;
mysql_select_db("cortex",$db);


// 1. Variablen
// 1.1 Rate mit der die Aktivierung der Neuronen zurückgeht
$forgetrate = 0.9;
// 1.2 Refraktärzeit der Neuronen
$recTime = 9;
// 1.3 Aktivierungsschwelle
$actBias = 1;


//

if (isset($inp)) { if ($inp!="") {

$charsid[]= Array();
//1.4 neues Neuron hinzufügen für den Kontext  
/*
$query1 = "INSERT INTO neurons (data) values ('')";
$result = mysql_query($query1,$db);
$newneuron = mysql_insert_id();
*/
$gedacht = "";
$lastspikeid = 0;

//2. Zeichen expandieren und damit die Worterkennung durchführen 
$chars = stripslashes($inp);

$newneuron = 0;  
$newneuroncounter = 0;

for ($l=0;$l<strlen($chars);$l++) {
  $query1 = "SELECT * FROM neurons WHERE data = '$chars[$l]'";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) == 0) {
	$query2 = "INSERT INTO neurons(data) values ('$chars[$l]')";
	$result = mysql_query($query2,$db);
	$result = mysql_query($query1,$db);
  } 
  $rel = mysql_fetch_array($result,MYSQL_ASSOC);
  $charsid[$l] = $rel["id"];
  /*/2.1 gefeuerte Neuronen entladen und Refraktärzeit einstellen
  $query1 = "UPDATE neurons set active = 0, recoveryTime = ($recTime + 1) WHERE id = $charsid[$l]";
  $result = mysql_query($query1,$db);*/


  //3. Behandlung der Aktivität der Neuronen.
  //3.1 Erstmal wieder "abregen":
  $query1 = "UPDATE neurons SET active = (active * $forgetrate) WHERE active > 0";
  $result = mysql_query($query1,$db);
  $query1 = "UPDATE neurons SET recoveryTime = (recoveryTime - 1) WHERE recoveryTime > 0";
  $result = mysql_query($query1,$db);


/*for ($l=0;$l<strlen($chars);$l++) {  
  //3.2 nun durch die äußeren Reize (gehörte Laute/Buchstaben) Neuronen und ihre Umgebung reizen.
  $query1 = "SELECT suc, strength as synapsen FROM axons WHERE dest=" . $charsid[$l];
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) > 0) {
    for ($y=0;$y<mysql_num_rows($result);$y++) {
      $erg = mysql_fetch_array($result,MYSQL_ASSOC);
      if ($erg["suc"] != 0) {
	 $query1 = "SELECT suc, sum(strength) as synapsen FROM axons WHERE suc=". $erg["suc"];
	 $result2 = mysql_query($query1,$db);
	 $erg2 = mysql_fetch_array($result2,MYSQL_ASSOC);
	 // Synfaktor ist die Anzahl der Synapsen (akt. Verbindung) durch die gesamtzahl der synpasen am zielneuron
	 $synfaktor = $erg["synapsen"] / $erg2["synapsen"];
	 $query1 = "UPDATE neurons set active = (active + 1/(" . mysql_num_rows($result2) * $synfaktor . ")), strength = (strength + 1) WHERE dest=". $erg["suc"] ." AND recoveryTime = 0";
	 $result2 = mysql_query($query1,$db);
      }
    }
  }
}*/

  //3.2 nun durch die äußeren Reize (gehörte Laute/Buchstaben) Neuronen aktivieren.
  $query1 = "UPDATE neurons SET active = (active + 1) WHERE id =" . $charsid[$l];
  $result = mysql_query($query1,$db);

// 4. Denken

$activeneurons = 1;

while ($activeneurons == 1) {
  // 4.1 aktivierte Neuronen suchen
  $query1 = "SELECT * FROM neurons WHERE active >= $actBias ORDER BY active DESC";
  $resultx = mysql_query($query1,$db);
  if(mysql_num_rows($resultx)>0) {  
    $relx = mysql_fetch_array($resultx,MYSQL_ASSOC);
    //4.1.1 aktivierte Neuronen feuern, d.h. entladen
    $query1 = "UPDATE neurons set active = 0, recoveryTime =  $recTime WHERE id = ". $relx["id"];
    echo "<b>Fire: </b>".$query1 . "<br>";
    $result = mysql_query($query1,$db);
    $gedacht = $gedacht . $relx["data"];
    //4.1.2 Axon zum gefeuerten Neuron hin stärken (Hebbsche Lernregel)
    $query1 = "UPDATE axons set strength = (strength + 1) WHERE suc = " . $relx["id"] . " AND dest IN (SELECT id FROM neurons WHERE recoveryTime > 0)";
    //$query1 = "UPDATE axons set strength = (strength + 1) WHERE suc = " . $relx["id"] . " AND dest = $lastspikeid";
    $result = mysql_query($query1,$db);
    //4.1.3 die Refraktärzeit bei allen Neuronen löschen, die den gerade gefeuerten Nachfolger haben.
    $query1 = "UPDATE neurons set recoveryTime = 0 WHERE id IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . ")";
    $result = mysql_query($query1,$db);
    $query1 = "SELECT * from neurons where recoveryTime > 0 order by recoveryTime ASC";
    $resultRec = mysql_query($query1,$db);
    $numlastfired = mysql_num_rows($resultRec);
    //echo $query1 . " --- $lastspikeid ---- " . mysql_affected_rows($db);
    if ( $numlastfired > 8 ) {
	$query1 = "INSERT INTO neurons (data) values ('')";
	$result = mysql_query($query1,$db);
	$newneuron = mysql_insert_id();	
        for ($y = 0; $y < $numlastfired;$y++) {
	//$query1 = "INSERT INTO axons (dest,suc) values ($lastspikeid," . $relx["id"] . ")";	  
        //$result = mysql_query($query1,$db);
		$erg = mysql_fetch_array($resultRec,MYSQL_ASSOC);
		$query1 = "INSERT INTO axons (dest,suc) values (".$erg["id"]."," . $newneuron . ")";	  
		$result = mysql_query($query1,$db);
		$query1 = "UPDATE neurons set recoveryTime = 0 WHERE id = ".$erg["id"];
		$result = mysql_query($query1,$db);
        }/* else if ($lastspikeid > 0 && $newneuron > 0 ) {
        $query1 = "INSERT INTO axons (dest,suc) values ($lastspikeid," . $newneuron . ")";	  
	$result = mysql_query($query1,$db);
        }*/
    }
    //4.2 nun Neuronen und ihre Umgebung reizen.
    $query1 = "SELECT suc, strength as synapsen FROM axons WHERE dest=" . $relx["id"];
    $result = mysql_query($query1,$db);
    if (mysql_num_rows($result) > 0) {
      for ($y=0;$y<mysql_num_rows($result);$y++) {
        $erg = mysql_fetch_array($result,MYSQL_ASSOC);
        if ($erg["suc"] != 0) {
  	 $query1 = "SELECT suc, sum(strength) as synapsen FROM axons GROUP BY suc HAVING suc=". $erg["suc"];
    	 //echo $query1 . "<br>";
	 $result2 = mysql_query($query1,$db); 
  	 $erg2 = mysql_fetch_array($result2,MYSQL_ASSOC);
  	 // Synfaktor ist die Anzahl der Synapsen (akt. Verbindung) durch die gesamtzahl der synpasen am zielneuron
  	 $synfaktor = $erg["synapsen"] / $erg2["synapsen"];
  	 $query1 = "UPDATE neurons set active = (active + (" . mysql_num_rows($result2) * $synfaktor . ")) WHERE id=". $erg["suc"] ." AND recoveryTime = 0";
    echo "<b>Activate: </b>:" . $query1 . "<br>";

	 $result2 = mysql_query($query1,$db);
        }
      }
    }
    //4.3 Axon vom gefeuerten zum "neuen" Neuron hinzufügen (versuch)
    /*$query1 = "INSERT INTO axons (dest,suc) values (" . $relx["id"] . ",$newneuron)";
    $result = mysql_query($query1,$db);*/
    $lastspikeid = $relx["id"];
  } else { $activeneurons = 0; }
	
}
}
    //-- nochmal neues Neuron einfügen für Rest
    $query1 = "SELECT * from neurons where recoveryTime > 0 order by recoveryTime ASC";
    $resultRec = mysql_query($query1,$db);
    $numlastfired = mysql_num_rows($resultRec);
    if ( $numlastfired > 2 ) {
	$query1 = "INSERT INTO neurons (data) values ('')";
	$result = mysql_query($query1,$db);
	$newneuron = mysql_insert_id();	
        for ($y = 0; $y < $numlastfired;$y++) {
	//$query1 = "INSERT INTO axons (dest,suc) values ($lastspikeid," . $relx["id"] . ")";	  
        //$result = mysql_query($query1,$db);
		$erg = mysql_fetch_array($resultRec,MYSQL_ASSOC);
		$query1 = "INSERT INTO axons (dest,suc) values (".$erg["id"]."," . $newneuron . ")";	  
		$result = mysql_query($query1,$db);
		$query1 = "UPDATE neurons set recoveryTime = 0 WHERE id = ".$erg["id"];
		$result = mysql_query($query1,$db);
        }/* else if ($lastspikeid > 0 && $newneuron > 0 ) {
        $query1 = "INSERT INTO axons (dest,suc) values ($lastspikeid," . $newneuron . ")";	  
	$result = mysql_query($query1,$db);
        }*/
    }
    //-- ende neues neuron einfügen
  //$query1 = "UPDATE neurons set recoveryTime = ($recTime + 1) WHERE id = $newneuron";
  //$result = mysql_query($query1,$db);
echo $gedacht;

}}
?>
<form name=mouth action="index4.php" method="get">

<input type=text name=inp>
<input type=submit value=OK>

</form>
