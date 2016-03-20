<?php
  $db = mysqli_connect("", "", "", "");



  $sel = mysqli_query($db, "SELECT uid,username from users where username = '".$_GET['username']."' AND password='".$_GET['password']."'");
  $q = mysqli_fetch_array( $sel, MYSQL_ASSOC );

  if ( mysqli_num_rows($sel) < 1 ){
    $arr = array("Error"=>"Invalid credentials", "__api_err"=>"0x001");
  }
  else{

    $uniq = uniqid() . uniqid() . uniqid();
    $arr = array("user"=>$q, "token"=>$uniq);

    mysqli_query($db, "INSERT INTO user_token (uid, token_string) VALUES ('".$q['uid']."', '".$uniq."')");
  }

//die("SELECT * from users where username = '".$_GET['username']."' AND password='".$_GET['password']."'");
  echo json_encode($arr);
?>
