<?php
  $db = mysqli_connect("", "", "", "io_socialapp");



  $sel = mysqli_query($db, "SELECT * from users where username = '".$_GET['username']."' AND password='".$_GET['password']."'");
  $q = mysqli_fetch_array( $sel, MYSQL_ASSOC );

  if ( mysqli_num_rows($sel) < 1 ){
    $arr = array("Error"=>"Invalid credentials", "__api_err"=>"0x001");
  }    else{
    $arr = array("Success"=>"Success: Logged in as " . $_GET['username']);
  }

//die("SELECT * from users where username = '".$_GET['username']."' AND password='".$_GET['password']."'");
  echo json_encode($arr);
?>
