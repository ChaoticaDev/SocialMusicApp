<?php

  include("db.php");



  $sel = mysqli_query($db, "SELECT uid,username from users where username = '".$_GET['username']."' OR uid = '".$_GET['UID']."' OR uid = '".$_GET['uid']."' limit 1");
  $user = mysqli_fetch_array ( $sel, MYSQL_ASSOC );

  if ( $user == NULL ){
    $error = array("error" => array("error_code"=>"0x001", "error_response"=>"User ID not found."));
    echo json_encode($error);
  }else{
    $user['tracks'] = mysqli_fetch_array(mysqli_query($db, "SELECT * FROM snippets WHERE uid = '".$user['uid']."'"));
    echo json_encode($user);
  }
?>
