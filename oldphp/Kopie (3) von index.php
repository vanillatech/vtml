
<?php
$DBSERVER="localhost";
$DATABASENAME = "test";
$USERNAME = "root";
$PASSWORD = "fisch95";
$db=mysql_connect($DBSERVER, $USERNAME, $PASSWORD) or die ("Couldnt connect")	;
mysql_select_db("test",$db);

$thinkmax = 5;
//Schwellenwert für Erkennung von Wörtern/Sätzen.
$recbias = 0.8; 


error_reporting (E_ALL);

if (isset($inp)) { if ($inp!="") {

$remember = "";
if ($rem=="") {
  $query1 = "SELECT max(relid) as relid from cortex2";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) > 0) {
    $erg = mysql_fetch_array($result,MYSQL_ASSOC);
    $remember = $erg["relid"]+1;
  }
} else { 
  $remember = $rem;
}

// Sätze expandieren
$sentences = split ('[.!?]',$inp);

//Jeden Satz einzeln verarbeiten, Satz als Einheit sehen
for ($s=0;$s<count($sentences) && $sentences[$s] != "";$s++) {
$wordskharma=0;
$sentences[$s] = trim($sentences[$s]);
// Wörter expandieren
$words = explode(" ",$sentences[$s]);

//Jedes Wort einzeln verarbeiten
$wordsid = Array();
for ($n=0;$n<count($words);$n++) {

  $charsid[]= Array();
  
  //Zeichen expandieren und damit die Worterkennung durchführen 
  $aword = $words[$n];
   
  for ($l=0;$l<strlen($words[$n]);$l++) {
	$query1 = "SELECT * FROM cortex0_0 WHERE data like '$aword[$l]'";
	$result = mysql_query($query1,$db);
	if (mysql_num_rows($result) == 0) {
		$query2 = "INSERT INTO cortex0_0(data) values ('$aword[$l]')";
		$result = mysql_query($query2,$db);
		$result = mysql_query($query1,$db);
	} 
	$rel = mysql_fetch_array($result,MYSQL_ASSOC);
	$charsid[$l] = $rel["id"];
	
  }

$notuseRword = "";
$charrelid = 0;
  
  // Überprüfen ob das Wort 100% identisch mit Bekanntem ist
  $query1 = "";
  $charsid[strlen($words[$n])] = 0;
  for ($l=0;$l<strlen($words[$n]);$l++) {
    if ($l!=0) $query1 .= " AND relid IN ( ";
    $query1 .= "SELECT relid FROM cortex0 WHERE dest=" . $charsid[$l] . " AND suc=" . $charsid[$l + 1];
    if ($l!=0) $query1 .= ")";
  }
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) > 0) {
      $erg = mysql_fetch_array($result,MYSQL_ASSOC);
      $charrelid = $erg["relid"];
  } else {
    // sich an das gesagte Wort erinnern
    $query1 = "SELECT max(relid) as relid from cortex0";
    $result = mysql_query($query1,$db);
    if (mysql_num_rows($result) != 0) {
      $erg = mysql_fetch_array($result,MYSQL_ASSOC);
      $charrelid = $erg["relid"];
    }
    $charrelid++;
    for ($l=0;$l<strlen($words[$n]);$l++) {
      $query1 = "INSERT INTO cortex0(relid,dest,suc) values ($charrelid,'" . $charsid[$l] . "','" . $charsid[$l + 1] . "')";
      $result = mysql_query($query1,$db);
    }
    $notuseRword = "relid <> " . $charrelid . " AND";  
  
  
  //Wort finden, das dem Gesagten am ähnlichsten ist, bzw. mit diesem übereinstimmt
  $query1 = "SELECT relid, COUNT(relid) AS hits, COUNT(relid) / " . strlen($words[$n]) . " AS ord FROM cortex0 WHERE $notuseRword (";
  for ($l=0;$l<strlen($words[$n]);$l++) {
    if ($l != 0) $query1 .= " OR ";
    $query1 .= "dest=$charsid[$l]";
  }
  $quote = strlen($words[$n]) * $recbias;
  $query1 .= ") GROUP BY relid HAVING hits > $quote ORDER BY ord DESC";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) > 0) {
    $erg = mysql_fetch_array($result,MYSQL_ASSOC);
    /*$query2 = "SELECT *, COUNT(suc) AS cdest FROM cortex0 WHERE dest=" . $erg["relid"] . " AND suc IN (SELECT dest from cortex0 WHERE pos >= 0 ORDER BY pos desc) Group By suc ORDER BY cdest DESC";
    $result2 = mysql_query($query2,$db);
    if (mysql_num_rows($result2) > 0) {
      $rels = mysql_fetch_array($result2,MYSQL_ASSOC);
      $charrelid = $rels["relid"];
    }*/
    $charrelid = $erg["relid"];
  } 
  }
  $foundcharrel = $charrelid;
  //Das Wort neu zuweisen und die ID speichern.
  $words[$n] = "";
  $wordsid[$n] = $foundcharrel;

  $query1 = "SELECT * from cortex0 WHERE relid = $foundcharrel ORDER BY id";
  $result = mysql_query($query1,$db);
  for ($g=0;$g<mysql_num_rows($result);$g++) {
    $rel = mysql_fetch_array($result,MYSQL_ASSOC);
    $query1 = "SELECT * from cortex0_0 WHERE id = " . $rel["dest"];
    $result2 = mysql_query($query1,$db);
    $outp = mysql_fetch_array($result2,MYSQL_ASSOC);
    $words[$n] .= $outp["data"];
  }
}



$task=0;
$layer=0;
$kharma=0;
$foundrels = array();


$notuserecent = "";
$relid = 0;

$wkharma = array();

// Überprüfen ob das Gesagte 100% identisch mit Bekanntem ist
$wordsid[count($words)]=0;
$query1 = "";
for ($n=0;$n<count($words);$n++) {
  //Kharma der Wörter rausfinden	
  $query2 = "SELECT AVG(pos) AS posavg FROM relations WHERE dest=$wordsid[$n]";
  $result2 = mysql_query($query2,$db);
  if (mysql_num_rows($result2) > 0) {
    $erg2 = mysql_fetch_array($result2,MYSQL_ASSOC);
    $wkharma[$n] = $erg2["posavg"];
    $wordskharma += $wkharma[$n];
  } else {
    $wkharma[$n] = 0;
  }
  //
  if ($n!=0) $query1 .= " AND relid IN ( ";
  $query1 .= "SELECT relid FROM relations WHERE dest=" . $wordsid[$n] . " AND suc=" . $wordsid[$n+1];
  if ($n!=0) $query1 .= ")";
}
$result = mysql_query($query1,$db);
if (mysql_num_rows($result) != 0) {
      $erg = mysql_fetch_array($result,MYSQL_ASSOC);
      $relid = $erg["relid"];
} else {
  // sich an das Gesagte/Gefragte erinnern
  $query1 = "SELECT max(relid) as relid from relations";
  $result = mysql_query($query1,$db);
  if (mysql_num_rows($result) != 0) {
      $erg = mysql_fetch_array($result,MYSQL_ASSOC);
      $relid = $erg["relid"];
  } 
  $relid++;
  for ($n=0;$n<count($words);$n++) {
    $query1 = "INSERT INTO relations(relid,dest,suc,pos) values ($relid,'" . $wordsid[$n] . "','" . $wordsid[$n+1] . "','$wkharma[$n]')";
    $result = mysql_query($query1,$db);
  }
  $notuserecent = "relid <> " . $relid . " AND";
}


//Den Zusammenhang zwischen Sätzen merken:

if (isset($lastrelid)) {
  $query1 = "INSERT INTO cortex2 (relid,dest,suc,pos) values ($remember,$lastrelid,$relid,$wordskharma)";
  $result = mysql_query($query1,$db);
  $lastrelid = $relid;
}
    


//get kharma  think about
$query1 = "SELECT relid, COUNT(relid) AS hits, COUNT(relid) / " . count($words) . " AS ord FROM relations WHERE $notuserecent (";
for ($n=0;$n<count($words);$n++) {
/*for ($m=0;$m<count($words);$m++) {
       if ($n != $m) {
              $thought = think($wordsid[$n],$wordsid[$m]);
       }
}*/
  if ($n != 0) $query1 .= "OR ";
  $query1 .= "dest=$wordsid[$n] OR suc=$wordsid[$n] ";
}
$quote =  count($words) * $recbias;
$query1 .= ") GROUP BY relid HAVING hits > $quote ORDER BY ord DESC";
$result = mysql_query($query1,$db);
if (mysql_num_rows($result) > 0) {
  $erg = mysql_fetch_array($result,MYSQL_ASSOC);
  $query2 = "SELECT *, COUNT(suc) AS cdest FROM cortex2 WHERE dest=" . $erg["relid"] . " AND suc IN (SELECT dest from cortex2 WHERE pos >= 0 ORDER BY pos desc) Group By suc ORDER BY cdest DESC";
  $result2 = mysql_query($query2,$db);
  if (mysql_num_rows($result2) > 0) {
    $rels = mysql_fetch_array($result2,MYSQL_ASSOC);
    $foundrels[0] = $rels["suc"];
  } else {
    //$foundrels[0] = $erg["relid"];
  }
}

//das Gedachte ausgeben

$output1 = "";

for ($z=0;$z<count($foundrels);$z++) {
  $query1 = "SELECT * from relations WHERE relid = $foundrels[$z] ORDER BY id";
  $result = mysql_query($query1,$db);
  for ($g=0;$g<mysql_num_rows($result);$g++) {
    $rel = mysql_fetch_array($result,MYSQL_ASSOC);
    $query2 = "SELECT * from cortex0 WHERE relid = " . $rel["dest"] . " ORDER BY id";
    $result2 = mysql_query($query2,$db);
    if ($g != 0) $output1 .= " ";
    for ($h=0;$h<mysql_num_rows($result2);$h++) {
      $rel2 = mysql_fetch_array($result2,MYSQL_ASSOC);
      $query3 = "SELECT * from cortex0_0 WHERE id = " . $rel2["dest"];
      $result3 = mysql_query($query3,$db);
      $outp = mysql_fetch_array($result3,MYSQL_ASSOC);
      $output1 .= $outp["data"];
    }
  }
}

echo $output1 . ". ";



//Den Zusammenhang zwischen dem Gehörten und dem Gesagten Satz merken:

if (count($foundrels) > 0) {
  $query1 = "INSERT INTO cortex2 (relid,dest,suc) values ($remember,$lastrelid,$foundrels[0])";
  $result = mysql_query($query1,$db);
  $relid = $foundrels[0]; 
  $lastrelid = $relid;
}

//Schleife Sentences zu ende:
}
//


}} else { //wenn nichts eigegeben
  $remember = "";
  $relid = "";
}

?>
<form name=mouth action="index.php" method="get">

<input type=text name=inp>
<input type=hidden name=rem value="<?php echo $remember; ?>">
<input type=hidden name=lastrelid value="<?php echo $relid; ?>">
<input type=submit value=OK>

</form>
