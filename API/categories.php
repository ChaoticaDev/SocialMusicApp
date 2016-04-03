<?php
  include("db.php");

  $sel = mysqli_query($db, "SELECT * FROM categories");
  $arr["categories"] = array();
  while ($track=mysqli_fetch_array($sel, MYSQL_ASSOC)){
    array_push($arr["categories"], $track);
  }
  echo json_encode($arr);

?>
