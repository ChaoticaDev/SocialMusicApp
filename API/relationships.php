<?php
  $db = mysqli_connect("", "", "", "");

$sel = mysqli_query($db, "SELECT users.uid, users.username, users.avatar, users.cover, users.bio FROM relations join users on users.uid = relations.follower_id and relations.uid ='".$_GET['user_id']."' group by users.uid");
  $arr["relations"] = array();
  while ($relation=mysqli_fetch_array($sel, MYSQL_ASSOC)){
    array_push($arr["relations"], $relation);
  }
  echo json_encode($arr);

?>
