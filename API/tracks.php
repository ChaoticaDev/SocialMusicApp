<?php
  $db = mysqli_connect("", "", "", "io_socialapp");

  $err = 0;
  $sel = NULL;
  if ( !isset($_GET['ID']) ){
    $uid = isset ($_GET['uid']) == true ? "and snippets.uid = '" . $_GET['uid'] . "'" : "";
    $genre = isset ($_GET['genre']) == true ? "and snippets.genre = '" . $_GET['genre'] . "'" : "";
    $sel = mysqli_query($db, "SELECT * FROM snippets WHERE `ID` > 0 " . $uid . " " . $genre);
    $arr["tracks"] = array();
    while ($track=mysqli_fetch_array($sel, MYSQL_ASSOC)){
      $track["likes"] = mysqli_num_rows(mysqli_query($db, "SELECT * FROM `likes` WHERE snippet_id = '".$track['ID']."'"));
      $track["views"] = mysqli_num_rows(mysqli_query($db, "SELECT * FROM `views` WHERE snippet_id = '".$track['ID']."'"));
      $track["user"] = mysqli_fetch_array(mysqli_query($db, "SELECT uid,username,avatar,cover,bio,location FROM `users` WHERE uid = '".$track['uid']."'"));
      array_push($arr["tracks"], $track);
    }
    echo json_encode($arr);
  }else{
    $sel = mysqli_query($db, "SELECT track_data.*, users.username FROM (SELECT s.*, count(likes.ID) as likes from (SELECT snippets.*, count(views.id) as views FROM snippets join views on views.uid = snippets.uid  ) as s join likes on likes.snippet_id = s.ID WHERE s.ID = '".$_GET['ID']."' group by s.id;) as track_data join users on users.uid = track_data.uid;");

    $track = mysqli_fetch_array ( $sel, MYSQL_ASSOC );
    echo json_encode($track);

    if ( $track == NULL ){
      $error = array("error" => array("error_code"=>"0x001", "error_response"=>"Snippet ID not found."));
      echo json_encode($error);
    }
  }
?>
