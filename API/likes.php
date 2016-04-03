<?php
  include("db.php");

  $err = 0;
  $sel = NULL;
  if ( isset($_GET['UID']) ){
    $query = mysqli_query($db, "SELECT * FROM (SELECT * FROM io_socialapp.likes WHERE likes.UID = '".$_GET['UID']."') as mylikes join snippets on snippets.ID = mylikes.snippet_id group by mylikes.snippet_id;");
    $arr["tracks"] = array();
    while ( $track = mysqli_fetch_array ( $query, MYSQL_ASSOC ) ){
      $track["likes"] = mysqli_num_rows(mysqli_query($db, "SELECT * FROM `likes` WHERE snippet_id = '".$track['ID']."'"));
      $track["views"] = mysqli_num_rows(mysqli_query($db, "SELECT * FROM `views` WHERE snippet_id = '".$track['ID']."'"));
      $track["user"] = mysqli_fetch_array(mysqli_query($db, "SELECT uid,username,avatar,cover,bio,location FROM `users` WHERE uid = '".$track['uid']."'"));
      array_push($arr["tracks"], $track);
    }
    
    echo json_encode($arr);
  }
?>
