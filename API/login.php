<?php
  $db = mysqli_connect("ubersnip.com", "psilowin1", "Psilo1992", "io_socialapp");



  $sel = mysqli_query($db, "SELECT uid,username,avatar,cover,bio,location from users where username = '".$_GET['username']."' AND password='".$_GET['password']."'");
  $q = mysqli_fetch_array( $sel, MYSQL_ASSOC );

  if ( mysqli_num_rows($sel) < 1 ){
    $sel = mysqli_query($db, "SELECT * from user_token where token_string='".$_GET['password']."'");
    $sel2 = mysqli_fetch_array ( $sel, MYSQL_ASSOC );

    $nrows = mysqli_num_rows($sel);
    if ( $nrows > 0 ){
      $arr = array();
      $arr["user"] = mysqli_fetch_array(mysqli_query($db, "SELECT uid,username,avatar,cover,bio,location from users where uid = '".$sel2['uid']."'"), MYSQL_ASSOC);
      $arr["token"] = $_GET["password"];
      //echo json_encode($sel);
    }else{
      $arr = array("Error"=>"Invalid credentials", "__api_err"=>"0x001");
    }
  }
  else{
    $uniq = uniqid() . uniqid() . uniqid();
    $arr = array("user"=>$q, "token"=>$uniq);

    mysqli_query($db, "INSERT INTO user_token (uid, token_string) VALUES ('".$q['uid']."', '".$uniq."')");
  }

//die("SELECT * from users where username = '".$_GET['username']."' AND password='".$_GET['password']."'");
  echo json_encode($arr);
?>
