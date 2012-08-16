<?php
//change index7.php: Aktivierungsbias auch bei Thinkdown berücksichtigt.
//change index8.php: recoveryQueue eingeführt
//change index9.php: Änderungen an Aktivierungsreihenfolgen + truncatedb funktion
//change index9a.php: Activationdelay --
//change index10.php: Aktivierungsschwelle: 50%, inhibitorische Synapsen
//change index10a.php: Activationdelay ++
//change index11.php: forget
//change index11a.php: Activationdelay --, minusActivation Variable
//change index11b.php: activationDelay ++
//change index12.php: Korrektur in Neuron einfügen: Delay wurde nicht korrekt berechnet.
//change index12a.php: Test: Aktivierungsschwelle = 0,9, Forgetrate = 0
//change index12b.php: thinkdown löscht nicht mehr neuronid, sondern id aus recoveryQueue
//change index13.php: newNeuron Funktion ausgelagert, bei Denkpausen Aufruf der newNeuron Funktion.
//change index13a.php: neue WTA Funktion (löscht nichts mehr aus der ActivationQueue)
//change index14.php: Mustererkennung in RecoveryQueue (nur rauslöschen, wenn 1:1 gefunden)
//change index14a.php: Aktivierungsschwelle wieder 0,5, Forgetrate = 0,9 - Eingabemöglichkeit im Formular hierfür
//change index14b.php: Änderung WTA Funktion (13a rückgängig machen)
//change index14c.php: Debug "inhibit" enthält falsches Neuron - gefixed
//change index14d.php: Einführung eines Schichtenmodells ("columnar arrangements") bei neuem Neuron
//change index14e.php: newneuron - schleife nach einfügen von neuem neuron
//change index14f.php: newneuron - fireorder korrigiert
//change index14g.php: newneuron - neu Gelerntes nicht gleich in Recoveryqueue
error_reporting (E_ALL);


// 1. Variablen
// 1.1 Rate mit der die Aktivierung der Neuronen zurückgeht
$forgetrate = 0.9;
if (isset($fgr)) $forgetrate = $fgr;
// 1.2 Refraktärzeit der Neuronen
$recTime = 9;
// 1.3 Aktivierungsschwelle
$actBias = 0.5;
if (isset($abs)) $actBias = $abs;
// Activationdelay bei neuem Neuron
$minusActivation = 0;


$DBSERVER="localhost";
$DATABASENAME = "cortex";
$USERNAME = "root";
$PASSWORD = "fisch95";
$db=mysql_connect($DBSERVER, $USERNAME, $PASSWORD) or die ("Couldnt connect")	;
mysql_select_db("cortex",$db);

if (isset($truncatedb)) if ($truncatedb=="yes") {
  $query1="TRUNCATE activationQueue";
  $result = mysql_query($query1,$db);
  $query1="TRUNCATE axons;";
  $result = mysql_query($query1,$db);
  $query1="TRUNCATE neurons;";
  $result = mysql_query($query1,$db);
  $query1="TRUNCATE recoveryQueue;";
  $result = mysql_query($query1,$db);
};

if (isset($forget)) if ($forget=="yes") {
  $query1="UPDATE axons set strength = strength - 1";
  $result = mysql_query($query1,$db);
  $delayval = 1 - $minusActivation;
  $query1 = "SELECT suc FROM axons WHERE strength = 0 AND delay=$delayval";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) > 0) for ($c=0;$c<mysql_num_rows($result);$c++) {
    $res1 = mysql_fetch_array($result,MYSQL_ASSOC);
    $query1="UPDATE axons set delay = delay - 1 WHERE suc =" . $res1["suc"];
    $r = mysql_query($query1,$db);
  };
  $query1="DELETE FROM axons WHERE strength = 0";
  $result = mysql_query($query1,$db);
};

function newNeuron () {
    global $db;
    global $minusActivation;
    $again = 1;
    // es kann sein, daß durch das neu eingefügte neuron sich wieder 2 Neurone auf einer Ebene befinden
    while ($again == 1) { 
    $again = 0;
    //Nur die Neuronen in r.Q. finden, bei denen es min. 2 auf der gleichen Schicht gibt.
    $query1 = "SELECT * from recoveryQueue WHERE layer IN (SELECT layer from recoveryQueue GROUP BY layer HAVING count(layer) > 1) order by layer, fireorder";
    $resultRec = mysql_query($query1,$db);
    $numlastfired = mysql_num_rows($resultRec);
    if ( $numlastfired > 1 ) {
	$clayer=0;
	$sumDelay = 0;
	$fireord = 0;
	$adj = 0;
	for ($y = 0; $y < $numlastfired;$y++) {
		$erg = mysql_fetch_array($resultRec,MYSQL_ASSOC);
		if ($clayer != $erg["layer"]) {
			$clayer = $erg["layer"];
			// neues Neuron ist eine Schicht höher ("Columnar Arrangements")
			$cnewlayer = $clayer + 1;
			$query1 = "INSERT INTO neurons (lastfired,recoveryTime,layer) values (0,0,$cnewlayer)";
			$result = mysql_query($query1,$db);
			$newneuron = mysql_insert_id();	
			//deaktiviert: index14g.php
			//$query1 = "INSERT INTO recoveryQueue (neuronid,fireorder,layer) values ($newneuron,0,$cnewlayer)";
			//$result = mysql_query($query1,$db);
			$again = 1;
			echo "<b><font color=#00FFFF>Learn: </font></b> NewNeuron: $newneuron Consists of: ";
			$sumDelay = 0;
			$fireord = 0;
			$adj = $erg["fireorder"] - 1;
        	}
	//$query1 = "INSERT INTO axons (dest,suc) values ($lastspikeid," . $relx["id"] . ")";	  
        //$result = mysql_query($query1,$db);
		
		//if ($y == 0) $fireord = $erg["fireorder"];
	        $sumDelay += $erg["fireorder"] - $fireord - $adj;
		$fireord = $erg["fireorder"] - $adj;	
		//$activationDelay = $numlastfired - $y;
		$activationDelay = $sumDelay - $minusActivation;
		$query1 = "INSERT INTO axons (dest,suc,delay) values (".$erg["neuronid"]."," . $newneuron . "," . $activationDelay . ")";	  
		$result = mysql_query($query1,$db);
		echo $erg["neuronid"] . " ";
		/*//Wenn das gefeuerte Neuron in der recoveryQueue ein Neuron ist, das eine Gruppe anderer Neurone zusammenfasst, 
		//muß man die delay entsprechend anpassen
		$query1 = "SELECT MAX(delay) as delay FROM axons WHERE suc IN (SELECT neuronid FROM recoveryQueue WHERE fireorder = ".$erg["fireorder"].")";
		$res2 = mysql_query($query1,$db);
		if (mysql_num_rows($res2) > 0 && $erg["fireorder"] != $fireord) {
		  $r3 = mysql_fetch_array($res2,MYSQL_ASSOC);
		  if ($r3["delay"] > 0) $sumDelay += $r3["delay"];
		  else $sumDelay++;
		} else $sumDelay++;*/
		
		//das neu erkannte element aus der recoveryQueueu kann nun entfernt werden:
		$query1 = "DELETE FROM recoveryQueue WHERE id = ".$erg["id"];
		$result = mysql_query($query1,$db);
        }/* else if ($lastspikeid > 0 && $newneuron > 0 ) {
        $query1 = "INSERT INTO axons (dest,suc) values ($lastspikeid," . $newneuron . ")";	  
	$result = mysql_query($query1,$db);
	}*/
	echo "<br>";
    }
    //-- ende neues neuron einfügen
    }
}

function deleRecQueue(&$fNeuron,$pos,$forderOld) {
    global $db;
    $fQuery = "";
    if ((count($fNeuron)) == $pos) return(1);
    //$res = mysql_fetch_array($result,MYSQL_ASSOC);
    //$posneu = $res["delay"] - $pos;
    if ($pos > 0) {
      $delay = $fNeuron[$pos-1]["delay"] - $fNeuron[$pos]["delay"];
    }
    if ($forderOld > 0) {

      $forderNew = $forderOld - $delay;
      $fQuery = " AND fireorder = " . $forderNew;
    } 
    $query1 = "SELECT * FROM recoveryQueue WHERE neuronid = " . $fNeuron[$pos]["dest"] . $fQuery . " ORDER BY fireorder DESC";
    $result2 = mysql_query($query1,$db);
    for ($y=0;$y<mysql_num_rows($result2);$y++) {
      $res2 = mysql_fetch_array($result2,MYSQL_ASSOC);
      $retVal = deleRecQueue($fNeuron,$pos+1,$res2["fireorder"]);
      if ($retVal == 1) {
        $query1 = "DELETE FROM recoveryQueue WHERE id = " . $res2["id"];
	$result = mysql_query($query1,$db);
	echo "<b><font color=#FFFF00>Delete From RecQueue: </font></b> NeuronID: " . $fNeuron[$pos]["dest"] . " Successor: " . $fNeuron[$pos]["suc"] . "<br>";
	return(1);
      } else { return(0); } 
    } 
    if (mysql_num_rows($result2) == 0) return(0);
}

if (isset($inp)) { if ($inp!="") {

$charsid[]= Array();
$gedacht = "";
$lastspikeid = 0;

//**********************************
//Teil 1: Zeichen expandieren und damit die Worterkennung durchführen 
//**********************************

$chars = stripslashes($inp);

$newneuron = 0;  
$newneuroncounter = 0;


//eintreffende Reize (Inputbox) aktivieren Neuronen	
for ($l=0;$l<strlen($chars);$l++) { // es gibt noch zeichen aus dem input	
  $charord = ord($chars[$l]);
  $query1 = "SELECT * FROM neurons WHERE data = $charord";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) == 0) {
	$query2 = "INSERT INTO neurons(data) values ($charord)";
	$result = mysql_query($query2,$db);
	$result = mysql_query($query1,$db);
  } 
  $rel = mysql_fetch_array($result,MYSQL_ASSOC);
  $charsid[$l] = $rel["id"];
  /*/2.1 gefeuerte Neuronen entladen und Refraktärzeit einstellen
  $query1 = "UPDATE neurons set active = 0, recoveryTime = ($recTime + 1) WHERE id = $charsid[$l]";
  $result = mysql_query($query1,$db);*/




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
  $delayReiz = $l+1;

  //3.2 nun durch die äußeren Reize (gehörte Laute/Buchstaben) Neuronen aktivieren.
  $query1 = "INSERT INTO activationQueue (neuronid, activationVal, activationDelay) VALUES (" . $charsid[$l] . ",1,$delayReiz)";
  $result = mysql_query($query1,$db);
} //keine zeichen mehr 


//**********************************
//SCHLEIFE: ActivationQueue (Neuronen aktivieren) - Neuronen feuern
//**********************************

$stepbystep = 1;
for ($l=0;$stepbystep==1;$l++) {

 // Erstmal wieder "abregen":
  $query1 = "UPDATE neurons SET active = (active * $forgetrate) WHERE active <> 0";
  $result = mysql_query($query1,$db);
  $query1 = "UPDATE neurons SET recoveryTime = (recoveryTime - 1) WHERE recoveryTime > 0";
  $result = mysql_query($query1,$db);
  $query1 = "UPDATE neurons SET lastfired = (lastfired - 1) WHERE lastfired > 0";
  $result = mysql_query($query1,$db);


// ActivationQueue - Schritt nach vorne

echo "-----------<br>";
$query1 = "UPDATE activationQueue set activationDelay = (activationDelay - 1) WHERE activationDelay > 0";
$resultx = mysql_query($query1,$db);
$activationQueue = 0;
if (mysql_affected_rows($db) > 0) $activationQueue = 1;
else {
  $query1 = "SELECT * FROM neurons WHERE active >= $actBias ORDER BY active DESC";
  $resultx = mysql_query($query1,$db);
  if(mysql_num_rows($resultx)==0) $stepbystep = 0;
}

//**********************************
//Teil 2: ActivationQueue bearbeiten
//**********************************

while ($activationQueue == 1) {


  
  $query1 = "SELECT * FROM activationQueue WHERE activationDelay = 0";
  $resultx = mysql_query($query1,$db);
  if(mysql_num_rows($resultx)>0) {
    for ($g = 0;$g < mysql_num_rows($resultx);$g++) {
      $erg = mysql_fetch_array($resultx,MYSQL_ASSOC);
      $query1 = "UPDATE neurons set active = (active + " . $erg["activationVal"] . ") WHERE id=". $erg["neuronid"];
      $result = mysql_query($query1,$db);
      echo "<b><font color=#00FF00>Activate: </font></b>" . $query1 . "<br>";
    }
    $query1 = "DELETE FROM activationQueue WHERE activationDelay = 0";
    $result = mysql_query($query1,$db);
  } else $activationQueue = 0;
} // ende while activationQueue == 1


$query1 = "SELECT * FROM recoveryQueue WHERE fireorder = 0";
$result = mysql_query($query1,$db);
if (mysql_num_rows($result) > 0) {
  $query1 = "UPDATE recoveryQueue set fireorder = fireorder + 1";
  $result = mysql_query($query1,$db);
} else {newNeuron();}

//**********************************
//Teil 3: aktive Neuronen feuern
//**********************************

$activeneurons = 1;
while ($activeneurons == 1) {
  // 4.1 aktivierte Neuronen suchen
  $query1 = "SELECT * FROM neurons WHERE active >= $actBias ORDER BY active DESC";
  $resultx = mysql_query($query1,$db);
  if(mysql_num_rows($resultx)>0) {  
    $relx = mysql_fetch_array($resultx,MYSQL_ASSOC);
    //4.1.1 aktivierte Neuronen feuern, d.h. entladen
    $recT2 = $recTime;
    //$query1 = "UPDATE axons set strength = (strength + 1) WHERE dest = " . $relx["id"] . " AND suc IN (SELECT id FROM neurons WHERE lastfired > 0)";
    //$result = mysql_query($query1,$db);
    //if (mysql_affected_rows($db) > 0) {
    //  // es gibt Neuronen, die eben gefeuert wurden und dieses neuron als nachfolge rhaben	  
    //  $recT2 = 0;
    //}
    $query1 = "UPDATE neurons set active = 0 WHERE id = ". $relx["id"];
    echo "<b><font color=#FF0000>Fire: </font></b>".$query1 . "<br>";
    $result = mysql_query($query1,$db);
    $query1 = "INSERT INTO recoveryQueue(neuronID,fireorder,layer) VALUES('". $relx["id"] ."',0,". $relx["layer"] .")";
    $result = mysql_query($query1,$db);
    $gedacht = $gedacht . chr($relx["data"]);
    
    //4.1.2 Axon zum gefeuerten Neuron hin stärken (Hebbsche Lernregel)
    $query1 = "UPDATE axons set strength = (strength + 1) WHERE suc = " . $relx["id"] . " AND dest IN (SELECT neuronid FROM recoveryQueue)";
    //$query1 = "UPDATE axons set strength = (strength + 1) WHERE suc = " . $relx["id"] . " AND dest = $lastspikeid";
    $result = mysql_query($query1,$db);
    
    //andere Neuronen entladen (Winner takes all)
    $query1 = "UPDATE neurons set active = 0 WHERE id IN ( SELECT suc FROM axons WHERE dest IN (SELECT neuronid FROM recoveryQueue WHERE neuronid IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . "))) AND id <> ". $relx["id"];
    $result = mysql_query($query1,$db);
    
    $query1 = "SELECT * FROM neurons WHERE id IN ( SELECT suc FROM axons WHERE dest IN (SELECT neuronid FROM recoveryQueue WHERE neuronid IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . "))) AND id <>" . $relx["id"];
    $result = mysql_query($query1,$db);
    if (mysql_num_rows($result) > 0) {echo "<b>Inhibit: </b>["; while ($row=mysql_fetch_array($result)) { echo $row["id"]."; "; } echo "]<br>"; };
    
    
    $query1 = "SELECT * FROM activationQueue WHERE neuronid IN ( SELECT suc FROM axons WHERE dest IN (SELECT neuronid FROM recoveryQueue WHERE neuronid IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . "))) AND sender IN (SELECT neuronid FROM recoveryQueue WHERE neuronid IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . ")) ";
    $result = mysql_query($query1,$db);

    if (mysql_num_rows($result) > 0) {echo "<b>Inhibit: </b>["; while ($row=mysql_fetch_array($result)) { echo $row["neuronid"]."; "; } echo "]<br>"; };
    $query1 = "DELETE FROM activationQueue WHERE neuronid IN ( SELECT suc FROM axons WHERE dest IN (SELECT neuronid FROM recoveryQueue WHERE neuronid IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . "))) AND sender IN (SELECT neuronid FROM recoveryQueue WHERE neuronid IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . "))";
    $result = mysql_query($query1,$db);
    
    /*$query1 = "SELECT suc FROM axons WHERE dest IN (SELECT neuronid FROM recoveryQueue WHERE neuronid IN (SELECT dest FROM axons WHERE suc = " . $relx["id"] . ")) AND suc <> ". $relx["id"];
    $result = mysql_query($query1,$db);
    echo "<b>Inhibit: </b>["; while ($row=mysql_fetch_array($result)) { echo $row["suc"]."; "; } echo "]<br>";*/


	//Aus der RecoveryQueue genau die Neuronen löschen, die dem GENAUEN Muster des gerade gefeuerten Neurons entsprechen.
    $query1 = "SELECT * FROM axons WHERE suc = " . $relx["id"] . " ORDER BY delay DESC";
    $result = mysql_query($query1,$db);
    $fNeuron[] = Array();
    for ($y=0;$y<mysql_num_rows($result);$y++) {
	    
	    $fNeuron[$y] = mysql_fetch_array($result,MYSQL_ASSOC);
    }
    if (mysql_num_rows($result) > 0) {
	    $a = deleRecQueue($fNeuron,0,0);
    }	   
    unset($fNeuron); 
    //4.2 nun Neuronen und ihre Umgebung reizen.
    $query1 = "SELECT suc, delay, strength as synapsen FROM axons WHERE dest=" . $relx["id"];
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
  	 $query1 = "INSERT INTO activationQueue (neuronid, activationVal, activationDelay,sender) VALUES (" . $erg["suc"] . ", " .  $synfaktor . "," . $erg["delay"] . "," . $relx["id"] . ")";
	 //$query1 = "UPDATE neurons set active = (active + (" . mysql_num_rows($result2) * $synfaktor . ")) WHERE id=". $erg["suc"] ." AND recoveryTime = 0";
         echo "<b><font color=#0000FF>Plan Activation: </font></b>" . $query1 . "<br>";
	 $activationQueue = 1;
	 $result2 = mysql_query($query1,$db);
        }
      }
    }
    //4.3 Axon vom gefeuerten zum "neuen" Neuron hinzufügen (versuch)
    /*$query1 = "INSERT INTO axons (dest,suc) values (" . $relx["id"] . ",$newneuron)";
    $result = mysql_query($query1,$db);*/
    $lastspikeid = $relx["id"];
  } else { $activeneurons = 0; }
	
} //end while ActiveNeurons


} //ende for-schleife


//**********************************
//Teil 4: neues Neuron anlegen
//**********************************


    newNeuron();
  
  
//**********************************
//Teil 5: Hierarchie runter denken
//**********************************

    //$query1 = "UPDATE neurons set recoveryTime = ($recTime + 1) WHERE id = $newneuron";
  //$result = mysql_query($query1,$db);

    // Ausgabe Vorbereiten
$gedacht = "";

function thinkDown ($neuronID) {
  global $db;
  global $gedacht;
  global $actBias;
  $query1 = "SELECT data from neurons where id = $neuronID";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result)>0) {
    $erg = mysql_fetch_array($result,MYSQL_ASSOC);	
    $gedacht .= chr($erg["data"]);
  }    
  $query1 = "SELECT max(strength) as strongest from axons where suc = $neuronID";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result)>0) {
	  $erg = mysql_fetch_array($result,MYSQL_ASSOC);
	  $strongest = $erg["strongest"];
  	  $query1 = "SELECT * from axons where suc = $neuronID ORDER BY delay DESC";	
 	  $result = mysql_query($query1,$db);
	  if (mysql_errno())
	    echo mysql_error();
	  for ($n=0;$n<mysql_num_rows($result);$n++) {
	    $erg = mysql_fetch_array($result,MYSQL_ASSOC);
	    if ($erg["strength"] >= $strongest * $actBias) {
		    thinkDown ($erg["dest"]);
	    }
	  }
  }
}
$neuronsleft = 1;    
//while ($neuronsleft > 0) {
	$query1 = "SELECT * from recoveryQueue order by fireorder DESC";
	$result = mysql_query($query1,$db);
	if (mysql_num_rows($result)>0) {
	  $erg = mysql_fetch_array($result,MYSQL_ASSOC);	
	  thinkDown($erg["neuronid"]);
	  $query1 = "DELETE from recoveryQueue WHERE id = " . $erg["id"];
	  $result = mysql_query($query1,$db);
	} else $neuronsleft = 0;
//}    

  
echo $gedacht;

$query1 = "UPDATE neurons set active = 0, recoveryTime = 0, lastfired = 0";
$result = mysql_query($query1,$db);

}}
$file = $_SERVER['PHP_SELF'];  
$fname = substr($file, strrpos($file, '/') + 1, strlen($file)); 

?>
<form name=mouth action="<?php echo $fname; ?>" method="get">

Truncate DB: <input type=checkbox name=truncatedb value="yes"><br>
Forget: <input type=checkbox name=forget value="yes"><br>
<input type=text name=inp >
<input type=submit value=OK><br>
Forgetrate: <input type=text name=fgr value="<?php echo $forgetrate;?>"><br>
Activation Bias: <input type=text name=abs value="<?php echo $actBias;?>"><br>
<script language="Javascript"> 
document.mouth.inp.focus();
</script>
</form>
