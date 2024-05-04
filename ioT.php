<?php
ini_set('display_errors', 1); error_reporting(-1);
// doc du lieu tu website gui ve
include("config.php");

$pumper = $_POST["r"];

$sql = "UPDATE IoT
SET  pumper = $pumper 
WHERE stt = 1"; 

if(!mysqli_query($conn,$sql)){
  echo "Error: " ,mysql_error($conn);
}

// ngat ket noi voi database
mysqli_close($conn);
?>