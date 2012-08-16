
<?php
$DBSERVER="localhost";
$DATABASENAME = "test";
$USERNAME = "root";
$PASSWORD = "fisch95";
$db=mysql_connect($DBSERVER, $USERNAME, $PASSWORD) or die ("Couldnt connect")	;
mysql_select_db("test",$db);

$thinkmax = 5;
//Schwellenwert für Erkennung von Wörtern.
$recbias = 0.5; 
//Schwellenwert für Erkennung von Sätzen.
$sbias = 0.5;

//Debug Output in neuem Fenster
//echo "<script language=javascript>debugwindow = window.open('debug.html','Debug');</script>";
function debug ($debugoutput) {
  echo "<script langauge=javascript>debugwindow.document.write('$debugoutput<br><br>');</script>";	
}

//

$saidsomething=0;
if (!isset($sst)) $sst = 0;
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

$foundrels = array();

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
  $aword = stripslashes($words[$n]);
   
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
    $notuseRword = "a.relid <> " . $charrelid . " AND";  
  
  
  //Wort finden, das dem Gesagten am ähnlichsten ist, bzw. mit diesem übereinstimmt
  
  $query1 = "SELECT a.relid, hits, hits/gesamt AS ord FROM (SELECT *, COUNT(relid) AS hits FROM cortex0 WHERE ";
  //$query1 = "SELECT b.relid, COUNT(a.relid) AS hits, COUNT(a.relid)/COUNT(b.relid) AS ord FROM cortex0 AS b LEFT JOIN cortex0 AS a ON a.relid=b.relid AND $notuseRword (";
  //  $query1 = "SELECT relid, COUNT(relid) AS hits, COUNT(relid) / COUNT(relid IN (SELECT relid FROM cortex0)) AS ord FROM cortex0 WHERE $notuseRword (";
  for ($l=0;$l<strlen($words[$n]);$l++) {
    if ($l != 0) $query1 .= " AND suc=$charsid[$l] OR ";
    $query1 .= "dest=$charsid[$l]";
  }
  $quote = strlen($words[$n]) * $recbias;
  $query1 .= " AND suc=0 GROUP BY relid) AS a INNER JOIN (SELECT relid, COUNT(relid) AS gesamt FROM cortex0 GROUP BY relid) AS b ON a.relid=b.relid group by a.relid HAVING $notuseRword hits > $quote AND $recbias < ord ORDER BY ord DESC, hits DESC";
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

$notuserecent = "";
$relid = 0;

$wkharma = array();

// Überprüfen ob das Gesagte 100% identisch mit Bekanntem ist
$wordsid[count($words)]=0;
$query1 = "";
for ($n=0;$n<count($words);$n++) {
  //Kharma der Wörter rausfinden, falls selber etwas geäußert wurde.
  if ($sst==1) {
  $query2 = "SELECT AVG(pos) AS posavg FROM relations WHERE dest=$wordsid[$n] AND pos <> 0";
  $result2 = mysql_query($query2,$db);
  if (mysql_num_rows($result2) > 0) {
    $erg2 = mysql_fetch_array($result2,MYSQL_ASSOC);
    $wkharma[$n] = $erg2["posavg"];
    $wordskharma += $wkharma[$n];
  } else {
    $wkharma[$n] = 0;
  }
  }
  //
  if ($n!=0) $query1 .= " AND relid IN ( ";
  $query1 .= "SELECT relid FROM relations WHERE dest=" . $wordsid[$n] . " AND suc=" . $wordsid[$n+1];
  if ($n!=0) $query1 .= ")";
}
if (count($words)>0) $wordskharma = $wordskharma/count($words);
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
    $query1 = "INSERT INTO relations(relid,dest,suc,pos) values ($relid,'" . $wordsid[$n] . "','" . $wordsid[$n+1] . "','$wordskharma')";
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
  $query1 = "SELECT a.relid, hits, hits/gesamt AS ord FROM (SELECT *, COUNT(relid) AS hits FROM relations WHERE ";
//$query1 = "SELECT b.relid, COUNT(a.relid) AS hits, COUNT(a.relid)/COUNT(b.relid) AS ord FROM relations AS b LEFT JOIN relations AS a ON a.relid=b.relid AND $notuserecent (";
//$query1 = "SELECT relid, COUNT(relid) AS hits, COUNT(relid) / " . count($words) . " AS ord FROM relations WHERE $notuserecent (";
for ($n=0;$n<count($words);$n++) {
/*for ($m=0;$m<count($words);$m++) {
       if ($n != $m) {
              $thought = think($wordsid[$n],$wordsid[$m]);
       }
}*/
  if ($n != 0) $query1 .= "OR ";
  $query1 .= "dest=$wordsid[$n] "; //OR suc=$wordsid[$n] ";
}
$quote =  count($words) * $sbias;
  $query1 .= " GROUP BY relid) AS a INNER JOIN (SELECT relid, COUNT(relid) AS gesamt FROM relations GROUP BY relid) AS b ON a.relid=b.relid group by a.relid HAVING $notuserecent hits > $quote AND $sbias < ord ORDER BY ord DESC, hits DESC";
//$query1 .= ") GROUP BY relid HAVING hits > $quote AND COUNT(a.relid)>0 ORDER BY ord DESC";
$result = mysql_query($query1,$db);
$foundrels[0] = 0;
for ($md=0;$md < mysql_num_rows($result) && $foundrels[0] == 0;$md++) {
  $erg = mysql_fetch_array($result,MYSQL_ASSOC);
  //Alle Reaktionen wählen.
  //$query2 = "SELECT *, COUNT(suc) AS cdest FROM cortex2 WHERE dest=" . $erg["relid"] . " AND suc IN (SELECT dest from cortex2 WHERE pos >= 0 ORDER BY pos desc) Group By suc ORDER BY cdest DESC";
  $query2 = "SELECT *, COUNT(suc) AS cdest FROM cortex2 WHERE dest=" . $erg["relid"] . " Group By suc ORDER BY cdest DESC";
  //debug($query2);
  $result2 = mysql_query($query2,$db);
  for ($mc=0;$mc < mysql_num_rows($result2) && $foundrels[0] == 0;$mc++) {
    $rels = mysql_fetch_array($result2,MYSQL_ASSOC);
    //... Davon die auswählen, bei denen durchschnittlich eine positive Rückmeldungen kam.
    $query3 = "SELECT AVG(pos) as posavg FROM cortex2 WHERE dest=" . $rels["suc"] ." AND relid IN (SELECT relid FROM cortex2 WHERE dest = " . $erg["relid"] . " AND suc= " . $rels["suc"] . ")"; 
    //debug ($query3);
    $result3 = mysql_query($query3,$db);
    if (mysql_num_rows($result3) > 0) {
      $relpos = mysql_fetch_array($result3,MYSQL_ASSOC);
      if ($relpos["posavg"] >= 0) {
        $foundrels[0] = $rels["suc"];
      }
    } else { //tritt auf, wenn es auf eine Bemerkung gar keine Rückmeldung gab
      $foundrels[0] = $rels["suc"];
    }
  } 
}







//Schleife Sentences zu ende:
}
//


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
      $saidsomething=1;
    }
  }
}

echo $output1 . ". ";


//Den Zusammenhang zwischen dem Gehörten und dem Gesagten Satz merken:

if ($foundrels[0] > 0) {
  $query1 = "INSERT INTO cortex2 (relid,dest,suc) values ($remember,$lastrelid,$foundrels[0])";
  $result = mysql_query($query1,$db);
  $relid = $foundrels[0]; 
  $lastrelid = $relid;
}

}} else { //wenn nichts eigegeben
  $remember = "";
  $relid = "";
}

?>
<form name=mouth action="index.php" method="get">

<input type=text name=inp>
<input type=hidden name=rem value="<?php echo $remember; ?>">
<input type=hidden name=lastrelid value="<?php echo $relid; ?>">
<input type=hidden name=sst value="<?php echo $saidsomething; ?>">
<input type=submit value=OK>

</form>
