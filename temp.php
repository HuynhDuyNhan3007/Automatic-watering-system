<?php
  header("refresh: 1");

  header("Content-Type: application/json");

  include("config.php");
  // doc data tu dttatabase
  $sql = "SELECT*FROM temp where stt>(SELECT max(stt) FROM temp)-5";
  $result = mysqli_query($conn,$sql);
  // convert thanh array
  $data = array();
  foreach($result as $row){
    $data[] = $row;
  }
  // encpde thanh dang json
  echo json_encode($data);

  mysqli_close($conn);

?>

