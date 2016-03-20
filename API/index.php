<?php
  $db = mysqli_connect("", "", "", "");

  $err = 0;
  if ( isset ($_GET['ID']) ){
    $sel = mysqli_query($db, "SELECT s.*, count(likes.ID) as likes from (SELECT snippets.*, count(views.id) as views FROM snippets join views on views.uid = snippets.uid  ) as s join likes on likes.snippet_id = s.ID WHERE s.ID = '".$_GET['ID']."' group by s.id;");
    $track = mysqli_fetch_array ( $sel, MYSQL_ASSOC );

    if ( $track == NULL ){
      $error = array("error" => array("error_code"=>"0x001", "error_response"=>"Snippet ID not found."));
      echo json_encode($error);
    }else{
      echo json_encode($track);
    }
  }else{

  }
?>
